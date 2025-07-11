#pragma once
#include "Singleton.h"
#include "config.h"
#include <map>
#include <functional>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <bcrypt/bcrypt.h>
#include <spdlog/spdlog.h>

/***********************************
 *  @To offer response.body/() for HttpConnection
 *  @此类处理request的body部分
 *************************************/

class RedisManager;
class HttpConnection;
class VerifygRPCClient; // Forward declaration to avoid circular dependency

using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;
class LogicSystem : public Singleton<LogicSystem>
{
    friend class Singleton<LogicSystem>; // Allow Singleton to create LogicSystem instance
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