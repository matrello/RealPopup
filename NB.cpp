/*
	nb.c
	msngmsg.c

   Copyright (C) 1998 by Andrey Shedel
   You may distribute under the terms of the GNU General Public License

   netbios() calls wrapper
   modified for RealPopup by Matro, 2001
*/

#define WIN32_LEAN_AND_MEAN
#define STRICT

#include <stdafx.h>
#include "RealPopup.h"
#include <nb30.h>
#include "nb.h"

static void MakeNetbiosName (unsigned char achDest[NCBNAMSZ], LPCTSTR szSrc, BYTE mark) /*fold00*/
{
	int cchSrc;
	
	cchSrc = lstrlen (szSrc);
	
	if (cchSrc >= NCBNAMSZ)
		cchSrc = NCBNAMSZ - 1;
	
	memset((void *)achDest, ' ', NCBNAMSZ);
	
	achDest[NCBNAMSZ - 1] = mark;
	
#if defined(UNICODE)
	WideCharToMultiByte(CP_ACP, 0, szSrc, cchSrc, achDest, NCBNAMSZ - 1, NULL, NULL);
#else
	CharToOemBuff(szSrc,(char *)achDest, cchSrc);
#endif
}

BOOL NBAddName (int nLana, LPCTSTR szName, BYTE mark, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	pncb->ncb_command = NCBADDNAME;
	pncb->ncb_lana_num = nLana;
	
	MakeNetbiosName (pncb->ncb_name, szName, mark);
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBAddName", "lana %i name %s *attempt*\n", pncb->ncb_lana_num, szName);

	Netbios (pncb);	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBAddName", "lana %i ret %x name %s lsn %x\n", pncb->ncb_lana_num, pncb->ncb_retcode,szName, pncb->ncb_lsn);

	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBDelName (int nLana, LPCTSTR szName, BYTE mark, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	pncb->ncb_command = NCBDELNAME;
	pncb->ncb_lana_num = nLana;
	
	MakeNetbiosName(pncb->ncb_name, szName, mark);
	
	Netbios(pncb);
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBDelName", "lana %i ret %x name %s\n", pncb->ncb_lana_num, pncb->ncb_retcode,szName);

	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBReset (int nLana, int nSessions, int nNames, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	pncb->ncb_command = NCBRESET;
	pncb->ncb_lsn = 0;
	pncb->ncb_lana_num = nLana;
	pncb->ncb_callname[0] = nSessions; 
	pncb->ncb_callname[2] = nNames;
	
	Netbios(pncb);
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBReset", "lsn %x lana %i ret %x\n", pncb->ncb_lsn, pncb->ncb_lana_num, pncb->ncb_retcode);
	
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBListen(int nLana, LPCTSTR szName, BYTE mark, LPCTSTR szOther, BYTE markOther, BYTE bySendTO, BYTE byRcTO, PNCB pncb, HANDLE hEvent) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	
	MakeNetbiosName(pncb->ncb_name, szName, mark);
	MakeNetbiosName(pncb->ncb_callname, szOther, markOther);
	
	pncb->ncb_rto = byRcTO;
	pncb->ncb_sto = bySendTO;
	pncb->ncb_command = NCBLISTEN;
	pncb->ncb_lana_num = nLana;
	
	if(NULL != hEvent)
	{
		pncb->ncb_command |= ASYNCH;
		pncb->ncb_event = hEvent;
	}
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBListen", "lana %i *attempt*\n", pncb->ncb_lana_num);

	Netbios(pncb);
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBListen", "lana %i ret %x\n", pncb->ncb_lana_num, pncb->ncb_retcode);
	
	return (NULL == hEvent) ? (NRC_GOODRET == pncb->ncb_retcode) : (NRC_PENDING == pncb->ncb_retcode);
}

BOOL NBReceive(int nLana, BYTE lsn, LPBYTE lpPacket, LPWORD pcbLength, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	
	pncb->ncb_command = NCBRECV;
	pncb->ncb_lsn = lsn;
	pncb->ncb_lana_num = nLana;
	
	if(pcbLength)
		pncb->ncb_length = *pcbLength;
	else
		pncb->ncb_length = 0xffff;
	
	pncb->ncb_buffer = lpPacket;
	
	Netbios(pncb);
	
	if((NRC_GOODRET == pncb->ncb_retcode) && pcbLength)
		*pcbLength = pncb->ncb_length;
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBReceive", "lana %x ret %x\n", pncb->ncb_lana_num, pncb->ncb_retcode);
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBReceiveDatagram(int nLana, BYTE lsn, LPBYTE lpPacket, LPWORD pcbLength, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	
	pncb->ncb_command = NCBDGRECV;
	pncb->ncb_lsn = lsn;
	pncb->ncb_lana_num = nLana;
	
	if(pcbLength)
		pncb->ncb_length = *pcbLength;
	else
		pncb->ncb_length = 0xffff;
	
	pncb->ncb_buffer = lpPacket;
	
	Netbios(pncb);
	
	if((NRC_GOODRET == pncb->ncb_retcode) && pcbLength)
		*pcbLength = pncb->ncb_length;
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBReceiveDatagram", "lsn %x lana %x ret %x\n",pncb->ncb_lsn,pncb->ncb_lana_num,pncb->ncb_retcode);
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBHangup(int nLana, BYTE lsn, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	
	pncb->ncb_command = NCBHANGUP;
	pncb->ncb_lsn = lsn;
	pncb->ncb_lana_num = nLana;
	
	Netbios(pncb);
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBHangup", "lsn %x lana %x ret %x\n",pncb->ncb_lsn,pncb->ncb_lana_num,pncb->ncb_retcode);
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBCall(int nLana, LPTSTR szName, BYTE mark, LPTSTR szOther, BYTE markOther, BYTE bySendTO, BYTE byRecvTO, LPBYTE plsn, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	
	MakeNetbiosName(pncb->ncb_name, szName, mark);
	MakeNetbiosName(pncb->ncb_callname, szOther, markOther);
	pncb->ncb_rto = byRecvTO;
	pncb->ncb_sto = bySendTO;
	pncb->ncb_command = NCBCALL;
	pncb->ncb_lana_num = nLana;
	
	Netbios(pncb);
	
	if((NRC_GOODRET == pncb->ncb_retcode) && plsn)
		*plsn = pncb->ncb_lsn;
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBCall", "lsn %x lana %x ret %x\n",pncb->ncb_lsn,pncb->ncb_lana_num,pncb->ncb_retcode);
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBSend(int nLana, BYTE lsn, LPBYTE lpPacket, WORD cbLength, PNCB pncb) /*fold00*/
{
	ZeroMemory(pncb, sizeof(NCB));
	
	pncb->ncb_command = NCBSEND;
	pncb->ncb_lsn = lsn;
	pncb->ncb_lana_num = nLana;
	
	if(cbLength > 0xffff)
		RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);
	
	pncb->ncb_length = cbLength;
	pncb->ncb_buffer = lpPacket;
	
	Netbios(pncb);
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBSend", "lsn %x lana %x ret %x\n",pncb->ncb_lsn,pncb->ncb_lana_num,pncb->ncb_retcode);
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBEnum(PLANA_ENUM plae, WORD cbLength, PNCB pncb)
{
	ZeroMemory(pncb, sizeof(NCB));
	pncb->ncb_command = NCBENUM;
	
	if(cbLength < sizeof(LANA_ENUM))
		RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);
	
	pncb->ncb_length = cbLength;
	pncb->ncb_buffer = (UCHAR*)plae;
	
	Netbios(pncb);
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBEnum", "num of lana's %i ret %x\n",plae->length,pncb->ncb_retcode);
	return NRC_GOODRET == pncb->ncb_retcode;
}

BOOL NBCancel(int nLana, PNCB pncb)
{
	NCB ncb;
	
	ZeroMemory(&ncb, sizeof(NCB));
	
	ncb.ncb_command = NCBCANCEL;
	ncb.ncb_lana_num = nLana;
	ncb.ncb_length = sizeof(NCB);
	ncb.ncb_buffer = (UCHAR*)pncb;
	
	Netbios (&ncb);
	
	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Netbios", ReTracerCommands(Update), "NBCancel", "lsn %x lana %x ret %x\n",pncb->ncb_lsn,pncb->ncb_lana_num,pncb->ncb_retcode);
	return NRC_GOODRET == ncb.ncb_retcode;
}

