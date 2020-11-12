#ifndef __VectorHelper_H__
#define __VectorHelper_H__
//******************************************************************************
#include <vector>
#include <string>
#include <sys/types.h>

template <class T>
inline void vectorAppend(std::vector<T> & v1, std::vector<T> v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template <class T>
inline void vectorAppend(std::vector<T> & v1, std::vector<T> v2, size_t Size)
{
    v1.insert(v1.end(), v2.begin(), v2.begin() + Size);
}

template <class T>
inline void vectorAppend(std::vector<T> & v1, const T * v2, size_t Size)
{
    v1.insert(v1.end(), v2, v2 + Size);
}

template <class T>
inline void vectorAppendStr(std::vector<T> & v1, std::string v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template <class T, class T_>
inline void vectorAppendData(std::vector<T> & v1, const T_ v2)
{
    v1.insert(v1.end(), (T *)&v2, (T *)&v2 + sizeof(T_));
}

template <class T>
inline void vectorPrepend(std::vector<T> & v1, std::vector<T> v2)
{
    v1.insert(v1.begin(), v2.begin(), v2.end());
}

template <class T>
inline void vectorPrepend(std::vector<T> & v1, std::vector<T> v2, size_t Size)
{
    v1.insert(v1.begin(), v2.begin(), v2.begin() + Size);
}

template <class T>
inline void vectorPrepend(std::vector<T> & v1, const T * v2, size_t Size)
{
    v1.insert(v1.begin(), v2, v2 + Size);
}

template <class T>
inline void vectorPrependStr(std::vector<T> & v1, std::string v2)
{
    v1.insert(v1.begin(), v2.begin(), v2.end());
}

template <class T, class T_>
inline void vectorPrependData(std::vector<T> & v1, const T_ v2)
{
    v1.insert(v1.begin(), (T *)&v2, (T *)&v2 + sizeof(T_));
}
//******************************************************************************
#endif