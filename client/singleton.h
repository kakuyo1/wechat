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
    template <typename... Args>
    // support for perfect forwarding
    static std::shared_ptr<T> GetInstance(Args&&... args) {
        static std::shared_ptr<T> instance(new T(std::forward(args)...));
        return instance;
    }
};

#endif // SINGLETON_H
