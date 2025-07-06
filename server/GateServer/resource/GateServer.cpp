#include <iostream>
#include <boost/asio.hpp>
#include "CServer.h"
#include "ConfigIniManager.h"
int main() {
    try{
        // Load configuration
        auto& config_manager = ConfigIniManager::Instance();
        std::string port_str = config_manager["GateServer"]["Port"]; //调用的是const std::string& operator[](const std::string& key) const;
        unsigned short port = static_cast<unsigned short>(std::atoi(port_str.c_str()));

        boost::asio::io_context ioc; //only for acceptor

        // handle signals to stop the server gracefully
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& ec, int signo) {
            if (ec) {
                std::cerr << "Error handling signal: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Stopping GateServer..." << std::endl;
            ioc.stop(); // Stop the io_context to exit the event loop
        });

        // Create and start the server
        auto server = std::make_shared<CServer>(ioc, port);
        server->Start();
        std::cout << "GateServer started on port " << port << std::endl;

        ioc.run(); // Run the io_context to start processing events
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}