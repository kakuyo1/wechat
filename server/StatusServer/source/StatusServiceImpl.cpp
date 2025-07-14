#include "StatusServiceImpl.h"
#include <spdlog/spdlog.h>
StatusServiceImpl::StatusServiceImpl()
{
    try
    {
        // get chatServerinfo by configIniMgr and fill the map
        auto &Config = ConfigIniManager::Instance();

        auto chatserver1 = Config["ChatServer1"];
        auto chatserver2 = Config["ChatServer2"];

        ChatServer chatserver_1{};
        ChatServer chatserver_2{};

        chatserver_1.host = chatserver1["Host"];
        chatserver_1.port = chatserver1["Port"];
        chatserver_1.name = chatserver1["Name"];
        chatserver_1.connection_count = 0;

        chatserver_2.host = chatserver2["Host"];
        chatserver_2.port = chatserver2["Port"];
        chatserver_2.name = chatserver2["Name"];
        chatserver_2.connection_count = 0;

        chat_servers_[chatserver_1.name] = chatserver_1;
        chat_servers_[chatserver_2.name] = chatserver_2;
    }
    catch (const std::exception &ex)
    {
        spdlog::error("Failed to initialize StatusServiceImpl: {}", ex.what());
    }
}

Status StatusServiceImpl::GetChatServer(ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response)
{
    try {
        ChatServer& chatserver = getChatServer();
        std::string token = generateTokenByUUID();
        response->set_token(std::move(token));
        response->set_host(chatserver.host);
        response->set_port(chatserver.port);
        response->set_error(static_cast<int>(ErrorCodes::SUCCESS));
        insertToken(request->uid(), response->token());
        spdlog::info("GetChatServer for user: uid: {}",
                        request->uid());
        return Status::OK;
    } catch (const std::exception& ex) {
        spdlog::error("GetChatServer failed: {}", ex.what());
        response->set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return Status::CANCELLED;
    }
}

Status StatusServiceImpl::Login(ServerContext *context, const message::LoginRequest *request, message::LoginResponse *response)
{
    try {
        auto uid = request->uid();
        auto token = request->token();
        auto it = _tokens.find(uid);
        if (uid <= 0 || token.empty()) {
            spdlog::error("Login failed: Invalid UID {}", uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_INVALID_AUTH_PARAMETERS));
            return Status::CANCELLED;
        }

        if (it == _tokens.end()) {
            spdlog::error("Login failed: UID {} not found", uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_UID_NOT_FOUND));
            return Status::CANCELLED;
        }

        if (it->second != token) {
            spdlog::error("Login failed: Token mismatch for UID {}", uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_TOKEN_MISMATCH));
            return Status::CANCELLED;
        }

        response->set_uid(uid);
        response->set_token(token);
        response->set_error(static_cast<int>(ErrorCodes::SUCCESS));
        spdlog::info("Login successful for UID: {}, Token: {}", uid, token);
        return Status::OK;
    } catch (const std::exception& ex) {
        spdlog::error("Login failed: {}", ex.what());
        response->set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return Status::CANCELLED;
    }
}

ChatServer &StatusServiceImpl::getChatServer()
{
    std::lock_guard<std::mutex> lk(_server_mtx);
    if (chat_servers_.empty()) {
        throw std::runtime_error("No available chat servers");
    }
    // iterate the chatserver map to find the chatserver which has the minimum connection_count
    auto idlest_iter = chat_servers_.begin();
    for (auto iter = chat_servers_.begin(); iter != chat_servers_.end(); ++iter) {
        if (iter->second.connection_count < idlest_iter->second.connection_count) {
            idlest_iter = iter;
        }
    }
    idlest_iter->second.connection_count++; //!UNKNOWN
    spdlog::info("Selected chat server: {} with host: {}, port: {}, connection count: {}",
                    idlest_iter->second.name, idlest_iter->second.host, idlest_iter->second.port, idlest_iter->second.connection_count);
    return idlest_iter->second;
}

std::string StatusServiceImpl::generateTokenByUUID()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string token = boost::uuids::to_string(uuid);
    return token;
}

void StatusServiceImpl::insertToken(int uid, const std::string &token)
{
    std::lock_guard<std::mutex> lk(_token_mtx);
    _tokens[uid] = token;
}
