#include "UserManager.h"

UserManager::~UserManager()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.clear();
}

std::shared_ptr<CSession> UserManager::getSessionByUid(int uid)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _sessions.find(uid);
    if (it != _sessions.end()) {
        return it->second; // Return the session associated with the uid
    } else {
        spdlog::warn("No session found for UID {}", uid);
        return nullptr; // Return nullptr if no session is found
    }
}

void UserManager::setUidToSession(int uid, std::shared_ptr<CSession> session)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions[uid] = session; // Store the session associated with the uid
    spdlog::info("Session for UID {} set successfully.", uid);
}

void UserManager::removeUidToSession(int uid)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _sessions.find(uid);
    if (it != _sessions.end()) {
        _sessions.erase(it); // Remove the session associated with the uid
        spdlog::info("Session for UID {} removed successfully.", uid);
    } else {
        spdlog::warn("No session found for UID {} to remove.", uid);
    }
}

bool UserManager::getFullUserInfo(int uid, std::shared_ptr<FullUserInfo> fulluserinfo) {
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

