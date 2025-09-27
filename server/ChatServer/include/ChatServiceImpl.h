#pragma once
#include "../proto/message.grpc.pb.h"
#include "../proto/message.pb.h"
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "config.h"
#include "RedisManager.h"
#include "MysqlManager.h"
#include "spdlog/spdlog.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include "UserManager.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using chat_message::AddFriendRequest;
using chat_message::AddFriendResponse;
using chat_message::AuthFriendRequest;
using chat_message::AuthFriendResponse;
using chat_message::ChatService;
using chat_message::TextChatTransRequest;
using chat_message::TextChatTransResponse;

class ChatServiceImpl final : public ChatService::Service {
public:
    ChatServiceImpl() = default;
    ~ChatServiceImpl() override = default;
    Status NotifyAddFriend(::grpc::ServerContext* context, const AddFriendRequest* request, AddFriendResponse* response) override;
    Status NotifyAuthFriend(::grpc::ServerContext* context, const AuthFriendRequest* request, AuthFriendResponse* response) override;
    Status NotifyTextChatTrans(::grpc::ServerContext* context, const chat_message::TextChatTransRequest* request, chat_message::TextChatTransResponse* response) override;
};
