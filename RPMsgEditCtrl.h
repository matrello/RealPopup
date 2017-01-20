/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_RPMSGEDITCTRL_H__EB67C183_C423_11D1_BC98_D58F3C31F152__INCLUDED_)
#define AFX_RPMSGEDITCTRL_H__EB67C183_C423_11D1_BC98_D58F3C31F152__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include <richole.h>

#include "RichEditCtrlEx.h"

#define C_LINK_NONE    -1
#define C_LINK_GENERIC 0
#define C_LINK_HTTP    1
#define C_LINK_EMAIL   2
#define C_LINK_PATH	   3

class CRPMsgEditCtrl : public CRichEditCtrlEx
{

	// Construction
public:
	CRPMsgEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRPMsgEditCtrl)
	//}}AFX_VIRTUAL

// Struttura interna
protected:
	struct SLink
	{
		long in;
		long fi;
		CString link;
	};

// Implementation
public:
	LPRICHEDITOLE m_lpRichEditOle;
	void SetFont(CFont *pFont, BOOL bRedraw = TRUE );
	CString * m_Browser;

	void	QuoteHandling(bool daDigitazione);
	void	QuoteMsg(bool forward=TRUE, bool answerOnTop=FALSE);
	virtual ~CRPMsgEditCtrl();

	CArray<SLink,SLink> Links;
	CString ForwardInfo;

	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Generated message map functions
protected:
	void HyperlinkJump(CPoint *point);
	void HyperlinkHandling(char *bufText, long ZinizPar, long ZfinePar);
	void FindIniziFinePar(char *bufText, FINDTEXTEX * ft, long * inParagrafo, long * fiParagrafo, long caretPos);
	void LinksShift();
	int LinkFromPoint(CPoint *point);

	bool m_Shift;

	//{{AFX_MSG(CRPMsgEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );

	DECLARE_MESSAGE_MAP()

// Interface Map
public:
	BEGIN_INTERFACE_PART(RichEditOleCallback, IRichEditOleCallback)
		INIT_INTERFACE_PART(CRPMsgEditCtrl, RichEditOleCallback)
		STDMETHOD(GetNewStorage) (LPSTORAGE*);
		STDMETHOD(GetInPlaceContext) (LPOLEINPLACEFRAME*,
									  LPOLEINPLACEUIWINDOW*,
									  LPOLEINPLACEFRAMEINFO);
		STDMETHOD(ShowContainerUI) (BOOL);
		STDMETHOD(QueryInsertObject) (LPCLSID, LPSTORAGE, LONG);
		STDMETHOD(DeleteObject) (LPOLEOBJECT);
		STDMETHOD(QueryAcceptData) (LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
		STDMETHOD(ContextSensitiveHelp) (BOOL);
		STDMETHOD(GetClipboardData) (CHARRANGE*, DWORD, LPDATAOBJECT*);
		STDMETHOD(GetDragDropEffect) (BOOL, DWORD, LPDWORD);
		STDMETHOD(GetContextMenu) (WORD, LPOLEOBJECT, CHARRANGE*, HMENU*);
	END_INTERFACE_PART(RichEditOleCallback)

	DECLARE_INTERFACE_MAP()

private:
	int fontY;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RPMSGEDITCTRL_H__EB67C183_C423_11D1_BC98_D58F3C31F152__INCLUDED_)

