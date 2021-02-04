#ifndef __Unicorn_H__
#define __Unicorn_H__
//******************************************************************************
#include <fcntl.h>
#include <unistd.h>
//******************************************************************************
class CUnicorn {
public:
    explicit CUnicorn(const char *path) {
        fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }

    ~CUnicorn() {
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }

public:
    bool acquire() {
        if (fd < 0)
            return false;

        return setFileLock(F_WRLCK);
    }

    bool release() {
        if (fd < 0)
            return false;

        return setFileLock(F_UNLCK);
    }

private:
    bool setFileLock(short int type) const {
        flock lock = {};

        lock.l_type = type;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;

        return fcntl(fd, F_SETLK, &lock) != -1;
    }

private:
    int fd;
};
//******************************************************************************
#endif