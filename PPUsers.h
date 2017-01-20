/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_PPUSERS_H__99CECEC3_D7A8_11D1_ABBA_BCBF2882BA70__INCLUDED_)
#define AFX_PPUSERS_H__99CECEC3_D7A8_11D1_ABBA_BCBF2882BA70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RPSuggEdit.h"
#include "RPUsers.h"
#include "Ddxfile.h"
#include "RPProtocolsHandler.h"
#include "RPNetworkBrowser.h"
#include "DlgCriteria.h"

class CPPUsers : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPUsers)

// Construction
public:
	CPPUsers();
	~CPPUsers();

// Dialog Data
	//{{AFX_DATA(CPPUsers)
	enum { IDD = IDD_PP_USERS };
	CButton	m_cmdSrcInsert;
	CButton	m_cmdNetwork3;
	CButton	m_cmdNetwork2;
	CButton	m_cmdNetwork;
	CSpinButtonCtrl	spinUser;
	CButton	btnRemove;
	CButton	btnAdd;
	CListBox	m_lstUsers;
	CRPSuggEdit	m_Redirect;
	CRPSuggEdit	m_Users;
	CRPSuggEdit	m_Members;
	CString	m_Log;
	int		m_chkIgnore;
	int		m_chkIncludeFavorites;
	int		m_chkLog;
	int		m_chkPopUp;
	int		m_chkBalloon;
	int		m_chkBalloonPreview;
	int		m_chkRedirect;
	int		m_chkUsesRealPopup;
	int		m_chkOnlineWarn;
	int		m_cboUsesRealPopup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPUsers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CRPUsers *m_UsersData;
    CRPProtocolsHandler	*m_ProtocolsHandler;

	
protected:
	CGetFilenameControl m_ctrlGetFilename;
	void OnOK();
	int	 lastPos;
	bool SF_NoUserChange;
	void UsersRefresh();
	void UserRefresh(int pos);
	// Generated message map functions
	//{{AFX_MSG(CPPUsers)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptGroup();
	afx_msg void OnOptUser();
	afx_msg void OnSelchangeListusers();
	afx_msg void OnChangeTxtUser();
	afx_msg void OnBtnRemove();
	afx_msg void OnBtnAdd();
	afx_msg void OnDeltaposSpinUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSrcinsert();
	//}}AFX_MSG
	afx_msg void OnBtnNetwork(UINT nID);
	afx_msg void OnBtnChecks(UINT nID);
	DECLARE_MESSAGE_MAP()
	void OnHelp();

private:
	CRealPopupApp	*m_rpApp;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERS_H__99CECEC3_D7A8_11D1_ABBA_BCBF2882BA70__INCLUDED_)
