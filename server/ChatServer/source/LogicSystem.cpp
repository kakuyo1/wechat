#include "../include/LogicSystem.h"
#include "../include/MessageNode.h"
#include "../include/CSession.h"
#include "../include/RedisManager.h"
#include "../include/UserManager.h"

void LogicSystem::RegisterHandler(short message_type, std::function<void(std::shared_ptr<CSession>, std::shared_ptr<RecieveMessageNode>)> handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    // check if the message type already exists
    if (_handlers.find(message_type) != _handlers.end()) {
        spdlog::warn("Handler for message type {} already exists, replacing it.", message_type);
    }
    _handlers[message_type] = std::move(handler);
}

LogicSystem::~LogicSystem()
{
    if (_worker_thread.joinable()) {
        _stopped = true;
        _consumers.notify_all(); // Notify the worker thread to stop
        _worker_thread.join();
    }
}

void LogicSystem::PostLogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node)
{
    std::unique_lock<std::mutex> lock(_mutex);
    if (_stopped) {
        spdlog::warn("LogicSystem is stopped, cannot post new logic node.");
        return;
    }
    _message_queue.push(std::make_shared<LogicNode>(session, message_node));
    _consumers.notify_one();
}

void LogicSystem::InitializeHandlers()
{
    // deal with login authentication messages
    RegisterHandler(static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH), [this](std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node) {
        // 1.Extract message data by json parsing
        std::string message_data(message_node->GetData(), message_node->GetCurrentLength());
        std::istringstream message_stream(message_data);
        Json::Value source;
        Json::Value response;
        Json::CharReaderBuilder reader_builder;
        std::string errors;
        if(!Json::parseFromStream(reader_builder, message_stream, &source, &errors)) {
            spdlog::error("Failed to parse JSON from message: {}", errors);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_JSON_PARSE);
            response["message"] = "Invalid JSON format";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            return;
        }
        auto uid = source["uid"].asInt();
        auto client_token = source["token"].asString();
        spdlog::info("[LogicSystem]Received login auth request for UID: {}, Token: {}", uid, client_token);

        // 2.Validate the token and uid by calling StatusServer(Login)
        auto status_server = StatusGrpcClient::GetInstance();
        auto status = status_server->Login(uid, client_token);
        if (status.error() != static_cast<int>(ErrorCodes::SUCCESS)) {
            if (status.error() == static_cast<int>(ErrorCodes::ERROR_UID_NOT_FOUND)) {
                spdlog::warn("UID {} not found, sending error response.", uid);
                response["error"] = static_cast<short>(ErrorCodes::ERROR_UID_NOT_FOUND);
                response["message"] = "UID not found";
                session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            } else if (status.error() == static_cast<int>(ErrorCodes::ERROR_TOKEN_MISMATCH)) {
                spdlog::warn("Token mismatch for UID {}, sending error response.", uid);
                response["error"] = static_cast<short>(ErrorCodes::ERROR_TOKEN_MISMATCH);
                response["message"] = "Token mismatch";
                session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            } else {
                spdlog::error("Unexpected error during login auth: {}", status.error());
                response["error"] = static_cast<short>(ErrorCodes::ERROR_RPC);
                response["message"] = "Internal server error";
                session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            }
        }
        // 3.If login is successful, cache the user into _users info by mysql, if already in, meaning user is online, return error
        auto it = _users.find(uid);
        if (it != _users.end()) {
            spdlog::warn("User with UID {} is already online, sending error response.", uid);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_USER_ALREADY_ONLINE);
            response["message"] = "User is already online";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            return;
        } else {
            auto mysql_manager = MysqlManager::GetInstance();
            auto user_info = mysql_manager->GetUserInfo(uid);
            if (user_info.uid == 0) {
                spdlog::error("Failed to retrieve user info for UID {}, sending error response.", uid);
                response["error"] = static_cast<short>(ErrorCodes::ERROR_UID_NOT_FOUND);
                response["message"] = "User not found";
                session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
                return;
            }
            // check if email exists
            _users[uid] = std::make_shared<UserInfo>(user_info);
        }
        // 4.get fullUserInfo for Client Initialization
        std::shared_ptr<FullUserInfo> userinfoptr = nullptr;
        bool isSuccess = UserManager::GetInstance()->getFullUserInfo(uid, userinfoptr);
        if (!isSuccess) {
            spdlog::error("Failed to get full user info for UID {}, sending error response.", uid);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_UID_NOT_FOUND);
            response["message"] = "User not found";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            return;
        }
        spdlog::info("[LogicSystem]User with UID {} authenticated successfully, sending response.", uid);
        // 5.prepare the BaseFullInfo response to the client
        response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
        response["uid"] = userinfoptr->uid;
        response["gender"] = userinfoptr->gender;
        response["name"] = userinfoptr->name;
        response["nickname"] = userinfoptr->nickname;
        response["email"] = userinfoptr->email;
        response["icon"] = userinfoptr->icon;
        response["desc"] = userinfoptr->desc;
        response["password"] = userinfoptr->password;
        response["token"] = client_token;
        response["message"] = "Login successful";
        // TODO 6.prepare the friend/contact list for the client

        // TODO 7.prepare the friend request list for the client

        // 8.increment the online user count in ChatServer by redis(maintain loginCount synchronization between all the ChatServers)
        auto server_name = ConfigIniManager::Instance()["SelfServer"]["Name"];
        auto current_login_count = RedisManager::GetInstance()->HGet(SERVER_LOGIN_COUNT_PREFIX, server_name);
        if (current_login_count.empty()) {
            spdlog::warn("No login count found for server: {}.", server_name);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to retrieve login count from Redis";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            return;
        }
        int new_login_count = std::stoi(current_login_count) + 1; // increment
        if (!RedisManager::GetInstance()->HSet(SERVER_LOGIN_COUNT_PREFIX, server_name, std::to_string(new_login_count))) {
            spdlog::error("Failed to increment login count for server: {}.", server_name);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to increment login count in Redis";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            return;
        }
        spdlog::info("[LogicSystem]server {} login count incremented to {}", server_name, new_login_count);

        // 9. Bind the uid and session then Store the session in UserManager(aim for across server communication)
        session->setSessionUid(uid);
        UserManager::GetInstance()->setUidToSession(uid, session);

        // 10. Set the serverIP for the user/session(aim for across server communication)
        std::string server_ip_key = SERVER_IP_PREFIX + std::to_string(uid);
        if (!RedisManager::GetInstance()->Set(server_ip_key, server_name)) {
            spdlog::error("Failed to set server IP for UID {} in Redis.", uid);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
            response["message"] = "Failed to set server IP in Redis";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
            return;
        }

        // send the final response to the client
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
    });
}

void LogicSystem::ProcessMessageQueue()
{
    while (true) {
        std::unique_lock<std::mutex> lock(_mutex);
        _consumers.wait(lock, [this] { return !_message_queue.empty() || _stopped; });
        if (_stopped) { // process all the nodes before real stopping
            while (!_message_queue.empty()) {
                auto logic_node = _message_queue.front();
                if (logic_node == nullptr) {
                    spdlog::warn("Received null logic node, skipping.");
                    _message_queue.pop();
                    continue;
                }
                lock.unlock();
                auto message_node = logic_node->GetMessageNode();
                auto session = logic_node->GetSession();
                // Find the handler for the message type
                auto it = _handlers.find(message_node->GetMessageType());
                if (it != _handlers.end()) {
                    // Call the handler with the session and message node
                    it->second(session, message_node);
                    continue; // Process the next message
                } else {
                    spdlog::warn("No handler found for message type: {}", message_node->GetMessageType());
                }
                lock.lock();
                _message_queue.pop();
            }
        }
        // not stopped, process the next message
        if (!_message_queue.empty()) {
            auto logic_node = _message_queue.front();
            if (logic_node == nullptr) {
                spdlog::warn("Received null logic node, skipping.");
                _message_queue.pop();
                continue;
            }
            lock.unlock();
            auto message_node = logic_node->GetMessageNode();
            auto session = logic_node->GetSession();
            // Find the handler for the message type
            auto it = _handlers.find(message_node->GetMessageType());
            if (it != _handlers.end()) {
                it->second(session, message_node);
            } else {
                spdlog::warn("No handler found for message type: {}", message_node->GetMessageType());
            }
            lock.lock();
            _message_queue.pop();
        }
    }
}

LogicSystem::LogicSystem() :
    _stopped(false),
    _worker_thread(&LogicSystem::ProcessMessageQueue, this), // LogicSystem一构造就不断处理消息队列(客户端的信息转发,好友申请,登陆验证等)
    _message_queue{},
    _handlers{},
    _mutex{},
    _consumers{}
{
    InitializeHandlers();
}
