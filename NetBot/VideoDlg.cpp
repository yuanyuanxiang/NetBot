// VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "VideoDlg.h"
#include "ExClass/AutoLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoDlg dialog

#define WM_UPDATE_PROGRESS WM_USER + 0x3000

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVideoDlg::IDD, pParent)
{
    m_ConnSocket = INVALID_SOCKET;
    m_bStop = FALSE;
    m_pAviFile = NULL;
}


void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_PROGRESS_VIDEO, m_Progress);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BTN_RECORDS, OnBtnRecords)
    ON_BN_CLICKED(IDC_BTN_RECORDE, OnBtnRecorde)
    ON_MESSAGE(WM_UPDATE_PROGRESS, OnUpdateProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoDlg message handlers
void CVideoDlg::SetConnSocket(SOCKET socket)
{
    m_ConnSocket = socket;

    sockaddr_in addr;
    int cb = sizeof(addr);
    int ir = getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
    CString OnlineIP;
    OnlineIP.Format("%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//ntohs������u_longתint

    SetWindowText("[��Ƶ��׽] " + OnlineIP);

    VideoStart();
}

void CVideoDlg::StatusTextOut(int iPane, LPCTSTR ptzFormat, ...)
{
    TCHAR tzText[1024];

    va_list vlArgs;
    va_start(vlArgs, ptzFormat);
    wvsprintf(tzText, ptzFormat, vlArgs);
    va_end(vlArgs);

    m_wndStatusBar.SetText(tzText, iPane, 0);
}

BOOL CVideoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Set the small icon for this dialog
    SetIcon(AfxGetApp()->LoadIcon(IDI_LIST_VIDEO), FALSE);
    CenterWindow();
    // TODO: Add extra initialization here
    //create statusbar=============================
    m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1300001);
    int strPartDim[2] = { 250,-1 };
    m_wndStatusBar.SetParts(2, strPartDim);

    //create picture box
    m_PicBox.Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL, CRect(0, 0, 1, 1), this, 0);
    CRect rc;
    GetClientRect(&rc);
    rc.bottom -= 50;
    m_PicBox.MoveWindow(&rc);
    m_PicBox.SetTipText("���Ժ��豸���ڳ�ʼ������");
    hRecvVideoThread = NULL;
    return TRUE;
}

void CVideoDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    if (m_pAviFile != NULL)
        delete m_pAviFile;
    m_pAviFile = NULL;

    m_bStop = TRUE;
    VideoStop();

    //��ģʽ�Ի�����Ҫ�������ٶԻ���
    DestroyWindow();
    //	CDialog::OnCancel();
}

void CVideoDlg::PostNcDestroy()
{
    // TODO: Add your specialized code here and/or call the base class
    // delete this;
    CDialog::PostNcDestroy();
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN) {
        int nVirtKey = (int)pMsg->wParam;
        if (nVirtKey == VK_RETURN) {
            //����ǻس�����������Ҫ��������,����ʲôҲ����
            return TRUE;
        }
        if (nVirtKey == VK_ESCAPE) {
            //�����ESC����������Ҫ��������,����ʲôҲ����
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CVideoDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if (m_PicBox.m_hWnd == NULL)
        return;

    CRect rc;
    GetClientRect(&rc);
    rc.bottom -= 50;
    m_PicBox.MoveWindow(&rc);
}

DWORD CVideoDlg::RecvVideo()
{
    CAutoRelease a(hRecvVideoThread);

    MsgHead msgHead;
    SafeBuffer chBuffer(256);
    //send socket type
    if (!RecvMsg(m_ConnSocket, chBuffer, &msgHead)) {
        MessageBox("��������ʧ��!", "��ʾ");
        closesocket(m_ConnSocket);
        return 0;//send socket type error
    }

    //��������
    switch (msgHead.dwCmd) {
    case 0:
        Mprintf(">>> ����ͷ�����ɹ���׼����������.\n");
        break;
    case 1: {
        MessageBox("�豸�����ڻ���������ռ��!", "��ʾ");
        PostMessage(WM_COMMAND, IDCANCEL);
        return 0;
    }
    break;
    case 2: {
        MessageBox("��Ƶ�豸��ʼ��ʧ��!", "��ʾ");
        PostMessage(WM_COMMAND, IDCANCEL);
        return 0;
    }
    break;
    default: {
        MessageBox("δ֪����!", "��ʾ");
        PostMessage(WM_COMMAND, IDCANCEL);
        return 0;
    }
    break;
    }

    LPBITMAPINFO pBmpheader = LPBITMAPINFO(chBuffer.c_str());
    CDC *dc = GetDC();
    BYTE *buffer = NULL;
    while (!m_bStop) {
        if (recv(m_ConnSocket, (char*)&msgHead, sizeof(MsgHead), 0) > 0) {
            BYTE* pCompress = new BYTE[msgHead.dwExtend2];
            BYTE* pUnCompress = new BYTE[msgHead.dwExtend1];
            //����֡����ʵ�ʳ��Ƚ��ܸ�֡����
            ULONG iRecvLen = 0, iRecved;
            while (iRecvLen < msgHead.dwSize) {
                if ((msgHead.dwSize - iRecvLen) >= BUFFER_MAXLEN) {
                    iRecved = recv(m_ConnSocket, (char*)pCompress + iRecvLen, BUFFER_MAXLEN, 0);
                    if (iRecved == SOCKET_ERROR) {
                        closesocket(m_ConnSocket);
                        break;
                    }
                } else {
                    iRecved = recv(m_ConnSocket, (char*)pCompress + iRecvLen, msgHead.dwSize - iRecvLen, 0);
                    if (iRecved == SOCKET_ERROR) {
                        closesocket(m_ConnSocket);
                        break;
                    }
                }
                iRecvLen += iRecved;
                PostMessage(WM_UPDATE_PROGRESS, iRecvLen, msgHead.dwSize); // ���ͽ�����������Ϣ
            }
            /////////////////////////////////////////////
            DWORD lenthUncompress = msgHead.dwExtend1;
            uncompress(pUnCompress,
                       &lenthUncompress,
                       pCompress,
                       msgHead.dwExtend2);

            //��ʾͼ��
            HBITMAP hBitmap = GetBitmapFromData(dc->GetSafeHdc(), pBmpheader, pUnCompress, &buffer);
            if (m_pAviFile != NULL)
                m_pAviFile->AppendNewFrame(hBitmap);
            m_PicBox.SetBitmap(hBitmap);
            StatusTextOut(1, _T("�ѽ��� %d ֡"), msgHead.dwCmd + 1);

            delete[] pCompress;
            delete[] pUnCompress;

            ::Sleep(1);
        }
    }
    ///////////////////////////
    ReleaseDC(dc);
    SAFE_DELETEARRAY(buffer);

    return 1;
}

DWORD CVideoDlg::RecvVideoThread(void* pThis)
{
    return ((CVideoDlg *)pThis)->RecvVideo();
}

BOOL CVideoDlg::VideoStart()
{
    //���������߳�
    hRecvVideoThread = CreateThread(NULL,
                                    0,
                                    (LPTHREAD_START_ROUTINE)RecvVideoThread,
                                    this,
                                    0,
                                    NULL);
    if (hRecvVideoThread == NULL)
        return FALSE;
    else
        return TRUE;
}

void CVideoDlg::VideoStop()
{
    //�ر�socket
    shutdown(m_ConnSocket, 0x02);
    closesocket(m_ConnSocket);
    // �ȴ��߳̽���
    WAIT_CONDITION(hRecvVideoThread);
}

FORCEINLINE BYTE clamp(BYTE value, BYTE min=0, BYTE max=255) {
    return value < min ? min : (value > max ? max : value);
}

void YUV2ToRGB(BYTE* yuv, BYTE* rgb, int width, int height) {
	int index = 0;
	for (int i = 0; i < width * height * 2; i += 4) {
        BYTE Y0 = yuv[i];
        BYTE U = yuv[i + 1];
        BYTE Y1 = yuv[i + 2];
        BYTE V = yuv[i + 3];

		// ��һ������
		rgb[index++] = clamp(Y0 + 1.402 * (V - 128));         // R
		rgb[index++] = clamp(Y0 - 0.344136 * (U - 128) - 0.714136 * (V - 128)); // G
		rgb[index++] = clamp(Y0 + 1.772 * (U - 128));         // B

		// �ڶ�������
		rgb[index++] = clamp(Y1 + 1.402 * (V - 128));         // R
		rgb[index++] = clamp(Y1 - 0.344136 * (U - 128) - 0.714136 * (V - 128)); // G
		rgb[index++] = clamp(Y1 + 1.772 * (U - 128));         // B
	}
}

void flipImage(BYTE* rgb, int width, int height) {
	int rowBytes = width * 3;  // ÿ�е��ֽ�����24 λͼ��ÿ������ 3 �ֽڣ�

	// ��ʱ������
    BYTE* tempRow = new BYTE[rowBytes];

	// ��תͼ����
	for (int y = 0; y < height / 2; ++y) {
        BYTE* topRow = rgb + y * rowBytes;
        BYTE* bottomRow = rgb + (height - y - 1) * rowBytes;

		// �������к͵���
		memcpy(tempRow, topRow, rowBytes);
		memcpy(topRow, bottomRow, rowBytes);
		memcpy(bottomRow, tempRow, rowBytes);
	}

	delete[] tempRow;
}

HBITMAP CVideoDlg::GetBitmapFromData(HDC hDC, LPBITMAPINFO lpBmpInfo, BYTE* pDibData, BYTE** buffer)
{
    BITMAPINFO info = *lpBmpInfo;
    BYTE* data = pDibData;
    if (info.bmiHeader.biCompression == 844715353)// YUV2
    {
        int width = info.bmiHeader.biWidth;
        int height = info.bmiHeader.biHeight;
        info.bmiHeader.biCompression = BI_RGB;
        info.bmiHeader.biSizeImage = width * height * 3;
        info.bmiHeader.biBitCount = 24;
        // ÿ������ 3 ���ֽ� (R, G, B)
        if (NULL == *buffer)
            *buffer = new BYTE[width * height * 3];
        YUV2ToRGB(pDibData, *buffer, width, height);
        flipImage(*buffer, width, height);
        data = *buffer;
    }
    return CreateDIBitmap(hDC, &info.bmiHeader, CBM_INIT, data, &info, DIB_RGB_COLORS);
}

void CVideoDlg::OnBtnRecords()
{
    // TODO: Add your control notification handler code here
    CString strAVIFile, strAVIName;
    CFileDialog dlgFileOpen(FALSE, ".avi", NULL, OFN_HIDEREADONLY, "AVI Files(*.avi)|*.avi||");
    if (dlgFileOpen.DoModal() != IDOK)
        return;
    strAVIFile = dlgFileOpen.GetPathName();
    strAVIName = dlgFileOpen.GetFileName();

    if (strAVIFile.IsEmpty()) {
        MessageBox(_T("¼���ļ����Ʋ���Ϊ��,��ʼ¼��ʧ��!"), _T("��ʼ¼��"), MB_ICONEXCLAMATION | MB_ICONERROR);
        return;
    }


    if (m_pAviFile != NULL)
        delete m_pAviFile;
    m_pAviFile = new CAviFile(strAVIFile, 0, 5);
    if (!m_pAviFile) {
        MessageBox(_T("�½���Ƶ¼���ļ�ʧ��!"), _T("��ʼ¼��"), MB_ICONEXCLAMATION | MB_ICONERROR);
    } else {
        StatusTextOut(0, _T("¼��Ϊ�ļ� %s"), strAVIName);
        GetDlgItem(IDC_BTN_RECORDS)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_RECORDE)->EnableWindow(TRUE);
    }
}

void CVideoDlg::OnBtnRecorde()
{
    // TODO: Add your control notification handler code here
    if (m_pAviFile != NULL)
        delete m_pAviFile;
    m_pAviFile = NULL;

    StatusTextOut(0, _T(""));
    GetDlgItem(IDC_BTN_RECORDS)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_RECORDE)->EnableWindow(FALSE);
}

// wParam: currentProgress, lParam: maxProgress
LRESULT CVideoDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	// ���յ����Թ����̵߳Ľ��ȸ�����Ϣ
	int currentProgress = (int)wParam;
	int maxProgress = (int)lParam;

	if (m_Progress.GetSafeHwnd())  // ���������ؼ��Ƿ���Ч
	{
		m_Progress.SetRange(0, maxProgress); // ���ý�������Χ
		m_Progress.SetPos(currentProgress);  // ���õ�ǰ����
	}

	return 0;
}
