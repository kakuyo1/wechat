#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifygRPCClient.h"

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
    // register post handler
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


