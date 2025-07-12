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
    TYPE_LOGIN_CHAT = 1005
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
    ERROR_PASSWORD_EMAIL_MISMATCH = 11
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

#endif // GLOBAL_H
