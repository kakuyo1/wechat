#ifndef CONTACTINFODATA_H
#define CONTACTINFODATA_H
#include <QString>

class SearchInfo {
public:
    SearchInfo(int uid, QString name, QString nickname, QString description, int gender);
    ~SearchInfo() = default;
    int getUid() const;
    QString getName() const;
    QString getNickname() const;
    QString getDescription() const;
    int getGender() const;
private:
    int _uid; // 用户ID
    QString _name; // 用户名
    QString _nickname; // 昵称
    QString _description; // 描述
    int _gender; // 性别
};

class AddContactRequest {
public:
    AddContactRequest(int fromUid, QString fromName, QString fromNickname,
                      QString fromAvatarPath, QString fromDescription, int fromGender);
    ~AddContactRequest() = default;
public:
    int _fromUid; // 发送请求的用户ID
    QString _fromName; // 发送请求的用户名
    QString _fromNickname; // 发送请求的用户昵称
    QString _fromAvatarPath; // 发送请求的用户头像路径
    QString _fromDescription; // 发送请求的用户描述
    int _fromGender; // 发送请求的用户性别
};

class RequestInfo {
public:
    RequestInfo(int uid, QString name, QString nickname, QString avatarPath,
                QString description, int gender, int status);
    RequestInfo(std::shared_ptr<AddContactRequest> addContactRequest);
public:
    int _uid;
    QString _name;
    QString _nickname;
    QString _description;
    QString _avatarPath;
    int _gender;
    int _status; // 请求状态
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
