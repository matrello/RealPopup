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
#include "PPEnvironment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPPEnvironment, CPropertyPage)

CPPEnvironment::CPPEnvironment() : CPropertyPage(CPPEnvironment::IDD)
{
	//{{AFX_DATA_INIT(CPPEnvironment)
	m_chkTop = FALSE;
	m_chkWarn = FALSE;
	m_chkAutoStart = FALSE;
	m_chkScrAway = FALSE;
	m_chkLeftTray = FALSE;
	m_chkNBDetermineFavorites = FALSE;
	m_chkWarnDel = FALSE;
	m_chkWarnDomain = FALSE;
	m_chkWarnRPProtocolNotActive = FALSE;
	//}}AFX_DATA_INIT

}

CPPEnvironment::~CPPEnvironment()
{
}

void CPPEnvironment::DoDataExchange(CDataExchange* pDX)
{
	
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPEnvironment)
	DDX_Control(pDX, IDC_RPHOTKEY, m_rpHotKey);
	DDX_Check(pDX, IDC_CHK_TOP, m_chkTop);
	DDX_Check(pDX, IDC_CHK_WARN, m_chkWarn);
	DDX_Check(pDX, IDC_CHK_AUTOSTART, m_chkAutoStart);
	DDX_Check(pDX, IDC_CHK_LEFTTRAY, m_chkLeftTray);
	DDX_Check(pDX, IDC_CHK_AWAY, m_chkScrAway);
	DDX_Check(pDX, IDC_CHK_NBDETERMINEFAVORITES, m_chkNBDetermineFavorites);
	DDX_Check(pDX, IDC_CHK_WARNDEL, m_chkWarnDel);
	DDX_Check(pDX, IDC_CHK_WARNDOMAIN, m_chkWarnDomain);
	DDX_Check(pDX, IDC_CHK_WARNRPPROTOCOLNOTACTIVE, m_chkWarnRPProtocolNotActive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPEnvironment, CPropertyPage)
	//{{AFX_MSG_MAP(CPPEnvironment)
	ON_BN_CLICKED(IDC_CHK_AUTOSTART, OnChkAutostart)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()

void CPPEnvironment::OnChkAutostart() 
{
	UpdateData();
	SetAutostart(m_chkAutoStart);
	CancelToClose();
}

BOOL CPPEnvironment::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_rpApp=(CRealPopupApp *)AfxGetApp();

	m_rpHotKey.SetRules(HKCOMB_NONE | HKCOMB_A | HKCOMB_C | HKCOMB_S,
		HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_rpHotKey.SetHotKey(opz_HotKey, opz_HotKeymd);

	if (m_Msn==OPZ_MSN_TAKECONTROL)
		CheckRadioButton(IDC_MSN,IDC_MSN2,IDC_MSN);
	else
		if (m_Msn==OPZ_MSN_LEAVE)
			CheckRadioButton(IDC_MSN,IDC_MSN2,IDC_MSN2);

	if (IsNT())
	{
		GetDlgItem(IDC_MSN)->EnableWindow(TRUE);
		GetDlgItem(IDC_MSN2)->EnableWindow(TRUE);
	}

	return TRUE;
}

void CPPEnvironment::OnHelp()
{
	m_rpApp->ShowHelp(this, "#application");
}

void CPPEnvironment::OnOK() 
{
	int Matrioska=GetCheckedRadioButton(IDC_MSN,IDC_MSN2);

	if (Matrioska!=0)
	{
		if (Matrioska==IDC_MSN)
			m_Msn=OPZ_MSN_TAKECONTROL;
		else
			m_Msn=OPZ_MSN_LEAVE;
	}

	m_rpHotKey.GetHotKey(opz_HotKey, opz_HotKeymd);
	CPropertyPage::OnOK();
}
