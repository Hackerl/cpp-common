#ifndef __CircularBuffer_H__
#define __CircularBuffer_H__
//******************************************************************************
#include <atomic>

template <class T, unsigned long N>
class CCircularBuffer {
private:
    enum emState {
        IDLE,
        PUTTING,
        VALID,
        TAKING
    };

public:
    bool enqueue(T item) {
        unsigned long index = mTail;

        do {
            if (full())
                return false;
        } while (!mTail.compare_exchange_weak(index, (index + 1) % mMaxSize));

        while (!__sync_bool_compare_and_swap(&mState[index], emState::IDLE, emState::PUTTING)) {

        }

        mBuffer[index] = item;

        __sync_lock_test_and_set(&mState[index], emState::VALID);

        return true;
    }

    bool dequeue(T& item) {
        unsigned long index = mHead;

        do {
            if (empty())
                return false;
        } while (!mHead.compare_exchange_weak(index, (index + 1) % mMaxSize));

        while (!__sync_bool_compare_and_swap(&mState[index], emState::VALID, emState::TAKING)) {

        }

        item = mBuffer[index];

        __sync_lock_test_and_set(&mState[index], emState::IDLE);

        return true;
    }

public:
    unsigned long size() {
        unsigned long head = mHead;
        unsigned long tail = mTail;

        if (tail >= head)
            return tail - head;

        return mMaxSize - (head - tail);
    }

    bool empty() {
        return mHead == mTail;
    }

    bool full() {
        return (mTail + 1) % mMaxSize == mHead;
    }

private:
    T mBuffer[N]{};
    emState mState[N]{IDLE};

private:
    unsigned long mMaxSize{N};

private:
    std::atomic<unsigned long> mHead{0};
    std::atomic<unsigned long> mTail{0};
};
//******************************************************************************
#endif