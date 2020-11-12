#ifndef __Interface_H__
#define __Interface_H__
//******************************************************************************
#include "guid.h"

class Interface {
public:
    virtual ~Interface() = default;
};

typedef CGUID InterfaceID;
//******************************************************************************
#endif