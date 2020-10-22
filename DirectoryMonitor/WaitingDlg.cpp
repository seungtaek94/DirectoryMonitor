// WaitingDlg.cpp : ���� �����Դϴ�.
//

#include "pch.h"
//#include "DirectoryMonitor.h"
//#include "DirectoryMonitorDlg.h"
#include "WaitingDlg.h"

// CWaitingDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWaitingDlg, CDialog)

	CWaitingDlg::CWaitingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitingDlg::IDD, pParent)
{

}

CWaitingDlg::~CWaitingDlg()
{
}

void CWaitingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SEARCH, m_ProgressWait);
}


BEGIN_MESSAGE_MAP(CWaitingDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWaitingDlg �޽��� ó�����Դϴ�.
BOOL CWaitingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	m_ProgressWait.SetShowPercent(false);

	CenterWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CWaitingDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CWaitingDlg::ProgressStart(CString strMsg)
{
	//g_pMainDlg->m_bProgressCancel = FALSE;
	CenterWindow();
	m_ProgressWait.SetShowPercent(false);
	m_ProgressWait.SetWindowText(strMsg);
	m_ProgressWait.SetMarquee(true,30);
	m_ProgressWait.Invalidate();

	ShowWindow(SW_SHOW);
	UpdateWindow();
}
void CWaitingDlg::ProgressStop()
{
	m_ProgressWait.SetMarquee(false,30);
	m_ProgressWait.SetPos(0);

	ShowWindow(SW_HIDE);
	//g_pMainDlg->m_bProgressCancel = TRUE;
}

void CWaitingDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//CDialog::OnClose();
	//g_pMainDlg->m_bProgressCancel = TRUE;
	ShowWindow(SW_HIDE);
}
