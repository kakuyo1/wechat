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


#define USER_IP_PREFIX "user_ip_" // Prefix for user IP keys in Redis
#define USER_TOKEN_PREFIX "user_token_" // Prefix for user token keys in Redis
#define USER_FULLINFO_PREFIX "user_fullinfo_" // Prefix for user full info keys in Redis
#define USER_SESSION_PREFIX "user_session_" // Prefix for user session keys in Redis
#define SERVER_LOGIN_COUNT "server_login_count" //server login count keys in Redis

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