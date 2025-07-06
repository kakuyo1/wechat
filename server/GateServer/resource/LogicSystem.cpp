#include "LogicSystem.h"
#include "HttpConnection.h"

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


