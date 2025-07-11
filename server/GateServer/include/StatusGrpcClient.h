#include "config.h"
#include "Singleton.h"
#include "ConfigIniManager.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include "message.grpc.pb.h"
#include <spdlog/spdlog.h>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::StatusService;
using message::GetChatServerRequest;
using message::GetChatServerResponse;

class StatusStubPool {
public:
    StatusStubPool(size_t pool_size, const std::string& RPCserver_address, const std::string& RPCserver_port);
    ~StatusStubPool();
    void Close();
    std::unique_ptr<StatusService::Stub> GetStub();
    void ReturnStub(std::unique_ptr<StatusService::Stub> stub);
    size_t GetPoolSize();
private:
    std::queue<std::unique_ptr<StatusService::Stub>> _stubs;
    std::mutex _mutex;
    std::condition_variable _cond_var;
    std::atomic<bool> _closed;
    std::string _RPCserver_address;
    std::string _RPCserver_port;
    size_t _pool_size;
};

class StatusGrpcClient : public Singleton<StatusGrpcClient> {
    friend class Singleton<StatusGrpcClient>; // Allow Singleton to create StatusGrpcClient instance
public:
    ~StatusGrpcClient() = default;
    StatusGrpcClient(const StatusGrpcClient&) = delete; // Disable copy constructor
    StatusGrpcClient& operator=(const StatusGrpcClient&) = delete; // Disable copy assignment
    StatusGrpcClient(StatusGrpcClient&&) = delete; // Disable move constructor
    StatusGrpcClient& operator=(StatusGrpcClient&&) = delete; // Disable move assignment
    GetChatServerResponse GetChatServer(int uid);
private:
    StatusGrpcClient();
    std::unique_ptr<StatusStubPool> _pool;
};
