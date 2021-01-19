#ifndef __CircularBuffer_H__
#define __CircularBuffer_H__
//******************************************************************************
#include <atomic>
#include <climits>

template <class T, unsigned long N>
class CCircularBuffer {
private:
    static constexpr auto MODULO = ULONG_MAX - (ULONG_MAX % N);

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
        } while (!mTail.compare_exchange_weak(index, (index + 1) % MODULO));

        index %= N;

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
        } while (!mHead.compare_exchange_weak(index, (index + 1) % MODULO));

        index %= N;

        while (!__sync_bool_compare_and_swap(&mState[index], emState::VALID, emState::TAKING)) {

        }

        item = mBuffer[index];

        __sync_lock_test_and_set(&mState[index], emState::IDLE);

        return true;
    }

public:
    unsigned long size() {
        return (mTail % N + N - mHead % N) % N;
    }

    bool empty() {
        return mHead == mTail;
    }

    bool full() {
        return (mTail + 1) % N == mHead % N;
    }

private:
    T mBuffer[N]{};
    emState mState[N]{IDLE};

private:
    std::atomic<unsigned long> mHead{0};
    std::atomic<unsigned long> mTail{0};
};
//******************************************************************************
#endif