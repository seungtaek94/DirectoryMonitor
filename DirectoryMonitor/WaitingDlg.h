#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TextProgressCtrl.h"
#include "Resource.h"
// CWaitingDlg ��ȭ �����Դϴ�.

class CWaitingDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitingDlg)

public:
	CWaitingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWaitingDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WAITING };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CTextProgressCtrl m_ProgressWait;

	void ProgressStart(CString strMsg);
	void ProgressStop();
	afx_msg void OnClose();
};
