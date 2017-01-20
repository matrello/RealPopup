/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_REALPOPUP_H__08B63E85_B76B_11D1_BEDC_0060087A5B02__INCLUDED_)
#define AFX_REALPOPUP_H__08B63E85_B76B_11D1_BEDC_0060087A5B02__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "ReTracer.h"
#include "RankedArray.h"
#include "resource.h"
#include "Globals.h"

#pragma warning(disable:4786)

class CRealPopupApp : public CWinApp
{
public:
	CRealPopupApp();

	#ifdef USE_RESDLL	
	public:
	HINSTANCE m_hInstDLL;
	#endif // USE_RESDLL

	void ShowHelp(CWnd *wnd, char *ancora);
	~CRealPopupApp();
	void LogInitialize();
	void LogTerminate();
	CString TogliUserName(CString * srg);
	void GetUserProfileFolder();
	void Close();

	CString			m_ResActual;
	CStringArray	m_ResDLLs;
	CStringArray	m_ResDLLsAbouts;
	CStringArray	m_ResDLLsDescs;

	CString m_Workstation;
	CString m_UserDomain;
	CString m_LoggedUser;
	CString m_Browser;
	CString m_TitleString;

	CReTracer oReTracer;
	CRankedArray oAutoCompleteRank;
	CRankedArray oWorkstationsRank;

	ATOM hkRealPopup;

	bool	font_Editchanged;
	bool	font_RichEditchanged;
	CFont	font_Edit;
	CFont	font_RichEdit;
	CString	font_sEdit;
	CString font_sRichEdit;

	CFont	font_System;
	LOGFONT	font_lfSystem;

	CString env_AutoMsgTo;
	CString env_AutoMsgFrom;
	CString env_DefUserName;
	CString env_DefAllUsers;
	CString env_LogFileName;
	CString env_UserProfileFolder;
	CString env_Version;
	double  env_nVersion;

	BOOL	opz_Log;
	BOOL	opz_LogRemote;
	BOOL	opz_LogInitialized;
	BOOL	opz_LogRemoteInitialized;
	CString	opz_LogRemoteIP;
	int		opz_LogRemotePort;

	int		opz_FirstTime;
	BOOL    opz_chkMinimize;
	BOOL    opz_chkMinimizeRepl;
	BOOL    opz_chkTop;
	BOOL    opz_chkBeep;
	BOOL    opz_chkBalloonOnMsgDeliveryIssues;
	BOOL    opz_chkWarn;
	BOOL    opz_chkWarnDel;
	BOOL    opz_chkWarnDomain;
	BOOL    opz_chkWarnRPProtocolNotActive;
	BOOL    opz_chkWav;
	BOOL    opz_chkLastMsg;
	BOOL    opz_chkDelOriginal;
	BOOL    opz_chkAutoStart;
	BOOL    opz_chkAutoStartWarn;
	BOOL    opz_chkAutoStartMsg;
	BOOL	opz_chkDblClick;
	BOOL    opz_chkLeftTray;
	BOOL    opz_chkScrAway;
	BOOL	opz_chkNBDetermineFavorites;
	BOOL	opz_chkSendPcName;
	BOOL    opz_chkReplyTop;
	BOOL    opz_chkCrSend;
	BOOL    opz_chkQuote;
	BOOL	opz_chkDisableDel;
	int     opz_wndTop;
	int     opz_wndLeft;
	int     opz_wndHeight;
	int     opz_wndWidth;
	int     opz_sndwndTop;
	int     opz_sndwndLeft;
	int     opz_sndwndHeight;
	int     opz_sndwndWidth;
	WORD	opz_HotKey;
	WORD	opz_HotKeymd;
	CString opz_Wav;
	CString opz_LastDomain;
	int		opz_Msn;

	BOOL    opz_chkLenChk;
	BOOL	opz_chkFullInternalPings;
	int 	opz_PingTimeOut;
	BOOL	opz_chkDomainSendRP;
	BOOL	opz_chkNameResolution;
	int 	opz_NameResolutionMinInterval;
	CString	opz_RPdatFolder;
	CString	opz_RPdatDefaultsFile;

	BOOL	opz_chkWinpopupSendMailslotEnabled;
	BOOL	opz_chkWinpopupSendNetbiosEnabled;
	BOOL	opz_chkWinpopupSendSMBEnabled;

	int		opz_MsgSentNum;
	int		opz_MsgReceivedNum;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealPopupApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRealPopupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnHelp();

private:
	void GetRegUserPrefs(HKEY hKey);
	void GetRegEntries();
	void SetRegUserPrefs();
	void SetRegUserDefaults(void);
	void SetRegUserFonts(void);
	void ShowMainDialog();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALPOPUP_H__08B63E85_B76B_11D1_BEDC_0060087A5B02__INCLUDED_)
