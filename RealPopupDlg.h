/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_REALPOPUPDLG_H__08B63E87_B76B_11D1_BEDC_0060087A5B02__INCLUDED_)
#define AFX_REALPOPUPDLG_H__08B63E87_B76B_11D1_BEDC_0060087A5B02__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RPUsers.h"
#include "TbStDialog.h"
#include "RPMsgEditCtrl.h"
#include "SystemTray.h"

#define WM_ICON_NOTIFY   WM_USER+10
#define TRAYFLASH_OFF	 -1
#define TRAYFLASH_ON	 -2
#define TRAYFLASH_UPDREQ -3
#define TRAYFLASH_DELAY	 16

#define BALLOONNOTIFY_MININTERVAL  15
#define BALLOONNOTIFY_PREVIEWCHARS 160

#define MAX_SENDDLG	20

class CRealPopupDlg : public CTbStDialog
{
// Construction
public:
	CRealPopupDlg(CWnd* pParent = NULL);
	~CRealPopupDlg();
	void OnAfterSendOK(int MsgDaEliminare, UINT nID);
	void HotKeySet();
	void SetFonts();
	int MsgAttuale;
	int oldMsgAttuale;
	void MsgVisualizza(bool bForza=false);
	CSystemTray	m_TrayIcon;
	int  m_TrayFlashGreen;

	// max 20 open Send dialogs; the array contains dialogs pointers
	// to destroy instances once rp terminates. whenever a Send dialog
	// has to be opened, the array is scanned for closed Send dialogs;
	// whenever one is found, it will be destroyed and array item reused.
	// last destroyed Send dialog reference will activate 'delete original
	// message when replied' and 'minimize when replied' options.
	CSendDlg *SendDlg[20];
	CSendDlg *SendDlgLast;
	int senddlgAct;

	CWinThread *netThread;
	int netThreadAbort;
	CDWordArray servers;
	sThreadUsersScan myTUSparams;

	// Dialog Data
	//{{AFX_DATA(CRealPopupDlg)
	enum { IDD = IDD_DLG_MAIN };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealPopupDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int  m_BalloonNotifyDelay;
	bool m_BalloonNotifyRequest;

	void myShowWindow(bool mode);
	int	delayToolbarActivate;
	void MsgSave(CMessaggio *msg);
	void MsgToolTipUpd();
	int MsgScan(int *popup);
	BOOL m_TrayFlash;
	void Options();
	bool IsAwayEnabled;
	bool m_AwayByScreenSaver;
	bool m_RedirectEnabled;
	bool m_OptionsOpen;
	bool m_Show;
	UINT m_hTimer;
	HICON				m_hIcon;
	CRPProtocolsHandler	m_ProtocolsHandler;
	CRPMsgEditCtrl		m_MsgEdit;
	CRPUsers			m_Users;
	HACCEL				m_hAccelTable;

	CDWordArray lpnrAllocated;

	CStatic			*m_LblFrom;
	CStatic			*m_LblTo;
	CEdit			*m_FldFrom;
	CEdit			*m_FldTo;
	CEdit			*m_FldInfo;

	// Generated message map functions
	//{{AFX_MSG(CRealPopupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBtnCopy();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
    void OnBtnNetwork();
	void OnBtnTlbNuovo(UINT nID);
	void OnBtnTlbMessaggio(UINT nID);
	void OnBtnTlbOther(UINT nID);
	void OnUpdateToolbarMenu(CCmdUI* pCmdUI);
	void OnUpdateStatusPanes(CCmdUI* pCmdUI);
	void OnTrayMenu(UINT nID);
	void OnUpdateTrayMenu(CCmdUI* pCmdUI);
	void OnEndSession(BOOL bEnding);
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	LRESULT OnPowerBroadcast(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void CtrlPosiziona(bool noresize=false);

public:
	void BalloonNotify(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALPOPUPDLG_H__08B63E87_B76B_11D1_BEDC_0060087A5B02__INCLUDED_)
