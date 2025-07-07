#pragma once
#include "config.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include "message.grpc.pb.h"
#include "Singleton.h"
#include "iostream"
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "ConfigIniManager.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::VerifyService;
using message::GetVerifyRequest;
using message::GetVerifyResponse;

/***************************************************************************
 * RPCStubPool: to manage the stubs
 * intial size as 5
****************************************************************************/

class RPCStubPool {
public:
    RPCStubPool(size_t pool_size, std::string RPCserver_address, std::string RPCserver_port);
    ~RPCStubPool();
    void Close();
    std::unique_ptr<VerifyService::Stub> GetStub();
    void ReturnStub(std::unique_ptr<VerifyService::Stub> stub);
    size_t GetPoolSize();
private:
    std::queue<std::unique_ptr<VerifyService::Stub>> _stubs;
    std::mutex _mutex;
    std::condition_variable _cond_var;
    std::atomic<bool> _closed; // Flag to indicate if the pool is closed
    std::string _RPCserver_address;
    std::string _RPCserver_port;
    size_t _pool_size;
};

/***************************************************************************
 * VerifygRPCClient: A singleton class to handle gRPC client requests for verification codes.
 * a fake GetVerifyCode function is provided to simulate the gRPC call.
 * the function by using RPC client stub to send a request(GetVerifyRequest)
 * to the server and receive a response(GetVerifyResponse).
 * the really GetVerifyCode function at the server side is implemented in VerifyService::Stub.
 * the Stub likes a messenger that sends the request to the server and receives the response by
 * traveling the channel.
 ****************************************************************************/

class VerifygRPCClient : public Singleton<VerifygRPCClient> {
    friend class Singleton<VerifygRPCClient>; // Allow Singleton to create VerifygRPCClient instance
public:
    ~VerifygRPCClient() = default;
    VerifygRPCClient(const VerifygRPCClient&) = delete; // Disable copy constructor
    VerifygRPCClient& operator=(const VerifygRPCClient&) = delete; // Disable copy assignment
    VerifygRPCClient(VerifygRPCClient&&) = delete; // Disable move constructor
    VerifygRPCClient& operator=(VerifygRPCClient&&) = delete; // Disable move assignment

    GetVerifyResponse GetVerifyCode(std::string email);
private:
    std::unique_ptr<RPCStubPool> _stub_pool; // Use RPCStubPool to manage stubs
private:
    VerifygRPCClient();
};
