#pragma once
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class HttpConnection;
class CServer : public std::enable_shared_from_this<CServer>
{
public:
    CServer(net::io_context& ioc, unsigned short port);
    void Start();
    ~CServer() = default;
private:
    net::io_context& _ioc; // only for acceptor
    tcp::acceptor _acceptor;
    tcp::socket _socket;
};