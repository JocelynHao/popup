#pragma once
#include "afxcmn.h"


// CViewVerDlg dialog

class CViewVerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewVerDlg)

public:
	CViewVerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CViewVerDlg();

// Dialog Data
	enum { IDD = IDD_VIEW_VER_DIALOG };
	CListCtrl m_lvVer;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

protected:
	void LoadModuleVer(void);
	CString ReadLocalFileVer(LPCTSTR lpszFileName);
public:
	afx_msg void OnLvnItemchangedListVer(NMHDR *pNMHDR, LRESULT *pResult);
};
