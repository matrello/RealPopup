/*
	RealPopup v2.3
	Code by Matro
	Rome, Italy, 1998-2002
	matro@usa.net   
	matro@realpopup.it

	last release 2-2002 
	designed for Windows 9x/NT kernels
*/

#include <afxtempl.h>
#include "RealPopup.h"

#define MSGFILE_MULTIPART_MSGSIZE 300

class CMsgFile
{
public:
	CString mPathSend;
	CString mPathReceive;
	CString mUser;
	int Scan();
	bool Invia(SMessaggio *Msg);
	CMsgFile();
	virtual ~CMsgFile();

	CArray<SMessaggio, SMessaggio> *mMessaggi;

protected:
	bool InviaMsg(SMessaggio *Msg);
	bool RiceviMsg(SMessaggio *Msg);
	void UNFtoMsg(CString UNFmsg, SMessaggio *msg);
	CArray<SMessaggio, SMessaggio> mMsgMultipart;

};
