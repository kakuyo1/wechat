#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <queue>
#include "Singleton.h"
#include "MessageNode.h"
#include "StatusGrpcClient.h"
#include "CSession.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include "MysqlManager.h"

using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
class LogicSystem : public Singleton<LogicSystem>, public std::enable_shared_from_this<LogicSystem> {
    friend class Singleton<LogicSystem>;
public:
    ~LogicSystem();
    void PostLogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node);
    void RegisterHandler(short message_type, std::function<void(std::shared_ptr<CSession>, std::shared_ptr<RecieveMessageNode>)> handler);
    void InitializeHandlers();
private:
    LogicSystem();
    void ProcessMessageQueue();
private:
    std::unordered_map<short, std::function<void(std::shared_ptr<CSession>, std::shared_ptr<RecieveMessageNode>)>> _handlers;
    std::queue<std::shared_ptr<LogicNode>> _message_queue;
    std::mutex _mutex;
    std::condition_variable _consumers;
    std::atomic<bool> _stopped;
    std::thread _worker_thread;
    std::unordered_map<int, std::shared_ptr<UserInfo>> _users;
};