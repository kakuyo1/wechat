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
    ERROR_ENCRPTION = 7
};

const std::string code_prefix = "code_"; // Prefix for verify code keys in Redis

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