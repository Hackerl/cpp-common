#ifndef __Process_H__
#define __Process_H__
//******************************************************************************
#include "string_helper.h"
#include <fstream>
#include <list>

constexpr auto QueryProcessStatCount = 17;
constexpr auto QueryCPUStatCount = 8;

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
    std::string flags;
    unsigned long pageOff;
    std::string dev;
    unsigned long inode;
    std::string file;
};

class CProcess {
public:
    static bool getProcessState(pid_t pid, CProcessState& processState) {
        std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
        std::ifstream infile(statPath);

        if (!infile.is_open())
            return false;

        std::string line;
        std::getline(infile, line);

        if (line.empty())
            return false;

        auto stats = CStringHelper::split(line, ' ');

        if (stats.size() < QueryProcessStatCount)
            return false;

        auto index = 0;

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

    static bool getSystemState(CSystemState& systemState) {
        std::ifstream infile("/proc/stat");

        if (!infile.is_open())
            return false;

        std::string line;
        std::getline(infile, line);

        if (line.empty())
            return false;

        auto stats = CStringHelper::split(line, ' ');

        // may contain ""
        if (stats.size() < QueryCPUStatCount + 1)
            return false;

        auto index = 0;

        systemState.name = stats[index++];

        // skip ""
        if (stats[index].empty())
            index++;

        CStringHelper::toNumber(stats[index++], systemState.user);
        CStringHelper::toNumber(stats[index++], systemState.nice);
        CStringHelper::toNumber(stats[index++], systemState.system);
        CStringHelper::toNumber(stats[index++], systemState.idle);
        CStringHelper::toNumber(stats[index++], systemState.ioWait);
        CStringHelper::toNumber(stats[index++], systemState.irq);
        CStringHelper::toNumber(stats[index++], systemState.softIrq);

        return true;
    }

    static bool getProcessStatus(pid_t pid, CProcessStatus& processStatus) {
        std::string statPath = "/proc/" + std::to_string(pid) + "/status";
        std::ifstream infile(statPath);

        if (!infile.is_open())
            return false;

        std::string line;

        while (std::getline(infile, line)) {
            auto tokens = CStringHelper::split(line, ':');

            if (tokens.size() != 2)
                continue;

            auto key = tokens[0];
            auto value = tokens[1];

            if (key == "Name") {
                processStatus.name = CStringHelper::trimCopy(value);
            } else if (key == "State") {
                processStatus.state = CStringHelper::trimCopy(value);
            } else if (key == "VmRSS") {
                CStringHelper::toNumber(value, processStatus.vmRSS);
            } else if (key == "Threads") {
                CStringHelper::toNumber(value, processStatus.threads);
            }
        }

        return true;
    }

    static bool getSystemMemory(CSystemMemory& systemMemory) {
        std::string statPath = "/proc/meminfo";
        std::ifstream infile(statPath);

        if (!infile.is_open())
            return false;

        std::string line;

        while (std::getline(infile, line)) {
            auto tokens = CStringHelper::split(line, ':');

            if (tokens.size() != 2)
                continue;

            auto key = tokens[0];
            auto value = tokens[1];

            if (key == "MemTotal") {
                CStringHelper::toNumber(value, systemMemory.memoryTotal);
            }
        }

        return true;
    }

    static bool getProcessMaps(pid_t pid, std::list<CProcessMap>& processMaps) {
        std::string mapsPath = "/proc/" + std::to_string(pid) + "/maps";
        std::ifstream infile(mapsPath);

        if (!infile.is_open())
            return false;

        std::string line;

        while (std::getline(infile, line)) {
            CStringHelper::trimExtraSpace(line);

            auto tokens = CStringHelper::split(line, ' ');

            if (tokens.size() < 5)
                continue;

            CProcessMap processMap;

            auto vm = CStringHelper::split(tokens[0], '-');

            if (vm.size() != 2)
                continue;

            CStringHelper::toNumber(vm[0], processMap.start, 16);
            CStringHelper::toNumber(vm[1], processMap.end, 16);

            processMap.flags = tokens[1];
            CStringHelper::toNumber(tokens[2], processMap.pageOff, 16);
            processMap.dev = tokens[3];
            CStringHelper::toNumber(tokens[4], processMap.inode);

            if (tokens.size() == 6)
                processMap.file = tokens[5];

            processMaps.push_back(processMap);
        }

        return true;
    };
};
//******************************************************************************
#endif