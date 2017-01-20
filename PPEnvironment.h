/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_PPENVIRONMENT_H__99CECEC2_D7A8_11D1_ABBA_BCBF2882BA70__INCLUDED_)
#define AFX_PPENVIRONMENT_H__99CECEC2_D7A8_11D1_ABBA_BCBF2882BA70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Ddxfile.h"

class CPPEnvironment : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPEnvironment)

// Construction
public:
	CPPEnvironment();
	~CPPEnvironment();
	BOOL CPPEnvironment::Init();

	int	m_ResSet;
	int m_Msn;

// Dialog Data
	//{{AFX_DATA(CPPEnvironment)
	enum { IDD = IDD_PP_ENV };
	CHotKeyCtrl	m_rpHotKey;
	BOOL	m_chkTop;
	BOOL	m_chkWarn;
	BOOL	m_chkScrAway;
	BOOL	m_chkAutoStart;
	BOOL	m_chkLeftTray;
	BOOL	m_chkNBDetermineFavorites;
	BOOL	m_chkWarnDel;
	BOOL	m_chkWarnDomain;
	BOOL	m_chkWarnRPProtocolNotActive;
	//}}AFX_DATA
	WORD opz_HotKey;
	WORD opz_HotKeymd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPEnvironment)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPPEnvironment)
	afx_msg void OnChkAutostart();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnHelp();

private:
	CRealPopupApp	*m_rpApp;
	ULONG_PTR m_ulActivationCookie;
	HANDLE m_hActCtx;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPENVIRONMENT_H__99CECEC2_D7A8_11D1_ABBA_BCBF2882BA70__INCLUDED_)
