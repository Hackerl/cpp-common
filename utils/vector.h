#ifndef __VectorHelper_H__
#define __VectorHelper_H__
//******************************************************************************
#include <vector>

class CVectorHelper {
public:
    template <class T, class T1>
    static void append(T &dst, T1 &src) {
        dst.insert(dst.end(), src.begin(), src.end());
    }

    template <class T, class T1>
    static void appendType(std::vector<T> &dst, T1 &src) {
        dst.insert(dst.end(), (T *)&src, (T *)((char *)&src + sizeof(T1)));
    }
};
//******************************************************************************
#endif