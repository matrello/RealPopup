/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_DLGCRITERIA_H__DE40D5AE_1869_11D3_A0B9_00A0244FD75F__INCLUDED_)
#define AFX_DLGCRITERIA_H__DE40D5AE_1869_11D3_A0B9_00A0244FD75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RPSuggEdit.h"

class CDlgCriteria : public CDialog
{
// Construction
public:
	CDlgCriteria(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCriteria)
	enum { IDD = IDD_CRITERIA };
	CRPSuggEdit	m_editCriteria;
	CString	m_txtCriteria;
	CString	m_lblMatro;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCriteria)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCriteria)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CRealPopupApp	*m_rpApp;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCRITERIA_H__DE40D5AE_1869_11D3_A0B9_00A0244FD75F__INCLUDED_)
