/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(AFX_MASKEDBITMAP_H__90848A24_1F0E_48DC_9E57_E1B7F80B17DC__INCLUDED_)
#define AFX_MASKEDBITMAP_H__90848A24_1F0E_48DC_9E57_E1B7F80B17DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaskedBitmap : public CBitmap  
{
public:
	CMaskedBitmap();
	virtual ~CMaskedBitmap();
	void DrawTransparent (CPaintDC* pDC, int x, int y, COLORREF crColor);
};

#endif // !defined(AFX_MASKEDBITMAP_H__90848A24_1F0E_48DC_9E57_E1B7F80B17DC__INCLUDED_)
