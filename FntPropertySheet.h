/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_FNTPROPERTYSHEET_H__8205BDDA_2370_11D3_A0B9_00A0244FD75F__INCLUDED_)
#define AFX_FNTPROPERTYSHEET_H__8205BDDA_2370_11D3_A0B9_00A0244FD75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RealPopup.h"

/////////////////////////////////////////////////////////////////////////////
// CFntPropertySheet

class CFntPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CFntPropertySheet)

// Construction
public:
	CFntPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFntPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFntPropertySheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFntPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFntPropertySheet)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRealPopupApp	*m_rpApp;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FNTPROPERTYSHEET_H__8205BDDA_2370_11D3_A0B9_00A0244FD75F__INCLUDED_)
