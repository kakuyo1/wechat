#pragma once
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>
#include <chrono>
#include <boost/url.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
    HttpConnection(tcp::socket socket);
    void Start();
private:
    void CheckDeadline();
    void HandleRequest();
    void SendResponse();
    tcp::socket _socket;
    beast::flat_buffer _buffer {8192}; // Buffer for reading requests
    http::request<http::dynamic_body> _request; // HTTP request object
    http::response<http::dynamic_body> _response; // HTTP response object
    net::steady_timer _deadline; // Timer for handling timeouts
};