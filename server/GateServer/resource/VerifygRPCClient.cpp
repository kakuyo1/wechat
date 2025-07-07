#include "VerifygRPCClient.h"

GetVerifyResponse VerifygRPCClient::GetVerifyCode(std::string email)
{
    ClientContext context; // enable client-side context for RPC calls
    GetVerifyRequest request;
    GetVerifyResponse response;
    request.set_email(email); // Set the email in the request
    // Call the GetVerifyCode method on the stub
    Status status = _stub->GetVerifyCode(&context, request, &response);
    if (!status.ok()) {
        std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
        // 只有在网络层失败时本地填 error
        response.set_error(static_cast<int>(ErrorCodes::ERROR_RPC));
        response.set_code(""); // 默认设置为无效验证码
    }
    return response; // fields filled by the server
}

VerifygRPCClient::VerifygRPCClient()
{
    // Create a gRPC channel to the server
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    _stub = VerifyService::NewStub(channel); // Create a stub for the service
}