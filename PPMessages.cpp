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
#include "realpopup.h"
#include "PPMessages.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPPMessages::CPPMessages() : CPropertyPage(CPPMessages::IDD)
{
	//{{AFX_DATA_INIT(CPPMessages)
	m_chkBeep = FALSE;
	m_chkMinimize = FALSE;
	m_chkDelOriginal = FALSE;
	m_chkBalloonOnMsgFails = FALSE;
	m_chkLastMsg = FALSE;
	m_chkDblClick = FALSE;
	m_chkWav = FALSE;
	m_Wav = _T("");
	m_chkMinimizeRepl = FALSE;
	m_chkSendPcName = FALSE;
	m_chkReplyTop = FALSE;
	m_chkCrSend = FALSE;
	m_chkQuote = FALSE;
	m_chkDisableDel = FALSE;
	//}}AFX_DATA_INIT
}

void CPPMessages::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPMessages)
	DDX_Control(pDX, IDC_PLAY, m_cmdPlay);
	DDX_Check(pDX, IDC_CHK_BEEP, m_chkBeep);
	DDX_Check(pDX, IDC_CHK_MINIMIZE, m_chkMinimize);
	DDX_Check(pDX, IDC_CHK_DELORIGINAL, m_chkDelOriginal);
	DDX_Check(pDX, IDC_CHK_BALLOONONMSGFAILS, m_chkBalloonOnMsgFails);
	DDX_Check(pDX, IDC_CHK_LASTMSG, m_chkLastMsg);
	DDX_Check(pDX, IDC_CHK_DBLCLICK, m_chkDblClick);
	DDX_Check(pDX, IDC_CHK_WAV, m_chkWav);
	DDX_Text(pDX, IDC_TXT_WAV, m_Wav);
	DDX_Check(pDX, IDC_CHK_MINIMIZEREPL, m_chkMinimizeRepl);
	DDX_Check(pDX, IDC_CHK_SENDPCNAME, m_chkSendPcName);
	DDX_Check(pDX, IDC_CHK_REPLYTOP, m_chkReplyTop);
	DDX_Check(pDX, IDC_CHK_QUOTE, m_chkQuote);
	DDX_Check(pDX, IDC_CHK_CRSEND, m_chkCrSend);
	DDX_Check(pDX, IDC_CHK_DISABLEDEL, m_chkDisableDel);
	//}}AFX_DATA_MAP
	DDX_FilenameControl(pDX, IDC_TXT_WAV, m_ctrlGetFilename, GF_FILEMUSTEXIST, MAKEINTRESOURCE(IDS_DDXFILE_TT_CAPTION), MAKEINTRESOURCE(IDS_DDXFILE_FILTER_WAV));
}

BEGIN_MESSAGE_MAP(CPPMessages, CDialog)
	//{{AFX_MSG_MAP(CPPMessages)
	ON_BN_CLICKED(IDC_CHK_WAV, OnChkWav)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()

BOOL CPPMessages::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_rpApp=(CRealPopupApp *)AfxGetApp();

	GetDlgItem(IDC_TXT_WAV)->EnableWindow(m_chkWav);	
	GetDlgItem(IDC_PLAY)->EnableWindow(m_chkWav);	

	m_cmdPlay.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_PLAY)));
	m_cmdPlay.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);

	UpdateData();

#ifdef USASETFONT
	CWnd *mywnd=GetTopWindow();
	while (mywnd!=NULL)
	{
		mywnd->SetFont(&m_rpApp->font_System,true);
		mywnd=mywnd->GetNextWindow(GW_HWNDNEXT);
	}
#endif
	
	return TRUE;  
}

void CPPMessages::OnChkWav() 
{
	UpdateData();
	GetDlgItem(IDC_TXT_WAV)->EnableWindow(m_chkWav);
	GetDlgItem(IDC_PLAY)->EnableWindow(m_chkWav);
}

void CPPMessages::OnPlay() 
{
	CString myFile;
	GetDlgItem(IDC_TXT_WAV)->GetWindowText(myFile);
	PlaySound( myFile, NULL, SND_ASYNC | SND_FILENAME);
	
}

void CPPMessages::OnHelp()
{
	m_rpApp->ShowHelp(this, "#message");
}
