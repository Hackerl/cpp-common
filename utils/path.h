#ifndef __Path_H__
#define __Path_H__
//******************************************************************************
#include <string>
#include <climits>
#include <libgen.h>
#include <unistd.h>
//******************************************************************************
#ifdef _WIN32
constexpr auto FILE_SYSTEM_PATH_SEPARATOR = '\\';
#else
constexpr auto FILE_SYSTEM_PATH_SEPARATOR = '/';
#endif
//******************************************************************************
class CPath {
public:
    static std::string getPathFromFD(int fd) {
        if (fd < 0)
            return "";

        std::string FDLinkPath = "/proc/self/fd/" + std::to_string(fd);

        char exePath[PATH_MAX] = {};
        ssize_t length = readlink(FDLinkPath.c_str(), exePath, PATH_MAX);

        if (length == -1)
            return "";

        return std::string(exePath, length);
    }

    static std::string getAPPDir() {
        char exePath[PATH_MAX] = {};
        ssize_t length = readlink("/proc/self/exe", exePath, PATH_MAX);

        if (length == -1)
            return "";

        return std::string(dirname(exePath));
    }

    static std::string getAPPPath() {
        char exePath[PATH_MAX] = {};
        ssize_t length = readlink("/proc/self/exe", exePath, PATH_MAX);

        if (length == -1)
            return "";

        return std::string(exePath, length);
    }

    static std::string getAPPName() {
        char exePath[PATH_MAX] = {};
        ssize_t length = readlink("/proc/self/exe", exePath, PATH_MAX);

        if (length == -1)
            return "";

        return std::string(basename(exePath));
    }

    static std::string getAbsolutePath(const char* path) {
        char absolutePath[PATH_MAX] = {};

        if (!realpath(path, absolutePath))
            return "";

        return std::string(absolutePath);
    }

    template<typename... Args>
    static std::string join(const std::string& path, Args... args) {
        if (path.empty())
            return join(args...);

        if (path.back() != FILE_SYSTEM_PATH_SEPARATOR) {
            return path + FILE_SYSTEM_PATH_SEPARATOR + join(args...);
        } else {
            return path + join(args...);
        }
    }

private:
    static std::string join(const std::string& path) {
        return path;
    }
};
//******************************************************************************
#endif