#include "../include/ConfigIniManager.h"
#include "../include/CServer.h"
#include "../include/AsioIOContextPool.h"
#include "../include/ChatServiceImpl.h"
#include <thread>
#include "../include/RedisManager.h"

int main() {
    try {
        // 0. Set the terminate handler to log unhandled exceptions
        // This will ensure that if an unhandled exception occurs, it will be logged before the program terminates.
        std::set_terminate([]() {
            spdlog::critical("Unhandled exception, terminating.");
            RedisManager::GetInstance()->Close();
            std::abort();
        });
        // 0.设置日志等级，显示 debug 及以上
        spdlog::set_level(spdlog::level::debug);
        // 0.Initialize the configuration manager
        auto& config = ConfigIniManager::Instance();
        std::string server_name = config["SelfServer"]["Name"];
        // 0.set the SERVER_LOGIN_COUNT to zero in redis
        auto redis = RedisManager::GetInstance();
        if (!redis || !redis->isConnected()) {
            spdlog::error("Failed to connect to Redis");
            return EXIT_FAILURE;
        }
        redis->HSet(SERVER_LOGIN_COUNT_PREFIX, server_name, "0");

        // 1.start the grpc server on a specified port1
        auto pool = AsioIOContextPool::GetInstance();
        auto &ioc_grpc = pool->GetNextIOContext();
        auto grpc_port_str = config["SelfServer"]["RPCPort"];
        auto grpc_host_str = config["SelfServer"]["Host"];
        if (grpc_port_str.empty() || grpc_host_str.empty()) {
            spdlog::error("Missing or empty 'RPCPort' or 'Host' setting in [SelfServer] section.");
            return EXIT_FAILURE;
        }
        unsigned short port_grpc = static_cast<unsigned short>(std::stoi(grpc_port_str));
        std::string RPCserver_address(grpc_host_str + ':' + grpc_port_str);
        ChatServiceImpl service;
        grpc::ServerBuilder builder;
        builder.AddListeningPort(RPCserver_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::shared_ptr<grpc::Server> grpc_server(builder.BuildAndStart());
        if (!grpc_server) {
            spdlog::error("Failed to start gRPC server on {}", RPCserver_address);
            return EXIT_FAILURE;
        }
        // run the gRPC server in a separate thread
        std::thread grpc_thread([grpc_server]() {
            try {
                grpc_server->Wait();
            } catch (const std::exception& e) {
                spdlog::error("Exception in gRPC thread: {}", e.what());
            }
        });
        spdlog::info("Starting gRPC server at {}", RPCserver_address);
        // 2.start the chat server on a specified port2
        auto& section = config["SelfServer"];
        if (section["Port"].empty()) {
            spdlog::error("Missing or empty 'Port' setting in [SelfServer] section.");
            return EXIT_FAILURE;
        }
        unsigned short port = static_cast<unsigned short>(std::stoi(section["Port"]));
        net::io_context ioc;
        // 3.listen on signals
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM, SIGQUIT);
        signals.async_wait([&ioc, &grpc_server, pool](const boost::system::error_code& /*ec*/, int /*signo*/) {
            spdlog::info("Received shutdown signal, stopping io_context.");
            grpc_server->Shutdown();  // 优先 shutdown gRPC(同步阻塞式, 防止异步回调正在进行(需要ioc)，被强行终止)
            pool->Stop();             // 然后停止 Asio 池
            ioc.stop();               // 最后停止当前 io_context
        });
        // Create and start the chat_server
        spdlog::info("Starting ChatServer1 on {}:{}", section["Host"], port);
        auto chat_server = std::make_shared<CServer>(ioc, port);
        chat_server->Start();
        ioc.run();
        // 4.clean work
        redis->HDel(SERVER_LOGIN_COUNT_PREFIX, server_name);
        redis->Close();
        grpc_thread.join(); // Wait for the gRPC server thread to finish
        spdlog::info("ChatServer1 has stopped.");
        return EXIT_SUCCESS; // Return success code
    } catch (const std::exception& e) {
        spdlog::error("Exception caught in main: {}", e.what());
        return EXIT_FAILURE; // Return failure code on exception
    }
}