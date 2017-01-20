/*
	RealPopup v2.6
	Code by Matro
	Rome, Italy, 1998-2005
	matro@email.it   
	matro@realpopup.it

	last release 1-2005 
	designed for Windows 9x/NT kernels
*/

#if !defined(_NB_H)
#define _NB_H

BOOL NBAddName (int nLana, LPCTSTR szName, BYTE mark, PNCB pncb);
BOOL NBDelName (int nLana, LPCTSTR szName, BYTE mark, PNCB pncb);
BOOL NBReset (int nLana, int nSessions, int nNames, PNCB pncb);
BOOL NBListen(int nLana, LPCTSTR szName, BYTE mark, LPCTSTR szOther, BYTE markOther, BYTE bySendTO, BYTE byRcTO, PNCB pncb, HANDLE hEvent);
BOOL NBReceive(int nLana, BYTE lsn, LPBYTE lpPacket, LPWORD pcbLength, PNCB pncb);
BOOL NBReceiveDatagram(int nLana, BYTE lsn, LPBYTE lpPacket, LPWORD pcbLength, PNCB pncb);
BOOL NBHangup(int nLana, BYTE lsn, PNCB pncb);
BOOL NBCall(int nLana, LPTSTR szName, BYTE mark, LPTSTR szOther, BYTE markOther, BYTE bySendTO, BYTE byRecvTO, LPBYTE plsn, PNCB pncb);
BOOL NBSend(int nLana, BYTE lsn, LPBYTE lpPacket, WORD cbLength, PNCB pncb);
BOOL NBEnum(PLANA_ENUM plae, WORD cbLength, PNCB pncb);
BOOL NBCancel(int nLana, PNCB pncb);

#endif //!defined(_NB_H)