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

void UserManager::intialContactListAfterLogin(std::vector<std::shared_ptr<AuthResponse> > contactInfoList)
{
    _contactList.clear(); // 清空之前的联系人列表
    _contactList = contactInfoList; // 初始化联系人列表
}

void UserManager::addItemToContactList(std::shared_ptr<AuthResponse> contactInfo)
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

std::shared_ptr<AuthResponse> UserManager::getContactInfo(int uid) const
{
    for (const auto& contact : _contactList) {
        if (contact->getUid() == uid) {
            return contact; // 找到对应的联系人信息
        }
    }
    return nullptr; // 未找到对应的联系人信息
}

void UserManager::intialFriendRequestListAfterLogin(std::vector<std::shared_ptr<FriendListItemInfo>> requestInfoList)
{
    _friendRequestList.clear(); // 清空之前的好友申请列表
    _friendRequestList = requestInfoList; // 初始化好友申请列表
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

std::shared_ptr<SessionInfo> UserManager::getFriendSessionInfoByUid(int uid)
{
    auto it = _friendSessionInfo_map.find(uid);
    if (it != _friendSessionInfo_map.end()) {
        return it.value(); // 返回找到的会话信息
    }
    return nullptr; // 如果没有找到对应的会话信息，返回nullptr
}

void UserManager::appendNewChatMsgToFriendSession(int friend_uid, std::shared_ptr<TextChatData> chatMsg)
{
    auto it = _friendSessionInfo_map.find(friend_uid);
    if (it == _friendSessionInfo_map.end()) {
        qDebug() << "Friend session info not found for uid:" << friend_uid;
        return; // 如果没有找到对应的会话信息，直接返回
    }
    it.value()->_chatHistory.push_back(chatMsg); // 添加新的聊天消息到对应的会话历史
}

void UserManager::initializeSessionList(std::vector<std::shared_ptr<SessionInfo> > sessionList)
{
    _sessionList.clear(); // 清空之前的会话列表
    _sessionList = sessionList; // 初始化会话列表
    _friendSessionInfo_map.clear(); // 清空之前的映射
    for (const auto& session : _sessionList) {
        if (session) {
            _friendSessionInfo_map[session->_peeruid] = session; // 构建好友uid和会话信息的映射
        }
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
