#include "ChatServiceImpl.h"
#include "CSession.h"

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
bool ChatServiceImpl::getFullUserInfo(int uid, std::shared_ptr<FullUserInfo> fulluserinfo) {
    // prepare for redis string key
    std::string redis_key = USER_FULLINFO_PREFIX + std::to_string(uid);
    // First Try: get full user info from redis
    auto redis_client = RedisManager::GetInstance();
    std::string fulluserinfo_jsonString = ""; // to be filled by redis
    if (redis_client->Get(redis_key, fulluserinfo_jsonString)) {
        Json::Value root;
        Json::Reader reader;
        reader.parse(fulluserinfo_jsonString, root);
        fulluserinfo->uid = root["uid"].asInt();
        fulluserinfo->gender = root["gender"].asInt();
        fulluserinfo->name = root["name"].asString();
        fulluserinfo->desc = root["desc"].asString();
        fulluserinfo->email = root["email"].asString();
        fulluserinfo->password = root["password"].asString();
        fulluserinfo->icon = root["icon"].asString();
        fulluserinfo->nickname = root["nickname"].asString();
        return true;
    }
    // Second Try: get full user info from Mysql
    fulluserinfo = MysqlManager::GetInstance()->getFullUserInfo(uid);
    if (fulluserinfo == nullptr) {
        spdlog::error("Failed to get full user info from Mysql for uid: {}", uid);
        return false;
    }
    // Store full user info to redis
    Json::Value root;
    root["uid"] = fulluserinfo->uid;
    root["gender"] = fulluserinfo->gender;
    root["name"] = fulluserinfo->name;
    root["desc"] = fulluserinfo->desc;
    root["email"] = fulluserinfo->email;
    root["password"] = fulluserinfo->password;
    root["icon"] = fulluserinfo->icon;
    root["nickname"] = fulluserinfo->nickname;
    Json::StreamWriterBuilder writer;
    std::string fulluserinfo_jsonString_toRedis = Json::writeString(writer, root);
    if (!redis_client->Set(redis_key, fulluserinfo_jsonString_toRedis)) {
        spdlog::error("Failed to set full user info to Redis for uid: {}", uid);
        return false;
    }
    return true;
}