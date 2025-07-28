#include "ChatGrpcClient.h"
ChatStubPool::ChatStubPool(size_t pool_size, const std::string& RPCserver_address, const std::string& RPCserver_port):
    _closed(false),
    _RPCserver_address(RPCserver_address),
    _RPCserver_port(RPCserver_port),
    _pool_size(pool_size)
{
    // Initialize the gRPC stubs
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

AddFriendResponse ChatGrpcClient::NotifyAddFriend(const std::string &peer_serverIP, const AddFriendRequest &request)
{
    return AddFriendResponse();
}

AuthFriendResponse ChatGrpcClient::NotifyAuthFriend(const std::string &peer_serverIP, const AuthFriendRequest &request)
{
    return AuthFriendResponse();
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
            config[single_server]["Host"], config[single_server]["Port"]);
    }
}
