#pragma once
#include "MysqlDAO.h"
#include "Singleton.h"
#include <spdlog/spdlog.h>

/*
    用于在LogicSystem进行调用，内含用户注册、登录等功能
    操作MysqlDAO
*/
class MysqlManager : public Singleton<MysqlManager> {
    friend class Singleton<MysqlManager>;
public:
    ~MysqlManager() = default;
    int RegisterUser(const std::string& name, const std::string& email,  const std::string& password);
    bool CheckEmailAndUserMatch(const std::string& name, const std::string& email);
    bool CheckEmailAndPasswordMatch(const std::string& email, const std::string& password, UserInfo& user_info);
    bool ResetPassword(const std::string& name, const std::string& new_password);
private:
    MysqlManager();
    MysqlManager(const MysqlManager&) = delete;
    MysqlManager& operator=(const MysqlManager&) = delete;
private:
    std::unique_ptr<MysqlDAO> _dao;
};