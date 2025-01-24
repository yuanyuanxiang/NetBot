#if !defined(AFX_LOGO_H__96A878BF_DCE1_4180_A3C4_75326834FB69__INCLUDED_)
#define AFX_LOGO_H__96A878BF_DCE1_4180_A3C4_75326834FB69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// logo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// logo dialog

class logo : public CDialog
{
// Construction
public:
    logo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(logo)
    enum { IDD = IDD_LOGO_DIALOG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(logo)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(logo)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGO_H__96A878BF_DCE1_4180_A3C4_75326834FB69__INCLUDED_)
