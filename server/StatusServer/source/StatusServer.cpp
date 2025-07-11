#include "StatusServiceImpl.h"
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <memory>
#include <spdlog/spdlog.h>

void RunServer() {
    auto& config = ConfigIniManager::Instance();
    auto section = config["StatusServer"];
    auto host = section["Host"];
    auto port = section["Port"];
    std::string server_address(host + ':' + port);

    // listen on signals
    boost::asio::io_context io_context;
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](const boost::system::error_code& e , int ) {
        if (!e) {
            spdlog::info("Received shutdown signal, stopping server...");
            io_context.stop(); // Stop the io_context to exit the run loop
        } else {
            spdlog::error("Error receiving shutdown signal: {}", e.message());
        }
    });

    // start a thread to run the io_context
    std::thread io_thread([&io_context]() {
        try {
            io_context.run();
        } catch (const std::exception& e) {
            spdlog::error("Error in io_context: {}", e.what());
        }
    });
    io_thread.detach(); // Detach the thread to run independently(to let the Wait() function block the main thread)

    StatusServiceImpl service{}; //overrided the StatusService::Service(GetChatServer)

    grpc::ServerBuilder builder; // Create a new ServerBuilder instance for building the server

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials()); // Add the listening port with insecure credentials

    builder.RegisterService(&service);// Register the service implementation with the server builder

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());// Build and start the server
    spdlog::info("StatusServer running on host: {}, port: {}.", host, port);

    server->Wait(); // block until the server is shutdown
}

int main() {
    try {
        RunServer();
    } catch(std::exception& e) {
        spdlog::critical("StatusServer run failed, error: {}", e.what());
        return EXIT_FAILURE;
    }
}