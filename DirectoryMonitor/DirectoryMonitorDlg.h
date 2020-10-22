﻿
// DirectoryMonitorDlg.h: 헤더 파일
//

#pragma once

#include "WaitingDlg.h"

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
	CWaitingDlg* m_pWaitingDlg;

	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editDirPath;
	

	CString m_strDirPath;

	CString GetDirPath();
	
	afx_msg void OnBnClickedBtnStartMonitoring();
	afx_msg void OnBnClickedBtnFinishMonitoring();	
	afx_msg void OnBnClickedBtnSelectDir();


	void ProgressStart(CString strMsg = L"");
	void ProgressStop();
	
};
