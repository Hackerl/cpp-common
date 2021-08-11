#ifndef INTERFACE_H
#define INTERFACE_H

#include "guid.h"

class Interface {
public:
    virtual ~Interface() = default;
};

using InterfaceID = GUID;

#endif