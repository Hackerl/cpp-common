#ifndef __Base64_H__
#define __Base64_H__
//******************************************************************************
#include <vector>
#include <string>

constexpr unsigned char from_base64[] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,  62, 255,  63,
        52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255, 255, 255, 255,
        255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
        15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255,  63,
        255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255 };

constexpr char to_base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789+/";

class CBase64 {
public:
    static std::string encode(const std::vector<unsigned char> &buffer) {
        if (buffer.empty())
            return "";

        return encode(buffer.data(), buffer.size());
    }

    static std::string encode(const unsigned char *buf, size_t bufLen) {
        size_t missing = 0;
        size_t ret_size = bufLen;

        while ((ret_size % 3) != 0) {
            ret_size++;
            missing++;
        }

        ret_size = 4 * ret_size / 3;

        std::string ret;
        ret.reserve(ret_size);

        for (unsigned int i = 0; i < ret_size / 4; i++) {
            size_t index = i * 3;
            unsigned char b3[3];

            b3[0] = (index + 0 < bufLen) ? buf[index + 0] : 0;
            b3[1] = (index + 1 < bufLen) ? buf[index + 1] : 0;
            b3[2] = (index + 2 < bufLen) ? buf[index + 2] : 0;

            unsigned char b4[4];

            b4[0] = ((b3[0] & 0xfcU) >> 2U);
            b4[1] = ((b3[0] & 0x03U) << 4U) + ((b3[1] & 0xf0U) >> 4U);
            b4[2] = ((b3[1] & 0x0fU) << 2U) + ((b3[2] & 0xc0U) >> 6U);
            b4[3] = ((b3[2] & 0x3fU) << 0U);

            ret.push_back(to_base64[b4[0]]);
            ret.push_back(to_base64[b4[1]]);
            ret.push_back(to_base64[b4[2]]);
            ret.push_back(to_base64[b4[3]]);
        }

        for (size_t i = 0; i < missing; i++)
            ret[ret_size - i - 1] = '=';

        return ret;
    }

    static std::vector<unsigned char> decode(std::string encoded_string) {
        while ((encoded_string.size() % 4) != 0)
            encoded_string.push_back('=');

        size_t encoded_size = encoded_string.size();

        std::vector<unsigned char> ret;
        ret.reserve(3 * encoded_size / 4);

        for (size_t i = 0; i < encoded_size; i += 4) {
            unsigned char b4[4];

            b4[0] = (encoded_string[i + 0] <= 'z') ? from_base64[encoded_string[i + 0]] : 0xff;
            b4[1] = (encoded_string[i + 1] <= 'z') ? from_base64[encoded_string[i + 1]] : 0xff;
            b4[2] = (encoded_string[i + 2] <= 'z') ? from_base64[encoded_string[i + 2]] : 0xff;
            b4[3] = (encoded_string[i + 3] <= 'z') ? from_base64[encoded_string[i + 3]] : 0xff;

            unsigned char b3[3];

            b3[0] = ((b4[0] & 0x3fU) << 2U) + ((b4[1] & 0x30U) >> 4U);
            b3[1] = ((b4[1] & 0x0fU) << 4U) + ((b4[2] & 0x3cU) >> 2U);
            b3[2] = ((b4[2] & 0x03U) << 6U) + ((b4[3] & 0x3fU) >> 0U);

            if (b4[1] != 0xff) ret.push_back(b3[0]);
            if (b4[2] != 0xff) ret.push_back(b3[1]);
            if (b4[3] != 0xff) ret.push_back(b3[2]);
        }

        return ret;
    }
};
//******************************************************************************
#endif