#ifndef __Singleton_H__
#define __Singleton_H__
//******************************************************************************
#include "mutex.h"

template<typename T>
struct Singleton {
    static T* getInstance() {
        static T instance;
        return &instance;
    }
};

#define SINGLETON_(T) Singleton<T>::getInstance()
//******************************************************************************
#endif