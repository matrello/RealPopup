/*
RealPopup v2.6
Code by Matro
Rome, Italy, 1998-2005
matro@email.it   
matro@realpopup.it

last release 1-2005 
designed for Windows 9x/NT kernels
*/

#if !defined(MATRO_CRANKEDIT_INCLUDED)
#define MATRO_CRANKEDIT_INCLUDED

class CRankedArray
{
public:
	void Load();
	void Save();
	bool Exists(CString item);
	void RankAdd(CString item);
	void RankRemove(CString item);
	const CString &GetAt(INT_PTR nIndex) const;
	INT_PTR GetSize();

	CString FileName;
	CString SectionName;
	int ItemsMax;

private:
	CStringArray items;

};

#endif