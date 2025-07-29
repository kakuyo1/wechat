#include "../include/ChatServiceImpl.h"
#include "../include/CSession.h"

Status ChatServiceImpl::NotifyAddFriend(::grpc::ServerContext* context,
    const AddFriendRequest* request, AddFriendResponse* response) {
        // check if the user(friendRequest receiver) online(temporary by check if uid:csession exists)
        int to_uid = request->to_uid();
        std::shared_ptr<CSession> session = UserManager::GetInstance()->getSessionByUid(to_uid);
        if (session == nullptr) {
            spdlog::error("User with UID {} is not online.", to_uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_USER_OFFLINE));
            return Status::CANCELLED; // Return error status
        }
        // prepare for json string to send
        Json::Value root;
        root["from_uid"] = request->from_uid();
        root["to_uid"] = request->to_uid();
        std::string jsonString = root.toStyledString();

        // send the json string to the user
        session->Send(jsonString, static_cast<int>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_PUSH));
        return Status::OK; // Return success status
    }
Status ChatServiceImpl::NotifyAuthFriend(::grpc::ServerContext* context,
    const AuthFriendRequest* request, AuthFriendResponse* response) {
        // check if the user(friendRequest receiver) online(temporary by check if uid:csession exists)
        int to_uid = request->to_uid();
        std::shared_ptr<CSession> session = UserManager::GetInstance()->getSessionByUid(to_uid);
        if (session == nullptr) {
            spdlog::error("User with UID {} is not online.", to_uid);
            response->set_error(static_cast<int>(ErrorCodes::ERROR_USER_OFFLINE));
            return Status::CANCELLED; // Return error status
        }
        // prepare for json string to send
        Json::Value root;
        root["from_uid"] = request->from_uid();
        root["to_uid"] = request->to_uid();
        std::string jsonString = root.toStyledString();
        // send the json string to the user
        session->Send(jsonString, static_cast<int>(MessageType::MESSAGE_CHATSERVER_AUTHFRIEND_PUSH));
        return Status::OK; // Return success status
    }
