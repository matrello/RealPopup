/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_RPNETWORKBROWSER_H__8EF54141_EB4A_11D1_BEDD_0060087A5B02__INCLUDED_)
#define AFX_RPNETWORKBROWSER_H__8EF54141_EB4A_11D1_BEDD_0060087A5B02__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RPProtocolsHandler.h"
#include "MaskedBitmap.h"

UINT ThreadFillTree(LPVOID pParam);

#define STRUTTURAVALIDA 0x88328048

struct sThreadUsersScan
{
	// ThreadUsersScan parameters
	HWND			m_hWnd;
	CTreeCtrl		*m_NetTree;
	CDWordArray		*servers;
	CDWordArray		*lpnrAllocated;
	CRPProtocolsHandler	*m_ProtocolsHandler;
	CString			*m_UserDomain;
	CStringArray	*m_UsersSelected;

	int				*netThreadAbort;
	bool			ResetThreadId;

	long			ValidStruct;
	bool			inUse;

	LPNETRESOURCE	lpnr;
	HTREEITEM		hParent;
	HTREEITEM		hParentDefDominio;
	bool			EnumServers;

	CStringArray	*m_domini;
	bool			InviaEndMsg;

	LPNETRESOURCE   lpnrLocal;
	DWORD			cbBuffer;
	DWORD			cEntries;
	DWORD			dwResult;
	DWORD			dwResultEnum;
	HANDLE			hEnum;
	int				wnetType;
};

class CRPNetworkBrowser : public CDialog
{
// Construction
public:
	CWinThread *netThread;
	int netThreadAbort;
	CDWordArray servers;
	CRect m_parentRect;
	bool m_SoloUno;
	CStringArray m_UsersSelected;
	CString      *m_UsersSelectedString;
	CString		 *m_UserDomain;
	
	CRPNetworkBrowser(CWnd* pParent = NULL);
	~CRPNetworkBrowser();

    CRPProtocolsHandler *m_ProtocolsHandler;

    void Pong(CString *pongUser, CString *pongMsg,  CString *pongVersion, HTREEITEM myItem=TVI_ROOT);

// Dialog Data					
	//{{AFX_DATA(CRPNetworkBrowser)
	enum { IDD = IDD_DLG_NETWORK };
	CTreeCtrl	m_NetTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRPNetworkBrowser)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool UserInserted(CString item);
	void AddSelectedUser();

	sThreadUsersScan myTUSparams;
	bool SF_NoOKDisable;
	bool SF_DoubleClickPressed;
	CImageList m_NetTreeImages;
	CMaskedBitmap myRPLogo;
	CStringArray m_UsersInserted;
	CDWordArray lpnrAllocated;

	// Generated message map functions
	//{{AFX_MSG(CRPNetworkBrowser)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeydownTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnHelp();

private:
	CRealPopupApp	*m_rpApp;

public:
	afx_msg void OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RPNETWORKBROWSER_H__8EF54141_EB4A_11D1_BEDD_0060087A5B02__INCLUDED_)
