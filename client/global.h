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
    TYPE_LOGIN_CHAT_SERVER_RESPONSE = 1006
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
    ERROR_EMAIL_DOES_NOT_EXIST = 16
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
    ContactItem, // 联系人列表项
    NonClickableItem // 非点击项
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
