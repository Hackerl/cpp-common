#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
struct Singleton {
    static T *getInstance() {
        static T instance;
        return &instance;
    }
};

#define SINGLETON(T) Singleton<T>::getInstance()

#endif