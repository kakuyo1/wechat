#ifndef CONTACTINFODATA_H
#define CONTACTINFODATA_H
#include <QString>


/*SearchInfo用于用户通过uid/name在搜索栏搜索用户时使用*/
class SearchInfo {
public:
    SearchInfo(int uid, QString name, QString nickname, QString description, int gender, QString avatarPath, QString emial);
    ~SearchInfo() = default;
    int getUid() const;
    QString getName() const;
    QString getNickname() const;
    QString getDescription() const;
    int getGender() const;
    QString getAvatarPath() const { return _avatarPath; } // 获取头像路径
    QString getEmail() const { return _emial; } // 获取邮箱地址
private:
    int _uid; // 用户ID
    QString _name; // 用户名
    QString _nickname; // 昵称
    QString _description; // 描述
    int _gender; // 性别
    QString _avatarPath; // 头像路径
    QString _emial; // 邮箱地址
};

class FriendListItemInfo {
public:
    FriendListItemInfo(int self_uid, int request_uid);
    ~FriendListItemInfo() = default;
public:
    int _selfUid; // 自己的用户ID
    int _requestUid; // 请求的用户ID
};

class AddContactRequest {
public:
    AddContactRequest(int fromUid, QString fromName, QString fromNickname,
                      QString fromAvatarPath, QString fromDescription, int fromGender, QString fromEmail);
    ~AddContactRequest() = default;
public:
    int _fromUid; // 发送请求的用户ID
    int _fromGender; // 发送请求的用户性别
    QString _fromName; // 发送请求的用户名
    QString _fromNickname; // 发送请求的用户昵称(等待对方设置，这是别人看到的你的昵称)
    QString _fromAvatarPath; // 发送请求的用户头像路径
    QString _fromDescription; // 发送请求的用户描述
    QString _fromEmail; // 发送请求的用户邮箱地址
};

class AddContactResponse {
public:
    AddContactResponse(int Uid, QString Name, QString Nickname,
                      QString AvatarPath, QString Description, int Gender, QString Email);
    ~AddContactResponse() = default;
public:
    int _uid;
    int _gender;
    QString _name;
    QString _nickname;
    QString _description;
    QString _avatarPath; // 头像路径
    QString _email; // 邮箱地址
};

class RequestInfo {
public:
    RequestInfo(int uid, QString name, QString nickname, QString avatarPath,
                QString description, int gender, int status);
    RequestInfo(std::shared_ptr<AddContactRequest> addContactRequest);
public:
    int _uid;
    int _gender;
    QString _name;
    QString _nickname;
    QString _description;
    QString _avatarPath;
    QString _email;
    int _status; // 请求状态
};

class AuthRequest {
public:
    AuthRequest(int uid, QString name, QString nickname, QString avatarPath,
                int gender);
    ~AuthRequest() = default;
public:
    int _uid;
    QString _name;
    QString _nickname;
    QString _icon;
    int _gender;
};

class AuthResponse {
public:
    AuthResponse(int peeruid, QString peername, QString peernickname,
                 QString peericon, int peergender);
    ~AuthResponse() = default;
public:
    int _peeruid;
    QString _peername;
    QString _peernickname;
    QString _peericon;
    int _peergender;
};


class UserInfo {
public:
    UserInfo(int uid, QString name, QString avatarPath);
    ~UserInfo() = default;
private:
    int _uid;
    QString _name;
    QString _nickname;
    QString _description;
    QString _avatarPath; // 头像路径
    int _gender;
};

#endif // CONTACTINFODATA_H
