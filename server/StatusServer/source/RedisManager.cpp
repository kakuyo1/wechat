#include "RedisManager.h"
#include <spdlog/spdlog.h>
RedisManager::RedisManager() {
    auto& config = ConfigIniManager::Instance();
    auto& redis_section = config["Redis"];
    std::string host = redis_section["Host"];
    std::string port_str = redis_section["Port"];
    std::string password = redis_section["Password"];
    int port = std::stoi(port_str);
    _context_pool = std::make_unique<RedisContextPool>(5, host.c_str(), port, password.c_str());
}

RedisManager::~RedisManager() {
    Close();
}

bool RedisManager::CheckContextValid(redisContext* context) const
{
    if (context == nullptr || context->err != 0) {
        spdlog::error("Redis context error: {}", (context ? context->errstr : "null context"));
        return false;
    }
    return true;
}

bool RedisManager::CheckReplyValid(redisReply* reply, int expectedType, const std::string& command, std::string& outValue)
{
    if (reply == nullptr) {
        spdlog::error("[RedisManager::CheckReply] Command failed: Redis connection lost or error occurred");
        return false; // 调用者不要再归还这个 context，应直接丢弃
    }

    if (reply->type != expectedType) {
        if (reply->type == REDIS_REPLY_NIL) {
            spdlog::error("[RedisManager::CheckReply] Key not found for command: {}", command);
        } else {
            spdlog::error("[RedisManager::CheckReply] Unexpected reply type: {} for command: {}, message: {}", reply->type, command, (reply->str ? reply->str : "null"));
        }
        freeReplyObject(reply);
        return false;
    }

    // 如果是 STRING 或 STATUS 或 INTEGER，提取值
    if (expectedType == REDIS_REPLY_STRING || expectedType == REDIS_REPLY_STATUS || expectedType == REDIS_REPLY_INTEGER) {
        // 注意INTEGER的值在reply->integer，不在reply->str
        if (expectedType == REDIS_REPLY_INTEGER) {
            outValue = std::to_string(reply->integer);
        } else {
            outValue = reply->str ? reply->str : "";
        }
    }

    freeReplyObject(reply);
    return true;
}

bool RedisManager::Set(const std::string &key, const std::string &value)
{
    auto context = _context_pool->GetContext();

    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    std::string result;
    redisReply *reply = (redisReply *)redisCommand(context, "SET %s %s", key.c_str(), value.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_STATUS, "SET", result) && strcasecmp(result.c_str(), "OK") == 0;
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::Get(const std::string &key, std::string &value)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context, "GET %s", key.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_STRING, "GET", value);
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::Exists(const std::string &key)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    std::string result;
    redisReply *reply = (redisReply *)redisCommand(context, "EXISTS %s", key.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_INTEGER, "EXISTS", result) && result == "1";
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::Delete(const std::string &key)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    std::string result;
    redisReply *reply = (redisReply *)redisCommand(context, "DEL %s", key.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_INTEGER, "DEL", result) && result == "1";
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::LPush(const std::string &key, const std::string &value)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    std::string result;
    redisReply *reply = (redisReply *)redisCommand(context, "LPUSH %s %s", key.c_str(), value.c_str());

    if (!CheckReplyValid(reply, REDIS_REPLY_INTEGER, "LPUSH", result)) {
        redisFree(context); // reply为nullptr，释放连接
        return false;
    }

    try {
        long long len = std::stoll(result);
        _context_pool->returnContext(context);
        return len > 0;
    } catch (const std::exception &e) {
        spdlog::error("Failed to parse LPUSH reply: {}", e.what());
        _context_pool->returnContext(context);
        return false;
    }
}

bool RedisManager::RPush(const std::string &key, const std::string &value)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    std::string result;
    redisReply *reply = (redisReply *)redisCommand(context, "RPUSH %s %s", key.c_str(), value.c_str());

    if (!CheckReplyValid(reply, REDIS_REPLY_INTEGER, "RPUSH", result)) {
        redisFree(context); // reply为nullptr，释放连接
        return false;
    }

    try {
        long long len = std::stoll(result);
        _context_pool->returnContext(context);
        return len > 0;
    } catch (const std::exception &e) {
        _context_pool->returnContext(context);
        spdlog::error("Failed to parse RPUSH reply: {}", e.what());
        return false;
    }
}

bool RedisManager::LPop(const std::string &key, std::string &value)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context, "LPOP %s", key.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_STRING, "LPOP", value);
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::RPop(const std::string &key, std::string &value)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context, "RPOP %s", key.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_STRING, "RPOP", value);
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::HSet(const std::string &key, const std::string &field, const std::string &value)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        return false;
    }

    std::string result;
    redisReply *reply = (redisReply *)redisCommand(context, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
    bool ok = CheckReplyValid(reply, REDIS_REPLY_INTEGER, "HSET", result);
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

bool RedisManager::HSet(const char *key, const char *hkey, const char *hvalue, size_t hvalue_len)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return false;
    }

    const char* argv[4];
    size_t argvlen[4];

    argv[0] = "HSET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    argv[3] = hvalue;
    argvlen[3] = hvalue_len;

    redisReply *reply = (redisReply*)redisCommandArgv(context, 4, argv, argvlen);

    std::string result;
    bool ok = CheckReplyValid(reply, REDIS_REPLY_INTEGER, "HSET", result);
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return ok;
}

std::string RedisManager::HGet(const std::string &key, const std::string &field)
{
    auto context = _context_pool->GetContext();
    if (!CheckContextValid(context)) {
        _context_pool->returnContext(context);
        return "";
    }

    const char* argv[3];
    size_t argvlen[3];

    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.size();
    argv[2] = field.c_str();
    argvlen[2] = field.size();

    redisReply* reply = (redisReply*)redisCommandArgv(context, 3, argv, argvlen);

    std::string value;
    if (!CheckReplyValid(reply, REDIS_REPLY_STRING, "HGET", value)) {
        if (context == nullptr || context->err != 0) redisFree(context);
        else _context_pool->returnContext(context);
        return "";
    }
    if (context == nullptr || context->err != 0) redisFree(context);
    else _context_pool->returnContext(context);
    return value;
}

void RedisManager::Close() {
    _context_pool->Close();
}

RedisContextPool::RedisContextPool(size_t pool_size, const std::string &host, int port, const std::string &password) :
    _pool_size(pool_size),
    _host(host),
    _port(port),
    _stopped(false)
{
    size_t created = 0;
    int retry = 0;
    while (created < pool_size && retry < 10 * pool_size) {
        redisContext * context = redisConnect(host.c_str(), port);
        retry++;
        if (!context || context->err) {
            if (context) redisFree(context); // 创建失败时释放资源
            continue; // 连接失败，重试
        }

        auto reply = (redisReply*)redisCommand(context, "AUTH %s", password.c_str());
        if (!reply || reply->type == REDIS_REPLY_ERROR) {
            spdlog::error("认证失败");
            if (reply) freeReplyObject(reply);
            continue;
        }
        freeReplyObject(reply);
        _contexts.push(context);
        created++;
    }
    if (created < pool_size) {
        spdlog::error("Redis连接池初始化不足!期望: {}, 实际: {}", pool_size, created);
    }
}

RedisContextPool::~RedisContextPool()
{
    std::lock_guard<std::mutex> lock(_mutex);
    Close();
    while (!_contexts.empty()) {
        redisContext *context = _contexts.front();
        _contexts.pop();
        if (context) {
            redisFree(context); // 释放每个redisContext
        }
    }
}
void RedisContextPool::Close() {
    _stopped.store(true);
    _cv.notify_all();
}
redisContext *RedisContextPool::GetContext()
{
    std::unique_lock<std::mutex> lock(_mutex);
    // 等待直到有可用的上下文或池已关闭
    _cv.wait(lock, [this]() {
        return !_contexts.empty() || _stopped.load();
    });

    if (_stopped.load()) {
        return nullptr; // 如果池已关闭，返回nullptr
    }

    redisContext *context = _contexts.front();
    _contexts.pop();
    return context;
}

void RedisContextPool::returnContext(redisContext *context)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stopped.load()) {
        redisFree(context); // 如果池已关闭，释放上下文
        return;
    }
    _contexts.push(context); // 将上下文放回队列
    _cv.notify_one(); // 通知等待的线程有新的上下文可用
}
