#pragma once
#include <string>
#include <functional>
enum class ErrorCodes {
    SUCCESS = 0,
    ERROR_JSON_PARSE = 1,
    ERROR_RPC = 2,
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
    ERROR_SEARCH_FAILED_BY_NAME = 19
};

const std::string code_prefix = "code_"; // Prefix for verify code keys in Redis

enum class MessageType {
    MESSAGE_CHATSERVER_LOGIN_AUTH = 1005,
    MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE = 1006,
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

struct UserInfo {
    int uid;
    std::string name;
    std::string email;
    std::string password;
};

struct FullUserInfo {
    int uid;
    int gender;
    std::string name;
    std::string nickname;
    std::string email;
    std::string password;
    std::string icon; // User's icon URL or path
    std::string desc;
};

#define MAX_MESSAGE_LENGTH 1024 // Maximum message length
#define DATA_HEADER_SIZE 4 // Size of the data header (2 bytes for message type, 2 bytes for message length)
#define MAX_SEND_QUEUE 1000 // Maximum size of the send queue
#define MAX_RECEIVE_QUEUE 1000 // Maximum size of the receive queue
#define DATA_MESSAGE_TYPE_SIZE 2
#define DATA_MESSAGE_LENGTH 2


#define SERVER_IP_PREFIX "server_ip_" // Prefix for server IP keys in Redis
#define USER_TOKEN_PREFIX "user_token_" // Prefix for user token keys in Redis
#define USER_FULLINFO_PREFIX "user_fullinfo_" // Prefix for user full info keys in Redis
#define USER_SESSION_PREFIX "user_session_" // Prefix for user session keys in Redis
#define SERVER_LOGIN_COUNT "server_login_count" // server login count keys in Redis


class Defer {
public:
    Defer(std::function<void()> func) : _func(std::move(func)) {}
    ~Defer() {
        if (_func) {
            _func();
        }
    }

private:
    std::function<void()> _func;
};