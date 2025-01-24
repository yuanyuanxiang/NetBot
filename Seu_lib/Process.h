#ifndef __SEURAT_PROCESS_H__
#define __SEURAT_PROCESS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Command.h"

//************************************
// Method:      ProcessList
// Returns:     void
// Parameter:   LPMsgHead lpMsgHead
// Parameter:   char *pBuf
// description: 列出进程
//************************************
void ProcessList(char *pBuf, LPMsgHead lpMsgHead);

//************************************
// Method:      ProcessKill
// Returns:     void
// Parameter:   LPMsgHead lpMsgHead
// Parameter:   char *pBuf
// description: 关闭进程ID的进程
//************************************
void ProcessKill(char *pBuf, LPMsgHead lpMsgHead);

#endif //__SEURAT_PROCESS_H__
