#include "usermanager.h"

void UserManager::setName(QString name)
{
    _name = name;
}

void UserManager::setUid(int uid)
{
    _uid = uid;
}


void UserManager::setToken(QString token)
{
    _token = token;
}

int UserManager::getUid() const
{
    return _uid;
}

QString UserManager::getName() const
{
    return _name;
}

std::vector<std::shared_ptr<RequestInfo> > UserManager::getFriendRequestList() const
{
    return _friendRequestList;
}

UserManager::UserManager() : _name(""), _token(""), _uid(0) {}
