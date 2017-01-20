/*
Module : DDXFILE.CPP
Purpose: implementation for a MFC DDX routine to get a filename
         Using the file open/save as common dialogs
Created: PJN / 19-03-1997
History: PJN / 17-09-1998 1.  VC 5 mak files provided now as standard
                          2   All code now compiles cleanly at warning level 4
                          3.  Unicode enabled all the code and provision of Unicode build configurations.
                          4.  Replaced all TRACE0 calls with TRACE
                          5.  General tidy up of the sample app including removing all the AppWizard
                              generated comments. 
                          6.  Module name has been changed from ddxgetfolder to ddxfolder.
                          7.  Option to have non read only edit control.
                          8.  Changed name of main function from DDX_GetFilenameControl to DDX_FilenameControl
                          9.  Sample app now allows read only state of widget to be toggled
                          10. Change the ID's of the strings in the resource table which the code uses
                          11. Provision of a DDV function
                          12. Addition of a DDX_FolderValue function
                          13. Updated all the documentation to be HTML based as with my other shareware / freeware.
         PJN / 11-12-1999 1.  Cosmetic item where now a 1 pixel space is included between the edit control and 
                              the "..." button
         PJN / 14-05-2000 The "..." button now supports being tabbed into i.e. the WS_TABSTOP style
         PJN / 17-05-2000 The width of the underlying edit control is now shrunk when the "..." button is created.
                          This makes layout of the control using the resource designer a lot simpler.
         PJN / 18-07-2000 Width of the edit button is now determined from the text which it displays.
         PJN / 24-08-2001 1. Fixed a problem with the Z-Order of the control being set incorrectly
                          2. Updated copyright message 
         PJN / 27-12-2001 1. Fixed an issue where the browse button would do nothing when you type in for example
                          "c:\". Thanks to Stephen Agate for spotting this.


Copyright (c) 1997 - 2001 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include <dlgs.h>
#include "resource.h"
#include "ddxfile.h"



///////////////////////////////// defines /////////////////////////////////////

#define GETFILENAME_EDIT_ID 100


BEGIN_MESSAGE_MAP(CModifyButton, CButton)
  //{{AFX_MSG_MAP(CModifyButton)
  ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()
        
CModifyButton::CModifyButton()
{
  m_pBuddy = NULL;
  m_bFirstCall = TRUE;
}

void CModifyButton::SetBuddy(CGetFilenameControl* pBuddy)
{
  m_pBuddy = pBuddy;
}

BOOL CModifyButton::PreTranslateMessage(MSG* pMsg) 
{                     
  //create the tooltip
  if (m_bFirstCall)
  {
    m_ToolTip.Create(this);
    m_ToolTip.Activate(TRUE);
    m_ToolTip.AddTool(this, IDS_DDXFILE_TT_MODIFY);
    m_bFirstCall = FALSE;
  }
  
  //give the tooltip a chance to handle the message
  m_ToolTip.RelayEvent(pMsg);

  return CButton::PreTranslateMessage(pMsg);
}

void CModifyButton::OnClicked() 
{
  if (m_pBuddy)
    m_pBuddy->Edit();
  else
    TRACE(_T("CModifyButton: No auto buddy defined\n"));
}                        



IMPLEMENT_DYNAMIC(CGetFileNameDialog, CFileDialog)

CGetFileNameDialog::CGetFileNameDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, 
                                       CWnd* pParentWnd) :	CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, 0)
{
}

BEGIN_MESSAGE_MAP(CGetFileNameDialog, CFileDialog)
	//{{AFX_MSG_MAP(CGetFileNameDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGetFileNameDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();

  //modify the text on the IDOK button to ok
  //if we are using the old style common dialog
  if ( ((m_ofn.Flags & OFN_EXPLORER) == 0) || ((GetVersion() & 0xff) < 4) )
  {
	  CString sText;
    if (!sText.LoadString(IDS_DDXFILE_OK))
      ASSERT(FALSE);
    CWnd* pOkButton = GetDlgItem(IDOK);
    ASSERT(pOkButton);
    if (pOkButton)
	    pOkButton->SetWindowText(sText);
  }
	
	return TRUE;
}

void CGetFileNameDialog::OnOK()
{
  BOOL bCallParent = TRUE;

  if ( ((m_ofn.Flags & OFN_EXPLORER) == 0) || ((GetVersion() & 0xff) < 4) )
  {
    //check for overwrite if that flag is used
    if (m_ofn.Flags & OFN_OVERWRITEPROMPT)
    {
      //Retrieve the filename selected
      TCHAR sPath[_MAX_PATH];
      GetDlgItemText(edt1, sPath, _MAX_PATH);

      //check to see if the file already exists
      CFileStatus fs;
      if (CFile::GetStatus(sPath, fs))
      {
        CString sMessage;
        AfxFormatString1(sMessage, IDS_DDXFILE_OVERWRITE_PROMPT, sPath);

        //ensure the title is the same as the common dialog caption
        CString sCaption;
        GetWindowText(sCaption);
        bCallParent = (MessageBox(sMessage, sCaption, MB_YESNO | MB_ICONQUESTION) == IDYES);
      }
    }
  }

  if (bCallParent)
    CFileDialog::OnOK();
}

void CGetFileNameDialog::OnInitDone()
{
	CString sText;
  if (!sText.LoadString(IDS_DDXFILE_OK))
    ASSERT(FALSE);
  LPTSTR pszBuffer = sText.GetBuffer(sText.GetLength());

  //modify the text on the IDOK button to ok
  CommDlg_OpenSave_SetControlText(GetParent()->m_hWnd, IDOK, pszBuffer);

  sText.ReleaseBuffer();
}

BOOL CGetFileNameDialog::OnFileNameOK()
{
  BOOL rVal = CFileDialog::OnFileNameOK();

  if (!rVal)  //if the filename passed the parent tests
             //then put it throught our own tests
  {
    //check for overwrite if that flag is used, since the file open
    //common dialog will not bother looking at this flag
    if (m_ofn.Flags & OFN_OVERWRITEPROMPT)
    {
      //Retrieve the filename selected
      TCHAR sPath[_MAX_PATH];
      CommDlg_OpenSave_GetFilePath(GetParent()->m_hWnd, sPath, _MAX_PATH);

      //check to see if the file already exists
      CFileStatus fs;
      if (CFile::GetStatus(sPath, fs))
      {
        CString sMessage;
        AfxFormatString1(sMessage, IDS_DDXFILE_OVERWRITE_PROMPT, sPath);

        //ensure the title is the same as the common dialog caption
        CString sCaption;
        GetParent()->GetWindowText(sCaption);
        rVal = (MessageBox(sMessage, sCaption, MB_YESNO | MB_ICONQUESTION) != IDYES);
      }
    }
  }
  return rVal;
}



BEGIN_MESSAGE_MAP(CGetFilenameControl, CEdit)
  //{{AFX_MSG_MAP(CGetFilenameControl)
	ON_WM_ENABLE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGetFilenameControl::CGetFilenameControl()
{
}

void CGetFilenameControl::OnEnable(BOOL bEnable) 
{
	CEdit::OnEnable(bEnable);
	
  //reflect the enabled state of this control to the buddy edit control
	m_Edit.EnableWindow(bEnable);
}

BOOL CGetFilenameControl::SubclassEdit(HWND hEdit)
{   
  //test our inputs
  ASSERT(this);
  if (!IsWindow(hEdit))
  {
    ASSERT(FALSE);
    TRACE(_T("CGetFilenameControl::SubclassEdit -- window handle is invalid!\n"));
    return FALSE;
  }                
  
  //subclass the control
  if (SubclassWindow(hEdit))
    return AddEditButton();
  else
  {
    TRACE(_T("CGetFilenameControl::SubclassEdit -- Could not subclass edit control!\n"));
    ASSERT(FALSE);
    return FALSE;
  }
}

BOOL CGetFilenameControl::AddEditButton()
{
  CWnd* pParent = GetParent();
  ASSERT(pParent);

  //Load up the text which is to go on the button
  CString sEditControlText;
  if (!sEditControlText.LoadString(IDS_DDXFILE_EDIT_TEXT))
    ASSERT(FALSE);

  //Work out the dimensions of the "..." button and the resized edit control
  CRect editRect;  
  GetWindowRect(editRect);
  pParent->ScreenToClient(editRect);
  CRect btnRect;
  btnRect.top    = editRect.top;
  btnRect.bottom = editRect.bottom;
  btnRect.right  = editRect.right;

  //Calculate the width of the button based on the text we are going to put on in
  CDC* pDC = pParent->GetDC();
  int nButtonWidth = 0;
	if (pDC)
  {
    //Setup the DC
		CFont* pNewFont = pParent->GetFont();
		CFont* pOldFont = pDC->SelectObject(pNewFont);

    //Get the button width
		CSize SizeText = pDC->GetTextExtent(_T("    "),4); //We add space around text button
		pDC->LPtoDP(&SizeText);
		nButtonWidth = SizeText.cx;
		SizeText = pDC->GetTextExtent(sEditControlText, sEditControlText.GetLength());
		pDC->LPtoDP(&SizeText);
		nButtonWidth += SizeText.cx;

    //Restore the DC
		pDC->SelectObject(pOldFont);
	}
	else
    nButtonWidth = btnRect.Height()*8/10;  //width is 8/10 of height

  //Now that we have the width we can fill out the rest of the buttons rect position
  btnRect.left = btnRect.right - nButtonWidth;

  //Tidy up the device context
  pParent->ReleaseDC(pDC);

  //Resize the edit control
  SetWindowPos(NULL, 0, 0, editRect.Width() - btnRect.Width() - 1, editRect.Height(), SWP_NOACTIVATE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOZORDER);

  //dynamically create the edit button control
  BOOL bSuccess = m_Edit.Create(sEditControlText, WS_DISABLED | WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_GROUP, btnRect, pParent, GETFILENAME_EDIT_ID);

  //Set the tab order of the button
  m_Edit.SetWindowPos(this, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

  //tell the button to call this class when it is clicked
  m_Edit.SetBuddy(this);

  //ensure it is using the same font as the parent
  m_Edit.SetFont(pParent->GetFont());

  return bSuccess;
}

void CGetFilenameControl::Edit()
{
  //bring up the common file open dialog to select the filename

  //retreive the current filename to use as the initial value
  CString sCurrentFilename;
  GetWindowText(sCurrentFilename);

  //Make sure the browse button at least works if a directory is 
  //currently selected
  int nCurrentFilenameLength = sCurrentFilename.GetLength();
  if (nCurrentFilenameLength)
  {
    if (sCurrentFilename.GetAt(nCurrentFilenameLength-1) == _T('\\'))
      sCurrentFilename += _T("*.*");
  }

  //work out the flags to use
  DWORD dwFlags = OFN_HIDEREADONLY;
  if (m_dwFlags & GF_OVERWRITEPROMPT)
    dwFlags |= OFN_OVERWRITEPROMPT;
  if (m_dwFlags & GF_FILEMUSTEXIST)
    dwFlags |= OFN_FILEMUSTEXIST;
                 
  CGetFileNameDialog dlg(TRUE, NULL, sCurrentFilename, dwFlags, m_sExtFilter, GetParent());

  //Modify the title to the desired value
  dlg.m_ofn.lpstrTitle = m_sDialogTitle;
  
  //uncomment the following line if you want to use the old style file open Scommon dialog
  if (m_dwFlags & GF_OLD_STYLE_DIALOG)
    dlg.m_ofn.Flags &= ~OFN_EXPLORER;

  //bring up the dialog and if hit ok set the text in this control to the new filename
  if (dlg.DoModal() == IDOK)
    SetWindowText(dlg.GetPathName());
}



void DDX_FilenameControl(CDataExchange* pDX, int nIDC, CGetFilenameControl& rCGetFilenameControl, DWORD dwFlags, const CString& sDialogTitle, const CString& sExtFilter)
{
  HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
  if (!pDX->m_bSaveAndValidate && rCGetFilenameControl.m_hWnd == NULL)    // not subclassed yet
  {
    if (!rCGetFilenameControl.SubclassEdit(hWndCtrl))
    {
      ASSERT(FALSE);      // possibly trying to subclass twice ?
      AfxThrowNotSupportedException();
    }
  }
  rCGetFilenameControl.SetFlags(dwFlags);
  rCGetFilenameControl.SetDialogTitle(sDialogTitle);
  rCGetFilenameControl.SetExtensionFilter(sExtFilter);
}



void DDX_FilenameValue(CDataExchange* pDX, CGetFilenameControl& rCGetFilenameControl, CString& sFile)
{
  if (pDX->m_bSaveAndValidate)
    rCGetFilenameControl.GetWindowText(sFile); 
  else
    rCGetFilenameControl.SetWindowText(sFile);     
}



void DDV_FilenameControl(CDataExchange* pDX, CGetFilenameControl& rCGetFilenameControl)
{
  BOOL bFileExists = FALSE;
  DWORD dwFlags = rCGetFilenameControl.GetFlags();
	if (dwFlags & GF_FILEMUSTEXIST)
  {
    CString sFile;
    rCGetFilenameControl.GetWindowText(sFile);

    //Check to see if the file exists
    DWORD dwAttributes = GetFileAttributes(sFile);
    if (dwAttributes != 0xFFFFFFFF)
      bFileExists = ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);

   	if (!pDX->m_bSaveAndValidate && !bFileExists)
	  {
		  TRACE(_T("Warning: initial dialog data contains a non - existant file name.\n"));
		  return;     // don't stop now
    }

    if (pDX->m_bSaveAndValidate && !bFileExists)
    {
	    AfxMessageBox(IDS_DDXFILE_REQUIRE_VALID_FILE);
      pDX->PrepareEditCtrl(rCGetFilenameControl.GetDlgCtrlID());
	    pDX->Fail();
    }
  }

	if (dwFlags & GF_OVERWRITEPROMPT)
  {
    CString sFile;
    rCGetFilenameControl.GetWindowText(sFile);

    //Check to see if the file exists
    DWORD dwAttributes = GetFileAttributes(sFile);
    if (dwAttributes != 0xFFFFFFFF)
      bFileExists = ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);

    if (pDX->m_bSaveAndValidate && bFileExists)
    {
      CString sMsg;
      AfxFormatString1(sMsg, IDS_DDXFILE_OVERWRITE_PROMPT, sFile);
	    if (AfxMessageBox(sMsg, MB_YESNO | MB_ICONQUESTION) != IDYES)
      {
        pDX->PrepareEditCtrl(rCGetFilenameControl.GetDlgCtrlID());
	      pDX->Fail();
      }
    }
  }
}

