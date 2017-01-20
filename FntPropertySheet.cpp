/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#include "stdafx.h"
#include "realpopup.h"
#include "FntPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFntPropertySheet

IMPLEMENT_DYNAMIC(CFntPropertySheet, CPropertySheet)

CFntPropertySheet::CFntPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CFntPropertySheet::CFntPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CFntPropertySheet::~CFntPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CFntPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CFntPropertySheet)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFntPropertySheet message handlers

BOOL CFntPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	m_rpApp=(CRealPopupApp *)AfxGetApp();

#ifdef USASETFONT
	CWnd *mywnd=GetTabControl();
	mywnd->SetFont(&m_rpApp->font_System,true);
	SetFont(&m_rpApp->font_System,true);
#endif
	
	return bResult;
}

void CFntPropertySheet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CPropertySheet::OnActivate(nState, pWndOther, bMinimized);
	
	
}
