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
#include "Globals.h"
#include "RankedArray.h"

#define RPRNKSCHEMA		1

void CRankedArray::Save()
{
	CString ItemNumber;
	char iValue[65];

	WritePrivateProfileString("RealPopup", "RNKVersion", itoa(RPRNKSCHEMA,iValue,10), FileName);

	for (int k=0; (k<items.GetSize()); k++)
	{
		ItemNumber.Format("Item%03i", k+1);
		WritePrivateProfileString(SectionName, ItemNumber, items.GetAt(k), FileName);
	}

	// this dummy item will cause load loop not to consider exceeding old groups
	ItemNumber.Format("Item%03i", k+1);
	WritePrivateProfileString(SectionName, ItemNumber, "", FileName);
}

void CRankedArray::Load()
{
	CString ItemString, ItemNumber;
	int k=1;

	while (true)
	{
		ItemNumber.Format("Item%03i", k++);
		GetPrivateProfileString(SectionName, ItemNumber, "", ItemString.GetBufferSetLength(USERLIST_BUFFER_LEN), USERLIST_BUFFER_LEN, FileName);
		ItemString.ReleaseBuffer();
		if (ItemString.GetLength()==0)
			break;
		items.Add(ItemString);
	}
}

const CString &CRankedArray::GetAt(INT_PTR nIndex) const
{
	return items.GetAt(nIndex);
}

INT_PTR CRankedArray::GetSize()
{
	return items.GetSize();
}

bool CRankedArray::Exists(CString item)
{
	for (int q=0;q<items.GetSize();q++)
	{
		if (items.GetAt(q).CompareNoCase(item)==0)
			return true;
	}
	
	return false;
}

void CRankedArray::RankRemove(CString item)
{
	for (int k=0;k<items.GetSize() && k<ItemsMax;k++)
	{
		if (items.GetAt(k)==item)
		{
			items.RemoveAt(k);
			break;
		}
	}
}

void CRankedArray::RankAdd(CString item)
{
	for (int k=0;k<items.GetSize() && k<ItemsMax;k++)
	{
		if (items.GetAt(k)==item)
		{
			items.RemoveAt(k);
			break;
		}
	}

	if (k==items.GetSize() && k>=ItemsMax)
		items.RemoveAt(items.GetCount()-1);

	items.InsertAt(0, item);
}