#include "usermanager.h"

void UserManager::setName(QString name)
{
    _name = name;
}

void UserManager::setUid(int uid)
{
    _uid = uid;
}


int UserManager::getUid() const
{
    return _uid;
}

void UserManager::addItemToContactList(std::shared_ptr<SearchInfo> contactInfo)
{
    if (contactInfo) {
        // 检查是否已经存在该联系人
        for (const auto& info : _contactList) {
            if (info->getUid() == contactInfo->getUid()) {
                return; // 已经存在，直接返回
            }
        }
        _contactList.push_back(contactInfo); // 添加到联系人列表
    }
}

std::shared_ptr<SearchInfo> UserManager::getContactInfo(int uid) const
{
    for (const auto& contact : _contactList) {
        if (contact->getUid() == uid) {
            return contact; // 找到对应的联系人信息
        }
    }
    return nullptr; // 未找到对应的联系人信息
}

void UserManager::addFriendRequest(std::shared_ptr<FriendListItemInfo> requestInfo)
{
    if (requestInfo) {
        // 检查是否已经存在该好友申请
        for (const auto& info : _friendRequestList) {
            if (info->_selfUid == requestInfo->_selfUid && info->_requestUid == requestInfo->_requestUid) {
                return; // 已经存在，直接返回
            }
        }
        _friendRequestList.push_back(requestInfo); // 添加到好友申请列表
    }
}

QString UserManager::getName() const
{
    return _name;
}

UserManager::UserManager() : _name(""),  _uid(0) {
    // 连接添加新的好友请求信号到槽函数
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_add_newFriendListItem, this, [this](std::shared_ptr<FriendListItemInfo> requestInfo) {
        addFriendRequest(requestInfo); // 添加好友申请到列表
    });
}
