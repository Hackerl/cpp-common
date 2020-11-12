#ifndef __Random_H__
#define __Random_H__
//******************************************************************************
#include <random>
#include <Common/Singleton.h>

class CRandom {
public:
    unsigned int next() {
        return mRandomDev();
    }

private:
    std::random_device mRandomDev;
};
//******************************************************************************
#endif