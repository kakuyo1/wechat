#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifygRPCClient.h"
#include "RedisManager.h"
#include "MysqlManager.h"

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
        std::cout << "Received POST body: " << body_str << std::endl;// DEBUG: Print the body content
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
            response["error"] = static_cast<int>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Bad Request: Verify code does not match"; // wrong verify code from client
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }

        // check if the user already exists by mysql
        int uid = MysqlManager::GetInstance()->RegisterUser(user, email, encrpted_password);
        if (uid == -3 || uid == -4 || uid == -5) {
            std::cerr << "Failed to register user: " << uid << std::endl;
            std::cerr << "con is nullptr || stored procedure did't execute sucessfully || sqlException happened" << std::endl;
            response["error"] = static_cast<int>(ErrorCodes::ERROR_MYSQL);
            response["message"] = "Failed to register user in MySQL";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        if (uid == -1) {
            std::cerr << "Failed to register user: " << uid << std::endl;
            std::cerr << "User already exists" << std::endl;
            response["error"] = static_cast<int>(ErrorCodes::ERROR_EXISTING_USER);
            response["message"] = "User already exists";
            beast::ostream(con->_response.body()) << response.toStyledString();
            return;
        }
        if (uid == -2) {
            std::cerr << "Failed to register user: " << uid << std::endl;
            std::cerr << "Email already exists" << std::endl;
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
        std::cout << "register user successed" << std::endl; //!debug
        return;
    });
}

bool LogicSystem::HandleGet(const std::string &target_route, std::shared_ptr<HttpConnection> connection)
{
    if (_get_handlers.find(target_route) == _get_handlers.end()) {
        std::cerr << "No GET handler found for route: " << target_route << std::endl;
        return false; // No handler found for this route
    }
    _get_handlers[target_route](connection);
    return true;
}

bool LogicSystem::HandlePost(const std::string &target_route, std::shared_ptr<HttpConnection> connection)
{
    if (_post_handlers.find(target_route) == _post_handlers.end()) {
        std::cerr << "No POST handler found for route: " << target_route << std::endl;
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


