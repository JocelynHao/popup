#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPersonalDefineDlg dialog

class CPersonalDefineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonalDefineDlg)

public:
	CPersonalDefineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPersonalDefineDlg();

// Dialog Data
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPopupSetting();
    afx_msg void OnBnClickedCheckCopy();
	afx_msg void OnBnClickedCheckCapture();
	afx_msg void OnBnClickedCheckTimesetting();
	CSliderCtrl m_slider_timesetting;
	afx_msg void OnNMCustomdrawSliderTimesetting(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit_timesetting;
	afx_msg void OnEnChangeEditTimesetting();
	CStatic m_static_timeshow;



private:
	float m_edit_timesettingshow;
public:
	afx_msg void OnEnChangeEditTimesettingshow();
	afx_msg void OnBnClickedButtonConfirmTime();
};
