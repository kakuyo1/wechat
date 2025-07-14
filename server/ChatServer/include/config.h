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
    ERROR_EMAIL_DOES_NOT_EXIST = 16
};

const std::string code_prefix = "code_"; // Prefix for verify code keys in Redis

enum class MessageType {
    MESSAGE_CHATSERVER_LOGIN_AUTH = 1005,
    MESSAGE_CHATSERVER_LOGIN_AUTH_RESPONSE = 1006
};

struct UserInfo {
    int uid;
    std::string name;
    std::string email;
    std::string password;
};

#define MAX_MESSAGE_LENGTH 1024 // Maximum message length
#define DATA_HEADER_SIZE 4 // Size of the data header (2 bytes for message type, 2 bytes for message length)
#define MAX_SEND_QUEUE 1000 // Maximum size of the send queue
#define MAX_RECEIVE_QUEUE 1000 // Maximum size of the receive queue
#define DATA_MESSAGE_TYPE_SIZE 2
#define DATA_MESSAGE_LENGTH 2

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