#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0502

#include <tchar.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include "../Seu_lib/SystemInfo.h"
#include "../Seu_lib/Command.h"

#include "../Seu_lib/common.h"

// TODO: reference additional headers your program requires here
