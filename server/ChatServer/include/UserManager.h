#pragma once
#include "Singleton.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <spdlog/spdlog.h>
#include "config.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include "RedisManager.h"
#include "MysqlManager.h"

class CSession;

class UserManager : public Singleton<UserManager> {
    friend class Singleton<UserManager>; // Allow Singleton to create UserManager instance
public:
    ~UserManager();
    std::shared_ptr<CSession> getSessionByUid(int uid);
    void setUidToSession(int uid, std::shared_ptr<CSession> session);
    void removeUidToSession(int uid);
    bool getFullUserInfo(int uid, std::shared_ptr<FullUserInfo> fulluserinfo);
private:
    UserManager() = default;
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;
private:
    /* uid : session*/
    std::unordered_map<int, std::shared_ptr<CSession>> _sessions;
    std::mutex _mutex; // Mutex to protect access to _sessions
};
