#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TextProgressCtrl.h"
#include "Resource.h"
// CWaitingDlg 대화 상자입니다.

class CWaitingDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitingDlg)

public:
	CWaitingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWaitingDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_WAITING };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CTextProgressCtrl m_ProgressWait;

	void ProgressStart(CString strMsg);
	void ProgressStop();
	afx_msg void OnClose();
};
