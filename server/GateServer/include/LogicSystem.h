#pragma once
#include "singleton.h"
#include "config.h"
#include <map>
#include <functional>

class HttpConnection;
using HttpHandler = std::functional<void(std::shared_ptr<HttpConnection>)>;
class LogicSystem : public Singleton<LogicSystem>
{
public:
    ~LogicSystem() = default;
    bool HandleGet(const std::string& target_route, std::shared_ptr<HttpConnection> connection);
    bool HandlePost(const std::string& target_route, std::shared_ptr<HttpConnection> connection);
    void RegisterGetHandler(const std::string& route, HttpHandler handler);
    void RegisterPostHandler(const std::string& route, HttpHandler handler);
private:
    LogicSystem();
    LogicSystem(const LogicSystem&) = delete;
    LogicSystem& operator=(const LogicSystem&) = delete;
    std::map<std::string, HttpHandler> _get_handlers; // Map of GET request handlers
    std::map<std::string, HttpHandler> _post_handlers; // Map of POST
};