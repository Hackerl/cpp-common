#ifndef PATH_H
#define PATH_H

#include <string>
#include <climits>
#include <libgen.h>
#include <unistd.h>

#ifdef _WIN32
constexpr auto FILE_SYSTEM_PATH_SEPARATOR = '\\';
#else
constexpr auto FILE_SYSTEM_PATH_SEPARATOR = '/';
#endif

class CPath {
public:
    static std::string getFileDescriptorPath(int fd) {
        char buffer[PATH_MAX + 1] = {};

        if (readlink(join("/proc/self/fd", std::to_string(fd)).c_str(), buffer, PATH_MAX) == -1)
            return "";

        return {buffer};
    }

    static std::string getApplicationDirectory() {
        char buffer[PATH_MAX + 1] = {};

        if (readlink("/proc/self/exe", buffer, PATH_MAX) == -1)
            return "";

        return {dirname(buffer)};
    }

    static std::string getApplicationPath() {
        char buffer[PATH_MAX + 1] = {};

        if (readlink("/proc/self/exe", buffer, PATH_MAX) == -1)
            return "";

        return {buffer};
    }

    static std::string getApplicationName() {
        char buffer[PATH_MAX + 1] = {};

        if (readlink("/proc/self/exe", buffer, PATH_MAX) == -1)
            return "";

        return {basename(buffer)};
    }

    static std::string getAbsolutePath(const char *path) {
        char buffer[PATH_MAX] = {};

        if (!realpath(path, buffer))
            return "";

        return {buffer};
    }

    template<typename... Args>
    static std::string join(const std::string &path, Args... args) {
        if (path.empty())
            return join(args...);

        if (path.back() != FILE_SYSTEM_PATH_SEPARATOR) {
            return path + FILE_SYSTEM_PATH_SEPARATOR + join(args...);
        } else {
            return path + join(args...);
        }
    }

private:
    static std::string join(const std::string &path) {
        return path;
    }
};

#endif