#pragma once
#include "config.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include "message.grpc.pb.h"
#include "Singleton.h"
#include "iostream"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::VerifyService;
using message::GetVerifyRequest;
using message::GetVerifyResponse;

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
    std::unique_ptr<VerifyService::Stub> _stub;

private:
    VerifygRPCClient();
};
