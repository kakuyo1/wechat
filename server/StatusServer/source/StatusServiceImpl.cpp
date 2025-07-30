#include "../include/StatusServiceImpl.h"
#include <spdlog/spdlog.h>
#include "../include/RedisManager.h"
StatusServiceImpl::StatusServiceImpl()
{
    try
    {
        // get chatServerinfo by configIniMgr and fill the map
        auto &Config = ConfigIniManager::Instance();
        auto &chatServers = Config["chatservers"]["Name"];
        std::vector<std::string> server_names;
        std::stringstream ss(chatServers);
        std::string server_name;
        while (std::getline(ss, server_name, ',')) {
            server_names.push_back(server_name);
        }
        for (const auto &name : server_names) {
            auto host = Config[name]["Host"];
            auto port = Config[name]["Port"];
            if (host.empty() || port.empty()) {
                spdlog::error("Chat server {} is missing host or port configuration", name);
                continue;
            }
            ChatServer chat_server{host, port, name, 0};
            chat_servers_[name] = chat_server;
            spdlog::info("Chat server {} initialized with host: {}, port: {}", name, host, port);
        }
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


/* 其实这里的Login可以直接在ChatServer中实现，不需要RPC*/
Status StatusServiceImpl::Login(ServerContext *context, const chat_message::LoginRequest *request, chat_message::LoginResponse *response)
{
    try {
        spdlog::debug("Enter Login method with request: UID: {}, Token: {}",
                        request->uid(), request->token());
        auto uid = request->uid();
        auto origin_token = request->token();
        if (uid <= 0 || origin_token.empty()) {
            spdlog::error("Login failed: Invalid UID {}", uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_UID_NOT_FOUND));
            return Status::CANCELLED;
        }
        // get stored_token from Redis
        std::string token_key = USER_TOKEN_PREFIX + std::to_string(uid);
        std::string stored_token;
        if (!RedisManager::GetInstance()->Get(token_key, stored_token)) {
            spdlog::error("Failed to get token for UID: {}", uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_REDIS));
            return Status::CANCELLED;
        }
        // compare the token
        if (stored_token != origin_token) {
            spdlog::error("Login failed: Token mismatch for UID {}", uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_TOKEN_MISMATCH));
            return Status::CANCELLED;
        }

        response->set_uid(uid);
        response->set_token(origin_token);
        response->set_error(static_cast<int>(ErrorCodes::SUCCESS));
        spdlog::info("Login successful for UID: {}, Token: {}", uid, origin_token);
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

    // Find the chat server with the minimum connection_count by get LOGIN_COUNT from Redis
    ChatServer &min_chat_server = chat_servers_.begin()->second;
    std::string login_count_str = RedisManager::GetInstance()->HGet(SERVER_LOGIN_COUNT, min_chat_server.name);
    spdlog::debug("Login count for minserver {}: {}", min_chat_server.name, login_count_str); // !ChatServer2 has not been added to Redis yet, thus this place causes a warning
    if (!login_count_str.empty()) {
        min_chat_server.connection_count = std::stoi(login_count_str);
    } else {
        min_chat_server.connection_count = INT_MAX; // Default to INT_MAX if not found in Redis
    }

    // iterate the chatserver map to find the chatserver which has the minimum connection_count
    for (auto iter = chat_servers_.begin(); iter != chat_servers_.end(); ++iter) {
        // if self skip
        if (iter->second.name == min_chat_server.name) {
            continue;
        }
        // get the login count from Redis
        std::string login_count_str = RedisManager::GetInstance()->HGet(SERVER_LOGIN_COUNT, iter->second.name);
        spdlog::debug("Login count for iter server {}: {}", iter->second.name, login_count_str);
        if (!login_count_str.empty()) {
            iter->second.connection_count = std::stoi(login_count_str);
        } else {
            iter->second.connection_count = INT_MAX; // Default to INT_MAX if not found in Redis
        }

        if (iter->second.connection_count < min_chat_server.connection_count) {
            min_chat_server = iter->second;
        }
    }
    spdlog::info("Selected chat server: {} with host: {}, port: {}, connection count: {}",
                    min_chat_server.name, min_chat_server.host, min_chat_server.port, min_chat_server.connection_count);
    return min_chat_server;
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
    std::string token_key = USER_TOKEN_PREFIX + std::to_string(uid);
    RedisManager::GetInstance()->Set(token_key, token);
}
