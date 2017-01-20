/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_DLGSERVICE_H__95EA1AE7_882C_446A_B6E1_83F515C5239A__INCLUDED_)
#define AFX_DLGSERVICE_H__95EA1AE7_882C_446A_B6E1_83F515C5239A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgService dialog

class CDlgService : public CDialog
{
// Construction
public:
	CDlgService(CWnd* pParent = NULL);   // standard constructor
	int m_Msn;

// Dialog Data
	//{{AFX_DATA(CDlgService)
	enum { IDD = IDD_SERVICE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgService)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgService)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERVICE_H__95EA1AE7_882C_446A_B6E1_83F515C5239A__INCLUDED_)
