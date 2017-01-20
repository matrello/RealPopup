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
#include <afxmt.h>
#include "realpopup.h"
#include "RPNetworkBrowser.h"

#define WM_NETTHREADABORTED   1	// user closed dialog during netThread
#define WM_NETTHREADABORTEDOK 2 // same as above, but the user pressed OK
#define WM_NETTHREADENDED	  3 // thread ended normally (may happen! :-)

#define NB_NUMPICS	5	// number of icons for the Network Browser

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRPNetworkBrowser::CRPNetworkBrowser(CWnd* pParent /*=NULL*/)
	: CDialog(CRPNetworkBrowser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRPNetworkBrowser)
	//}}AFX_DATA_INIT

	m_SoloUno=false;
	SF_DoubleClickPressed=false;
}

CRPNetworkBrowser::~CRPNetworkBrowser()
{
	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::(destructor)", "check if thread is active (netThreadAbort: %i)\n", netThreadAbort);
	if (IsThreadRunning(netThread))
	{
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::(destructor)", "wait for pending thread to finish...\n");
		netThreadAbort=1;
		netThread->m_bAutoDelete=true;
		WaitForSingleObject(netThread->m_hThread, INFINITE);
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::(destructor)", "...OK\n");
	}

	for (int k=0; k<lpnrAllocated.GetSize(); k++)
	{
		LPNETRESOURCE lpnr=(LPNETRESOURCE)lpnrAllocated.GetAt(k);
		free(lpnr);
	}
}

void CRPNetworkBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRPNetworkBrowser)
	DDX_Control(pDX, IDC_TREE1, m_NetTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRPNetworkBrowser, CDialog)
	//{{AFX_MSG_MAP(CRPNetworkBrowser)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, OnKeydownTree1)
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE1, OnSetfocusTree1)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_HELP, OnHelp)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnNMDblclkTree)
END_MESSAGE_MAP()

UINT ThreadUsersScanWNet(LPVOID pParam)
{	// invokes the WNet* functions

	sThreadUsersScan *myParams=(sThreadUsersScan *) pParam;

	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScanWNet", "started (%X)...\n", AfxGetThread());

	myParams->inUse=true;
	LPNETRESOURCE lpnr=myParams->lpnr, lpnrLocal=myParams->lpnrLocal;
	DWORD dwResult, dwResultEnum, cEntries=myParams->cEntries, cbBuffer=myParams->cbBuffer;
	HANDLE hEnum=myParams->hEnum;

	if (myParams->wnetType==1)
	{
		myParams->inUse=false;
		dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, 0, RESOURCEUSAGE_CONTAINER, lpnr, &hEnum);	
	}
	else
	{
		myParams->inUse=false;
		dwResultEnum = WNetEnumResource(hEnum, &cEntries, lpnrLocal, &cbBuffer);
	}

	if (myParams->ValidStruct==STRUTTURAVALIDA)
	{
		myParams->inUse=true;
		myParams->dwResult=dwResult;
		myParams->dwResultEnum =dwResultEnum;
		myParams->lpnr=lpnr; myParams->lpnrLocal=lpnrLocal;
		myParams->cEntries=cEntries; myParams->cbBuffer=cbBuffer;
		myParams->hEnum=hEnum;
		myParams->inUse=false;
	}

	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScanWNet", "...ended (%X)\n", AfxGetThread());

	return TRUE;
}

UINT ThreadUsersScan(LPVOID pParam)
{	// scans for domains and workstations on the network

	sThreadUsersScan *myParams=(sThreadUsersScan *) pParam;
	long oldc;

	if (myParams->m_NetTree)
	{
		oldc=GetClassLong(myParams->m_NetTree->m_hWnd,GCL_HCURSOR);
		SetClassLong(myParams->m_NetTree->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL, IDC_APPSTARTING));
	}

    myParams->cEntries = 0xFFFFFFFF;
	myParams->cbBuffer=16384;
	HTREEITEM myItem;
    DWORD i;

	if (myParams->hParent!=TVI_ROOT && !myParams->EnumServers && myParams->m_NetTree)
	{
		myItem=myParams->m_NetTree->GetChildItem(myParams->hParent);

		if (myItem)
			myParams->m_NetTree->DeleteItem(myItem);
		else
			return TRUE; 
	}
	
	myParams->wnetType=1;	// invokes the WNetOpenEnum()
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetOpenEnum invoking...\n");
	CWinThread *wnetThread=AfxBeginThread(ThreadUsersScanWNet,(LPVOID)myParams, NULL, NULL, CREATE_SUSPENDED, NULL);
	wnetThread->m_bAutoDelete=false;
	wnetThread->ResumeThread();

	while (WaitForSingleObject(wnetThread->m_hThread, 200)==WAIT_TIMEOUT || myParams->inUse)
	{
		// parent thread asked for abort (cancel or ok buttons) [1/3]
		if (*myParams->netThreadAbort!=0)
			break;
	}

	wnetThread->m_bAutoDelete=true;

	if (*myParams->netThreadAbort!=0)
		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetOpenEnum ... **aborted**\n");
	else
		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetOpenEnum ...returned\n");

	if ((*myParams->netThreadAbort==0))
	{
		if (myParams->dwResult != NO_ERROR)
		{
			if (myParams->m_NetTree)
				SetClassLong(myParams->m_NetTree->m_hWnd,GCL_HCURSOR,oldc);

			return FALSE;
		}

		if (myParams->m_NetTree) myParams->m_NetTree->SetRedraw(false);
		do {  
			myParams->lpnrLocal = (LPNETRESOURCE) calloc(myParams->cbBuffer, 1);  
			myParams->lpnrAllocated->Add( (long) myParams->lpnrLocal );

			myParams->wnetType=2;	// invokes the WNetEnumResource()
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetEnumResource invoking...\n");
			CWinThread *wnetThread=AfxBeginThread(ThreadUsersScanWNet,(LPVOID)myParams, NULL, NULL, CREATE_SUSPENDED, NULL);
			wnetThread->m_bAutoDelete=false;
			wnetThread->ResumeThread();
			while (WaitForSingleObject(wnetThread->m_hThread, 200)==WAIT_TIMEOUT || myParams->inUse)
			{
				// parent thread asked for abort (cancel or ok buttons) [2/3]
				if (*myParams->netThreadAbort!=0)
					break;
			}

			wnetThread->m_bAutoDelete=true;

			if (*myParams->netThreadAbort!=0)
			{
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetEnumResource ... **aborted**\n");
				break;
			}
			else
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetEnumResource ...returned\n");
			
			if (myParams->dwResultEnum == NO_ERROR) { 
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "(a) enter the cEntries loop\n");
				for(i = 0; i < myParams->cEntries; i++)
				{ 
					// parent thread asked for abort (cancel or ok buttons) [3/3]
					if (*myParams->netThreadAbort!=0)
						break;

					if (myParams->lpnrLocal[i].lpRemoteName!=NULL)
					{
						if (strcmp(myParams->lpnrLocal[i].lpRemoteName,"\\\\tsclient")==0)
							continue;

						if (strcmp(myParams->lpnrLocal[i].lpRemoteName,"\\\\.host")==0)
							continue;
					}

					// recursively call if NETRESOURCE is a parent
					if(RESOURCEUSAGE_CONTAINER == (myParams->lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER))
					{
						if (myParams->EnumServers)
							myParams->servers->Add( (long) &(myParams->lpnrLocal)[i] );
						else
						{
							CString myCaption;
							bool	isComputer=false;
							TV_INSERTSTRUCT TreeCtrlItem;

							myCaption=myParams->lpnrLocal[i].lpRemoteName;
							if (myCaption.Left(2)=="\\\\")
							{
								myCaption=myCaption.Right(myCaption.GetLength()-2);
								isComputer=true;
							}

							if (!myParams->m_NetTree)
							{
								// rp internal domains list
								myParams->m_domini->Add(myCaption);
								ReTracer(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "found domain %s\n",myCaption);
								ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "found domains [%i]\n",myParams->m_domini->GetCount());
							}
							else
							{
								// network browser treeview
								TreeCtrlItem.item.pszText = (char *) LPCTSTR(myCaption);
								TreeCtrlItem.hParent = myParams->hParent;
								TreeCtrlItem.hInsertAfter = TVI_SORT;
								TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
								TreeCtrlItem.item.lParam = (long) &(myParams->lpnrLocal)[i];

								if (isComputer)
									TreeCtrlItem.item.iImage=0;
								else
									TreeCtrlItem.item.iImage=2;
								TreeCtrlItem.item.iSelectedImage=TreeCtrlItem.item.iImage;

								HTREEITEM myItem = myParams->m_NetTree->InsertItem(&TreeCtrlItem);

								if (!isComputer)
								{
									TreeCtrlItem.hParent = myItem;
									TreeCtrlItem.hInsertAfter = TVI_SORT;
									TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
									TreeCtrlItem.item.pszText = "CodeByMatro";
									TreeCtrlItem.item.lParam = 0;
									myItem = myParams->m_NetTree->InsertItem(&TreeCtrlItem);
								}
							}
						}
					}
				}
				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "(b) exited the cEntries loop\n");
			}  
			else if (myParams->dwResultEnum != ERROR_NO_MORE_ITEMS)
			{
				if (myParams->m_NetTree)
				{
					SetClassLong(myParams->m_NetTree->m_hWnd,GCL_HCURSOR,oldc);
					myParams->m_NetTree->SetRedraw(true);
				}

				ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "aborted: WNetEnumResource returned an error.\n");
				return FALSE;
			}

		} while(myParams->dwResultEnum != ERROR_NO_MORE_ITEMS);  
		if (myParams->m_NetTree) myParams->m_NetTree->SetRedraw(true);

		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetCloseEnum: begin...\n");
		DWORD dwResult = WNetCloseEnum(myParams->hEnum);  
		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "WNetCloseEnum: ...end\n");
		if(dwResult != NO_ERROR)
		{
			SetClassLong(myParams->m_NetTree->m_hWnd,GCL_HCURSOR,oldc);
			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "aborted: WNetCloseEnum returned an error.\n");
			return FALSE;
		}

		if (myParams->m_NetTree)
		{
			CString srcItem;
			int myImage, myselImage;

			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "(c) inside the setimage\n");

			if (myParams->hParent==TVI_ROOT)
				myParams->hParent=myParams->m_NetTree->GetRootItem();
			else
				myParams->hParent=myParams->m_NetTree->GetChildItem(myParams->hParent);

			myParams->hParentDefDominio=NULL;
			do
			{
				srcItem=myParams->m_NetTree->GetItemText(myParams->hParent);

				myParams->m_NetTree->GetItemImage(myParams->hParent, myImage, myselImage);
				if (myImage==NB_NUMPICS)
					myParams->m_NetTree->SetItemImage(myParams->hParent,2,2);

				if (srcItem.CompareNoCase(*myParams->m_UserDomain)==0)
					myParams->hParentDefDominio=myParams->hParent;

				for (int k=0; k<myParams->m_UsersSelected->GetSize();k++)
				{
					if (myParams->m_UsersSelected->GetAt(k)==srcItem)
					{
						myParams->m_NetTree->GetItemImage(myParams->hParent, myImage, myselImage);
						myParams->m_NetTree->SetItemImage(myParams->hParent,myImage+NB_NUMPICS,myselImage+NB_NUMPICS);
					}
				}

				myParams->hParent=myParams->m_NetTree->GetNextItem(myParams->hParent,TVGN_NEXT);
			} while (myParams->hParent!=NULL);

			if (myParams->lpnr!=0)
			{	// the special MSG_RPNETWORKBROWSERPING requests
				// infos to remote rp client (mainly for logged
				// username and away state)

				CMessaggio pingMsg;
				pingMsg.Dest = myParams->lpnr->lpRemoteName;
				pingMsg.Msg = MSG_RPNETWORKBROWSERPING;
				pingMsg.Version=((CRealPopupApp *)AfxGetApp())->env_Version;

				myParams->m_ProtocolsHandler->Ping(&pingMsg, NULL, true);
			}

			SetClassLong(myParams->m_NetTree->m_hWnd,GCL_HCURSOR,oldc);

			ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "(d) exited the setimage\n");

		}
	}

    // parent thread asked for abort (cancel or ok buttons)
	if (*myParams->netThreadAbort==WM_NETTHREADABORTED)
	{
		PostMessage(myParams->m_hWnd, WM_USER+WM_NETTHREADABORTED,0,0);
		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "thread aborted as requested (WM_NETTHREADABORTED sent)\n");
	}
	else if (*myParams->netThreadAbort==WM_NETTHREADABORTEDOK)
	{
		PostMessage(myParams->m_hWnd, WM_USER+WM_NETTHREADABORTEDOK,0,0);
		ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "thread aborted as requested (OK button pressed, WM_NETTHREADABORTEDOK sent)\n");
	}

	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "ThreadUsersScan", "thread finished\n");

	return TRUE; 
}

bool CRPNetworkBrowser::UserInserted(CString item)
{
	for (int q=0;q<m_UsersInserted.GetSize();q++)
	{
		if (m_UsersInserted.GetAt(q).CompareNoCase(item)==0)
			return true;
	}

	return false;
}

BOOL CRPNetworkBrowser::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_rpApp=(CRealPopupApp *)AfxGetApp();

	HICON TrayIcon = m_rpApp->LoadIcon(IDR_TRAYICON);
	SetIcon(TrayIcon, TRUE);

	myRPLogo.LoadBitmap(IDB_BITMAP_RPLOGO);

	m_ProtocolsHandler->m_NetworkBrowser=this;

	CBitmap bmpNetTree;

	CDC *mydc=this->GetDC();
	int caps=mydc->GetDeviceCaps(BITSPIXEL);
	this->ReleaseDC(mydc);

	if (caps>8)
	{
		bmpNetTree.LoadBitmap(IDB_NETTREEHI);
		m_NetTreeImages.Create(16,16,ILC_COLOR32|ILC_MASK, 10, 0);
	}
	else
	{
		bmpNetTree.LoadBitmap(IDB_NETTREE);
		m_NetTreeImages.Create(16,16,ILC_COLOR8|ILC_MASK, 10, 0);
	}

	m_NetTreeImages.Add(&bmpNetTree,(COLORREF) 0x00808000);
	m_NetTree.SetImageList(&m_NetTreeImages, TVSIL_NORMAL);

	// fills up 'Favorite users' item

		// a. adds WorkstationsRank list
		TV_INSERTSTRUCT TreeCtrlItem;
		CString rpusers, itemWorkstationRank;

		rpusers.LoadString(MSG_NBROWSER_RPUSERS);

		TreeCtrlItem.item.pszText = (char *) LPCTSTR(rpusers);
		TreeCtrlItem.hParent = NULL;
		TreeCtrlItem.hInsertAfter = NULL;
		TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TreeCtrlItem.item.lParam = NULL;
		TreeCtrlItem.item.iImage=4;
		TreeCtrlItem.item.iSelectedImage=4;
		HTREEITEM myRPUsers = m_NetTree.InsertItem(&TreeCtrlItem);

		if (m_rpApp->opz_chkNBDetermineFavorites)
		{
			for (int i=0; i<m_rpApp->oWorkstationsRank.GetSize(); i++)
			{
				itemWorkstationRank=m_rpApp->oWorkstationsRank.GetAt(i);
				TreeCtrlItem.item.pszText = itemWorkstationRank.GetBuffer();
				TreeCtrlItem.hParent = myRPUsers;
				TreeCtrlItem.hInsertAfter = TVI_SORT;
				TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				TreeCtrlItem.item.lParam = NULL;
				TreeCtrlItem.item.iImage=0;
				TreeCtrlItem.item.iSelectedImage=0;
				HTREEITEM myItem = m_NetTree.InsertItem(&TreeCtrlItem);
				itemWorkstationRank.ReleaseBuffer();
				m_UsersInserted.Add(itemWorkstationRank);

				CMessaggio pingMsg;
				pingMsg.Dest = m_rpApp->oWorkstationsRank.GetAt(i);
				pingMsg.Msg = MSG_RPNETWORKBROWSERPING;
				m_ProtocolsHandler->Ping(&pingMsg, NULL, true);		
			}
		}

		// b. adds options->users and groups items marked with IDC_CHK_NBINCLUDEFAVORITES
		for (int i=0; i<m_ProtocolsHandler->m_Users->GetCount(); i++)
		{
			SUser *item=m_ProtocolsHandler->m_Users->GetAt(i);
			CString members=item->name, workstation;
			SUser *itemProfile=m_ProtocolsHandler->m_Users->GetProfile(&members);
			
			if ( !item->AllUsers & itemProfile->chkIncludeFavorites )
			{
				if (item->group && !itemProfile->includeAsGroup)
					members=item->members;

				for (int k=0; AfxExtractSubString(workstation, members, k, (char) ','); k++)
				{
					if ( !UserInserted(workstation) && workstation.Find('[')==-1)
					{
						TreeCtrlItem.item.pszText = (char *) LPCTSTR(workstation);
						TreeCtrlItem.hParent = myRPUsers;
						TreeCtrlItem.hInsertAfter = TVI_SORT;
						TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
						TreeCtrlItem.item.lParam = NULL;
						TreeCtrlItem.item.iImage=0;
						TreeCtrlItem.item.iSelectedImage=0;
						m_UsersInserted.Add(workstation);

						if (itemProfile->group && item->includeAsGroup)
						{
							TreeCtrlItem.item.iImage=2;
							TreeCtrlItem.item.iSelectedImage=2;
							HTREEITEM myItem = m_NetTree.InsertItem(&TreeCtrlItem);
						}
						else
						{
							TreeCtrlItem.item.iImage=0;
							TreeCtrlItem.item.iSelectedImage=0;
							HTREEITEM myItem = m_NetTree.InsertItem(&TreeCtrlItem);
			
							CMessaggio pingMsg;
							pingMsg.Dest = workstation;
							pingMsg.Msg = MSG_RPNETWORKBROWSERPING;
							m_ProtocolsHandler->Ping(&pingMsg, NULL, true);
						}
					}
				}
			}
		}


	// network domains scan
	myTUSparams.m_NetTree=&this->m_NetTree;
	myTUSparams.servers=&this->servers;
	myTUSparams.lpnrAllocated=&this->lpnrAllocated;
	myTUSparams.m_ProtocolsHandler=this->m_ProtocolsHandler;
	myTUSparams.m_UserDomain=this->m_UserDomain;
	myTUSparams.m_UsersSelected=&this->m_UsersSelected;
	myTUSparams.lpnr=NULL;
	myTUSparams.hParent=TVI_ROOT;
	myTUSparams.EnumServers=true;
	myTUSparams.netThreadAbort=&this->netThreadAbort;
	myTUSparams.ResetThreadId=false;
	myTUSparams.m_hWnd=this->m_hWnd;
	myTUSparams.InviaEndMsg=true;
	myTUSparams.ValidStruct=STRUTTURAVALIDA;
	myTUSparams.inUse=false;

	netThreadAbort=0;
	netThread=AfxBeginThread(ThreadFillTree, &myTUSparams, NULL, NULL, CREATE_SUSPENDED, NULL);
	netThread->m_bAutoDelete=false;
	netThread->ResumeThread();

	SF_NoOKDisable=false;
	
	MoveWindow(m_parentRect);
	m_NetTree.SelectItem(NULL);

#ifdef USASETFONT
	CWnd *mywnd=GetTopWindow();
	while (mywnd!=NULL)
	{
		mywnd->SetFont(&m_rpApp->font_System,true);
		mywnd=mywnd->GetNextWindow(GW_HWNDNEXT);
	}
#endif

	if (netThreadAbort!=0)
		return FALSE;

	return TRUE;
}

void CRPNetworkBrowser::OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (SF_DoubleClickPressed)
		return;

	if (IsThreadRunning(netThread))
	{
		*pResult = true;
		return;
	}
	
	if (pNMTreeView->itemNew.lParam==NULL)
		return;

	if (pNMTreeView->action==TVE_EXPAND)
	{	// scans for domains or workstations under expanded item

		myTUSparams.m_NetTree=&m_NetTree;
		myTUSparams.servers=&servers;
		myTUSparams.lpnrAllocated=&lpnrAllocated;
		myTUSparams.m_ProtocolsHandler=m_ProtocolsHandler;
		myTUSparams.m_UserDomain=m_UserDomain;
		myTUSparams.m_UsersSelected=&m_UsersSelected;

		myTUSparams.lpnr=(LPNETRESOURCE)pNMTreeView->itemNew.lParam;
		myTUSparams.hParent=pNMTreeView->itemNew.hItem;
		myTUSparams.EnumServers=false;

		myTUSparams.netThreadAbort=&netThreadAbort;
		myTUSparams.ResetThreadId=true;
		myTUSparams.m_hWnd=m_hWnd;

		netThread=AfxBeginThread(ThreadUsersScan,(LPVOID)&myTUSparams, NULL, NULL, CREATE_SUSPENDED, NULL);
		netThread->m_bAutoDelete=false;
		netThread->ResumeThread();
	}
	else
	{
		while (m_NetTree.ItemHasChildren(pNMTreeView->itemNew.hItem))
			m_NetTree.DeleteItem(m_NetTree.GetChildItem(pNMTreeView->itemNew.hItem));

		TV_INSERTSTRUCT TreeCtrlItem;

		TreeCtrlItem.hParent = pNMTreeView->itemNew.hItem;
		TreeCtrlItem.hInsertAfter = TVI_SORT;
		TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
		TreeCtrlItem.item.pszText = "CodeByMatro"; // :-)
		TreeCtrlItem.item.lParam = 0;
		m_NetTree.InsertItem(&TreeCtrlItem);

	}
	*pResult = 0;
}

void CRPNetworkBrowser::Pong(CString *pongUser, CString *pongMsg, CString *pongVersion, HTREEITEM myItem)
{	// remote rp client replied to MSG_RPNETWORKBROWSERPING with its infos

	CString	srcItem;
	int myImage, myselImage;

	if (!(::IsWindow(m_NetTree.m_hWnd)))
		return;

	if (myItem==TVI_ROOT)
		myItem=m_NetTree.GetRootItem();
	else
		myItem=m_NetTree.GetChildItem(myItem);
	
	do
	{
		srcItem=m_NetTree.GetItemText(myItem);
		if (pongUser->Compare(srcItem)==0)
		{
			m_NetTree.GetItemImage(myItem,myImage,myselImage);

			if (GetRPVersion(pongVersion)>=1.577)	// previous rp releases doesn't
			{										// provide required infos
				CString sAway, sUser;
				AfxExtractSubString(sAway, *pongMsg, 0, '|');
				AfxExtractSubString(sUser, *pongMsg, 3, '|');
				sUser.MakeLower();

				if (atoi(sAway)>0)
					m_NetTree.SetItemImage(myItem,myImage+3,myselImage+3);
				else
					m_NetTree.SetItemImage(myItem,myImage+1,myselImage+1);

				m_NetTree.SetItemText(myItem,srcItem + " (" + sUser + ")");
			}
			else
				m_NetTree.SetItemImage(myItem,myImage+1,myselImage+1);
		}

		if (m_NetTree.ItemHasChildren(myItem))
			Pong(pongUser,pongMsg,pongVersion,myItem);

		myItem=m_NetTree.GetNextItem(myItem,TVGN_NEXT);
	} while (myItem!=NULL);
}

void CRPNetworkBrowser::AddSelectedUser()
{

	HTREEITEM myItem=m_NetTree.GetSelectedItem();
	int myImage, myselImage;

	if (myItem!=NULL)
	{
		int pos;
		m_NetTree.GetItemImage(myItem,myImage,myselImage);

		if (myImage==4)
			return;

		if (myImage<NB_NUMPICS && ( (m_UsersSelected.GetSize()==0 && m_SoloUno) || !m_SoloUno) )
		{
			CString myUser;
			m_NetTree.SetItemImage(myItem,myImage+NB_NUMPICS,myselImage+NB_NUMPICS);
			myUser=m_NetTree.GetItemText(myItem);
			pos=myUser.Find('(');
			if (pos>-1)
				myUser=myUser.Left(pos-1);
			m_UsersSelected.Add(myUser);
		}

		if (myImage>=NB_NUMPICS)
		{
			CString myUser;
			m_NetTree.SetItemImage(myItem,myImage-NB_NUMPICS,myselImage-NB_NUMPICS);
			myUser=m_NetTree.GetItemText(myItem);
			pos=myUser.Find('(');
			if (pos>-1)
				myUser=myUser.Left(pos-1);
			for (int i=0; i<m_UsersSelected.GetSize(); i++)
			{
				if (m_UsersSelected.GetAt(i).Compare(myUser)==0)
				{
					m_UsersSelected.RemoveAt(i);
					if (!SF_NoOKDisable && m_UsersSelected.GetSize()==0)
						GetDlgItem(IDOK)->EnableWindow(FALSE);
				}
			}
		}
	}

}

void CRPNetworkBrowser::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (pNMTreeView->action==TVC_BYMOUSE)
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		AddSelectedUser();
		m_NetTree.SelectItem(NULL);

		HTREEITEM myItem=pNMTreeView->itemNew.hItem;
		if (!m_NetTree.ItemHasChildren(myItem))
			myItem=m_NetTree.GetParentItem(myItem);
		CString myDomain=m_NetTree.GetItemText(myItem);
		*m_UserDomain=myDomain;
	}

	int myImage, myselImage;
	m_NetTree.GetItemImage(pNMTreeView->itemNew.hItem,myImage,myselImage);
	CString myDomain=m_NetTree.GetItemText(pNMTreeView->itemNew.hItem);
	
	*pResult = 0;
}

void CRPNetworkBrowser::OnOK() 
{
	for (int k=0; k<m_UsersSelected.GetSize(); k++)
	{
		if (m_UsersSelectedString->GetLength()>0)
			*m_UsersSelectedString+=",";
		*m_UsersSelectedString+=m_UsersSelected[k];
	}

	m_ProtocolsHandler->PendingPurge();

	if (IsThreadRunning(netThread))
	{
		netThreadAbort=2;
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::OnOK", "ThreadUsersScan abort requested (user pressed OK)\n");
		return;
	}

	CDialog::OnOK();
}

void CRPNetworkBrowser::OnHelp()
{
	m_rpApp->ShowHelp(this, "#network");
}

void CRPNetworkBrowser::OnSize(UINT nType, int cx, int cy) 
{

	CDialog::OnSize(nType, cx, cy);
	
    if (GetDlgItem(IDC_TREE1) != NULL)
    {
        CRect rcClient, rcClientNow;
        GetClientRect(rcClient);

        CWnd *pIDC_TREE1 =		GetDlgItem(IDC_TREE1);
        CWnd *pIDOK =			GetDlgItem(IDOK);    
        CWnd *pIDCANCEL =		GetDlgItem(IDCANCEL);    
        CWnd *pID_HELP =		GetDlgItem(ID_HELP);   

        CRect rcIDC_TREE1;		pIDC_TREE1->GetWindowRect(rcIDC_TREE1);				ScreenToClient(rcIDC_TREE1);
        CRect rcIDOK;			pIDOK->GetWindowRect  (rcIDOK);						ScreenToClient(rcIDOK);
        CRect rcIDCANCEL;		pIDCANCEL->GetWindowRect(rcIDCANCEL);				ScreenToClient(rcIDCANCEL);
        CRect rcID_HELP;		pID_HELP->GetWindowRect(rcID_HELP);					ScreenToClient(rcID_HELP);
        
		int BtnAsseX=rcClient.Width()-rcIDOK.Width()-10;

		pIDOK->MoveWindow(BtnAsseX,9,rcIDOK.Width(), rcIDOK.Height(), false);
		pIDCANCEL->MoveWindow(BtnAsseX,39,rcIDCANCEL.Width(), rcIDCANCEL.Height(), false);

		pIDC_TREE1->MoveWindow(8,9, BtnAsseX-14, rcClient.Height()-14, false);
        pIDC_TREE1->GetWindowRect(rcIDC_TREE1); ScreenToClient(rcIDC_TREE1);

		pID_HELP->MoveWindow(BtnAsseX,rcIDC_TREE1.Height()-rcID_HELP.Height()+8,rcID_HELP.Width(), rcID_HELP.Height(), false);
        pID_HELP->GetWindowRect(rcID_HELP); ScreenToClient(rcID_HELP);

		RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_ERASE );
	}
	
}

void CRPNetworkBrowser::OnKeydownTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

	if (pTVKeyDown->wVKey==32)
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		AddSelectedUser();
	}
	
	*pResult = 0;
}

void CRPNetworkBrowser::OnSetfocusTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_NetTree.SelectItem(NULL);
	
	*pResult = 0;
}

UINT ThreadFillTree(LPVOID pParam)
{

	sThreadUsersScan *myTUSparams=(sThreadUsersScan *) pParam;

	ThreadUsersScan((LPVOID)myTUSparams);

	if (*myTUSparams->netThreadAbort!=0)
	{
		myTUSparams->ValidStruct=NULL;
		return 0;
	}

	myTUSparams->EnumServers=false;

	for (int k=0; k<myTUSparams->servers->GetSize(); k++)
	{
		myTUSparams->lpnr=(LPNETRESOURCE)myTUSparams->servers->GetAt(k);
		myTUSparams->hParent=TVI_ROOT;
		ThreadUsersScan((LPVOID)myTUSparams);
		if (myTUSparams->ValidStruct!=STRUTTURAVALIDA)
			return 0;
		if (*myTUSparams->netThreadAbort!=0)
		{
			myTUSparams->ValidStruct=NULL;
			return 0;
		}
	}

	if (myTUSparams->InviaEndMsg)
		PostMessage(myTUSparams->m_hWnd, WM_USER+WM_NETTHREADENDED,0,0);

	return 0;
}


BOOL CRPNetworkBrowser::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message==WM_USER+WM_NETTHREADABORTED)
	{
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::PreTranslateMessage", "WM_NETTHREADABORTED received\n");
		if (netThreadAbort==1)
		{
			m_ProtocolsHandler->PendingPurge();
			EndDialog(0);
			return -1;
		}
	}
	if (pMsg->message==WM_USER+WM_NETTHREADABORTEDOK)
	{
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::PreTranslateMessage", "WM_NETTHREADABORTEDOK received\n");
		if (netThreadAbort==2)
		{
			m_ProtocolsHandler->PendingPurge();
			EndDialog(IDOK);
			return -1;
		}
	}

	if (pMsg->message==WM_USER+WM_NETTHREADENDED)
	{
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::PreTranslateMessage", "WM_NETTHREADENDED received\n");
		if (myTUSparams.hParentDefDominio!=NULL && netThreadAbort==0)
		{
			m_NetTree.Expand(myTUSparams.hParentDefDominio,TVE_EXPAND);
			m_NetTree.EnsureVisible(myTUSparams.hParentDefDominio);
		}
		return -1;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CRPNetworkBrowser::OnCancel()
{
	if (IsThreadRunning(netThread))
	{	
		netThreadAbort=1;
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPNetworkBrowser::OnCancel", "ThreadUsersScan abort requested (user pressed Cancel)\n");
		return;
	}

	CDialog::OnCancel();
}


void CRPNetworkBrowser::OnPaint() 
{
	CPaintDC dc(this);
	
    CWnd *pID_HELP = GetDlgItem(ID_HELP);   
    CRect rcID_HELP; pID_HELP->GetWindowRect(rcID_HELP); ScreenToClient(rcID_HELP);

	myRPLogo.DrawTransparent(&dc,rcID_HELP.left + 2,rcID_HELP.top-42,(COLORREF) 0x0000ffff);

	// Do not call CDialog::OnPaint() for painting messages
}

void CRPNetworkBrowser::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (GetDlgItem(IDOK)->IsWindowEnabled())
	{
		SF_DoubleClickPressed=true;
		OnOK();
	}
	*pResult = 0;
}
