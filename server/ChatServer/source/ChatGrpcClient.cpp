#include "../include/ChatGrpcClient.h"
ChatStubPool::ChatStubPool(size_t pool_size, const std::string& RPCserver_address, const std::string& RPCserver_port):
    _closed(false),
    _RPCserver_address(RPCserver_address),
    _RPCserver_port(RPCserver_port),
    _pool_size(pool_size)
{
    // Initialize the gRPC stubs
    spdlog::debug("[ChatGrpcClient1] Creating stubs for chatserver1 at {}:{}", RPCserver_address, RPCserver_port);
    for (size_t i = 0; i < pool_size; ++i) {
        auto channel = grpc::CreateChannel(RPCserver_address + ':' + RPCserver_port, grpc::InsecureChannelCredentials());
        _stubs.push(ChatService::NewStub(channel));
    }
}

ChatStubPool::~ChatStubPool() {
    Close();
    std::lock_guard<std::mutex> lock(_mutex);
    while (!_stubs.empty()) {
        _stubs.pop(); // Clear the pool
    }
}

void ChatStubPool::Close() {
    if (_closed) {
        return;
    }
    _closed = true;
    _cond_var.notify_all();
}

std::unique_ptr<ChatService::Stub> ChatStubPool::GetStub() {
    std::unique_lock<std::mutex> lock(_mutex);
    // Wait until a stub is available or the pool is closed
    _cond_var.wait(lock, [this]() {
        return !_stubs.empty() || _closed;
    });
    if (_closed) {
        return nullptr; // If the pool is closed, return nullptr
    }
    auto stub = std::move(_stubs.front());
    _stubs.pop();
    return stub;
}

void ChatStubPool::ReturnStub(std::unique_ptr<ChatService::Stub> stub) {
    std::lock_guard<std::mutex> lock(_mutex);
    _stubs.push(std::move(stub));
    _cond_var.notify_one();
}

size_t ChatStubPool::GetPoolSize() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _stubs.size();
}

AddFriendResponse ChatGrpcClient::NotifyAddFriend(const std::string &peer_serverName, const AddFriendRequest &request)
{
    AddFriendResponse response;
    Defer defer([&response, &request](){
        response.set_from_uid(request.from_uid());
        response.set_to_uid(request.to_uid());
    });
    auto it = _stubPools.find(peer_serverName);

    if (it == _stubPools.end()) {
        spdlog::error("No stub pool found for peer server name: {}", peer_serverName);
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return response; // Return empty response
    }

    auto& stubPool = it->second;
    std::unique_ptr<ChatService::Stub> stub = stubPool->GetStub();
    if (!stub) {
        spdlog::error("Failed to get stub from pool for peer server IP: {}", peer_serverName);
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return response; // Return empty response
    }
    // Use Defer to ensure the stub is returned to the pool
    Defer deferReturnStub([&stubPool, &stub]() {
        if (stub) {
            stubPool->ReturnStub(std::move(stub));
        }
    });

    grpc::ClientContext context;
    Status status = stub->NotifyAddFriend(&context, request, &response);
    spdlog::debug("[ChatGrpcClient] after NotifyAddFriend");
    if (!status.ok()) {
        spdlog::error("gRPC call failed: {}", status.error_message());
        return response;
    }
    // Successfully sent the add friend request
    spdlog::info("[ChatGrpcClient]Add friend request sent successfully from UID {} to UID {}", request.from_uid(), request.to_uid());
    return response;
}

AuthFriendResponse ChatGrpcClient::NotifyAuthFriend(const std::string &peer_serverIP, const AuthFriendRequest &request)
{
    AuthFriendResponse response;
    Defer defer([&response, &request](){
        response.set_from_uid(request.from_uid());
        response.set_to_uid(request.to_uid());
    });
    auto it = _stubPools.find(peer_serverIP);

    if (it == _stubPools.end()) {
        spdlog::error("No stub pool found for peer server IP: {}", peer_serverIP);
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return response; // Return empty response
    }

    auto& stubPool = it->second;
    std::unique_ptr<ChatService::Stub> stub = stubPool->GetStub();
    if (!stub) {
        spdlog::error("Failed to get stub from pool for peer server IP: {}", peer_serverIP);
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return response; // Return empty response
    }
    // Use Defer to ensure the stub is returned to the pool
    Defer deferReturnStub([&stubPool, &stub]() {
        if (stub) {
            stubPool->ReturnStub(std::move(stub));
        }
    });

    grpc::ClientContext context;
    Status status = stub->NotifyAuthFriend(&context, request, &response);
    if (!status.ok()) {
        spdlog::error("gRPC call failed: {}", status.error_message());
        return response;
    }
    return response;
}

ChatGrpcClient::ChatGrpcClient()
{
    auto& config = ConfigIniManager::Instance();
    auto server_list = config["PeerServer"]["Servers"];
    // get all the servers by split ','
    std::vector<std::string> servers;
    std::string server;
    std::stringstream ss(server_list);
    while (std::getline(ss, server, ',')) {
        servers.push_back(server);
    }
    // create stubPool for all servers
    for (const auto& single_server : servers) {
        if (config[single_server]["Name"].empty()) {
            continue;
        }
        _stubPools[single_server] = std::make_unique<ChatStubPool>(5,
            config[single_server]["Host"], config[single_server]["RPCPort"]);
        spdlog::info("[ChatGrpcClient1] Created stub pool for chatserver1: {} at {}:{}",
                    single_server, config[single_server]["Host"], config[single_server]["RPCPort"]);
    }
}
