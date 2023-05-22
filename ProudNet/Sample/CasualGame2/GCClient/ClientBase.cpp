#include "StdAfx.h"
#include "ClientBase.h"
#include "World.h"

CClientBase::CClientBase(void)
{
}

CClientBase::~CClientBase(void)
{
}


/************************************* Print Error *************************************/
/************************************* 오류 출력 *************************************/
void CClientBase::GotoErrorForm( LPCWSTR text )
{
	CStringW str;
	str += m_NetClientName;
	str += text;

	CWorld::GetSharedPtr()->m_errorForm.GetStatic(IDC_STATIC)->SetText(str);
	CWorld::GetSharedPtr()->m_FormState = CWorld::ErrorForm;
}

ErrorInfoPtr CClientBase::GetTestErrorInfoPtr()
{
	ErrorInfoPtr Info = ErrorInfoPtr(new ErrorInfo);
	Info->m_errorType = ErrorType_ProtocolVersionMismatch;
	Info->m_comment = L"This is Error test.";
	return Info;
}
