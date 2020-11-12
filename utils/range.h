#ifndef __RANGE_H__
#define __RANGE_H__

#include <limits>

template <class T>
class CRange {
public:
    CRange() {
        start = std::numeric_limits<T>::min();
        end = std::numeric_limits<T>::max();
    }

public:
    bool isFullRange() const {
        return start == std::numeric_limits<T>::min() && end == std::numeric_limits<T>::max();
    }

    void reset() {
        start = std::numeric_limits<T>::min();
        end = std::numeric_limits<T>::max();
    }

    bool isValid() {
        return start <= end;
    }

    CRange<T>& verify() {
        if (!isValid())
            reset();

        return *this;
    }

public:
    T start;
    T end;
};

#endif
