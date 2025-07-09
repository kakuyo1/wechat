#include "MysqlManager.h"

MysqlManager::MysqlManager() :
    _dao(std::make_unique<MysqlDAO>())
{

}

int MysqlManager::RegisterUser(const std::string& name, const std::string& email,  const std::string& password) {
    return _dao->RegisterUser(name, email, password);
}