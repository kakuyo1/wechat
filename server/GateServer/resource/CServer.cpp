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
        std::cerr << "Acceptor is not open, cannot accept connections." << std::endl;
        return;
    }
    // create a new HttpConnection for each accept
    auto& new_ioc = AsioIOContextPool::GetInstance()->GetNextIOContext();
    auto new_connection = std::make_shared<HttpConnection>(new_ioc);
    std::cout << "io_context size: " << AsioIOContextPool::GetInstance()->GetPoolSize() << std::endl; //! Debug
    _acceptor.async_accept(new_connection->GetSocket(), [self, new_connection](boost::system::error_code ec){
        try{
            if (ec) {
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
                self->Start(); // Restart accepting
                return;
            }
            // Handle the accepted socket here, submit to HttpConnection
            new_connection->Start();
            //continue accepting new connections
            self->Start();
        } catch (const std::exception& e) {
            std::cerr << "Exception in CServer::Start: " << e.what() << std::endl;
        }
    });
}