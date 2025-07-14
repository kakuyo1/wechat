#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <map>
#include "AsioIOContextPool.h"
#include <mutex>
#include <spdlog/spdlog.h>

using tcp = boost::asio::ip::tcp;
namespace net = boost::asio;

class CSession;
class CServer : public std::enable_shared_from_this<CServer> {
public:
    CServer(boost::asio::io_context& ioc, unsigned short port);
    ~CServer() = default;
    void ClearSession(const std::string& uuid);
    void Start();
private:
    boost::asio::io_context& _ioc;
    unsigned short _port;
    tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession>> _sessions; // Map of session UUID to CSession pointer
    std::mutex _mutex; // Mutex to protect access to _sessions
};
