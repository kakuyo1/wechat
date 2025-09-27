#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QObject>
#include <memory>
#include "singleton.h"
#include <vector>
#include "contactinfodata.h"
#include "tcpmanager.h"

/*
 * 改类用来管理当前登陆用户的信息，如uid，name，联系人列表，好友申请列表
 */

class UserManager : public QObject, public Singleton<UserManager>, public std::enable_shared_from_this<UserManager>
{
    Q_OBJECT
    friend class Singleton<UserManager>;
public:
    ~UserManager() = default;
    void setName(QString name);
    void setUid(int uid);
    void setEmail(QString email) { _email = email; }
    void setDescription(QString desc) { _desc = desc; }
    void setIconPath(QString iconPath) { _iconPath = iconPath; }
    void setGender(int gender) { _gender = gender;}

     QString getName() const;
    int getUid() const;
    QString getEmail() const { return _email; }
    QString getDescription() const { return _desc; }
    QString getIconPath() const { return _iconPath; }
    int getGender() const { return _gender;};

    void addItemToContactList(std::shared_ptr<AuthResponse> contactInfo); // 当receiver 同意请求成功后才调用
    std::shared_ptr<AuthResponse> getContactInfo(int uid) const; // 根据uid获取联系人信息
    std::vector<std::shared_ptr<AuthResponse>> getContactList() const { return _contactList; } // 获取联系人列表
    void intialContactListAfterLogin(std::vector<std::shared_ptr<AuthResponse>> contactInfoList); // 在登录成功后初始化联系人列表

    void intialFriendRequestListAfterLogin(std::vector<std::shared_ptr<FriendListItemInfo>> requestInfoList); // 在登录成功后初始化好友申请列表
    void addFriendRequest(std::shared_ptr<FriendListItemInfo> requestInfo); // 收到服务端ACK成功后增加好友申请
    std::vector<std::shared_ptr<FriendListItemInfo>> getFriendRequestList() const { return _friendRequestList; } // 获取好友申请列表

    std::shared_ptr<SessionInfo> getFriendSessionInfoByUid(int uid);
    void appendNewChatMsgToFriendSession(int friend_uid, std::shared_ptr<TextChatData> chatMsg); // 添加新的聊天消息到好友会话

    void initializeSessionList(std::vector<std::shared_ptr<SessionInfo>> sessionList); // 初始化会话列表(其实就是复用联系人列表)
private:
    UserManager();
    QString _name;
    QString _email;
    QString _desc;
    QString _iconPath; // 头像路径
    int _gender;
    int _uid;

    std::vector<std::shared_ptr<FriendListItemInfo>> _friendRequestList; // 好友申请列表(self_uid : request_uid)
    std::vector<std::shared_ptr<AuthResponse>> _contactList; // 联系人列表(将服务器返回的FullUserInfo去除password后存储在这里)

    std::vector<std::shared_ptr<SessionInfo>> _sessionList; // 会话列表(将服务器返回的SessionInfo存储在这里TODO)(其实就是复用联系人列表)
    QMap<int, std::shared_ptr<SessionInfo>> _friendSessionInfo_map; // 好友uid和会话信息的映射TODO(其实就是复用联系人列表)
};

#endif // USERMANAGER_H
