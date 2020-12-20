#ifndef __Thread_H__
#define __Thread_H__
//******************************************************************************
#include <thread>

template<typename T>
class CThread_ {
public:
    typedef void (T::*PFN_WorkRoutine)();

public:
    CThread_() {
        mThread = nullptr;
    }

public:
    bool start(T* t, PFN_WorkRoutine router) {
        if (isRunning()) {
            stop();
        }

        mThread = new std::thread(router, t);

        return mThread != nullptr;
    }

    bool isRunning() {
        return mThread != nullptr;
    }

    void stop(bool force = false) {
        if (mThread) {
            if (!force && mThread->joinable())
                mThread->join();

            delete mThread;
            mThread = nullptr;
        }
    }

private:
    std::thread*    mThread;
};
//******************************************************************************
#endif