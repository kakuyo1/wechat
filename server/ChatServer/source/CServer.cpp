#include "CServer.h"
#include "CSession.h"
CServer::CServer(net::io_context& ioc, unsigned short port) :
    _ioc(ioc),
    _port(port),
    _acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{

}

void CServer::Start() {
    try {
        auto self = shared_from_this();
        auto &ioc = AsioIOContextPool::GetInstance()->GetNextIOContext();
        auto new_session = std::make_shared<CSession>(ioc, self);
        _acceptor.async_accept(new_session->GetSocket(), [self, new_session](const boost::system::error_code& ec){
            if (!ec) {
                // Successfully accepted a new connection
                spdlog::info("New connection accepted.");
                self->_sessions[new_session->GetUUID()] = new_session;
                new_session->Start();
            } else {
                spdlog::error("Failed to accept new connection: {}", ec.message());
            }
            // Continue accepting new connections
            self->Start();
        });
    } catch (const std::exception& e) {
        spdlog::error("Failed to start server: {}", e.what());
        throw; // Re-throw the exception after logging
    }
}

void CServer::ClearSession(const std::string& uuid) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _sessions.find(uuid);
    if (it != _sessions.end()) {
        _sessions.erase(it);
    } else {
        // Handle the case where the session does not exist
        throw std::runtime_error("Session not found");
    }
}