#ifndef __DNS_H__
#define __DNS_H__
//******************************************************************************
#include <vector>

#ifdef __linux__
#include <netdb.h>
#elif _WIN32
#include <WinSock2.h>
#else
#error "OS not supported!"
#endif

class CDnsHelper {
public:
    static std::vector<in_addr> queryAdvance(const char * hostName) {
        std::vector<in_addr> IPList;

        addrinfo * dnsInfo;
        addrinfo hints = {};

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        int res = getaddrinfo(hostName, nullptr, &hints, &dnsInfo);

        if (res) {
            return IPList;
        }

        for(addrinfo * i = dnsInfo; i != nullptr; i = i->ai_next)
            IPList.push_back(((sockaddr_in *)i->ai_addr)->sin_addr);

        freeaddrinfo(dnsInfo);

        return IPList;
    }
};
//******************************************************************************
#endif