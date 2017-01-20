/*
Module : DDXFILE.H
Purpose: Defines the interface to a for a MFC DDX_ routine to get a filename
         Using the file open/save as common dialogs
Created: PJN / 19-03-1997
History: None

Copyright (c) 1997 - 2001 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


////////////////////////////////// Macros ///////////////////////////

#ifndef _DDXFILE_H__
#define _DDXFILE_H__


////////////////////////////////// Consts /////////////////////////////////////

//flags used to control how the DDX_GetFilenameControl routine workds

const DWORD GF_OVERWRITEPROMPT  = 0x0001;   //User will be prompted about overwriting existing file 
                                            //prior to allowing selection
const DWORD GF_FILEMUSTEXIST    = 0x0002;   //File must exist to be selected
const DWORD GF_OLD_STYLE_DIALOG = 0x0004;   //Use the old style file open dialog instead of the
                                            //style as used in Windows Explorer





////////////////////// foward declaration ///////////////////////////
class CGetFilenameControl;


/////////////////////////// Classes /////////////////////////////////
class CModifyButton : public CButton
{
public:
  CModifyButton();
  void SetBuddy(CGetFilenameControl* pBuddy);

protected:
  //{{AFX_VIRTUAL(CModifyButton)
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  //}}AFX_VIRTUAL

  //{{AFX_MSG(CModifyButton)
  afx_msg void OnClicked();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

  CGetFilenameControl* m_pBuddy;
  BOOL m_bFirstCall;
  CToolTipCtrl m_ToolTip;
};


class CGetFileNameDialog : public CFileDialog
{
public:
	CGetFileNameDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);

protected:
	DECLARE_DYNAMIC(CGetFileNameDialog)

  virtual void OnInitDone();
  virtual BOOL OnFileNameOK();

	//{{AFX_MSG(CGetFileNameDialog)
	//}}AFX_MSG
  virtual BOOL OnInitDialog();
  virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


class CGetFilenameControl : public CEdit
{
public:
//Constructors / Destructors
  CGetFilenameControl();

//Methods
  BOOL SubclassEdit(HWND hEdit);

//Accessors/Mutators

  void CGetFilenameControl::OnEnable(BOOL bEnable);

  //Set/Get the flags
  void SetFlags(DWORD dwFlags) { m_dwFlags = dwFlags; };
  DWORD GetFlags() const { return m_dwFlags; };

  //Set/Get the dialog title
  void SetDialogTitle(const CString& sDialogTitle) { m_sDialogTitle = sDialogTitle; };
  CString GetDialogTitle() const { return m_sDialogTitle; };

  //Set/Get the extension filter string
  void SetExtensionFilter(const CString& sExtFilter) { m_sExtFilter = sExtFilter; };
  CString GetExtensionFilter() const { return m_sExtFilter; };

  //Bring up the file picker dialog
  void Edit();

protected:
  //{{AFX_VIRTUAL(CGetFilenameControl)
  //}}AFX_VIRTUAL

  //{{AFX_MSG(CGetFilenameControl)
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

  BOOL AddEditButton();

  CModifyButton m_Edit;
  DWORD         m_dwFlags;
  CString       m_sDialogTitle;
  CString       m_sExtFilter;
};





/////////////// MFC Data exchange routines //////////////////////////

void DDX_FilenameControl(CDataExchange* pDX, int nIDC, CGetFilenameControl& rCGetFilenameControl, DWORD dwFlags, const CString& sDialogTitle, const CString& sExtFilter);
void DDX_FilenameValue(CDataExchange* pDX, CGetFilenameControl& rCGetFilenameControl, CString& sFile);
void DDV_FilenameControl(CDataExchange* pDX, CGetFilenameControl& rCGetFilenameControl);


#endif //_DDXFILE_H__