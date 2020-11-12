#ifndef __COM_H__
#define __COM_H__
//******************************************************************************
#include "interface.h"
#include "utils/string_helper.h"
//******************************************************************************
typedef int(*PFN_GetInterface)(InterfaceID, void**);
//******************************************************************************
#ifdef __linux__ 
//******************************************************************************
#include <dlfcn.h>
//******************************************************************************
constexpr auto LibraryExtension = ".so";
//******************************************************************************
inline int CreateInstance(const char* path, InterfaceID iid, void** lpInterface)
{
    if (!lpInterface)
        return -1;

    std::string libraryPath = path;

    if (!CStringHelper::endWith(libraryPath, LibraryExtension))
        libraryPath.append(LibraryExtension);

    void* DLHandle = dlopen(libraryPath.c_str(), RTLD_LAZY);

    if (!DLHandle)
        return -1;

    auto pfnGetInterface = (PFN_GetInterface)dlsym(DLHandle, "GetInterface");

    if (!pfnGetInterface) {
        dlclose(DLHandle);
        return -1;
    }

    return pfnGetInterface(iid, lpInterface);
}
//******************************************************************************
#elif _WIN32
//******************************************************************************
#include <Windows.h>
//******************************************************************************
constexpr auto LibraryExtension = ".dll";
//******************************************************************************
inline int CreateInstance(const char* path, InterfaceID iid, void** lpInterface)
{
    if (!lpInterface)
        return -1;

    std::string libraryPath = path;

    if (!CStringHelper::endWith(libraryPath, LibraryExtension))
        libraryPath.append(LibraryExtension);

    HMODULE DLHandle = LoadLibraryExA(libraryPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!DLHandle)
        return -1;

    auto pfnGetInterface = (PFN_GetInterface)GetProcAddress(DLHandle, "GetInterface");

    if (!pfnGetInterface) {
        FreeLibrary(DLHandle);
        return -1;
    }

    return pfnGetInterface(iid, lpInterface);;
}
//******************************************************************************
#else
#error "OS not supported!"
#endif
//******************************************************************************
#endif