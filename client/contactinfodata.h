#ifndef CONTACTINFODATA_H
#define CONTACTINFODATA_H
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

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

// 好友申请列表项信息
class FriendListItemInfo {
public:
    FriendListItemInfo(int self_uid, int request_uid, QString name = "",
                       QString desc = "", QString avatarPath = "", int status = 0);
    ~FriendListItemInfo() = default;
public:
    int _selfUid; // 自己的用户ID
    int _requestUid; // 请求的用户ID
    QString _name; // 请求的用户名
    QString _desc; // 请求的用户描述
    QString _avatarPath; // 请求的用户头像路径
    int _status; // 请求状态(0: 未处理, 1: 已同意, 2: 已拒绝)
};
// 同意添加好友请求
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
// 服务器同意添加好友请求后返回的用户信息
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

/*未使用该类*/
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
    AuthResponse(int peeruid, int peergender, QString peername, QString peernickname,
                 QString peericon, QString peeremail, QString peerdescription);
    ~AuthResponse() = default;
    int getUid() const { return _peeruid; } // 获取对方的用户ID
    QString getName() const { return _peername; } // 获取对方的用户名
    QString getNickname() const { return _peernickname; } // 获取对方的昵称
    QString getIcon() const { return _peericon; } // 获取对方的头像路径
    int getGender() const { return _peergender; } // 获取对方的性别
    QString getEmail() const { return _peeremail; } // 获取对方的邮箱地址
    QString getDescription() const { return _peerdescription; } // 获取对方的描述信息
public:
    int _peeruid;
    int _peergender;
    QString _peername;
    QString _peernickname;
    QString _peericon;
    QString _peeremail; // 邮箱地址
    QString _peerdescription; // 描述
};

struct TextChatData { // 一条消息
    TextChatData(QString message_uuid, QString message_content, int from_uid, int to_uid)
        : _message_uuid(message_uuid), _message_content(message_content), _from_uid(from_uid), _to_uid(to_uid) {}
    QString _message_uuid;
    QString _message_content; // 消息内容
    int _from_uid;
    int _to_uid;
};

struct TextChatBatch { // 一批消息
    TextChatBatch(int from_uid, int to_uid, QJsonArray array) : _from_uid(from_uid), _to_uid(to_uid) {
        for (const auto& value : array) {
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                QString message_uuid = obj.value("message_uuid").toString();
                QString message_content = obj.value("message_content").toString();
                auto message = std::make_shared<TextChatData>(message_uuid, message_content, from_uid, to_uid);
                _messages.push_back(message);
            }
        }
    }
    int _from_uid; // 发送者用户ID
    int _to_uid; // 接收者用户ID
    std::vector<std::shared_ptr<TextChatData>> _messages; // 消息列表
};
// 会话信息，也就是好友信息+聊天历史记录，FriendInfo（这是up主的命名）
class SessionInfo {
public:
    SessionInfo(std::shared_ptr<AuthResponse> peerInfo, std::vector<std::shared_ptr<TextChatData>> chatHistory)
        : _peeruid(0), _peergender(0), _peername(""), _peernickname(""), _peericon(""), _peeremail(""), _peerdescription(""),
          _chatHistory(chatHistory) {
        if (nullptr != peerInfo) {
            _peeruid = peerInfo->getUid();
            _peergender = peerInfo->getGender();
            _peername = peerInfo->getName();
            _peernickname = peerInfo->getNickname();
            _peericon = peerInfo->getIcon();
            _peeremail = peerInfo->getEmail();
            _peerdescription = peerInfo->getDescription();
            _last_message_to_show = chatHistory.empty() ? "" : chatHistory.back()->_message_content;
        }
    }
public:
    int _peeruid;
    int _peergender;
    QString _peername;
    QString _peernickname;
    QString _peericon;
    QString _peeremail; // 邮箱地址
    QString _peerdescription; // 描述
    QString _last_message_to_show; // 最后一条消息内容展现在会话项名称下方
    std::vector<std::shared_ptr<TextChatData>> _chatHistory; // 聊天历史记录
};

#endif // CONTACTINFODATA_H
