#ifndef __Singleton_H__
#define __Singleton_H__
//******************************************************************************
#include "mutex.h"

template<typename T>
struct Singleton {
    static T* Instance() {
        static Mutex MutexLock;

        MutexLock.lock();

        static T _instance_;

        MutexLock.unlock();

        return &_instance_;
    }

    static T* InstanceNoLock() {
        static T _instance_;
        return &_instance_;
    }
};

#define SINGLETON_(class, ...) ((class*)Singleton<class>::Instance())
#define SINGLETON_NO_LOCK_(class, ...) ((class*)Singleton<class>::InstanceNoLock())
//******************************************************************************
#endif