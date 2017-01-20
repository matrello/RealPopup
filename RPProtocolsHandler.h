/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_RPMSGHANDLER_H__D9A6DE42_C0F4_11D1_ABBA_A7C311D6826B__INCLUDED_)
#define AFX_RPMSGHANDLER_H__D9A6DE42_C0F4_11D1_ABBA_A7C311D6826B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "dlgbars.h"
#include "MMStatusBar.h"
#include "RPWinPopupProtocol.h"
#include "RPRealPopupProtocol.h"
#include "RPUsers.h"
#include "DlgService.h"

#define PONG_TIMEOUT_MULT			4		// timeout factor for multipart msg (parts*PONG_TIMEOUT_MULT)
#define PROTOCOL_MSGPOPUP_ENABLED	true
#define PROTOCOL_MSGPOPUP_MAXLEN	3900	// ex limite prima di SMB (2.3) 890

class CRPNetworkBrowser;

class CRPProtocolsHandler
{
public:
	CRPProtocolsHandler();
	bool Init(bool bShowErrorMessage, bool warm);
	void Close(bool warm);
	void PendingPurge();
	void Ping(CMessaggio *Msg, CString *UserDest=NULL, bool pingCallback=false, CString *CurrentMsgsThreadID=NULL);
	void SetNotificationEnabled(bool newValue);
	bool GetNotificationEnabled();
	void SetAwayEnabled(bool newValue);
	bool GetAwayEnabled();
	void LogMsgWrite(CMessaggio *msg, bool primaMitt);
	bool Send(CMessaggio *Msg, CString *CurrentMsgsThreadID=NULL);
	int Scan();
	void ShowInternalMessage(CString *msg, CString *head=NULL, DWORD icon=NIIF_INFO, CString *balloonMsg=NULL);

	CStringArray IgnoredThreads;
	CMessaggi Messaggi;

	CRPNetworkBrowser *m_NetworkBrowser;
	CRPUsers *m_Users;
	MMStatusBar *m_wndStatusBar;
	CSystemTray *m_TrayIcon;

	CRPWinPopupProtocol	 WinPopupProtocol;
	CRPRealPopupProtocol RealPopupProtocol;

	CNetworkDomains NetworkDomains;

	bool IsWinPopupProtocolEnabled;
	bool IsRealPopupProtocolEnabled;

	bool *m_RedirectEnabled;
	bool SF_ScanInProgress;

	bool m_NameResolutionRequested;

protected:
	void LogMsgIn(int startPos);
	void Redirect(int startPos);
	void Pending();
	void MessaggiFiltra();
	bool HandleMessengerService(bool Start);

	bool isNotificationEnabled;
	bool isAwayEnabled;
	bool m_ServiceStoppedByRp;

	int  m_NameResolutionIntervalCounter;

	CPenMessaggi penMessaggi;

private:
	CRealPopupApp	*m_rpApp;
};

#endif // !defined(AFX_RPMSGHANDLER_H__D9A6DE42_C0F4_11D1_ABBA_A7C311D6826B__INCLUDED_)
