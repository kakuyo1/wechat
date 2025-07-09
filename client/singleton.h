#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>

template <typename T>
class Singleton {
protected:
    Singleton() = default; // must be protected to allow derived classes to call it
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
public:
    ~Singleton() = default;
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> instance = std::shared_ptr<T>(new T()); // 无参构造
        return instance;
    }
};

#endif // SINGLETON_H
