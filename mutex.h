#ifndef __Mutex_H__
#define __Mutex_H__
//******************************************************************************
#include <mutex>
//******************************************************************************
typedef std::mutex Mutex;
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