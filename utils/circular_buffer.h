#ifndef __CircularBuffer_H__
#define __CircularBuffer_H__
//******************************************************************************
template <class T, unsigned long N>
class CCircularBuffer {
public:
    bool enqueue(T item) {
        if (full())
            return false;

        mBuffer[mTail] = item;
        mTail = (mTail + 1) % mMaxSize;

        return true;
    }

    bool dequeue(T& item) {
        if (empty())
            return false;

        item = mBuffer[mHead];
        mHead = (mHead + 1) % mMaxSize;

        return true;
    }

public:
    unsigned long size() {
        if (mTail >= mHead)
            return mTail - mHead;

        return mMaxSize - (mHead - mTail);
    }

    bool empty() {
        return mHead == mTail;
    }

    bool full() {
        return (mTail + 1) % mMaxSize == mHead;
    }

private:
    T mBuffer[N]{};

private:
    unsigned long mHead{};
    unsigned long mTail{};
    unsigned long mMaxSize{N};
};
//******************************************************************************
#endif