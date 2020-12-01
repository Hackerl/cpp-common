#ifndef __Random_H__
#define __Random_H__
//******************************************************************************
#include "binascii.h"
#include <random>
#include <common/guid.h>

class CRandom {
public:
    unsigned int next() {
        return mRandomDev();
    }

    void fill(char *buffer, unsigned long length) {
        for (int i = 0; i < length; i++)
            buffer[i] = static_cast<char>(next() & 0xFFU);
    }

    GUID generateGUID() {
        GUID guid = {};

        fill((char *)&guid.data1, sizeof(guid.data1));
        fill((char *)&guid.data2, sizeof(guid.data2));
        fill((char *)&guid.data3, sizeof(guid.data3));
        fill((char *)&guid.data4, sizeof(guid.data4));

        return guid;
    }

    std::string generateUUID() {
        GUID guid = generateGUID();
        std::ostringstream ss;

        ss <<  CBinascii::hexlify((const unsigned char *)&guid.data1, sizeof(guid.data1));
        ss << "-" << CBinascii::hexlify((const unsigned char *)&guid.data2, sizeof(guid.data2));
        ss << "-" << CBinascii::hexlify((const unsigned char *)&guid.data3, sizeof(guid.data3));
        ss << "-" << CBinascii::hexlify((const unsigned char *)&guid.data4, 2);
        ss << "-" << CBinascii::hexlify((const unsigned char *)&guid.data4 + 2, sizeof(guid.data4) - 2);

        return ss.str();
    }

private:
    std::random_device mRandomDev;
};
//******************************************************************************
#endif