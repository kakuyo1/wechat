#include "CServer.h"
#include "HttpConnection.h"
CServer::CServer(net::io_context& ioc, unsigned short port) :
    _ioc(ioc),
    _acceptor(ioc, tcp::endpoint(tcp::v4(), port)),
    _socket(ioc)
{

}

CServer::Start() {
    auto self = shared_from_this();
    _acceptor.async_accept(_socket, [self](boost::system::error_code ec){
        try{
            if (ec) {
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
                self->Start(); // Restart accepting
                return;
            }
            // Handle the accepted socket here, submit to HttpConnection
            std::shared_ptr<HttpConnection> connection = std::make_shared<HttpConnection>(std::move(self->_socket));
            connection->Start(); // same name as HttpConnection::Start

            //continue accepting new connections
            self->Start();
        } catch (const std::exception& e) {
            std::cerr << "Exception in CServer::Start: " << e.what() << std::endl;
        }
    });
}