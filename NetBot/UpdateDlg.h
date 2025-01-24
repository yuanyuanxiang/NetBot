#if !defined(AFX_UPDATEDLG_H__4B1865D2_D586_4E97_B1AE_E41DED8DA2D3__INCLUDED_)
#define AFX_UPDATEDLG_H__4B1865D2_D586_4E97_B1AE_E41DED8DA2D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpdateDlg.h : header file
//
#include "ExControl/StatLink.h"
#include "ExClass/IniFile.h"
/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg dialog

class CUpdateDlg : public CDialog
{
// Construction
public:
    CUpdateDlg(CWnd* pParent = NULL);   // standard constructor

    enum { IDD = IDD_UPDATE_DLG };
    CStaticLink	m_3322;
    CComboBox	m_Combo_DnsIP;
    CComboBox	m_Combo_FtpIP;
    CString	m_DnsUser;
    CString	m_DnsPass;
    CString	m_DnsDomain;
    CString	m_FtpIP;
    UINT	m_FtpPort;
    CString	m_FtpPass;
    CString	m_FtpUser;
    CString	m_FtpUrl;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DWORD FtpUpdate();
    static DWORD __stdcall FtpUpdateThread(void* pThis);

    CIniFile m_Ini;
    void ReadIniFile();

    afx_msg void OnBtnDnsupdate();
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnFtpupdate();

    DECLARE_MESSAGE_MAP()
};

#endif
