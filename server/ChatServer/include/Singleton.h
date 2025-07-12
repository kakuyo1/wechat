#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>

/**************************************************************
 * version 1.0 : 暂时不支持参数构造
 *************************************************************/


template <typename T>
class Singleton {
protected:
    Singleton() = default;
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
