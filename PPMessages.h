/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_PPMESSAGES_H__2CA4A880_112F_11D3_A0B9_00A0244FD75F__INCLUDED_)
#define AFX_PPMESSAGES_H__2CA4A880_112F_11D3_A0B9_00A0244FD75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ddxfile.h"

class CPPMessages : public CPropertyPage
{
// Construction
public:
	CPPMessages();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPPMessages)
	enum { IDD = IDD_PP_MSG };
	CButton	m_cmdPlay;
	BOOL	m_chkBeep;
	BOOL	m_chkMinimize;
	BOOL	m_chkDelOriginal;
	BOOL	m_chkBalloonOnMsgFails;
	BOOL	m_chkLastMsg;
	BOOL	m_chkDblClick;
	BOOL	m_chkWav;
	CString	m_Wav;
	BOOL	m_chkMinimizeRepl;
	BOOL	m_chkSendPcName;
	BOOL	m_chkReplyTop;
	BOOL	m_chkCrSend;
	BOOL	m_chkQuote;
	BOOL	m_chkDisableDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPMessages)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CGetFilenameControl m_ctrlGetFilename;

	// Generated message map functions
	//{{AFX_MSG(CPPMessages)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkWav();
	afx_msg void OnPlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnHelp();

private:
	CRealPopupApp	*m_rpApp;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPMESSAGES_H__2CA4A880_112F_11D3_A0B9_00A0244FD75F__INCLUDED_)
