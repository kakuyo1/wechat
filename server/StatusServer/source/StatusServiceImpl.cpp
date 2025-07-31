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
            ChatServer chat_server{host, port, name};
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

    // 使用指针来跟踪最小负载服务器
    ChatServer* min_chat_server = nullptr;
    int min_count = INT_MAX;

    for (auto &pair : chat_servers_) {
        auto &server = pair.second;
        std::string login_count_str = RedisManager::GetInstance()->HGet(SERVER_LOGIN_COUNT, server.name);
        int login_count = 0;
        try {
            login_count = login_count_str.empty() ? 0 : std::stoi(login_count_str);
        } catch (const std::exception& ex) {
            spdlog::warn("Invalid login count for server {}: {}", server.name, ex.what());
            login_count = INT_MAX;  // 不考虑这个服务器
        }
        spdlog::debug("Checking server {} with count {}", server.name, login_count);

        if (login_count < min_count) {
            min_count = login_count;
            min_chat_server = &server;
        }
    }

    if (!min_chat_server) {
        throw std::runtime_error("No available chat servers");
    }
    spdlog::info("Selected chat server: {} with host: {}, port: {}",
                min_chat_server->name, min_chat_server->host,
                min_chat_server->port);

    return *min_chat_server;
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
