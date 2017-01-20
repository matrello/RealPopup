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
#include "RPMsgEditCtrl.h"

#include <richedit.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRPMsgEditCtrl::CRPMsgEditCtrl()
{
}

CRPMsgEditCtrl::~CRPMsgEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CRPMsgEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRPMsgEditCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DROPFILES()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRPMsgEditCtrl::OnDestroy()
{
	if (m_lpRichEditOle != NULL)
		m_lpRichEditOle->Release();
}

int CRPMsgEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetOLECallback(&m_xRichEditOleCallback);
	m_lpRichEditOle = GetIRichEditOle();

	if (!(GetStyle()&ES_READONLY))
		DragAcceptFiles();

	CFont* pFont = GetParent()->GetFont();
	LOGFONT lf;
	pFont->GetObject(sizeof(LOGFONT), &lf);

	lf.lfCharSet=ANSI_CHARSET;
	SetFont(pFont);

	return 0;
}

void CRPMsgEditCtrl::QuoteMsg(bool forward, bool answerOnTop)
{
	if (GetTextLength()>0)
	{
		CString txtMsg, txtQuotedMsg;
		GETTEXTEX gt;
		int pos;

		gt.codepage = CP_ACP;
		gt.flags=GT_DEFAULT;
		gt.lpDefaultChar=NULL;
		gt.lpUsedDefChar=NULL;
		gt.cb=GetTextLength();
		SendMessage(EM_GETTEXTEX,(WPARAM)&gt, (LPARAM)txtMsg.GetBuffer(GetTextLength()));
		txtMsg.ReleaseBuffer(GetTextLength());
		pos=txtMsg.Find((TCHAR)0,0);

		if (pos>0 && pos<GetTextLength())
			txtMsg=txtMsg.Left(pos);

		if (forward)
			txtQuotedMsg="\n\n-----\n"+ForwardInfo;
		else
		{
			if (answerOnTop)
				txtQuotedMsg="\n\n";
		}

		if (!forward)
		{
			txtMsg.Insert(0,">");
			Normalizza0D(&txtMsg);
			txtMsg.Replace("\r","\r>");
			if (txtMsg.Right(2)=="\r>")
				txtMsg=txtMsg.Left(txtMsg.GetLength()-1);
			if (txtMsg.Right(1)!="\r")
				txtMsg.Append("\r");
		}

		txtQuotedMsg.Append(txtMsg);
		SetWindowText(txtQuotedMsg);
		QuoteHandling(false);
		
		if (!forward && !answerOnTop)
			SetSel(GetTextLength(),GetTextLength());
	}
}

void CRPMsgEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	CRichEditCtrl::OnKeyUp(nChar, nRepCnt, nFlags);

	if (nChar == 16)
	{
		m_Shift = FALSE;
		return;
	}

	switch (nChar)
	{
	case 45:
	case 46:
	case 13:
		QuoteHandling(false);
		break;
	}

}

void CRPMsgEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	if (nChar != 27)
		CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == 16)
	{
		m_Shift = TRUE;
		return;
	}

}

void CRPMsgEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case 0x22:
	case '(':
	case ')':
	case ' ':
	case '.':
	case ',':
	case '?':
	case ':':
	case ';':
	case '@':
	case '/':
	case '\\':
		LinksShift();
		QuoteHandling(true);
		return;
		break;
	case '>':
	case 24:
	case 3:
	case 22:
		QuoteHandling(false);
		break;
	default:
		LinksShift();
	}

}

void CRPMsgEditCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{

	if (LinkFromPoint(&point)!=-1)
	{
		HICON cur = AfxGetApp()->LoadCursor(IDC_CURSOR_LINK);
		::SetCursor(cur);
	}	

	CRichEditCtrl::OnMouseMove(nFlags, point);
}


int CRPMsgEditCtrl::LinkFromPoint(CPoint *point)
{
	CRect rt[3], winRect;
	CPoint in, fi;
	int scost;

	GetClientRect(winRect);

	for (int k=0;k<Links.GetSize();k++)
	{
		in=GetCharPos(Links[k].in);
		fi=GetCharPos(Links[k].fi);
		scost=fi.y-in.y;

		if (in.y==fi.y)
		{
			rt[0].SetRect(in,fi);
			rt[0].bottom+=fontY;
			if (rt[0].PtInRect(*point))
				return k;
		}
		else
		{
			rt[0].SetRect(in.x,in.y,(winRect.right),in.y+fontY);
			rt[1].SetRect(0,fi.y,fi.x,fi.y+fontY);
			rt[2].SetRect(0,in.y+fontY,winRect.right,fi.y-1);

			if (rt[0].PtInRect(*point) || rt[1].PtInRect(*point) || rt[2].PtInRect(*point))
				return k;

		}
	}

	return -1;
}

void CRPMsgEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ( !((CRealPopupApp *)AfxGetApp())->opz_chkDblClick )
		HyperlinkJump(&point);
	
	CRichEditCtrl::OnLButtonDown(nFlags, point);
}

void CRPMsgEditCtrl::LinksShift()
{
	CHARRANGE cr;
	GetSel(cr);

	for (int k=0;k<Links.GetSize();k++)
	{
		if (Links[k].in >= cr.cpMin)
		{
			Links[k].in+=1;
			Links[k].fi+=1;
		}
	}
}

void CRPMsgEditCtrl::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRichEditCtrl::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	static bool sredraw=FALSE;
	static int prevLineCount=GetLineCount();

	if (GetLineCount()!=prevLineCount)
	{
		CRect myrect;
		GetClientRect(myrect);

		// this is a really dirty way... :)
		bool redraw=((myrect.Height()/14)<GetLineCount());
		if (redraw!=sredraw)
		{
			sredraw=redraw;
			SetWindowPos(NULL, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
		}
	}
	
}

void CRPMsgEditCtrl::QuoteHandling(bool daDigitazione)
{	

	char *bufText;
	char src[2]={13,0};

	CHARFORMAT cf;
	memset(&cf,0,sizeof(CHARFORMAT));
	cf.dwMask = CFM_COLOR;

	CHARRANGE cr;
	memset(&cr,0,sizeof(CHARRANGE));

	GetSel(cr);

	FINDTEXTEX ft;
	memset(&ft,0,sizeof(FINDTEXTEX));

	long pos=0, epos=-1;

	bufText = (char *) calloc(GetTextLength() + (GetLineCount() * 3 * 2) + 10,1);

	GETTEXTEX mygt;
	mygt.cb=GetTextLength();
	mygt.flags=GT_DEFAULT;
	mygt.codepage=CP_ACP;
	mygt.lpDefaultChar=NULL;
	mygt.lpUsedDefChar=NULL;
	SendMessage(EM_GETTEXTEX,(WPARAM)&mygt,(LPARAM)bufText);

	if (daDigitazione)
	{
		int q=0;

		FindIniziFinePar(bufText, &ft, &pos, &epos, cr.cpMin);

		while (q<Links.GetSize())
		{
			if ( (Links[q].in >= pos) && 
				(Links[q].fi <= epos) )
				Links.RemoveAt(q);
			else
				q++;
		}
	}
	else
		Links.RemoveAll();

	SetOptions(ECOOP_XOR,ES_AUTOVSCROLL | ECO_AUTOHSCROLL );
	HideSelection(TRUE,FALSE);

	SCROLLINFO myScrollInfo;
	myScrollInfo.fMask=SIF_POS | SIF_RANGE | SIF_PAGE;
	GetScrollInfo(SB_VERT,&myScrollInfo);

	char *bufParStart = bufText+pos;
	char *bufParEnd = bufText+epos;;
	
	if (daDigitazione)
		bufParEnd=bufText + GetTextLength();

	do
	{
		bufParEnd=strchr(bufParStart,13);
		
		if (bufParEnd==NULL)
			bufParEnd=bufText+GetTextLength();

		SetSel(bufParStart-bufText, bufParEnd-bufText);
		if (*(bufParStart)==(char) '>')
		{
			cf.crTextColor = (COLORREF) 0x00ff0000;
		}
		else
			cf.crTextColor = GetSysColor(COLOR_WINDOWTEXT);

		SetSelectionCharFormat(cf);

		HyperlinkHandling(bufText, bufParStart-bufText, bufParEnd-bufText);
		
		bufParStart=bufParEnd+1;
	} while ( (bufParEnd<bufText+GetTextLength()) && (!daDigitazione));

	free(bufText);

	SetSel(cr);
	cf.crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	SetSelectionCharFormat(cf);
	
	SetOptions(ECOOP_OR,ES_AUTOVSCROLL | ECO_AUTOHSCROLL );
	HideSelection(FALSE,FALSE);

	if (myScrollInfo.nMax <= (myScrollInfo.nPos+(int)myScrollInfo.nPage))
		SendMessage(WM_VSCROLL,SB_BOTTOM,NULL);
}

void CRPMsgEditCtrl::FindIniziFinePar(char *bufText, FINDTEXTEX * ft, long * inParagrafo, long * fiParagrafo, long caretPos)
{
	char src[2]={13,0};
	int q=0;
	bool almenoUno=FALSE;

	ft->lpstrText = src;
	ft->chrg.cpMin=caretPos;
	ft->chrg.cpMax=-1;
	*fiParagrafo=FindText(FR_DOWN,ft);

	do
	{
		*inParagrafo=q;
		ft->chrg.cpMin=q+1;
		ft->chrg.cpMax=caretPos;
		q=FindText(FR_DOWN,ft);
		if (q!=-1) almenoUno=TRUE;
	} while (q!=-1);

	if (almenoUno) (*inParagrafo)++;
}

void CRPMsgEditCtrl::HyperlinkHandling(char *bufText, long ZinizPar, long ZfinePar)
{

	SLink nuovoLink;
	CHARFORMAT cf;
	memset(&cf,0,sizeof(CHARFORMAT));
	cf.dwMask = CFM_COLOR;

	char *inParagrafo = bufText + ZinizPar;
	char *fiParagrafo = bufText + ZfinePar;
	char *offset;
	char *k, *q;

	int ok;

	do
	{
		ok=C_LINK_NONE;

		// detect single word by the "stopchars"
		offset=inParagrafo;
		do
		{
			q=offset + strcspn(offset,"\x22\x0a\x0d !.,?;()");

			if (*q==0x22)	// anything inside quotes are treated as 'one word' 
			{
				q=strchr(q+1,0x22);
				if (q==NULL)
					q=fiParagrafo;
				inParagrafo++;
				break;
			}

			offset=q+1;
		} while (  ( strchr(".!,?;()",*q) != NULL ) &&
				   ( strchr("\x0a\x0d !,?;()",*(q+1)) == NULL ) );

		// email are detected through the '@'...
		k=strchr(inParagrafo,'@');
		if ( (k!=NULL) && (k<q) )
		{
			// ...through a dot between the '@' and the end...
			do
			{
				if (*(++k)=='.')
				{
					ok=C_LINK_EMAIL;
					// ... if last char is a dot, it's not
					// included in the email address 
					if (*(q-1)=='.') q--;
				}
			} while (k<q);
		}

		// all other link types...
		if ( (strstr(inParagrafo,"http://")==inParagrafo) ||
			 (strstr(inParagrafo,"news://")==inParagrafo) ||
			 (strstr(inParagrafo,"ftp://")==inParagrafo) ||
			 (strstr(inParagrafo,"https://")==inParagrafo) ||
			 (strstr(inParagrafo,"mk:@")==inParagrafo) ||
			 (strstr(inParagrafo,"file://")==inParagrafo) )
				 ok=C_LINK_GENERIC;
		else
		{
			if (strstr(inParagrafo,"www.")==inParagrafo)
				ok=C_LINK_HTTP;
			if (strstr(inParagrafo,"\\")==inParagrafo)
				ok=C_LINK_PATH;
		}

		if (ok!=C_LINK_NONE)
		{
			char *nuovoLinkText=(char *) calloc(q-inParagrafo+2,1);

			SetSel((long) (inParagrafo-bufText), (long) (q-bufText));
			cf.crTextColor = (COLORREF) 0x00008000;
			SetSelectionCharFormat(cf);

			nuovoLink.in=(long) (inParagrafo-bufText);
			nuovoLink.fi=(long) (q-bufText);

			switch (ok)
			{
			case C_LINK_EMAIL:
				nuovoLink.link = "mailto:";
				break;
			case C_LINK_HTTP:
				nuovoLink.link = *m_Browser;
				if (nuovoLink.link.GetLength() > 0)
					nuovoLink.link += "§";
				nuovoLink.link +=  "http://";
				break;
			case C_LINK_GENERIC:
				nuovoLink.link = *m_Browser;
				if (nuovoLink.link.GetLength() > 0)
					nuovoLink.link += "§";
				break;
			case C_LINK_PATH:
				nuovoLink.link = "";
				break;
			}

			nuovoLink.link += strncpy(nuovoLinkText,inParagrafo,(q-inParagrafo));
			Links.Add(nuovoLink);
			free(nuovoLinkText);
		}

		inParagrafo=q+1;
	} while (inParagrafo<fiParagrafo);

}

void CRPMsgEditCtrl::HyperlinkJump(CPoint *point)
{

	int linkn=LinkFromPoint(point);

	if (linkn!=-1)
	{
		CString exe, parms, par;
		int k=1;
		
		AfxExtractSubString(exe, Links.GetAt(linkn).link, 0, '§');
		AfxExtractSubString(parms, Links.GetAt(linkn).link, 1, '§');

		// hypertext handling is performed by the system shell
		ShellExecute(NULL,"open", exe, parms, NULL, SW_SHOWNORMAL);
	}	
}

void CRPMsgEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ( ((CRealPopupApp *)AfxGetApp())->opz_chkDblClick )
		HyperlinkJump(&point);
	
	CRichEditCtrl::OnLButtonDblClk(nFlags, point);
}

void CRPMsgEditCtrl::SetFont(CFont *pFont, BOOL bRedraw)
{
	// takes control to get actual font height,
	// it's needed for hypertexts handling

	CDC *cdcMatro=GetDC();
	cdcMatro->SelectObject(pFont);
	CSize fsizeMatro=cdcMatro->GetOutputTextExtent("MATRO");
	fontY=fsizeMatro.cy;
	ReleaseDC(cdcMatro);

	CRichEditCtrl::SetFont(pFont, bRedraw);
}

void CRPMsgEditCtrl::OnDropFiles(HDROP hDropInfo)
{
	CString filename, texttoadd;
	UINT pos=-1, count, buflen;
	CHARRANGE cr;
	GetSel(cr);

	count=DragQueryFile(hDropInfo, pos, NULL, NULL);
	for (pos=0; pos<count; pos++)
	{
		buflen=DragQueryFile(hDropInfo, pos, NULL, NULL)+1;
		DragQueryFile(hDropInfo, pos, filename.GetBuffer(buflen), buflen);
		filename.ReleaseBuffer();

		if (filename.Right(4).CompareNoCase(".url")==0)
		{
			CString urlfilename=filename;
			GetPrivateProfileString("InternetShortcut","URL",urlfilename,filename.GetBuffer(MAX_PATH),MAX_PATH,urlfilename);
			filename.ReleaseBuffer();
		}

		if (filename.Find(' ')>-1 && filename.Left(2).Compare("\\\\")==0)
		{
			filename.Insert(0, 34);
			filename.AppendChar(34);
		}
		texttoadd.Append(filename + "\n");
	}
	SetSel(cr);
	ReplaceSel(texttoadd, true);
	QuoteHandling(false);

	::DragFinish(hDropInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CRPMsgEditCtrl::XRichEditOleCallback

BEGIN_INTERFACE_MAP(CRPMsgEditCtrl, CRichEditCtrl)
	// we use IID_IUnknown because richedit doesn't define an IID
	INTERFACE_PART(CRPMsgEditCtrl, IID_IUnknown, RichEditOleCallback)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) CRPMsgEditCtrl::XRichEditOleCallback::AddRef()
{
	METHOD_PROLOGUE_EX_(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP_(ULONG) CRPMsgEditCtrl::XRichEditOleCallback::Release()
{
	METHOD_PROLOGUE_EX_(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::GetNewStorage(LPSTORAGE* ppstg)
{
	METHOD_PROLOGUE_EX_(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::GetInPlaceContext(
	LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	METHOD_PROLOGUE_EX(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
	METHOD_PROLOGUE_EX(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::QueryInsertObject(
	LPCLSID /*lpclsid*/, LPSTORAGE /*pstg*/, LONG /*cp*/)
{
	METHOD_PROLOGUE_EX(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::DeleteObject(LPOLEOBJECT /*lpoleobj*/)
{
	METHOD_PROLOGUE_EX_(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::QueryAcceptData(
	LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD reco,
	BOOL fReally, HGLOBAL hMetaPict)
{
	METHOD_PROLOGUE_EX(CRPMsgEditCtrl, RichEditOleCallback)

	// accepts anything =o)
	return S_OK;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::GetClipboardData(
	CHARRANGE* lpchrg, DWORD reco, LPDATAOBJECT* lplpdataobj)
{
	METHOD_PROLOGUE_EX(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::GetDragDropEffect(
	BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (DROPEFFECT_LINK & *pdwEffect)
		*pdwEffect = DROPEFFECT_LINK;
	return S_OK;
}

STDMETHODIMP CRPMsgEditCtrl::XRichEditOleCallback::GetContextMenu(
	WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg,
	HMENU* lphmenu)
{
	METHOD_PROLOGUE_EX(CRPMsgEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}
