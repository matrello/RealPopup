// Dictionary.h: interface/implementation for the Dictionary class.
//
// Dictionary is a templatized class which is a direct copy of
// the VBScript Scripting.Dictionary object.  All methods
// and properties are provided with a syntax that is either
// identical to the VBS implementation, or with only trivial
// differences (mainly, C++ member functions require parentheses
// where VBscript methods don't)
//
// Comparison to VBS given in sample at end of header file.
// For most things, VBS docs can be used.
// 
// Copyright 1998, James M. Curran, All Rights reserved.
//  May be used freely, provided copyright noticed remains.
//  May not be sold in source code form.
//
// email: JamesCurran@CompuServe.Com
//   WWW: http://www.NJTheater.Com/JamesCurran
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICTIONARY_H__A2B46E03_6E48_11D2_A434_00805F355F6A__INCLUDED_)
#define AFX_DICTIONARY_H__A2B46E03_6E48_11D2_A434_00805F355F6A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>

template<typename KEY, typename ITEM>
class Dictionary  : std::map<KEY, ITEM>
{
private:
	class DIH
	{
	private:
		iterator					iter;
	public:
		DIH(iterator i) : iter(i) {}
		void operator=(ITEM i)
		{	*iter = std::pair<KEY,ITEM>( (*iter).first, i);	}

		operator ITEM()
		{	return (*iter).second;	}

		friend std::ostream& operator<<(std::ostream& os, const DIH& dih);
	};

	class DKH
	{
	private:
		iterator					iter;
		std::map<KEY, ITEM>&	mp;
	public:
		DKH(std::map<KEY, ITEM>& m, iterator i) : mp(m), iter(i) {}
		void operator=(KEY k)
		{	std::pair<KEY,ITEM>  p(k, (*iter).second);
			mp.erase(iter);
			mp.insert(p);
		}
	};

	void operator=(const Dictionary<KEY, ITEM>& d);	// disallowed (not written)

public:
	Dictionary() {};
	virtual ~Dictionary(){};
	
	void	Add(KEY k, ITEM i)
	{	insert(std::pair<KEY,ITEM>(k,i));	}

	bool Exists(KEY k) const 
	{	return (find(k) != end());	}

	size_type Count() const
	{	return size();	}

	void Remove(KEY k)
	{	erase(k);	}

	void RemoveAll()
	{	clear();	}


	std::vector<KEY>	Keys() const
	{	std::vector<KEY>		keys;
		std::transform(begin(), end(), std::back_inserter(keys), getKEY);
		return keys;
	}

	std::vector<ITEM>	Items() const
	{	std::vector<ITEM>		items;
		std::transform(begin(), end(), std::back_inserter(items), getITEM);
		return items;
	}

	DIH	Item(KEY k)
	{
		iterator	iter = find(k);
		if (iter == end())
			throw "Key not found in Item() call";
		return (DIH(iter));
	}

	DIH operator[](KEY k)
	{	return Item(k);		}

	DIH operator()(KEY k)
	{	return Item(k);		}

	DKH	Key(KEY k)
	{
		iterator	iter = find(k);
		if (iter == end())
			throw "Key not found in Key() call";
		return (DKH(*this, iter));
	}

protected:
	static KEY getKEY(std::pair<KEY,ITEM> p)
	{	return p.first; }

	static KEY getITEM(std::pair<KEY,ITEM> p)
	{	return p.second; }

};

// This doesn't work... When I figure out why, I'll reinstate it.

/*
template<typename KEY, typename ITEM>
inline std::ostream& operator<<(std::ostream& os, const Dictionary<KEY, ITEM>::DIH& dih)
{	os << (*dih.iter).second;
	return(os);
}

/// The remainder is a same sample code which demonstrates the similarity 
// between the syntaxes for the VBS Dictionary object & this Dictionary template
//

#if defined(DICTIONARY_TEST)
#include <vector>
#include <iostream>
#include <string>

#pragma warning(disable:4786)

using std::vector;
using std::string;
using std::cout;
using std::endl;

int dictionary_test()
{
	// VBScript sample code from Add Method
	// Dim d
	// Set d = CreateObject("Scripting.Dictionary")
	// d.Add "a", "Athens"
	// d.Add "b", "Belgarde"
	// d.Add "c", "Cairo"

	Dictionary<string, string>	d;

	d.Add("a", "Athens");
	d.Add("b", "Belgarde");
	d.Add("c", "Cairo");


	cout << "Keys:" << endl;
	// VBScript sample code from both Count property & Keys method
	// dim a
	// a = d.Keys
	// For i = 0 to d.Count -1
	//	 s = s & a(i) & "<BR>"
	// Next
	vector<string>	a1;
	string			s;
	a1 = d.Keys();
	for (int i = 0; i < d.Count(); i++)
		s = s + a1[i] + '\n';

	cout << s ;
	
	cout << endl ;
	cout << "Items:" << endl;
	// VBScript sample code from Items  method
	// dim a
	// a = d.Items
	// For i = 0 to d.Count -1
	//	 s = s & a(i) & "<BR>"
	// Next
	vector<string>	a2 = d.Items();
	for (i = 0; i < d.Count(); i++)
		cout << a2[i] << endl;	

	cout << endl ;

	// VBScript sample code from Exists method
	//	If d.Exists("c") Then    
	//		msg = "Specified key exists."  
	//	Else
	//		msg = "Specified key doesn't exist."  
	//	End If

	if (d.Exists("c"))
		cout << "Key: 'c' Exists" << endl;
	else
		cout << "Key: 'c' does not exist" << endl;

	// VBScript sample code from Remove method
	// a = d.Remove("b")       'Remove second pair
	// (docs never explain the return value of Remove method)

	d.Remove("b");

	// VBScript sample code from Key property
	//	d.Key("c") = "d"        'Set key for "c" to "d".
	d.Key("c") = "d";

	string dd = d.Item("d");
	cout << endl ;
	cout << "Item for new key 'd': " << dd << endl;

	cout << endl ;
	cout << "# Items in dict: " << d.Count() << endl;

	// VBScript sample code from RemoveAll method
	// a = d.RemoveAll         'Clear the dictionary
	// (docs never explain the return value of RemoveAll method)
	d.RemoveAll();

	cout << endl ;
	cout << "# Items in dict: " << d.Count() << endl;

	return 1;
}
#endif
*/

#endif // !defined(AFX_DICTIONARY_H__A2B46E03_6E48_11D2_A434_00805F355F6A__INCLUDED_)
