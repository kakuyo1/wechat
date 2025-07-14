#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifygRPCClient.h"
#include "RedisManager.h"
#include "MysqlManager.h"
#include "StatusGrpcClient.h"

LogicSystem::LogicSystem()
{
    // register get handler
    RegisterGetHandler("/get_test", [](std::shared_ptr<HttpConnection> con){
        beast::ostream(con->_response.body()) << R"({"message": "GET request successful"})" + std::string("\n");
        // write the query parameters
        int i = 0;
        for (const auto& KV : con->_query_params) {
            i++;
            beast::ostream(con->_response.body()) << "Query Param " << i << ": " << KV.first << " = " << KV.second << "\n";
        }
    });
    // get verifycode handler
    RegisterPostHandler("/get_verifycode", [](std::shared_ptr<HttpConnection> con){
        auto body_str = beast::buffers_to_string(con->_request.body().data());
        if (body_str.empty()) {
            beast::ostream(con->_response.body()) << "Bad Request: Empty body";
            return;
        }
        spdlog::info("Received body: {}", body_str); // Log the received body
        con->_response.set(http::field::content_type, "application/json");
        // Parse the JSON body
        Json::Value source;
        Json::Reader reader;
        Json::Value response;
        if (!reader.parse(body_str, source)) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Invalid JSON format";
            std::string error_str = response.toStyledString();
            beast::ostream(con->_response.body()) << error_str;
            return;
        }
        // Check if the required fields are present
        if (!source.isMember("email")) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Missing 'email' field";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // send to verifyService to get verify code
        std::string email = source["email"].asString();
        GetVerifyResponse verify_response = VerifygRPCClient::GetInstance()->GetVerifyCode(email);
        if (verify_response.error() != static_cast<int>(ErrorCodes::SUCCESS)) {
            response["error"] = verify_response.error();
            response["message"] = "Failed to get verify code";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // If successful, return the verify code
        response["email"] = verify_response.email();
        response["error"] = static_cast<int>(verify_response.error());
        response["code"] = verify_response.code();
        response["message"] = "Verify code retrieved successfully";
        std::string response_str = response.toStyledString();
        beast::ostream(con->_response.body()) << response_str;
        return;
    });

    // user register handler
    RegisterPostHandler("/user_register", [](std::shared_ptr<HttpConnection> con){
        auto body_str = beast::buffers_to_string(con->_request.body().data());
        if (body_str.empty()) {
            beast::ostream(con->_response.body()) << "Bad Request: Empty body";
            return;
        }
        con->_response.set(http::field::content_type, "application/json");
        // Parse the JSON body
        Json::Value source;
        Json::Reader reader;
        Json::Value response;
        if (!reader.parse(body_str, source)) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Invalid JSON format";
            std::string error_str = response.toStyledString();
            beast::ostream(con->_response.body()) << error_str;
            return;
        }
        // Check if the required fields are present
        if (!source.isMember("user") || !source.isMember("email") || !source.isMember("confirm")
                || !source.isMember("password") || !source.isMember("verifycode")) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Missing required fields";
            std::string error_str = response.toStyledString();
            beast::ostream(con->_response.body()) << error_str;
            return;
        }
        // Extract the fields
        std::string user = source["user"].asString();
        std::string email = source["email"].asString();
        std::string confirm = source["confirm"].asString();
        std::string raw_password = source["password"].asString(); // need to be encrypted
        std::string verifycode = source["verifycode"].asString();

        // encrypt the password using bcrypt
        // generate a salt, BCRYPT_HASHSIZE is 64 representing the length of the salt and hash
        char salt[BCRYPT_HASHSIZE]; // for storing the salt
        char hash[BCRYPT_HASHSIZE]; // for storing the hashed password
        if (bcrypt_gensalt(12, salt) != 0 || bcrypt_hashpw(raw_password.c_str(), salt, hash) != 0) { // 12 is the work factor, the higher the more secure, the longer it takes to hash
            response["error"] = static_cast<int>(ErrorCodes::ERROR_ENCRPTION);
            response["message"] = "Bad Request: Failed to hash password";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        std::string encrpted_password(hash);

        // check if the verifycode still cached in redis
        auto redis_manager = RedisManager::GetInstance();
        std::string cached_code;
        std::string key = code_prefix + email;
        if (!redis_manager->Get(key, cached_code)) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to retrieve verify code from Redis(expired or not found)";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // compare the verify code
        if (cached_code != verifycode) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_INVALID_VERIFY_CODE);
            response["message"] = "Bad Request: Verify code does not match"; // wrong verify code from client
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }

        // check if the user already exists by mysql
        int uid = MysqlManager::GetInstance()->RegisterUser(user, email, encrpted_password);
        if (uid == -3 || uid == -4 || uid == -5) {
            spdlog::error("Failed to register user: {}", uid);
            spdlog::error("con is nullptr || stored procedure did't execute sucessfully || sqlException happened");
            response["error"] = static_cast<int>(ErrorCodes::ERROR_MYSQL);
            response["message"] = "Failed to register user in MySQL";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        if (uid == -1) {
            spdlog::error("Failed to register user: {}", uid);
            spdlog::error("User already exists");
            response["error"] = static_cast<int>(ErrorCodes::ERROR_EXISTING_USER);
            response["message"] = "User already exists";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        if (uid == -2) {
            spdlog::error("Failed to register user: {}", uid);
            spdlog::error("Email already exists");
            response["error"] = static_cast<int>(ErrorCodes::ERROR_EXISTING_EMAIL);
            response["message"] = "Email already exists";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // all checks passed, registerrd the user successfully

        // clear the verify code in redis if the user is registered successfully
        if (!redis_manager->Delete(key)) {
            response["error"] = static_cast<int>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to delete verify code from Redis";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // If successful, return success message
        response["error"] = static_cast<int>(ErrorCodes::SUCCESS);
        response["message"] = "User registered successfully";
        std::string response_str = response.toStyledString();
        beast::ostream(con->_response.body()) << response_str;
        spdlog::info("User {} registered successfully with email {}", user, email);
        return;
    });

    // reset password handler
    RegisterPostHandler("/reset_password", [](std::shared_ptr<HttpConnection> con){
        auto body_str = beast::buffers_to_string(con->_request.body().data());
        if (body_str.empty()) {
            beast::ostream(con->_response.body()) << "Bad Request: Empty body";
            return;
        }
        con->_response.set(http::field::content_type, "application/json");
        // Parse the JSON body
        Json::Value source;
        Json::Reader reader;
        Json::Value response;
        if (!reader.parse(body_str, source)) {
            spdlog::error("Failed to parse JSON: {}", body_str);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Invalid JSON format";
            std::string error_str = response.toStyledString();
            beast::ostream(con->_response.body()) << error_str;
            return;
        }
        // Check if the required fields are present
        if (!source.isMember("email") || !source.isMember("verifycode") ||
            !source.isMember("newpassword") || !source.isMember("user")) {
            spdlog::error("Missing required fields in JSON: {}", body_str);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Missing required fields";
            std::string error_str = response.toStyledString();
            beast::ostream(con->_response.body()) << error_str;
            return;
        }
        // Extract the fields
        std::string email = source["email"].asString();
        std::string verifycode = source["verifycode"].asString();
        std::string new_password = source["newpassword"].asString(); // need to be encrypted
        std::string user = source["user"].asString();
        // encrypt the password using bcrypt
        // generate a salt, BCRYPT_HASHSIZE is 64 representing the length of the salt and hash
        char salt[BCRYPT_HASHSIZE]; // for storing the salt
        char hash[BCRYPT_HASHSIZE]; // for storing the hashed password
        if (bcrypt_gensalt(12, salt) != 0 || bcrypt_hashpw(new_password.c_str(), salt, hash) != 0) { // 12 is the work factor, the higher the more secure, the longer it takes to hash
            spdlog::error("Failed to hash password for user: {}", user);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_ENCRPTION);
            response["message"] = "Failed to encrypt password";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        std::string encrypted_password(hash);
        // check if the verifycode still cached in redis
        auto redis_manager = RedisManager::GetInstance();
        std::string cached_code;
        std::string key = code_prefix + email;
        if (!redis_manager->Get(key, cached_code)) {
            spdlog::error("Failed to retrieve verify code from Redis for email: {}", email);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to retrieve verify code from Redis(expired or not found)";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // compare the verify code
        if (cached_code != verifycode) {
            spdlog::error("Verify code does not match for email: {}", email);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_INVALID_VERIFY_CODE);
            response["message"] = "Bad Request: Verify code does not match"; // wrong verify code from client
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // check if the user and email match by mysql
        if (!MysqlManager::GetInstance()->CheckEmailAndUserMatch(user, email)) {
            spdlog::error("User and email do not match for user: {} and email: {}", user, email);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_USER_EMAIL_MISMATCH);
            response["message"] = "Bad Request: User and email do not match";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // reset the password
        if (!MysqlManager::GetInstance()->ResetPassword(user, encrypted_password)) {
            spdlog::error("Failed to reset password for user: {}", user);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_MYSQL);
            response["message"] = "Failed to reset password in MySQL";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }

        // clear the verify code in redis if the user is reset successfully
        if (!redis_manager->Delete(key)) {
            spdlog::error("Failed to delete verify code from Redis for email: {}", email);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to delete verify code from Redis";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // If successful, return success message
        response["error"] = static_cast<int>(ErrorCodes::SUCCESS);
        response["message"] = "Password reset successfully";
        std::string response_str = response.toStyledString();
        beast::ostream(con->_response.body()) << response_str;
        spdlog::info("User {} reset password successfully with email {}", user, email);
        return;
    });

    // login handler
    RegisterPostHandler("/user_login", [](std::shared_ptr<HttpConnection> con){
        auto body_str = beast::buffers_to_string(con->_request.body().data());
        if (body_str.empty()) {
            spdlog::warn("Bad Request: Empty body");
            beast::ostream(con->_response.body()) << "Bad Request: Empty body";
            return;
        }
        Json::Value source;
        Json::Value response;
        Json::Reader reader;
        if (!reader.parse(body_str, source)) {
            spdlog::warn("Failed to parse JSON: {}", body_str);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Invalid JSON format";
            std::string err_string = response.toStyledString();
            beast::ostream(con->_response.body()) << err_string;
            return;
        }
        // check the required fields
        if (!source.isMember("email") || !source.isMember("password")) {
            spdlog::warn("Bad Request: Missing fields");
            response["error"] = static_cast<int>(ErrorCodes::ERROR_MISSING_FIELDS);
            response["message"] = "Bad Request: Missing fields";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }

        std::string email = source["email"].asString();
        std::string password = source["password"].asString();
        // check if email exists
        if (!MysqlManager::GetInstance()->CheckEmailExists(email)) {
            spdlog::warn("Email does not exist: {}", email);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_EMAIL_DOES_NOT_EXIST);
            response["message"] = "Bad Request: Email does not exist";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // check if the email and password match by mysql
        UserInfo user_info{};
        if (!MysqlManager::GetInstance()->CheckEmailAndPasswordMatch(email, password, user_info)) {
            spdlog::warn("Email and password do not match for email: {}", email);
            response["error"] = static_cast<int>(ErrorCodes::ERROR_PASSWORD_EMAIL_MISMATCH);
            response["message"] = "Bad Request: Email and password do not match";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // call for StatusServer to get the ChatServer address which is the idlest
        auto reply = StatusGrpcClient::GetInstance()->GetChatServer(user_info.uid);
        if (reply.error() != static_cast<int>(ErrorCodes::SUCCESS)) {
            spdlog::warn("Failed to get ChatServer address");
            response["error"] = static_cast<int>(ErrorCodes::ERROR_RPC);
            response["message"] = "Failed to get ChatServer address";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        // If successful, return the ChatServer address
        response["error"] = reply.error();
        response["message"] = "Login successful";
        response["host"] = reply.host();
        response["port"] = reply.port();
        response["uid"] = user_info.uid;
        response["token"] = reply.token();
        std::string response_str = response.toStyledString();
        beast::ostream(con->_response.body()) << response_str;
        spdlog::info("User {} logged in successfully, redirecting to ChatServer at {}:{}", email, reply.host(), reply.port());
        return;
    });
}

bool LogicSystem::HandleGet(const std::string &target_route, std::shared_ptr<HttpConnection> connection)
{
    if (_get_handlers.find(target_route) == _get_handlers.end()) {
        spdlog::error("No GET handler found for route: {}", target_route);
        return false; // No handler found for this route
    }
    _get_handlers[target_route](connection);
    return true;
}

bool LogicSystem::HandlePost(const std::string &target_route, std::shared_ptr<HttpConnection> connection)
{
    if (_post_handlers.find(target_route) == _post_handlers.end()) {
        spdlog::error("No POST handler found for route: {}", target_route);
        return false; // No handler found for this route
    }
    _post_handlers[target_route](connection);
    return true;
}

void LogicSystem::RegisterGetHandler(const std::string &route, HttpHandler handler)
{
    _get_handlers[route] = std::move(handler);
}

void LogicSystem::RegisterPostHandler(const std::string &route, HttpHandler handler)
{
    _post_handlers[route] = std::move(handler);
}


