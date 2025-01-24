// XScreenXor.h: interface for the XScreenXor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSCREENXOR_H__8CEE63D4_ACD7_456D_9539_465B47DC3530__INCLUDED_)
#define AFX_XSCREENXOR_H__8CEE63D4_ACD7_456D_9539_465B47DC3530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define _WINSOCKAPI_    // 防止包含 winsock.h
#include <windows.h>

class XScreenXor
{
public:
    XScreenXor();
    virtual ~XScreenXor();

    int MAXWIDTH;
    float Radio;

    void  SetColor(int iColor);
    void  InitGlobalVar();

    int   GetInfoSize() const;
    int   GetBmpSize() const;
    BYTE* GetBmpData();
    BYTE* GetBmpSaveData();
    void  CaputreFrame(DWORD dwFrame);
    void  CaputreFrameFirst(DWORD dwFrame);
    void  CaputreFrameNext(DWORD dwFrame);

    //显示端用的函数
    void SetInfoSize(const int iInfoSize);
    void SetBmpSize(const int iBmpSize);
    void LoadBmpData(const BYTE* pData);
    void LoadBmpSaveData(const BYTE* pSaveData);

    void XorFrame();
    HBITMAP GetBitmapFromData();

private:
    void SaveScreenBits();

    BOOL OpenUserDesktop();
    BOOL CloseUserDesktop();

private:
    BYTE* m_pData;    //bmp颜色数据指针
    BYTE* m_pDataSave;//存放前一帧位图数据

    int	m_BmpSize;    //记录图象数据的大小
    int m_InfoSize;   //位图头+调色板大小

    int m_ScrWidth;   //屏幕宽度
    int m_ScrHeight;  //屏幕高度

    int DestWidth;
    int DestHeigth;

    int m_nColor;     //图像颜色位数

    HDESK   hdeskCurrent;
    HDESK   hdesk;
    HWINSTA hwinstaCurrent;
    HWINSTA hwinsta;

    BITMAPINFOHEADER bi; //位图信息头

    HDC              hMemDC, hScreenDC;
    HBITMAP          hBitmap;
};

#endif // !defined(AFX_XSCREENXOR_H__8CEE63D4_ACD7_456D_9539_465B47DC3530__INCLUDED_)
