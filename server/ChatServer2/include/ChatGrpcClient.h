#pragma once
#include "config.h"
#include "../proto/message.grpc.pb.h"
#include "../proto/message.pb.h"
#include <grpcpp/grpcpp.h>
#include "ConfigIniManager.h"
#include "Singleton.h"
#include <memory>
#include <string>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <iostream>
#include "RedisManager.h"
#include "MysqlManager.h"


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
class ChatStubPool {
public:
    ChatStubPool(size_t pool_size, const std::string& RPCserver_address, const std::string& RPCserver_port);
    ~ChatStubPool();
    void Close();
    std::unique_ptr<ChatService::Stub> GetStub();
    void ReturnStub(std::unique_ptr<ChatService::Stub> stub);
    size_t GetPoolSize() const;
private:
    std::queue<std::unique_ptr<ChatService::Stub>> _stubs;
    mutable std::mutex _mutex;
    std::condition_variable _cond_var;
    std::atomic<bool> _closed; // Flag to indicate if the pool is closed
    std::string _RPCserver_address;
    std::string _RPCserver_port;
    size_t _pool_size;
};

class ChatGrpcClient : public Singleton<ChatGrpcClient> {
    friend class Singleton<ChatGrpcClient>; // Allow Singleton to create ChatGrpcClient instance
public:
    ~ChatGrpcClient() = default;
    /* addFriend request, send to peer*/
    AddFriendResponse NotifyAddFriend(const std::string& peer_serverIP, const AddFriendRequest& request);
    /* authFriend request, send to peer*/
    AuthFriendResponse NotifyAuthFriend(const std::string& peer_serverIP, const AuthFriendRequest& request);
    /* text chat transfer request, send to peer*/
    TextChatTransResponse NotifyTextChatTrans(const std::string& peer_serverIP, const TextChatTransRequest& request);
private:
    ChatGrpcClient();
    /* serverIP(chat1, chat2...) : stubPool*/
    std::unordered_map<std::string, std::unique_ptr<ChatStubPool>> _stubPools;
};