#ifndef __Condition_H__
#define __Condition_H__
//******************************************************************************
#include <mutex>
#include <condition_variable>
//******************************************************************************
class Condition {
public:
    Condition() {
        mSignal = false;
    }

public:
    void wait() {
        std::unique_lock<std::mutex> lock(mMutex);

        while (!mSignal)
            mCond.wait(lock);

        mSignal = false;
    }

    void notify() {
        std::unique_lock<std::mutex> lock(mMutex);

        mSignal = true;
        mCond.notify_one();
    }

private:
    bool mSignal;
    std::mutex mMutex;
    std::condition_variable mCond;
};
//******************************************************************************
#endif