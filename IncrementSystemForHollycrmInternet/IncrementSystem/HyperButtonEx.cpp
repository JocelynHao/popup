// HyperButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "HyperButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperButtonEx

CHyperButtonEx::CHyperButtonEx()
{
/***********************************************************************************

					USER DEFINED DEFAULT SETTINGS

	The default settings create a rounded rectangle button with mouseover color.

************************************************************************************/

	m_bTransparent = FALSE; //these 2 must be both false or only 1 true
	m_bFlat = TRUE;	//is button flat

	m_bRounded = TRUE; //if rounded button-only works with flat buttons
	ptRRect.x = 13;	//x and y coords for button rounding
	ptRRect.y = 13;
	m_crBorder = SHADOW;

	nBitmapRes = 0; //UINT_ID of normal image (zero disables images)
	nBitmapDis = 0; //UINT_ID of disabled image (zero uses grayout effect)

	m_nMouseover = 0;  //0-NONE, 1-IMAGE, 2-COLOR
	m_crMouseFrom = SILVER; //holds transparency color map values
	m_crMouseTo = SILVER;	//NORMAL mouseover color image effect
	m_crMouseHover=SILVER;	//HOVER mouseover color image effect
	m_idMouseImg = 0;	//holds mousover image UINT_ID

	m_nUnderline = NEVER; //defaults to underline on mouseover only

	m_crNText = DKBLUE;	//normal text = bright blue
	m_crHText = BLUE;	//hover text = bright blue
	m_crSystem = GetSysColor(COLOR_ACTIVEBORDER); //transparent color
	m_crNBackground = WHITE;  //transparent normal background
	m_crHBackground = WHITE;  //transparent hover background
	m_bBold = FALSE;	 //force all fonts to boldface
	m_nBgStyle = SOLID;  //0-solid, 1-bitmap, 2-hatch
	m_nPattern = SOLID;  //if above is not SOLID then this must reflect
						 //appropriate pattern

		//image transparency bit defaults to silver - RGB(192,192,192)
	cm.from = 0x00C0C0C0; //colormap for making bitmap appear transparent

///////////////////////////////////////////////////////////////////////////////////////
//
//	If you prefer to load a cursor image from resource instead of using an OEM
//	then import a cursor image into your project resources then use the second
//	LoadCursor() statement to load it by resource ID.  Be sure to comment out 
//	the above ::LoadCursor() statement if you choose this option
//
//	If you are targeting your app to Win98 or earlier systems you will have to use this
//	second option as the OEM hand cursor is only available in Win2000 or later.
//
//	Do NOT alter anything but the LoadCursor() statements below this line.
//
///////////////////////////////////////////////////////////////////////////////////////

	m_hHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649)); // OCR_HAND in winuser.h
//	m_hHand = AfxGetApp()->LoadCursor(IDC_CURSOR1);

/***********************************************************************************

							END USER DEFINED SETTINGS

************************************************************************************/

	bTColorSet = FALSE; //if not FALSE on first draw OnEraseBkgrnd will fail in
						//color detection mode

	bImageBg = FALSE;	// if this is TRUE then the bitmap resource ID must also be
	m_background = 0;	//initialized in the m_background variable.  It is HIGHLY
						//recommended that you leave this as is, and set the
						//background image through the control's SetBgImage() method.

	m_hReg = NULL; 
	m_bOverControl = FALSE;
	m_bLButtonDown = FALSE;
    m_nTimerID     = 1;
	pFont = NULL;
	m_bCustomFont = FALSE;  //signifies whether a custom font has been passed to 
							//the class
}

CHyperButtonEx::~CHyperButtonEx()
{
	m_hReg = NULL;
	DestroyCursor(m_hHand);
}


BEGIN_MESSAGE_MAP(CHyperButtonEx, CButton)
	//{{AFX_MSG_MAP(CHyperButtonEx)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperButtonEx message handlers

/*******************************************************************************
	SHELL ACCESS

	These functions are fairly self-explanatory.  Look up the 
	ShellExecute() method in the Platform SDK if you need a better under-
	standing of these procedures.
********************************************************************************/

BOOL CHyperButtonEx::OpenUrl(LPCTSTR szUrl)
{
	int iRes = (int) ShellExecute(NULL, _T("open"), szUrl, NULL,NULL, SW_SHOWDEFAULT);

	if(iRes <= 32)
		return FALSE;
	else
		return TRUE;
}

BOOL CHyperButtonEx::Explore(LPCTSTR szPath)
{
   int iRes = (int) ShellExecute(NULL, _T("explore"), szPath, NULL, NULL, SW_SHOW);

    if (iRes <= 32)
		return FALSE;
	else
		return TRUE;
}

BOOL CHyperButtonEx::RunApp(LPCTSTR szPath)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	if(CreateProcess(szPath, 0, 0, 0, 0, 0, 0, 0, &si, &pi))
		return TRUE;
	else
		return FALSE;
}

BOOL CHyperButtonEx::OpenFileDefaultApp(LPCTSTR szPath)
{
	CString ext;

	CString fp = _T("\"");
	fp += szPath;
	fp += _T("\"");

	int l = fp.ReverseFind('.') + 1;
	ext = fp.Mid(l, 3);
	ext.MakeLower();

	char buf[MAX_PATH];
	GetSystemDirectory((LPTSTR)buf, MAX_PATH);

	CString app = _T("\"");
	app += buf;
	app += _T("\\");
	app += _T("notepad.exe\"");

	int iRes;
	if(ext == _T("txt"))
		iRes = (int) ShellExecute(NULL, _T("open"), app, fp, NULL, SW_SHOW);
	else
		iRes = (int) ShellExecute(NULL, _T("open"), szPath, NULL, NULL, 0);

    if (iRes <= 32)
		return FALSE;
	else
		return TRUE;
}

BOOL CHyperButtonEx::PrintLocalFile(LPCTSTR szPath)
{
    int iRes = (int) ShellExecute(NULL, _T("print"), szPath, NULL, NULL, 0);

    if (iRes <= 32)
		return FALSE;
	else
		return TRUE;
}


/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

void CHyperButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bLButtonDown)
	{
		if(this->IsWindowEnabled())	// if button is enabled
		{

			ShowCursor(FALSE);
			m_hReg = ::SetCursor(m_hHand);	//change to hand cursor
			ShowCursor(TRUE);
			
			m_ToolTip.Activate(TRUE);		//show tooltip
		}
		else
		{
			m_ToolTip.Activate(FALSE);		//hide tooltip if window not enabled
		}

	   if (!m_bOverControl)      //flag=FALSE means cursor has just moved over control
		{

			TRACE0("Entering control\n");

			m_bOverControl = TRUE;              // Set flag telling us the mouse is in
			TRACE0("Invalidate\n");									// and preventing continual redraws (flicker)
			Invalidate();						// Force a redraw

			TRACE0("Setting Timer\n");

			SetTimer(m_nTimerID, 100, NULL);    // Keep checking back every 1/10 sec
		}
	}
			
		CButton::OnMouseMove(nFlags, point);
}

void CHyperButtonEx::OnTimer(UINT nIDEvent) 
{
	if(!m_bLButtonDown)
	{
		// Where is the mouse?
		CPoint p(GetMessagePos());
		ScreenToClient(&p);

		// Get the bounds of the control (just the client area)
		CRect rect;
		GetClientRect(rect);

		// Check the mouse is inside the control
		if (!rect.PtInRect(p))
		{
			TRACE0("Leaving control\n");

			// if not then stop looking...
			m_bOverControl = FALSE;

			KillTimer(m_nTimerID);

			// ...and redraw the control
				Invalidate();
		}
	}
	
	CButton::OnTimer(nIDEvent);
}

/*************************************************************************
NOTE::	FLAT state and TRANSPARENT state cannot BOTH be true... However,
		they can both be FALSE, in which case a standard 3D button is drawn.
**************************************************************************/

void CHyperButtonEx::IsFlat(BOOL bFlat, int nRound, COLORREF crBorder)
{
	m_bFlat = bFlat;
	m_crBorder = crBorder;

	if(bFlat)
		m_bTransparent = FALSE;

	RoundedRect(nRound, nRound);
}

/*
void CHyperButtonEx::IsFlat(BOOL bFlat, int nRoundX, int nRoundY, COLORREF crBorder)
{
	m_bFlat = bFlat;
	m_crBorder = crBorder;

	if(bFlat)
		m_bTransparent = FALSE;

	RoundedRect(nRoundX, nRoundY);
}
*/

void CHyperButtonEx::IsTransparent(BOOL bTransparent)
{
	m_bTransparent = bTransparent; 

	if(bTransparent)
		m_bFlat = FALSE;
}

void CHyperButtonEx::PaintBackground(CDC *pDC, CRect rect, COLORREF color, BOOL bState)
{
	TRACE0("BEGIN PaintBackground\n");


	CBrush pBrush;
	if(bState)
	{
		switch(m_nBgStyle)
		{
		case SOLID:
			{
				if(m_bTransparent)
					pBrush.CreateSolidBrush(m_crSystem);
				else
					pBrush.CreateSolidBrush(color);

				break;
			}
		case HATCH:		//paint hatch background
			{
				pBrush.CreateHatchBrush(m_nPattern, color);
				break;
			}
		case PATTERN:			//paint pattern background
			{
				CBitmap bm;
				bm.LoadBitmap(m_nPattern);
				pBrush.CreatePatternBrush(&bm);
			}
		}
	}
	else
	{
		pBrush.CreateSolidBrush(m_crSystem);
	}


	if(bIsPressed)
			pBrush.CreateSolidBrush(m_crSystem);

	if(m_bRounded && m_bFlat)
	{
		/*if(m_b3D)
		{
			int r,g,b,x,y,c;
			r=g=b=192;
			m_crBorder=RGB(r-50,g-50,b-50);
			x=rect.Width();
			y=rect.Height();
			if(x > y)
				c = y/2;
			else
				c = x/2;

		}
		else
		{*/
		CPen pen(PS_SOLID, 1, m_crBorder);
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBrush = pDC->SelectObject(&pBrush);
		pDC->RoundRect(rect, ptRRect);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pen.DeleteObject();
		//}
	}
	else
	{
			pDC->FillRect(rect, &pBrush);
	}

	pBrush.DeleteObject();
	TRACE0("END PaintBackground\n");
}

void CHyperButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	TRACE0("Enter DrawItem\n");
    CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;

	    UINT state = lpDrawItemStruct->itemState;

	CRect orect = rect;	//offset rect - used on button press

	//BOOL bIsPressed; is declared globally in HyperButtonEx.h
	if(state & ODS_SELECTED)
		bIsPressed = FALSE;
	else
		bIsPressed = FALSE;

	if (state & ODS_DISABLED)
		bIsEnabled = FALSE;
	else
		bIsEnabled = TRUE;

	BOOL bTransBg = FALSE;
	if(m_bTransparent && !bIsEnabled && (m_crNBackground != m_crSystem || m_nBgStyle != SOLID))
		bTransBg = TRUE;

	COLORREF crText;	//text color	
	TextOffset = 0; //default to no bitmap

		//copy default font if not user defined
		if(!m_bCustomFont)
		{
			pFont = GetFont(); 

			ASSERT(pFont);
			pFont->GetLogFont(&lf);
		}

	 CString strText;

	 if(m_szButtonText.IsEmpty())
		 GetWindowText(strText);	//if label not assigned get text from resource editor
	 else
		 strText = m_szButtonText;	//else use assigned text label

	 if(!m_bTransparent)	//if not transparent then draw button edges
	 {
		if (bIsPressed && !m_bRounded)
		{
			pDC->DrawFrameControl(orect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
		}
		else
		{
			if(m_bFlat)	//draw flat button UP
			{
				if(!m_bRounded)
				{
					if(bIsEnabled)
						pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_FLAT);
					else
						pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_MONO | DFCS_INACTIVE);
				}
			}
			else	//draw 3D button UP
			{
				if(bIsEnabled)
					pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH);
				else
					pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_MONO |DFCS_INACTIVE);
			}
		}
	 }
	 else
	 {
		 if(bTransBg)
			 pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_MONO | DFCS_INACTIVE);
	 }
	 

    // Deflate the drawing rect by the size of the button's edges
	 if(!m_bTransparent || bTransBg)
	    rect.DeflateRect( CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
    
	 TRACE0("Calling PaintBackground\n");
    // Fill the interior color if necessary
    if (m_bOverControl) //on mouseover
	{
		//if(!(bImageBg && m_bTransparent))
		if(bImageBg && m_bTransparent)	//erase background
			OnEraseBkgnd(pDC);
		else
			PaintBackground(pDC, rect, m_crHBackground, bIsEnabled);

		if(m_nUnderline == NEVER) //if underline set to NEVER
			lf.lfUnderline = FALSE;	//turn underline off for mouseover
		else
			lf.lfUnderline = TRUE; //else enable underline of mouseover
		
		if(m_nMouseover == COLOR)
		{
			cm.from = m_crMouseFrom;
			cm.to = m_crMouseHover;
		}
		else
		{
			if((m_bTransparent && !bTransImage) || !bIsEnabled)
				cm.to = m_crSystem;
			else
				if(!bTransImage)
					cm.to = m_crHBackground; //set transparency bit == hover background color
		}

		crText = m_crHText; //set text color to hover text color
	}
	else	//if mouse not over control
	{
		//if(!(bImageBg && m_bTransparent) || !bIsEnabled)
		if(bImageBg && m_bTransparent)
			OnEraseBkgnd(pDC);
		else
			PaintBackground(pDC, rect, m_crNBackground, bIsEnabled);

		if(m_nMouseover == COLOR)
		{
			cm.from = m_crMouseFrom;
			cm.to = m_crMouseTo;
		}
		else
		{
			if((m_bTransparent && !bTransImage) || !bIsEnabled)
				cm.to = m_crSystem;
			else
				if(!bTransImage)
					cm.to = m_crNBackground; //transparency bit = normal background color
		}

		if(m_nUnderline == ALWAYS) //if underline set to ALWAYS
			lf.lfUnderline = TRUE; //enable underline when mouse not over
		else
			lf.lfUnderline = FALSE; //disable underline when mouse not over

		crText = m_crNText;	//text color = normal text color
	}

    // Draw the text
    if (!strText.IsEmpty())
    {
        int nMode = pDC->SetBkMode(TRANSPARENT); //make text transparent
		
		if(m_bBold)
			lf.lfWeight = FW_BOLD; //set bold font
			
		//the underline flag was set or disabled previously, so all we do now
		//is to create the font
			TRACE0("Creating Font\n");
			fUnderline.CreateFontIndirect(&lf); 

			TRACE0("Selecting Font\n");
			//select new font - return ptr to old font
			CFont* pOldFont = NULL;
			pOldFont = pDC->SelectObject(&fUnderline); 
    
			CSize Extent = pDC->GetTextExtent(strText);

				//Draw the Bitmap
			if(nBitmapRes > 0)	//if bitmap mode is enabled
				DrawBitmap(lpDrawItemStruct, Extent.cx, bIsEnabled);

			rect.left += TextOffset; //offset the drawing region 0-width of bitmap

        CPoint pt( rect.CenterPoint().x - (Extent.cx/2), 
        rect.CenterPoint().y - (Extent.cy/2) );
			
		TRACE0("Drawing Text\n");
        if (!bIsEnabled || bIsPressed) //disabled button state
		{
			pDC->SetTextColor(m_crSystem); //set the text color

			//draw the text centered
			pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); 

			if(m_bTransparent || bIsPressed)
				pt.Offset(-1,-1);

            pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
        else
		{
			pDC->SetTextColor(crText); //set the text color

			//draw the text centered
			pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //center text
		}
		
		if (pOldFont) 
			pDC->SelectObject(pOldFont); //restore original font

        pDC->SetBkMode(nMode); //set background mode for text - transparent here
    }
	else
	{
		if(nBitmapRes > 0)	//if bitmap mode is enabled
			DrawBitmap(lpDrawItemStruct, 0, bIsEnabled);
	}

	fUnderline.DeleteObject();
}

void CHyperButtonEx::MakeDisabled(CBitmap &ioBM)
{
	// For MFC - Adapted by Koen Zagers from Zundert

	CDC dc;
	CDC dcBW;

	// Create memory DCs
	dc.CreateCompatibleDC(NULL);
	dcBW.CreateCompatibleDC(NULL);

	// Get bitmap dimensions
	BITMAP bm;
	ioBM.GetBitmap(&bm);
	int nWidth = bm.bmWidth;
	int nHeight = bm.bmHeight;

	// Create a monochrome DIB section with a black and white palette
	struct
	{
		BITMAPINFOHEADER	bmiHeader;
		RGBQUAD				bmiColors[2];
	} RGBBWBITMAPINFO =
	{

		{	// a BITMAPINFOHEADER
			sizeof(BITMAPINFOHEADER),	// biSize 
			nWidth, 					// biWidth; 
			nHeight,					// biHeight; 
			1,							// biPlanes; 
			1,							// biBitCount 
			BI_RGB, 					// biCompression; 
			0,							// biSizeImage; 
			0,							// biXPelsPerMeter; 
			0,							// biYPelsPerMeter; 
			0,							// biClrUsed; 
			0							// biClrImportant; 
		},

		{
			{ 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }
		}
	};

	VOID *pbitsBW;
	HBITMAP hbmBW = CreateDIBSection(dcBW.m_hDC, (LPBITMAPINFO) &RGBBWBITMAPINFO, DIB_RGB_COLORS, &pbitsBW, NULL, 0);

	ASSERT(hbmBW);

	if (hbmBW)
	{
		// Attach the monochrome DIB section and the bitmap to the DCs
		SelectObject(dcBW.m_hDC, hbmBW);
		SelectObject(dc.m_hDC, ioBM.m_hObject);
		CBrush brush;

		// BitBlt the bitmap into the monochrome DIB section
		dcBW.BitBlt(0, 0, nWidth, nHeight, &dc, 0, 0, SRCCOPY);

		// Paint the destination bitmap in gray
		brush.CreateSysColorBrush(COLOR_3DFACE);
		dc.FillRect(CRect(0, 0, nWidth, nHeight), &brush);

		// BitBlt the black bits in the monochrome bitmap into COLOR_3DHILIGHT bits in the destination DC
		// The magic ROP comes from the Charles Petzold's book
		brush.DeleteObject();
		brush.CreateSolidBrush(GetSysColor(COLOR_3DHILIGHT));
		dc.SelectObject(&brush);
		dc.BitBlt(1, 1, nWidth, nHeight, &dcBW, 0, 0, 0xB8074A);

		// BitBlt the black bits in the monochrome bitmap into COLOR_3DSHADOW bits in the destination DC
		brush.DeleteObject();
		brush.CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
		dc.SelectObject(&brush);
		dc.BitBlt(0, 0, nWidth, nHeight, &dcBW, 0, 0, 0xB8074A);

		brush.DeleteObject();
		DeleteObject(hbmBW);
	}
}

void CHyperButtonEx::SetBitmap(UINT nNormal, UINT nMouseover, BOOL bIsTrans, COLORREF crTrans, UINT nDisabled)
{
	nBitmapRes = nNormal;
	nBitmapDis = nDisabled;
	bTransImage = bIsTrans;

	if(bIsTrans)
		cm.from = cm.to = crTrans;

	MouseOverImage(nMouseover);
}

void CHyperButtonEx::SetBitmapEx(UINT nNormal, COLORREF crFrom, COLORREF crNormal, COLORREF crHover, UINT nDisabled)
{
	nBitmapRes = nNormal;
	nBitmapDis = nDisabled;
	bTransImage = FALSE;
	bImageBg = FALSE;
	m_nMouseover = COLOR;
	m_crMouseFrom = crFrom;
	m_crMouseTo = crNormal;
	m_crMouseHover = crHover;
}

void CHyperButtonEx::SetToolTip(LPCTSTR szToolTip, COLORREF crBkColor, COLORREF crTextColor)
{
	CString str = szToolTip;
	if(!str.IsEmpty())
	{
		m_ToolTip.UpdateTipText(szToolTip,this);
		m_ToolTip.SetTipBkColor(crBkColor);
		m_ToolTip.SetTipTextColor(crTextColor);
		m_ToolTip.Activate(TRUE);
	}
	else
	{
		m_ToolTip.Activate(FALSE);
	}
	
}

void CHyperButtonEx::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();

	ModifyStyle(0, BS_OWNERDRAW);	// make the button owner drawn

	//create the tooltip
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(this);
	m_ToolTip.UpdateTipText(_T("Click Here!"), this); //default tooltip if one not set
}

BOOL CHyperButtonEx::PreTranslateMessage(MSG* pMsg) 
{
		if (m_ToolTip.m_hWnd)
		{
			m_ToolTip.RelayEvent(pMsg); //tell tooltip control what mouse state is
		}	
	return CButton::PreTranslateMessage(pMsg);
}

void CHyperButtonEx::SetBoldFont(BOOL bBold)
{
	m_bBold = bBold;
}

void CHyperButtonEx::SetBGStyle(int nStyle, UINT nPattern)
{
	m_nBgStyle = nStyle;  //0-solid, 1-pattern, 2-hatch
	m_nPattern = nPattern; //ex: IDB_BITMAP1 or HS_DIAGCROSS

	if(m_nBgStyle >=3)
		m_nBgStyle = m_nPattern = SOLID;
}

void CHyperButtonEx::SetCustomFont(LOGFONT &nlf)
{
		lf = nlf;
		m_bCustomFont = TRUE;
}

void CHyperButtonEx::SetCustomFont(LPCTSTR szName, int nSize, int nWeight, BOOL bItalic)
{
	CString cmp = szName;
	if(cmp.CompareNoCase(_T("Default")) == 0)
	{
		m_bCustomFont = FALSE;
	}
	else
	{	
		#pragma warning( disable : 4244 )
		int h = nSize;
		if(h <= 5) h=6;
		h *= -1.38;
		if(nSize==16)
			h++;

		if(nWeight <= 99)
			nWeight = 100;

		if(nWeight >= 1000)
			nWeight = 900;

		pFont = GetFont();
		pFont->GetLogFont(&lf);
		_tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName), szName);
		lf.lfHeight = h;
		lf.lfWeight = nWeight;
		lf.lfWidth=0;
		lf.lfItalic = bItalic;
		lf.lfQuality = PROOF_QUALITY;
		lf.lfOutPrecision = OUT_STROKE_PRECIS;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfCharSet = 0;
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

		m_bCustomFont = TRUE;
	}
}

void CHyperButtonEx::MouseOverImage(UINT nImage)
{
	if(nImage == 0)
		m_nMouseover = NONE;
	else
		m_nMouseover = IMAGE;

	m_idMouseImg = nImage;
}


void CHyperButtonEx::RoundedRect(int x, int y)
{
	if(x == 0 && y == 0)
		m_bRounded = FALSE;
	else
		m_bRounded = TRUE;

	ptRRect.x = x;
	ptRRect.y = y;
}

BOOL CHyperButtonEx::OnEraseBkgnd(CDC* pDC) 
{
	//this function is called before controls are drawn for the first time
	//assuring us that getting pixel color or image snapshot will be dialog
	//background only (not corrupted by control text)

	if(bImageBg && m_background != 0) //if gradient color or image background
	{
		if(m_Bmp.GetSafeHandle() == NULL)
		{
		  CBitmap bm;
		  m_Bmp.LoadBitmap(m_background);
		}

		  CRect DRect, Rect;
		  CWnd *pParent = GetParent();
		  pParent->GetWindowRect(&DRect);
		  this->GetClientRect(&Rect);
		  this->ClientToScreen(&Rect);

		  int x = Rect.left - DRect.left;
		  int y = Rect.top - DRect.top;

		  this->ScreenToClient(&Rect);

		  CDC MemDC;
		  MemDC.CreateCompatibleDC(pDC);
		  CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		  pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,x,y,SRCCOPY);
		  MemDC.SelectObject(pOldBmp);
		  MemDC.DeleteDC();	   
	}
	else	//if solid color
	{
		CRect rect;
		this->GetClientRect(&rect);

		if(!bTColorSet) 
		{
			CPoint pt = rect.CenterPoint(); //get center point of static control
			m_crSystem =  pDC->GetPixel((POINT)pt); //place pixel color in member var
			bTColorSet = TRUE;	//set flag to true so this code doesn't execute again
		}

		pDC->FillSolidRect(&rect, m_crSystem); //paint rect with dialog bg color
	}

	return TRUE;  //no need to call base class
	
//	return CButton::OnEraseBkgnd(pDC);
}

//this code adapted from an article on Codeguru by ScoobyDownUnder
//http://www.codeguru.com/cpp/g-m/bitmap/article.php/c1753/
void CHyperButtonEx::DrawTBitmap(CDC* pDC, CBitmap* pBmp, int x, int y, int w, int h, COLORREF crColor)
{
	COLORREF crOldBack = pDC->SetBkColor(WHITE);
	COLORREF crOldText = pDC->SetTextColor(BLACK);
	CDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage.CreateCompatibleDC(pDC);
	dcTrans.CreateCompatibleDC(pDC);

	// Select the image into the appropriate dc
	CBitmap* pOldBitmapImage = dcImage.SelectObject(pBmp);

	// Create the mask bitmap
	CBitmap bitmapTrans;
	bitmapTrans.CreateBitmap(w, h, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// Build mask based on transparent colour
	dcImage.SetBkColor(crColor);
	dcTrans.BitBlt(0, 0, w, h, &dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	pDC->BitBlt(x, y, w, h, &dcImage, 0, 0, SRCINVERT);
	pDC->BitBlt(x, y, w, h, &dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(x, y, w, h, &dcImage, 0, 0, SRCINVERT);

	// Restore settings
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	pDC->SetBkColor(crOldBack);
	pDC->SetTextColor(crOldText);
}

void CHyperButtonEx::DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct, int nTextLen, BOOL bEnabled)
{
	int tlen = nTextLen;

		CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
	    CRect rect = lpDrawItemStruct->rcItem;

		CBitmap bi;
		if(!bEnabled)
			if(nBitmapDis != 0)
				bi.LoadMappedBitmap(nBitmapDis, 0, &cm, 1);
			else
				bi.LoadMappedBitmap(nBitmapRes);
		else
			if(m_nMouseover == IMAGE && m_bOverControl)
				bi.LoadMappedBitmap(m_idMouseImg, 0, &cm, 1);
			else
				bi.LoadMappedBitmap(nBitmapRes, 0, &cm, 1);

		BITMAP bms;
		bi.GetBitmap(&bms); //loads the bitmap info into the bms var
		
		CSize cs;
		cs.cx = bms.bmWidth; //load bitmap size into a CSize var
		cs.cy = bms.bmHeight;

		TextOffset = cs.cx;
		if(nTextLen >= 1)
			TextOffset += 2;	// text offset = width of bitmap + 2 spaces

		nTextLen += TextOffset;
		int sp = (nTextLen/2);

		if(sp <= -1)
			sp=0;

		CPoint cp;	//center bitmap w/text horizontlly & vertically
		cp.x = rect.CenterPoint().x - sp;
			if(cp.x <= -1) cp.x = 0;
		cp.y = (rect.Height() - bms.bmHeight) / 2;

		if(bIsPressed)
		{
			if(tlen >= 64 || m_bCustomFont)
				cp.x -=1;

			if(tlen >= 74 || m_bCustomFont)
				cp.y -=1;
		}

		COLORREF tcolor = cm.to;

		if((!bEnabled || bIsPressed) && nBitmapDis == 0)
		{
			MakeDisabled(bi);
			tcolor = GetSysColor(COLOR_3DFACE);
		}

		if(bTransImage || !bEnabled || bIsPressed)
			DrawTBitmap(pDC, &bi, cp.x, cp.y, cs.cx, cs.cy, tcolor);
		else
			pDC->DrawState(cp, cs, bi, DST_BITMAP | DSS_NORMAL, NULL);
}



void CHyperButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = TRUE;

	CButton::OnLButtonDown(nFlags, point);
}

void CHyperButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = FALSE;

	CButton::OnLButtonUp(nFlags, point);
}

void CHyperButtonEx::SetBgImage(BOOL bIsImage, UINT BitmapID)
{
	bImageBg = bIsImage; 
	
	if(m_nMouseover == COLOR || BitmapID == 0) 
		bImageBg = FALSE;

	if(bImageBg)
		m_background = BitmapID;
}
