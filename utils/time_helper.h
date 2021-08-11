#ifndef TIME_HELPER_H
#define TIME_HELPER_H

#include <ctime>
#include <string>

class CTimeHelper {
public:
    static long getUnixTimestamp() {
        return static_cast<long>(std::time(nullptr));
    }

    static std::string getTimeString() {
        tm timeInfo = {};
        time_t rawTime = {};

        std::time(&rawTime);
        localtime_r(&rawTime, &timeInfo);

        char buffer[64] = {};
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

        return buffer;
    }
};

#endif