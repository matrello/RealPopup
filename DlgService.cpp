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
#include "DlgService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgService dialog


CDlgService::CDlgService(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgService::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgService)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgService::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgService)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgService, CDialog)
	//{{AFX_MSG_MAP(CDlgService)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgService message handlers

BOOL CDlgService::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
	
	return TRUE;  
}

void CDlgService::OnOK() 
{
	if (GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2) == IDC_RADIO1)
		m_Msn=OPZ_MSN_TAKECONTROL;
	else
		m_Msn=OPZ_MSN_LEAVE;
	
	CDialog::OnOK();
}
