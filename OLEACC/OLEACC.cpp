/*  
	
	empty OLEACC.DLL, fixes MFC70.DLL compatibility with Windows95
    coded by Matro (matro@realpopup.it), created by Ted (Ted@t--x.org)

	published post:
	http://groups.google.com/groups?dq=&hl=it&lr=&ie=UTF-8&oe=UTF-8&selm=%23cFMlyy2BHA.1552%40tkmsftngp05

	entire thread:
	http://groups.google.com/groups?hl=it&lr=&ie=UTF-8&oe=UTF-8&threadm=uYdm4Vf2BHA.1660%40tkmsftngp02&rnum=6&prev=/groups%3Fas_q%3Dmfc70%2520windows%252095%26ie%3DUTF-8%26oe%3DUTF-8%26lr%3D%26hl%3Dit

*/ 

#include "stdafx.h"
#include "unknwn.h" 
#include "OLEACC.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

extern "C"  int    __declspec(dllexport) AccessibleObjectFromWindow(HWND
hwnd, DWORD dwId, REFIID riid, void **ppvObject) { return 0; }

extern "C"  int    __declspec(dllexport) CreateStdAccessibleObject(HWND
hwnd, LONG idObject, REFIID riid, void** ppvObject) {return 0; }

extern "C"  LRESULT      __declspec(dllexport) LresultFromObject(REFIID
riid, WPARAM wParam, LPUNKNOWN punk) { return 0; }


// This is the constructor of a class that has been exported.
// see OLEACC.h for the class definition
COLEACC::COLEACC()
{ 
	return; 
}
