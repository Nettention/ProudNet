#pragma once

#include "DumpReporter.h"

class CDumpReporter
{
public:
	CDumpReporter(void);
	~CDumpReporter(void);

	CStringW m_dumpFileName;
	CStringW m_serverAddr;
	WORD m_serverPort;

	bool DoReportByExecParam(LPCWSTR cmdLine, LPCWSTR host, WORD port);

private:
	void GetDumpFilePath(LPWSTR output);
};
