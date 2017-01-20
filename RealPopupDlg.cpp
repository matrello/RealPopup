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
#include "RealPopup.h"
#include "RichEditCtrlEx.h"

#include "dlgbars.h"
#include "SendDlg.h"
#include "DlgService.h"

#include "TbStDialog.h"

#include <afxpriv.h>

#include "FntPropertySheet.h"
#include "PPEnvironment.h"
#include "PPLanguages.h"
#include "PPMessages.h"
#include "PPUsers.h"

#include "RPMsgEditCtrl.h"
#include "RealPopupDlg.h"

#include "RPNetworkBrowser.h"

#include "mmsystem.h"

#define EM_SHOWSCROLLBAR	  (WM_USER + 96)
#define WM_NETTHREADENDED	  3 // thread ended normally (may happen! :-)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
int AutoStartWarnTimer=4000;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRPMsgEditCtrl m_AboutMsg;

	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CRealPopupApp	*m_rpApp;

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRealPopupDlg::CRealPopupDlg(CWnd* pParent)	: CTbStDialog(CRealPopupDlg::IDD, pParent)
{
	m_hIcon = m_rpApp->LoadIcon(IDR_MAINFRAME);
	bMinimizeOnEsc=true;

	m_TrayFlash=TRAYFLASH_OFF;
	MsgAttuale = -1;
	oldMsgAttuale = -2;
	m_RedirectEnabled = TRUE;
	m_AwayByScreenSaver = false;

	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_ACCMENU1), RT_ACCELERATOR); 
	m_hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCMENU1)); 

	m_ProtocolsHandler.m_wndStatusBar = &m_wndStatusBar;
	m_ProtocolsHandler.m_TrayIcon = &m_TrayIcon;
	m_ProtocolsHandler.m_Users = &m_Users;
	m_ProtocolsHandler.RealPopupProtocol.m_Users = &m_Users;
	
	m_ProtocolsHandler.m_RedirectEnabled=&m_RedirectEnabled;
}

void CRealPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CTbStDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealPopupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRealPopupDlg, CTbStDialog)
	//{{AFX_MSG_MAP(CRealPopupDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_ENDSESSION()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_BTN_COPY, OnBtnCopy)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POWERBROADCAST, OnPowerBroadcast)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
    ON_COMMAND(ID_BTN_NETWORK, OnBtnNetwork)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRAY_OPEN, ID_TRAY_AWAY, OnUpdateTrayMenu)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BTN_NEW, ID_BTN_SAVE, OnUpdateToolbarMenu)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND_RANGE(ID_TRAY_OPEN, ID_TRAY_AWAY, OnTrayMenu)
	ON_COMMAND_RANGE(ID_BTN_NEW, ID_BTN_FORWARD, OnBtnTlbNuovo)
	ON_COMMAND_RANGE(ID_BTN_DELTHREAD, ID_BTN_SAVE, OnBtnTlbMessaggio)
	ON_COMMAND_RANGE(ID_BTN_OPTIONS, ID_BTN_HELP, OnBtnTlbOther)
	ON_UPDATE_COMMAND_UI_RANGE(STATUS_WPPROTOCOLENABLED, STATUS_RPPROTOCOLENABLED, OnUpdateStatusPanes)
END_MESSAGE_MAP()

BOOL CRealPopupDlg::OnInitDialog()
{
	// ToolBar and StatusBar init
		IDToolbar = IDR_TOOLBAR1;
		rgbMask = RGB(255,0,255);
		IDStatusIdleMessage = AFX_IDS_IDLEMESSAGE;

		bStatusBarEnabled=true;
		
		sStatusBarPanels iStatusBarPanels={ 3,
			{ID_SEPARATOR, STATUS_WPPROTOCOLENABLED, STATUS_RPPROTOCOLENABLED,},
			{NULL, NULL, NULL,},
			{NULL, NULL, NULL,},
			{NULL, TOOLTIP_STATUS_WPENABLED, TOOLTIP_STATUS_RPENABLED},
			{NULL, TOOLTIP_STATUS_WPDISABLED, TOOLTIP_STATUS_RPDISABLED},
			{SBPS_NOBORDERS, SBPS_NORMAL, SBPS_NORMAL,},
			{160, 16, 16,} };

		StatusBarPanels=iStatusBarPanels;

		HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_TRAYICON),RT_GROUP_ICON);
		StatusBarPanels.EnabledIcons[1]=(HICON)LoadImage(hInst,MAKEINTRESOURCE(IDR_WPENABLED),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		StatusBarPanels.DisabledIcons[1]=(HICON)LoadImage(hInst,MAKEINTRESOURCE(IDR_WPDISABLED),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		StatusBarPanels.EnabledIcons[2]=(HICON)LoadImage(hInst,MAKEINTRESOURCE(IDR_RPENABLED),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		StatusBarPanels.DisabledIcons[2]=(HICON)LoadImage(hInst,MAKEINTRESOURCE(IDR_RPDISABLED),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

		CTbStDialog::OnInitDialog();
	////

	m_BalloonNotifyDelay=0;
	m_BalloonNotifyRequest=false;
	senddlgAct=0;
	netThreadAbort=0;
	delayToolbarActivate=0;
	SendDlgLast=NULL;
	m_OptionsOpen=false;
	m_Show=false;

	HICON TrayIcon = m_rpApp->LoadIcon(IDR_TRAYICON);
	SetIcon(m_hIcon, TRUE);

    HMENU hMenu = ::GetSystemMenu(this->m_hWnd, FALSE);
    EnableMenuItem( hMenu, SC_CLOSE, MF_GRAYED ); 

	CRect rec(50,50,2500,2050);		
	int h=m_MsgEdit.Create(ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY | ES_WANTRETURN |
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_VSCROLL, rec, this, TXT_MAIN_MSG);
	m_MsgEdit.SetBackgroundColor(FALSE, GetSysColor(COLOR_BTNFACE));
	m_MsgEdit.m_Browser=&((CRealPopupApp *) AfxGetApp())->m_Browser;
	m_MsgEdit.SendMessage(EM_SETTEXTMODE, TM_RICHTEXT | TM_MULTILEVELUNDO | TM_SINGLECODEPAGE ,0);

#ifdef BETA
		m_rpApp->m_TitleString.Format("RealPopup %s %s build %s", TIPO, m_rpApp->env_Version, BUILD);
#else
		m_rpApp->m_TitleString = m_rpApp->env_AutoMsgFrom;
#endif

	SetWindowText(m_rpApp->m_TitleString);

    m_LblFrom =	(CStatic *)GetDlgItem(IDC_MAIN_FROM);    
    m_LblTo =	(CStatic *)GetDlgItem(IDC_MAIN_TO);    
    m_FldFrom =	(CEdit *)GetDlgItem(TXT_MAIN_FROM);    
    m_FldTo =	(CEdit *)GetDlgItem(TXT_MAIN_TO);    
    m_FldInfo =	(CEdit *)GetDlgItem(TXT_MAIN_INFO);    

	m_hTimer = SetTimer(1,2000,NULL);

	SetFonts();
	CtrlPosiziona();

	if (m_rpApp->opz_chkTop)
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	if (m_rpApp->opz_wndWidth>0)
	{
		SetWindowPos(NULL, m_rpApp->opz_wndLeft, m_rpApp->opz_wndTop,
			m_rpApp->opz_wndWidth, m_rpApp->opz_wndHeight,SWP_NOZORDER);
	}

	m_Users.UserProfileFolder=m_rpApp->env_UserProfileFolder;
	m_Users.Load();
	if (m_Users.myUsers.GetSize()<2)
	{	// the following groups are added by default

		if (m_Users.myUsers.GetSize()==0)
		{
			SUser newUser;
			m_Users.GetDefaultProfile(&newUser, true);
			if (m_TrayIcon.IsWin2K())
			{
				newUser.chkPopUp=0;
				newUser.chkBalloon=1;
			}
			m_Users.myUsers.Add(newUser);
		}

		if (m_Users.myUsers.GetSize()==1)
		{	// this group should help users to better understand how
			// to tell rp some users has got winpopup/messenger for
			// sure. basing on feedbacks, it works! :-)

			SUser newUser;
			m_Users.GetDefaultProfile(&newUser, false);
			newUser.name="WINPOPUP";
			newUser.members="MYPC1,MYPC2";
			newUser.group=true;
			newUser.chkUsesRealPopup=1;
			newUser.cboUsesRealPopup=1;
			m_Users.myUsers.InsertAt(0,newUser);
		}
	}

	return TRUE;
}

CRealPopupDlg::~CRealPopupDlg()
{
	m_ProtocolsHandler.Close(false);
	for (int k=0; k<lpnrAllocated.GetSize(); k++)
	{
		LPNETRESOURCE lpnr=(LPNETRESOURCE)lpnrAllocated.GetAt(k);
		free(lpnr);
	}
}

void CRealPopupDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		{
			myShowWindow(false);
			break;
		}
	default:
		{
			CTbStDialog::OnSysCommand(nID, lParam);
		}
	}

    OnSetMessageString(IDStatusIdleMessage);
}

void CRealPopupDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CTbStDialog::OnPaint();
	}
}

HCURSOR CRealPopupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CRealPopupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRealPopupDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CRealPopupDlg::OnBtnTlbNuovo(UINT nID) 
{
	CSendDlg *dlg;
	int k=0;

	if (senddlgAct==MAX_SENDDLG) return;
	dlg=new CSendDlg();

	for (k=0;k<senddlgAct;k++)
	{
		if (SendDlg[k]->m_hWnd==NULL)
		{
			delete(SendDlg[k]);
			break;
		}
	}
	if (k==senddlgAct)
		k=senddlgAct++;
	SendDlg[k]=dlg;
	SendDlgLast=dlg;

	myShowWindow(true);

	dlg->m_ProtocolsHandler = &m_ProtocolsHandler;
	dlg->m_Users = &m_Users;
	int MsgDaEliminare=-1;

	switch (nID)
	{
	case ID_BTN_FORWARD:
		{
			CString fmtDaA;
			fmtDaA.LoadString(FMT_LOG_FROMTO);

			CMessaggio Messaggio = m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale);
			dlg->PredefMsg = Messaggio.Msg;
			dlg->m_forward = TRUE;
			dlg->m_MsgEdit.ForwardInfo.Format(fmtDaA, Messaggio.Mitt, Messaggio.Tutti, Messaggio.Data);
			break;
		}
	case ID_BTN_ANSWER:
		{
			MsgDaEliminare=MsgAttuale;
			CMessaggio Messaggio = m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale);
			dlg->PredefTo  = Messaggio.Mitt;
			dlg->m_forward = FALSE;

			if (Messaggio.Cmd=="NOTRECEIVED")
			{
				char *Matro=strstr(LPCTSTR(Messaggio.Msg),"---");
				if (Matro!=NULL)
				{
					dlg->PredefMsg = (Matro + 4);
					dlg->m_noquote = TRUE;
				}
			}
			else
				dlg->PredefMsg = Messaggio.Msg;
			
			break;
		}
	case ID_BTN_ANSWERALL:
		{
			MsgDaEliminare=MsgAttuale;
			CMessaggio Messaggio = m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale);
			dlg->PredefTo  = Messaggio.Mitt + m_rpApp->TogliUserName(&Messaggio.Tutti);
			dlg->PredefMsg = Messaggio.Msg;
			dlg->m_forward = FALSE;
			dlg->CurrentMsgsThreadID  = Messaggio.MsgThreadID;
			break;
		}
	}

	dlg->Create(IDD_DLG_SENDMESSAGE,this);
	dlg->m_RealPopupDlg=this;
	dlg->MsgDaEliminare=MsgDaEliminare;
	dlg->TipoSend=nID;
	dlg->ShowWindow(SW_SHOW);
}

void CRealPopupDlg::OnAfterSendOK(int MsgDaEliminare, UINT nID)
{	// the function is called from last Send dialog open

	if( m_rpApp->opz_chkDelOriginal && 
		(nID==ID_BTN_ANSWER || nID==ID_BTN_ANSWERALL) &&
		(m_ProtocolsHandler.Messaggi.GetSize()>0) )
	{	// handles 'delete original message when replied'

		m_ProtocolsHandler.Messaggi.RemoveAt(MsgDaEliminare);

		if (MsgAttuale>=m_ProtocolsHandler.Messaggi.GetSize())
			MsgAttuale-=1;
		if (m_ProtocolsHandler.Messaggi.GetSize()==0)
		{
			MsgAttuale= -1;
			if (m_rpApp->opz_chkMinimize)
				myShowWindow(false);
		}
		MsgVisualizza(true);
	}
	if( m_rpApp->opz_chkMinimizeRepl && (nID==ID_BTN_ANSWER || nID==ID_BTN_ANSWERALL) )
		ShowWindow(false);
}


void CRealPopupDlg::OnBtnTlbMessaggio(UINT nID) 
{

	switch (nID)
	{

	case ID_BTN_PREV:
		{
			MsgAttuale-=1;
			MsgVisualizza(true);
			break;
		}
	case ID_BTN_NEXT:
		{
			MsgAttuale+=1;
			MsgVisualizza(true);
			break;
		}
	case ID_BTN_DEL:
		{
			m_ProtocolsHandler.Messaggi.RemoveAt(MsgAttuale);
			if (MsgAttuale>=m_ProtocolsHandler.Messaggi.GetSize())
				MsgAttuale-=1;
			if (m_ProtocolsHandler.Messaggi.GetSize()==0)
			{
				MsgAttuale= -1;
				if (m_rpApp->opz_chkMinimize)
					myShowWindow(false);
			}
			MsgVisualizza(true);
			break;
		}
	case ID_BTN_DELALL:
		{
			while (m_ProtocolsHandler.Messaggi.GetSize()!=0)
				m_ProtocolsHandler.Messaggi.RemoveAt(0);

			MsgAttuale= -1;
			if (m_rpApp->opz_chkMinimize)
				myShowWindow(false);

			MsgVisualizza(true);
			break;
		}
	case ID_BTN_DELTHREAD:
		{
			bool addIgnore=true;
			for (int k=0;k<m_ProtocolsHandler.IgnoredThreads.GetSize();k++)
			{
				if (m_ProtocolsHandler.Messaggi[MsgAttuale].MsgThreadID.Compare(m_ProtocolsHandler.IgnoredThreads.GetAt(k))==0)
				{
					addIgnore=false;
					break;
				}
			}
			if (addIgnore)
				m_ProtocolsHandler.IgnoredThreads.Add(m_ProtocolsHandler.Messaggi[MsgAttuale].MsgThreadID);

			m_ProtocolsHandler.Messaggi.RemoveAt(MsgAttuale);
			if (MsgAttuale>=m_ProtocolsHandler.Messaggi.GetSize())
				MsgAttuale-=1;
			if (m_ProtocolsHandler.Messaggi.GetSize()==0)
			{
				MsgAttuale= -1;
				if (m_rpApp->opz_chkMinimize)
					myShowWindow(false);
			}
			MsgVisualizza(true);
			break;
		}
	case ID_BTN_RESUMETHREADS:
		{
			m_ProtocolsHandler.IgnoredThreads.RemoveAll();
			break;
		}
	case ID_BTN_SAVE:
		{
			MsgSave(&m_ProtocolsHandler.Messaggi[MsgAttuale]);
		}
	}
}

void CRealPopupDlg::MsgVisualizza(bool bForza)
{
	CMessaggio Messaggio;
	CString	   Info;

	MsgToolTipUpd();
    OnSetMessageString(IDStatusIdleMessage);

	if (m_rpApp->opz_chkLastMsg && !bForza)
	{
		MsgAttuale=m_ProtocolsHandler.Messaggi.GetSize()-1;
		while (m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale).Cmd=="PONG")
		{
			if (--MsgAttuale==-1)
			{
				GetDlgItem(TXT_MAIN_FROM)->SetWindowText("");
				GetDlgItem(TXT_MAIN_TO)->SetWindowText("");
				GetDlgItem(TXT_MAIN_MSG)->SetWindowText("");
				GetDlgItem(TXT_MAIN_INFO)->SetWindowText("");
				oldMsgAttuale=-2;
				return;
			}
		}
	}
	else
	{
		if (MsgAttuale==-1)
		{
			if (m_ProtocolsHandler.Messaggi.GetSize()==0)
			{
				GetDlgItem(TXT_MAIN_FROM)->SetWindowText("");
				GetDlgItem(TXT_MAIN_TO)->SetWindowText("");
				GetDlgItem(TXT_MAIN_MSG)->SetWindowText("");
				GetDlgItem(TXT_MAIN_INFO)->SetWindowText("");
				oldMsgAttuale=-2;
				return;
			}
			else
				MsgAttuale=0;
		}
	}

	if (MsgAttuale!=-1)
	{
		while (m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale).Cmd=="PONG")
		{
			if (++MsgAttuale==m_ProtocolsHandler.Messaggi.GetSize())
			{
				GetDlgItem(TXT_MAIN_FROM)->SetWindowText("");
				GetDlgItem(TXT_MAIN_TO)->SetWindowText("");
				GetDlgItem(TXT_MAIN_MSG)->SetWindowText("");
				GetDlgItem(TXT_MAIN_INFO)->SetWindowText("");
				oldMsgAttuale=-2;
				return;
			}
		}
	}

	Messaggio = m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale);
	Info.Format("%s (%i/%i)", LPCTSTR(Messaggio.Data), MsgAttuale+1, m_ProtocolsHandler.Messaggi.GetSize());
	GetDlgItem(TXT_MAIN_INFO)->SetWindowText(Info);

	if (oldMsgAttuale!=MsgAttuale || bForza)
	{
		GetDlgItem(TXT_MAIN_FROM)->SetWindowText(Messaggio.Mitt);
		
		CHARFORMAT cf;
		cf.bCharSet=m_rpApp->font_lfSystem.lfCharSet;
		cf.dwMask=CFM_CHARSET;
		m_MsgEdit.SetDefaultCharFormat(cf);
		m_MsgEdit.SetSelectionCharFormat(cf);
		m_MsgEdit.SetWordCharFormat(cf);
		m_MsgEdit.SetWindowText(Messaggio.Msg);

		if (Messaggio.Tutti.GetLength()>0)
			GetDlgItem(TXT_MAIN_TO)->SetWindowText(Messaggio.Tutti);
		else
			GetDlgItem(TXT_MAIN_TO)->SetWindowText(Messaggio.Dest);

		m_MsgEdit.QuoteHandling(false);

		m_MsgEdit.SendMessage(EM_SHOWSCROLLBAR,SB_VERT,false);
		CtrlPosiziona(true);
		m_MsgEdit.SendMessage(EM_SHOWSCROLLBAR,SB_VERT,true);
	}
	oldMsgAttuale=MsgAttuale;
}

void CRealPopupDlg::OnUpdateToolbarMenu(CCmdUI* pCmdUI) 
{
	bool isProtocolEnabled=(m_ProtocolsHandler.IsRealPopupProtocolEnabled || m_ProtocolsHandler.IsWinPopupProtocolEnabled);

	switch (pCmdUI->m_nID)
	{
	case ID_BTN_NEW:
		{
			pCmdUI->Enable(strcmp(TIPO,"le") &&	isProtocolEnabled);
			break;
		}
	case ID_BTN_DEL:
		{
			pCmdUI->Enable(MsgAttuale!=-1 &&
				(delayToolbarActivate==0 || !m_rpApp->opz_chkDisableDel));
			break;
		}
	case ID_BTN_SAVE:
	case ID_BTN_COPY:
	case ID_BTN_DELALL:
		{
			pCmdUI->Enable(MsgAttuale!=-1);
			break;
		}
	case ID_BTN_ANSWER:
	case ID_BTN_ANSWERALL:
	case ID_BTN_FORWARD:
		{
			pCmdUI->Enable(MsgAttuale!=-1 && strcmp(TIPO,"le") && isProtocolEnabled);
			break;
		}
	case ID_BTN_PREV:
		{
			pCmdUI->Enable(m_ProtocolsHandler.Messaggi.GetSize()>0 && MsgAttuale>0);
			break;
		}
	case ID_BTN_NEXT:
		{
			pCmdUI->Enable(m_ProtocolsHandler.Messaggi.GetSize()>0 && MsgAttuale<(m_ProtocolsHandler.Messaggi.GetSize()-1));
			break;
		}
	case ID_BTN_DELTHREAD:
		{
			if (MsgAttuale==-1)
				pCmdUI->Enable(false);
			else
				pCmdUI->Enable( (m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale).MsgThreadID.Left(2)=="ID") && 
								(m_ProtocolsHandler.Messaggi.GetAt(MsgAttuale).Tutti.Find(',')>-1)  && 
								delayToolbarActivate==0);
			break;
		}
	case ID_BTN_RESUMETHREADS:
		{
			pCmdUI->Enable(m_ProtocolsHandler.IgnoredThreads.GetSize()>0);
			break;
		}
	}

}

void CRealPopupDlg::OnClose() 
{
	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRealPopupDlg.OnClose", "called\n");

	KillTimer(m_hTimer);
	m_TrayIcon.RemoveIcon();

	for (int k=--senddlgAct;k>=0;k--)
		delete(SendDlg[k]);

	EndDialog(0);
}

void CRealPopupDlg::OnEndSession(BOOL bEnding)
{
	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRealPopupDlg.OnEndSession", "called\n");

	if (bEnding)
	{
		OnClose();
		m_rpApp->Close();
	}

	return;
}

void CRealPopupDlg::CtrlPosiziona(bool noresize)
{
    if (GetDlgItem(TXT_MAIN_TO) != NULL)
    {
        CRect rcClient, rcClientNow;
        GetClientRect(rcClient);

        RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);
        CPoint ptOffset(rcClientNow.left - rcClient.left, rcClientNow.top - rcClient.top);

        CWnd *pIDC_MAIN_FROM =  GetDlgItem(IDC_MAIN_FROM);    
        CWnd *pIDC_MAIN_TO =    GetDlgItem(IDC_MAIN_TO);    
        CWnd *pTXT_MAIN_FROM =	GetDlgItem(TXT_MAIN_FROM);    
        CWnd *pTXT_MAIN_TO =	GetDlgItem(TXT_MAIN_TO);    
        CWnd *pTXT_MAIN_MSG =	GetDlgItem(TXT_MAIN_MSG);   
        CWnd *pTXT_MAIN_INFO =	GetDlgItem(TXT_MAIN_INFO);   

        CRect rcIDC_MAIN_FROM; pIDC_MAIN_FROM->GetWindowRect(rcIDC_MAIN_FROM); ScreenToClient(rcIDC_MAIN_FROM);
        CRect rcIDC_MAIN_TO;   pIDC_MAIN_TO->GetWindowRect  (rcIDC_MAIN_TO);   ScreenToClient(rcIDC_MAIN_TO);
        CRect rcTXT_MAIN_FROM; pTXT_MAIN_FROM->GetWindowRect(rcTXT_MAIN_FROM); ScreenToClient(rcTXT_MAIN_FROM);
        CRect rcTXT_MAIN_TO;   pTXT_MAIN_TO->GetWindowRect(rcTXT_MAIN_TO);	   ScreenToClient(rcTXT_MAIN_TO);
        CRect rcTXT_MAIN_MSG;  pTXT_MAIN_MSG->GetWindowRect(rcTXT_MAIN_MSG);   ScreenToClient(rcTXT_MAIN_MSG);
        CRect rcTXT_MAIN_INFO; pTXT_MAIN_INFO->GetWindowRect(rcTXT_MAIN_INFO); ScreenToClient(rcTXT_MAIN_INFO);

		CString myTestoFrom, myTestoTo;
		CDC *cdcFrom=m_LblFrom->GetDC();
		cdcFrom->SelectObject(m_rpApp->font_Edit);
		m_LblFrom->GetWindowText(myTestoFrom);
		m_LblTo->GetWindowText(myTestoTo);
		CSize fsizeFrom;
		if (myTestoFrom.GetLength()>myTestoTo.GetLength())
			fsizeFrom=cdcFrom->GetOutputTextExtent(myTestoFrom);
		else
			fsizeFrom=cdcFrom->GetOutputTextExtent(myTestoTo);

		m_FldInfo->GetWindowText(myTestoFrom);
		CSize fsizeInfo=cdcFrom->GetOutputTextExtent(myTestoFrom);

		m_LblFrom->ReleaseDC(cdcFrom);

		pTXT_MAIN_INFO->MoveWindow(rcClientNow.Width() - (10 + fsizeInfo.cx),ptOffset.y, fsizeInfo.cx + 5, fsizeInfo.cy, false);
        pTXT_MAIN_INFO->GetWindowRect(rcTXT_MAIN_INFO); ScreenToClient(rcTXT_MAIN_INFO);

        pIDC_MAIN_FROM->MoveWindow(MARG_SINISTRO,ptOffset.y,fsizeFrom.cx,fsizeFrom.cy, false);
        pIDC_MAIN_FROM->GetWindowRect(rcIDC_MAIN_FROM); ScreenToClient(rcIDC_MAIN_FROM);

        pTXT_MAIN_FROM->MoveWindow(MARG_SINISTRO + 5 + fsizeFrom.cx,ptOffset.y, 
			rcTXT_MAIN_INFO.left-(MARG_SINISTRO + 5 + fsizeFrom.cx), fsizeFrom.cy, false);
        pTXT_MAIN_FROM->GetWindowRect(rcTXT_MAIN_FROM); ScreenToClient(rcTXT_MAIN_FROM);

		pIDC_MAIN_TO->MoveWindow(MARG_SINISTRO,rcIDC_MAIN_FROM.top + rcIDC_MAIN_FROM.Height() + SPAZ_VERTICALE,(rcIDC_MAIN_FROM.Width()),fsizeFrom.cy, false);
        pIDC_MAIN_TO->GetWindowRect(rcIDC_MAIN_TO); ScreenToClient(rcIDC_MAIN_TO);

		pTXT_MAIN_TO->MoveWindow(MARG_SINISTRO + 5 + fsizeFrom.cx,rcIDC_MAIN_TO.top, rcClientNow.Width()- (MARG_SINISTRO + rcIDC_MAIN_TO.Width()) - MARG_DESTRO,fsizeFrom.cy, false);
        pTXT_MAIN_TO->GetWindowRect(rcTXT_MAIN_TO); ScreenToClient(rcTXT_MAIN_TO);

        if (!noresize)
		{
		pTXT_MAIN_MSG->MoveWindow(rcIDC_MAIN_TO.left,rcTXT_MAIN_TO.top + rcTXT_MAIN_TO.Height() ,
                             (rcClientNow.Width()-rcIDC_MAIN_TO.left - 5), rcClientNow.Height() - (rcTXT_MAIN_TO.top + rcTXT_MAIN_TO.Height() - 31 ), false);
		}

		if (!noresize) RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE );

    };
}

void CRealPopupDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_MINIMIZED)
		myShowWindow(false);
	else
		CtrlPosiziona();

	CTbStDialog::OnSize(nType, cx, cy);

}

void CRealPopupDlg::OnTimer(UINT nIDEvent) 
{	// the timer pumps up main event handler for rp

	// small delay before activating toolbar; this should be improved
	// so to let messaging subsystem tell when to enable buttons
	if (delayToolbarActivate>0)
		delayToolbarActivate--;

	// TrayIcon
	static BOOL trayInit=FALSE;
	if (!trayInit)
	{

		HICON TrayIcon = AfxGetApp()->LoadIcon(IDR_TRAYICON);
		trayInit=m_TrayIcon.Create(this, WM_ICON_NOTIFY, "RealPopup", TrayIcon, IDR_MENU_TRAY);
	}

	// enable messaging drivers; on certain 9x machines,
	// messaging subsystem is not suddenly ready at startup,
	// that's why I used retries here
	static bool onInit=false;
	static int msgInit=10;
	m_hTimer = SetTimer(1,300,NULL);
	if (!onInit)
	{
		if (msgInit-->=0)
		{
			onInit=true;
			if (m_ProtocolsHandler.Init(msgInit==0, false))
				msgInit=-1;
			onInit=false;
		}
	}

	static bool init=FALSE;
	if (!init)
	{
		init=TRUE;

		// enable notification for incoming msgs
		m_ProtocolsHandler.SetNotificationEnabled(true);
		m_ProtocolsHandler.SetAwayEnabled(false);

		// warning for debug logging active
		if (m_rpApp->opz_Log==1 || m_rpApp->opz_LogRemote==1)
		{
			CString warnMsg="The debug logging is active; this feature consumes resources. Send the msg @LOG to yourself for info about how to disable it.";
			m_TrayIcon.ShowBalloon(warnMsg, m_rpApp->m_TitleString, NIIF_WARNING, 30);
		}

		// welcome and beta/expiration messages
		if (m_rpApp->opz_FirstTime==0)
		{
			m_rpApp->opz_FirstTime=1;
			HRSRC myrh=FindResource(m_rpApp->m_hInstDLL,MAKEINTRESOURCE(IDR_WELCOMEMSG),"TEXT");
			HGLOBAL mylrh=LoadResource(m_rpApp->m_hInstDLL,myrh);
			LPVOID myrptr=LockResource(mylrh);

			CMessaggio beta;
			beta.Mitt=m_rpApp->env_AutoMsgFrom;
			beta.Dest=m_rpApp->env_AutoMsgTo;
			beta.Msg.Format((char *) myrptr,m_rpApp->env_Version);
			m_ProtocolsHandler.Messaggi.Add(beta);

#ifdef BETA
			CMessaggio warnMsg;
			warnMsg.Mitt=m_rpApp->env_AutoMsgFrom;
			warnMsg.Dest=m_rpApp->env_AutoMsgTo;
			warnMsg.Msg.Format(MSG_VERSION, BETA_ANNO, BETA_MESE, BETA_GIORNO);
			warnMsg.Cmd="ONLINEWARN";

			m_ProtocolsHandler.Messaggi.Add(warnMsg);
#endif
			beta.Msg.LoadString(MSG_WELCOME);
			m_TrayIcon.ShowBalloon(beta.Msg, m_rpApp->m_TitleString, NIIF_INFO, 30);
		}
		else
		{
			if (strstr(AfxGetApp()->m_lpCmdLine,"BOOT")!=NULL)
				myShowWindow(false);
			else
			{
				m_Show=false;
				ShowWindow(m_rpApp->m_nCmdShow);
			}
		}

#ifdef BETA
		CTime t = CTime::GetCurrentTime();
		CTime expire(BETA_ANNO,BETA_MESE,BETA_GIORNO,0,0,0);
		if (t>=expire)
		{
			ErrMsg(this, MSG_BETA_OUT);
			ExitMFCApp();
		}
#endif

		// systemwide hotkey
		HotKeySet();

		// initial domain scan (served by rp NetworkBrowser routines)
		myTUSparams.m_NetTree=NULL;
		myTUSparams.servers=&this->servers;
		myTUSparams.lpnrAllocated=&this->lpnrAllocated;
		myTUSparams.m_ProtocolsHandler=&this->m_ProtocolsHandler;
		myTUSparams.m_UserDomain=NULL;
		myTUSparams.m_UsersSelected=NULL;
		myTUSparams.lpnr=NULL;
		myTUSparams.hParent=TVI_ROOT;
		myTUSparams.EnumServers=true;
		myTUSparams.netThreadAbort=&this->netThreadAbort;
		myTUSparams.ResetThreadId=false;
		myTUSparams.m_hWnd=this->m_hWnd;
		myTUSparams.m_domini=&m_ProtocolsHandler.NetworkDomains.Domains;
		myTUSparams.InviaEndMsg=true;
		myTUSparams.ValidStruct=STRUTTURAVALIDA;
		myTUSparams.inUse=false;
		netThread=AfxBeginThread(ThreadFillTree, &myTUSparams);
	}

	// autostart reminder message
	if (!m_rpApp->opz_chkAutoStartMsg && 
		!m_rpApp->opz_chkAutoStart)
	{
		if (--AutoStartWarnTimer==0)
		{
			m_rpApp->opz_chkAutoStartMsg=TRUE;

			CString msg(MAKEINTRESOURCE(MSG_AUTOSTART_MSG));
			m_ProtocolsHandler.ShowInternalMessage(&msg);
		}
	}

	// handle of commandline commands passed by another rp instance

		// a. message send
		if (g_InstNewMsg && strcmp(TIPO,"le"))
		{
			g_InstNewMsg=FALSE;
			CMessaggio cmdlineMsg;
			if (g_InstMsgInterno>0)
			{
				// internal msg body (for example MSG_CMDLINE_HELP) 
				cmdlineMsg.Mitt=m_rpApp->m_Workstation;
				cmdlineMsg.Dest=m_rpApp->env_AutoMsgTo;
				cmdlineMsg.Tutti=m_rpApp->env_AutoMsgTo;
				cmdlineMsg.Msg.Format(g_InstMsgInterno,m_rpApp->env_Version);
				m_ProtocolsHandler.Messaggi.Add(cmdlineMsg);
			}
			else
			{
				CString Dest=g_InstTo, Uno;
				cmdlineMsg.Mitt=m_rpApp->m_Workstation;
				cmdlineMsg.Msg=g_InstMsg;
				m_Users.EspandiGruppi(&cmdlineMsg.Tutti,&Dest,&cmdlineMsg.Msg);

				for (int k=0; AfxExtractSubString(Uno, cmdlineMsg.Tutti, k, (char) ','); k++)
				{
					cmdlineMsg.Dest=Uno;
					m_ProtocolsHandler.Send(&cmdlineMsg);
				}
			}
		}

		// b. show main dialog (true as default)
		if (g_InstShowWindow)
		{
			WINDOWPLACEMENT myPLACEMENT;
			GetWindowPlacement(&myPLACEMENT);

			if (myPLACEMENT.showCmd==SW_MINIMIZE)
				myShowWindow(false);
			else
				myShowWindow(true);

			g_InstShowWindow=FALSE;
			SetForegroundWindow();
		}

		// c. main dialog position and size reset
		if (g_InstComando==INST_COMANDO_POSRESET)
		{
			g_InstComando=INST_COMANDO_NESSUNO;

			CRect wndRect(0, 0, RP_WINSIZEX, RP_WINSIZEY);
			MoveWindow(wndRect);
			CenterWindow();
			GetWindowRect(wndRect);

			m_rpApp->opz_wndTop = wndRect.top;
			m_rpApp->opz_wndLeft = wndRect.left;
			m_rpApp->opz_wndHeight = wndRect.Height();
			m_rpApp->opz_wndWidth = wndRect.Width();
		}

	// main: scan for messages
	int popup;
	if (!m_ProtocolsHandler.SF_ScanInProgress)
	{

		// balloon notification
		if (m_BalloonNotifyRequest) BalloonNotify();

		if (MsgScan(&popup)>0)
		{
			m_BalloonNotifyRequest=true;
			m_rpApp->opz_MsgReceivedNum+=m_ProtocolsHandler.Messaggi.GetSize();

			if (m_ProtocolsHandler.Messaggi.GetSize()>1)
				m_TrayFlashGreen=IDR_MSGARRIVED2;
			else
				m_TrayFlashGreen=IDR_MSGARRIVED;

			if (!m_ProtocolsHandler.GetAwayEnabled())
			{
				if ( m_rpApp->opz_chkWav )
					PlaySound(m_rpApp->opz_Wav, NULL, SND_ASYNC | SND_FILENAME);

				if ( m_rpApp->opz_chkBeep )
					MessageBeep(-1);
			}

			MsgVisualizza();

			// disable toolbar for a while
			delayToolbarActivate=10;

			if (popup==1 && !m_ProtocolsHandler.GetAwayEnabled())
			{
				for (int k=0;k<senddlgAct;k++)
				{
					if (SendDlg[k]->m_hWnd!=NULL)
						break;
				}
				if (k==senddlgAct)
				{
					// popup if no active send dialogs
					WINDOWPLACEMENT myPLACEMENT;
					GetWindowPlacement(&myPLACEMENT);

					if (myPLACEMENT.showCmd==SW_MINIMIZE)
						myShowWindow(false);
					else
						myShowWindow(true);

					SetForegroundWindow();
				}
			}
			m_TrayFlash=TRAYFLASH_DELAY;
		}
	}

	// try icon flash
	static bool bIcon=false;
	if ((m_TrayFlash!=TRAYFLASH_OFF || bIcon) && m_ProtocolsHandler.GetNotificationEnabled())
	{
		if (!bIcon)
		{
			bIcon=true;
			m_TrayIcon.SetIcon(m_TrayFlashGreen);
		}
		else
		{
			bIcon=false;
			if (m_ProtocolsHandler.GetAwayEnabled())
				m_TrayIcon.SetIcon(IDR_MSGAWAY);
			else
				m_TrayIcon.SetIcon(IDR_TRAYICON);
		}

		m_TrayFlash--;
		if (m_TrayFlash<=0)
		{
			m_TrayFlash=TRAYFLASH_OFF;
			bIcon=false;

			if (m_ProtocolsHandler.Messaggi.GetSize()>0)
				m_TrayIcon.SetIcon(m_TrayFlashGreen);
			else
			{
				if (m_ProtocolsHandler.GetAwayEnabled())
					m_TrayIcon.SetIcon(IDR_MSGAWAY);
				else
					m_TrayIcon.SetIcon(IDR_TRAYICON);
			}
		}
	}

	// handle for 'set away when screensaver runs'
	static int ScreenAwayDelay=15;
	if (m_rpApp->opz_chkScrAway && ScreenAwayDelay--==0)
	{
		ScreenAwayDelay=15;
		if (IsScreenSaverRunning())
		{
			if (!m_ProtocolsHandler.GetAwayEnabled())
			{
				m_ProtocolsHandler.SetAwayEnabled(true);
				m_ProtocolsHandler.SetNotificationEnabled(true);
				m_RedirectEnabled=true;
				m_TrayIcon.SetIcon(IDR_MSGAWAY);
				m_AwayByScreenSaver=true;
			}
		}
		else
		{
			if (m_AwayByScreenSaver)
			{
				m_AwayByScreenSaver=false;
				m_ProtocolsHandler.SetAwayEnabled(false);

				if (m_ProtocolsHandler.Messaggi.GetSize()>0)
					m_TrayIcon.SetIcon(m_TrayFlashGreen);
				else
					m_TrayIcon.SetIcon(IDR_TRAYICON);
			}
		}
	}

	CDialog::OnTimer(nIDEvent);

}

LRESULT CRealPopupDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{

	if (m_OptionsOpen)
		return 0;

	if (lParam==0x0405)
		OnTrayMenu(ID_TRAY_OPEN);

	if ( m_rpApp->opz_chkLeftTray )
	{
		if (lParam==517)
			return 0;

		if (lParam==514)
			lParam=517;
	}

	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CRealPopupDlg::OnTrayMenu(UINT nID) 
{

	switch (nID)
	{
	case ID_TRAY_OPEN:
		{
			m_TrayFlashGreen=IDR_MSGARRIVED;

			myShowWindow(true);
			m_TrayFlash=TRAYFLASH_UPDREQ;
			break;
		}

	case ID_TRAY_MINIMIZE:
		{
			myShowWindow(false);
			break;
		}
	case ID_TRAY_DISABLE:
		{
			if (m_ProtocolsHandler.GetNotificationEnabled())
			{
				m_ProtocolsHandler.SetNotificationEnabled(false);
				m_ProtocolsHandler.SetAwayEnabled(false);
				m_RedirectEnabled=true;
				m_TrayIcon.SetIcon(IDR_MSGDISABLED);
			}
			else
			{
				m_ProtocolsHandler.SetNotificationEnabled(true);
				if (m_ProtocolsHandler.Messaggi.GetSize()>0)
					m_TrayIcon.SetIcon(m_TrayFlashGreen);
				else
					m_TrayIcon.SetIcon(IDR_TRAYICON);
			}

			break;
		}
	case ID_TRAY_AWAY:
		{
			if (m_ProtocolsHandler.GetAwayEnabled())
			{
				m_ProtocolsHandler.SetAwayEnabled(false);
				if (m_ProtocolsHandler.Messaggi.GetSize()>0)
					m_TrayIcon.SetIcon(m_TrayFlashGreen);
				else
					m_TrayIcon.SetIcon(IDR_TRAYICON);
			}
			else
			{
				m_ProtocolsHandler.SetAwayEnabled(true);
				m_ProtocolsHandler.SetNotificationEnabled(true);
				m_RedirectEnabled=true;
				m_TrayIcon.SetIcon(IDR_MSGAWAY);
			}

			break;
		}
	case ID_TRAY_REDIRECT:
		{
			if (m_RedirectEnabled)
			{
				m_ProtocolsHandler.SetAwayEnabled(false);
				m_ProtocolsHandler.SetNotificationEnabled(true);
				m_TrayIcon.SetIcon(IDR_TRAYICON);
			}
	
			m_RedirectEnabled = !m_RedirectEnabled;

				
			break;
		}
	case ID_TRAY_EXIT:
		{
			if (!m_rpApp->opz_chkAutoStartWarn && 
				!m_rpApp->opz_chkAutoStart)
			{
				m_rpApp->opz_chkAutoStartWarn=TRUE;

				CString msg;
				CString caption;
				msg.LoadString(MSG_AUTOSTART_WARN);
				caption.LoadString(MSG_DIALOG_CAPTION);
				if (MessageBox(msg,caption,MB_ICONQUESTION+MB_YESNO)==IDYES)
				{
					m_rpApp->opz_chkAutoStart=TRUE;
					SetAutostart(TRUE);
				}
			}

			if (m_rpApp->opz_chkWarn)
			{
				CString msg;
				CString caption;
				msg.LoadString(MSG_WARNONEXIT);
				caption.LoadString(MSG_DIALOG_CAPTION);
				if (MessageBox(msg,caption,MB_ICONQUESTION+MB_YESNO)==IDNO)
					break;
			}
			ExitMFCApp();
			break;
		}
	}
	OnSetMessageString(IDStatusIdleMessage);
}

void CRealPopupDlg::OnUpdateTrayMenu(CCmdUI* pCmdUI) 
{
	WINDOWPLACEMENT wp;

	GetWindowPlacement(&wp);
	switch (pCmdUI->m_nID)
	{
	case ID_TRAY_MINIMIZE:
		{
			pCmdUI->Enable(wp.showCmd != SW_SHOWMINIMIZED);
			break;
		}
	case ID_TRAY_DISABLE:
		{
			pCmdUI->SetCheck(!m_ProtocolsHandler.GetNotificationEnabled());
			break;
		}
	case ID_TRAY_AWAY:
		{
			pCmdUI->SetCheck(m_ProtocolsHandler.GetAwayEnabled());
			break;
		}
	case ID_TRAY_REDIRECT:
		{
			pCmdUI->SetCheck(!m_RedirectEnabled);
			break;
		}
	}
}

void CRealPopupDlg::OnBtnTlbOther(UINT nID) 
{

	switch (nID)
	{

	case ID_BTN_HELP:
		{
			m_rpApp->ShowHelp(this, "");
			
			break;
		}

	case ID_BTN_WELCOMEMSG:
		{
			HRSRC myrh=FindResource(m_rpApp->m_hInstDLL,MAKEINTRESOURCE(IDR_WELCOMEMSG),"TEXT");
			HGLOBAL mylrh=LoadResource(m_rpApp->m_hInstDLL,myrh);
			LPVOID myrptr=LockResource(mylrh);

			CMessaggio beta;
			beta.Mitt=m_rpApp->env_AutoMsgFrom;
			beta.Dest=m_rpApp->env_AutoMsgTo;
			beta.Msg.Format((char *) myrptr,m_rpApp->env_Version);
			beta.RiempiCampiData();
			m_ProtocolsHandler.Messaggi.Add(beta);

#ifdef BETA
			CMessaggio warnMsg;
			warnMsg.Mitt=m_rpApp->env_AutoMsgFrom;
			warnMsg.Dest=m_rpApp->env_AutoMsgTo;
			warnMsg.Msg.Format(MSG_VERSION, BETA_ANNO, BETA_MESE, BETA_GIORNO);
			warnMsg.Cmd="ONLINEWARN";
			warnMsg.RiempiCampiData();

			m_ProtocolsHandler.Messaggi.Add(warnMsg);
#endif

			break;
		}

	case ID_BTN_ABOUT:
		{
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
			break;
		}

	case ID_BTN_OPTIONS:
		{
			Options();
			break;
		}

	}
}

void CRealPopupDlg::Options()
{

	CPPEnvironment ppEnvironment;
	CPPLanguages ppLanguages;
	CPPMessages ppMessages;
	CPPUsers ppUsers;

	ppMessages.m_psp.dwFlags |= PSH_NOAPPLYNOW;
	ppMessages.m_chkBeep=m_rpApp->opz_chkBeep;
	ppMessages.m_chkMinimize=m_rpApp->opz_chkMinimize;
	ppMessages.m_chkMinimizeRepl=m_rpApp->opz_chkMinimizeRepl;
	ppMessages.m_chkDelOriginal=m_rpApp->opz_chkDelOriginal;
	ppMessages.m_chkBalloonOnMsgFails=m_rpApp->opz_chkBalloonOnMsgDeliveryIssues;
	ppMessages.m_chkLastMsg=m_rpApp->opz_chkLastMsg;
	ppMessages.m_chkDblClick=m_rpApp->opz_chkDblClick;
	ppMessages.m_chkWav=m_rpApp->opz_chkWav;
	ppMessages.m_Wav=m_rpApp->opz_Wav;
	ppMessages.m_chkSendPcName=m_rpApp->opz_chkSendPcName;
	ppMessages.m_chkReplyTop=m_rpApp->opz_chkReplyTop;
	ppMessages.m_chkCrSend=m_rpApp->opz_chkCrSend;
	ppMessages.m_chkQuote=m_rpApp->opz_chkQuote;
	ppMessages.m_chkDisableDel=m_rpApp->opz_chkDisableDel;

	ppEnvironment.m_ResSet=-1;
	ppEnvironment.m_psp.dwFlags |= PSH_NOAPPLYNOW;
	ppEnvironment.m_chkScrAway=m_rpApp->opz_chkScrAway;
	ppEnvironment.m_chkTop=m_rpApp->opz_chkTop;
	ppEnvironment.m_chkWarn=m_rpApp->opz_chkWarn;
	ppEnvironment.m_chkWarnDel=m_rpApp->opz_chkWarnDel;
	ppEnvironment.m_chkWarnDomain=m_rpApp->opz_chkWarnDomain;
	ppEnvironment.m_chkWarnRPProtocolNotActive=m_rpApp->opz_chkWarnRPProtocolNotActive;
	ppEnvironment.m_chkNBDetermineFavorites=m_rpApp->opz_chkNBDetermineFavorites;
	ppEnvironment.m_chkAutoStart=m_rpApp->opz_chkAutoStart;
	ppEnvironment.m_chkLeftTray=m_rpApp->opz_chkLeftTray;
	ppEnvironment.m_Msn=m_rpApp->opz_Msn;
	ppEnvironment.opz_HotKey=m_rpApp->opz_HotKey;
	ppEnvironment.opz_HotKeymd=m_rpApp->opz_HotKeymd;

	ppLanguages.m_ResSet=-1;
	ppLanguages.m_psp.dwFlags |= PSH_NOAPPLYNOW;

	ppUsers.m_psp.dwFlags |= PSH_NOAPPLYNOW;
	ppUsers.m_UsersData = &m_Users;
	ppUsers.m_ProtocolsHandler = &m_ProtocolsHandler;

	CString caption;
	caption.LoadString(OPTIONS_CAPTION);
	CFntPropertySheet mySheet(caption);

	mySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	mySheet.m_psh.dwFlags |= PSH_USEPAGELANG;

	mySheet.AddPage(&ppEnvironment);
	mySheet.AddPage(&ppLanguages);
	mySheet.AddPage(&ppMessages);
	mySheet.AddPage(&ppUsers);

	m_OptionsOpen=true;
	if (mySheet.DoModal()==IDOK)
	{
		SetFonts();		
		CtrlPosiziona();

		m_rpApp->opz_chkBeep=ppMessages.m_chkBeep;
		m_rpApp->opz_chkMinimize=ppMessages.m_chkMinimize;
		m_rpApp->opz_chkMinimizeRepl=ppMessages.m_chkMinimizeRepl;
		m_rpApp->opz_chkDelOriginal=ppMessages.m_chkDelOriginal;
		m_rpApp->opz_chkBalloonOnMsgDeliveryIssues=ppMessages.m_chkBalloonOnMsgFails;
		m_rpApp->opz_chkLastMsg=ppMessages.m_chkLastMsg;
		m_rpApp->opz_chkDblClick=ppMessages.m_chkDblClick;
		m_rpApp->opz_Wav=ppMessages.m_Wav;
		m_rpApp->opz_chkWav=ppMessages.m_chkWav;
		m_rpApp->opz_chkSendPcName=ppMessages.m_chkSendPcName;
		m_rpApp->opz_chkReplyTop=ppMessages.m_chkReplyTop;
		m_rpApp->opz_chkCrSend=ppMessages.m_chkCrSend;
		m_rpApp->opz_chkQuote=ppMessages.m_chkQuote;
		m_rpApp->opz_chkDisableDel=ppMessages.m_chkDisableDel;

		m_rpApp->opz_chkTop=ppEnvironment.m_chkTop;
		m_rpApp->opz_chkWarn=ppEnvironment.m_chkWarn;
		m_rpApp->opz_chkScrAway=ppEnvironment.m_chkScrAway;
		m_rpApp->opz_chkWarnDel=ppEnvironment.m_chkWarnDel;
		m_rpApp->opz_chkWarnDomain=ppEnvironment.m_chkWarnDomain;
		m_rpApp->opz_chkWarnRPProtocolNotActive=ppEnvironment.m_chkWarnRPProtocolNotActive;
		m_rpApp->opz_chkNBDetermineFavorites=ppEnvironment.m_chkNBDetermineFavorites;
		m_rpApp->opz_chkAutoStart=ppEnvironment.m_chkAutoStart;
		m_rpApp->opz_chkLeftTray=ppEnvironment.m_chkLeftTray;
		m_rpApp->opz_Msn=ppEnvironment.m_Msn;
		m_rpApp->opz_HotKey=ppEnvironment.opz_HotKey;
		m_rpApp->opz_HotKeymd=ppEnvironment.opz_HotKeymd;

		if (ppEnvironment.m_chkTop)
			SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		else
			SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

		if (ppLanguages.m_ResSet!=-1)
			m_rpApp->m_ResActual=m_rpApp->m_ResDLLs.GetAt(ppLanguages.m_ResSet);

		m_Users.Save();
		HotKeySet();
	}
	m_OptionsOpen=false;

}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString sAboutMsg, sBuildMsg;
	m_rpApp=(CRealPopupApp *)AfxGetApp();

	sAboutMsg.Format(MSG_ABOUT, m_rpApp->opz_MsgReceivedNum, m_rpApp->opz_MsgSentNum );
	sBuildMsg.Format("RealPopup %s %s build %s.", TIPO, m_rpApp->env_Version, BUILD);

	CRect rec(8,96,286,207);		
	int h=m_AboutMsg.Create(ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY | ES_WANTRETURN |
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL, rec, this, IDC_RICHEDIT_ABOUT);
	m_AboutMsg.SetBackgroundColor(FALSE, GetSysColor(COLOR_BTNFACE));
	m_AboutMsg.m_Browser=&(((CRealPopupApp *) AfxGetApp())->m_Browser);
	m_AboutMsg.SendMessage(EM_SETTEXTMODE, TM_RICHTEXT | TM_MULTILEVELUNDO | TM_SINGLECODEPAGE ,0);

	GetDlgItem(IDC_TXT_ABOUTVERSION)->SetWindowText(sBuildMsg);

	CHARFORMAT cf;
	cf.bCharSet=m_rpApp->font_lfSystem.lfCharSet;
	cf.dwMask=CFM_CHARSET;
	m_AboutMsg.SetDefaultCharFormat(cf);
	m_AboutMsg.SetSelectionCharFormat(cf);
	m_AboutMsg.SetWordCharFormat(cf);
	m_AboutMsg.SetWindowText(sAboutMsg);

	m_AboutMsg.QuoteHandling(false);
	
	return TRUE;
}

void CRealPopupDlg::OnOK()
{

	// rp doesn't handle 'common' OK...

}

void CRealPopupDlg::OnCancel()
{
	// ...and the same for Cancel! :)

}

int CRealPopupDlg::MsgScan(int *popup)
{	// scans for messages (highest level, activates the CRPProtocolsHandler class

	SUser *profile;
	int primoMsgNuovo = m_ProtocolsHandler.Scan();
	int NuoviMessaggi=0;
	static int lastNumMessaggi=0;

	if (primoMsgNuovo >= 0)
	{
		for (int k=primoMsgNuovo; k<m_ProtocolsHandler.Messaggi.GetSize(); k++)
		{
			if ( m_ProtocolsHandler.Messaggi[k].Mitt!=m_rpApp->env_AutoMsgFrom )
			{
				profile=m_Users.GetProfile(&m_ProtocolsHandler.Messaggi[k].Mitt,&m_ProtocolsHandler.Messaggi[k].Msg);
				if (profile!=NULL)
				{
					if (profile->chkIgnore==1 && m_RedirectEnabled)
					{
						m_ProtocolsHandler.Messaggi.RemoveAt(k);
						k--;
					}
					*popup=profile->chkPopUp;
				}
			}
		}
	}

	NuoviMessaggi=m_ProtocolsHandler.Messaggi.GetSize()-lastNumMessaggi;
	lastNumMessaggi = m_ProtocolsHandler.Messaggi.GetSize();

	return NuoviMessaggi;
}

void CRealPopupDlg::MsgToolTipUpd()
{

	int  msgs=m_ProtocolsHandler.Messaggi.GetSize();
	CString ttmsg;

	if (msgs==0)
	{
		ttmsg.LoadString(TOOLTIP_IDLE);
		if (m_ProtocolsHandler.GetAwayEnabled())
			m_TrayIcon.SetIcon(IDR_MSGAWAY);
		else
			m_TrayIcon.SetIcon(IDR_TRAYICON);
	}
	else
	{
		m_TrayIcon.SetIcon(IDR_MSGARRIVED);
		if (msgs>1)
			ttmsg.Format(TOOLTIP_MSGS,msgs);
		else
			ttmsg.Format(TOOLTIP_MSG,msgs);
	}

	m_TrayIcon.SetTooltipText(LPCTSTR(ttmsg));

}

void CRealPopupDlg::OnBtnNetwork() 
{

	CString myUsers;

	CRPNetworkBrowser dlg;
	dlg.m_ProtocolsHandler=&m_ProtocolsHandler;
	dlg.m_UsersSelectedString=&myUsers;
	dlg.m_UserDomain=&m_rpApp->opz_LastDomain;

	CRect ownRect;
	GetWindowRect(ownRect);
	ownRect.DeflateRect(20,20);
	dlg.m_parentRect=ownRect;
	
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK && strcmp(TIPO,"le"))
	{
		if (myUsers.GetLength()>0)
		{
			CSendDlg *dlg;
			int k=0;

			if (senddlgAct==MAX_SENDDLG) return;
			dlg=new CSendDlg();

			for (k=0;k<senddlgAct;k++)
			{
				if (SendDlg[k]->m_hWnd==NULL)
				{
					delete(SendDlg[k]);
					break;
				}
			}
			if (k==senddlgAct)
				k=senddlgAct++;
			SendDlg[k]=dlg;
			SendDlgLast=dlg;

			dlg->m_ProtocolsHandler = &m_ProtocolsHandler;
			dlg->m_Users = &m_Users;

			dlg->PredefTo = myUsers;
			dlg->m_forward = FALSE;

			dlg->Create(IDD_DLG_SENDMESSAGE,this);
			dlg->m_RealPopupDlg=this;
			dlg->MsgDaEliminare=-1;
			dlg->TipoSend=ID_BTN_NEW;
			dlg->ShowWindow(SW_SHOW);

			dlg->m_MsgEdit.SetFocus();
		}
	}
}


void CRealPopupDlg::OnBtnCopy() 
{
	long nStart, nEnd;

	m_MsgEdit.GetSel(nStart, nEnd);
	if (nStart==nEnd)
	{
		m_MsgEdit.HideSelection(true, true);
		m_MsgEdit.SetSel(0,-1);
		m_MsgEdit.Copy();
		m_MsgEdit.SetSel(0,0);
		m_MsgEdit.HideSelection(false, true);
	}
	else
		m_MsgEdit.Copy();
	
}


void CRealPopupDlg::MsgSave(CMessaggio *msg)
{
	CString caption, filter;
	caption.LoadString(IDS_DDXFILE_TT_CAPTION);
	filter.LoadString(IDS_DDXFILE_FILTER_TXT);

	CGetFileNameDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, filter, GetParent());
	dlg.m_ofn.lpstrTitle = caption;

	if (dlg.DoModal() == IDOK)
	{

		CStdioFile myLog;
		CString riga, ver;

		caption=dlg.GetPathName();
		if (caption.Left(4).CompareNoCase("*.txt")!=0)
			caption+=".txt";

		if (!myLog.Open(caption, CFile::modeCreate |
			CFile::modeWrite | CFile::typeText ) )
		{
			ErrMsg(this, MSG_ERR_NOTCREATABLE);
			return;
		}

		CString fmtData, fmtDaA;
		fmtData.LoadString(FMT_LOG_DATE);
		fmtDaA.LoadString(FMT_LOG_FROMTO);

		riga.Format(MSG_SAVEHEAD,m_rpApp->env_Version);
		myLog.WriteString(riga);

		CTime t = CTime::GetCurrentTime();
		ver = t.Format(fmtData);

		riga.Format(fmtDaA, msg->Mitt, msg->Dest, ver);
		myLog.SeekToEnd();
		myLog.WriteString(riga);
		myLog.WriteString(msg->Msg);

		myLog.Close();
	}
}

BOOL CRealPopupDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) 
	{ 
		if (m_hAccelTable != NULL 
		&& ::TranslateAccelerator(GetSafeHwnd(), m_hAccelTable, pMsg)) 
			return TRUE; 
	} 

	if (pMsg->message==WM_USER+WM_NETTHREADENDED)
	{	// the NetworkBrowser routines has ended network scan

		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "RealPopupDlg.PreTranslateMessage", "WM_NETTHREADENDED received\n");

		// I experimented with rp users that performing network scans
		// in peer-to-peer networks against other domains gives
		// the ability to 'see' remote workstations, that's why
		// I update naming resolution table here
		m_ProtocolsHandler.m_NameResolutionRequested=true;

		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "RealPopupDlg.PreTranslateMessage", "NameResolutionPing requested\n");
	}

	return CTbStDialog::PreTranslateMessage(pMsg);
}

void CRealPopupDlg::SetFonts()
{
	if (m_rpApp->font_RichEditchanged)
	{
		CFont *font_RichEdit=&m_rpApp->font_RichEdit;
		char *bufText = (char *) malloc(m_MsgEdit.GetTextLength()+1);
		m_MsgEdit.GetWindowText(bufText,m_MsgEdit.GetTextLength()+1);
		
		m_MsgEdit.HideSelection(true,true);
		m_MsgEdit.SetSel(0,-1);
		m_MsgEdit.SetFont(font_RichEdit,true);
		m_MsgEdit.ReplaceSel(bufText);
		m_MsgEdit.SetSel(0,0);
		free(bufText);
		m_MsgEdit.HideSelection(false,true);
		m_MsgEdit.SendMessage(WM_SIZE,0,0);
/*		m_MsgEdit.SetFont(font_RichEdit,true);

		CHARFORMAT cf;
		cf.bCharSet=m_rpApp->font_lfSystem.lfCharSet;
		cf.dwMask=CFM_CHARSET;
		m_MsgEdit.SetDefaultCharFormat(cf);
		m_MsgEdit.SetSelectionCharFormat(cf);
		m_MsgEdit.SetWordCharFormat(cf);*/
	
	}
	if (m_rpApp->font_Editchanged)
	{
		CFont *font_Edit=&m_rpApp->font_Edit;
/*
#ifdef USASETFONT
		CWnd *mywnd=GetTopWindow();
		while (mywnd!=NULL)
		{
			mywnd->SetFont(&m_rpApp->font_System,true);
			mywnd=mywnd->GetNextWindow(GW_HWNDNEXT);
		}
		/*AfxSendMessage(
		::SendMessage(m_LblFrom->m_hWnd,WM_SETFONT,(WPARAM)HFONT(font_Edit),TRUE);
		::SendMessage(m_LblTo->m_hWnd,WM_SETFONT,(WPARAM)HFONT(font_Edit),TRUE);
		::SendMessage(m_FldFrom->m_hWnd,WM_SETFONT,(WPARAM)(HFONT)font_Edit,TRUE);
		::SendMessage(m_FldTo->m_hWnd,WM_SETFONT,(WPARAM)(HFONT)font_Edit,TRUE);
		::SendMessage(m_FldInfo->m_hWnd,WM_SETFONT,(WPARAM)(HFONT)font_Edit,TRUE);*/
//#else
		m_LblFrom->SetFont(font_Edit,true);
		m_LblTo->SetFont(font_Edit,true);
		m_FldFrom->SetFont(font_Edit,true);
		m_FldTo->SetFont(font_Edit,true);
		m_FldInfo->SetFont(font_Edit,true);
//#endif
	}

}

void CRealPopupDlg::HotKeySet()
{
	UINT matro = m_rpApp->opz_HotKeymd;
	matro=( ((matro & HOTKEYF_ALT)/4) + (matro & HOTKEYF_CONTROL) + ((matro & HOTKEYF_SHIFT)*4) );

	UnregisterHotKey(this->m_hWnd, m_rpApp->hkRealPopup);

	if (m_rpApp->opz_HotKey!=0)
	{
		if (RegisterHotKey((HWND)this->m_hWnd,m_rpApp->hkRealPopup,	matro,m_rpApp->opz_HotKey) == 0)
		{
			CString stHead, stMsg, stBalloonMsg;
			stHead.LoadString(MSG_ERR_HOTKEYHEAD);
			stMsg.LoadString(MSG_ERR_HOTKEY);
			stBalloonMsg.LoadString(MSG_ERR_HOTKEY);
			m_ProtocolsHandler.ShowInternalMessage(&stMsg, &stHead, NIIF_WARNING, &stBalloonMsg);
		}
	}
}

LRESULT CRealPopupDlg::OnPowerBroadcast(WPARAM wParam, LPARAM lParam)
{
	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update),
		"CRPProtocolsHandler::OnPowerBroadcast", "WM_POWERBROADCAST detected, type %x\n", wParam);

	if (wParam==PBT_APMSUSPEND || wParam==PBT_APMRESUMECRITICAL)
		m_ProtocolsHandler.Close(true);

	if (wParam==PBT_APMRESUMEAUTOMATIC || wParam==PBT_APMRESUMECRITICAL || wParam==PBT_APMRESUMESUSPEND)
		m_ProtocolsHandler.Init(true, true);

	return 0;
}

LRESULT CRealPopupDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	myShowWindow(true);
	m_TrayFlash=TRAYFLASH_OFF;
	SetForegroundWindow();

	return 0;
}


void CRealPopupDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	if (!m_Show)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}

void CRealPopupDlg::myShowWindow(bool mode)
{

	m_Show=mode;
	if (mode)
	{
		ShowWindow(SW_SHOW);
		ShowWindow(SW_RESTORE);
	}
	else
		ShowWindow(SW_HIDE);

}

void CRealPopupDlg::BalloonNotify(void)
{
	if((m_BalloonNotifyDelay--)<=0)
	{
		int pos=0, numMsg=m_ProtocolsHandler.Messaggi.GetSize();
		bool displayBalloon=false;
		m_BalloonNotifyRequest=true;
		while (pos<numMsg)
		{
			if (!m_ProtocolsHandler.Messaggi.GetAt(pos).Notified)
			{
				SUser *myUser=m_Users.GetProfile(&m_ProtocolsHandler.Messaggi.GetAt(pos).Mitt, &m_ProtocolsHandler.Messaggi.GetAt(pos).Msg);
				m_ProtocolsHandler.Messaggi[pos].Notified=true;

				// conditions not to show balloon notifications are coded
				// in different if() statements for simplicity

				if (m_ProtocolsHandler.Messaggi[pos].Mitt.CompareNoCase(m_rpApp->env_AutoMsgFrom)==0)
					m_BalloonNotifyRequest=false;

				if (IsWindowVisible() || m_Show)
					m_BalloonNotifyRequest=false;

				if (myUser!=NULL)
				{
					if (myUser->chkBalloon==0 && m_ProtocolsHandler.Messaggi[pos].Cmd!="NOTRECEIVED"
											  && m_ProtocolsHandler.Messaggi[pos].Cmd!="NOTREAD")
						m_BalloonNotifyRequest=false;
				}

				if ( (m_ProtocolsHandler.Messaggi[pos].Cmd=="NOTREAD" || m_ProtocolsHandler.Messaggi[pos].Cmd=="NOTRECEIVED")
					&& !m_rpApp->opz_chkBalloonOnMsgDeliveryIssues )
						m_BalloonNotifyRequest=false;

				if (m_BalloonNotifyRequest)
				{
					CString stHead, stMsg;
					bool isNotReceivedNotification=(m_ProtocolsHandler.Messaggi[pos].Cmd=="NOTRECEIVED");
					bool isNotReadNotification=(m_ProtocolsHandler.Messaggi[pos].Cmd=="NOTREAD");
					
					m_BalloonNotifyDelay=BALLOONNOTIFY_MININTERVAL;
					m_BalloonNotifyRequest=true;

					if (isNotReceivedNotification || isNotReadNotification)
						stHead.Format(MSG_BALLOON_ERRMSG, m_ProtocolsHandler.Messaggi[pos].Mitt);
					else
						stHead.Format(MSG_BALLOON_NEWMSG, m_ProtocolsHandler.Messaggi[pos].Mitt);

					if (numMsg>1)
						stHead.AppendFormat(MSG_MESSAGESWAITING, numMsg);
											
					if (myUser->chkBalloonPreview && !isNotReceivedNotification)
					{
						// balloon preview message formatting
						CString stPreview=m_ProtocolsHandler.Messaggi[pos].Msg;//.Left(BALLOONNOTIFY_PREVIEWCHARS);
						int lpos=0, fpos=0, cutchars=0, rows=-1;
						bool found=true;
						
						// set all cr and lf combinations to lf only
						cutchars+=Normalizza0D(&stPreview);

						// remove quoted text
						while (found)
						{
							found=false;
							fpos=stPreview.Find(">");
							if (fpos!=-1)
							{
								lpos=stPreview.Find("\r",fpos+1);
								if (lpos!=-1)
								{
									cutchars+=lpos-fpos;
									stPreview.Replace(stPreview.Mid(fpos, lpos-fpos+1), "");
									found=true;
								}
							}
						}

						// take first 4 rows (if any)
						lpos=0; fpos=0;
						while (fpos!=-1 && ++rows<4)
						{
							fpos=stPreview.Find("\r",lpos+1);
							if (fpos>0) lpos=fpos;
						}
						if (rows==4)
							stPreview=stPreview.Left(lpos);

						//remove leading CRs
						while (stPreview.Right(1)=="\r")
						{
							cutchars++;
							stPreview=stPreview.Left(stPreview.GetLength()-1);
						}

						stPreview=stPreview.Left(BALLOONNOTIFY_PREVIEWCHARS);

						// append preview and ... if msg was truncated
						stMsg.Append(stPreview);
						if (stPreview.GetLength()<(m_ProtocolsHandler.Messaggi[pos].Msg.GetLength()-cutchars) || rows==4)
							stMsg.Append("...");

						m_ProtocolsHandler.ShowInternalMessage(&stMsg, &stHead, (isNotReceivedNotification?NIIF_WARNING:NIIF_INFO));
					}
					else
					{
						stMsg.Append(".");
						stHead.Append(stMsg);
						m_ProtocolsHandler.ShowInternalMessage(&stHead, NULL, (isNotReceivedNotification?NIIF_WARNING:NIIF_INFO));
					}
					break;
				}
			}
			pos++;
		}
	}
}

void CRealPopupDlg::OnUpdateStatusPanes(CCmdUI* pCmdUI)
{
	static bool bWPProtocolUpdated=false, bRPProtocolUpdated=false;
	bool bIsPanelUpdate=false, *bProtocolUpdated;

	if(MM_STATUS_PANE_UPDATE == (int)pCmdUI->m_pMenu)
		bIsPanelUpdate = true;

	MMStatusCmdUI* pStatusUI = (MMStatusCmdUI*)pCmdUI;

	if(bIsPanelUpdate)
	{
		bProtocolUpdated=(pStatusUI->m_nID==STATUS_WPPROTOCOLENABLED?&bWPProtocolUpdated:&bRPProtocolUpdated);
		bool bEnabled=(pStatusUI->m_nID==STATUS_WPPROTOCOLENABLED?m_ProtocolsHandler.IsWinPopupProtocolEnabled:m_ProtocolsHandler.IsRealPopupProtocolEnabled);
		
		if (bEnabled!=*bProtocolUpdated)
		{
			CString text;
			int nPanel=(pStatusUI->m_nID==STATUS_WPPROTOCOLENABLED?1:2);

			if (bEnabled)
			{
				pStatusUI->SetIcon(StatusBarPanels.EnabledIcons[nPanel]);
				text.LoadString(StatusBarPanels.EnabledToolTips[nPanel]);
				pStatusUI->SetToolTipText(text);
			}
			else
			{
				pStatusUI->SetIcon(StatusBarPanels.DisabledIcons[nPanel]);
				text.LoadString(StatusBarPanels.DisabledToolTips[nPanel]);
				pStatusUI->SetToolTipText(text);
			}
			pStatusUI->SetText("");
			*bProtocolUpdated=bEnabled;
		}
	}
	else
	{
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update),
			"CRPProtocolsHandler::OnUpdateStatusPanes", "double click on lower right icons detected\n");

		m_ProtocolsHandler.Init(true, true);
		pCmdUI->ContinueRouting();
	}
}
