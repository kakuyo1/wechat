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
    TYPE_REGISTER = 1002
};

// Error Codes
enum class ErrorCode {
    SUCCESS = 0,
    ERROR_JSON_PARSE = 1,
    ERROR_NETWORK = 2,
    ERROR_EXISTING_USER = 5,
    ERROR_EXISTING_EMAIL = 6
};

// Modules
enum class Module {
    MODULE_LOGIN = 1,
    MODULE_REGISTER = 2
};

#endif // GLOBAL_H
