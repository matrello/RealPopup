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
#include "RPRealPopupProtocol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRPRealPopupProtocol::CRPRealPopupProtocol()
{
	m_rpApp=(CRealPopupApp *)AfxGetApp();
	MsgThreadID_AutoCnt=0;
	MsgThreadID_Cnt=0;
	isSendExecuteInProgress=false;
	IsProtocolEnabled=false;
	sendExecuteMsgQueue.LogType=sendExecuteMsgQueue.LogTypes::SendQueue;
}

CRPRealPopupProtocol::~CRPRealPopupProtocol()
{
}

void CRPRealPopupProtocol::Close()
{
	if (IsProtocolEnabled)
	{
		// send log out notification to rp workstations listed in nameresolution table
		NameResolutionLogOff();

		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPRealPopupProtocol.Close", "close handle and exit\n");
		mailslotServer.Close();
		IsProtocolEnabled=false;
	}
}

bool CRPRealPopupProtocol::Send(CMessaggio *Msg, CString *CurrentMsgsThreadID, bool isInternalMessage)
{
	if (!IsProtocolEnabled)
		return false;

	static CMessaggio lastMsg;
	static oldMsgThreadID_AutoCnt;
	unsigned int myMsgThreadID_AutoCnt=MsgThreadID_AutoCnt;

	if (isInternalMessage)
		SendExecuteMsgQueueAdd(Msg);
	else
	{
		// setup of MsgThreadID: create new one or derive from current "thread"
		// (thread is meant as "message thread between users")

		if (lastMsg.Msg.Compare(Msg->Msg)!=0 || lastMsg.Tutti.Compare(Msg->Tutti)!=0)
		{
			MsgThreadID_Cnt++;
			oldMsgThreadID_AutoCnt=MsgThreadID_AutoCnt;
		}
		else
			myMsgThreadID_AutoCnt=oldMsgThreadID_AutoCnt;

		if (CurrentMsgsThreadID==NULL)
		{
			Msg->MsgThreadID.Format("ID%s%x%x", m_rpApp->m_Workstation.Right(2), myMsgThreadID_AutoCnt,MsgThreadID_Cnt);
		}
		else
		{
			if (CurrentMsgsThreadID->GetLength()==0)
			{
				Msg->MsgThreadID.Format("ID%s%x%x", m_rpApp->m_Workstation.Right(2), myMsgThreadID_AutoCnt,MsgThreadID_Cnt);
			}
			else
				Msg->MsgThreadID=*CurrentMsgsThreadID;
		}

		SendExecuteMsgQueueAdd(Msg);
		lastMsg=*Msg;
	}

	return true;
}

int CRPRealPopupProtocol::Scan(CString *stMsg, CString *blMsg)
{	// scans for new messages and reacts in some way
	// sends msgs in send queue

	if (!IsProtocolEnabled)
		return 0;

	CString	sbuffer, pongDest, sVer="RealPopup %s %s build %s.";
	bool msgInterno, bScarta, bInviaSecondoPong=false;
	DWORD bufferSize=0, messagesWaiting=0;
	static CString	lastsbuffer = "";
	CMessaggio NuovoMessaggio;
	char *buffer=NULL;
	int counter=0;

	MsgThreadID_AutoCnt++;

	for (int q=0; q<sendExecuteMsgQueue.GetSize(); q++)
	{	// wait until no msg send in progress to same dest

		if (StrIndex(&SendExecuteDestinations,&sendExecuteMsgQueue[q].Dest)==-1)
		{
			m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Scan", "sending msg to %s (sendExecuteMsgQueue: %i)\n",sendExecuteMsgQueue[q].Dest,sendExecuteMsgQueue.GetSize());

			SendExecuteInvoke(&sendExecuteMsgQueue[q]);
			sendExecuteMsgQueue.RemoveAt(q);
			break;
		}
	}

	while (mailslotServer.Read(buffer, bufferSize, messagesWaiting))
	{
		if (bufferSize==0)
			break;

		--bufferSize;
		while ((--bufferSize)>0)
		{
			if (buffer[bufferSize]==0)
				buffer[bufferSize]=(char) MSG_BUFFER_ZERO_REPL;
		}

		sbuffer = buffer;
		if (lastsbuffer != sbuffer)
		{
			bScarta=false;
			lastsbuffer = sbuffer;
			UNFtoMsg(sbuffer, &NuovoMessaggio);

			for (int q=0;q<mMessaggi->GetSize();q++)
			{
				if (mMessaggi->GetAt(q).Mitt==NuovoMessaggio.Mitt)
				{
					if (mMessaggi->GetAt(q).Data==NuovoMessaggio.Data && mMessaggi->GetAt(q).Cmd==NuovoMessaggio.Cmd)
					{
						if (mMessaggi->GetAt(q).Msg==NuovoMessaggio.Msg)
						{
							m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Scan", "drop msg %s from %s about %s: duplicate\n",NuovoMessaggio.Cmd,NuovoMessaggio.Mitt, NuovoMessaggio.Data);
							bScarta=true;
						}
					}
				}
			}

			if (!bScarta)
			{	// handles reaction to msg just received
				
				// versions prior V2.31 use OEM charset
				if (GetRPVersion(&NuovoMessaggio.Version)<2.31)
				{
					NuovoMessaggio.Msg.OemToAnsi();
					m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Scan", "OemToAnsi applied to msg from %s version %s\n",NuovoMessaggio.Mitt,NuovoMessaggio.Version);
				}

				pongDest=NuovoMessaggio.Mitt;

				msgInterno=FALSE;

				m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Scan",
					"msg received from %s [cmd:%s, %s]: %s\n", 
					NuovoMessaggio.Mitt,(NuovoMessaggio.Cmd.GetLength()>0?NuovoMessaggio.Cmd:"none"),
					NuovoMessaggio.Version,NuovoMessaggio.Msg.Left(LOG_LEFTMSG));

				if (NuovoMessaggio.Cmd=="PING")
				{	// reaction to first msg received: PING

					CString listNameResolution;
					CMessaggio msgReply;
					msgReply.NParts="1";
					msgReply.Part="1";
					msgReply.Mitt=m_rpApp->m_Workstation;
					msgReply.Dest=NuovoMessaggio.Mitt;
					msgReply.Cmd="PONG";

					msgReply.Msg.Format("%i|%s|%s|%s|PONG -- RealPopup internal message",
							IsAwayEnabled,m_rpApp->m_Workstation,m_rpApp->m_UserDomain,m_rpApp->m_LoggedUser);

					msgReply.Version=m_rpApp->env_Version;

					if (NuovoMessaggio.Dest!=m_rpApp->m_Workstation)
						msgReply.Broadcast=NuovoMessaggio.Dest;

					Send(&msgReply, NULL, true);

					if (NuovoMessaggio.Msg.Find(MSG_RPNAMERESOLUTIONLOGOFF)>-1)
					{	// internal msg MSG_RPNAMERESOLUTIONLOGOFF is sent from
						// rp v2.5 prior shuttingdown to let other rp clients to
						// update their name resolution table by deleting user infos

						if (NameResolutionUpdate(&NuovoMessaggio, true))
						{
							CString UserName;
							AfxExtractSubString(UserName, NuovoMessaggio.Msg, 3, '|');
							stMsg->Format(MSG_OFFLINEWARNSTATUS, UserName, NuovoMessaggio.Mitt, NuovoMessaggio.Version);
						}
					}

					if (NuovoMessaggio.Msg.Find(MSG_RPNAMERESOLUTIONPING)>-1)
					{	// internal msg MSG_RPNAMERESOLUTIONPING is sent from
						// rp v2.x on start to let other rp clients to
						// update their name resolution table with sender infos

						if (NameResolutionUpdate(&NuovoMessaggio, false) && (NuovoMessaggio.Mitt.Compare(m_rpApp->m_Workstation)!=0))
						{	// Warn when online option

							SUser *myUser;

							myUser=m_Users->GetProfile(&NuovoMessaggio.Mitt, &NuovoMessaggio.Msg);
							if (myUser!=NULL)
							{
								if (myUser->chkOnlineWarn==1)
								{
									CString UserName;
									AfxExtractSubString(UserName, NuovoMessaggio.Msg, 3, '|');

									stMsg->Format(MSG_ONLINEWARNSTATUS, UserName, NuovoMessaggio.Mitt, NuovoMessaggio.Version);
									blMsg->Format(MSG_ONLINEWARN, UserName, NuovoMessaggio.Mitt);
								}
							}
						}
					}
					msgInterno=TRUE;
				}

				if (NuovoMessaggio.Cmd.Left(8)=="REALMITT")
					pongDest=NuovoMessaggio.Cmd.Mid(8);

				// rp2.0 detects and fixes previous rp1.x hidden 
				// msg "@FROM" which could contain false FROM info
				if (NuovoMessaggio.Cmd.Left(9)=="REALMITT@")
					NuovoMessaggio.Mitt=NuovoMessaggio.Cmd.Mid(9);

				
				if (NuovoMessaggio.Cmd.Left(8)=="REDIRECT")
				{	// handles message redirection to other rp clients

					int k=NuovoMessaggio.Cmd.Find('@',10);
					pongDest=NuovoMessaggio.Cmd.Mid(9,k-9);
				}

				if (NuovoMessaggio.Cmd=="VER")
				{	// reacts to hidden msg @VER: rp clients
					// reply with version information

					CMessaggio msgReply;
					msgReply.NParts="1";
					msgReply.Part="1";
					msgReply.Mitt=m_rpApp->m_Workstation;
					msgReply.Dest=NuovoMessaggio.Mitt;
					msgReply.Msg.Format(sVer,TIPO,m_rpApp->env_Version,BUILD);

					Send(&msgReply, NULL, true);

					bInviaSecondoPong=true;
					msgInterno=TRUE;
				}

				if (NuovoMessaggio.Cmd=="LOG")
				{	// reacts to hidden msg @LOG:
					// executes associated LOG command and sends back a confirmation msg
					CString msgReplyHead;
					CMessaggio msgReply;
					msgReplyHead.Format(sVer,TIPO,m_rpApp->env_Version,BUILD);
					msgReply.NParts="1";
					msgReply.Part="1";
					msgReply.Mitt=m_rpApp->m_Workstation;
					msgReply.Dest=NuovoMessaggio.Mitt;

#ifndef BETA
					if (NuovoMessaggio.Mitt!=m_rpApp->m_Workstation)
					{
						msgReply.Msg.Format("%s\n\n@LOG command is accepted from remote by beta releases only.", msgReplyHead);
						msgInterno=TRUE;
					}
					else
#endif
					{
						if (NuovoMessaggio.Msg.Left(11)=="@LOG|GETDAT")
						{
							CString datFile;
							m_Users->GetDATFile(&datFile);
							msgReply.Msg.Format("%s\n\nCommand OK: %s\n\n%s", msgReplyHead, NuovoMessaggio.Msg, datFile);
							msgInterno=TRUE;
						}
						if (NuovoMessaggio.Msg.Left(9)=="@LOG|SET|")
						{
							CString sPort;
							AfxExtractSubString(m_rpApp->opz_LogRemoteIP, NuovoMessaggio.Msg, 2, (char) '|');
							AfxExtractSubString(sPort, NuovoMessaggio.Msg, 3, (char) '|');
							if (sPort.GetLength()>0)
								m_rpApp->opz_LogRemotePort=atoi(sPort);
							else
								m_rpApp->opz_LogRemotePort=7777;
							m_rpApp->LogTerminate();
							if ((m_rpApp->opz_Log) || (m_rpApp->opz_LogRemote))
								m_rpApp->LogInitialize();

							msgReply.Msg.Format("%s\n\nCommand OK: %s", msgReplyHead, NuovoMessaggio.Msg);
							msgInterno=TRUE;
						}
						if (NuovoMessaggio.Msg.Left(7)=="@LOG|ON")
						{
							m_rpApp->opz_Log=TRUE;
							m_rpApp->opz_LogRemote=TRUE;
							m_rpApp->LogInitialize();

							msgReply.Msg.Format("%s\n\nCommand OK: %s", msgReplyHead, NuovoMessaggio.Msg);
							msgInterno=TRUE;
						}
						if (NuovoMessaggio.Msg.Left(8)=="@LOG|OFF")
						{
							m_rpApp->opz_Log=FALSE;
							m_rpApp->opz_LogRemote=FALSE;
							m_rpApp->LogTerminate();

							msgReply.Msg.Format("%s\n\nCommand OK: %s", msgReplyHead, NuovoMessaggio.Msg);
							msgInterno=TRUE;
						}
						if (!msgInterno || NuovoMessaggio.Msg=="@LOG")
						{
							msgReply.Msg.Format("%s\n\nAccepted @LOG commands:\n\n@LOG|ON\n@LOG|OFF\n@LOG|SET|<ip>|<port>\n@LOG|GETDAT\n\ncurrent configuration:\nlog %i logremote %i at %s:%i\nfilename: %s", 
								msgReplyHead,m_rpApp->opz_Log, m_rpApp->opz_LogRemote,
								m_rpApp->opz_LogRemoteIP, m_rpApp->opz_LogRemotePort, (m_rpApp->opz_Log==1?m_rpApp->oReTracer.fileName:"<none>"));

							msgInterno=TRUE;
						}
					}

					Send(&msgReply, NULL, true);
					bInviaSecondoPong=true;
				}

				if (NuovoMessaggio.Cmd=="PONG")
				{	// this is an additional update process for internal
					// name resolution table, activated once a PONG msg is
					// received from destination. it's needed because
					// sometimes broadcast msgs (used by MSG_RPNAMERESOLUTIONPING)
					// were not received by specific rp client

					NameResolutionUpdate(&NuovoMessaggio, false);
				}

				if (NuovoMessaggio.Cmd=="ABOUT")
				{	// reacts to hidden msg @ABOUT: rp clients
					// reply with hardcoded about information

					CMessaggio beta;
					beta.Mitt=NuovoMessaggio.Mitt;
					beta.Dest=m_rpApp->env_AutoMsgTo;
					// string should be crypted but anyway... :-)
					beta.Msg="RealPopup written by Matro, Rome (Italy), email: matro@realpopup.it.";
					beta.RiempiCampiData();
					mMessaggi->Add(beta);

					bInviaSecondoPong=true;
					msgInterno=TRUE;
				}

				if (NuovoMessaggio.Cmd=="RECERR")
				{	// rp1.x, not handled anymore
					msgInterno=TRUE;
				}

				if (NuovoMessaggio.Cmd=="ONLINEWARN")
				{	// rp1.x, not handled anymore [different handling]
					msgInterno=TRUE;
				}

				if (!msgInterno)
				{
					// this is where msg is read by rp
					if (Receive(&NuovoMessaggio) && NuovoMessaggio.Cmd!="PONG")
					{
						bInviaSecondoPong=true;
						counter+=1;
					}
				}

				if (bInviaSecondoPong)
				{	// sends the 2nd PONG to notify good msg reception

					CMessaggio msgReply;
					msgReply.NParts="1";
					msgReply.Part="1";
					msgReply.Mitt=m_rpApp->m_Workstation;
					msgReply.Dest=pongDest;
					msgReply.Cmd="PONG";
					msgReply.Msg="";
					
					msgReply.Version=m_rpApp->env_Version;

					if (NuovoMessaggio.Dest!=m_rpApp->m_Workstation)
						msgReply.Broadcast=NuovoMessaggio.Dest;

					Send(&msgReply, NULL, true);

					m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Scan", "2th PONG in send queue for %s\n", msgReply.Dest);
				}
			}
		}

	}

	return counter;
}

void CRPRealPopupProtocol::UNFtoMsg(CString UNFmsg, CMessaggio *NuovoMessaggio)
{
    AfxExtractSubString(NuovoMessaggio->NParts, UNFmsg, 0, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Part, UNFmsg, 1, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Mitt, UNFmsg, 2, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Dest, UNFmsg, 3, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Tutti, UNFmsg, 4, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Cmd, UNFmsg, 5, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Msg, UNFmsg, 6, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Broadcast, UNFmsg, 7, (char) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Version, UNFmsg, 8, (char) MSG_BUFFER_ZERO_REPL);
    // 9th field contains the "DistinguishChar" not read here
    AfxExtractSubString(NuovoMessaggio->MsgThreadID, UNFmsg, 10, (char) MSG_BUFFER_ZERO_REPL);
	NuovoMessaggio->RiempiCampiData();
}

bool CRPRealPopupProtocol::Init()
{
	if (!IsProtocolEnabled)
		IsProtocolEnabled=mailslotServer.Open(true, "realp");

	return IsProtocolEnabled;
}

void CRPRealPopupProtocol::Ping(CMessaggio *Msg)
{	// PING internal msg is sent prior real msg sending;
	// a PONG is expected from destination

	CMessaggio msgReply;
	msgReply.NParts="1";
	msgReply.Part="1";
	msgReply.Mitt=m_rpApp->m_Workstation;
	msgReply.Dest=Msg->Dest;
	msgReply.Cmd="PING";
	msgReply.Msg="PING -- RealPopup internal message";

	Send(&msgReply, NULL, true);
}

bool CRPRealPopupProtocol::Pong(CPenMessaggio *Msg)
{	// returns true if a PONG msg is received
	// for specified message, and removes it from
	// received messages queue

	// PONG internal msg is sent in reaction of PING
	// and once msg body is succesfully retrieved

	int k;

	for (k=0; k<mMessaggi->GetSize(); k++)
	{
		//CMessaggio testItem = mMessaggi->GetAt(k); // item watch debug

		if ( (mMessaggi->GetAt(k).Cmd == "PONG") &&
			 ((mMessaggi->GetAt(k).Mitt == Msg->Msg.Dest) || (mMessaggi->GetAt(k).Broadcast == Msg->Msg.Dest)) )
		{
			Msg->Msg.Version=mMessaggi->GetAt(k).Version;
			Msg->PongMsg=mMessaggi->GetAt(k).Msg;
			Msg->PongVersion=mMessaggi->GetAt(k).Version;
			Msg->PongMitt=mMessaggi->GetAt(k).Mitt;
			
			if (mMessaggi->GetAt(k).Broadcast.GetLength()>0)
				Msg->Msg.Broadcast=mMessaggi->GetAt(k).Broadcast;

			m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Pong", "PONG received from %s (%s)\n", mMessaggi->GetAt(k).Mitt, Msg->Msg.Dest);

			mMessaggi->RemoveAt(k);

			return true;
		}
	}
	
	return false;
}

UINT SendExecuteSinglePart(CMessaggio *Msg)
{	// low level send routine

	CSharedMailslot sendMailslot;

	Msg->Version=((CRealPopupApp *)AfxGetApp())->env_Version;

	// note: if new internal command is added, modify IsBackdoor() also
	CString backdoorCmd;
	if (Msg->Msg=="@VER")
	{
		Msg->Msg="";
		Msg->Cmd="VER";
	}
	if (Msg->Msg=="@ABOUT")
	{
		Msg->Msg="";
		Msg->Cmd="ABOUT";
	}
	if (Msg->Msg.Left(4)=="@LOG")
		Msg->Cmd="LOG";

	/*
	the following hidden commands are not handled anymore since rp2.x
	
	backdoorCmd=Msg->Msg.Left(5);
	if (backdoorCmd=="@FROM")
	{
		backdoorCmd=Msg->Msg.Mid(5,20);
		int k = backdoorCmd.Find('@');
		if (k>0)
		{
			Msg->Cmd="REALMITT@"+Msg->Mitt;
			Msg->Mitt=backdoorCmd.Left(k);
			backdoorCmd=Msg->Msg.Mid(k+6);
			Msg->Msg=backdoorCmd;
		}
	}*/

	ReTracer(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "SendExecuteSinglePart", "sending msg to %s part %s: %s\n",Msg->Dest,Msg->Part, Msg->Msg.Left(LOG_LEFTMSG));
	
	// since rp2.x, a "DistinguishChar" is added to mailslot msg body: it seems
	// sometimes msg with same body were not sent by the system, so the DistinguishChar
	// makes a msg different from another
	static char DistinguishChar=33;
	if (DistinguishChar++==80) DistinguishChar=33;

	// versions prior V2.31 use OEM charset
	if (Msg->DestVersion.GetLength()>0)
		if (GetRPVersion(&Msg->DestVersion)<2.31)
			Msg->Msg.AnsiToOem();

	CString UNFmsg = Msg->NParts + (char) 0 + Msg->Part + (char) 0 +
		Msg->Mitt + (char) 0 + Msg->Dest + (char) 0 + 
		Msg->Tutti + (char) 0 + Msg->Cmd + (char) 0 + Msg->Msg + (char) 0 +
		Msg->Broadcast + (char) 0 + Msg->Version + 
		(char) 0 + DistinguishChar + (char) 0 + Msg->MsgThreadID + (char) 0;

	sendMailslot.Write(UNFmsg.GetBuffer(), UNFmsg.GetLength()+1, "realp", Msg->Dest.GetBuffer());

	UNFmsg.ReleaseBuffer();
	Msg->Dest.ReleaseBuffer();

	return 0;
}

UINT SendExecute( LPVOID pParam)
{	// middle level send routine (handles multiparts)

	SendExecuteInfo *sendExecuteInfo= (SendExecuteInfo *)pParam;
	
	CMessaggio *tmpMsg=sendExecuteInfo->Msg;
	CMessaggio Msg;

	tmpMsg->Copy(&Msg);
	*sendExecuteInfo->isSendExecuteInProgress=false;

	// no multiple destination field set if they're more than MAX_SEND_LEN chars
	if (Msg.Tutti.GetLength()>MAX_SEND_LEN)
		Msg.Tutti=Msg.Dest;

	CMessaggio msgSingolo;

	long msgLen=Msg.Msg.GetLength();
	long bytesSent=0;
	char myNParts[5];
	char myPart[5];
	int NParts=0,Part=0;

	int msgSize=MSGREALPOPUP_MULTIPART_MSGSIZE-
		Msg.Broadcast.GetLength()-
		Msg.Cmd.GetLength()-
		Msg.Data.GetLength()-
		Msg.Dest.GetLength()-
		Msg.Mitt.GetLength()-
		Msg.Tutti.GetLength()-
		Msg.Version.GetLength() - 20;
	
	Msg.Copy(&msgSingolo);

	NParts=(msgLen/msgSize)+1;
	do
	{
		msgSingolo.NParts= _itoa(NParts,myNParts,10);
		msgSingolo.Part= _itoa(++Part,myPart,10);
		msgSingolo.Msg=Msg.Msg.Mid(bytesSent,msgSize);
		SendExecuteSinglePart(&msgSingolo);
		bytesSent+=msgSize;
		Sleep(100);
	} while (bytesSent<msgLen);

	ReTracer(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "SendExecute", "msg to %s sent: %s\n",Msg.Dest, Msg.Msg.Left(LOG_LEFTMSG));

	sendExecuteInfo->SendExecuteDestinations->RemoveAt(StrIndex(sendExecuteInfo->SendExecuteDestinations,&msgSingolo.Dest));

	return 0;
}

bool CRPRealPopupProtocol::SendExecuteInvoke(CMessaggio *Msg)
{	// invokes the SendExecute function

	// msg is sent with separate thread, because sometimes
	// (specially on win2k/xp machines) the WriteFile()
	// mailslot routine takes lots of seconds to run

	long myWait=0;

	isSendExecuteInProgress=true;
	SendExecuteDestinations.Add(Msg->Dest);

	sendExecuteInfo.isSendExecuteInProgress=&isSendExecuteInProgress;
	sendExecuteInfo.SendExecuteDestinations=&SendExecuteDestinations;
	sendExecuteInfo.Msg=Msg;

	AfxBeginThread(SendExecute,&sendExecuteInfo);

	while (isSendExecuteInProgress)
	{
		myWait++;	// logging purposes
		Sleep(40);
	}

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.SendExecuteInvoke", "SendExecute for %s ended OK (%i wait loop)\n",Msg->Dest,myWait);

	return true;
}

void CRPRealPopupProtocol::SendExecuteMsgQueueAdd(CMessaggio *Msg)
{
	int k=sendExecuteMsgQueue.GetSize();
	if (k>0)
	{
		while (sendExecuteMsgQueue[k-1].Priority > Msg->Priority)
		{
			k--;
			if (k==0) break;
		};
		sendExecuteMsgQueue.InsertAt(k,*Msg);
	}
	else
		sendExecuteMsgQueue.Add(*Msg);

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.SendExecuteMsgQueueAdd", "msg to %s added in sendExecuteMsgQueue with priority %i\n", Msg->Dest, Msg->Priority);
}

bool CRPRealPopupProtocol::Receive(CMessaggio *Msg)
{
	bool msgArrivatoTutto=true;
	int part, nparts;
	nparts=	atoi(Msg->NParts);
	part= atoi(Msg->Part);

	if (nparts > 1)
	{
		for (int k=0;k<mMsgMultipart.GetSize();k++)
		{
			if (mMsgMultipart[k].Mitt==Msg->Mitt)
			{
				int partMulti, npartsMulti;
				npartsMulti=atoi(LPCTSTR(mMsgMultipart[k].NParts));
				partMulti=	atoi(LPCTSTR(mMsgMultipart[k].Part));

				m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.Receive",
					"multipart msg from %s, part %i/%i (expected %i/%i)\n",
					mMsgMultipart[k].Mitt,part,nparts,partMulti+1, npartsMulti);

				if ( (npartsMulti!=nparts) || (part != (partMulti+1) ) )
				{	// different message part received than expected

					/* rp1.x handled this situation by notifying the
					   user with specific error msg. rp2.x easily
					   timeouts the sender. consider this is a rare
					   (but possible, stating ms guidelines) situation.
					
					mMsgMultipart[k].Cmd="RECERR";
					if (nparts==part)
					{
						CString myMsg, myFrom;
						myMsg.LoadString(MSG_RECEIVE_MULTIPARTERROR);
						myFrom.LoadString(AUTOMSG_BETA_FROM);
						mMsgMultipart[k].Msg.Format(myMsg,mMsgMultipart[k].Mitt);

						mMsgMultipart[k].Msg.Format("multipart %i di %i, expected %i di %i",
							part,nparts,partMulti+1,npartsMulti);
						mMsgMultipart[k].Mitt=myFrom;
						mMessaggi->Add(mMsgMultipart[k]);

						//	mMsgMultipart[k].Msg=" ";
						//	mMsgMultipart[k].Mitt=((CRealPopupApp *)AfxGetApp())->m_Workstation;
						//	mMsgMultipart[k].Dest=Msg->Mitt;
						//
						//	sendExecuteMsgQueue.Add(mMsgMultipart[k]);
						
						mMsgMultipart.RemoveAt(k);
						return false;
					}
					else
					{
						/*	 PER IL DEBUG DEI MULTIPART 
						CString myMsg;
						myMsg.Format("\n\nmultipart PART %i di %i, expected %i di %i",
							part,nparts,partMulti+1,npartsMulti);
						mMsgMultipart[k].Msg=myMsg+mMsgMultipart[k].Msg;
						mMessaggi->Add(mMsgMultipart[k]);
					*/

					mMsgMultipart.RemoveAt(k);
					msgArrivatoTutto=false;
				}
				else
				{
					mMsgMultipart[k].Msg+=Msg->Msg;

					if (++partMulti == npartsMulti)
					{
						ReceiveMsgAdd(&mMsgMultipart.GetAt(k));
						mMsgMultipart.RemoveAt(k);
						return true;
					}
					else
					{

		/*	 multipart debug
		CString myMsg;
		myMsg.Format("\n\nmultipart PARTOK %i di %i\n\n",partMulti,npartsMulti);
		mMsgMultipart[k].Msg=myMsg+mMsgMultipart[k].Msg;
		mMessaggi->Add(mMsgMultipart[k]);
		*/

						char myPart[5];

						mMsgMultipart[k].Part = _itoa(partMulti,myPart,10);
						msgArrivatoTutto=false;
					}
				}

				break;
			}
		}

		if (k==mMsgMultipart.GetSize())
		{
			mMsgMultipart.Add(*Msg);
			msgArrivatoTutto=false;
		}
	}
	else
	{
		ReceiveMsgAdd(Msg);
		if (IsIgnoredThread(Msg))
			mMessaggi->RemoveAt(mMessaggi->GetSize()-1);
	}

	return msgArrivatoTutto;
}

void CRPRealPopupProtocol::ReceiveMsgAdd(CMessaggio *NuovoMessaggio)
{	// adds in mMessaggi and auto response with 'I'm away' or 'Do not disturb'

	if ( ((!IsNotificationEnabled) || IsAwayEnabled ) && NuovoMessaggio->Cmd!="PONG" &&
		 !mMessaggi->IsDuplicate(*NuovoMessaggio) && !NetworkDomains->IsDomain(NuovoMessaggio->Dest))
	{
		CString ReplyTo;
		ReplyTo.LoadString(MSG_ERR_REPLY);

		CString msg;
		CMessaggio msgReply;

		msg.LoadString((IsAwayEnabled)?MSG_ERRAWAY:MSG_ERRDISABLED);
		if (NuovoMessaggio->Cmd.Left(8)=="REALMITT")
		{
			msgReply.Dest=NuovoMessaggio->Cmd.Mid(8);
			msgReply.Tutti=NuovoMessaggio->Mitt;
			msgReply.Mitt=m_rpApp->m_LoggedUser;
			msgReply.Msg.Format(msg,m_rpApp->env_Version,m_rpApp->m_LoggedUser);
		}
		else
		{
			msgReply.Dest=NuovoMessaggio->Mitt;
			msgReply.Mitt=m_rpApp->m_Workstation;
			msgReply.Msg.Format(msg,m_rpApp->env_Version,m_rpApp->m_Workstation);
		}

		msgReply.Cmd=((IsAwayEnabled)?"NOTREAD":"NOTRECEIVED");
		msgReply.Broadcast=NuovoMessaggio->Broadcast;
		
		if (!IsAwayEnabled)
		{
			msgReply.Msg+=ReplyTo;
			msgReply.Msg+=NuovoMessaggio->Msg;
		}

		Send(&msgReply, NULL, true);
	}

	if (IsNotificationEnabled)
		mMessaggi->Add(*NuovoMessaggio);

	return;
}

void CRPRealPopupProtocol::NameResolutionPing()
{	// broadcasts MSG_RPNAMERESOLUTIONPING to all rp clients
	// to allow them to update their name resolution list

	if (!m_rpApp->opz_chkFullInternalPings)
	{	
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "no PING to domains(registry set FullInternalPings=0)\n");
		return;
	}

	if (!m_rpApp->opz_chkNameResolution)
	{	
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "no PING to domains(registry set NameResolution=0)\n");
		return;
	}

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "sending PING to domains\n");

	for (int k=0;k<NetworkDomains->Domains.GetSize();k++)
	{
		CMessaggio nrPing;
		nrPing.NParts="1";
		nrPing.Part="1";
		nrPing.Mitt=m_rpApp->m_Workstation;
		nrPing.Dest=NetworkDomains->Domains[k];
		nrPing.Cmd="PING";
		nrPing.Msg.Format("%i|%s|%s|%s|%s -- RealPopup broadcast internal message",
				IsAwayEnabled,m_rpApp->m_Workstation,m_rpApp->m_UserDomain,m_rpApp->m_LoggedUser,MSG_RPNAMERESOLUTIONPING);

		nrPing.Priority=MSG_PRIORITY_LOW;
		Send(&nrPing, NULL, true);
	}

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "sending PING to WorkstationsRank\n");

	for (k=0;k<m_rpApp->oWorkstationsRank.GetSize();k++)
	{
		CMessaggio nrPing;
		nrPing.NParts="1";
		nrPing.Part="1";
		nrPing.Mitt=m_rpApp->m_Workstation;
		nrPing.Dest=m_rpApp->oWorkstationsRank.GetAt(k);
		nrPing.Cmd="PING";
		nrPing.Msg.Format("%i|%s|%s|%s|%s -- RealPopup internal message",
				IsAwayEnabled,m_rpApp->m_Workstation,m_rpApp->m_UserDomain,m_rpApp->m_LoggedUser,MSG_RPNAMERESOLUTIONPING);

		nrPing.Priority=MSG_PRIORITY_LOW;
		Send(&nrPing, NULL, true);
	}

}

void CRPRealPopupProtocol::NameResolutionLogOff()
{	// broadcasts MSG_RPNAMERESOLUTIONLOGOFF to all rp clients
	// to allow them to update their name resolution list

	if (!m_rpApp->opz_chkFullInternalPings)
	{	
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "no PING to domains(registry set FullInternalPings=0)\n");
		return;
	}

	if (!m_rpApp->opz_chkNameResolution)
	{	
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "no PING to domains(registry set NameResolution=0)\n");
		return;
	}

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionPing", "sending PING to LastDestinations for LogOff notification\n");

	vector<string>  myNR;
	myNR=m_names.Keys();
	int myWait=0;

	for (unsigned int k=0;k<m_names.Count();k++)
	{
		CMessaggio nrPing;
		nrPing.NParts="1";
		nrPing.Part="1";
		nrPing.Mitt=m_rpApp->m_Workstation;
		nrPing.Dest=myNR[k].c_str();
		nrPing.Cmd="PING";
		nrPing.Msg.Format("%i|%s|%s|%s|%s -- RealPopup internal message",
				IsAwayEnabled,m_rpApp->m_Workstation,m_rpApp->m_UserDomain,m_rpApp->m_LoggedUser,MSG_RPNAMERESOLUTIONLOGOFF);

		nrPing.Priority=MSG_PRIORITY_HIGH;
		Send(&nrPing, NULL, true);
	}

	while (SendExecuteDestinations.GetSize()>0)
	{
		myWait++;
		Sleep(40);
	}

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionLogOff", "PING for LogOff notifications sent (%i wait loop)\n",myWait);
}

bool CRPRealPopupProtocol::NameResolutionUpdate(CMessaggio *msg, bool remove)
{	// updates internal name resolution list

	if (GetRPVersion(&msg->Version)>=1.578 &&	// previous rp releases didn't
		m_rpApp->opz_chkNameResolution)			// send required informations
	{										
		CString UserName;
		AfxExtractSubString(UserName, msg->Msg, 3, '|');

		if (UserName.GetLength()>0)	// UserName should always exists
		{
			if (m_names.Exists(LPCSTR(msg->Mitt)))
			{
				if (remove)
				{
					m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(List), "NameResolution", ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionUpdate", "%s [--]\n", msg->Mitt);
					m_names.Remove(LPCSTR(msg->Mitt));
					return true;
				}
				else
				{
					string actUserName=m_names.Item(LPCSTR(msg->Mitt));
					if (UserName.Compare(actUserName.c_str())!=0)
					{
						m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(List), "NameResolution", ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionUpdate", "%s [%s]\n", msg->Mitt, UserName);

						m_names.Remove(LPCSTR(msg->Mitt));
						m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(List), "NameResolution", ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionUpdate", "%s [%s]\n", msg->Mitt, UserName);
						m_names.Add(LPCSTR(msg->Mitt),LPCSTR(UserName));

						return true;
					}
				}
			}
			else
			{
				if (!remove)
				{
					m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(List), "NameResolution", ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionUpdate", "%s [%s]\n", msg->Mitt, UserName);
					m_names.Add(LPCSTR(msg->Mitt),LPCSTR(UserName));

					return true;
				}
			}
		}
	}

	return false;
}

bool CRPRealPopupProtocol::NameResolutionIsNameOnly(CString *dest)
{
	if (m_rpApp->opz_chkNameResolution)
	{	
		vector<string>	myNR;
		vector<string>  myNRitems;
		myNR=m_names.Keys();
		myNRitems=m_names.Items();
		for (unsigned int i=0; i<m_names.Count(); i++)
		{
			if ( (dest->Compare(myNRitems[i].c_str())==0) && 
				(dest->Compare(myNR[i].c_str())!=0) )
				return true;
		}
	}
	
	return false;
}

void CRPRealPopupProtocol::NameResolutionDo(CStringArray *computers, CString *dest)
{	// returns workstations associated to specified user

	if (m_rpApp->opz_chkNameResolution)
	{	
		vector<string>	myNR;
		vector<string>  myNRitems;
		myNR=m_names.Keys();
		myNRitems=m_names.Items();

		for (unsigned int i=0; i<m_names.Count(); i++)
		{
			m_rpApp->oReTracer.Trace(ReTracerTraceLevels(DebugVerbose), ReTracerControlTypes(List), "NameResolution", ReTracerCommands(Update), "CRPRealPopupProtocol.NameResolutionDo", "%s [%s]\n", myNR[i].c_str(), myNRitems[i].c_str());
			if (dest->Compare(myNRitems[i].c_str())==0)
				computers->Add(myNR[i].c_str());
		}
	}
}

bool CRPRealPopupProtocol::IsIgnoredThread(CMessaggio *NuovoMessaggio)
{
	for (int k=0;k<IgnoredThreads->GetSize();k++)
	{
		CString test;
		test=IgnoredThreads->GetAt(k);
		if (NuovoMessaggio->MsgThreadID.Compare(IgnoredThreads->GetAt(k))==0)
			return true;
	}

	return false;
}

void CRPRealPopupProtocol::NameResolutionGetList(CString *list)
{
	/*		this routine builds a string with name resolution table
			list, in the format PC§USER,PC§USER...

			written with v2.41, never used.
	*/
		vector<string>	myNR;
		vector<string>  myNRitems;
		myNR=m_names.Keys();
		myNRitems=m_names.Items();
		for (unsigned int i=0; i<m_names.Count(); i++)
		{
			list->Append(myNR[i].c_str());
			list->Append("§");
			list->Append(myNRitems[i].c_str());
			list->Append(",");
		}
}
