/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_PPLANGUAGES_H__102566EC_14B4_4369_AC96_289AC4906375__INCLUDED_)
#define AFX_PPLANGUAGES_H__102566EC_14B4_4369_AC96_289AC4906375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "getfile.h"

class CPPLanguages : public CPropertyPage
{
		DECLARE_DYNCREATE(CPPLanguages)

// Construction
public:
	CPPLanguages();   // standard constructor
	~CPPLanguages();

	int	m_ResSet;

// Dialog Data
	//{{AFX_DATA(CPPLanguages)
	enum { IDD = IDD_PP_LANG };
	CListBox	m_lstLanguage;
	CButton	m_cmdSaveWinDef;
	CButton	m_cmdSaveWin;
	CButton	m_cmdFontEdit;
	CButton	m_cmdFontRichEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPLanguages)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPPLanguages)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSavewin();
	afx_msg void OnBtnSavewindef();
	afx_msg void OnBtnFontrichedit();
	afx_msg void OnBtnFontedit();
	afx_msg void OnSelchangeLstlanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnHelp();

private:
	CRealPopupApp	*m_rpApp;
	CString			*m_ResActual;
	CStringArray	*m_ResDLLs;
	CStringArray	*m_ResDLLsAbouts;
	CStringArray	*m_ResDLLsDescs;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPLANGUAGES_H__102566EC_14B4_4369_AC96_289AC4906375__INCLUDED_)
