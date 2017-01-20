/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_MSGREALPOPUP_H__885CE8C0_C308_11D1_8340_00609711E948__INCLUDED_)
#define AFX_MSGREALPOPUP_H__885CE8C0_C308_11D1_8340_00609711E948__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "dlgbars.h"
#include "MMStatusBar.h"
#include "SystemTray.h"

#pragma warning(disable:4786)

#include "RealPopup.h"
#include "RPUsers.h"
#include "SharedMailslot.h"

#include "dictionary.h"
#include <vector>
#include <iostream>
#include <string>

#pragma warning(disable:4786)

#define MSGREALPOPUP_MULTIPART_MSGSIZE 400

typedef CArray<CMessaggio, CMessaggio> CMessaggiArray;

using std::vector;
using std::string;

struct SendExecuteInfo
{
	bool *isSendExecuteInProgress;
	CMessaggio *Msg;

	CStringArray *SendExecuteDestinations;
};

class CRPRealPopupProtocol
{
public:
	CRPRealPopupProtocol();
	virtual ~CRPRealPopupProtocol();

	bool Init();
	void Close();
	int Scan(CString *stMsg, CString *blMsg);
	bool Send(CMessaggio *Msg, CString *CurrentMsgsThreadID=NULL, bool isInternalMessage=false);
	void Ping(CMessaggio *Msg);
	bool Pong(CPenMessaggio *Msg);
	void NameResolutionPing();
	void NameResolutionDo(CStringArray *computers, CString *dest);
	bool NameResolutionIsNameOnly(CString *dest);

	CStringArray *IgnoredThreads;
	CNetworkDomains *NetworkDomains;

	CMessaggi *mMessaggi;
	CRPUsers *m_Users;

	bool IsProtocolEnabled;
	bool IsNotificationEnabled;
	bool IsAwayEnabled;

	CStringArray SendExecuteDestinations; // destinations of msgs being sent

protected:
	bool NameResolutionUpdate(CMessaggio *msg, bool remove);
	void NameResolutionLogOff();
	void NameResolutionGetList(CString *list);
	bool SendExecuteInvoke(CMessaggio *Msg);
	void SendExecuteMsgQueueAdd(CMessaggio *Msg);
	bool IsIgnoredThread(CMessaggio *NuovoMessaggio);
	void ReceiveMsgAdd(CMessaggio *NuovoMessaggio);
	bool Receive(CMessaggio *Msg);
	void UNFtoMsg(CString UNFmsg, CMessaggio *msg);

	CSharedMailslot mailslotServer;
	CMessaggiArray mMsgMultipart;
	CMessaggi sendExecuteMsgQueue;
	CRealPopupApp *m_rpApp;

	Dictionary<string, string> m_names;
	
	HANDLE mMailslot;
	unsigned int MsgThreadID_AutoCnt;		// MsgThreadID: timeout auto increment factor
	unsigned char MsgThreadID_Cnt;			// MsgThreadID: timeout increment factor

	bool isSendExecuteInProgress;			// true if a SendExecute thread is in progress
	SendExecuteInfo sendExecuteInfo;

};

#endif
