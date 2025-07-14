#include "ConfigIniManager.h"
#include "CServer.h"

int main() {
    try {
        // 设置日志等级，显示 debug 及以上
        spdlog::set_level(spdlog::level::debug);

        auto& config = ConfigIniManager::Instance();
        auto& section = config["ChatServer1"];
        if (section["Port"].empty()) {
            spdlog::error("Missing or empty 'Port' setting in [ChatServer1] section.");
            return EXIT_FAILURE;
        }
        unsigned short port = static_cast<unsigned short>(std::stoi(section["Port"]));
        net::io_context ioc;
        // listen on signals
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& /*ec*/, int /*signo*/) {
            spdlog::info("Received shutdown signal, stopping io_context.");
            ioc.stop(); // Stop the io_context on signal
        });
        // Create and start the server
        spdlog::info("Starting ChatServer1 on port {}", port);
        auto server = std::make_shared<CServer>(ioc, port);
        server->Start();
        ioc.run();
    } catch (const std::exception& e) {
        spdlog::error("Exception caught in main: {}", e.what());
        return EXIT_FAILURE; // Return failure code on exception
    }
}