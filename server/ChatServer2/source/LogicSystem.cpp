#include "../include/LogicSystem.h"
#include "../include/MessageNode.h"
#include "../include/CSession.h"
#include "../include/RedisManager.h"
#include "../include/UserManager.h"
#include "../include/ChatGrpcClient.h"

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
    RegisterHandler(static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH),
    std::bind(&LogicSystem::HandleLoginAuth, this, std::placeholders::_1, std::placeholders::_2));

    // deal with client search user messages
    RegisterHandler(static_cast<short>(MessageType::MESSAGE_CLIENT_SEARCH_USER_REQUEST),
    std::bind(&LogicSystem::HandleClientSearchUser, this, std::placeholders::_1, std::placeholders::_2));

    // deal with add friend messages
    RegisterHandler(static_cast<short>(MessageType::MESSAGE_CLIENT_ADDFRIEND_REQUEST),
    std::bind(&LogicSystem::HandleAddFriend, this, std::placeholders::_1, std::placeholders::_2));

    // deal with auth friend messages
    RegisterHandler(static_cast<short>(MessageType::MESSAGE_CLIENT_AUTHFRIEND_REQUEST),
    std::bind(&LogicSystem::HandleAuthFriend, this, std::placeholders::_1, std::placeholders::_2));
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
                it->second(session, message_node); // Call the handler with the session and message node
            } else {
                spdlog::warn("No handler found for message type: {}", message_node->GetMessageType());
            }
            lock.lock();
            _message_queue.pop();
        }
    }
}

void LogicSystem::HandleLoginAuth(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node)
{
    // 1.Extract message data by json parsing
    spdlog::debug("Step 1");
    std::string message_data(message_node->GetData(), message_node->GetCurrentLength());
    std::istringstream message_stream(message_data);
    Json::Value source;
    Json::Value response;
    Json::CharReaderBuilder reader_builder;
    std::string errors;
    if (!Json::parseFromStream(reader_builder, message_stream, &source, &errors))
    {
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
    spdlog::debug("Step 2");
    auto status_server = StatusGrpcClient::GetInstance();
    auto status = status_server->Login(uid, client_token);
    if (status.error() != static_cast<int>(ErrorCodes::SUCCESS))
    {
        if (status.error() == static_cast<int>(ErrorCodes::ERROR_UID_NOT_FOUND))
        {
            spdlog::warn("UID {} not found, sending error response.", uid);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_UID_NOT_FOUND);
            response["message"] = "UID not found";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        }
        else if (status.error() == static_cast<int>(ErrorCodes::ERROR_TOKEN_MISMATCH))
        {
            spdlog::warn("Token mismatch for UID {}, sending error response.", uid);
            response["error"] = static_cast<short>(ErrorCodes::ERROR_TOKEN_MISMATCH);
            response["message"] = "Token mismatch";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        }
        else
        {
            spdlog::error("Unexpected error during login auth: {}", status.error());
            response["error"] = static_cast<short>(ErrorCodes::ERROR_RPC);
            response["message"] = "Internal server error";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        }
    }
    // 3.If login is successful, cache the user into _users info by mysql, if already in, meaning user is online, return error
    spdlog::debug("Step 3");
    auto it = _users.find(uid);
    if (it != _users.end())
    {
        spdlog::warn("User with UID {} is already online, sending error response.", uid);
        response["error"] = static_cast<short>(ErrorCodes::ERROR_USER_ALREADY_ONLINE);
        response["message"] = "User is already online";
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        return;
    }
    else
    {
        auto mysql_manager = MysqlManager::GetInstance();
        auto user_info = mysql_manager->GetUserInfo(uid);
        if (user_info.uid == 0)
        {
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
    spdlog::debug("Step 4");
    std::shared_ptr<FullUserInfo> userinfoptr = std::make_shared<FullUserInfo>();
    bool isSuccess = UserManager::GetInstance()->getFullUserInfoByUid(uid, userinfoptr);
    if (!isSuccess)
    {
        spdlog::error("Failed to get full user info for UID {}, sending error response.", uid);
        response["error"] = static_cast<short>(ErrorCodes::ERROR_UID_NOT_FOUND);
        response["message"] = "User not found";
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        return;
    }
    spdlog::info("[LogicSystem]User with UID {} authenticated successfully, sending response.", uid);
    // 5.prepare the BaseFullInfo response to the client
    spdlog::debug("Step 5");
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
    spdlog::debug("Step 8");
    auto server_name = ConfigIniManager::Instance()["SelfServer"]["Name"];
    auto current_login_count = RedisManager::GetInstance()->HGet(SERVER_LOGIN_COUNT, server_name);
    if (current_login_count.empty())
    {
        spdlog::warn("No login count found for server: {}.", server_name);
        response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
        response["message"] = "Failed to retrieve login count from Redis";
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        return;
    }
    int new_login_count = std::stoi(current_login_count) + 1; // increment
    if (!RedisManager::GetInstance()->HSet(SERVER_LOGIN_COUNT, server_name, std::to_string(new_login_count)))
    {
        spdlog::error("Failed to increment login count for server: {}.", server_name);
        response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
        response["message"] = "Failed to increment login count in Redis";
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        return;
    }
    spdlog::info("[LogicSystem]server {} login count incremented to {}", server_name, new_login_count);

    // 9. Bind the uid and session then Store the session in UserManager(aim for across server communication)
    spdlog::debug("Step 9");
    session->setSessionUid(uid);
    UserManager::GetInstance()->setUidToSession(uid, session);

    // 10. Set the serverIP for the user/session(aim for across server communication)
    spdlog::debug("Step 10");
    std::string server_ip_key = SERVER_IP_PREFIX + std::to_string(uid);
    if (!RedisManager::GetInstance()->Set(server_ip_key, server_name))
    {
        spdlog::error("Failed to set server IP for UID {} in Redis.", uid);
        response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
        response["message"] = "Failed to set server IP in Redis";
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
        return;
    }

    // send the final response to the client
    spdlog::debug("Step final");
    session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE));
}

void LogicSystem::HandleLoginOut(std::shared_ptr<CSession> session)
{
    // 1. Extract user ID from session
    int uid = session->getSessionUid();
    if (uid == -1)
    {
        spdlog::error("Failed to get UID from session.");
        return;
    }

    // 2. Remove user session from UserManager and _users map
    UserManager::GetInstance()->removeUidToSession(uid);
    auto it = _users.find(uid);
    if (it != _users.end())
    {
        _users.erase(it);
        spdlog::info("[LogicSystem]User with UID {} logged out successfully.", uid);
    }
    else
    {
        spdlog::warn("[LogicSystem]User with UID {} not found in _users map.", uid);
    }

    // 3. Decrement the online user count in ChatServer by redis(maintain loginCount synchronization between all the ChatServers)
    auto server_name = ConfigIniManager::Instance()["SelfServer"]["Name"];
    auto current_login_count = RedisManager::GetInstance()->HGet(SERVER_LOGIN_COUNT, server_name);
    if (current_login_count.empty())
    {
        spdlog::warn("No login count found for server: {}.", server_name);
        return;
    }
    int new_login_count = std::stoi(current_login_count) - 1; // decrement
    if (!RedisManager::GetInstance()->HSet(SERVER_LOGIN_COUNT, server_name, std::to_string(new_login_count)))
    {
        spdlog::error("Failed to decrement login count for server: {}.", server_name);
        return;
    }
    spdlog::info("[LogicSystem]server {} login count decremented to {}", server_name, new_login_count);

    // 4. Remove the server IP for the user/session from Redis
    std::string server_ip_key = SERVER_IP_PREFIX + std::to_string(uid);
    if (!RedisManager::GetInstance()->Delete(server_ip_key))
    {
        spdlog::error("Failed to delete server IP for UID {} in Redis.", uid);
        return;
    }
}

void LogicSystem::HandleClientSearchUser(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node)
{
    // 1.Extract message data by json parsing
    std::string message_data(message_node->GetData(), message_node->GetCurrentLength());
    std::istringstream message_stream(message_data);
    Json::Value source;
    Json::Value response;
    Json::CharReaderBuilder reader_builder;
    std::string errors;
    if (!Json::parseFromStream(reader_builder, message_stream, &source, &errors))
    {
        spdlog::error("Failed to parse JSON from message: {}", errors);
        response["error"] = static_cast<short>(ErrorCodes::ERROR_JSON_PARSE);
        response["message"] = "Invalid JSON format";
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE));
        return;
    }
    // 2.Distinguish the Client Input, search by uid or name
    auto userManager = UserManager::GetInstance();
    std::shared_ptr<FullUserInfo> fulluserinfo = std::make_shared<FullUserInfo>();
    if (source.isMember("uid") && source["uid"].isInt()) { // By uid
        if(!userManager->getFullUserInfoByUid(source["uid"].asInt(), fulluserinfo))
        {
            spdlog::error("Failed to get userInfo by uid.");
            response["error"] = static_cast<short>(ErrorCodes::ERROR_SEARCH_FAILED_BY_UID);
            response["message"] = "Failed to get userInfo by uid.";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE));
            return;
        }
        // search success, send the response(no password back)
        response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
        response["message"] = "Successfully retrieved userInfo by uid.";
        response["uid"] = fulluserinfo->uid;
        response["gender"] = fulluserinfo->gender;
        response["name"] = fulluserinfo->name;
        response["desc"] = fulluserinfo->desc;
        response["email"] = fulluserinfo->email;
        response["icon"] = fulluserinfo->icon;
        spdlog::debug("Client Icon Path: {}", fulluserinfo->icon);
        response["nickname"] = fulluserinfo->nickname;
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE));
        spdlog::info("[LogicSystem]User search by uid successful: {}", fulluserinfo->uid);
    } else if (source.isMember("name") && source["name"].isString()) { // By name
        if(!userManager->getFullUserInfoByName(source["name"].asString(), fulluserinfo))
        {
            spdlog::error("Failed to get userInfo by name.");
            response["error"] = static_cast<short>(ErrorCodes::ERROR_SEARCH_FAILED_BY_NAME);
            response["message"] = "Failed to get userInfo by name.";
            session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE));
            return;
        }
        // search success, send the response(no password back)
        response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
        response["message"] = "Successfully retrieved userInfo by name.";
        response["uid"] = fulluserinfo->uid;
        response["gender"] = fulluserinfo->gender;
        response["name"] = fulluserinfo->name;
        response["desc"] = fulluserinfo->desc;
        response["email"] = fulluserinfo->email;
        response["icon"] = fulluserinfo->icon;
        response["nickname"] = fulluserinfo->nickname;
        session->Send(response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE));
        spdlog::info("[LogicSystem]User search by name successful: {}", fulluserinfo->name);
    }
}

void LogicSystem::HandleAddFriend(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node)
{
    // 1.Extract message data by json parsing
    std::string message_data(message_node->GetData(), message_node->GetCurrentLength());
    std::istringstream message_stream(message_data);
    Json::Value source;
    Json::Value ack_response;
    Json::Value notification_response;
    Json::CharReaderBuilder reader_builder;
    std::string errors;
    if (!Json::parseFromStream(reader_builder, message_stream, &source, &errors))
    {
        spdlog::error("Failed to parse JSON from message: {}", errors);
        ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_JSON_PARSE);
        ack_response["message"] = "Invalid JSON format";
        session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
        return;
    }
    int to_uid = source["to_uid"].asInt();
    int from_uid = source["from_uid"].asInt();
    std::string nickname = source["nickname"].asString(); // The nickname of the sender to the receiver
    spdlog::debug("Step 1: Received add friend request from UID {} to UID {}, nickname: {}", from_uid, to_uid, nickname);

    // 2.update the friend request list in Mysql
    if (!MysqlManager::GetInstance()->AddItemToFriendRequestList(from_uid, to_uid)) {
        spdlog::error("Failed to add item to friend request list from UID {} to UID {}.", from_uid, to_uid);
        ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_MYSQL);
        ack_response["message"] = "Failed to add friend request";
        session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
        return;
    }
    spdlog::debug("Step 2: Successfully added item to friend request list from UID {} to UID {}.", from_uid, to_uid);

    // 3. figure out which ChatServer the receiver's session is on
    auto current_server_name = ConfigIniManager::Instance()["SelfServer"]["Name"];
    std::string server_ip_key = SERVER_IP_PREFIX + std::to_string(to_uid);
    std::string peer_server_name = "";
    if (!RedisManager::GetInstance()->Get(server_ip_key, peer_server_name)) {
        spdlog::error("Failed to get server IP for UID {} from Redis.", to_uid);
        ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_REDIS);
        ack_response["message"] = "Failed to retrieve server IP for friend request";
        MysqlManager::GetInstance()->RemoveItemFromFriendRequestList(from_uid, to_uid);
        session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
        return;
    }
    if (peer_server_name.empty()) {
        spdlog::warn("No server IP found for UID {}, sending error response.", to_uid);
        ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_USER_OFFLINE);
        ack_response["message"] = "User is offline or not found";
        MysqlManager::GetInstance()->RemoveItemFromFriendRequestList(from_uid, to_uid);
        session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
        return;
    }
    spdlog::debug("Step 3: Found peer server {} for UID {}.", peer_server_name, to_uid);

    /* 4. compare the current server name with the peer server name to decide how to handle the request
        (1) If they are the same, send the response through this ChatServer)
        (2) If they are different, forward the request to the peer server by using gRPC)*/
    if (current_server_name == peer_server_name) { // (1)
        spdlog::debug("entering the same server branch");
        // 4.1 send the ack response to the sender
        ack_response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
        ack_response["message"] = "Friend request sent successfully";
        ack_response["to_uid"] = to_uid;
        ack_response["from_uid"] = from_uid;
        session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));

        // 4.2 send the notification to the receiver directly through this ChatServer
            // 4.2.1 get the receiver's session
            auto receiver_session = UserManager::GetInstance()->getSessionByUid(to_uid);
            if (!receiver_session) {
                spdlog::warn("No session found for UID {}, sending error response.", to_uid);
                ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_USER_OFFLINE);
                ack_response["message"] = "User is offline or not found";
                MysqlManager::GetInstance()->RemoveItemFromFriendRequestList(from_uid, to_uid);
                session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
                return;
            }
            // 4.2.2 get the fulluserinfo of sender then prepare the notification responses
            notification_response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
            notification_response["message"] = "Friend request received";
            std::shared_ptr<FullUserInfo> sender_fulluserinfo = std::make_shared<FullUserInfo>();
            if (!UserManager::GetInstance()->getFullUserInfoByUid(from_uid, sender_fulluserinfo)) {
                spdlog::error("Failed to get full user info for UID {}.", from_uid);
                ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_UID_NOT_FOUND);
                ack_response["message"] = "User not found";
                MysqlManager::GetInstance()->RemoveItemFromFriendRequestList(from_uid, to_uid);
                session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
                return;
            }
            notification_response["uid"] = sender_fulluserinfo->uid;
            notification_response["gender"] = sender_fulluserinfo->gender;
            notification_response["name"] = sender_fulluserinfo->name;
            notification_response["nickname"] = sender_fulluserinfo->nickname;
            notification_response["icon"] = sender_fulluserinfo->icon;
            notification_response["desc"] = sender_fulluserinfo->desc;
            notification_response["email"] = sender_fulluserinfo->email;
            receiver_session->Send(notification_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_PUSH));

        spdlog::debug("Step 4: Current server is the same as peer server, sending response directly.");
    } else { // (2)
        spdlog::debug("entering the different server branch");
        // Implement gRPC forwarding
        AddFriendRequest request;
        request.set_from_uid(from_uid);
        request.set_to_uid(to_uid);
        AddFriendResponse response = ChatGrpcClient::GetInstance()->NotifyAddFriend(peer_server_name, request);
        if (response.error() != static_cast<int>(ErrorCodes::SUCCESS)) {
            spdlog::error("Failed to forward add friend request to peer server {}, error code {}.", peer_server_name, response.error());
            ack_response["error"] = static_cast<short>(ErrorCodes::ERROR_RPC);
            ack_response["message"] = "Failed to forward add friend request";
            MysqlManager::GetInstance()->RemoveItemFromFriendRequestList(from_uid, to_uid);
            session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
            return;
        }
        // 4.1 send the ack response to the sender
        ack_response["error"] = static_cast<short>(ErrorCodes::SUCCESS);
        ack_response["message"] = "Friend request forwarded successfully";
        ack_response["to_uid"] = to_uid;
        ack_response["from_uid"] = from_uid;
        session->Send(ack_response.toStyledString(), static_cast<short>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_ACK));
        // 4.2 notification response is handled by the peer server(by gRPC), so we don't need to send it here
        spdlog::debug("Step 4: Current server is different from peer server, forwarding request.");
    }
}
void LogicSystem::HandleAuthFriend(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node)
{

}
LogicSystem::LogicSystem() : _stopped(false),
                             _worker_thread(&LogicSystem::ProcessMessageQueue, this), // LogicSystem一构造就不断处理消息队列(客户端的信息转发,好友申请,登陆验证等)
                             _message_queue{},
                             _handlers{},
                             _mutex{},
                             _consumers{}
{
    InitializeHandlers();
}
