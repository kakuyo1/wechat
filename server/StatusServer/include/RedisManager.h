#pragma once
#include <hiredis/hiredis.h>
#include <string>
#include <iostream>
#include "Singleton.h"
#include <cstring>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include "ConfigIniManager.h"

/*
    暂时未使用
*/

/***************************************************
 * RedisContextPool 用于管理 Redis 连接池
 * 支持多线程安全获取和归还 Redis 上下文
 * pool_size初始值为5
 ****************************************************/

class RedisContextPool {
public:
    RedisContextPool(size_t pool_size, const std::string &host, int port, const std::string &password);
    ~RedisContextPool();
    void Close();
    redisContext* GetContext();
    void returnContext(redisContext* context);
private:
    std::string _host;
    int _port;
    std::atomic<bool> _stopped;
    size_t _pool_size;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::queue<redisContext *> _contexts;
};

/***************************************************
 * 当connect, auth出错时会直接关闭连接
 * 其它方法出错时不会关闭连接
 * 频繁出现(if (context == nullptr || context->err != 0) redisFree(context);
 * else _context_pool->returnContext(context);)
 * 1.可使用
    inline void SafeReturnContext(redisContext* context, RedisContextPool* pool) {
    if (context == nullptr || context->err != 0) redisFree(context);
    else pool->returnContext(context);
    * 2.可使用ExecuteRedisCommand()来封装命令执行和上下文管理
}
 ****************************************************/

class RedisManager : public Singleton<RedisManager> {
    friend class Singleton<RedisManager>; // Allow Singleton to create RedisManager instance
public:
    ~RedisManager();
    bool Set(const std::string& key, const std::string& value);
    bool Get(const std::string& key, std::string& value);
    bool Exists(const std::string& key);
    bool Delete(const std::string& key);
    bool LPush(const std::string& key, const std::string& value);
    bool RPush(const std::string& key, const std::string& value);
    bool LPop(const std::string& key, std::string& value);
    bool RPop(const std::string& key, std::string& value);
    bool HSet(const std::string& key, const std::string& field, const std::string& value); // 简单字符串处理
    bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvalue_len); // 存储二进制、包含\0的数据

    std::string HGet(const std::string& key, const std::string& field);
    void Close();
private:
    RedisManager();
    bool CheckContextValid(redisContext* context) const;
    bool CheckReplyValid(redisReply* reply, int expectedType, const std::string& command, std::string& outValue);
private:
    std::unique_ptr<RedisContextPool> _context_pool;
};