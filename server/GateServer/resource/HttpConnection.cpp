#include "HttpConnection.h"
#include "LogicSystem.h"
HttpConnection::HttpConnection(net::io_context& ioc) :
    _socket(ioc),
    _deadline(_socket.get_executor(), std::chrono::seconds(60))
{

}

void HttpConnection::Start() {
    auto self = shared_from_this();
    self->CheckDeadline();// start deadline check for 60 seconds
    http::async_read(_socket, _buffer, _request, [self](beast::error_code ec, std::size_t bytes_transferred){ // raw _buffer data to construct _request
        if (ec) {
            std::cerr << "Error reading request: " << ec.message() << std::endl;
            return;
        }
        boost::ignore_unused(bytes_transferred);
        // Handle the request
        self->HandleRequest();
    });
}

void HttpConnection::PreParseUrlToGetParams()
{
    // use boost::urls to parse the URL and extract query parameters
    _query_params.clear();
    _target_route.clear();
    // _request.target() 形如 /get_test?q=boost&lang=en
    auto target_str = std::string(_request.target());
    // 解析相对路径和查询，parse_relative_ref 适合这个格式
    boost::system::result<boost::urls::url_view> result = boost::urls::parse_relative_ref(target_str);
    if (result.has_error()) {
        std::cerr << "Error parsing URL: " << result.error().message() << std::endl;
        return;
    }
    auto url = result.value();
    // get the encoded path as the target route
    _target_route = std::string(url.encoded_path());
    // get the query parameters
    for (const auto& param : url.params()) {
        _query_params.emplace(std::string(param.key), std::string(param.value));
    }
}

tcp::socket &HttpConnection::GetSocket()
{
    return _socket;
}

void HttpConnection::HandleRequest() {
    _response.version(_request.version());
    _response.keep_alive(false); // short connection
    _response.set(http::field::server, "GateServer");
    PreParseUrlToGetParams(); // parse the URL to get target route and query parameters
    // for get method
    if (_request.method() == http::verb::get) {
        bool success = LogicSystem::GetInstance()->HandleGet(_target_route, shared_from_this()); //LogicSystem to get the body
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "Not Found";
            SendResponse();
            return;
        }
        _response.result(http::status::ok);
        _response.set(http::field::content_type, "application/json");
        SendResponse();
        return;
    }
    // for post method
    if (_request.method() == http::verb::post) {
        bool success = LogicSystem::GetInstance()->HandlePost(_target_route, shared_from_this()); //LogicSystem to get the body
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "Not Found";
            SendResponse();
            return;
        }
        _response.result(http::status::ok);
        _response.set(http::field::content_type, "application/json");
        SendResponse();
        return;
    }

    // for other methods
    _response.result(http::status::method_not_allowed);
    _response.set(http::field::content_type, "text/plain");
    _response.set(http::field::server, "GateServer");
    beast::ostream(_response.body()) << "Method Not Allowed";
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
        self->_deadline.cancel(); // cause Deadline check error
    });
}

void HttpConnection::CheckDeadline() {
    auto self = shared_from_this();
    _deadline.async_wait([self](beast::error_code ec){
        if (ec) { //timer being cancelled or error
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

