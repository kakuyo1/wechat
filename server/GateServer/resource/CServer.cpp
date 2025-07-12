#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOContextPool.h"
CServer::CServer(net::io_context& ioc, unsigned short port) :
    _ioc(ioc),
    _acceptor(ioc, tcp::endpoint(tcp::v4(), port)),
    _socket(ioc)
{

}

void CServer::Start() {
    auto self = shared_from_this();
    if (!_acceptor.is_open()) {
        spdlog::error("Acceptor is not open, cannot accept connections.");
        return;
    }
    // create a new HttpConnection for each accept
    auto& new_ioc = AsioIOContextPool::GetInstance()->GetNextIOContext(); //监听（accept）和数据读写分离
    auto new_connection = std::make_shared<HttpConnection>(new_ioc);
    _acceptor.async_accept(new_connection->GetSocket(), [self, new_connection](boost::system::error_code ec){
        try{//告诉 acceptor 新的客户端连接对应的 socket(GetSocket()),当连接建立时，acceptor 会把客户端的连接关联到这个 socket。
            if (ec) {
                spdlog::error("Error accepting connection: {}", ec.message());
                self->Start(); // Restart accepting
                return;
            }
            // Handle the accepted socket here, submit to HttpConnection
            new_connection->Start();
            //continue accepting new connections
            self->Start();
        } catch (const std::exception& e) {
            spdlog::error("Exception in CServer::Start: {}", e.what());
            self->Start(); // Restart accepting
        }
    });
}