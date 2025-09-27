#include "../include/ChatServiceImpl.h"
#include "../include/CSession.h"

Status ChatServiceImpl::NotifyAddFriend(::grpc::ServerContext* context, /* if you sent a addFriend request, now we are in a diffent chatServer*/
    const AddFriendRequest* request, AddFriendResponse* response) {
        spdlog::debug("[ChatServiceImpl] Enter NotifyAddFriend");
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
        root["error"] = static_cast<int>(ErrorCodes::SUCCESS);
        root["message"] = "Friend request received";
        std::shared_ptr<FullUserInfo> sender_fulluserinfo = std::make_shared<FullUserInfo>();
        if (!UserManager::GetInstance()->getFullUserInfoByUid(request->from_uid(), sender_fulluserinfo)) {
            spdlog::error("Failed to get full user info for UID {}", request->from_uid());
            response->set_error(static_cast<int>(ErrorCodes::ERROR_UID_NOT_FOUND));
            return Status::CANCELLED; // Return error status
        }
        root["uid"] = sender_fulluserinfo->uid;
        root["name"] = sender_fulluserinfo->name;
        root["nickname"] = sender_fulluserinfo->nickname;
        root["icon"] = sender_fulluserinfo->icon;
        root["desc"] = sender_fulluserinfo->desc;
        root["gender"] = sender_fulluserinfo->gender;
        root["email"] = sender_fulluserinfo->email;
        std::string jsonString = root.toStyledString();

        // send the json string to the user
        session->Send(jsonString, static_cast<int>(MessageType::MESSAGE_CHATSERVER_ADDFRIEND_PUSH));
        spdlog::debug("[ChatServiceImpl] MessageType::MESSAGE_CHATSERVER_ADDFRIEND_PUSH sent to UID {}", to_uid);
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

        // get the userInfo of the 验证sender(from_uid) to prepare the Push response to the 验证reciver(to_uid)
        std::shared_ptr<FullUserInfo> sender_fulluserinfo = std::make_shared<FullUserInfo>();
        if (!UserManager::GetInstance()->getFullUserInfoByUid(request->from_uid(), sender_fulluserinfo)) {
            spdlog::error("Failed to get full user info for UID {}", request->from_uid());
            response->set_error(static_cast<int>(ErrorCodes::ERROR_UID_NOT_FOUND));
            return Status::CANCELLED; // Return error status
        }

        // prepare for json string to send
        Json::Value root;
        root["error"] = static_cast<int>(ErrorCodes::SUCCESS);
        root["message"] = "Friend request authentication received";
        root["from_uid"] = sender_fulluserinfo->uid;
        root["from_name"] = sender_fulluserinfo->name;
        root["from_nickname"] = sender_fulluserinfo->nickname;
        root["from_icon"] = sender_fulluserinfo->icon;
        root["from_desc"] = sender_fulluserinfo->desc;
        root["from_email"] = sender_fulluserinfo->email;
        root["from_gender"] = sender_fulluserinfo->gender;

        std::string jsonString = root.toStyledString();
        // send the json string to the user
        session->Send(jsonString, static_cast<int>(MessageType::MESSAGE_CHATSERVER_AUTHFRIEND_PUSH));
        return Status::OK; // Return success status
    }

Status ChatServiceImpl::NotifyTextChatTrans(::grpc::ServerContext *context, const chat_message::TextChatTransRequest *request, chat_message::TextChatTransResponse *response)
{
    int to_uid = request->to_uid();
    auto session = UserManager::GetInstance()->getSessionByUid(to_uid);
    if (session == nullptr) { // the peer is offline, here we just return ok
        spdlog::warn("User with UID {} is not online, cannot forward text chat message.", to_uid);
        response->set_error(static_cast<int>(ErrorCodes::ERROR_USER_OFFLINE));
        return Status::OK;
    }
    Json::Value returnJson;
    returnJson["error"] = static_cast<int>(ErrorCodes::SUCCESS);
    returnJson["message"] = "Text chat message transfer successful";
    returnJson["from_uid"] = request->from_uid();
    returnJson["to_uid"] = request->to_uid();
    Json::Value textArrays(Json::arrayValue);
    for (int i = 0; i < request->textchats_size(); ++i) {
        Json::Value textItem;
        textItem["message_content"] = request->textchats(i).textchatcontent();
        textItem["message_uuid"] = request->textchats(i).textchatid();
        textArrays.append(textItem);
    }
    returnJson["text_array"] = textArrays;
    std::string jsonString = returnJson.toStyledString();
    session->Send(jsonString, static_cast<int>(MessageType::MESSAGE_CHATSERVER_CHATTEXT_PUSH));
    spdlog::debug("Forwarded text chat message from UID {} to UID {}.", request->from_uid(), to_uid);
    return Status::OK;
}