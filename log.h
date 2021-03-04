#ifndef __Log_H__
#define __Log_H__
//******************************************************************************
#include "thread.h"
#include "interface.h"
#include "singleton.h"
#include "condition.h"
#include "utils/shell.h"
#include "utils/time_helper.h"
#include "utils/string_helper.h"
#include "utils/circular_buffer.h"
#include "utils/path.h"
#include <list>
#include <fstream>
#include <unistd.h>

constexpr const char * LOG_TAGS[] = {"ERROR", "WARN", "INFO", "DEBUG"};

enum emLogLevel {
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

class ILogProvider: public Interface {
public:
    virtual void write(const std::string &message) = 0;
};

class CConsoleProvider: public ILogProvider {
public:
    void write(const std::string &message) override {
        fwrite(message.c_str(), 1, message.length(), stderr);
    }
};

class CFileProvider: public ILogProvider {
public:
    explicit CFileProvider(
            const char *name,
            unsigned long limit = 10 * 1024 * 1024,
            const char *directory = "/tmp",
            unsigned long remain = 10) {
        mName = name;
        mLimit = limit;
        mDirectory = directory;
        mRemain = remain;

        mFile.open(getLogPath());
    }

private:
    std::string getLogPath() {
        std::string filename = CStringHelper::format(
                "%s.%d.%ld.log",
                mName.c_str(),
                getpid(),
                CTimeHelper::getUnixTimestamp()
        );

        return CPath::join(mDirectory, filename);
    }

    void clean() {
        std::list<std::string> logs;
        std::string pattern = CStringHelper::format("%s.%d.*.log", mName.c_str(), getpid());

        if (!CShellAPI::match(CPath::join(mDirectory, pattern), logs))
            return;

        unsigned long size = logs.size();

        if (size <= mRemain)
            return;

        std::list<std::string> expired(logs.begin(), std::next(logs.begin(), size - mRemain));

        for (const auto &path: expired) {
            remove(path.c_str());
        }
    }

public:
    void write(const std::string &message) override {
        if (mFile.tellp() > mLimit) {
            mFile = std::ofstream(getLogPath());
            clean();
        }

        mFile.write(message.c_str(), message.length());
        mFile.flush();
    }

private:
    std::string mName;
    std::string mDirectory;

private:
    unsigned long mLimit;
    unsigned long mRemain;

private:
    std::ofstream mFile;
};

template<typename T>
class CAsyncProvider: public T {
public:
    template<typename... Args>
    explicit CAsyncProvider<T>(Args... args) : T(args...) {
        mThread.start(this, &CAsyncProvider<T>::loopThread);
    }

    ~CAsyncProvider<T>() override {
        mExit = true;
        mCondition.notify();
        mThread.stop();
    }

public:
    void write(const std::string &message) override {
        if (mBuffer.full())
            return;

        mBuffer.enqueue(message);
        mCondition.notify();
    }

public:
    void loopThread() {
        while (!mExit) {
            if (mBuffer.empty())
                mCondition.wait();

            std::string message = {};

            if (!mBuffer.dequeue(message))
                continue;

            T::write(message);
        }
    }

private:
    bool mExit{false};

private:
    Condition mCondition;
    CThread_<CAsyncProvider<T>> mThread;
    CCircularBuffer<std::string, 100> mBuffer;
};

struct CProviderRegister {
    emLogLevel level;
    ILogProvider *provider;
};

class CLogger {
#define gLogger SINGLETON_(CLogger)
public:
    ~CLogger() {
        for (const auto &r : mRegistry) {
            delete r.provider;
        }
    }

public:
    template<typename T, typename... Args>
    void log(emLogLevel level, const T format, Args... args) {
        std::string message = CStringHelper::format(format, args...);

        for (const auto &r : mRegistry) {
            if (level > r.level)
                continue;

            r.provider->write(message);
        }
    }

public:
    void addProvider(emLogLevel level, ILogProvider *provider) {
        mRegistry.push_back({level, provider});
    }

private:
    std::list<CProviderRegister> mRegistry;
};

#define INIT_CONSOLE_LOG(level)         gLogger->addProvider(level, new CConsoleProvider())
#define INIT_FILE_LOG(level, name, args...) \
                                        gLogger->addProvider(level, new CAsyncProvider<CFileProvider>(name, ## args))

#define NEWLINE                         "\n"
#define SOURCE                          strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define LOG_FMT                         "%s | %-5s | %20s:%-4d] "
#define LOG_TAG(level)                  LOG_TAGS[level]
#define LOG_ARGS(level)                 CTimeHelper::getTimeString().c_str(), LOG_TAG(level), SOURCE, __LINE__

#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARNING
#undef LOG_ERROR

#define LOG_DEBUG(message, args...)     gLogger->log(DEBUG, LOG_FMT message NEWLINE, LOG_ARGS(DEBUG), ## args)
#define LOG_INFO(message, args...)      gLogger->log(INFO, LOG_FMT message NEWLINE, LOG_ARGS(INFO), ## args)
#define LOG_WARNING(message, args...)   gLogger->log(WARNING, LOG_FMT message NEWLINE, LOG_ARGS(WARNING), ## args)
#define LOG_ERROR(message, args...)     gLogger->log(ERROR, LOG_FMT message NEWLINE, LOG_ARGS(ERROR), ## args)
//******************************************************************************
#endif