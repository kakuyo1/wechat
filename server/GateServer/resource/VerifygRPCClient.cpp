#include "VerifygRPCClient.h"
#include "RedisManager.h"
#include <spdlog/spdlog.h>
GetVerifyResponse VerifygRPCClient::GetVerifyCode(std::string email)
{
    ClientContext context; // enable client-side context for RPC calls
    GetVerifyRequest request;
    GetVerifyResponse response;
    request.set_email(email); // Set the email in the request
    // Get a stub from the pool
    auto stub = _stub_pool->GetStub();
    spdlog::info("stub obtained from pool, current pool size: {}", _stub_pool->GetPoolSize());
    if (!stub) {
        spdlog::error("Failed to get a stub from the pool.");
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        response.set_code("");
        return response;
    }
    // Call the GetVerifyCode method on the stub
    Status status = stub->GetVerifyCode(&context, request, &response);
    if (!status.ok()) {
        spdlog::error("gRPC call failed: {}", status.error_message());
        // 只有在网络层失败时本地填 error
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        response.set_code(""); // 默认设置为无效验证码
    }
    _stub_pool->ReturnStub(std::move(stub));
    spdlog::info("stub returned to pool, current pool size: {}", _stub_pool->GetPoolSize());
    return response; // fields filled by the server
}

VerifygRPCClient::VerifygRPCClient()
{
    // get host and port from config.ini
    auto& config = ConfigIniManager::Instance();
    auto& verify_server_section = config["VerifyServer"];
    std::string host = verify_server_section["Host"];
    std::string port = verify_server_section["Port"];
    if (host.empty() || port.empty()) {
        spdlog::error("Verify server configuration is missing or incomplete in config.ini.");
        throw std::runtime_error("Verify server configuration error.");
    }

    // initialize the RPC stub pool with a size of 4 and default server address and port
    _stub_pool = std::make_unique<RPCStubPool>(5, host, port); // Create a stub pool
    if (!_stub_pool) {
        spdlog::error("Failed to create RPCStubPool.");
        throw std::runtime_error("Failed to create RPCStubPool.");
    }
    spdlog::info("VerifygRPCClient initialized with stub pool size: {}", _stub_pool->GetPoolSize());
}

RPCStubPool::RPCStubPool(size_t pool_size, std::string RPCserver_address, std::string RPCserver_port) :
    _closed(false),
    _RPCserver_address("localhost"),
    _RPCserver_port("50051"),
    _pool_size(pool_size)
{
    // Initialize the gRPC stubs
    for (size_t i = 0; i < pool_size; ++i) {
        auto channel = grpc::CreateChannel(RPCserver_address + ':' + RPCserver_port, grpc::InsecureChannelCredentials());
        _stubs.push(VerifyService::NewStub(channel));
    }
}

RPCStubPool::~RPCStubPool()
{
    std::lock_guard<std::mutex> lock(_mutex);
    Close();
    while(!_stubs.empty()) {
        _stubs.pop(); // Clear the pool
    }
}

void RPCStubPool::Close()
{
    if (_closed) {
        return; // If already closed, do nothing
    }
    _closed = true; // Set the closed flag to true
    _cond_var.notify_all(); // Notify all waiting threads
}

std::unique_ptr<VerifyService::Stub> RPCStubPool::GetStub()
{
    std::unique_lock<std::mutex> lock(_mutex);
    // Wait until a stub is available or the pool is closed
    _cond_var.wait(lock, [this](){
        return !_stubs.empty() || _closed;
    });
    if (_closed) {
        return nullptr; // If the pool is closed, return nullptr, no need to serve
    }
    auto stub = std::move(_stubs.front());
    _stubs.pop();
    return stub;
}

void RPCStubPool::ReturnStub(std::unique_ptr<VerifyService::Stub> stub)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_closed) {
        return; // If the pool is closed, do not return the stub
    }
    _stubs.push(std::move(stub));
    _cond_var.notify_one(); // Notify one waiting thread that a stub is available
}

size_t RPCStubPool::GetPoolSize()
{
    return _stubs.size();
}
