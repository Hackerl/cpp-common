#ifndef __StringHelper_H__
#define __StringHelper_H__
//******************************************************************************
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <memory>
//******************************************************************************
class CStringHelper {
public:
    static bool findStringIC(const std::string & strHaystack, const std::string & strNeedle) {
        auto it = std::search(
            strHaystack.begin(), strHaystack.end(),
            strNeedle.begin(), strNeedle.end(),
            [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
        );

        return (it != strHaystack.end());
    }

    static std::vector<std::string> split(const std::string& str, char splitter) {
        std::vector<std::string> tokens;

        std::stringstream ss(str);
        std::string temp;

        while (getline(ss, temp, splitter)) {
            tokens.push_back(temp);
        }

        return tokens;
    }

    template<class T>
    static std::string join(const T& containers, const char* delimiter) {
        if (containers.empty())
            return "";

        return std::accumulate(
                std::next(containers.begin()),
                containers.end(),
                containers.front(),
                [=](auto result, const auto &value) {
                    return result + delimiter + value;
                }
        );
    }

    static bool startsWith(const std::string &str, const std::string &prefix) {
        if (str.length() < prefix.length())
            return false;

        return str.compare(0, prefix.length(), prefix) == 0;
    }

    static bool endsWith(const std::string &str, const std::string &suffix) {
        if (str.length() < suffix.length())
            return false;

        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    template<class T>
    static bool toNumber(const std::string& str, T& number, int base = 10) {
        std::istringstream ss(str);

        switch (base) {
            case 8:
                ss >> std::oct >> number;
                break;

            case 10:
                ss >> std::dec >> number;
                break;

            case 16:
                ss >> std::hex >> number;
                break;

            default:
                ss >> number;
        }

        return (!ss.fail() && !ss.bad());
    }

    static void ltrim(std::string &str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    static void rtrim(std::string &str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), str.end());
    }

    static void trim(std::string &str) {
        ltrim(str);
        rtrim(str);
    }

    static std::string ltrimCopy(std::string str) {
        ltrim(str);
        return str;
    }

    static std::string rtrimCopy(std::string str) {
        rtrim(str);
        return str;
    }

    static std::string trimCopy(std::string str) {
        trim(str);
        return str;
    }

    static void trimExtraSpace(std::string &str) {
        str.erase(std::unique(std::begin(str), std::end(str), [](unsigned char a, unsigned char b) {
            return std::isspace(a) && std::isspace(b);
        }), std::end(str));
    }

    static std::string trimExtraSpaceCopy(std::string str) {
        trimExtraSpace(str);
        return str;
    }

    static std::string tolower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    static std::string toupper(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    template<typename... Args>
    static std::string format(const char *fmt, Args... args) {
        int length = snprintf(nullptr, 0, fmt, args...);

        if (length <= 0)
            return "";

        std::unique_ptr<char> buffer(new char[length + 1]);
        snprintf(buffer.get(), length + 1, fmt, args...);

        return std::string(buffer.get(), length);
    }
};
//******************************************************************************
#endif