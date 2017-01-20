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

*warning* please update RETRACER_BUILD accordling.

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

#pragma once

// * winsock version and defines
//   use the following #declares to use winsock 1 library
//   or uncomment to use your current winsock version.
#define _WINSOCK_1_1_
#pragma comment(lib, "wsock32.lib")
//   define your required winsock version (such as 2.2)
#define WINSOCK_VERSION MAKEWORD(1, 1)
//   include your required winsock header (such as <Winsock2.h>)
#include <Winsock.h>

// CStr class by soso_pub
#include "Str.h"

// the following enums must match ReTracer Console ones
enum ReTracerTypes{ReTracerConsole,ReTracerCpp,ReTracerCSharp,ReTracerVb,RechiRuntimeTrace,Other};
enum ReTracerControlTypes{Process,Text,List};
enum ReTracerCommands{Begin,End,Error,Update,Delete};
enum ReTracerTraceLevels{Critical,Warning,Information,Debug,DebugVerbose};

class CReTracer
{
public:
	CReTracer(void);
	~CReTracer(void);
	void Initialize(char *hostName, char *processName, bool fileEnabled, bool remoteEnabled, char *fileName=NULL, char *remoteIpAddress=NULL, int remotePort=NULL);
	void Terminate();
	void Trace(ReTracerTraceLevels TraceLevel, ReTracerControlTypes ControlType, char *Control, ReTracerCommands Command, char *Position, char *traceFormat, ...);
	void Trace(ReTracerTraceLevels TraceLevel, ReTracerControlTypes ControlType, char *Control, ReTracerCommands Command, char *Position, char *traceString, va_list ParamList);

	bool	fileEnabled;
	bool	remoteEnabled;
	bool	remoteConnected;

	CStr	hostName;
	CStr	processName;
	CStr	fileName;

private:
	bool InitializeRemote();
	bool SetTarget(char *remoteIpAddress, short remotePort, CStr *errorMessage);

	SOCKET	remoteSocket;


};
