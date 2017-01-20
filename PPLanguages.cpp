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
#include "senddlg.h"
#include "realpopupdlg.h"
#include "PPLanguages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPPLanguages, CPropertyPage)

CPPLanguages::CPPLanguages() : CPropertyPage(CPPLanguages::IDD)
{
	//{{AFX_DATA_INIT(CPPLanguages)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPLanguages::~CPPLanguages()
{
}

void CPPLanguages::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPLanguages)
	DDX_Control(pDX, IDC_LSTLANGUAGE, m_lstLanguage);
	DDX_Control(pDX, IDC_BTN_FONTEDIT, m_cmdFontEdit);
	DDX_Control(pDX, IDC_BTN_FONTRICHEDIT, m_cmdFontRichEdit);
	DDX_Control(pDX, IDC_BTN_SAVEWINDEF, m_cmdSaveWinDef);
	DDX_Control(pDX, IDC_BTN_SAVEWIN, m_cmdSaveWin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPLanguages, CPropertyPage)
	//{{AFX_MSG_MAP(CPPLanguages)
	ON_BN_CLICKED(IDC_BTN_SAVEWIN, OnBtnSavewin)
	ON_BN_CLICKED(IDC_BTN_SAVEWINDEF, OnBtnSavewindef)
	ON_BN_CLICKED(IDC_BTN_FONTRICHEDIT, OnBtnFontrichedit)
	ON_BN_CLICKED(IDC_BTN_FONTEDIT, OnBtnFontedit)
	ON_LBN_SELCHANGE(IDC_LSTLANGUAGE, OnSelchangeLstlanguage)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()

void CPPLanguages::OnBtnSavewin() 
{
	CRealPopupDlg	*rpDlg=(CRealPopupDlg *)AfxGetMainWnd();
	CSendDlg		*snDlg=rpDlg->SendDlgLast;
	CRect wndRect;
	rpDlg->GetWindowRect(wndRect);
	
	m_rpApp->opz_wndTop = wndRect.top;
	m_rpApp->opz_wndLeft = wndRect.left;
	m_rpApp->opz_wndHeight = wndRect.Height();
	m_rpApp->opz_wndWidth = wndRect.Width();

	if (snDlg!=NULL)
	{
		if (IsWindow(snDlg->m_hWnd))
		{
		snDlg->GetWindowRect(wndRect);
		m_rpApp->opz_sndwndTop = wndRect.top;
		m_rpApp->opz_sndwndLeft = wndRect.left;
		m_rpApp->opz_sndwndHeight = wndRect.Height();
		m_rpApp->opz_sndwndWidth = wndRect.Width();
		}
	}

	CancelToClose();

}

void CPPLanguages::OnBtnSavewindef() 
{
	CRealPopupDlg	*rpDlg=(CRealPopupDlg *)AfxGetMainWnd();
	CSendDlg		*snDlg=rpDlg->SendDlgLast;
	CRect wndRect;
	
	rpDlg->GetWindowRect(wndRect);
	rpDlg->MoveWindow(wndRect.left,wndRect.top,RP_WINSIZEX,RP_WINSIZEY);
	rpDlg->CenterWindow();

	OnBtnSavewin();

	m_rpApp->opz_sndwndTop = 0;
	m_rpApp->opz_sndwndLeft = 0;
	m_rpApp->opz_sndwndHeight = 0;
	m_rpApp->opz_sndwndWidth = 0;
	
}

BOOL CPPLanguages::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_rpApp=(CRealPopupApp *)AfxGetApp();

	m_cmdSaveWin.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_POSSAVE)));
	m_cmdSaveWin.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);
	m_cmdSaveWinDef.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_POSDEFAULT)));
	m_cmdSaveWinDef.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);

	m_ResActual=&m_rpApp->m_ResActual;
	m_ResDLLs=&m_rpApp->m_ResDLLs;
	m_ResDLLsAbouts=&m_rpApp->m_ResDLLsAbouts;
	m_ResDLLsDescs=&m_rpApp->m_ResDLLsDescs;

	for (int k=0; k<m_ResDLLsDescs->GetSize(); k++)
	{
		m_lstLanguage.AddString(m_ResDLLsDescs->GetAt(k));
		if (m_ResActual->CompareNoCase(m_ResDLLs->GetAt(k))==0)
		{
			m_lstLanguage.SetCurSel(k);
			OnSelchangeLstlanguage();
		}
	}

	m_cmdFontRichEdit.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_RICHEDITFONT)));
	m_cmdFontRichEdit.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);
	m_cmdFontEdit.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_EDITFONT)));
	m_cmdFontEdit.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);

	return TRUE;
}

void CPPLanguages::OnHelp()
{
	m_rpApp->ShowHelp(this, "#layout");
}

void CPPLanguages::OnBtnFontrichedit() 
{

	LOGFONT lgold;
	m_rpApp->font_RichEdit.GetLogFont(&lgold);
	CFontDialog dlgMsg(&lgold);
	if (dlgMsg.DoModal() == IDOK)
	{

		CancelToClose();
		CFont *font_RichEdit=&m_rpApp->font_RichEdit;
		LOGFONT *lgFont = dlgMsg.m_cf.lpLogFont;
		lgFont->lfCharSet=m_rpApp->font_lfSystem.lfCharSet;
		font_RichEdit->DeleteObject();
		font_RichEdit->CreateFontIndirect(lgFont);				
		m_rpApp->font_RichEditchanged=true;
	}
	
}

void CPPLanguages::OnBtnFontedit() 
{
	LOGFONT lgold;
	m_rpApp->font_Edit.GetLogFont(&lgold);
	CFontDialog dlgMsg(&lgold);
	if (dlgMsg.DoModal() == IDOK)
	{
		CancelToClose();
		CFont *font_Edit=&m_rpApp->font_Edit;
		LOGFONT *lgFont = dlgMsg.m_cf.lpLogFont;
		lgFont->lfCharSet=m_rpApp->font_lfSystem.lfCharSet;
		font_Edit->DeleteObject();
		font_Edit->CreateFontIndirect(lgFont);
		m_rpApp->font_Editchanged=true;
	}
	
}

void CPPLanguages::OnSelchangeLstlanguage() 
{
	CWnd *pIDC_LANGUAGE = GetDlgItem(IDC_LANGUAGE);
	pIDC_LANGUAGE->SetWindowText(m_ResDLLsAbouts->GetAt(m_lstLanguage.GetCurSel()));
	m_ResSet=m_lstLanguage.GetCurSel();
	
}

void CPPLanguages::OnOK() 
{
	CPropertyPage::OnOK();
}


