#pragma once

#include "explorer.h"
#include "afxext.h"
#include "HyperButtonEx.h"
#include "..\..\..\..\Public\Class\StaticEx.h"

// CLogonDlg dialog

class CLogonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonDlg();

// Dialog Data
	enum { IDD = IDD_LOGON_DIALOG };
	CExplorer m_ctlExplorer;
	int m_nNetwork;
	CString m_strUser;
	CString m_strPassword;
	CString m_strCode;
	CHyperButtonEx	m_btnOK;
	CHyperButtonEx	m_btnCancel;
	CHyperButtonEx	m_btnReset;
	CHyperButtonEx	m_btnResend;
	CHyperButtonEx	m_btnTelnet;
	CStaticEx		m_seStatus;

public:
	BOOL	m_bBeginLogon;
	BOOL	m_bCheckTelnet;
	CPoint	m_pntMouse;


// Implementation
protected:
	void	ShowBitmap(CDC* pDC, CBitmap &bmp, CRect &rect);
	CString	GetResponse(CString strBody);
	BOOL	ReadParam(int nNetwork);
	void	ConverSpecialChar(CString &str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupView();
	afx_msg void OnPopupClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedResetPwd();
	afx_msg void OnBnClickedResendCode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedTelnet();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL);

//  Ù–‘
protected:
	CBitmap m_bmpLogon;		
	CString m_strURL;
	CString m_strResetPwdURL;
	CString m_strLastUser;
	CString m_strLastPassword;
	BOOL	m_bHasTimer;
	BOOL	m_bCanValidate;
	BOOL	m_bRetry;
	BOOL	m_bAuthentication;
	int		m_nRemainSecond;
	
//public:
//	afx_msg void OnBnClickedCheckCopy();
//	BOOL m_bCopy;
};

UINT TelnetThread(LPVOID lpParam);