/*  
	
	empty OLEACC.DLL, fixes MFC70.DLL compatibility with Windows95
    coded by Matro (matro@realpopup.it), created by Ted (Ted@t--x.org)

	published post:
	http://groups.google.com/groups?dq=&hl=it&lr=&ie=UTF-8&oe=UTF-8&selm=%23cFMlyy2BHA.1552%40tkmsftngp05

	entire thread:
	http://groups.google.com/groups?hl=it&lr=&ie=UTF-8&oe=UTF-8&threadm=uYdm4Vf2BHA.1660%40tkmsftngp02&rnum=6&prev=/groups%3Fas_q%3Dmfc70%2520windows%252095%26ie%3DUTF-8%26oe%3DUTF-8%26lr%3D%26hl%3Dit

*/ 

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OLEACC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OLEACC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef OLEACC_EXPORTS
#define OLEACC_API __declspec(dllexport)
#else
#define OLEACC_API __declspec(dllimport)
#endif

// This class is exported from the OLEACC.dll
class OLEACC_API COLEACC {
public:
	COLEACC(void);
	// TODO: add your methods here.
};

extern OLEACC_API int nOLEACC;

OLEACC_API int fnOLEACC(void);
