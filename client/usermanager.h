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

    void addItemToContactList(std::shared_ptr<SearchInfo> contactInfo); // 当receiver 同意请求成功后才调用
    std::shared_ptr<SearchInfo> getContactInfo(int uid) const; // 根据uid获取联系人信息
    std::vector<std::shared_ptr<SearchInfo>> getContactList() const { return _contactList; } // 获取联系人列表

    void addFriendRequest(std::shared_ptr<FriendListItemInfo> requestInfo); // 在好友申请页面点击发送成功后调用
    std::vector<std::shared_ptr<FriendListItemInfo>> getFriendRequestList() const { return _friendRequestList; } // 获取好友申请列表
private:
    UserManager();
    QString _name;
    QString _email;
    QString _desc;
    QString _iconPath; // 头像路径
    int _gender;
    int _uid;

    std::vector<std::shared_ptr<FriendListItemInfo>> _friendRequestList; // 好友申请列表(self_uid : request_uid)
    std::vector<std::shared_ptr<SearchInfo>> _contactList; // 联系人列表(直接将搜索后拿到的服务器返回的FullUserInfo存入这里)
};

#endif // USERMANAGER_H
