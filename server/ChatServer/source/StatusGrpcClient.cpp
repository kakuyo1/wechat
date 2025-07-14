#include "StatusGrpcClient.h"

StatusStubPool::StatusStubPool(size_t pool_size, const std::string& RPCserver_address, const std::string& RPCserver_port):
    _closed(false),
    _RPCserver_address(RPCserver_address),
    _RPCserver_port(RPCserver_port),
    _pool_size(pool_size)
{
    // Initialize the gRPC stubs
    for (size_t i = 0; i < pool_size; ++i) {
        auto channel = grpc::CreateChannel(RPCserver_address + ':' + RPCserver_port, grpc::InsecureChannelCredentials());
        _stubs.push(StatusService::NewStub(channel));
    }
}

StatusStubPool::~StatusStubPool() {
    Close();
    std::lock_guard<std::mutex> lock(_mutex);
    while (!_stubs.empty()) {
        _stubs.pop(); // Clear the pool
    }
}

void StatusStubPool::Close() {
    if (_closed) {
        return;
    }
    _closed = true;
    _cond_var.notify_all();
}

std::unique_ptr<StatusService::Stub> StatusStubPool::GetStub() {
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

void StatusStubPool::ReturnStub(std::unique_ptr<StatusService::Stub> stub) {
    std::lock_guard<std::mutex> lock(_mutex);
    _stubs.push(std::move(stub));
    _cond_var.notify_one();
}

size_t StatusStubPool::GetPoolSize() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _stubs.size();
}

GetChatServerResponse StatusGrpcClient::GetChatServer(int uid)
{
    GetChatServerRequest request;
    request.set_uid(uid);
    GetChatServerResponse response;

    auto stub = _pool->GetStub();
    if (!stub) {
        spdlog::error("Failed to get stub from pool");
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return response; // Return empty response on failure
    }

    Defer defer([this, &stub](){
        _pool->ReturnStub(std::move(stub));
    });

    ClientContext context;
    Status status = stub->GetChatServer(&context, request, &response);

    if (!status.ok()) {
        spdlog::error("gRPC call failed: {}", status.error_message());
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        return response; // Return empty response on failure
    }

    return response;
}

LoginResponse StatusGrpcClient::Login(int uid, const std::string &token)
{
    LoginRequest request;
    request.set_uid(uid);
    request.set_token(token);

    ClientContext context;
    LoginResponse response;

    auto stub = _pool->GetStub();

    Defer defer([this, &stub]()
                { _pool->ReturnStub(std::move(stub)); });

    if (!stub)
    {
        spdlog::error("Failed to get gRPC stub from pool");
        response.set_error(static_cast<int32_t>(ErrorCodes::ERROR_RPC));
        return response;
    }

    Status status = stub->Login(&context, request, &response);
    if (!status.ok())
    {
        spdlog::error("gRPC call failed: {}", status.error_message());
        return response; // Return empty response on failure(invalid token or uid)
    }
    return response; // Return the response on success
}

StatusGrpcClient::StatusGrpcClient()
{
    auto& config = ConfigIniManager::Instance();
    auto& section = config["StatusServer"];

    const auto& host = section["Host"];
    const auto& port = section["Port"];

    _pool = std::make_unique<StatusStubPool>(5, host, port);
}
