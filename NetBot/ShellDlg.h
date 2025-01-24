#if !defined(AFX_SHELLDLG_H__0A5383CB_2943_4569_8431_F761D761F21F__INCLUDED_)
#define AFX_SHELLDLG_H__0A5383CB_2943_4569_8431_F761D761F21F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellDlg.h : header file
//
#include "./ExControl/CmdEdit.h"

#include "../Seu_lib/Command.h"
#include <winsock.h>
/////////////////////////////////////////////////////////////////////////////
// CShellDlg dialog

class CShellDlg : public CDialog
{
// Construction
public:
    CShellDlg(CWnd* pParent = NULL);   // standard constructor

public:
    void SetConnSocket(SOCKET socket);
    void StatusTextOut(int iPane, LPCTSTR ptzFormat, ...);

    void OnWorkBegin();
    void OnWorkEnd();

    enum { IDD = IDD_SHELL_DLG };
    CCmdEdit	m_CmdEdit;
    CString	m_strCmdLine;

    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy();

// Implementation
protected:
    SOCKET  m_ConnSocket;
    MsgHead m_MsgHead;            //消息头
    char    m_Buffer[512 * 1024]; //数据缓冲区

    HANDLE m_hWorkThread;
    DWORD DOSShell();
    static DWORD __stdcall DOSShellThread(void* pThis);

    void StopWork();

    CStatusBarCtrl    m_wndStatusBar;

    afx_msg void OnBtnShellrun();
    virtual BOOL OnShowWindow(BOOL bShow, UINT nStatus);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnShellSave();
    afx_msg void OnShellClear();
    afx_msg void OnShellExit();
    afx_msg void OnShellHelp();
    afx_msg void OnShellAbout();

    afx_msg LRESULT OnSave(WPARAM   wparam,LPARAM   lparam);
    DECLARE_MESSAGE_MAP()
};

#endif
