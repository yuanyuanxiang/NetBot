// XScreenXor.cpp: implementation of the XScreenXor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "XScreenXor.h"
#include <tchar.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
// #define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XScreenXor::XScreenXor()
{
    m_pData = NULL;
    m_pDataSave = NULL;
    m_BmpSize = 0;
    m_InfoSize = 0;
    m_ScrWidth = 0;
    m_ScrHeight = 0;
    m_nColor = 8;//默认256色
    MAXWIDTH = 1920;
    Radio = 1;

    OpenUserDesktop();
}

XScreenXor::~XScreenXor()
{
    //清除
    DeleteDC(hMemDC);
    DeleteDC(hScreenDC);
    DeleteObject(hBitmap);

    if (m_pData != NULL)
        delete[] m_pData;

    if (m_pDataSave != NULL)
        delete[] m_pDataSave;

    //CloseUserDesktop();
}

void XScreenXor::SetColor(int iColor)
{
    switch (iColor) {
    case 8:
    case 16:
    case 24:
    case 32:
        m_nColor = iColor;
        break;

    default:
        m_nColor = 8;
    }
}

void XScreenXor::InitGlobalVar()
{
    //获得屏幕大小
    m_ScrWidth = GetSystemMetrics(SM_CXSCREEN);
    m_ScrHeight = GetSystemMetrics(SM_CYSCREEN);

    if(m_ScrWidth > MAXWIDTH) {
        Radio = (float)m_ScrWidth / MAXWIDTH;
        DestHeigth = m_ScrHeight / Radio;
        DestWidth = MAXWIDTH;
    } else {
        Radio = 1;
        DestHeigth = m_ScrHeight;
        DestWidth = m_ScrWidth;
    }

    //计算位图头大小和位图大小
    int biSize = sizeof(BITMAPINFOHEADER);
    if (m_nColor > 8)
        m_InfoSize = biSize;
    else
        m_InfoSize = biSize + (1 << m_nColor) * sizeof(RGBQUAD);

    m_BmpSize = m_InfoSize + ((DestWidth * m_nColor + 31) / 32 * 4) * DestHeigth;

    //申请位图存储空间
    if (m_pData != NULL)
        delete[] m_pData;
    m_pData = new BYTE[m_BmpSize];

    if (m_pDataSave != NULL)
        delete[] m_pDataSave;
    m_pDataSave = new BYTE[m_BmpSize];

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = DestWidth;
    bi.biHeight = DestHeigth;
    bi.biPlanes = 1;
    bi.biBitCount = m_nColor;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    //获取桌面HDC
    hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    //为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScreenDC);
    //创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScreenDC, DestWidth, DestHeigth);
}

int XScreenXor::GetInfoSize() const
{
    return m_InfoSize;
}

int XScreenXor::GetBmpSize() const
{
    return m_BmpSize;
}

BYTE* XScreenXor::GetBmpData()
{
    return m_pData;
}

BYTE* XScreenXor::GetBmpSaveData()
{
    return m_pDataSave;
}

void XScreenXor::SetInfoSize(const int iInfoSize)
{
    m_InfoSize = iInfoSize;
}

void XScreenXor::SetBmpSize(const int iBmpSize)
{
    m_BmpSize = iBmpSize;

    if (m_pData != NULL)
        delete[] m_pData;
    m_pData = new BYTE[m_BmpSize];

    if (m_pDataSave != NULL)
        delete[] m_pDataSave;
    m_pDataSave = new BYTE[m_BmpSize];
}

void XScreenXor::LoadBmpData(const BYTE* pData)
{
    int iCount = m_BmpSize / sizeof(DWORD);
    for (int i = 0; i < iCount; i++) {
        ((PDWORD)m_pData)[i] = ((PDWORD)pData)[i];
    }
}

void XScreenXor::LoadBmpSaveData(const BYTE* pSaveData)
{
    int iCount = m_BmpSize / sizeof(DWORD);
    for (int i = 0; i < iCount; i++) {
        ((PDWORD)m_pDataSave)[i] = ((PDWORD)pSaveData)[i];
    }
}

void  XScreenXor::CaputreFrame(DWORD dwFrame)
{
    //截取屏幕
    SaveScreenBits();

    int iCount = m_BmpSize / sizeof(DWORD);
    if (dwFrame == 0) {
        for (int i = 0; i < iCount; i++) {
            ((PDWORD)m_pDataSave)[i] = ((PDWORD)m_pData)[i];
        }
    }

    if (dwFrame > 0) {
        for (int i = 0; i < iCount; i++) {
            ((PDWORD)m_pData)[i] ^= ((PDWORD)m_pDataSave)[i];
            ((PDWORD)m_pDataSave)[i] ^= ((PDWORD)m_pData)[i];
        }
    }
}

void  XScreenXor::CaputreFrameFirst(DWORD dwFrame)
{
    //截取屏幕
    SaveScreenBits();

    memcpy(m_pDataSave, m_pData, m_BmpSize);

    // 	int iCount = m_BmpSize / sizeof(DWORD);
    // 	for (int i = 0; i < iCount; i++)
    // 	{
    // 		((PDWORD)m_pDataSave)[i] = ((PDWORD)m_pData)[i];
    // 	}
}

void  XScreenXor::CaputreFrameNext(DWORD dwFrame)
{
    //截取屏幕
    SaveScreenBits();

    int iCount = m_BmpSize / sizeof(DWORD);

    for (int i = 0; i < iCount; i++) {
        //((PDWORD)m_pData)[i] ^= ((PDWORD)m_pDataSave)[i];
        //((PDWORD)m_pDataSave)[i] ^= ((PDWORD)m_pData)[i];

        ((PDWORD)m_pDataSave)[i] ^= ((PDWORD)m_pData)[i];
    }

    BYTE* tmp = m_pDataSave;
    m_pDataSave = m_pData;
    m_pData = tmp;
}

//you should call function 'InitGlobalVar' first
inline void XScreenXor::SaveScreenBits()
{
    PBITMAPINFO lpBmpInfo;	//位图信息

    // 把新位图选到内存设备描述表中
    SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    //::BitBlt(hMemDC, 0, 0, m_ScrWidth, m_ScrHeight, hScreenDC, 0, 0, SRCCOPY);

    //SetStretchBltMode(hScreenDC, HALFTONE);
    SetStretchBltMode(hScreenDC, COLORONCOLOR);

    ::StretchBlt(hMemDC, 0, 0, DestWidth, DestHeigth, hScreenDC, 0, 0, m_ScrWidth, m_ScrHeight, SRCCOPY);

    lpBmpInfo = PBITMAPINFO(m_pData);
    //把数据拷进去
    memcpy(m_pData, &bi, sizeof(BITMAPINFOHEADER));

    ::GetDIBits(
        hMemDC,
        hBitmap,
        0,
        DestHeigth,
        m_pData + m_InfoSize,
        lpBmpInfo,
        DIB_RGB_COLORS);
}

void XScreenXor::XorFrame()
{
    //更新差异到m_pData
    int iCount = m_BmpSize / sizeof(DWORD);
    for (int i = 0; i < m_BmpSize; i++) {
        ((PDWORD)m_pData)[i] ^= ((PDWORD)m_pDataSave)[i];
    }
}

HBITMAP XScreenXor::GetBitmapFromData()
{
    HBITMAP hBitmap;
    PBITMAPINFO lpBmpInfo; //位图信息

    lpBmpInfo = PBITMAPINFO(m_pData);

    HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    // 创建DDB位图
    hBitmap = CreateDIBitmap(
                  hDC,
                  &lpBmpInfo->bmiHeader,
                  CBM_INIT,
                  m_pData + m_InfoSize,
                  lpBmpInfo,
                  DIB_RGB_COLORS);

    DeleteDC(hDC);

    return hBitmap;
}

////////////////////////////////////////////////////////////////////////////
//屏幕传输会出现白屏,可能有两个原因:
//一是系统处于锁定或未登陆桌面.
//二是处于屏幕保护桌面.
//这时候要将当前桌面切换到该桌面才能抓屏.
BOOL XScreenXor::OpenUserDesktop()
{
    hwinstaCurrent = GetProcessWindowStation();
    if (hwinstaCurrent == NULL)
        return FALSE;

    hdeskCurrent = GetThreadDesktop(GetCurrentThreadId());
    if (hdeskCurrent == NULL)
        return FALSE;

    hwinsta = OpenWindowStation(_T("winsta0"), FALSE,
                                WINSTA_ACCESSCLIPBOARD |
                                WINSTA_ACCESSGLOBALATOMS |
                                WINSTA_CREATEDESKTOP |
                                WINSTA_ENUMDESKTOPS |
                                WINSTA_ENUMERATE |
                                WINSTA_EXITWINDOWS |
                                WINSTA_READATTRIBUTES |
                                WINSTA_READSCREEN |
                                WINSTA_WRITEATTRIBUTES);
    if (hwinsta == NULL)
        return FALSE;

    if (!SetProcessWindowStation(hwinsta))
        return FALSE;

    hdesk = OpenDesktop(_T("default"), 0, FALSE,
                        DESKTOP_CREATEMENU |
                        DESKTOP_CREATEWINDOW |
                        DESKTOP_ENUMERATE |
                        DESKTOP_HOOKCONTROL |
                        DESKTOP_JOURNALPLAYBACK |
                        DESKTOP_JOURNALRECORD |
                        DESKTOP_READOBJECTS |
                        DESKTOP_SWITCHDESKTOP |
                        DESKTOP_WRITEOBJECTS);
    if (hdesk == NULL)
        return FALSE;

    SetThreadDesktop(hdesk);

    return TRUE;
}

BOOL XScreenXor::CloseUserDesktop()
{
    if (!SetProcessWindowStation(hwinstaCurrent))
        return FALSE;

    if (!SetThreadDesktop(hdeskCurrent))
        return FALSE;

    if (!CloseWindowStation(hwinsta))
        return FALSE;

    if (!CloseDesktop(hdesk))
        return FALSE;

    return TRUE;
}
