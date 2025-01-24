#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER > 1700
#pragma comment(linker,"/NODEFAULTLIB:msvcrt.lib")
#else
#pragma comment(linker,"/FILEALIGN:0x200 /IGNORE:4078 /OPT:NOWIN98")
#pragma comment(linker,"/NODEFAULTLIB:libcmt.lib")
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include <tchar.h>
#pragma warning(disable : 4200)

#include "Functions.h"

#include "../Seu_lib/common.h"

// TODO: reference additional headers your program requires here
