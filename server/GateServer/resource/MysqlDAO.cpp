#include "MysqlDAO.h"
#include "MysqlManager.h"
#include <spdlog/spdlog.h>
MysqlConnection::MysqlConnection(sql::Connection *con, int64_t last_used_time) :
    con_(std::unique_ptr<sql::Connection>(con)), last_used_time_(last_used_time)
{
    if (con_ == nullptr) {
        throw std::runtime_error("MySQL connection is null");
    }
}


MysqlConnectionPool::MysqlConnectionPool(size_t pool_size, const std::string& host, const std::string& user, const std::string& password, const std::string& schema) :
    _pool_size(pool_size),
    _host(host),
    _user(user),
    _password(password),
    _schema(schema),
    _stopped(false)
{
    if (_pool_size == 0) {
        throw std::invalid_argument("MySQL connection pool size must be > 0");
    }

    try{
        for (size_t i = 0; i < _pool_size; ++i) {
            // get driver instance
            sql::mysql::MySQL_Driver * driver = sql::mysql::get_mysql_driver_instance();
            if (driver == nullptr) {
                throw std::runtime_error("Failed to get MySQL driver instance");
            }
            // create a new connection
            sql::Connection * con = driver->connect(_host, _user, _password);
            if (con == nullptr) {
                throw std::runtime_error("Failed to create MySQL connection");
            }
            con->setSchema(_schema);

            // get current time in seconds since 1970-01-01
            auto now_from_epoch_seconds = GetNowTime();

            // create a new MysqlConnection and add it to the pool
            _connections.push(std::make_unique<MysqlConnection>(con, now_from_epoch_seconds));
        }
        // start a thread to check connections periodically
        if (!_check_thread.joinable()) {
            _check_thread = std::thread([this]() {
                while (!_stopped.load()) {
                    CheckConnections();
                    std::this_thread::sleep_for(std::chrono::seconds(60)); // Check every 60 seconds
                }
            });
        }
    } catch (sql::SQLException& e) {
        spdlog::error("Mysql Connection Pool initialization failed: {}", e.what());
        while (!_connections.empty()) _connections.pop();
        throw;
    }
}

void MysqlConnectionPool::Stop()
{
    _stopped.store(true);
    _cv.notify_all();
    if (_check_thread.joinable()) {
        _check_thread.join();
    }
}

MysqlConnectionPool::~MysqlConnectionPool() {
    Stop();
    std::lock_guard<std::mutex> lock(_mutex);
    while (!_connections.empty()) {
        _connections.pop(); // Clear the pool
    }
}

void MysqlConnectionPool::CheckConnections() {
    std::lock_guard<std::mutex> lock(_mutex);
    // get the connections in turns check if they are used in the last 5 minutes,
    // if not, check if they are still valid, otherwise, continue
    int pool_size = _connections.size();
    auto now_from_epoch_seconds = GetNowTime();

    for(size_t i = 0; i < pool_size; ++i) {
        auto conn = std::move(_connections.front());
        _connections.pop();

        Defer defer([this, &conn](){
            if (conn) {
                _connections.push(std::move(conn)); // Return the connection to the pool
            }
        });

        if (now_from_epoch_seconds - conn->last_used_time_ < 300) {
            continue; // If the connection was used in the last 5 minutes, skip it
        }

        // If the connection was not used in the last 5 minutes, check if it is still valid
        try {
            if (CheckConnectionValid(*conn)) {
                // If the connection is valid, update the last used time
                conn->last_used_time_ = now_from_epoch_seconds;
            } else {
                // If the connection is not valid, create a new connection
                sql::mysql::MySQL_Driver * driver = sql::mysql::get_mysql_driver_instance();
                if (driver == nullptr) {
                    throw std::runtime_error("Failed to get MySQL driver instance");
                }
                sql::Connection * new_con = driver->connect(_host, _user, _password);
                if (new_con == nullptr) {
                    throw std::runtime_error("Failed to create MySQL connection");
                }
                new_con->setSchema(_schema);
                conn->con_.reset(new_con); // Replace the old connection with the new one
                conn->last_used_time_ = now_from_epoch_seconds;
            }
        } catch (sql::SQLException& e) {
            spdlog::error("Error checking MySQL connection: {}", e.what());
        }
    }
}

size_t MysqlConnectionPool::GetPoolSize() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _connections.size();
}

int64_t MysqlConnectionPool::GetNowTime() const
{
    // steady_clock
    auto now = std::chrono::steady_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    return now_seconds;
}

bool MysqlConnectionPool::CheckConnectionValid(const MysqlConnection& conn) const{
    // use select 1(heart beat) to check if the connection is valid
    try {
        std::unique_ptr<sql::Statement> stmt(conn.con_->createStatement());
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery("SELECT 1"));
            if (result && result->next()) {
                return true;
            } else {
            return false;
}
    } catch (sql::SQLException& e) {
        spdlog::error("MySQL connection error: {}", e.what());
        return false; // If any exception occurs, the connection is not valid
    }
}

std::unique_ptr<MysqlConnection> MysqlConnectionPool::GetConnection() {
    std::unique_lock<std::mutex> lock(_mutex);

    _cv.wait(lock, [this](){
        return !_connections.empty() || _stopped.load();
    });

    if (_stopped.load()) {
        return nullptr;
    }

    auto conn = std::move(_connections.front());
    _connections.pop();
    return conn;
}

void MysqlConnectionPool::ReturnConnection(std::unique_ptr<MysqlConnection> conn) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_connections.size() >= _pool_size) {
        spdlog::warn("Connection pool is full, returning connection will be ignored.");
        return;
    }
    if (_stopped.load()) {
        return;
    }
    _connections.push(std::move(conn));
    _cv.notify_one(); // Notify one waiting thread that a connection is available
}

MysqlDAO::MysqlDAO()
{
    auto& config = ConfigIniManager::Instance();
    auto& section = config["Mysql"];

    const auto& host = section["Host"];
    const auto& port = section["Port"];
    const auto& password = section["Password"];
    const auto& user = section["Uses"];
    const auto& schema = section["Schema"];
    _pool = std::make_unique<MysqlConnectionPool>(5, host + ":" + port, user, password, schema);
}

MysqlDAO::~MysqlDAO()
{
    _pool->Stop();
}

int MysqlDAO::RegisterUser(const std::string &name, const std::string &email, const std::string &password)
{
    auto con = _pool->GetConnection();
    try {
        if (con == nullptr) {
            return -4;
        }
        // prepare for Stored Procedure
        std::unique_ptr <sql::PreparedStatement> stmt(con->con_->prepareStatement("CALL register_user(?, ?, ?, @result)"));
        // set parameters
        stmt->setString(1, name);
        stmt->setString(2, email);
        stmt->setString(3, password);
        // excute Stored Procedure
        stmt->execute();
        while (stmt->getMoreResults()) { } // Clear any remaining results
        // using "select @result as result" to get stored procedure result(session variable)
        std::unique_ptr<sql::Statement> get_result(con->con_->createStatement());
        std::unique_ptr<sql::ResultSet> result(get_result->executeQuery("select @result as result"));
        if (result->next()) { // Stored Procedure execute success
            int result_code = result->getInt("result");
            _pool->ReturnConnection(std::move(con));
            return result_code;
        }
        // Stored Procedure execute failed
        _pool->ReturnConnection(std::move(con));
        return -5;
    } catch(sql::SQLException& e) {
        _pool->ReturnConnection(std::move(con));
        spdlog::error("SQLException: {}", e.what());
        spdlog::error("(MYSQL error code: {}, SQLState: {})", e.getErrorCode(), e.getSQLState());
        return -3;
    }
}

bool MysqlDAO::CheckEmailAndUserMatch(const std::string &name, const std::string &email)
{
    auto con = _pool->GetConnection();
    try {
        if (con == nullptr) {
            return false;
        }
        std::unique_ptr<sql::PreparedStatement> stmt(con->con_->prepareStatement("select count(*) from user where name = ? and email = ?"));
        stmt->setString(1, name);
        stmt->setString(2, email);
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());
        if (result->next()) {
            int count = result->getInt(1);
            if (count > 0) {
                _pool->ReturnConnection(std::move(con));
                return true; // Email exists for the user
            } else {
                _pool->ReturnConnection(std::move(con));
                return false; // Email does not exist for the user
            }
        }
        // If no result is returned, assume email does not exist
        _pool->ReturnConnection(std::move(con));
        return false;
    } catch(sql::SQLException& e) {
        _pool->ReturnConnection(std::move(con));
        spdlog::error("SQLException: {}", e.what());
        spdlog::error("(MYSQL error code: {}, SQLState: {})", e.getErrorCode(), e.getSQLState());
        return false;
    }
}

bool MysqlDAO::CheckEmailAndPasswordMatch(const std::string& email, const std::string& password, UserInfo& user_info) {
    auto con = _pool->GetConnection();
    try {
        if (con == nullptr) {
            return false;
        }

        Defer defer([this, &con](){
            _pool->ReturnConnection(std::move(con));
        });

        // Prepare the SQL statement to get the encrypted password
        std::unique_ptr<sql::PreparedStatement> stmt0(con->con_->prepareStatement("select * from user where email = ?"));
        stmt0->setString(1, email);
        std::unique_ptr<sql::ResultSet> result0(stmt0->executeQuery());
        if (!result0->next()) {
            // If no result is returned, assume email does not exist
            result0.reset(); // Reset the result set to release resources
            return false;
        }
        std::string stored_password = result0->getString("password");

        if (bcrypt_checkpw(password.c_str(), stored_password.c_str()) != 0) {
            // Password does not match
            return false;
        }
        // If the password matches, retrieve user info
        user_info.uid = result0->getInt("uid");
        user_info.name = result0->getString("name");
        user_info.email = result0->getString("email");
        user_info.password = stored_password; // Store the encrypted password
        return true; // Email and password match

    } catch (sql::SQLException& e) {
        spdlog::error("SQLException: {}", e.what());
        spdlog::error("(MYSQL error code: {}, SQLState: {})", e.getErrorCode(), e.getSQLState());
        return false; // Error occurred while checking email and password
    }
}


bool MysqlDAO::ResetPassword(const std::string &name, const std::string &new_password)
{
    auto con = _pool->GetConnection();
    try {
        if (con == nullptr) {
            return false;
        }
        std::unique_ptr<sql::PreparedStatement> stmt(con->con_->prepareStatement("UPDATE user SET password = ? WHERE name = ?"));
        stmt->setString(1, new_password);
        stmt->setString(2, name);
        auto result = stmt->executeUpdate();
        if (result == 0) {
            // No rows updated, which means the user does not exist
            _pool->ReturnConnection(std::move(con));
            return false;
        }
        _pool->ReturnConnection(std::move(con));
        return true;
    } catch(sql::SQLException& e) {
        _pool->ReturnConnection(std::move(con));
        spdlog::error("SQLException: {}", e.what());
        spdlog::error("(MYSQL error code: {}, SQLState: {})", e.getErrorCode(), e.getSQLState());
        return false; // Password reset failed
    }
}

UserInfo MysqlDAO::GetUserInfo(int uid) {
    auto con = _pool->GetConnection();
    try {
        if (con == nullptr) {
            return UserInfo(); // Return empty UserInfo on failure
        }
        std::unique_ptr<sql::PreparedStatement> stmt(con->con_->prepareStatement("SELECT * FROM user WHERE uid = ?"));
        stmt->setInt(1, uid);
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());
        if (result->next()) {
            UserInfo user_info;
            user_info.uid = result->getInt("uid");
            user_info.name = result->getString("name");
            user_info.email = result->getString("email");
            user_info.password = result->getString("password"); // Store the encrypted password
            _pool->ReturnConnection(std::move(con));
            return user_info; // Return the retrieved user info
        }
        _pool->ReturnConnection(std::move(con));
        return UserInfo(); // Return empty UserInfo if no record found
    } catch (sql::SQLException& e) {
        _pool->ReturnConnection(std::move(con));
        spdlog::error("SQLException: {}", e.what());
        spdlog::error("(MYSQL error code: {}, SQLState: {})", e.getErrorCode(), e.getSQLState());
        return UserInfo(); // Return empty UserInfo on error
    }
}

bool MysqlDAO::CheckEmailExists(const std::string& email) {
    auto con = _pool->GetConnection();
    try {
        if (con == nullptr) {
            return false; // Connection failed
        }
        std::unique_ptr<sql::PreparedStatement> stmt(con->con_->prepareStatement("SELECT COUNT(*) FROM user WHERE email = ?"));
        stmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());
        if (result->next()) {
            int count = result->getInt(1);
            _pool->ReturnConnection(std::move(con));
            return count > 0; // Return true if email exists, false otherwise
        }
        _pool->ReturnConnection(std::move(con));
        return false; // Email does not exist
    } catch (sql::SQLException& e) {
        _pool->ReturnConnection(std::move(con));
        spdlog::error("SQLException: {}", e.what());
        spdlog::error("(MYSQL error code: {}, SQLState: {})", e.getErrorCode(), e.getSQLState());
        return false; // Error occurred while checking email existence
    }
}