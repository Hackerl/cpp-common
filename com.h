#ifndef COM_H
#define COM_H

#include "interface.h"
#include "utils/string_helper.h"

using GetInterfacePtr = int (*)(InterfaceID, void **);

#ifdef __linux__ 
#include <dlfcn.h>

constexpr auto EXTENSION = ".so";

static int CreateInstance(const char *path, InterfaceID iid, void **instance) {
    if (!instance)
        return -1;

    std::string libraryPath = path;

    if (!CStringHelper::endWith(libraryPath, EXTENSION))
        libraryPath.append(EXTENSION);

    void *handle = dlopen(libraryPath.c_str(), RTLD_LAZY);

    if (!handle)
        return -1;

    auto fn = (GetInterfacePtr)dlsym(handle, "GetInterface");

    if (!fn) {
        dlclose(handle);
        return -1;
    }

    return fn(iid, instance);
}

#elif _WIN32
#include <Windows.h>

constexpr auto EXTENSION = ".dll";

static int CreateInstance(const char *path, InterfaceID iid, void **instance) {
    if (!instance)
        return -1;

    std::string libraryPath = path;

    if (!CStringHelper::endWith(libraryPath, EXTENSION))
        libraryPath.append(EXTENSION);

    HMODULE handle = LoadLibraryExA(libraryPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!handle)
        return -1;

    auto fn = (GetInterfacePtr)GetProcAddress(handle, "GetInterface");

    if (!fn) {
        FreeLibrary(handle);
        return -1;
    }

    return fn(iid, instance);
}

#else
#error "unknown arch"
#endif

#endif