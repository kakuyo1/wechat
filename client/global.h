#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QStyle>

// repolish the stylesheet dynamically
extern std::function<void (QWidget*)> repolish;

extern QString gate_url_prefix;

// Request Types
enum class RequestType {
    TYPE_GET_VERIFY_CODE = 1001,
    TYPE_REGISTER = 1002,
    TYPE_RESETPASSWORD = 1003,
    TYPE_LOGIN = 1004,
    TYPE_LOGIN_CHAT = 1005,
    TYPE_LOGIN_CHAT_SERVER_RESPONSE = 1006,
    MESSAGE_CLIENT_ADDFRIEND_REQUEST = 1007, // 客户端 → 服务端：发起好友申请
    MESSAGE_CHATSERVER_ADDFRIEND_ACK = 1008, // 服务端 → 发起方客户端：处理结果的反馈
    MESSAGE_CHATSERVER_ADDFRIEND_PUSH = 1009, // 服务端 → 接收方客户端：转发好友申请通知
    MESSAGE_CLIENT_AUTHFRIEND_REQUEST = 1010, // 客户端 → 服务端：处理好友验证
    MESSAGE_CHATSERVER_AUTHFRIEND_ACK = 1011, // 服务端 → 发起方客户端：处理验证结果的反馈
    MESSAGE_CHATSERVER_AUTHFRIEND_PUSH = 1012, // 服务端 → 接收方客户端：转发好友验证处理结果通知
    MESSAGE_CLIENT_CHATTEXT_REQUEST = 1013, // 客户端 → 服务端：发送聊天文本消息
    MESSAGE_CHATSERVER_CHATTEXT_ACK = 1014, // 服务端 → 发起方客户端：处理聊天文本消息的反馈
    MESSAGE_CHATSERVER_CHATTEXT_PUSH = 1015, // 服务端 → 接收方客户端：转发聊天文本消息
    MESSAGE_CLIENT_SEARCH_USER_REQUEST = 1016, // 客户端 → 服务端：搜索用户
    MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE = 1017 // 服务端 → 客户端：搜索用户结果
};

// Error Codes
enum class ErrorCode {
    SUCCESS = 0,
    ERROR_JSON_PARSE = 1,
    ERROR_NETWORK = 2,
    ERROR_REDIS = 3,
    ERROR_MYSQL = 4,
    ERROR_EXISTING_USER = 5,
    ERROR_EXISTING_EMAIL = 6,
    ERROR_ENCRPTION = 7,
    ERROR_USER_EMAIL_MISMATCH = 8,
    ERROR_INVALID_VERIFY_CODE = 9,
    ERROR_MISSING_FIELDS = 10,
    ERROR_PASSWORD_EMAIL_MISMATCH = 11,
    ERROR_UID_NOT_FOUND = 12,
    ERROR_TOKEN_MISMATCH = 13,
    ERROR_INVALID_AUTH_PARAMETERS = 14,
    ERROR_USER_ALREADY_ONLINE = 15,
    ERROR_EMAIL_DOES_NOT_EXIST = 16,
    ERROR_USER_OFFLINE = 17,
    ERROR_SEARCH_FAILED_BY_UID = 18,
    ERROR_SEARCH_FAILED_BY_NAME = 19,
    ERROR_NO_FRIENDREQUEST_RECORD = 20,
    ERROR_NO_FRIEND_RECORD = 21,
    ERROR_FRIEND_REQUEST_EXISTS = 22, // New error code for existing friend request
    ERROR_ALREADY_FRIENDS = 23 // New error code for already friends
};

// Modules
enum class Module {
    MODULE_LOGIN = 1,
    MODULE_REGISTER = 2,
    MODULE_RESETPASSWORD = 3
};

struct serverInfo {
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

struct messageInfo {
    QString messageType;
    QString messageContent; // passage, url
    QPixmap image;
};

// interface modules
enum class ChatUIMode {
    SearchMode, // 搜索页面
    ChatMode, // 聊天页面
    ContactMode // 联系人页面
};

// QListWidget customizied item types
enum class ListItemType {
    ChatItem, // 聊天列表项
    SearchItem, // 搜索列表项
    AddContactTipItem, // 添加联系人提示项
    ContactItem, // 联系人列表项
    AddContactItem, // 添加联系人项
    ContactGroupTipItem, // 联系人分组提示项
    NonClickableItem, // 非点击项
    FriendRequestItem // 好友请求项
};

// roles for message
enum class MessageRole {
    OTHER, // 其他人发来的消息
    SELF // 自己发出的消息
};

// 测试用例
const std::vector<QString>  strs ={"hello world !",
                                   "nice to meet u",
                                   "New year，new life",
                                   "You have to love yourself",
                                   "My love is written in the wind ever since the whole world is you"};

const std::vector<QString> heads = {
    ":/images/head_1.jpg",
    ":/images/head_2.jpg",
    ":/images/head_3.jpg",
    ":/images/head_4.jpg",
    ":/images/head_5.jpg"
};

const std::vector<QString> names = {
    "HanMeiMei",
    "Lily",
    "Ben",
    "Androw",
    "Max",
    "Summer",
    "Candy",
    "Hunter"
};
#endif // GLOBAL_H
