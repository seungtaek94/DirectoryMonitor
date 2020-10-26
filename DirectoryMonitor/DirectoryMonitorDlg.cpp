
// DirectoryMonitorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DirectoryMonitor.h"
#include "DirectoryMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDirectoryMonitorDlg 대화 상자



CDirectoryMonitorDlg::CDirectoryMonitorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTORYMONITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDirMonitorThread = NULL;
	m_pListenDirAlterdThread = NULL;
	m_listIndex = 0;
}

void CDirectoryMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DIR_PATH, m_editDirPath);
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Control(pDX, IDC_PROGRESS_MONITORING, m_ProgressWait);
}

BEGIN_MESSAGE_MAP(CDirectoryMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_MONITORING, &CDirectoryMonitorDlg::OnBnClickedBtnStartMonitoring)
	ON_BN_CLICKED(IDC_BTN_FINISH_MONITORING, &CDirectoryMonitorDlg::OnBnClickedBtnFinishMonitoring)
	ON_BN_CLICKED(IDC_BTN_SELECT_DIR, &CDirectoryMonitorDlg::OnBnClickedBtnSelectDir)
END_MESSAGE_MAP()


// CDirectoryMonitorDlg 메시지 처리기

BOOL CDirectoryMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	ProgressStop(_T("Stopped !!"));

	if (m_pDirMonitorThread == NULL)
	{
		m_pDirMonitorThread = new CDirectoryMonitorThread;
	}

	InitListCrl();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDirectoryMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDirectoryMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDirectoryMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDirectoryMonitorDlg::InitListCrl()
{
	ListView_SetExtendedListViewStyleEx(m_listFile.m_hWnd, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);
	ListView_SetExtendedListViewStyleEx(m_listFile.m_hWnd, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_listFile.m_hWnd, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyleEx(m_listFile.m_hWnd, LVS_EX_SUBITEMIMAGES, LVS_EX_SUBITEMIMAGES);

	m_listFile.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 50);
	m_listFile.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 135);
	m_listFile.InsertColumn(2, _T("Action Type"), LVCFMT_LEFT, 135);
	m_listFile.InsertColumn(3, _T("File Name"), LVCFMT_LEFT, 500);

	m_listFile.EnableToolTips(TRUE);
	m_listFile.SetBkColor(RGB(255, 255, 255));
}


CString CDirectoryMonitorDlg::GetDirPath()
{
	CString _strPath;
	CFolderPickerDialog Picker(_T(""), OFN_FILEMUSTEXIST);
	Picker.GetOFN().lpstrTitle = _T("Please Select Directory For Monitoring");

	if (Picker.DoModal() == IDOK)
	{
		return Picker.GetFolderPath();
	}

	return _T("");
}

UINT CDirectoryMonitorDlg::ListenDirAlterd(LPVOID _pMain)
{
	CDirectoryMonitorDlg* pMainDlg = (CDirectoryMonitorDlg*)_pMain;
	FILE_ACTION_INFO _fileActionInfo;
	while (TRUE)
	{
		if (pMainDlg->m_pDirMonitorThread->IsEmpty()) {
			Sleep(100);
			continue;
		}

		_fileActionInfo = pMainDlg->m_pDirMonitorThread->Pop();
		pMainDlg->SetMonitoringInfo(_fileActionInfo);
	}
	return 0;
}

void CDirectoryMonitorDlg::SetMonitoringInfo(FILE_ACTION_INFO _fileActionInfo)
{
	int index = m_listFile.GetItemCount();
	CString strTime = _fileActionInfo.timeAction.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString strFileActionType = GetStrFileActionType(_fileActionInfo.actionType);
	CString strIndex;
	strIndex.Format(_T("%d"), index+1);

	m_listFile.InsertItem(index, strIndex);
	m_listFile.SetItemText(index, 1, strTime);
	m_listFile.SetItemText(index, 2, strFileActionType);
	m_listFile.SetItemText(index, 3, _fileActionInfo.strFileName);
}

CString CDirectoryMonitorDlg::GetStrFileActionType(int fileActionType)
{
	switch(fileActionType)
	{
	case(FILE_ACTION_ADDED):
		return  _T("FILE_ACTION_ADDED");
	
	case(FILE_ACTION_REMOVED):
		return _T("FILE_ACTION_REMOVED");
	
	default:
		break;
	}
	return _T("NULL");
}

void CDirectoryMonitorDlg::OnBnClickedBtnFinishMonitoring()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ProgressStop(_T("Stopped !!"));
	if (m_pDirMonitorThread->isRunning() == TRUE) {
		m_pDirMonitorThread->stop();
		GetDlgItem(IDC_BTN_START_MONITORING)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FINISH_MONITORING)->EnableWindow(FALSE);
	}
	
}

void CDirectoryMonitorDlg::OnBnClickedBtnSelectDir()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strDirPath = GetDirPath();
	m_editDirPath.SetWindowTextW(m_strDirPath);

	if (m_strDirPath.IsEmpty() == FALSE) {
		GetDlgItem(IDC_BTN_START_MONITORING)->EnableWindow(TRUE);
	}
}

void CDirectoryMonitorDlg::OnBnClickedBtnStartMonitoring()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_strDirPath.IsEmpty() == FALSE) {
		if (m_pDirMonitorThread->isRunning() == FALSE)
		{
			m_pDirMonitorThread->StartDirMonitor(m_strDirPath);
			m_pListenDirAlterdThread = AfxBeginThread(ListenDirAlterd, this);

			GetDlgItem(IDC_BTN_START_MONITORING)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_FINISH_MONITORING)->EnableWindow(TRUE);
			ProgressStart(_T("Monitoring.."));
		}
	}
	else
	{
		AfxMessageBox(_T("Directory path is wrong !!"));
	}
}

void CDirectoryMonitorDlg::ProgressStart(CString strMsg)
{
	m_ProgressWait.SetShowPercent(TRUE);
	m_ProgressWait.SetWindowText(strMsg);
	m_ProgressWait.SetMarquee(true, 30);
	m_ProgressWait.Invalidate();	
}

void CDirectoryMonitorDlg::ProgressStop(CString strMsg)
{
	m_ProgressWait.SetMarquee(false, 30);
	m_ProgressWait.SetWindowText(strMsg);
	m_ProgressWait.SetPos(0);
}
