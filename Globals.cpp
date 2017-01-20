/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#include "stdafx.h"
#include "resource.h"
#include "Globals.h"
#include "RPUsers.h"
#include "ReTracer.h"

#pragma data_seg("Shared")

BOOL g_InstComando=INST_COMANDO_NESSUNO;
BOOL g_InstLoaded=FALSE;
BOOL g_InstNewMsg=FALSE;
BOOL g_InstShowWindow=FALSE;
int  g_InstMsgInterno=0;
char g_InstTo[USERLIST_BUFFER_LEN]="CODEDBYMATRO";
char g_InstMsg[USERLIST_BUFFER_LEN]="V20JANUARY2000";

#pragma data_seg()
#pragma comment(linker,"/section:Shared,rws")

bool IsThreadRunning(CWinThread *thread, bool forceDelete)
{
	DWORD thState;

	if (!GetExitCodeThread(thread->m_hThread, &thState))
	{
		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "IsThreadRunning", "GetExitCodeThread() returned an error\n");
		return false;
	}

	if (thState!=STILL_ACTIVE)
	{
		if (forceDelete)
		{
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "IsThreadRunning", "thread not active: trying to delete...\n");
			delete thread;
			thread=0;
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "IsThreadRunning", "thread not active: ..deleted\n");
		}
		else
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "IsThreadRunning", "thread not active\n");
		
		return false;
	}

	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "IsThreadRunning", "thread active\n");
	return true;
}

void SetAutostart(int enable)
{
	HKEY key=NULL;
	CString sPath;
	char fullPath[255];

	GetModuleFileName(NULL,fullPath,255);
	sPath='"';
	sPath+=fullPath;
	sPath+='"'; 
	sPath+= " BOOT";

	if(RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL)==0)
	{
		if (enable)
			RegSetValueEx(key, "RealPopup", 0, REG_SZ, (const unsigned char *) LPCTSTR(sPath), sPath.GetLength());
		else
			RegDeleteValue(key, "RealPopup");
	}

	RegCloseKey(key);

	// remove old rp entry (prior v2.1)
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &key)==0)
		RegDeleteValue(key, "RealPopup");
	RegCloseKey(key);		
}

BOOL EnablePrivilege(HANDLE hToken, LPCTSTR szPrivName, BOOL fEnable)
{
   TOKEN_PRIVILEGES tp;
   tp.PrivilegeCount = 1;
   LookupPrivilegeValue(NULL, szPrivName, &tp.Privileges[0].Luid);
   tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
   AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
   return((GetLastError() == ERROR_SUCCESS));
}

int StrIndex(CStringArray *myArray, CString *myString)
{
	for (int i=0;i<myArray->GetSize();i++)
	{
		if (myArray->GetAt(i).Compare(*myString)==0)
			return i;
	}
	return -1;
}

int Normalizza0D(CString *msg)
{
	int iLen=msg->GetLength();
	while (msg->Replace("\r\n","\r")>0);
	while (msg->Replace("\r\r","\r")>0);
	while (msg->Replace("\n\n","\r")>0);
	return (iLen-msg->GetLength());
}

void ExitMFCApp()
{
    ASSERT(AfxGetMainWnd() != NULL);
    AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

BOOL IsNT(VOID)
{
	static int i = -1;
	if(i < 0)
	{
		OSVERSIONINFO vi = {sizeof(OSVERSIONINFO)};
		if(!GetVersionEx(&vi))
			return FALSE;

		i = (BOOL)(VER_PLATFORM_WIN32_NT == vi.dwPlatformId);
	}

	return (BOOL)i;
}

bool IsScreenSaverRunning()
{

	// (1) direct method for 9x/2000/XP platforms
	BOOL bMatro=0;
	
	if (SystemParametersInfo(SPI_GETSCREENSAVERRUNNING,0,&bMatro,0))
		return (bMatro!=0);
	else
	{
	// (2) handy-made method with NT platforms

       HDESK hDesktop;
       hDesktop = OpenDesktop(TEXT("screen-saver"), 0, FALSE, MAXIMUM_ALLOWED);

       if(hDesktop == NULL) 
	   {
           if(GetLastError() == ERROR_ACCESS_DENIED)
		   {
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "IsScreenSaverRunning", "OpenDesktop access denied (screensaver is probably running)\n");

			   return true;
		   }
		   ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "IsScreenSaverRunning", "OpenDesktop returned null (screensaver is not running)\n");

           return false;
       }

       CloseDesktop(hDesktop);

	   ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "IsScreenSaverRunning", "OpenDesktop returned ok (screensaver is running)\n");
       return true;
   }
}

double GetRPVersion(CString *Version)
{
	double destVersion;
	CString sdestVersion;

	if (Version->GetLength()==0)
		return 1.0;

	sdestVersion=Version->Mid(1);
	if (sdestVersion.Right(1)=="ß")
		destVersion=atof(LPCTSTR(sdestVersion.Left(sdestVersion.GetLength()-1)));
	else
		destVersion=atof(LPCTSTR(sdestVersion));

	return destVersion;
}

void ReTracer(ReTracerTraceLevels TraceLevel, ReTracerControlTypes ControlType, char *Control, ReTracerCommands Command, char *Position, char *format, ...)
{	
	CRealPopupApp *m_rpApp=(CRealPopupApp *)AfxGetApp();

	va_list args;
	va_start (args, format);
	m_rpApp->oReTracer.Trace(TraceLevel, ControlType, Control, Command, Position, format, args);
	va_end (args);
}

void ErrMsg(CWnd *owner, UINT nID, UINT nCAPTION, UINT icon)
{
	CString msg, caption;
	msg.LoadString(nID);
	caption.LoadString(nCAPTION);
	owner->MessageBox(msg, caption, icon);
}

bool IsBackdoor(CString *msg)
{
	if ( (msg->Left(4)=="@VER") || (msg->Left(4)=="@LOG") ||
		 (msg->Left(5)=="@FROM") )
		 return true;

	return false;
}

CMessaggio::CMessaggio()
{
	Notified=false;
	Priority=MSG_PRIORITY_NORMAL;
}

void CMessaggio::Copy (CMessaggio *dest)
{
	dest->Mitt=Mitt;
	dest->Dest=Dest;
	dest->Tutti=Tutti;
	dest->Data=Data;
	dest->Msg=Msg;
	dest->Cmd=Cmd;
	dest->Part=Part;
	dest->NParts=NParts;
	dest->Broadcast=Broadcast;
	dest->Version=Version;
	dest->DestVersion=DestVersion;
	dest->MsgThreadID=MsgThreadID;
	dest->Notified=Notified;
	dest->Priority=Priority;
	dest->ReceivedTime=ReceivedTime;
}

void CMessaggio::RiempiCampiData()
{
	CString Weekdays, weekday;
	Weekdays.LoadString(IDS_WEEKDAYS);
	CTime t = CTime::GetCurrentTime();
	AfxExtractSubString(weekday, Weekdays, t.GetDayOfWeek()-1, '|');
	Data = t.Format("%H:%M.%S, ") + weekday + t.Format(" %d");
	ReceivedTime=t;
}

CMessaggi::CMessaggi()
{
	LogPongCounter=0;
}

int CMessaggi::GetCountPriority(int priority)
{
	int cnt=0;

	for (int k=0; k<GetCount(); k++)
	{
		if (((CMessaggio) GetAt(k)).Priority==priority)
			cnt++;
	}

	return cnt;
}
bool CMessaggi::IsDuplicate(CMessaggio newElement)
{
	for (int k=0; k<GetCount(); k++)
	{
		if (((CMessaggio) GetAt(k)).Mitt.Compare(newElement.Mitt)==0)
		{
			if (((CMessaggio) GetAt(k)).Msg.Compare(newElement.Msg)==0)
			{
				CTimeSpan ts=newElement.ReceivedTime-((CMessaggio) GetAt(k)).ReceivedTime;
				if (ts.GetTotalSeconds()<20)
				{
					// msg with same mitt and body of a previous one received or being sent on
					// last 20 secs is considered a duplicate (such as msg sent to domains by rp)
					return true;
				}
			}
		}
	}

	return false;
}

INT_PTR CMessaggi::Add(CMessaggio newElement)
{
	switch (LogType)
	{
	case LogTypes::Messaggi:
		{
			if (IsDuplicate(newElement))
			{
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CMessaggi.Add", "Message from %s being added is considered a duplicate\n", newElement.Mitt);
				return -1;
			}

			if (newElement.Cmd=="PONG")
			{
				LogPongCounter++;
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "CMessaggi.Add", "Messages 'PONG' count [%i]\n", LogPongCounter);
			}
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "CMessaggi.Add", "Messages count [%i]\n", m_nSize+1);
			break;
		}
	case LogTypes::SendQueue:
		{
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "CMessaggi.Add", "SendQueue count [%i]\n", m_nSize+1);
			break;
		}
	}

	return CArray<CMessaggio,const CMessaggio&>::Add(newElement);
}

void CMessaggi::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
{
	switch (LogType)
	{
	case LogTypes::Messaggi:
		{
			CMessaggio element=(CMessaggio )GetAt(nIndex);
			if (element.Cmd=="PONG")
			{
				LogPongCounter--;
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "CMessaggi.RemoveAt", "Messages 'PONG' count [%i]\n", LogPongCounter);
			}
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "CMessaggi.RemoveAt", "Messages count [%i]\n", m_nSize-1);
			break;
		}
	case LogTypes::SendQueue:
		{
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "CMessaggi.RemoveAt", "SendQueue count [%i]\n", m_nSize-1);
			break;
		}
	}

	CArray<CMessaggio,const CMessaggio&>::RemoveAt(nIndex, nCount);
}

INT_PTR CPenMessaggi::Add(CPenMessaggio newElement)
{
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "penMessaggi::Add", "Pending messages count [%i]\n", m_nSize+1);

	return CArray<CPenMessaggio,const CPenMessaggio&>::Add(newElement);
}

void CPenMessaggi::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
{
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), NULL, ReTracerCommands(Update), "penMessaggi::RemoveAt", "Pending messages count [%i]\n", m_nSize-1);

	CArray<CPenMessaggio,const CPenMessaggio&>::RemoveAt(nIndex, nCount);
}

bool CNetworkDomains::IsDomain(CString hostname)
{
	for (int k=0;k<Domains.GetSize();k++)
	{
		if (Domains[k].Compare(hostname)==0)
			return true;
	}

	return false;
}
