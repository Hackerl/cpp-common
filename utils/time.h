#ifndef __Time_H__
#define __Time_H__
//******************************************************************************
#include <ctime>

class CTime {
public:
    static long getUnixTimestamp() {
        return static_cast<long>(std::time(nullptr));
    }
};
//******************************************************************************
#endif