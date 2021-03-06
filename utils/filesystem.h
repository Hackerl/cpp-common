#ifndef __FileSystem_H__
#define __FileSystem_H__
//******************************************************************************
#include <sys/types.h>
#include <sys/stat.h>
//******************************************************************************
struct CFileState {
    unsigned long size;
    unsigned long lastAccessTime;
    unsigned long lastModifyTime;
    unsigned long lastChangeTime;
};

class CFileSystem {
public:
    static bool getFileState(const char* path, CFileState& fileState) {
        struct stat info = {};

        if (stat(path, &info) != 0)
            return false;

        fileState.size = info.st_size;

# if defined(__APPLE__)
        fileState.lastAccessTime = info.st_atimespec.tv_sec;
        fileState.lastModifyTime = info.st_mtimespec.tv_sec;
        fileState.lastChangeTime = info.st_ctimespec.tv_sec;
#else
        fileState.lastAccessTime = info.st_atim.tv_sec;
        fileState.lastModifyTime = info.st_mtim.tv_sec;
        fileState.lastChangeTime = info.st_ctim.tv_sec;
#endif

        return true;
    }

    static bool isFileExists(const char* path) {
        struct stat info = {};

        if (stat(path, &info) != 0)
            return false;

        return (info.st_mode & static_cast<unsigned int>(S_IFREG)) != 0;
    }

    static bool isDirExists(const char* path) {
        struct stat info = {};

        if (stat(path, &info) != 0)
            return false;

        return (info.st_mode & static_cast<unsigned int>(S_IFDIR)) != 0;
    }

    static bool createDir(const char* path, mode_t mode = 0755) {
        return mkdir(path, mode) == 0;
    }
};
//******************************************************************************
#endif
