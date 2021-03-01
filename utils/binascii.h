#ifndef __Binascii_H__
#define __Binascii_H__
//******************************************************************************
#include "string_helper.h"
#include <vector>
#include <sstream>

constexpr char HEX_MAP[] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

class CBinascii {
public:
    static std::string hexlify(const unsigned char *buffer, unsigned long length) {
        std::string s(length * 2, ' ');

        for (int i = 0; i < length; i++) {
            s[2 * i] = HEX_MAP[(buffer[i] & 0xF0U) >> 4U];
            s[2 * i + 1] = HEX_MAP[buffer[i] & 0x0FU];
        }

        return s;
    }

    static std::vector<unsigned char> unHexlify(const std::string &hex) {
        std::vector<unsigned char> buffer;

        for (unsigned long i = 0; i < hex.size(); i += 2) {
            unsigned int number;

            if (!CStringHelper::toNumber(hex.substr(i, 2), number, 16))
                break;

            buffer.push_back(number & 0xFFU);
        }

        return buffer;
    }
};
//******************************************************************************
#endif