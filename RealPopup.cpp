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
#include <atlbase.h>
#include "RealPopup.h"

#include "dlgbars.h"
#include "SendDlg.h"
#include "RealPopupDlg.h"
#include "CmdLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef BOOL (STDMETHODCALLTYPE FAR * LPFNGETUSERPROFILEDIR) 
( HANDLE hToken, LPTSTR lpProfileDir, LPDWORD lpcchSize ); 

BEGIN_MESSAGE_MAP(CRealPopupApp, CWinApp)
	//{{AFX_MSG_MAP(CRealPopupApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CRealPopupApp::OnHelp)
END_MESSAGE_MAP()

CRealPopupApp::CRealPopupApp()
{


}

CRealPopupApp::~CRealPopupApp()
{

}

void CRealPopupApp::OnHelp()
{
	int r;
	r=0;
}

// ------------------------------------------------------------------
CRealPopupApp theApp;	// welcome to RealPopup, developed by Matro,
						// Rome, Italy from 1998 to 2004. =o)
						// (every new year I change the above)
// ------------------------------------------------------------------

BOOL CRealPopupApp::InitInstance()
{

	 //_CrtSetBreakAlloc(4466); // memory leak debug control
	 //_CrtSetBreakAlloc(4259); // memory leak debug control
	 //_CrtSetBreakAlloc(42985); // memory leak debug control

	CoInitialize(NULL);
    if (!AfxOleInit())
    {
        AfxMessageBox("Ole Initialization Failed");
        return FALSE;
    }
	AfxEnableControlContainer();
	InitCommonControls();
	SetRegistryKey("Matro");

	opz_LogInitialized=false;
	opz_LogRemoteInitialized=false;

	if(g_InstLoaded)
	{
		// rp is already executing: pass parameters to it, then exit
		CCmdLine cmdLine;

		g_InstShowWindow=TRUE;

		if (cmdLine.SplitLine(__argc,__argv)<1)
			return 0;

		if (cmdLine.HasSwitch("-noactivate"))
			g_InstShowWindow=FALSE;

		if (cmdLine.HasSwitch("-posreset"))
			g_InstComando=INST_COMANDO_POSRESET;

		if (cmdLine.HasSwitch("-send"))
		{
			if (cmdLine.GetArgumentCount("-send")==2)
			{
				CString sendTo, sendMsg;

				sendTo=cmdLine.GetArgument("-send",0);
				sendMsg=cmdLine.GetArgument("-send",1);
				
				strcpy(g_InstTo,LPCTSTR(sendTo));
				strcpy(g_InstMsg,LPCTSTR(sendMsg));
				g_InstMsgInterno=0;
				g_InstNewMsg=TRUE;
			}
		}

		if (cmdLine.HasSwitch("-h") || cmdLine.HasSwitch("-help"))
		{
			g_InstMsgInterno=MSG_CMDLINE_HELP;
			g_InstNewMsg=TRUE;
		}

		return 0;
	}
	g_InstLoaded=TRUE;

	#ifdef USE_RESDLL

	// session-wide hotkey
	hkRealPopup=GlobalAddAtom("RealPopupHotKey");

	// session mutex to allow setup to detect the instance running
	CreateMutex(NULL, FALSE, "RealPopupByMatro"); 

	// load resource DLLs with localized dialogs and strings
	CString filename;
	char fullPath[255];
	GetModuleFileName(NULL,fullPath,255);
	*(strrchr(fullPath,'\\')+1)=0;
	filename=fullPath;
	filename += "RPres*.DLL";

	CString		resAbout;
	CFileFind	resDir;
	BOOL bFound = resDir.FindFile(filename);
	if (!bFound)
	{
		AfxMessageBox("Cannot find any resource DLL; you should have one RPres_{langid}.dll file at least.");
		return false;
	}

	while (bFound)
	{
		bFound=resDir.FindNextFile();
		if ((m_hInstDLL = LoadLibrary(resDir.GetFilePath())) == NULL)
		{
			AfxMessageBox("Failed to load resource DLL while enumerating.");
			return FALSE;
		}
		AfxSetResourceHandle(m_hInstDLL); 
		resAbout.LoadString(MSG_LOCALIZE_ABOUT);
		if (resAbout.Find("V2.6")!=-1)
		{	// looks for DLLs versioned 2.6 only

			m_ResDLLs.Add(resDir.GetFileName());
			m_ResDLLsAbouts.Add(resAbout);
			resAbout.LoadString(MSG_LOCALIZE_DESC);
			m_ResDLLsDescs.Add(resAbout);
		}

		FreeLibrary(m_hInstDLL);
	}

	// loads localized resource DLL defined during the setup or selected by the user
	DWORD dwCount=255;
	HKEY key=NULL;
	m_ResActual="RPres_eng.dll";
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Matro\\RealPopup\\Environment", 0, KEY_ALL_ACCESS, &key)==ERROR_SUCCESS)
	{
		RegQueryValueEx(key, "ResourceDLL", NULL, NULL, (LPBYTE) m_ResActual.GetBuffer(255), &dwCount); m_ResActual.ReleaseBuffer();
		RegCloseKey(key);
	}
	m_ResActual=GetProfileString("Environment","ResourceDLL",m_ResActual);

	if ((m_hInstDLL = LoadLibrary(m_ResActual)) == NULL)
	{
		AfxMessageBox("Failed to load main resource DLL; language set back to english: try to restart RealPopup.");
		WriteProfileString("Environment","ResourceDLL","RPres_eng.dll");
		return FALSE;
	}
	else
	{
		AfxSetResourceHandle(m_hInstDLL); 
		resAbout.LoadString(MSG_LOCALIZE_ABOUT);
		if (resAbout.Find("V2.6")==-1)
		{
			AfxMessageBox("The specified RealPopup language resource is not version 2.6: language set back to english; try to restart RealPopup.");
			WriteProfileString("Environment","ResourceDLL","RPres_eng.dll");
			return FALSE;
		}

	}
	#endif

	// setup user selected font
	NONCLIENTMETRICS matro;
	matro.cbSize=sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), (void *)&matro, 0);
	font_lfSystem=matro.lfMessageFont;
	font_System.CreateFontIndirect(&font_lfSystem);

	// setup environment variables
	env_DefAllUsers.LoadString(DEF_ALLUSERS);
	env_DefUserName.LoadString(DEF_USERNAME);
	env_AutoMsgFrom.LoadString(AUTOMSG_BETA_FROM);
	env_AutoMsgTo.LoadString(DEF_USERNAME);
	env_Version=VERSIONE;
	CString smyVersion=env_Version.Mid(1);
	if (smyVersion.Right(1)=="ß")
		env_nVersion=atof(LPCTSTR(smyVersion.Left(smyVersion.GetLength()-1)));
	else
		env_nVersion=atof(LPCTSTR(smyVersion));

	// read required system settings
	GetRegEntries();

	// read user registry settings
	SetRegUserDefaults();
	GetRegUserPrefs(HKEY_LOCAL_MACHINE);
	GetRegUserPrefs(HKEY_CURRENT_USER);

	// get user profile folder (windows shell one or user defined in opz_RPdatFolder)
	GetUserProfileFolder();

	// init logging features
	LogInitialize();

	// set and load RankedArrays
	oAutoCompleteRank.ItemsMax=AUTOCOMPLETE_RANK_MAX;
	oAutoCompleteRank.FileName=env_UserProfileFolder;
	oAutoCompleteRank.FileName+="RP" + m_LoggedUser + ".rnk";
	oAutoCompleteRank.SectionName="AutoCompleteRank";
	oAutoCompleteRank.Load();

	oWorkstationsRank.ItemsMax=WORKSTATIONS_RANK_MAX;
	oWorkstationsRank.FileName=env_UserProfileFolder;
	oWorkstationsRank.FileName+="RP" + m_LoggedUser + ".rnk";
	oWorkstationsRank.SectionName="WorkstationsRank";
	oWorkstationsRank.Load();

	// set user specified fonts
	SetRegUserFonts();

	// main dialog: RealPopup runs here :-)
	ShowMainDialog();

	Close();

	m_ResDLLs.RemoveAll();
	m_ResDLLsAbouts.RemoveAll();
	m_ResDLLsDescs.RemoveAll();

	GlobalDeleteAtom(hkRealPopup);

	#ifdef USE_RESDLL
	FreeLibrary(m_hInstDLL);
	#endif

	CoUninitialize();

	return FALSE;
}

void CRealPopupApp::GetRegEntries()
{

	DWORD dwCount=255;
	HKEY key=NULL;
	int pos=0;

	m_Workstation=env_DefUserName;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\control\\ComputerName\\ComputerName", 0, KEY_READ, &key)==ERROR_SUCCESS)
		RegQueryValueEx(key, "ComputerName", NULL, NULL, (LPBYTE) m_Workstation.GetBuffer(255), &dwCount);

	m_Workstation.ReleaseBuffer();
	m_Workstation.MakeUpper();
	RegCloseKey(key);

	dwCount=255;
	if(RegOpenKeyEx(HKEY_CLASSES_ROOT,"HTTP\\shell\\open\\command", 0, KEY_ALL_ACCESS, &key)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(key, NULL, NULL, NULL, (LPBYTE) m_Browser.GetBuffer(255), &dwCount)==ERROR_SUCCESS)
		{
			char *codedbyMatro=m_Browser.GetBuffer();
			bool bDentroVirgolette=false;

			while (*codedbyMatro!=0)
			{
				if (*codedbyMatro==34)
					bDentroVirgolette=!bDentroVirgolette;
				if ((*codedbyMatro==32) && (!bDentroVirgolette))
				{
					*codedbyMatro=0;
					break;
				}
				codedbyMatro++;
			}
			m_Browser.ReleaseBuffer();
		}
	}
	RegCloseKey(key);

	dwCount=255;
	m_LoggedUser=env_DefUserName;
	WNetGetUser(NULL,m_LoggedUser.GetBuffer(255), &dwCount);
	m_LoggedUser.ReleaseBuffer();
	m_LoggedUser.MakeUpper();
	env_AutoMsgTo=m_LoggedUser;

	dwCount=255;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", 0, KEY_ALL_ACCESS, &key)==ERROR_SUCCESS)
		RegQueryValueEx(key, "CachePrimaryDomain", NULL, NULL, (LPBYTE) m_UserDomain.GetBuffer(255), &dwCount);
	m_UserDomain.ReleaseBuffer();
	RegCloseKey(key);
}

void CRealPopupApp::GetRegUserPrefs(HKEY hKey)
{
	CString first="WelcomeTo",rpusers; first += env_Version;
	rpusers.LoadString(MSG_NBROWSER_RPUSERS);

	DWORD dwCount=255, dwData;
	HKEY key=NULL;

	if(RegOpenKeyEx(hKey,"SOFTWARE\\Matro\\RealPopup\\Environment", 0, KEY_ALL_ACCESS, &key)==ERROR_SUCCESS)
	{
		dwCount=4; RegQueryValueEx(key, first, NULL, NULL, (LPBYTE) &opz_FirstTime, &dwCount);
		dwCount=4; RegQueryValueEx(key, "BeepOnNewMessage", NULL, NULL, (LPBYTE) &opz_chkBeep, &dwCount);
		dwCount=4; RegQueryValueEx(key, "BalloonOnMsgDeliveryIssues", NULL, NULL, (LPBYTE) &opz_chkBalloonOnMsgDeliveryIssues, &dwCount);
		dwCount=4; RegQueryValueEx(key, "AlwaysOnTop", NULL, NULL, (LPBYTE) &opz_chkTop, &dwCount);
		dwCount=4; RegQueryValueEx(key, "MinimizeOnDelLastMessage", NULL, NULL, (LPBYTE) &opz_chkMinimize, &dwCount);
		dwCount=4; RegQueryValueEx(key, "MinimizeWhenReplied", NULL, NULL, (LPBYTE) &opz_chkMinimizeRepl, &dwCount);
		dwCount=4; RegQueryValueEx(key, "DelOriginalWhenReplied", NULL, NULL, (LPBYTE) &opz_chkDelOriginal, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WarnOnExit", NULL, NULL, (LPBYTE) &opz_chkWarn, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WarnOnCancelingSend", NULL, NULL, (LPBYTE) &opz_chkWarnDel, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WarnWhenSendToDomain", NULL, NULL, (LPBYTE) &opz_chkWarnDomain, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WarnRPProtocolNotActive", NULL, NULL, (LPBYTE) &opz_chkWarnRPProtocolNotActive, &dwCount);
		dwCount=4; RegQueryValueEx(key, "AutoStart", NULL, NULL, (LPBYTE) &opz_chkAutoStart, &dwCount);
		dwCount=4; RegQueryValueEx(key, "PlayWav", NULL, NULL, (LPBYTE) &opz_chkWav, &dwCount);
		dwCount=4; RegQueryValueEx(key, "ShowLastMessage", NULL, NULL, (LPBYTE) &opz_chkLastMsg, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowTop", NULL, NULL, (LPBYTE) &opz_wndTop, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowLeft", NULL, NULL, (LPBYTE) &opz_wndLeft, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowHeight", NULL, NULL, (LPBYTE) &opz_wndHeight, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowWidth", NULL, NULL, (LPBYTE) &opz_wndWidth, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowSndTop", NULL, NULL, (LPBYTE) &opz_sndwndTop, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowSndLeft", NULL, NULL, (LPBYTE) &opz_sndwndLeft, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowSndHeight", NULL, NULL, (LPBYTE) &opz_sndwndHeight, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WindowSndWidth", NULL, NULL, (LPBYTE) &opz_sndwndWidth, &dwCount);
		dwCount=4; dwData=opz_HotKey; RegQueryValueEx(key, "HotKey", NULL, NULL, (LPBYTE) &dwData, &dwCount); opz_HotKey=dwData;
		dwCount=4; dwData=opz_HotKeymd; RegQueryValueEx(key, "HotKeyModifiers", NULL, NULL, (LPBYTE) &dwData, &dwCount); opz_HotKeymd=dwData;
		dwCount=4; RegQueryValueEx(key, "HyperlinksDoubleClick", NULL, NULL, (LPBYTE) &opz_chkDblClick, &dwCount);
		dwCount=4; RegQueryValueEx(key, "LeftButtonTray", NULL, NULL, (LPBYTE) &opz_chkLeftTray, &dwCount);
		dwCount=4; RegQueryValueEx(key, "AwayOnScreenSaver", NULL, NULL, (LPBYTE) &opz_chkScrAway, &dwCount);
		dwCount=4; RegQueryValueEx(key, "DetermineFavoriteUsers", NULL, NULL, (LPBYTE) &opz_chkNBDetermineFavorites, &dwCount);
		dwCount=4; RegQueryValueEx(key, "SendAlwaysPcName", NULL, NULL, (LPBYTE) &opz_chkSendPcName, &dwCount);
		dwCount=4; RegQueryValueEx(key, "CaretTopOnReply", NULL, NULL, (LPBYTE) &opz_chkReplyTop, &dwCount);
		dwCount=4; RegQueryValueEx(key, "UseEnterToSendMessage", NULL, NULL, (LPBYTE) &opz_chkCrSend, &dwCount);
		dwCount=4; RegQueryValueEx(key, "QuoteMessage", NULL, NULL, (LPBYTE) &opz_chkQuote, &dwCount);
		dwCount=4; RegQueryValueEx(key, "DisableDelKeyForAWhile", NULL, NULL, (LPBYTE) &opz_chkDisableDel, &dwCount);
		dwCount=4; RegQueryValueEx(key, "AutoStartWarn", NULL, NULL, (LPBYTE) &opz_chkAutoStartWarn, &dwCount);
		dwCount=4; RegQueryValueEx(key, "AutoStartMsg", NULL, NULL, (LPBYTE) &opz_chkAutoStartMsg, &dwCount);
		dwCount=4; RegQueryValueEx(key, "MessengerService", NULL, NULL, (LPBYTE) &opz_Msn, &dwCount);
		dwCount=4; RegQueryValueEx(key, "MsgSentNum", NULL, NULL, (LPBYTE) &opz_MsgSentNum, &dwCount);
		dwCount=4; RegQueryValueEx(key, "MsgReceivedNum", NULL, NULL, (LPBYTE) &opz_MsgReceivedNum, &dwCount);

		dwCount=255; RegQueryValueEx(key, "WavFile", NULL, NULL, (LPBYTE) opz_Wav.GetBuffer(255), &dwCount); opz_Wav.ReleaseBuffer();
		dwCount=255; RegQueryValueEx(key, "LastDomain", NULL, NULL, (LPBYTE) opz_LastDomain.GetBuffer(255), &dwCount); opz_LastDomain.ReleaseBuffer();
		dwCount=255; RegQueryValueEx(key, "FontMessages", NULL, NULL, (LPBYTE) font_sRichEdit.GetBuffer(255), &dwCount); font_sRichEdit.ReleaseBuffer();
		dwCount=255; RegQueryValueEx(key, "FontInfos", NULL, NULL, (LPBYTE) font_sEdit.GetBuffer(255), &dwCount); font_sEdit.ReleaseBuffer();
	}
	RegCloseKey(key);

	if(RegOpenKeyEx(hKey,"SOFTWARE\\Matro\\RealPopup\\Advanced", 0, KEY_ALL_ACCESS, &key)==ERROR_SUCCESS)
	{

		dwCount=4; RegQueryValueEx(key, "DisableMessageLengthCheck", NULL, NULL, (LPBYTE) &opz_chkLenChk, &dwCount);
		dwCount=4; RegQueryValueEx(key, "FullInternalPings", NULL, NULL, (LPBYTE) &opz_chkFullInternalPings, &dwCount);
		dwCount=4; RegQueryValueEx(key, "PingTimeOut", NULL, NULL, (LPBYTE) &opz_PingTimeOut, &dwCount);
		dwCount=4; RegQueryValueEx(key, "DomainSendToRealPopupClientsOnly", NULL, NULL, (LPBYTE) &opz_chkDomainSendRP, &dwCount);
		dwCount=4; RegQueryValueEx(key, "NameResolution", NULL, NULL, (LPBYTE) &opz_chkNameResolution, &dwCount);
		dwCount=4; RegQueryValueEx(key, "NameResolutionMinInterval", NULL, NULL, (LPBYTE) &opz_NameResolutionMinInterval, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WinpopupSendMailslotEnabled", NULL, NULL, (LPBYTE) &opz_chkWinpopupSendMailslotEnabled, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WinpopupSendNetbiosEnabled", NULL, NULL, (LPBYTE) &opz_chkWinpopupSendNetbiosEnabled, &dwCount);
		dwCount=4; RegQueryValueEx(key, "WinpopupSendSMBEnabled", NULL, NULL, (LPBYTE) &opz_chkWinpopupSendSMBEnabled, &dwCount);
		dwCount=4; RegQueryValueEx(key, "Log", NULL, NULL, (LPBYTE) &opz_Log, &dwCount);
		dwCount=4; RegQueryValueEx(key, "LogRemote", NULL, NULL, (LPBYTE) &opz_LogRemote, &dwCount);
		dwCount=4; RegQueryValueEx(key, "LogRemotePort", NULL, NULL, (LPBYTE) &opz_LogRemotePort, &dwCount);

		dwCount=255; RegQueryValueEx(key, "RPdatFolder", NULL, NULL, (LPBYTE) opz_RPdatFolder.GetBuffer(255), &dwCount); opz_RPdatFolder.ReleaseBuffer();
		dwCount=255; RegQueryValueEx(key, "RPdatDefaultsFile", NULL, NULL, (LPBYTE) opz_RPdatDefaultsFile.GetBuffer(255), &dwCount); opz_RPdatDefaultsFile.ReleaseBuffer();
		dwCount=255; RegQueryValueEx(key, "LogRemoteIP", NULL, NULL, (LPBYTE) opz_LogRemoteIP.GetBuffer(255), &dwCount); opz_LogRemoteIP.ReleaseBuffer();
	}
	RegCloseKey(key);
}

void CRealPopupApp::SetRegUserPrefs()
{

	CString first="WelcomeTo"; first += env_Version;

	WriteProfileInt("Environment",first,opz_FirstTime);
	WriteProfileInt("Environment","BeepOnNewMessage",opz_chkBeep);
	WriteProfileInt("Environment","BalloonOnMsgDeliveryIssues",opz_chkBalloonOnMsgDeliveryIssues);
	WriteProfileInt("Environment","AlwaysOnTop",opz_chkTop);
	WriteProfileInt("Environment","MinimizeOnDelLastMessage",opz_chkMinimize);
	WriteProfileInt("Environment","MinimizeWhenReplied",opz_chkMinimizeRepl);
	WriteProfileInt("Environment","DelOriginalWhenReplied",opz_chkDelOriginal);
	WriteProfileInt("Environment","WarnOnExit",opz_chkWarn);
	WriteProfileInt("Environment","WarnOnCancelingSend",opz_chkWarnDel);
	WriteProfileInt("Environment","WarnWhenSendToDomain",opz_chkWarnDomain);
	WriteProfileInt("Environment","WarnRPProtocolNotActive",opz_chkWarnRPProtocolNotActive);
	WriteProfileInt("Environment","PlayWav",opz_chkWav);
	WriteProfileInt("Environment","ShowLastMessage",opz_chkLastMsg);
	WriteProfileInt("Environment","AutoStart",opz_chkAutoStart);
	WriteProfileInt("Environment","WindowTop",opz_wndTop);
	WriteProfileInt("Environment","WindowLeft",opz_wndLeft);
	WriteProfileInt("Environment","WindowHeight",opz_wndHeight);
	WriteProfileInt("Environment","WindowWidth",opz_wndWidth);
	WriteProfileInt("Environment","WindowSndTop",opz_sndwndTop);
	WriteProfileInt("Environment","WindowSndLeft",opz_sndwndLeft);
	WriteProfileInt("Environment","WindowSndHeight",opz_sndwndHeight);
	WriteProfileInt("Environment","WindowSndWidth",opz_sndwndWidth);
	WriteProfileInt("Environment","HotKey",opz_HotKey);
	WriteProfileInt("Environment","HotKeyModifiers",opz_HotKeymd);
	WriteProfileInt("Environment","HyperlinksDoubleClick",opz_chkDblClick);
	WriteProfileInt("Environment","LeftButtonTray",opz_chkLeftTray);
	WriteProfileInt("Environment","AwayOnScreenSaver",opz_chkScrAway);
	WriteProfileInt("Environment","DetermineFavoriteUsers",opz_chkNBDetermineFavorites);
	WriteProfileInt("Environment","SendAlwaysPcName",opz_chkSendPcName);
	WriteProfileInt("Environment","UseEnterToSendMessage",opz_chkCrSend);
	WriteProfileInt("Environment","QuoteMessage",opz_chkQuote);
	WriteProfileInt("Environment","CaretTopOnReply",opz_chkReplyTop);
	WriteProfileInt("Environment","DisableDelKeyForAWhile",opz_chkDisableDel);
	WriteProfileString("Environment","WavFile",opz_Wav);
	WriteProfileString("Environment","LastDomain",opz_LastDomain);
	WriteProfileString("Environment","ResourceDLL",m_ResActual);
	WriteProfileInt("Environment","AutoStartWarn",opz_chkAutoStartWarn);
	WriteProfileInt("Environment","AutoStartMsg",opz_chkAutoStartMsg);
	WriteProfileInt("Environment","MessengerService",opz_Msn);

	WriteProfileInt("Advanced","DisableMessageLengthCheck",opz_chkLenChk);
	WriteProfileInt("Advanced","FullInternalPings",opz_chkFullInternalPings);
	WriteProfileInt("Advanced","PingTimeOut",opz_PingTimeOut);
	WriteProfileInt("Advanced","DomainSendToRealPopupClientsOnly",opz_chkDomainSendRP);
	WriteProfileInt("Advanced","NameResolution",opz_chkNameResolution);
	WriteProfileInt("Advanced","NameResolutionMinInterval",opz_NameResolutionMinInterval);
	WriteProfileString("Advanced","RPdatFolder",opz_RPdatFolder);
	WriteProfileInt("Advanced","WinpopupSendMailslotEnabled",opz_chkWinpopupSendMailslotEnabled);
	WriteProfileInt("Advanced","WinpopupSendNetbiosEnabled",opz_chkWinpopupSendNetbiosEnabled);
	WriteProfileInt("Advanced","WinpopupSendSMBEnabled",opz_chkWinpopupSendSMBEnabled);

	WriteProfileInt("Advanced","Log",opz_Log);
	WriteProfileInt("Advanced","LogRemote",opz_LogRemote);
	WriteProfileString("Advanced","LogRemoteIP",opz_LogRemoteIP);
	WriteProfileInt("Advanced","LogRemotePort",opz_LogRemotePort);

	WriteProfileInt("Environment","MsgSentNum",opz_MsgSentNum);
	WriteProfileInt("Environment","MsgReceivedNum",opz_MsgReceivedNum);

	if (font_RichEditchanged)
	{
		LOGFONT lf;
		font_RichEdit.GetLogFont(&lf);
		CString myFont;
		myFont.Format("%s,%i,%i,%i,%i,%i,%i,%i,%i",
			lf.lfFaceName,	lf.lfHeight,	lf.lfWeight,
			lf.lfItalic,	lf.lfUnderline,	lf.lfStrikeOut,
			lf.lfCharSet,	lf.lfQuality,	lf.lfPitchAndFamily);
		WriteProfileString("Environment","FontMessages",myFont);
	}

	if (font_Editchanged)
	{
		LOGFONT lf;
		font_Edit.GetLogFont(&lf);
		CString myFont;
		myFont.Format("%s,%i,%i,%i,%i,%i,%i,%i,%i",
			lf.lfFaceName,	lf.lfHeight,	lf.lfWeight,
			lf.lfItalic,	lf.lfUnderline,	lf.lfStrikeOut,
			lf.lfCharSet,	lf.lfQuality,	lf.lfPitchAndFamily);
		WriteProfileString("Environment","FontInfos",myFont);
	}

	oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRealPopupApp.SetRegUserPrefs", "RealPopup registry settings updated\n");
}

CString CRealPopupApp::TogliUserName(CString * srg)
{

	LPTSTR buffer=srg->GetBuffer(MAX_SEND_LEN);
	char sep[]=",";
	CString dest;

	char *token;
	token=strtok(buffer,sep);
	while (token!=NULL)
	{
		if (m_Workstation!=token)
		{
			dest += ",";
			dest += token;
		}
		token=strtok(NULL,sep);
	}

	return dest;
}

void CRealPopupApp::ShowHelp(CWnd *wnd, char *ancora)
{
	CString sHelp, sPath;
	sPath=m_pszHelpFilePath;
	sPath=sPath.Left(sPath.ReverseFind('\\')+1);
	sHelp.LoadString(ID_HELPFILE);
	sHelp="file:///" + sPath+ sHelp + ancora;
	ShellExecute(wnd->m_hWnd,"open",m_Browser,sHelp,"",SW_SHOWNORMAL);
}

void CRealPopupApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// disabled, so custom F1 handling is used
}


void CRealPopupApp::LogInitialize(void)
{
	CString logFileName, logProcessName;

	logFileName="RP" + m_LoggedUser + ".log";
	logProcessName.Format("RealPopup %s build %s - %s (%X)", env_Version, BUILD, m_LoggedUser, AfxGetThread()->m_nThreadID);
	oReTracer.Initialize(m_Workstation.GetBuffer(), logProcessName.GetBuffer(), (opz_Log!=0), (opz_LogRemote!=0), logFileName.GetBuffer(), opz_LogRemoteIP.GetBuffer(), opz_LogRemotePort);
	m_Workstation.ReleaseBuffer();
	logProcessName.ReleaseBuffer();
	logFileName.ReleaseBuffer();
	opz_LogRemoteIP.ReleaseBuffer();
}

void CRealPopupApp::LogTerminate()
{
	oReTracer.Terminate();
}
void CRealPopupApp::SetRegUserDefaults(void)
{
	CString first="WelcomeTo",rpusers; first += env_Version;
	rpusers.LoadString(MSG_NBROWSER_RPUSERS);

	opz_FirstTime=0;
	opz_chkBeep=TRUE;
	opz_chkBalloonOnMsgDeliveryIssues=FALSE;
	opz_chkTop=FALSE;
	opz_chkMinimize=FALSE;
	opz_chkMinimizeRepl=FALSE;
	opz_chkDelOriginal=FALSE;
	opz_chkWarn=TRUE;
	opz_chkWarnDel=TRUE;
	opz_chkWarnDomain=TRUE;
	opz_chkWarnRPProtocolNotActive=TRUE;
	opz_chkAutoStart=FALSE;
	opz_chkWav=FALSE;
	opz_chkLastMsg=TRUE;
	opz_wndTop=0;
	opz_wndLeft=0;
	opz_wndHeight=0;
	opz_wndWidth=0;
	opz_sndwndTop=0;
	opz_sndwndLeft=0;
	opz_sndwndHeight=0;
	opz_sndwndWidth=0;
	opz_HotKey=0;
	opz_HotKeymd=0;
	opz_chkDblClick=FALSE;
	opz_chkLeftTray=FALSE;
	opz_chkScrAway=TRUE;
	opz_chkNBDetermineFavorites=TRUE;
	opz_chkSendPcName=FALSE;
	opz_chkReplyTop=FALSE;
	opz_chkCrSend=FALSE;
	opz_chkQuote=TRUE;
	opz_chkDisableDel=TRUE;
	opz_Wav="";
	opz_chkAutoStartWarn=FALSE;
	opz_chkAutoStartMsg=FALSE;
	opz_Msn=OPZ_MSN_UNDEFINED;
	opz_MsgSentNum=0;
	opz_MsgReceivedNum=0;
	opz_LastDomain=rpusers;

	opz_chkLenChk=FALSE;
	opz_chkFullInternalPings=TRUE;
	opz_PingTimeOut=100;
	opz_chkDomainSendRP=0;
	opz_chkNameResolution=TRUE;
	opz_NameResolutionMinInterval=1000;
	opz_RPdatFolder="";
	opz_RPdatDefaultsFile="";
	opz_chkWinpopupSendMailslotEnabled=TRUE;
	opz_chkWinpopupSendNetbiosEnabled=TRUE;
	opz_chkWinpopupSendSMBEnabled=TRUE;

	opz_Log=FALSE;
	opz_LogRemote=FALSE;
	opz_LogRemoteIP="";
	opz_LogRemotePort=7777;

	font_sRichEdit="Arial,-11,400,0,0,0,0,1,34";
	font_sEdit="Arial,-11,400,0,0,0,0,1,34";
}

void CRealPopupApp::SetRegUserFonts(void)
{
	LOGFONT lf;
	CString myParam;

	memset(&lf,0,sizeof(LOGFONT));
	AfxExtractSubString(myParam, font_sRichEdit, 0, ',');
	strcpy((char *)&lf.lfFaceName,LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 1, ',');
	lf.lfHeight=atol(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 2, ',');
	lf.lfWeight=atol(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 3, ',');
	lf.lfItalic=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 4, ',');
	lf.lfUnderline=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 5, ',');
	lf.lfStrikeOut=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 6, ',');
	lf.lfCharSet=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 7, ',');
	lf.lfQuality=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sRichEdit, 8, ',');
	lf.lfPitchAndFamily=atoi(LPCTSTR(myParam));
	font_RichEdit.DeleteObject();
	font_RichEdit.CreateFontIndirect(&lf);
	font_RichEditchanged=true;

	AfxExtractSubString(myParam, font_sEdit, 0, ',');
	strcpy((char *)&lf.lfFaceName,LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 1, ',');
	lf.lfHeight=atol(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 2, ',');
	lf.lfWeight=atol(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 3, ',');
	lf.lfItalic=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 4, ',');
	lf.lfUnderline=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 5, ',');
	lf.lfStrikeOut=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 6, ',');
	lf.lfCharSet=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 7, ',');
	lf.lfQuality=atoi(LPCTSTR(myParam));
	AfxExtractSubString(myParam, font_sEdit, 8, ',');
	lf.lfPitchAndFamily=atoi(LPCTSTR(myParam));
	font_Edit.DeleteObject();
	font_Edit.CreateFontIndirect(&lf);
	font_Editchanged=true;
}

void CRealPopupApp::ShowMainDialog()
{
	// here is where RealPopup runs :-)
	CRealPopupDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
}

void CRealPopupApp::Close()
{
	SetRegUserPrefs();
	oAutoCompleteRank.Save();
	oWorkstationsRank.Save();

	// terminate logging features
	LogTerminate();
}

void CRealPopupApp::GetUserProfileFolder()
{
	HKEY key=NULL;
	unsigned char userpath[255];
	unsigned long userpathlen=255;

	// user defined folder (\RealPopup is added)
	if(opz_RPdatFolder.GetLength()>0)
	{
		env_UserProfileFolder=opz_RPdatFolder+"\\RealPopup\\";
		CreateDirectory(LPCTSTR(env_UserProfileFolder),NULL);
		return;
	}

	// default: RealPopup executable folder
	char fullPath[255];
	GetModuleFileName(NULL,fullPath,255);
	*(strrchr(fullPath,'\\')+1)=0;
	env_UserProfileFolder=fullPath;

	// User Application Data folder (Windows Shell)
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 0, KEY_READ, &key)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(key, "AppData", NULL, NULL, (unsigned char *)userpath, &userpathlen)==ERROR_SUCCESS)
		{
			env_UserProfileFolder=userpath;
			env_UserProfileFolder+="\\RealPopup\\";
			RegCloseKey(key);
			CreateDirectory(LPCTSTR(env_UserProfileFolder),NULL);
			return;
		}
	}
	RegCloseKey(key);

	// User Profile folder (Windows Shell)
	HMODULE g_hUserEnvLib=NULL;
	LPFNGETUSERPROFILEDIR GetUserProfileDirectory = NULL; 

	g_hUserEnvLib = LoadLibrary( _T("userenv.dll") );
	if (!g_hUserEnvLib)
		return;

	GetUserProfileDirectory = (LPFNGETUSERPROFILEDIR) GetProcAddress( g_hUserEnvLib, "GetUserProfileDirectoryA" ); 
	if (!GetUserProfileDirectory)
		return;

	HANDLE hToken=NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		int yo = GetLastError();
		FreeLibrary(g_hUserEnvLib);
		return;
	}

	if (!GetUserProfileDirectory(hToken, (char *)userpath, &userpathlen))
	{
		CloseHandle(hToken);
		FreeLibrary(g_hUserEnvLib);
		return;
	}

	env_UserProfileFolder=userpath;
	env_UserProfileFolder+="\\RealPopup\\";
	CreateDirectory(LPCTSTR(env_UserProfileFolder),NULL);

	CloseHandle(hToken);
	FreeLibrary(g_hUserEnvLib);
	return;
}

