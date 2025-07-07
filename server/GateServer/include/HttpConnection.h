#pragma once
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>
#include <chrono>
#include <boost/url.hpp>
#include <unordered_map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class LogicSystem;
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
    friend class LogicSystem; // Allow LogicSystem to access private members
public:
    HttpConnection(net::io_context& ioc);
    void Start();
    void PreParseUrlToGetParams();
    tcp::socket& GetSocket();
private:
    void CheckDeadline();
    void HandleRequest();
    void SendResponse();
    tcp::socket _socket;
    beast::flat_buffer _buffer {8192}; // Buffer for reading requests
    http::request<http::dynamic_body> _request; // HTTP request object
    http::response<http::dynamic_body> _response; // HTTP response object
    net::steady_timer _deadline; // Timer for handling timeouts
    std::string _target_route; // Store the target route for the request
    std::unordered_map<std::string, std::string> _query_params; // Store query parameters
};