#if !defined(AFX_XPICTUREBOX_H__D5FA7D84_3039_4C7B_9F6E_3CAE1AD87414__INCLUDED_)
#define AFX_XPICTUREBOX_H__D5FA7D84_3039_4C7B_9F6E_3CAE1AD87414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XPictureBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XPictureBox window

class XPictureBox : public CWnd
{
// Construction
public:
    XPictureBox();

public:
    void SetTipText(CString strText);
    BOOL SaveBmp(const char *FileName);
    void SetBitmap(HBITMAP hBitmap);
    void CleanBitmap();

// Implementation
public:
    virtual ~XPictureBox();

    // Generated message map functions
protected:
    CString m_TipText;
    CBitmap m_hBitmap;
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

#endif
