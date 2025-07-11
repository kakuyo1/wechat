#pragma once
#include <thread>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <string>
#include <chrono>
#include "config.h"
#include "ConfigIniManager.h"
#include <bcrypt/bcrypt.h>
#include <spdlog/spdlog.h>

struct UserInfo {
    int uid;
    std::string name;
    std::string email;
    std::string password; // Encrypted password
};

class MysqlConnection {
public:
    MysqlConnection(sql::Connection* con, int64_t last_used_time);
    std::unique_ptr<sql::Connection> con_;
    int64_t last_used_time_;
};

class MysqlConnectionPool {
public:
    MysqlConnectionPool(size_t pool_size, const std::string& host, const std::string& user, const std::string& password, const std::string& schema);
    ~MysqlConnectionPool();

    std::unique_ptr<MysqlConnection> GetConnection();
    void ReturnConnection(std::unique_ptr<MysqlConnection> conn);
    void Stop();

    // 检查连接是否有效
    bool CheckConnectionValid(const MysqlConnection& conn) const;

    void CheckConnections();// 定期检查连接的有效性
    size_t GetPoolSize() const;

    int64_t GetNowTime() const;
private:
    std::queue<std::unique_ptr<MysqlConnection>> _connections;
    mutable std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _stopped;
    size_t _pool_size;
    std::thread _check_thread;
    std::string _host;
    std::string _user;
    std::string _password;
    std::string _schema; // 数据库名
};

class MysqlDAO {
public:
    MysqlDAO();
    ~MysqlDAO();
    int RegisterUser(const std::string& name, const std::string& email, const std::string& password);
    bool CheckEmailAndUserMatch(const std::string& name, const std::string& email);
    bool CheckEmailAndPasswordMatch(const std::string& email, const std::string& password, UserInfo& user_info);
    bool ResetPassword(const std::string& name, const std::string& new_password);
private:
    std::unique_ptr<MysqlConnectionPool> _pool;
};