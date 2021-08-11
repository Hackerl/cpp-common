#ifndef GUID_H
#define GUID_H

#include <cstring>

struct GUID {
    unsigned int    data1;
    unsigned short  data2;
    unsigned short  data3;
    unsigned char   data4[8];

    bool operator == (const GUID& guid) const {
        return (data1 == guid.data1 && data2 == guid.data2 && data3 == guid.data3 && !memcmp(data4, guid.data4, sizeof(data4)));
    }
};

#endif