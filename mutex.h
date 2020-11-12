#ifndef __Mutex_H__
#define __Mutex_H__
//******************************************************************************
#include <mutex>
//******************************************************************************
class Mutex {
public:
    void lock() {
        mMutex.lock();
    }

    void unlock() {
        mMutex.unlock();
    }

private:
    std::mutex mMutex;
};
//******************************************************************************
template<typename T>
class AutoMutex_ {
public:
    explicit AutoMutex_(T& Lock) : mLock(Lock) {
        mLock.lock();
    }

    ~AutoMutex_() {
        mLock.unlock();
    }

private:
    T& mLock;
};
//******************************************************************************
typedef AutoMutex_<Mutex> AutoMutex;
//******************************************************************************
#endif