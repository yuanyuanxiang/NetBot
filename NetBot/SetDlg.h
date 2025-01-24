#if !defined(AFX_SETDLG_H__22924D22_6FCC_455A_A386_692050E3046F__INCLUDED_)
#define AFX_SETDLG_H__22924D22_6FCC_455A_A386_692050E3046F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDlg.h : header file
//
#include "./ExClass/IniFile.h"

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog
class CSetDlg : public CDialog
{
// Construction
public:
    CSetDlg(CWnd* pParent = NULL);   // standard constructor

    enum { IDD = IDD_SET_DLG };
    int		m_ListenPort;
    int		m_ConnectMax;
    BOOL	m_Skin;
    BOOL	m_China;
    BOOL	m_WIN7;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    CIniFile m_Ini;
    void ReadIniFile();

    afx_msg void OnBtnSetsave1();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnBtnSetsave4();
    afx_msg void OnCheckSkin();
    afx_msg void OnCheckChina();
    afx_msg void OnCheckWin7();
    DECLARE_MESSAGE_MAP()
};

#endif
