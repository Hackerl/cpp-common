#ifndef __GUID_H__
#define __GUID_H__
//******************************************************************************
#include <cstring>

struct CGUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];

    bool operator == (const CGUID& guid) const {
        return (Data1 == guid.Data1 && Data2 == guid.Data2 && Data3 == guid.Data3 && !memcmp(Data4, guid.Data4, sizeof(Data4)));
    }
};
//******************************************************************************
#endif