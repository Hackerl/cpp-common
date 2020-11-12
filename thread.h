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
        mThreadHandle = nullptr;
        mThis = nullptr;
        mRouter = nullptr;
    }

public:
    bool start(T* t, PFN_WorkRoutine router) {
        if (mThreadHandle) {
            mThreadHandle->join();
            delete mThreadHandle;
            mThreadHandle = nullptr;
        }

        mThis = t;
        mRouter = router;

        mThreadHandle = new std::thread(staticThreadRouter, (void *) this);

        return mThreadHandle != nullptr;
    }

    bool isRunning() {
        return mThreadHandle != nullptr;
    }

    void stop(bool ForceTerminate = false) {
        if (mThreadHandle) {
            if (!ForceTerminate && mThreadHandle->joinable())
                mThreadHandle->join();

            delete mThreadHandle;
            mThreadHandle = nullptr;
        }
    }

private:
    static void staticThreadRouter(void * t) {
        ((CThread_<T> *) t)->threadRouter();
    }

    void threadRouter() {
        (mThis->*mRouter)();
    }

private:
    T*              mThis;
    std::thread*    mThreadHandle;
    PFN_WorkRoutine mRouter;
};
//******************************************************************************
#endif