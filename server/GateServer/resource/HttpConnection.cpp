#include "HttpConnection.h"

HttpConnection::HttpConnection(tcp::socket socket) :
    _socket(std::move(socket)),
    _deadline(_socket.get_executor(), std::chrono::seconds(60))
{

}

void HttpConnection::Start() {
    auto self = shared_from_this();
    self->CheckDeadline();// start deadline check for 60 seconds
    http::async_read(_socket, _buffer, _request, [self](beast::error_code ec, std::size_t bytes_transferred){
        if (ec) {
            std::cerr << "Error reading request: " << ec.message() << std::endl;
            return;
        }
        boost::ignore_unused(bytes_transferred);
        // Handle the request
        self->HandleRequest();
    });
}

void HttpConnection::HandleRequest() {
    _response.version(_request.version());
    _response.keep_alive(false); // short connection

    // for get method
    if (_request.method() == http::verb::get) {
        bool success = LogicSystem::GetInstance().HandleGet(_request.target(), shared_from_this());
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            _response.set(http::field::server, "GateServer");
            _response.body() = "Not Found";
            SendResponse();
            return;
        }
        _response.result(http::status::ok);
        _response.set(http::field::content_type, "application/json");
        _response.set(http::field::server, "GateServer");
        SendResponse();
        return;
    }
    // for post method

    // for other methods
    _response.result(http::status::method_not_allowed);
    _response.set(http::field::content_type, "text/plain");
    _response.set(http::field::server, "GateServer");
    _response.body() = "Method Not Allowed";
    SendResponse();
    return;
}

void HttpConnection::SendResponse() {
    auto self = shared_from_this();
    http::async_write(_socket, _response, [self](beast::error_code ec, std::size_t bytes_transferred){
        if (ec) {
            std::cerr << "Error sending response: " << ec.message() << std::endl;
            return;
        }
        boost::ignore_unused(bytes_transferred);
        // Close the socket after sending the response
        beast::error_code close_ec;
        self->_socket.shutdown(tcp::socket::shutdown_send, close_ec);
        if (close_ec && close_ec != beast::errc::not_connected) {
            std::cerr << "Error shutting down socket: " << close_ec.message() << std::endl;
        }
        self->_socket.close(close_ec);
        if (close_ec) {
            std::cerr << "Error closing socket: " << close_ec.message() << std::endl;
        }
        self->_deadline.cancel();
    });
}

void HttpConnection::CheckDeadline() {
    auto self = shared_from_this();
    _deadline.async_wait([self](beast::error_code ec){
        if (ec) {
            std::cerr << "Deadline check error: " << ec.message() << std::endl;
            return;
        }
        // If deadline is reached, close the socket
        if (self->_deadline.expiry() <= std::chrono::steady_clock::now()) {
            beast::error_code ec;
            self->_socket.shutdown(tcp::socket::shutdown_both, ec);
            if (ec && ec != beast::errc::not_connected) {
                std::cerr << "Error shutting down socket: " << ec.message() << std::endl;
            }
            self->_socket.close(ec);
            if (ec) {
                std::cerr << "Error closing socket: " << ec.message() << std::endl;
            }
            return;
        }
    });
    // short connection no need for another check
}

