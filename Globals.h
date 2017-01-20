/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

////////////////////////////////////
#define VERSIONE	"V2.6" // ß
#define BUILD		"167"
//#define BETA
#define BETA_ANNO	2005
#define BETA_MESE	3
#define BETA_GIORNO	11
#ifdef VERSIONELE
  #define TIPO "le"
#else
  #define TIPO ""
#endif
//#ifdef BETA
  #define RETRACER_REMOTETRACE_ACTIVATED
//#endif
////////////////////////////////////

/*
    RealPopup published releases history

	2.6 bld 166 09/02/2005 
	2.5 bld 149 17/09/2004 
	2.5 bld 142 21/03/2004
	2.5 bld 141 21/03/2004
	2.5 bld 138 04/02/2004
	2.5 bld 136 04/02/2004
	2.4 bld 095 24/04/2003
	2.4 bld 093 03/03/2003
	2.4 bld 086 20/02/2003
	2.3 bld 048 09/09/2002
	2.3 bld 047 01/03/2002
	2.2 bld 041 05/02/2002
	2.2         18/09/2001
	2.0         24/05/2000
	1.5         28/07/1999
	1.3         07/03/1999
	1.2         18/02/1999
	1.0         16/03/1998


	open source classes used by rp

	CCmdLine + CCmdParam 1999	   Chris Losinger										http://www.smalleranimals.com
	CGetFilenameControl	 2001	   PJ Naughter											http://www.naughter.com
	CGetFilenameDialog	 2001	   PJ Naughter											http://www.naughter.com
	Dictionary			 1998	   James Curran JamesCurran@CompuServe.Com				http://www.codeguru.com/cpp_mfc/Dictionary.shtml
	CMRUComboBox		 9/9/1998  Michael Dunn mdunn@inreach.com
	CRichEditCtrlEx		 01/18/99  AndrewFo
	CStr				 21/6/2003 soso_pub												http://www.codeproject.com/string/yasr.asp
	CSystemTray			 4/8/2003  Chris Maunder cmaunder@mail.com modified by matro	http://www.codeproject.com/shell/systemtray.asp
	MMStatusBar			 3/9/2003  Kiran T. Sanjeeva sanjeeva_kiran@hotmail.com			http://www.codeguru.com/statusbar/statdemo.html

*/

#if !defined(REALPOPUP_GLOBALS_H_INCLUDED)
#define REALPOPUP_GLOBALS_H_INCLUDED

#if !defined(AFX_REALPOPUP_H__08B63E85_B76B_11D1_BEDC_0060087A5B02__INCLUDED_)
#include "Realpopup.h"
#endif

#include <afxtempl.h>

#define SPI_GETSCREENSAVERRUNNING 0x0072

const MARG_SINISTRO = 5;
const MARG_DESTRO = 6;
const SPAZ_VERTICALE = 3;
const SPAZ_ORIZZONTALE = 4;

#define LOG_LEFTMSG 40

#define RP_WINSIZEX 353
#define RP_WINSIZEY 324

#define MSG_BUFFER_LEN 65535
#define MSG_BUFFER_ZERO_REPL 21
#define USERLIST_BUFFER_LEN 2048

#define MSG_PRIORITY_NONE	0
#define MSG_PRIORITY_HIGH	1
#define MSG_PRIORITY_NORMAL 2
#define MSG_PRIORITY_LOW	3

#define OPZ_MSN_UNDEFINED	0
#define OPZ_MSN_TAKECONTROL	1
#define OPZ_MSN_LEAVE		2

#define WM_RPNETWORK_PONG WM_USER+11

#define MSG_RPNETWORKBROWSERPING	"RPNETWORKBROWSERPING"
#define MSG_RPNAMERESOLUTIONPING	"RPNAMERESOLUTIONPING"
#define MSG_RPNAMERESOLUTIONLOGOFF	"RPNAMERESOLUTIONLOGOFF"

#define INST_COMANDO_NESSUNO	0
#define INST_COMANDO_POSRESET	1

#define AUTOCOMPLETE_RANK_MAX	30
#define WORKSTATIONS_RANK_MAX	20

extern BOOL g_InstComando;
extern BOOL g_InstLoaded;
extern BOOL g_InstNewMsg;
extern BOOL g_InstShowWindow;
extern int  g_InstMsgInterno;
extern char g_InstTo[USERLIST_BUFFER_LEN];
extern char g_InstMsg[USERLIST_BUFFER_LEN];

bool IsThreadRunning(CWinThread *thread, bool forceDelete=true);
void SetAutostart(int enable);
BOOL EnablePrivilege(HANDLE hToken, LPCTSTR szPrivName, BOOL fEnable);
int StrIndex(CStringArray *myArray, CString *myString);
bool IsBackdoor(CString *msg);
BOOL IsNT(VOID);
bool IsScreenSaverRunning();
void ExitMFCApp();
double GetRPVersion(CString *Version);
void ReTracer(ReTracerTraceLevels TraceLevel, ReTracerControlTypes ControlType, char *Control, ReTracerCommands Command, char *Position, char *format, ...);
void ErrMsg(CWnd *owner, UINT nID, UINT nCAPTION = MSG_DIALOG_CAPTION, UINT icon = MB_ICONEXCLAMATION);
int Normalizza0D(CString *msg);

struct SUser
{
	CString name;
	CString members;
	int	group;
	int	chkIgnore;
	int	chkIncludeFavorites;
	int includeAsGroup;
	int	chkPopUp;
	int	chkBalloon;
	int	chkBalloonPreview;
	int	chkUsesRealPopup;
	int cboUsesRealPopup;
	int	chkRedirect;
	int	chkLog;
	int	chkOnlineWarn;
	CString	Redirect;
	CString	Log;
	int	AllUsers;

	bool isDatDefaults;
};

class CMessaggio
{
	public: 
		CString Mitt;			// sender
		CString Dest;			// receiver
		CString Tutti;			// receivers list (or same of Dest if one receiver only)
		CString Data;			// received datetime (string format)
		CString Msg;			// msg body
		CString Cmd;			// internal command (if any)
		CString Part;			// msg part of a multipart msg
		CString NParts;			// number of parts of a multipart msg
		CString Broadcast;		// contiene realdest per i broadcast quando rispondono col pong
		CString Version;		// sender rp version
		CString DestVersion;	// receiver rp version (if available)
		CString MsgThreadID;	// msg thread id

		bool    Notified;		// true if msg was shown on balloon
		int		Priority;		// msg priority (for send queue)
		CTime	ReceivedTime;	// received datetime

		CMessaggio();
		void Copy (CMessaggio *dest);
		void RiempiCampiData();
};

class CPenMessaggio
{
	public:
		CMessaggio  Msg;
		int			Counter;		// timeout counter
		int			PongCounter;	// PONGs to receive counter
		bool		PongOnly;		// if true, instructs to just pong (no msg handling)
									// (actually used on CRPNetworkBrowser's PINGs from sender)
		CString		PongMsg;		// info taken from received PONG
		CString		PongVersion;	// "				"
		CString		PongMitt;		// "				"		(starting from v2.4)
		CString		UserDest;		// original user input of receiver
									// (it may be translated from username to pcname)
};

class CMessaggi : public CArray<CMessaggio,const CMessaggio&>
{
	public:
		enum LogTypes{Messaggi,SendQueue};

		CMessaggi();
		INT_PTR Add(CMessaggio newElement);
		void RemoveAt(INT_PTR nIndex, INT_PTR nCount=1);
		int GetCountPriority(int priority);
		bool IsDuplicate(CMessaggio newElement);

		LogTypes LogType;

	private:
		int LogPongCounter;
};	

class CPenMessaggi : public CArray<CPenMessaggio,const CPenMessaggio&>
{
public:
	enum LogTypes{Messaggi,SendQueue};

	INT_PTR Add(CPenMessaggio newElement);
	void RemoveAt(INT_PTR nIndex, INT_PTR nCount=1);

	LogTypes LogType;
};	

class CNetworkDomains
{
public:
	bool IsDomain(CString hostname);
	CStringArray Domains;
};

#endif //(REALPOPUP_GLOBALS_H_INCLUDED)
