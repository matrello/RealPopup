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
#include <direct.h>
#include <winsvc.h>
#include "realpopup.h"
#include "RPProtocolsHandler.h"
#include "RPNetworkBrowser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRPProtocolsHandler::CRPProtocolsHandler()
{
	m_rpApp=(CRealPopupApp *)AfxGetApp();

	WinPopupProtocol.mMessaggi = &Messaggi;
	WinPopupProtocol.NetworkDomains=&NetworkDomains;
	RealPopupProtocol.mMessaggi = &Messaggi;
	RealPopupProtocol.NetworkDomains=&NetworkDomains;
	RealPopupProtocol.IgnoredThreads = &IgnoredThreads;
	IsRealPopupProtocolEnabled=false;
	IsWinPopupProtocolEnabled=false;
	isNotificationEnabled=true;
	isAwayEnabled=false;
	RealPopupProtocol.IsAwayEnabled=false;
	RealPopupProtocol.IsNotificationEnabled=true;
	SF_ScanInProgress=false;
	m_NameResolutionRequested=false;
	m_NameResolutionIntervalCounter=0;
	m_ServiceStoppedByRp=false;
	Messaggi.LogType=Messaggi.LogTypes::Messaggi;
}

void CRPProtocolsHandler::Close(bool warm)
{
	WinPopupProtocol.Close();
	RealPopupProtocol.Close();
	IsRealPopupProtocolEnabled=IsWinPopupProtocolEnabled=false;

	if (!warm)
		HandleMessengerService(true);

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update),
		"CRPProtocolsHandler::Close", "%s close: RealPopupProtocol %s, WinPopupProtocol %s\n", (warm?"Warm":"Cold"), (IsRealPopupProtocolEnabled?"enabled":"disabled"), (IsWinPopupProtocolEnabled?"enabled":"disabled") );
}


int CRPProtocolsHandler::Scan()
{	// scans for new messages (high level routine)

	int NuoviMessaggi=0, ret=-1;

	if (!SF_ScanInProgress)
	{
		static int lastNumMessaggi=0;

		SF_ScanInProgress=true;

		if (IsRealPopupProtocolEnabled)
		{
			CString stMsg, blMsg;
			
			if (RealPopupProtocol.Scan(&stMsg, &blMsg)==-1)
			{
				IsRealPopupProtocolEnabled=false;
				ErrMsg(AfxGetMainWnd(),MSG_REALPOPUP_ERRINIT);
			}

			if (stMsg.GetLength()>0)
				m_wndStatusBar->SetWindowText(stMsg);
			if (blMsg.GetLength()>0)
				ShowInternalMessage(&blMsg);

			Pending();
		}

		if (IsWinPopupProtocolEnabled)
		{
			if (WinPopupProtocol.Scan()==-1)
			{
				IsWinPopupProtocolEnabled=false;
				ErrMsg(AfxGetMainWnd(),MSG_POPUP_ERRINIT);
			}
		}

		MessaggiFiltra();

		NuoviMessaggi=Messaggi.GetSize()-lastNumMessaggi;
		if (NuoviMessaggi>0)
			ret = lastNumMessaggi;
		lastNumMessaggi = Messaggi.GetSize();

		if (ret>-1)
		{
			if (*m_RedirectEnabled)
				Redirect(ret);
			LogMsgIn(ret);
		}
		
		SF_ScanInProgress=false;
	}

	return (ret);
}

bool CRPProtocolsHandler::Send(CMessaggio *Msg, CString *CurrentMsgsThreadID)
{	// sends msgs (high level routine)

	bool ret=false, IsDomain=NetworkDomains.IsDomain(Msg->Dest);

	if (m_rpApp->opz_chkWarnDomain && IsDomain)
	{
		CString msg;
		CString caption;
		msg.Format(MSG_WARNDOMAIN,Msg->Dest);
		caption.LoadString(MSG_DIALOG_CAPTION);
		if (AfxGetMainWnd()->MessageBox(msg,caption,MB_ICONQUESTION+MB_YESNO)==IDNO)
			return false;
	}

	SUser *profile;
	profile=m_Users->GetProfile(&Msg->Dest, &Msg->Msg);
	if (profile==NULL)
		return false;

	if (IsRealPopupProtocolEnabled && 		
		!(profile->chkUsesRealPopup==1 && profile->cboUsesRealPopup==1))
	{
		bool duplicato,testduplicato=true;
		CStringArray computers;
		CString actComputer,ctrlComputer;
		CString oldDest=Msg->Dest;

		// handles name resolution
		RealPopupProtocol.NameResolutionDo(&computers,&Msg->Dest);

		if (computers.GetSize()==0)
		{
			computers.Add(Msg->Dest);
			testduplicato=false;
		}

		for (int k=0;k<computers.GetSize();k++)
		{
			// checks for duplicates
			duplicato=false;
			if (testduplicato && computers.GetAt(k).CompareNoCase(Msg->Dest)!=0)
			{
				for (int q=0; AfxExtractSubString(ctrlComputer, Msg->Tutti, q, (char) ','); q++)
				{
					if (computers.GetAt(k).Compare(ctrlComputer)==0)
					{
						duplicato=true;
						break;
					}
				}
			}

			if (!duplicato)
			{
				Msg->Dest=computers[k];
				if (IsDomain)	// no PING/PONG when sending to domains
					ret=RealPopupProtocol.Send(Msg);
				else
				{
					if (Msg->Dest.Compare(m_rpApp->m_Workstation)!=0)
						m_rpApp->oWorkstationsRank.RankAdd(Msg->Dest);

					Ping(Msg, &oldDest, false, CurrentMsgsThreadID);
					ret=true;
				}

				// if allowed by user options, sends also through winpopup protocol if this is a broadcast msg
				if (IsDomain && IsWinPopupProtocolEnabled && !m_rpApp->opz_chkDomainSendRP && Msg->Msg!="@VER")
					WinPopupProtocol.Send(Msg);
			}
		}
		Msg->Dest=oldDest;
	}
	else
	{
		if (IsWinPopupProtocolEnabled)
		{
			CString stMsg;

			// send through winpopup protocol: check for msg length
			if (!(m_rpApp->opz_chkLenChk) && Msg->Msg.GetLength()>PROTOCOL_MSGPOPUP_MAXLEN)
			{
				CString ReplyTo;
				ReplyTo.LoadString(MSG_ERR_REPLY);

				CMessaggio errMsg;
				errMsg.Dest=m_rpApp->env_AutoMsgTo;
				errMsg.Mitt=Msg->Dest;
				errMsg.Msg.Format(MSG_ERRTOOLONG, m_rpApp->env_Version, Msg->Dest, Msg->Msg.GetLength());
				errMsg.Msg += ReplyTo;
				errMsg.Msg += Msg->Msg;
				errMsg.Cmd="NOTRECEIVED";

				Messaggi.Add(errMsg);
				stMsg.Format(MSG_STATUS_TOOLONG,Msg->Dest);
				m_wndStatusBar->SetWindowText(stMsg);

				return true;
			}
			else
			{
				ret = WinPopupProtocol.Send(Msg);
				LogMsgWrite(Msg,false);
				stMsg.Format(MSG_STATUS_WPSENT,Msg->Dest);
				m_wndStatusBar->SetWindowText(stMsg);
				m_rpApp->opz_MsgSentNum++;
			}
		}
	}

	return ret;
}

bool CRPProtocolsHandler::Init(bool bShowErrorMessage, bool warm)
{
	if (!IsWinPopupProtocolEnabled)
	{
		bool bOk=true;

		if (!warm)
		{
			if (!HandleMessengerService(false))
			{
				bOk=false;
				if (bShowErrorMessage)
				{
					CString stHead, stMsg, stBalloonMsg;
					stHead.LoadString(TOOLTIP_STATUS_WPDISABLED);
					stMsg.LoadString(MSG_POPUP_ERRMSGSERVICE);
					stBalloonMsg.LoadString(MSG_BALLOON_POPUP_ERRMSGSERVICE);
					ShowInternalMessage(&stMsg, &stHead, NIIF_WARNING, &stBalloonMsg);
				}
			}
		}

		if (bOk && (m_rpApp->opz_Msn!=OPZ_MSN_LEAVE))
		{
			if (WinPopupProtocol.Init())
				IsWinPopupProtocolEnabled = true;
			else
			{
				if (bShowErrorMessage)
				{
					CString stHead, stMsg, stBalloonMsg;
					stHead.LoadString(TOOLTIP_STATUS_WPDISABLED);
					stMsg.LoadString(MSG_POPUP_ERRINIT);
					stBalloonMsg.LoadString(MSG_BALLOON_POPUP_ERRINIT);
					ShowInternalMessage(&stMsg, &stHead, NIIF_WARNING, &stBalloonMsg);
				}
			}
		}
	}

	if (!IsRealPopupProtocolEnabled)
	{
		if (RealPopupProtocol.Init())
			IsRealPopupProtocolEnabled = true;
		else
		{
			if (bShowErrorMessage && m_rpApp->opz_chkWarnRPProtocolNotActive)
			{
				CString stHead, stMsg, stBalloonMsg;
				stHead.LoadString(TOOLTIP_STATUS_RPDISABLED);
				stMsg.LoadString(MSG_REALPOPUP_ERRINIT);
				stBalloonMsg.LoadString(MSG_BALLOON_REALPOPUP_ERRINIT);
				ShowInternalMessage(&stMsg, &stHead, NIIF_WARNING, &stBalloonMsg);
			}
		}
	}

	m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update),
		"CRPProtocolsHandler::Init", "%s init: RealPopupProtocol %s, WinPopupProtocol %s, Messenger service action: %s\n",
		(warm?"Warm":"Cold"), (IsRealPopupProtocolEnabled?"enabled":"disabled"), (IsWinPopupProtocolEnabled?"enabled":"disabled"), (m_rpApp->opz_Msn==OPZ_MSN_LEAVE?"leave":(m_rpApp->opz_Msn==OPZ_MSN_UNDEFINED?"undefined":"take")) );

	return (IsWinPopupProtocolEnabled && IsRealPopupProtocolEnabled);
}

bool CRPProtocolsHandler::HandleMessengerService(bool Restart)
{
	// handle Messenger Service start/stop based on user preference;
	// returns True if WinPopup protocol can be initialized.
	// the return value is irrelevant if Messenger must be restarted (Restart=true).

	if ( (!Restart && (IsNT() && (m_rpApp->opz_Msn!=OPZ_MSN_LEAVE))) || (Restart && m_ServiceStoppedByRp) )
	{	// if allowed, stops messenger service. it will be
		// started again when rp is closed. to start/stop
		// services, privilege SE_LOAD_DRIVER_NAME is required.
		// it's usually granted to Power Users group, but this
		// depends on specific Windows release and security fixes.

		DWORD dwState = 0xFFFFFFFF, dwErr = 0xFFFFFFFF;
		HANDLE hToken;

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			if (EnablePrivilege(hToken, SE_LOAD_DRIVER_NAME, TRUE))
				m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "privilege SE_LOAD_DRIVER_NAME ok\n");
			else
				m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "privilege SE_LOAD_DRIVER_NAME ko\n");

			SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
			if (hSCM==NULL)
				m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "OpenSCManager: null\n");
			else
			{
				if (Restart)
				{
					// if rp stopped messenger service, it starts it again
					SC_HANDLE hService = OpenService(hSCM, "Messenger", SERVICE_START);
					if (hService==NULL)
						m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "OpenService: null\n");
					else
					{
						BOOL b = StartService(hService, 0, NULL);
						if(!b) 
							m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "attempt to start it again failed\n");
						else
							m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "started it again (won't wait for it)\n");

						CloseServiceHandle(hService);
					}
					CloseServiceHandle(hSCM);
				}
				else
				{
					// ok, let's stop messenger service. it's quite dirty, as
					// it should be an async process, but it works.
					SC_HANDLE hService = OpenService(hSCM, "Messenger", SERVICE_QUERY_STATUS | SERVICE_STOP);
					if (hService==NULL)
						m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "OpenService: null\n");
					else
					{
						SERVICE_STATUS ss;
						memset(&ss, 0, sizeof(ss));
						BOOL b = QueryServiceStatus(hService, &ss);
						if(b)
						{
							dwErr = ss.dwWin32ExitCode;
							dwState = ss.dwCurrentState;

							if (dwState == SERVICE_RUNNING) 
							{
								if (m_rpApp->opz_Msn==OPZ_MSN_UNDEFINED)
								{
									// opens the CDlgService dialog to let the user decide whether
									// to take control of Messenger Service or not
									CDlgService dlg;
									if (dlg.DoModal()==IDOK)
										m_rpApp->opz_Msn=dlg.m_Msn;
								}

								if (m_rpApp->opz_Msn==OPZ_MSN_TAKECONTROL)
								{
									// stops the service
									SERVICE_STATUS ss;
									BOOL b = ControlService(hService, SERVICE_CONTROL_STOP, &ss);
									if(!b) 
									{
										m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "(takecontrol on) attempt to stop service failed\n");
										return false;
									}
									else
									{
										m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "attempt to stop...\n");
										SERVICE_STATUS ssPen;
										memset(&ssPen, 0, sizeof(ssPen));
										BOOL b = QueryServiceStatus(hService, &ssPen);
										dwState = ssPen.dwCurrentState;
										while (b && dwState==SERVICE_STOP_PENDING)
										{
											BOOL b = QueryServiceStatus(hService, &ssPen);
											dwState = ssPen.dwCurrentState;
										}
										if (b)
										{
											m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "service stopped\n");
											m_ServiceStoppedByRp=true;
										}
										else
											m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "error while stopping\n");
									}
								}
								else
								{
									m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "(takecontrol off) service is running\n");
									return false;
								}
							}
							else
								m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "service is not running\n");
						}
						CloseServiceHandle(hService);
					}
					CloseServiceHandle(hSCM);
				}
			}
			CloseHandle(hToken);
		}
		else
			m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "Init", ReTracerCommands(Update), "CRPProtocolsHandler::HandleMessengerService", "OpenProcessToken ko\n");
	}

	return true;
}

void CRPProtocolsHandler::MessaggiFiltra()
{
	for (int k=Messaggi.GetSize()-1; k>=0; k--)
	{
		if ( (Messaggi.GetAt(k).Cmd == "PONG") )
			Messaggi.RemoveAt(k);
	}
}

void CRPProtocolsHandler::Pending()
{	// update status for pending (to be sent) messages

	CString stMsg;
	double destVersion;

	// handles NameResolutionPing fired at rp launch and at PONG timeouts
	if (m_NameResolutionIntervalCounter>0)
		m_NameResolutionIntervalCounter-=1;
	else
	{
		if (m_NameResolutionRequested)
		{
			m_NameResolutionIntervalCounter=m_rpApp->opz_NameResolutionMinInterval;
			m_NameResolutionRequested=false;
			RealPopupProtocol.NameResolutionPing();
		}
	}

	for (int k=0; k<penMessaggi.GetSize(); k++)
	{
		if (penMessaggi[k].Counter!=-1)
		{
			if (RealPopupProtocol.Pong(&penMessaggi[k]))
			{	// destination sent a PONG for this message

				m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), "Pending", ReTracerCommands(Delete), "CRPProtocolsHandler::Pending", "pending timeout for %s [Ok]\n", penMessaggi[k].Msg.Dest);

				CString sdestVersion;
				sdestVersion=penMessaggi[k].Msg.Version;

				//CPenMessaggio testItem=penMessaggi[k];	// item watch debug

				if (sdestVersion.GetLength()>0)
				{
					destVersion=GetRPVersion(&sdestVersion);
					if (destVersion<1.03)
						sdestVersion="V1.0x";
				}
				else
				{
					destVersion=1.0;
					sdestVersion="V1.0x";
				}

				if (penMessaggi[k].PongCounter==1)
				{
					// CMessaggio testItem=penMessaggi[k].Msg; // item watch debug

					// first PONG from destination: ready to send msg body.
					// reset timeout counter, as we know destination is alive;

					// counters for all other mgs are doubled, as we know system
					// was busy for this msg [dirty here, may be improved]
					penMessaggi[k].PongCounter=0;
					for (int w=0; w<penMessaggi.GetSize(); w++)
					{
						if (penMessaggi[w].Msg.Dest==penMessaggi[k].Msg.Dest)
							penMessaggi[w].Counter=((penMessaggi[w].Msg.Msg.GetLength()/MSGREALPOPUP_MULTIPART_MSGSIZE)+1)*m_rpApp->opz_PingTimeOut*2;
					}

					if (penMessaggi[k].PongOnly)
					{	// the PONG is a response to our networkbrowser PING

						m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPProtocolsHandler::Pending", "received PONG for NetworkBroser from %s\n",penMessaggi[k].Msg.Dest);

						if (IsWindow(m_NetworkBrowser->m_hWnd))
							m_NetworkBrowser->Pong(&penMessaggi[k].PongMitt,&penMessaggi[k].PongMsg,&penMessaggi[k].PongVersion);

						if (penMessaggi[k].Msg.Broadcast.GetLength()==0)
							penMessaggi.RemoveAt(k--);
						else
							penMessaggi[k--].PongCounter=1;
					}
					else
					{
						if (RealPopupProtocol.NameResolutionIsNameOnly(&penMessaggi[k].UserDest))
						{	// handle for 'send always pc name on from field' option
							if (!m_rpApp->opz_chkSendPcName)
							{
								penMessaggi[k].Msg.Cmd="REALMITT"+penMessaggi[k].Msg.Mitt;
								penMessaggi[k].Msg.Mitt=m_rpApp->m_LoggedUser;
							}
						}

						// PONG is ok, so we send message body
						penMessaggi[k].Msg.DestVersion=penMessaggi[k].PongVersion;
						RealPopupProtocol.Send(&penMessaggi[k].Msg,&penMessaggi[k].Msg.MsgThreadID);
	     				LogMsgWrite(&penMessaggi[k].Msg,false);
					}

					if (destVersion<1.03)
					{
						/*  rpv1.x not used anymore [changed behaviour]
						if ( (penMessaggi[k].Msg.Cmd!="ONLINEWARN") && (penMessaggi[k].Msg.Cmd.Left(8)!="REDIRECT") )
						{
							stMsg.Format(MSG_STATUS_RPSENT_OLDVER,penMessaggi[k].Msg.Dest,sdestVersion,m_rpApp->env_Version);
							m_wndStatusBar->SetWindowText(stMsg);
						}
						// int g=penMessaggi[k].Counter; // debug
						*/

						if (penMessaggi[k].Msg.Broadcast.GetLength()==0)
							penMessaggi.RemoveAt(k--);
					}
				}
				else
				{
					// the PONG is then 2nd one from destination: message correctly delivered
					if ( (penMessaggi[k].Msg.Cmd!="ONLINEWARN") && (penMessaggi[k].Msg.Cmd.Left(8)!="REDIRECT")  && (penMessaggi[k].Msg.Msg!=MSG_RPNETWORKBROWSERPING))
					{
						if (destVersion<m_rpApp->env_nVersion)
							stMsg.Format(MSG_STATUS_RPSENT_OLDVER,penMessaggi[k].Msg.Dest,sdestVersion,m_rpApp->env_Version);
						else
							stMsg.Format(MSG_STATUS_RPSENT,penMessaggi[k].Msg.Dest);

						m_wndStatusBar->SetWindowText(stMsg);
						m_rpApp->opz_MsgSentNum++;
					}

					if (penMessaggi[k].Msg.Broadcast.GetLength()==0)
						penMessaggi.RemoveAt(k--);
				}
			}
			else
			{	// msg hasn't received a PONG yet

				// if it's not currently in sending process, decrease timeout counter
				if (StrIndex(&RealPopupProtocol.SendExecuteDestinations,&penMessaggi[k].Msg.Dest)==-1)
					penMessaggi[k].Counter-=1;

				if ((penMessaggi[k].Counter%10)==0)
					m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), "Pending", ReTracerCommands(Update), "CRPProtocolsHandler::Pending", "pending timeout for %s [%i]\n", penMessaggi[k].Msg.Dest,penMessaggi[k].Counter);

				if (penMessaggi[k].Counter==0)
				{	// handles pending timeout
					m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Debug), ReTracerControlTypes(List), "Pending", ReTracerCommands(Delete), "CRPProtocolsHandler::Pending", "pending timeout for %s [-]\n", penMessaggi[k].Msg.Dest);

					if ( (!penMessaggi[k].PongOnly) && (penMessaggi[k].Msg.Broadcast.GetLength()==0) )
					{
						bool  ok=false;
						SUser *profile;
						penMessaggi[k].Counter=-1;

						profile=m_Users->GetProfile(&penMessaggi[k].Msg.Dest, &penMessaggi[k].Msg.Msg);
						if (profile!=NULL)
						{
							if (profile->chkUsesRealPopup==1 && profile->cboUsesRealPopup==0)
							{	// Handle for "Uses RealPopup for sure"
								ok=true;

								if (penMessaggi[k].Msg.Cmd!="ONLINEWARN")
								{
									CString ReplyTo;
									ReplyTo.LoadString(MSG_ERR_REPLY);

									CMessaggio errMsg;
									errMsg.Dest=m_rpApp->env_AutoMsgTo;
									errMsg.Mitt=penMessaggi[k].Msg.Dest;
									errMsg.Msg.Format(MSG_ERRNOTRECEIVED, m_rpApp->env_Version, penMessaggi[k].Msg.Dest);
									errMsg.Msg += ReplyTo;
									errMsg.Msg += penMessaggi[k].Msg.Msg;
									errMsg.Cmd="NOTRECEIVED";

									m_rpApp->oWorkstationsRank.RankRemove(errMsg.Mitt);
									Messaggi.Add(errMsg);
									stMsg.Format(MSG_STATUS_NOTRECEIVED,penMessaggi[k].Msg.Dest);
									m_wndStatusBar->SetWindowText(stMsg);

									// maybe the msg was sent to an unknown username:
									// request a NameResolutionPing to update name tables
									m_NameResolutionRequested=true;
									m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), NULL, ReTracerCommands(Update), "CRPProtocolsHandler::Pending", "NameResolutionPing requested\n");
								}
								else
								{
									stMsg.Format(MSG_STATUS_ONLINEWARNNOTRECEIVED,penMessaggi[k].Msg.Dest);
									m_wndStatusBar->SetWindowText(stMsg);
								}
							}

							if (profile->chkUsesRealPopup==0 && !(((CRealPopupApp *) AfxGetApp())->opz_chkLenChk) && penMessaggi[k].Msg.Msg.GetLength()>PROTOCOL_MSGPOPUP_MAXLEN)
							{	// send through winpopup protocol: check for msg length
								ok=true;

								CString ReplyTo;
								ReplyTo.LoadString(MSG_ERR_REPLY);

								CMessaggio errMsg;
								errMsg.Dest=m_rpApp->env_AutoMsgTo;
								errMsg.Mitt=penMessaggi[k].Msg.Dest;
								errMsg.Msg.Format(MSG_ERRTOOLONG, m_rpApp->env_Version, penMessaggi[k].Msg.Dest);
								errMsg.Msg += ReplyTo;
								errMsg.Msg += penMessaggi[k].Msg.Msg;
								errMsg.Cmd="NOTRECEIVED";

								Messaggi.Add(errMsg);
								stMsg.Format(MSG_STATUS_TOOLONG,penMessaggi[k].Msg.Dest);
								m_wndStatusBar->SetWindowText(stMsg);
							}
						}

						// if it's not an internal message, attempt to send through winpopup protocol
						if (!ok && penMessaggi[k].Msg.Cmd.GetLength()==0 && !IsBackdoor(&penMessaggi[k].Msg.Msg))
						{
							WinPopupProtocol.Send(&penMessaggi[k].Msg);
			     			LogMsgWrite(&penMessaggi[k].Msg,false);
							stMsg.Format(MSG_STATUS_WPSENT,penMessaggi[k].Msg.Dest);
							m_wndStatusBar->SetWindowText(stMsg);
							m_rpApp->opz_MsgSentNum++;
						}
					}
					penMessaggi.RemoveAt(k--);
				}
			}
		}
	}
}

void CRPProtocolsHandler::Redirect(int startPos)
{
	SUser *profile;
	char dest[MAX_SEND_LEN];
	CString msgInterno;
	msgInterno.LoadString(AUTOMSG_BETA_FROM);

	for (int k=startPos; k<Messaggi.GetSize(); k++)
	{
		// any internal message is ignored
		if ( (Messaggi[k].Mitt!=msgInterno) && 
			 ( (Messaggi[k].Cmd.GetLength()==0) || (Messaggi[k].Cmd.Left(8)=="REDIRECT")
											    || (Messaggi[k].Cmd.Left(8)=="REALMITT")) )
		{
			profile=m_Users->GetProfile(&Messaggi[k].Mitt, &Messaggi[k].Msg);
			if (profile!=NULL)
			{
				if (profile->chkRedirect==1)
				{
					int loops=0; CString cloops; char mycount;
					cloops=Messaggi[k].Cmd.Left(8);
					if (cloops.Compare("REDIRECT")==0)
					{
						cloops=Messaggi[k].Cmd.Right(1);
						mycount=cloops[0];
						loops=atoi(&mycount);
					}

					if (loops<4)
					{
						itoa(++loops,&mycount,10);

						CString stMsg;
						stMsg.Format(MSG_REDIRECTING, Messaggi[k].Mitt, profile->Redirect);
						m_wndStatusBar->SetWindowText(stMsg);

						CMessaggio redMsg;
						redMsg.Cmd = "REDIRECT@" + ((CRealPopupApp *) AfxGetApp())->m_Workstation + "@" + mycount;
						redMsg.Tutti = profile->Redirect + m_rpApp->TogliUserName(&Messaggi[k].Tutti);
						redMsg.Mitt = Messaggi[k].Mitt;
						redMsg.Msg = Messaggi[k].Msg;
						redMsg.Broadcast = Messaggi[k].Broadcast;
					
						strcpy(dest,LPCTSTR(profile->Redirect));

						// Prende ogni singolo user specificato
						for (int q=0; AfxExtractSubString(redMsg.Dest, dest, q, (char) ','); q++)
							Send(&redMsg);
					}
					else
					{	// I received feedbacks telling about students who were used to
						// setup an 'rp ring' with endless message redirecting :-)
						// rp now allows no more than 5 redirects for same message

						CString stMsg;
						stMsg.Format(MSG_REDIRECTING_ERR, Messaggi[k].Mitt, profile->Redirect);
						m_wndStatusBar->SetWindowText(stMsg);
					}
				}
			}
		}
	}
}

void CRPProtocolsHandler::LogMsgIn(int startPos)
{
	for (int k=startPos; k<Messaggi.GetSize(); k++)
		LogMsgWrite(&Messaggi[k],true);
}

void CRPProtocolsHandler::LogMsgWrite(CMessaggio *msg, bool primaMitt)
{

	SUser *profile;
	CString msgInterno, riga, ver;
	msgInterno.LoadString(AUTOMSG_BETA_FROM);
	CStdioFile myLog;

	// any internal message is ignored
	if ( (msg->Mitt!=msgInterno) && 
		 ( (msg->Cmd.GetLength()==0) || (msg->Cmd.Left(8)=="REDIRECT") || (msg->Cmd.Left(8)=="REALMITT") ))
	{
		if (primaMitt)
			profile=m_Users->GetProfile(&msg->Mitt, &msg->Msg);
		else
		{
			// look for myself: it's possible to define myself
			// so to log my own messages.
			profile=m_Users->GetProfile(&msg->Dest, &msg->Msg);
			if (profile->name.Compare(m_rpApp->env_DefAllUsers)==0)
				profile=m_Users->GetProfile(&msg->Mitt, &msg->Msg);
		}

		if (profile!=NULL)
		{
			if (profile->chkLog==1 && !profile->chkIgnore)
			{
				if (!myLog.Open(profile->Log, CFile::modeNoTruncate |
					CFile::modeWrite | CFile::typeText ) )
				{
					if (!myLog.Open(profile->Log, CFile::modeCreate | CFile::modeWrite | CFile::typeText ) )
					{
						return;
						/*	CString myErr;				// debug
						myErr.Format("ERR OPENING");
						m_wndStatusBar->SetWindowText(myErr);*/
					}
					else
					{
						riga.Format(MSG_LOGHEAD,m_rpApp->env_Version);
						myLog.WriteString(riga);
					}
				}

				CString fmtData, fmtDaA;
				fmtData.LoadString(FMT_LOG_DATE);
				fmtDaA.LoadString(FMT_LOG_FROMTO);

				CTime t = CTime::GetCurrentTime();
				ver = t.Format(fmtData);

				riga.Format(fmtDaA, msg->Mitt, msg->Dest, ver);
				myLog.SeekToEnd();
				myLog.WriteString (riga);

				char *mymsg=(char *) calloc(msg->Msg.GetLength() + 5,1);
				char *mymsgstart=mymsg;
				for (int k=0; k<msg->Msg.GetLength(); k++)
				{
					if (msg->Msg.GetAt(k)!='\r')
						*mymsg++=msg->Msg.GetAt(k);
				}
				*mymsg=0;
				myLog.WriteString (mymsgstart);
				myLog.WriteString ("\n----\n");

				free(mymsgstart);
			}
		}
	}

}

void CRPProtocolsHandler::Ping(CMessaggio * Msg, CString *UserDest, bool pingCallback, CString *CurrentMsgsThreadID)
{
	
	if (!m_rpApp->opz_chkFullInternalPings && pingCallback)
	{
		m_rpApp->oReTracer.Trace(ReTracerTraceLevels(Information), ReTracerControlTypes(Text), "NetworkBrowser", ReTracerCommands(Update), "CRPProtocolsHandler::Ping", "no PING for NetworkBrowser (registry set FullInternalPings=0)\n");
		return;
	}
			
	CPenMessaggio newPenMsg;
	Msg->Copy(&newPenMsg.Msg);
	
	if (UserDest!=NULL)
		newPenMsg.UserDest=*UserDest;
	else
		newPenMsg.UserDest=Msg->Dest;

	newPenMsg.Counter=((Msg->Msg.GetLength()/MSGREALPOPUP_MULTIPART_MSGSIZE)+1)*m_rpApp->opz_PingTimeOut;
	newPenMsg.PongCounter=1;
	newPenMsg.PongOnly = pingCallback;
	if (CurrentMsgsThreadID!=NULL)
		newPenMsg.Msg.MsgThreadID = *CurrentMsgsThreadID;
	penMessaggi.Add(newPenMsg);
	RealPopupProtocol.Ping(Msg);
}

bool CRPProtocolsHandler::GetNotificationEnabled()
{
	return isNotificationEnabled;
}

bool CRPProtocolsHandler::GetAwayEnabled()
{
	return isAwayEnabled;
}

void CRPProtocolsHandler::SetNotificationEnabled(bool newValue)
{
	isNotificationEnabled=newValue;
	WinPopupProtocol.IsNotificationEnabled=newValue;
	RealPopupProtocol.IsNotificationEnabled=newValue;
}

void CRPProtocolsHandler::SetAwayEnabled(bool newValue)
{
	isAwayEnabled=newValue;
	WinPopupProtocol.IsAwayEnabled=newValue;
	RealPopupProtocol.IsAwayEnabled=newValue;
}

void CRPProtocolsHandler::PendingPurge()
{
	int k=0;

	while (k<penMessaggi.GetSize())
	{
		if (penMessaggi[k].PongOnly)
			penMessaggi.RemoveAt(k);
		else
			k++;
	}
}

void CRPProtocolsHandler::ShowInternalMessage(CString *msg, CString *head, DWORD icon, CString *balloonMsg)
{
	if (m_TrayIcon->IsWin2K())
	{
		if (balloonMsg==NULL)
			m_TrayIcon->ShowBalloon(*msg, (head==NULL?"RealPopup":*head), icon, 10);
		else
			m_TrayIcon->ShowBalloon(*balloonMsg, (head==NULL?"RealPopup":*head), icon, 10);
	}

	if (!m_TrayIcon->IsWin2K() || balloonMsg!=NULL)
	{
		CMessaggio warnMsg;
		warnMsg.Msg.Format(MSG_INTERNALMSGHEAD, m_rpApp->env_Version);
		warnMsg.Msg.Append(*msg);
		warnMsg.Mitt=m_rpApp->env_AutoMsgFrom;
		warnMsg.Dest=m_rpApp->env_AutoMsgTo;
		warnMsg.RiempiCampiData();
		Messaggi.Add(warnMsg);
	}
}
