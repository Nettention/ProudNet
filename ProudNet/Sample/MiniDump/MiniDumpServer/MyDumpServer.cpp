#include "StdAfx.h"
#include ".\mydumpserver.h"
#include <conio.h>

CMyDumpServer::CMyDumpServer(void)
{
	m_server = CDumpServer::Create(this);
	m_mustStopNow = false;
}

CMyDumpServer::~CMyDumpServer(void)
{
	delete m_server;
}

void CMyDumpServer::Run()
{
	AfxBeginThread(ThreadProc, this);
	while (1)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 27:
				goto done;
			}
		}
		Sleep(1);
	}
done:
	m_mustStopNow = true;
}

UINT CMyDumpServer::ThreadProc( void* ctx )
{
	CMyDumpServer* server = (CMyDumpServer*)ctx;
	server->m_server->RunMainLoop();

	return 0;
}

Proud::String CMyDumpServer::GetDumpFilePath(HostID clientEnid, const Proud::AddrPort& clientAddr, const ::timespec& tp)
{
	String out;

	tm dumpTime;
	localtime_pn(&tp.tv_sec, &dumpTime);

	out.Format(L"Dumped_HostID%d_%02d-%02d-%02d_%02d-%02d-%02d.dmp",
				clientEnid,
				dumpTime.tm_year+1900, dumpTime.tm_mon+1, dumpTime.tm_mday,
				dumpTime.tm_hour+1, dumpTime.tm_min, dumpTime.tm_sec);

	//출력 되는 덤프 파일 명 예시) Dumped_HostID3_2017-03-06_19-36-13.dmp
	return out;
}

void CMyDumpServer::OnServerStartComplete( Proud::ErrorInfo *result )
{
	if(!result)
		printf("Start dump Server. ESC Key: End.\n");
	else
		printf("Start dump server failed! ESC key: End.\n");

}

bool CMyDumpServer::MustStopNow()
{
	return m_mustStopNow;
}

CriticalSection* CMyDumpServer::GetCriticalSection()
{
	return &m_cs;
}

void CMyDumpServer::OnStartServer( CStartServerParameter &refParam ) 
{
	refParam.m_tcpPorts.Add(20004);
}