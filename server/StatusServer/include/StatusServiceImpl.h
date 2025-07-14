#pragma once
#include "message.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <unordered_map>
#include <mutex>
#include "ConfigIniManager.h"
#include "config.h"
#include <spdlog/spdlog.h>

struct ChatServer {
    std::string host;
    std::string port;
    std::string name;
    int connection_count; // 连接数
};

using grpc::Status;
using grpc::ServerContext;

using message::StatusService;
using message::GetChatServerRequest;
using message::GetChatServerResponse;

class StatusServiceImpl final : public StatusService::Service {
public:
    StatusServiceImpl();
    Status GetChatServer(ServerContext* context, const GetChatServerRequest* request, GetChatServerResponse* response) override;
    Status Login(ServerContext* context, const message::LoginRequest* request, message::LoginResponse* response) override;
private:
    ChatServer& getChatServer();
    std::string generateTokenByUUID();
    void insertToken(int uid, const std::string& token);
private:
    std::unordered_map<std::string, ChatServer> chat_servers_;
    std::unordered_map<int, std::string> _tokens;
    std::mutex _server_mtx;
    std::mutex _token_mtx;
};