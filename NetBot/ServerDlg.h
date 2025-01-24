#if !defined(AFX_SERVERDLG_H__F79ACBA2_0E25_4EB1_BA23_9B12BB2A35B2__INCLUDED_)
#define AFX_SERVERDLG_H__F79ACBA2_0E25_4EB1_BA23_9B12BB2A35B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//
#include "./ExClass/IniFile.h"
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
    CServerDlg(CWnd* pParent = NULL);   // standard constructor
    char  ServerUrl[256];

    char Path[512];

    enum { IDD = IDD_SERVER_DLG };
    CListBox	m_LogList;
    CProgressCtrl	m_ServerProgress;
    CString	m_Url;
    CString	m_ServiceName;
    CString	m_ServiceDisp;
    CString	m_ServiceDesc;
    CString	m_port;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    UINT CompressType;
    CIniFile m_Ini;
    void ReadIniFile();

    int Compress(char File[], DWORD id);
    void CompressFsg();
    void CompressUpx();

    virtual BOOL OnInitDialog();
    afx_msg void OnOk();

    afx_msg void OnCompressType(UINT nID);
    afx_msg void OnRelpaceService(UINT nID);
    DECLARE_MESSAGE_MAP()
};

#endif
