#ifndef __SEURAT_DOSSHELL_H__
#define __SEURAT_DOSSHELL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Command.h"
//************************************
// Method:      DOSShell
// Access:      public
// Returns:     void
// Parameter:   LPMsgHead lpMsgHead
// Parameter:   char *pBuf
// description: Ö´ÐÐÔ¶³ÌShell
//************************************
void DOSShell(char *pBuf, LPMsgHead lpMsgHead);

#endif //__SEURAT_DOSSHELL_H__
