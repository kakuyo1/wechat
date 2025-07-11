#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include "Singleton.h"

/*******************************************************
 * 暂时不支持传递参数
 *********************************************************/

using IOContext = boost::asio::io_context;
using WorkGuard = boost::asio::executor_work_guard<IOContext::executor_type>;
using WorkGuardPtr = std::unique_ptr<WorkGuard>;

class AsioIOContextPool : public Singleton<AsioIOContextPool> {
    friend class Singleton<AsioIOContextPool>; // Allow Singleton to create AsioIOContextPool instance
public:
    ~AsioIOContextPool();
    void Stop();
    boost::asio::io_context& GetNextIOContext();
    size_t GetPoolSize() const;
private:
    AsioIOContextPool(size_t pool_size = std::thread::hardware_concurrency()); // （Singleton<T>::GetInstance<>()）模板中调用构造函数，定义不可见,放入头文件

    AsioIOContextPool(const AsioIOContextPool&) = delete; // Disable copy constructor
    AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;
    AsioIOContextPool(AsioIOContextPool&&) = delete; // Disable move constructor
    AsioIOContextPool& operator=(AsioIOContextPool&&) = delete;
private:
    std::vector<IOContext> _io_contexts;
    std::vector<WorkGuardPtr> _work_guards;
    std::vector<std::thread> _threads;
    std::size_t _pool_size;
    std::size_t _next_index; // Index for round-robin scheduling
    std::atomic<bool> _stopped;
};
