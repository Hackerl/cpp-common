#ifndef PROCESS_H
#define PROCESS_H

#include "path.h"
#include "string_helper.h"
#include <fstream>
#include <list>

constexpr auto SYSTEM_STAT_FIELDS = 8;
constexpr auto PROCESS_STAT_FIELDS = 17;
constexpr auto PROCESS_MAP_FIELDS = 5;

struct CProcessState {
    int pid;
    std::string processName;
    std::string status;
    int ppid;
    int processGroup;
    int session;
    int ttyNR;
    int tpgID;
    unsigned int flags;
    unsigned long minFlt;
    unsigned long cMinFlt;
    unsigned long majFlt;
    unsigned long cMajFlt;
    unsigned long uTime;
    unsigned long sTime;
    long cuTime;
    long csTime;
};

struct CSystemState {
    std::string name;
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long ioWait;
    unsigned long irq;
    unsigned long softIrq;
};

struct CProcessStatus {
    std::string name;
    std::string state;
    unsigned long vmRSS;
    unsigned long threads;
};

struct CSystemMemory {
    unsigned long memoryTotal;
};

struct CProcessMap {
    unsigned long start;
    unsigned long end;
    std::string permission;
    unsigned long offset;
    std::string device;
    unsigned long inode;
    std::string pathname;
};

class CProcess {
public:
    static bool getProcessState(pid_t pid, CProcessState &processState) {
        std::string path = CPath::join("/proc", std::to_string(pid), "stat");
        std::ifstream stream(path);

        if (!stream.is_open())
            return false;

        std::string line;
        std::getline(stream, line);

        if (line.empty())
            return false;

        std::vector<std::string> stats = CStringHelper::split(line, ' ');

        if (stats.size() < PROCESS_STAT_FIELDS)
            return false;

        int index = 0;

        CStringHelper::toNumber(stats[index++], processState.pid);

        processState.processName = stats[index++];
        processState.status = stats[index++];

        CStringHelper::toNumber(stats[index++], processState.ppid);
        CStringHelper::toNumber(stats[index++], processState.processGroup);
        CStringHelper::toNumber(stats[index++], processState.session);
        CStringHelper::toNumber(stats[index++], processState.ttyNR);
        CStringHelper::toNumber(stats[index++], processState.tpgID);
        CStringHelper::toNumber(stats[index++], processState.flags);
        CStringHelper::toNumber(stats[index++], processState.minFlt);
        CStringHelper::toNumber(stats[index++], processState.cMinFlt);
        CStringHelper::toNumber(stats[index++], processState.majFlt);
        CStringHelper::toNumber(stats[index++], processState.cMajFlt);
        CStringHelper::toNumber(stats[index++], processState.uTime);
        CStringHelper::toNumber(stats[index++], processState.sTime);
        CStringHelper::toNumber(stats[index++], processState.cuTime);
        CStringHelper::toNumber(stats[index++], processState.csTime);

        return true;
    }

    static bool getSystemState(CSystemState &systemState) {
        std::ifstream stream("/proc/stat");

        if (!stream.is_open())
            return false;

        std::string line;
        std::getline(stream, line);

        if (line.empty())
            return false;

        CStringHelper::trimExtraSpace(line);
        std::vector<std::string> stats = CStringHelper::split(line, ' ');

        if (stats.size() < SYSTEM_STAT_FIELDS)
            return false;

        int index = 0;

        systemState.name = stats[index++];

        CStringHelper::toNumber(stats[index++], systemState.user);
        CStringHelper::toNumber(stats[index++], systemState.nice);
        CStringHelper::toNumber(stats[index++], systemState.system);
        CStringHelper::toNumber(stats[index++], systemState.idle);
        CStringHelper::toNumber(stats[index++], systemState.ioWait);
        CStringHelper::toNumber(stats[index++], systemState.irq);
        CStringHelper::toNumber(stats[index++], systemState.softIrq);

        return true;
    }

    static bool getProcessStatus(pid_t pid, CProcessStatus &processStatus) {
        std::string path = CPath::join("/proc", std::to_string(pid), "status");
        std::ifstream stream(path);

        if (!stream.is_open())
            return false;

        std::string line;

        while (std::getline(stream, line)) {
            std::vector<std::string> tokens = CStringHelper::split(line, ':');

            if (tokens.size() != 2)
                continue;

            if (tokens[0] == "Name") {
                processStatus.name = CStringHelper::trimCopy(tokens[1]);
            } else if (tokens[0] == "State") {
                processStatus.state = CStringHelper::trimCopy(tokens[1]);
            } else if (tokens[0] == "VmRSS") {
                CStringHelper::toNumber(tokens[1], processStatus.vmRSS);
            } else if (tokens[0] == "Threads") {
                CStringHelper::toNumber(tokens[1], processStatus.threads);
            }
        }

        return true;
    }

    static bool getSystemMemory(CSystemMemory &systemMemory) {
        std::ifstream steam("/proc/meminfo");

        if (!steam.is_open())
            return false;

        std::string line;

        while (std::getline(steam, line)) {
            std::vector<std::string> tokens = CStringHelper::split(line, ':');

            if (tokens.size() != 2)
                continue;

            if (tokens[0] == "MemTotal") {
                CStringHelper::toNumber(tokens[1], systemMemory.memoryTotal);
            }
        }

        return true;
    }

    static bool getImageBase(pid_t pid, const std::string &pathname, CProcessMap &processMap) {
        std::list<CProcessMap> processMaps;

        if (!getProcessMaps(pid, processMaps))
            return false;

        auto it = std::find_if(processMaps.begin(), processMaps.end(), [=](const auto &m) {
            return CStringHelper::findStringIC(m.pathname, pathname);
        });

        if (it == processMaps.end())
            return false;

        processMap = *it;

        return true;
    }

    static bool getProcessMaps(pid_t pid, std::list<CProcessMap> &processMaps) {
        std::string path = CPath::join("/proc", std::to_string(pid), "maps");
        std::ifstream stream(path);

        if (!stream.is_open())
            return false;

        std::string line;

        while (std::getline(stream, line)) {
            CStringHelper::trimExtraSpace(line);

            std::vector<std::string> tokens = CStringHelper::split(line, ' ');

            if (tokens.size() < PROCESS_MAP_FIELDS)
                continue;

            CProcessMap processMap = {};

            std::vector<std::string> address = CStringHelper::split(tokens[0], '-');

            if (address.size() != 2)
                continue;

            CStringHelper::toNumber(address[0], processMap.start, 16);
            CStringHelper::toNumber(address[1], processMap.end, 16);

            processMap.permission = tokens[1];
            CStringHelper::toNumber(tokens[2], processMap.offset, 16);
            processMap.device = tokens[3];
            CStringHelper::toNumber(tokens[4], processMap.inode);

            if (tokens.size() == 6)
                processMap.pathname = tokens[5];

            processMaps.push_back(processMap);
        }

        return true;
    };
};

#endif