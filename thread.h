#ifndef THREAD_H
#define THREAD_H

#include <thread>

template<typename T>
class CThread {
public:
    CThread() {
        mThread = nullptr;
    }

public:
    bool start(T *t, void (T::*router)()) {
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
    std::thread *mThread;
};

#endif