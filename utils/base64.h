#ifndef __Base64_H__
#define __Base64_H__
//******************************************************************************
#include <vector>
#include <string>

constexpr unsigned char BASE64_FROM_MAP[] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255, 62,  255, 63,
        52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255, 255, 255, 255, 255,
        255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
        15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  255, 255, 255, 255, 63,
        255, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  255, 255, 255, 255, 255
};

constexpr char BASE64_TO_MAP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789+/";

class CBase64 {
public:
    static std::string encode(const std::vector<unsigned char> &buffer) {
        if (buffer.empty())
            return "";

        return encode(buffer.data(), buffer.size());
    }

    static std::string encode(const unsigned char *buffer, size_t length) {
        size_t missing = 0;
        size_t size = length;

        while ((size % 3) != 0) {
            size++;
            missing++;
        }

        size = 4 * size / 3;

        std::string encoded;
        encoded.reserve(size);

        for (unsigned int i = 0; i < size / 4; i++) {
            size_t index = i * 3;
            unsigned char b3[3] = {};

            b3[0] = (index + 0 < length) ? buffer[index + 0] : 0;
            b3[1] = (index + 1 < length) ? buffer[index + 1] : 0;
            b3[2] = (index + 2 < length) ? buffer[index + 2] : 0;

            unsigned char b4[4] = {};

            b4[0] = ((b3[0] & 0xfcU) >> 2U);
            b4[1] = ((b3[0] & 0x03U) << 4U) + ((b3[1] & 0xf0U) >> 4U);
            b4[2] = ((b3[1] & 0x0fU) << 2U) + ((b3[2] & 0xc0U) >> 6U);
            b4[3] = ((b3[2] & 0x3fU) << 0U);

            encoded.push_back(BASE64_TO_MAP[b4[0]]);
            encoded.push_back(BASE64_TO_MAP[b4[1]]);
            encoded.push_back(BASE64_TO_MAP[b4[2]]);
            encoded.push_back(BASE64_TO_MAP[b4[3]]);
        }

        for (size_t i = 0; i < missing; i++)
            encoded[size - i - 1] = '=';

        return encoded;
    }

    static std::vector<unsigned char> decode(std::string encoded) {
        while ((encoded.size() % 4) != 0)
            encoded.push_back('=');

        size_t size = encoded.size();

        std::vector<unsigned char> buffer;
        buffer.reserve(3 * size / 4);

        for (size_t i = 0; i < size; i += 4) {
            unsigned char b4[4] = {};

            b4[0] = (encoded[i + 0] <= 'z') ? BASE64_FROM_MAP[encoded[i + 0]] : 0xff;
            b4[1] = (encoded[i + 1] <= 'z') ? BASE64_FROM_MAP[encoded[i + 1]] : 0xff;
            b4[2] = (encoded[i + 2] <= 'z') ? BASE64_FROM_MAP[encoded[i + 2]] : 0xff;
            b4[3] = (encoded[i + 3] <= 'z') ? BASE64_FROM_MAP[encoded[i + 3]] : 0xff;

            unsigned char b3[3] = {};

            b3[0] = ((b4[0] & 0x3fU) << 2U) + ((b4[1] & 0x30U) >> 4U);
            b3[1] = ((b4[1] & 0x0fU) << 4U) + ((b4[2] & 0x3cU) >> 2U);
            b3[2] = ((b4[2] & 0x03U) << 6U) + ((b4[3] & 0x3fU) >> 0U);

            if (b4[1] != 0xff)
                buffer.push_back(b3[0]);

            if (b4[2] != 0xff)
                buffer.push_back(b3[1]);

            if (b4[3] != 0xff)
                buffer.push_back(b3[2]);
        }

        return buffer;
    }
};
//******************************************************************************
#endif