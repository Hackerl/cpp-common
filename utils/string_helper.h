#ifndef __StringHelper_H__
#define __StringHelper_H__
//******************************************************************************
#include <vector>
#include <sstream>
#include <algorithm>
//******************************************************************************
class CStringHelper {
public:
    static bool findStringIC(const std::string & strHaystack, const std::string & strNeedle) {
        auto it = std::search(
            strHaystack.begin(), strHaystack.end(),
            strNeedle.begin(), strNeedle.end(),
            [](char ch1, char ch2) { return toupper(ch1) == toupper(ch2); }
        );

        return (it != strHaystack.end());
    }

    static std::vector<std::string> split(const std::string& str, char splitter) {
        std::vector<std::string> tokens;

        std::stringstream ss(str);
        std::string temp;

        // split into new "lines" based on character
        while (getline(ss, temp, splitter)) {
            tokens.push_back(temp);
        }

        return tokens;
    }

    static bool startWith(std::string const &fullString, std::string const &ending) {
        if (fullString.length() < ending.length())
            return false;

        return fullString.compare(0, ending.length(), ending) == 0;
    }

    static bool endWith(std::string const &fullString, std::string const &ending) {
        if (fullString.length() < ending.length())
            return false;

        return fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0;
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

    static void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    static void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    static std::string ltrimCopy(std::string s) {
        ltrim(s);
        return s;
    }

    static std::string rtrimCopy(std::string s) {
        rtrim(s);
        return s;
    }

    static std::string trimCopy(std::string s) {
        trim(s);
        return s;
    }
};
//******************************************************************************
#endif