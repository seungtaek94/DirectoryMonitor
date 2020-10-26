
// DirectoryMonitorDlg.h: 헤더 파일
//

#pragma once

#include <algorithm>
#include "DirectoryMonitorThread.h"
#include "TextProgressCtrl.h"

// CDirectoryMonitorDlg 대화 상자
class CDirectoryMonitorDlg : public CDialogEx
{
// 생성입니다.
public:
	CDirectoryMonitorDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTORYMONITOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	CDirectoryMonitorThread* m_pDirMonitorThread;

	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CTextProgressCtrl m_ProgressWait;
	
	CEdit m_editDirPath;
	CListCtrl m_listFile;
	CString m_strDirPath;
	int m_listIndex;

	afx_msg void OnBnClickedBtnStartMonitoring();
	afx_msg void OnBnClickedBtnFinishMonitoring();	
	afx_msg void OnBnClickedBtnSelectDir();
	
	CString GetDirPath();
	void ProgressStart(CString strMsg = L"");
	void ProgressStop(CString strMsg = L"");
	void InitListCrl();
	CString GetStrFileActionType(int fileActionType);
	void SetMonitoringInfo(FILE_ACTION_INFO _fileActionInfo);

private:
	CWinThread* m_pListenDirAlterdThread;
	static UINT ListenDirAlterd(LPVOID _pMain);
};
