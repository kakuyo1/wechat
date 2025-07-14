#include "LogicSystem.h"
#include "MessageNode.h"
#include "CSession.h"

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
        // Extract message data by json parsing
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
        auto token = source["token"].asString();
        spdlog::info("[LogicSystem]Received login auth request for UID: {}, Token: {}", uid, token);

        // Validate the token and uid by calling StatusServer
        auto status_server = StatusGrpcClient::GetInstance();
        auto status = status_server->Login(uid, token);
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
        // If login is successful, cache the user into _users info by mysql, if already in, meaning user is online, return error
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
            _users[uid] = std::make_shared<UserInfo>(user_info);
        }
        spdlog::info("[LogicSystem]User with UID {} authenticated successfully, sending response.", uid);
        response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
        response["uid"] = uid;
        response["token"] = token;
        response["message"] = "Login successful";
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
    _worker_thread(&LogicSystem::ProcessMessageQueue, this),
    _message_queue{},
    _handlers{},
    _mutex{},
    _consumers{}
{
    InitializeHandlers();
}
