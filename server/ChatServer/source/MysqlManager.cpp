#include "MysqlManager.h"

MysqlManager::MysqlManager() :
    _dao(std::make_unique<MysqlDAO>())
{

}

int MysqlManager::RegisterUser(const std::string& name, const std::string& email,  const std::string& password) {
    return _dao->RegisterUser(name, email, password);
}

bool MysqlManager::CheckEmailAndUserMatch(const std::string& name, const std::string& email) {
    return _dao->CheckEmailAndUserMatch(name, email);
}

bool MysqlManager::CheckEmailAndPasswordMatch(const std::string &email, const std::string &password, UserInfo& user_info)
{
    return _dao->CheckEmailAndPasswordMatch(email, password, user_info);
}

bool MysqlManager::ResetPassword(const std::string& name, const std::string& new_password) {
    return _dao->ResetPassword(name, new_password);
}