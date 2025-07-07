#include "AsioIOContextPool.h"

AsioIOContextPool::~AsioIOContextPool()
{
    Stop();
}

void AsioIOContextPool::Stop()
{
    if (_stopped.load()) {
        return; // Already stopped
    }
    _stopped.store(true); // Set the stopped flag to true

    for (auto &guard : _work_guards) {
        guard->reset(); // 先释放 keep-alive 令 io_context::run 能返回
    }

    for (auto &ctx : _io_contexts) {
        ctx.stop();     // 再显式停止每个 io_context
    }

    for (auto &thread : _threads) {
        if (thread.joinable()) {
            thread.join(); // Wait for all threads to finish
        }
    }
    _io_contexts.clear();
    _work_guards.clear();
    _threads.clear();
    _next_index = 0;
    _pool_size = 0;
}

boost::asio::io_context &AsioIOContextPool::GetNextIOContext()
{
    auto index = _next_index++ % _pool_size; // Round-robin scheduling
    if (index >= _io_contexts.size()) {
        throw std::out_of_range("Index out of range in IOContext pool");
    }
    return _io_contexts[index];
}

size_t AsioIOContextPool::GetPoolSize() const
{
    return std::max(_pool_size, _io_contexts.size());
}

AsioIOContextPool::AsioIOContextPool(size_t pool_size) : // （Singleton<T>::GetInstance<>()）模板中调用构造函数，定义不可见,放入头文件
    _io_contexts(pool_size),
    _work_guards(pool_size),
    _pool_size(pool_size),
    _next_index(0),
    _stopped(false)
{
    if (_pool_size == 0) {
        throw std::invalid_argument("IO context pool size must be > 0");
    }
    // Create WorkGuard for each IOContext
    for (size_t i = 0; i < pool_size; ++i) {
        _work_guards[i] = std::unique_ptr<WorkGuard>(new WorkGuard(_io_contexts[i].get_executor()));
    }
    // Create threads for each IOContext
    for (size_t i = 0; i < pool_size; ++i) {
        _threads.emplace_back([this, i]() {
            _io_contexts[i].run();
        });
    }
}

