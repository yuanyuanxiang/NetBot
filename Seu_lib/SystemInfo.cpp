#include "stdafx.h"
#include "SystemInfo.h"
#include "Functions.h"

BOOL GetSystemInfo(SysInfo& info)
{
    ZeroMemory(&info, sizeof(SysInfo));

    DWORD iSize = 64;
    GetComputerName(info.cComputer, &iSize);

    char szSystem[32];
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (!GetVersionEx((OSVERSIONINFO *)&osvi))
        return FALSE;

    switch (osvi.dwPlatformId) {
    case VER_PLATFORM_WIN32_NT:
        if (osvi.dwMajorVersion == 6) {
            if (osvi.dwMinorVersion == 0) lstrcpy(szSystem, "Windows Vista");
            if (osvi.dwMinorVersion == 1) lstrcpy(szSystem, "Windows 7");
            if (osvi.dwMinorVersion == 2) lstrcpy(szSystem, "Windows 8");
            if (osvi.dwMinorVersion == 3) lstrcpy(szSystem, "Windows 8.1");
        } else if (osvi.dwMajorVersion == 5) {
            if (osvi.dwMinorVersion == 0) lstrcpy(szSystem, "Windows 2000");
            if (osvi.dwMinorVersion == 1) lstrcpy(szSystem, "Windows XP");
            if (osvi.dwMinorVersion == 2) lstrcpy(szSystem, "Windows 2003");
        } else if (osvi.dwMajorVersion == 10) lstrcpy(szSystem, "Windows 10");
        else if (osvi.dwMajorVersion <= 4) lstrcpy(szSystem, "Win NT");
        break;

    case VER_PLATFORM_WIN32_WINDOWS:
        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
            lstrcpy(szSystem, "Win 95");

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
            lstrcpy(szSystem, "Win 98");
        break;
    }
    if (IsX64System()) {
        lstrcat(szSystem, " x64");
    } else {
        lstrcat(szSystem, " x86");
    }
    wsprintf(info.cOS, "%s SP%d (Build %d)", szSystem, osvi.wServicePackMajor, osvi.dwBuildNumber);

    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);

    typedef void(WINAPI* FunctionGlobalMemoryStatusEx)(LPMEMORYSTATUSEX);
    FunctionGlobalMemoryStatusEx GlobalMemoryStatusEx;
    GlobalMemoryStatusEx = (FunctionGlobalMemoryStatusEx)GetProcAddress(GetModuleHandle("kernel32.dll"), "GlobalMemoryStatusEx");

    GlobalMemoryStatusEx(&mem);//调用函数取得系统的内存情况

    wsprintf(info.cMemorySize, "%dMB", mem.ullTotalPhys / 1024 / 1024 + 1);

    return TRUE;
}
