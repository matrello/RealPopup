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
#include "PPUsers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPPUsers, CPropertyPage)

CPPUsers::CPPUsers() : CPropertyPage(CPPUsers::IDD)
{
	//{{AFX_DATA_INIT(CPPUsers)
	m_Log = _T("");
	m_chkIgnore = 2;
	m_chkIncludeFavorites = 2;
	m_chkLog = 2;
	m_chkPopUp = 2;
	m_chkBalloon = 2;
	m_chkBalloonPreview = 2;
	m_chkRedirect = 2;
	m_chkUsesRealPopup = 2;
	m_chkOnlineWarn = 2;
	m_cboUsesRealPopup = 0;
	//}}AFX_DATA_INIT

	SF_NoUserChange=false;
	lastPos=-1;
}

CPPUsers::~CPPUsers()
{
}

void CPPUsers::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPUsers)
	DDX_Control(pDX, IDC_SRCINSERT, m_cmdSrcInsert);
	DDX_Control(pDX, IDC_NETWORK3, m_cmdNetwork3);
	DDX_Control(pDX, IDC_NETWORK2, m_cmdNetwork2);
	DDX_Control(pDX, IDC_NETWORK, m_cmdNetwork);
	DDX_Control(pDX, IDC_SPIN_USER, spinUser);
	DDX_Control(pDX, IDC_BTN_REMOVE, btnRemove);
	DDX_Control(pDX, IDC_BTN_ADD, btnAdd);
	DDX_Control(pDX, IDC_LISTUSERS, m_lstUsers);
	DDX_Control(pDX, IDC_TXT_REDIRECT, m_Redirect);
	DDX_Control(pDX, IDC_TXT_USER, m_Users);
	DDX_Control(pDX, IDC_TXT_MEMBERS, m_Members);
	DDX_Text(pDX, IDC_TXT_LOG, m_Log);
	DDX_Check(pDX, IDC_CHK_IGNORE, m_chkIgnore);
	DDX_Check(pDX, IDC_CHK_NBINCLUDEFAVORITES, m_chkIncludeFavorites);
	DDX_Check(pDX, IDC_CHK_LOG, m_chkLog);
	DDX_Check(pDX, IDC_CHK_POPUPNEW, m_chkPopUp);
	DDX_Check(pDX, IDC_CHK_BALLOONNEW, m_chkBalloon);
	DDX_Check(pDX, IDC_CHK_BALLOONPREVIEW, m_chkBalloonPreview);
	DDX_Check(pDX, IDC_CHK_REDIRECT, m_chkRedirect);
	DDX_Check(pDX, IDC_CHK_USESRP, m_chkUsesRealPopup);
	DDX_Check(pDX, IDC_CHK_ONLINEWARN, m_chkOnlineWarn);
	DDX_CBIndex(pDX, IDC_CBO_USESRP, m_cboUsesRealPopup);
	//}}AFX_DATA_MAP

	DDX_FilenameControl(pDX, IDC_TXT_LOG, m_ctrlGetFilename, 0 , MAKEINTRESOURCE(IDS_DDXFILE_TT_CAPTION), MAKEINTRESOURCE(IDS_DDXFILE_FILTER_LOG));
}


BEGIN_MESSAGE_MAP(CPPUsers, CPropertyPage)
	//{{AFX_MSG_MAP(CPPUsers)
	ON_BN_CLICKED(IDC_OPT_GROUP, OnOptGroup)
	ON_BN_CLICKED(IDC_OPT_USER, OnOptUser)
	ON_LBN_SELCHANGE(IDC_LISTUSERS, OnSelchangeListusers)
	ON_EN_CHANGE(IDC_TXT_USER, OnChangeTxtUser)
	ON_BN_CLICKED(IDC_BTN_REMOVE, OnBtnRemove)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_USER, OnDeltaposSpinUser)
	ON_BN_CLICKED(IDC_SRCINSERT, OnSrcinsert)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND_RANGE(IDC_NETWORK, IDC_NETWORK3, OnBtnNetwork)
	ON_COMMAND_RANGE(IDC_CHK_IGNORE, IDC_CHK_NBINCLUDEFAVORITES, OnBtnChecks)
END_MESSAGE_MAP()

BOOL CPPUsers::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_rpApp=(CRealPopupApp *)AfxGetApp();
	
	CheckRadioButton(IDC_OPT_USER,IDC_OPT_GROUP,IDC_OPT_USER);
	CheckRadioButton(IDC_OPT_NBINCLUDEASMEMBERS,IDC_OPT_NBINCLUDEASGROUP,IDC_OPT_NBINCLUDEASMEMBERS);

	m_cmdNetwork.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_NETWORK)));
	m_cmdNetwork.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);
	m_cmdNetwork2.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_NETWORK)));
	m_cmdNetwork2.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);
	m_cmdNetwork3.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_NETWORK)));
	m_cmdNetwork3.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);
	m_cmdSrcInsert.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_SRCINSERT)));
	m_cmdSrcInsert.SetButtonStyle(BS_CENTER | BS_PUSHBUTTON,true);

	CString tmp;
	tmp.LoadString(ITEM_USESREALPOPUP);
	((CComboBox *)GetDlgItem(IDC_CBO_USESRP))->AddString(tmp);
	tmp.LoadString(ITEM_USESWINPOPUP);
	((CComboBox *)GetDlgItem(IDC_CBO_USESRP))->AddString(tmp);

	UsersRefresh();
	m_Users.bSpacePermesso=false;
	m_Users.oAutoCompleteRank=&(m_rpApp->oAutoCompleteRank);
	m_Redirect.oAutoCompleteRank=&(m_rpApp->oAutoCompleteRank);
	m_Members.oAutoCompleteRank=&(m_rpApp->oAutoCompleteRank);

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

void CPPUsers::OnOptGroup() 
{
	GetDlgItem(IDC_NETWORK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_TXT_MEMBERS)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPT_NBINCLUDEASGROUP)->EnableWindow(IsDlgButtonChecked(IDC_CHK_NBINCLUDEFAVORITES)==1);
	GetDlgItem(IDC_OPT_NBINCLUDEASMEMBERS)->EnableWindow(IsDlgButtonChecked(IDC_CHK_NBINCLUDEFAVORITES)==1);
}

void CPPUsers::OnOptUser() 
{
	GetDlgItem(IDC_NETWORK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_TXT_MEMBERS)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPT_NBINCLUDEASGROUP)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPT_NBINCLUDEASMEMBERS)->EnableWindow(FALSE);
}

void CPPUsers::UsersRefresh()
{

	m_lstUsers.ResetContent();	
	for (int k=0; k<m_UsersData->GetCount(); k++)
	{
		m_lstUsers.AddString(m_UsersData->GetAt(k)->name);
	}
	UserRefresh(-1);

}

void CPPUsers::UserRefresh(int pos)
{

	static CString	selstrUser;

	CString users;
	m_Users.GetWindowText(users);
	if (users.GetLength()==0)
	{
		GetDlgItem(IDC_BTN_ADD)->EnableWindow( false );
		GetDlgItem(IDC_SRCINSERT)->EnableWindow( false );
	}
	else
	{
		GetDlgItem(IDC_BTN_ADD)->EnableWindow( pos==-1 );
		GetDlgItem(IDC_SRCINSERT)->EnableWindow( pos==-1 );
	}

	GetDlgItem(IDC_BTN_REMOVE)->EnableWindow( pos!=-1 );

	GetDlgItem(IDC_SPIN_USER)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_IGNORE)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_NBINCLUDEFAVORITES)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_POPUPNEW)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_BALLOONNEW)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_BALLOONPREVIEW)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_USESRP)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CBO_USESRP)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_REDIRECT)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_ONLINEWARN)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_TXT_REDIRECT)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_NETWORK3)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_CHK_LOG)->EnableWindow( pos!=-1 );
	m_ctrlGetFilename.EnableWindow( pos!=-1 );
	GetDlgItem(IDC_OPT_USER)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_OPT_GROUP)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_OPT_NBINCLUDEASMEMBERS)->EnableWindow( pos!=-1 );
	GetDlgItem(IDC_OPT_NBINCLUDEASGROUP)->EnableWindow( pos!=-1 );

	if (lastPos!=-1 && lastPos!=pos)
	{
		CString dummy;
		bool mod=false;
		UpdateData();
		SUser *lastUser = m_UsersData->GetAt(&selstrUser);

		if (lastUser!=NULL)
		{
			if (lastUser->chkIgnore!=m_chkIgnore)
			{
				mod=true;
				lastUser->chkIgnore=m_chkIgnore;
			}
			if (lastUser->chkIncludeFavorites!=m_chkIncludeFavorites)
			{
				mod=true;
				lastUser->chkIncludeFavorites=m_chkIncludeFavorites;
			}
			if (lastUser->chkLog!=m_chkLog)
			{
				mod=true;
				lastUser->chkLog=m_chkLog;
			}
			if (lastUser->chkOnlineWarn!=m_chkOnlineWarn)
			{
				mod=true;
				lastUser->chkOnlineWarn=m_chkOnlineWarn;
			}
			if (lastUser->chkPopUp!=m_chkPopUp)
			{
				mod=true;
				lastUser->chkPopUp=m_chkPopUp;
			}
			if (lastUser->chkBalloon!=m_chkBalloon)
			{
				mod=true;
				lastUser->chkBalloon=m_chkBalloon;
			}
			if (lastUser->chkBalloonPreview!=m_chkBalloonPreview)
			{
				mod=true;
				lastUser->chkBalloonPreview=m_chkBalloonPreview;
			}
			if (lastUser->chkRedirect!=m_chkRedirect)
			{
				mod=true;
				lastUser->chkRedirect=m_chkRedirect;
			}
			if (lastUser->chkUsesRealPopup!=m_chkUsesRealPopup)
			{
				mod=true;
				lastUser->chkUsesRealPopup=m_chkUsesRealPopup;
			}
			if (lastUser->cboUsesRealPopup!=m_cboUsesRealPopup)
			{
				mod=true;
				lastUser->cboUsesRealPopup=m_cboUsesRealPopup;
			}

			m_Redirect.GetWindowText(dummy);
			if (lastUser->Redirect.Compare(dummy)!=0)
			{	
				mod=true;
				lastUser->Redirect=dummy;
			}
			m_Members.GetWindowText(dummy);
			if (lastUser->members.Compare(dummy)!=0)
			{	
				mod=true;
				lastUser->members=dummy;
			}
			if (lastUser->Log.Compare(m_Log)!=0)
			{	
				mod=true;
				lastUser->Log=m_Log;
			}
			BOOL isGroup=( GetCheckedRadioButton(IDC_OPT_USER,IDC_OPT_GROUP) == IDC_OPT_GROUP);
			if (lastUser->group!=isGroup)
			{	
				mod=true;
				lastUser->group=isGroup;
			}

			isGroup=( GetCheckedRadioButton(IDC_OPT_NBINCLUDEASMEMBERS,IDC_OPT_NBINCLUDEASGROUP) == IDC_OPT_NBINCLUDEASGROUP);
			if (lastUser->includeAsGroup!=isGroup)
			{	
				mod=true;
				lastUser->includeAsGroup=isGroup;
			}

			if (mod)
			{
				lastUser->isDatDefaults=false;
				CancelToClose();
			}
		}
	}

	if (pos!=-1)
	{
		m_lstUsers.GetText(pos,selstrUser);

		SUser *selUser = m_UsersData->GetAt(&selstrUser);

		SF_NoUserChange=true;

		((CButton *)(GetDlgItem(IDC_CHK_IGNORE)))->SetCheck(selUser->chkIgnore);
		((CButton *)(GetDlgItem(IDC_CHK_NBINCLUDEFAVORITES)))->SetCheck(selUser->chkIncludeFavorites);
		((CButton *)(GetDlgItem(IDC_CHK_POPUPNEW)))->SetCheck(selUser->chkPopUp);
		((CButton *)(GetDlgItem(IDC_CHK_BALLOONNEW)))->SetCheck(selUser->chkBalloon);
		((CButton *)(GetDlgItem(IDC_CHK_BALLOONPREVIEW)))->SetCheck(selUser->chkBalloonPreview);
		((CButton *)(GetDlgItem(IDC_CHK_USESRP)))->SetCheck(selUser->chkUsesRealPopup);
		((CButton *)(GetDlgItem(IDC_CHK_ONLINEWARN)))->SetCheck(selUser->chkOnlineWarn);
		((CButton *)(GetDlgItem(IDC_CHK_REDIRECT)))->SetCheck(selUser->chkRedirect);
		((CButton *)(GetDlgItem(IDC_CHK_LOG)))->SetCheck(selUser->chkLog);

		GetDlgItem(IDC_CBO_USESRP)->EnableWindow(IsDlgButtonChecked(IDC_CHK_USESRP)==1);
		((CComboBox *)(GetDlgItem(IDC_CBO_USESRP)))->SetCurSel(selUser->cboUsesRealPopup);

		GetDlgItem(IDC_CHK_POPUPNEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_IGNORE)!=1);
		GetDlgItem(IDC_CHK_BALLOONNEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_IGNORE)!=1 && IsDlgButtonChecked(IDC_CHK_POPUPNEW)!=1);
		GetDlgItem(IDC_CHK_BALLOONPREVIEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_IGNORE) && IsDlgButtonChecked(IDC_CHK_POPUPNEW)!=1);
		GetDlgItem(IDC_CHK_BALLOONPREVIEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_BALLOONNEW)!=0 && (IsDlgButtonChecked(IDC_CHK_IGNORE)!=1 && IsDlgButtonChecked(IDC_CHK_POPUPNEW)!=1));

		GetDlgItem(IDC_TXT_MEMBERS)->SetWindowText(selUser->members);
		GetDlgItem(IDC_TXT_REDIRECT)->SetWindowText(selUser->Redirect);
		GetDlgItem(IDC_TXT_LOG)->SetWindowText(selUser->Log);

		if (selUser->group)
		{
			CheckRadioButton(IDC_OPT_USER,IDC_OPT_GROUP,IDC_OPT_GROUP);
			OnOptGroup();
		}
		else
		{
			CheckRadioButton(IDC_OPT_USER,IDC_OPT_GROUP,IDC_OPT_USER);
			OnOptUser();
		}

		GetDlgItem(IDC_OPT_NBINCLUDEASGROUP)->EnableWindow(IsDlgButtonChecked(IDC_CHK_NBINCLUDEFAVORITES)==1 && IsDlgButtonChecked(IDC_OPT_GROUP)==1);
		GetDlgItem(IDC_OPT_NBINCLUDEASMEMBERS)->EnableWindow(IsDlgButtonChecked(IDC_CHK_NBINCLUDEFAVORITES)==1 && IsDlgButtonChecked(IDC_OPT_GROUP)==1);
		if (selUser->includeAsGroup)
			CheckRadioButton(IDC_OPT_NBINCLUDEASMEMBERS,IDC_OPT_NBINCLUDEASGROUP,IDC_OPT_NBINCLUDEASGROUP);
		else
			CheckRadioButton(IDC_OPT_NBINCLUDEASMEMBERS,IDC_OPT_NBINCLUDEASGROUP,IDC_OPT_NBINCLUDEASMEMBERS);

		GetDlgItem(IDC_TXT_USER)->SetWindowText(selUser->name);

		if (selstrUser.Compare(m_rpApp->env_DefAllUsers)==0)
		{
			GetDlgItem(IDC_CHK_NBINCLUDEFAVORITES)->EnableWindow( false );
			GetDlgItem(IDC_BTN_REMOVE)->EnableWindow( false );
			GetDlgItem(IDC_OPT_GROUP)->EnableWindow( false );
			GetDlgItem(IDC_SPIN_USER)->EnableWindow( false );
		}

		SF_NoUserChange=false;
	}
	else
	{
		CheckRadioButton(IDC_OPT_USER,IDC_OPT_GROUP,IDC_OPT_USER);
		OnOptUser();
		CheckRadioButton(IDC_OPT_NBINCLUDEASMEMBERS,IDC_OPT_NBINCLUDEASGROUP,IDC_OPT_NBINCLUDEASMEMBERS);
		((CButton *)(GetDlgItem(IDC_CHK_IGNORE)))->SetCheck(2);
		((CButton *)(GetDlgItem(IDC_CHK_NBINCLUDEFAVORITES)))->SetCheck(2);
		((CButton *)(GetDlgItem(IDC_CHK_POPUPNEW)))->SetCheck(2);
		((CButton *)(GetDlgItem(IDC_CHK_USESRP)))->SetCheck(2);
		((CButton *)(GetDlgItem(IDC_CHK_ONLINEWARN)))->SetCheck(2);
		((CButton *)(GetDlgItem(IDC_CHK_REDIRECT)))->SetCheck(2);
		((CButton *)(GetDlgItem(IDC_CHK_LOG)))->SetCheck(2);
		
		GetDlgItem(IDC_CBO_USESRP)->EnableWindow(0);
		GetDlgItem(IDC_OPT_NBINCLUDEASMEMBERS)->EnableWindow(0);
		GetDlgItem(IDC_OPT_NBINCLUDEASGROUP)->EnableWindow(0);

		GetDlgItem(IDC_TXT_MEMBERS)->SetWindowText("");
		GetDlgItem(IDC_TXT_REDIRECT)->SetWindowText("");
		GetDlgItem(IDC_TXT_LOG)->SetWindowText("");

	}

	lastPos=pos;
}

void CPPUsers::OnSelchangeListusers() 
{
	UserRefresh(m_lstUsers.GetCurSel());
}

void CPPUsers::OnChangeTxtUser() 
{

	if (!SF_NoUserChange)
	{
		CString changedUser;
		GetDlgItem(IDC_TXT_USER)->GetWindowText(changedUser);

		int k=LB_ERR, lastk;
		bool ok=false;
		CString selUser;

		do
		{
			lastk=k;
			k=m_lstUsers.FindString(k,changedUser);
			if (k!=LB_ERR && k>=lastk)
			{
				m_lstUsers.GetText(k,selUser);
				if (selUser.CompareNoCase(changedUser)==0)
				{
					ok=true;
					m_lstUsers.SetCurSel(k);
					UserRefresh(m_lstUsers.GetCurSel());
				}
			}
		} while (k!=LB_ERR && k>lastk);

		if (!ok)
		{
			UserRefresh(-1);
			m_lstUsers.SetCurSel(-1);
		}

		GetDlgItem(IDC_NETWORK)->EnableWindow(changedUser.GetLength()==0);
	}

}

void CPPUsers::OnBtnRemove() 
{
	CString removeUser;
	GetDlgItem(IDC_TXT_USER)->GetWindowText(removeUser);

	m_UsersData->RemoveAt(&removeUser);
	UsersRefresh();
	m_Users.SetWindowText("");
	CancelToClose();
}

void CPPUsers::OnBtnAdd() 
{
	bool sbajata=false;

	SUser newUser;
	GetDlgItem(IDC_TXT_USER)->GetWindowText(newUser.name);
	int fnd1,fnd2,rev1,rev2;
	fnd1=newUser.name.Find('[');
	fnd2=newUser.name.Find(']');
	rev1=newUser.name.ReverseFind('[');
	rev2=newUser.name.ReverseFind(']');

	if (fnd1==-1)
	{
		if (fnd2>-1)
			sbajata=true;
	}
	else
	{
		if ((rev1!=fnd1) || (fnd1==0) || (fnd2==(fnd1+1)))
			sbajata=true;
		else
		{
			if (fnd2<fnd1)
				sbajata=true;
			else
			{
				if (rev2!=fnd2)
					sbajata=true;
			}
		}
	}

	if (sbajata)
		ErrMsg(this,MSG_ERR_USERINSERT);
	else
	{
		CRPUsers::GetDefaultProfile(&newUser, false);
		m_UsersData->myUsers.InsertAt(0,newUser);

		UsersRefresh();
		m_Users.SetWindowText("");
		CancelToClose();
	}
}

void CPPUsers::OnOK()
{
	UserRefresh(-1);
	CDialog::OnOK();
}

void CPPUsers::OnDeltaposSpinUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString moveUser;
	GetDlgItem(IDC_TXT_USER)->GetWindowText(moveUser);
	int cursel=m_lstUsers.GetCurSel();

	if ( (pNMUpDown->iDelta<0) && (cursel>0) )
	{
		m_UsersData->MoveUp(&moveUser);
		m_lstUsers.DeleteString(cursel);
		m_lstUsers.InsertString(cursel-1,moveUser);

		CancelToClose();
	}
	if ( (pNMUpDown->iDelta>0) && (cursel<m_lstUsers.GetCount()-2) )
	{
		m_UsersData->MoveDown(&moveUser);
		m_lstUsers.DeleteString(cursel);
		m_lstUsers.InsertString(cursel+1,moveUser);

		CancelToClose();
	}
	m_lstUsers.SelectString(-1,moveUser);
	OnSelchangeListusers();
	
	*pResult = 0;
}

void CPPUsers::OnBtnNetwork(UINT nID) 
{
	CString myUsers, myMatro;

	CRPNetworkBrowser dlg;
	dlg.m_ProtocolsHandler=m_ProtocolsHandler;
	dlg.m_UsersSelectedString=&myMatro;
	dlg.m_UserDomain=&(((CRealPopupApp *) AfxGetApp())->opz_LastDomain);

	CRect ownRect;
	GetWindowRect(ownRect);
	ownRect.DeflateRect(20,20);
	dlg.m_parentRect=ownRect;

	if (nID==IDC_NETWORK) dlg.m_SoloUno=true;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		switch (nID)
		{
		case IDC_NETWORK:
				{
					GetDlgItem(IDC_TXT_USER)->GetWindowText(myUsers);
					if (myUsers.GetLength()>0) myUsers+=',';
					myUsers+=myMatro;
					GetDlgItem(IDC_TXT_USER)->SetWindowText(myUsers);
					break;
				}
		case IDC_NETWORK2:
				{
					GetDlgItem(IDC_TXT_MEMBERS)->GetWindowText(myUsers);
					if (myUsers.GetLength()>0) myUsers+=',';
					myUsers+=myMatro;
					GetDlgItem(IDC_TXT_MEMBERS)->SetWindowText(myUsers);
					break;
				}
		case IDC_NETWORK3:
				{
					GetDlgItem(IDC_TXT_REDIRECT)->GetWindowText(myUsers);
					if (myUsers.GetLength()>0) myUsers+=',';
					myUsers+=myMatro;
					GetDlgItem(IDC_TXT_REDIRECT)->SetWindowText(myUsers);
					break;
				}
		}
	}
	else if (nResponse == IDCANCEL)
	{
	}	
}

void CPPUsers::OnBtnChecks(UINT nID) 
{

	if (IsDlgButtonChecked(nID)==2)
	{
		CString selUser;
		m_Users.GetWindowText(selUser);

		if (selUser.Compare(m_rpApp->env_DefAllUsers)==0)
			CheckDlgButton(nID,0);
	}

	switch (nID)
	{
	case IDC_CHK_NBINCLUDEFAVORITES:
		{
			GetDlgItem(IDC_OPT_NBINCLUDEASGROUP)->EnableWindow(IsDlgButtonChecked(IDC_CHK_NBINCLUDEFAVORITES)==1 && IsDlgButtonChecked(IDC_OPT_GROUP)==1);
			GetDlgItem(IDC_OPT_NBINCLUDEASMEMBERS)->EnableWindow(IsDlgButtonChecked(IDC_CHK_NBINCLUDEFAVORITES)==1 && IsDlgButtonChecked(IDC_OPT_GROUP)==1);
			break;
		}
	case IDC_CHK_USESRP:
		{
			GetDlgItem(IDC_CBO_USESRP)->EnableWindow(IsDlgButtonChecked(IDC_CHK_USESRP)==1);
			break;
		}
	case IDC_CHK_IGNORE:
		{
			GetDlgItem(IDC_CHK_POPUPNEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_IGNORE)!=1);
			GetDlgItem(IDC_CHK_BALLOONNEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_IGNORE)!=1);
			GetDlgItem(IDC_CHK_BALLOONPREVIEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_IGNORE)!=1);
			break;
		}
	case IDC_CHK_POPUPNEW:
		{
			GetDlgItem(IDC_CHK_BALLOONNEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_POPUPNEW)!=1);
			GetDlgItem(IDC_CHK_BALLOONPREVIEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_POPUPNEW)!=1);
			break;
		}
	case IDC_CHK_BALLOONNEW:
		{
			GetDlgItem(IDC_CHK_BALLOONPREVIEW)->EnableWindow(IsDlgButtonChecked(IDC_CHK_BALLOONNEW)!=0 && IsDlgButtonChecked(IDC_CHK_IGNORE)!=1);
			break;
		}
	}
}

void CPPUsers::OnHelp()
{
	m_rpApp->ShowHelp(this, "#usersgroups");
}

void CPPUsers::OnSrcinsert() 
{
	CDlgCriteria dlg;
	CString devby, matro;
	m_Users.GetWindowText(matro);
	AfxExtractSubString(devby, matro, 1, '[');
	devby.Remove('[');
	devby.Remove(']');
	dlg.m_txtCriteria=devby;

	int nRes=dlg.DoModal();
	if (nRes==IDOK)
	{
		AfxExtractSubString(devby, matro, 0, '[');
		devby += "[";
		devby += dlg.m_txtCriteria;
		devby += "]";
		m_Users.SetWindowText(devby);
	}
	
}
