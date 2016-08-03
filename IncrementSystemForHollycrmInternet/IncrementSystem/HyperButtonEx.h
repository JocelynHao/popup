#if !defined(AFX_HYPERBUTTONEX_H__49F0FF44_F3FD_4791_8E13_66669EEEC7E9__INCLUDED_)
#define AFX_HYPERBUTTONEX_H__49F0FF44_F3FD_4791_8E13_66669EEEC7E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperButtonEx.h : header file
//

#include "Defines.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperButtonEx window

class CHyperButtonEx : public CButton
{
// Construction
public:
	CHyperButtonEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

	UINT m_nTimerID;
	virtual ~CHyperButtonEx();

	// Generated message map functions
protected:
	BOOL m_bLButtonDown;
	POINT ptRRect;
	UINT m_background;
	UINT m_idMouseImg;
	COLORREF m_crMouseTo;
	COLORREF m_crMouseFrom;
	COLORREF m_crMouseHover;
	int m_nMouseover;
	COLORREF m_crSystem;
	int TextOffset;
	//{{AFX_MSG(CHyperButtonEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void PaintBackground(CDC* pDC, CRect rect, COLORREF color, BOOL bState);
	void DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct, int nTextLen, BOOL bEnabled = TRUE);
	void MakeDisabled(CBitmap &ioBM);
	void MouseOverImage(UINT nImage = 0);
	void RoundedRect(int x, int y);
	void DrawTBitmap(CDC* pDC, CBitmap* pBmp, int x, int y, int w, int h, COLORREF crColor);

	CFont* pFont;
	CFont fUnderline;
	LOGFONT lf;
	COLORREF m_crNText;
	COLORREF m_crHText;
	COLORREF m_crNBackground;
	COLORREF m_crHBackground;
	COLORREF m_crBorder;
	CBitmap m_Bmp;
	
	BOOL bImageBg;
	BOOL m_bBold;
	BOOL m_bCustomFont;
	BOOL m_bOverControl;
	BOOL m_bTransparent;
	BOOL m_bFlat;
	BOOL m_bRounded;
	BOOL bIsPressed;
	BOOL bIsEnabled;
	BOOL bTColorSet;
	BOOL bTransImage;

	int m_nUnderline;
	int m_nBgStyle;  //0-solid, 1-bitmap, 2-hatch
	UINT m_nPattern;
	UINT nBitmapRes;
	UINT nBitmapDis;

	CString m_szButtonText;
	CToolTipCtrl m_ToolTip;

	COLORMAP cm;

	HCURSOR m_hReg;
	HCURSOR m_hHand;

public:
	void SetCustomFont(LOGFONT &nlf);
	void SetCustomFont(LPCTSTR szName = _T("Default"), int nSize = 10, int nWeight = 500, BOOL bItalic = FALSE);
	void SetBoldFont(BOOL bBold = TRUE);
	void IsTransparent(BOOL bTransparent = TRUE);
	void IsFlat(BOOL bFlat, int nRound = NONE, COLORREF crBorder = SHADOW);
//	void IsFlat(BOOL bFlat, int nRoundX = 0, int nRoundY = 0, COLORREF crBorder = SHADOW);
	void SetToolTip(LPCTSTR szToolTip, COLORREF crBkColor = LTYELLOW, COLORREF crTextColor = BLACK);
	void SetBitmap(UINT nNormal = 0, UINT nMouseover = 0, BOOL bIsTrans = TRUE, COLORREF crTrans = RGB(192,192,192), UINT nDisabled = 0);
	void SetBitmapEx(UINT nNormal, COLORREF crFrom, COLORREF crNormal, COLORREF crHover, UINT nDisabled = 0);
	void SetBGStyle(int nStyle = 0, UINT nPattern = 0);
	void SetBgImage(BOOL bIsImage, UINT BitmapID = 0);

	void SetButtonText(LPCTSTR szText) { m_szButtonText = szText; }
	void SetUnderline(int nUnderline = HOVER) { m_nUnderline = nUnderline; }
	void SetNormalText(COLORREF crNewColor) { m_crNText = crNewColor; }
	void SetHoverText(COLORREF crNewColor) { m_crHText = crNewColor; }
	void SetNormalBackground(COLORREF crNewColor) { m_crNBackground = crNewColor; }
	void SetHoverBackground(COLORREF crNewColor) { m_crHBackground = crNewColor; }

	BOOL OpenUrl(LPCTSTR szUrl);
	BOOL Explore(LPCTSTR szPath);
	BOOL RunApp(LPCTSTR szPath);
	BOOL OpenFileDefaultApp(LPCTSTR szPath);
	BOOL PrintLocalFile(LPCTSTR szPath);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERBUTTONEX_H__49F0FF44_F3FD_4791_8E13_66669EEEC7E9__INCLUDED_)
