#include "stdafx.h"
#include <direct.h>

#include "MsgFile.h"


//////////////////////////////////////////////////////////////////////
// MsgFile Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFile::CMsgFile()
{

};

CMsgFile::~CMsgFile()
{

};

bool CMsgFile::Invia(SMessaggio *Msg)
{
	SMessaggio msgSingolo;

	long msgLen=Msg->Msg.GetLength();
	long bytesSent=0;
	char myNParts[5];
	char myPart[5];
	int NParts=0,Part=0;

	msgSingolo.Mitt=mUser;
	if (Msg->Cmd.GetLength()==0)
		msgSingolo.Cmd=" ";
	else
		msgSingolo.Cmd=Msg->Cmd;

	msgSingolo.Dest=Msg->Dest;
	NParts=(msgLen/MSGFILE_MULTIPART_MSGSIZE)+1;

	do
	{

		msgSingolo.NParts= _itoa(NParts,myNParts,10);
		msgSingolo.Part= _itoa(++Part,myPart,10);

		msgSingolo.Msg=Msg->Msg.Mid(bytesSent,MSGFILE_MULTIPART_MSGSIZE);

		InviaMsg(&msgSingolo);

		bytesSent+=MSGFILE_MULTIPART_MSGSIZE;

	} while (bytesSent<msgLen);

	return true;
}

bool CMsgFile::InviaMsg(SMessaggio *Msg)
{

	char	tmpNomeFile[128];
	CString backdoorCmd;

	backdoorCmd=Msg->Msg.Left(5);
	if (backdoorCmd=="@FROM")
	{
		backdoorCmd=Msg->Msg.Mid(5,20);
		int k = backdoorCmd.Find('@');
		if (k>0)
		{
			Msg->Mitt=backdoorCmd.Left(k);
			backdoorCmd=Msg->Msg.Mid(k+6);
			Msg->Msg=backdoorCmd;
		}
	}		
	
	GetTempFileName(mPathSend, "RP", 0, tmpNomeFile);
	CFile m_file(tmpNomeFile,CFile::modeCreate | CFile::modeWrite );

	m_file.Write(Msg->NParts, Msg->NParts.GetLength());
	m_file.Write("\0", 1);
	m_file.Write(Msg->Part, Msg->Part.GetLength());
	m_file.Write("\0", 1);
	m_file.Write(Msg->Cmd, Msg->Cmd.GetLength());
	m_file.Write("\0", 1);
	m_file.Write(Msg->Mitt, Msg->Mitt.GetLength());
	m_file.Write("\0", 1);
	m_file.Write(Msg->Dest, Msg->Dest.GetLength());
	m_file.Write("\0", 1);
	m_file.Write(Msg->Msg, Msg->Msg.GetLength());
	m_file.Write("\0", 1);

	m_file.Close();

	return true;
}

int CMsgFile::Scan()
{

	CFile	   UNFmessaggio;
    CFileFind  finder;
	UINT	   bytesletti;
	char	   buffer[MSG_BUFFER_LEN];
	CString	   sbuffer;
	int		   counter=0;
	SMessaggio NuovoMessaggio;

	CString myPath=mPathReceive+"\\rp*.tmp";

    BOOL bWorking = finder.FindFile(myPath);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		if (UNFmessaggio.Open(finder.GetFilePath(), CFile::modeRead ))
		{
			bytesletti = UNFmessaggio.Read(buffer, MSG_BUFFER_LEN);
			UNFmessaggio.Close();

			--bytesletti;
			while ((--bytesletti)>0)
			{
				if (buffer[bytesletti]==0)
				{
					buffer[bytesletti]=(char) MSG_BUFFER_ZERO_REPL;
				}
			}

			sbuffer = buffer;

			UNFtoMsg(sbuffer, &NuovoMessaggio);

			if (NuovoMessaggio.Cmd=="RECERR")
			{
				CString myMsg;
				myMsg.LoadString(MSG_SEND_MULTIPARTERROR);
				NuovoMessaggio.Msg.Format(myMsg,NuovoMessaggio.Mitt);
			}

			RiceviMsg(&NuovoMessaggio);
			
			counter+=1;

			CFile::Remove(finder.GetFilePath());
		}
    }

	return counter;
}

void CMsgFile::UNFtoMsg(CString UNFmsg, SMessaggio *NuovoMessaggio)
{
    AfxExtractSubString(NuovoMessaggio->NParts, UNFmsg, 0, (TCHAR) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Part, UNFmsg, 1, (TCHAR) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Cmd, UNFmsg, 2, (TCHAR) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Mitt, UNFmsg, 3, (TCHAR) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Dest, UNFmsg, 4, (TCHAR) MSG_BUFFER_ZERO_REPL);
    AfxExtractSubString(NuovoMessaggio->Msg, UNFmsg, 5, (TCHAR) MSG_BUFFER_ZERO_REPL);
	/*
	NuovoMessaggio->Msg+="@";
	NuovoMessaggio->Msg+=NuovoMessaggio->NParts;
	NuovoMessaggio->Msg+="@";
	NuovoMessaggio->Msg+=NuovoMessaggio->Part;
	*/
	CTime t = CTime::GetCurrentTime();
	NuovoMessaggio->Data = t.Format("%H:%M.%S");

}

bool CMsgFile::RiceviMsg(SMessaggio *Msg)
{

	int part, nparts;
	nparts=		atoi(Msg->NParts);
	part=		atoi(Msg->Part);

	if (nparts > 1)
	{
		for (int k=0;k<mMsgMultipart.GetSize();k++)
		{
			if (mMsgMultipart[k].Mitt==Msg->Mitt)
			{
				int partMulti, npartsMulti;
				npartsMulti=atoi(LPCTSTR(mMsgMultipart[k].NParts));
				partMulti=	atoi(LPCTSTR(mMsgMultipart[k].Part));

				if ( (npartsMulti!=nparts) || (partMulti!= (part-1) ) )
				{
					mMsgMultipart[k].Cmd="RECERR";
					if (nparts==part)
					{
						CString myMsg, myFrom;
						myMsg.LoadString(MSG_RECEIVE_MULTIPARTERROR);
						myFrom.LoadString(AUTOMSG_BETA_FROM);
						mMsgMultipart[k].Msg.Format(myMsg,mMsgMultipart[k].Mitt);
						mMsgMultipart[k].Mitt=myFrom;
						mMessaggi->Add(mMsgMultipart[k]);

						mMsgMultipart[k].Msg=" ";
						mMsgMultipart[k].Mitt=mUser;
						mMsgMultipart[k].Dest=Msg->Mitt;
						Invia(&mMsgMultipart[k]);
						
						mMsgMultipart.RemoveAt(k);
						return true;
					}
				}
				else
				{
					mMsgMultipart[k].Msg+=Msg->Msg;

					if (++partMulti == npartsMulti)
					{
						mMessaggi->Add(mMsgMultipart[k]);
						mMsgMultipart.RemoveAt(k);
						return true;
					}
					else
					{
						char myPart[5];

						mMsgMultipart[k].Part = _itoa(partMulti,myPart,10);
					}
				}

				break;
			}
		}

		if (k==mMsgMultipart.GetSize())
		{
			mMsgMultipart.Add(*Msg);
		}
	}
	else
		mMessaggi->Add(*Msg);

	return true;
}