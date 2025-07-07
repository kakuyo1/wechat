#include "VerifygRPCClient.h"

GetVerifyResponse VerifygRPCClient::GetVerifyCode(std::string email)
{
    ClientContext context; // enable client-side context for RPC calls
    GetVerifyRequest request;
    GetVerifyResponse response;
    request.set_email(email); // Set the email in the request
    // Get a stub from the pool
    auto stub = _stub_pool->GetStub();
    std::cout << "Get a new stub" << std::endl; //! debug
    if (!stub) {
        std::cerr << "Failed to get a stub from the pool." << std::endl;
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        response.set_code("");
        return response;
    }
    // Call the GetVerifyCode method on the stub
    Status status = stub->GetVerifyCode(&context, request, &response);
    if (!status.ok()) {
        std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
        // 只有在网络层失败时本地填 error
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        response.set_code(""); // 默认设置为无效验证码
    }
    _stub_pool->ReturnStub(std::move(stub));
    std::cout << "Stub returned" << std::endl; // !debug
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
        std::cerr << "Verify server host or port is not configured." << std::endl;
        throw std::runtime_error("Verify server configuration error.");
    }

    // initialize the RPC stub pool with a size of 4 and default server address and port
    _stub_pool = std::make_unique<RPCStubPool>(5, host, port); // Create a stub pool
    std::cout << "stub pool size: " << _stub_pool->GetPoolSize() << std::endl; //! debug
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
    return _pool_size;
}
