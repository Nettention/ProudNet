#pragma once

#include "../../../include/ProudNetServer.h"
#include "../../../include/DumpServer.h"

using namespace Proud;

class CMyDumpServer: public IDumpServerDelegate
{
	CriticalSection m_cs;
	CDumpServer* m_server;
	int m_serial;
	bool m_mustStopNow;
public:
	CMyDumpServer(void);
	~CMyDumpServer(void);

	virtual void OnStartServer(CStartServerParameter &refParam) override;
	
	virtual bool MustStopNow() override;
	virtual CriticalSection* GetCriticalSection() override;
	virtual void OnServerStartComplete(Proud::ErrorInfo *result) override;
	String GetDumpFilePath(HostID clientEnid, const Proud::AddrPort& clientAddr, const ::timespec& tp) override;

	static UINT ThreadProc(void* ctx);
	void Run();
};
