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
#include "DlgCriteria.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCriteria dialog


CDlgCriteria::CDlgCriteria(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCriteria::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCriteria)
	m_txtCriteria = _T("");
	m_lblMatro = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCriteria::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCriteria)
	DDX_Control(pDX, IDC_CRITERIA, m_editCriteria);
	DDX_Text(pDX, IDC_CRITERIA, m_txtCriteria);
	DDX_Text(pDX, IDC_SEARCHCRITERIA, m_lblMatro);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCriteria, CDialog)
	//{{AFX_MSG_MAP(CDlgCriteria)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCriteria message handlers

BOOL CDlgCriteria::OnInitDialog() 
{
	m_rpApp=(CRealPopupApp *)AfxGetApp();

	m_lblMatro.LoadString(MSG_SEARCHCRITERIA);
	CDialog::OnInitDialog();

#ifdef USASETFONT
	CWnd *mywnd=GetTopWindow();
	while (mywnd!=NULL)
	{
		mywnd->SetFont(&m_rpApp->font_System,true);
		mywnd=mywnd->GetNextWindow(GW_HWNDNEXT);
	}
#endif

	m_editCriteria.bCriteria=true;
	m_editCriteria.oAutoCompleteRank=&(m_rpApp->oAutoCompleteRank);
	
	return TRUE;  
}
