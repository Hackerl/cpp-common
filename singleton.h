#ifndef __Singleton_H__
#define __Singleton_H__
//******************************************************************************
#include "mutex.h"

template<typename T>
struct Singleton {
    static T* Instance() {
        static Mutex MutexLock;

        MutexLock.lock();

        static T __instance__;

        MutexLock.unlock();

        return &__instance__;
    }

    static T* InstanceNoLock() {
        static T __instance__;
        return &__instance__;
    }
};

#define SINGLETON_(class, ...) ((class*)Singleton<class>::Instance())
#define SINGLETON_NO_LOCK_(class, ...) ((class*)Singleton<class>::InstanceNoLock())
//******************************************************************************
#endif