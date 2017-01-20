/*
ReTracer v0.1
ReTracer.cpp C++ class build 002

Code by Matro
Rome, Italy, 2003
matro@email.it   
matro@realpopup.it

this source code contains extracts and ideas from Bogdan Rechi's
"Runtime Trace" package, published on Code Project at
http://www.codeproject.com/debug/runtime_trace.asp.

this source code uses the CStr class by soso_pub, published on
Code Project at http://www.codeproject.com/string/yasr.asp.
version used in this release is dated 22.june.2003.

11.2003 - 002 - first public beta

*warning* please update RETRACER_BUILD accordingly.

this code may be used in compiled form in any way you desire. This
file may be redistributed unmodified by any means PROVIDING it is 
not sold for profit without the authors written consent, and 
providing that this notice and the authors name is included. If 
the source code in this file is used in any commercial application 
then acknowledgement must be made to the author of this file 
(in whatever form you wish).

this file is provided "as is" with no expressed or implied warranty.
the author accepts no liability for any damage caused through use.

important: expect bugs.

use and enjoy. :-)

if you find bugs or want to suggest modifies and improvements, contact me
and I will fix/incorporate on next release.

matro

*/

// include the following #include if you use MFC precompiled headers
#include "stdafx.h"

#include <time.h>
#include "ReTracer.h"

// uncomment the following declare to compile and activate remote tracing;
// for security purposes, if you leave this undefined the remote tracing
// routines are not compiled at all. usually this is commented and
// the #define is placed somewhere else in your application source code.
// (for example, I use the remote tracing only on my freeware beta releases).
//#define RETRACER_REMOTETRACE_ACTIVATED
#include "Globals.h"

// uncomment the following declare to log *list control type*
// updates to the visual studio console
#define RETRACER_TRACETOCONSOLE_LIST_ACTIVATED

// uncomment the following declare to log *list control type*
// updates to the output file 
#define RETRACER_TRACETOFILE_LIST_ACTIVATED

// a change to the following defines should be coherent with the ReTracer Console
#define RETRACER_DEFAULT_PORT 7777
#define RETRACER_DEFAULT_IPADDRESS "127.0.0.1"
#define RETRACER_DEFAULT_FILENAME "ReTracer.log"
#define RETRACER_TRACESTRING_MAXLENGTH 1024
#define RETRACER_TRACESTRING_HEADER "ReTracer"
#define RETRACER_TRACESTRING_SPLIT "§"
#define RETRACER_TRACESTRING_VERSION "2"

// if you make a new build, change this define accordling
#define RETRACER_BUILD "002"

CReTracer::CReTracer(void)
{
	fileEnabled=remoteEnabled=false;
}

CReTracer::~CReTracer(void)
{
	if (fileEnabled || remoteEnabled)
		Terminate();
}

void CReTracer::Initialize(char *hostName, char *processName, bool fileEnabled, bool remoteEnabled, char *fileName, char *remoteIpAddress, int remotePort)
{
	bool showWelcome=false;

 	if (strlen(processName)==0)
		return;

	if (!fileEnabled && !remoteEnabled)
		return;

	if (hostName!=NULL)
		this->hostName=hostName;

	this->processName=processName;
	this->fileName="";

	if (fileEnabled)
	{
		char filePath[MAX_PATH];
		GetModuleFileName(NULL,filePath,MAX_PATH);
		*(strrchr(filePath,'\\')+1)=0;
		this->fileEnabled=showWelcome=true;

		if (fileName==NULL)
			this->fileName<<filePath<<RETRACER_DEFAULT_FILENAME;
		else
			this->fileName<<filePath<<fileName;

	}

#ifdef RETRACER_REMOTETRACE_ACTIVATED
	if (remoteEnabled)
	{
		CStr errorMessage;

		if (InitializeRemote())
		{
			if (SetTarget(remoteIpAddress, remotePort, &errorMessage))
				this->remoteEnabled=showWelcome=true;
			else
				Trace(ReTracerTraceLevels(Critical), ReTracerControlTypes(Process), NULL, ReTracerCommands(Error), "CReTracer.Initialize", errorMessage.Buffer());
		}
		else
			Trace(ReTracerTraceLevels(Critical), ReTracerControlTypes(Process), NULL, ReTracerCommands(Error), "CReTracer.Initialize", "cannot start socket library for remote logging.");
	}
#endif

	if (showWelcome)
		Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Process), NULL, ReTracerCommands(Begin), "CReTracer.Initialize", "\n\n\n%s trace session started\n", this->processName);
}

bool CReTracer::InitializeRemote()
{
	WSADATA wsaData;

	remoteConnected=false;
	if (WSAStartup(WINSOCK_VERSION,&wsaData))
		return false;

	if (hostName.Length()==0)
	{
		this->hostName.Realloc(128);
		if (gethostname(this->hostName.Buffer(), 128)!=0)
			hostName="(unknown)";
		hostName.Upper();
	}

	return true;
}

void CReTracer::Terminate()
{
	Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Process), NULL, ReTracerCommands(End), "CReTracer.Terminate", "\n\n\n%s trace session terminated\n", processName);
	fileEnabled=remoteEnabled=remoteConnected=false;

	shutdown(remoteSocket, 2);
	closesocket(remoteSocket);

	WSACleanup();	
}

void CReTracer::Trace(ReTracerTraceLevels TraceLevel, ReTracerControlTypes ControlType, char *Control, ReTracerCommands Command, char *Position, char *traceFormat, ...)
{
	va_list args;
	va_start (args, traceFormat);
	Trace(TraceLevel, ControlType, Control, Command, Position, traceFormat, args);
	va_end (args);
}

void CReTracer::Trace(ReTracerTraceLevels TraceLevel, ReTracerControlTypes ControlType, char *Control, ReTracerCommands Command, char *Position, char *traceFormat, va_list ParamList)
{
	if (!fileEnabled && !remoteEnabled)
		return;

	const char timeLongFormat[]="%y-%m-%d %H:%M.%S";
	const char timeShortFormat[]="%y%m%d%H%M%S";

	bool doVSLocalTrace=false, doFileLocalTrace=false;

	tm *sTime;
	char timeFormattedLong[25], timeFormattedShort[25];
	char tempBuffer[4][12], traceFormatted[RETRACER_TRACESTRING_MAXLENGTH], *pControl, *pPosition;
	CStr controlString, traceString, traceStringRemote;

	memset(tempBuffer, 0, 48);
	memset(traceFormatted, 0, RETRACER_TRACESTRING_MAXLENGTH);

	__time64_t long_time;
	_time64( &long_time );
	sTime = _localtime64( &long_time );

	strftime(timeFormattedLong, (size_t) 25, timeLongFormat, sTime);
	strftime(timeFormattedShort, (size_t) 25, timeShortFormat, sTime);

	if (Control==NULL)
		pControl=&tempBuffer[0][11];
	else
	{
		pControl=Control;
		controlString<<(strlen(Control)>0?" [":"")<<Control<<(strlen(Control)>0?"] ":"");
	}

	if (Position==NULL)
		pPosition=&tempBuffer[0][11];
	else
		pPosition=Position;

	_vsnprintf(traceFormatted,RETRACER_TRACESTRING_MAXLENGTH-sizeof(timeLongFormat),traceFormat,ParamList);

	if (ControlType==ReTracerControlTypes(Process) || ControlType==ReTracerControlTypes(Text))
		doVSLocalTrace=doFileLocalTrace=true;

#ifdef RETRACER_TRACETOCONSOLE_LIST_ACTIVATED && _DEBUG
	if (ControlType==ReTracerControlTypes(List))
		doVSLocalTrace=true;
#endif

#ifdef RETRACER_TRACETOFILE_LIST_ACTIVATED
	if (ControlType==ReTracerControlTypes(List))
		doFileLocalTrace=true;
#endif

	if (doVSLocalTrace || doFileLocalTrace)
	{
		traceString<<timeFormattedLong;
		switch(TraceLevel)
		{
			case ReTracerTraceLevels(Information):
				traceString<<" I ";
				break;
			case ReTracerTraceLevels(Critical):
				traceString<<" C ";
				break;
			case ReTracerTraceLevels(Warning):
				traceString<<" W ";
				break;
			default:
				traceString<<" D ";
				break;
		}

		traceString<<pPosition<<controlString.Buffer()<<": "<<traceFormatted;
	}

	if (doVSLocalTrace)
		OutputDebugString(traceString);

	if (doFileLocalTrace)
	{
		if (fileEnabled)
		{
			FILE *f=fopen(fileName, "a");
			if (f!=NULL)
			{
				fputs(traceString.Buffer(),f);
				fclose(f);
			}
		}
	}

#ifdef RETRACER_REMOTETRACE_ACTIVATED
	if (remoteEnabled)
	{
		memset(tempBuffer, 0, 12);
		traceStringRemote<<RETRACER_TRACESTRING_HEADER<<RETRACER_TRACESTRING_SPLIT
			<<RETRACER_TRACESTRING_VERSION<<RETRACER_TRACESTRING_SPLIT
			<<itoa(ReTracerTypes(ReTracerCpp), tempBuffer[0], 10)<<RETRACER_TRACESTRING_SPLIT
			<<RETRACER_BUILD<<RETRACER_TRACESTRING_SPLIT<<hostName.Buffer()<<RETRACER_TRACESTRING_SPLIT
			<<processName.Buffer()<<RETRACER_TRACESTRING_SPLIT<<pPosition<<RETRACER_TRACESTRING_SPLIT
			<<timeFormattedShort<<RETRACER_TRACESTRING_SPLIT<<itoa(TraceLevel, tempBuffer[1], 10)<<RETRACER_TRACESTRING_SPLIT
			<<itoa(ControlType, tempBuffer[2], 10)<<RETRACER_TRACESTRING_SPLIT<<pControl<<RETRACER_TRACESTRING_SPLIT
			<<itoa(Command, tempBuffer[3], 10)<<RETRACER_TRACESTRING_SPLIT<<traceFormatted;

		if (remoteConnected)
		{
			if (send(remoteSocket, (const char *)(LPCTSTR)traceStringRemote, traceStringRemote.Length(), 0)==SOCKET_ERROR)
				OutputDebugString("ReTracer: cannot send to remote tracer: socket send() returned an error.");
		}
	}
#endif

}

bool CReTracer::SetTarget(char *remoteIpAddress, short remotePort, CStr *errorMessage)
{
	struct sockaddr_in serv;
	struct sockaddr_in client;
	char localAddress[]=RETRACER_DEFAULT_IPADDRESS;

	if (remotePort==NULL)
		remotePort=RETRACER_DEFAULT_PORT;
	if (remoteIpAddress==NULL)
		remoteIpAddress=(char *)localAddress;

	if (remoteConnected)
	{
		shutdown(remoteSocket, 2);
		closesocket(remoteSocket);
	}

	if ((remoteSocket=socket(AF_INET, SOCK_DGRAM, 0))==INVALID_SOCKET) 
	{
		*errorMessage="cannot create socket.";
		return false;
	} 

	memset((void *)&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(const_cast<LPTSTR>(remoteIpAddress));
	serv.sin_port = htons(remotePort);

	memset((void *)&client, 0, sizeof(client));
	client.sin_family = AF_INET;

	try
	{
		if (bind(remoteSocket, (struct sockaddr*)&client, sizeof(client))==SOCKET_ERROR)
			throw CStr("error on binding the socket.");

		if (!(remoteConnected=SOCKET_ERROR != connect(remoteSocket, (struct sockaddr*)&serv, sizeof(serv))))
			throw CStr("error on connecting to socket.");
	}
	catch (CStr &szMan)
	{
		*errorMessage=szMan;
		shutdown(remoteSocket, 2);
		closesocket(remoteSocket);

		return false;
	}

	return true;
}
