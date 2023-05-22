#include "StdAfx.h"
#include ".\serverbase.h"
#include "GCServerDlg.h"

CServerBase::CServerBase(void) 
{
	m_disposed = false;
}

CServerBase::~CServerBase(void)
{
}

/// Renew text that indicate subsidiary state of this server.
/// 이 서버의 부수적 상황을 표시하는 문구를 갱신합니다.
void CServerBase::SetStatusText( LPCWSTR text )
{	
	if(!m_ownerDlg->m_hWnd)
		return;
	UiAccessParameter* p = new UiAccessParameter;
	p->m_text = text;
	p->m_serverObj = this;
	m_ownerDlg->PostMessage(WM_SET_STATUS, (WPARAM)p);
}

/// Set indicated name (title) of this server.
/// 이 서버의 표시되는 이름(제목)을 설정합니다.
void CServerBase::SetTitle( LPCWSTR text )
{
	if(!m_ownerDlg->m_hWnd)
		return;
	UiAccessParameter* p = new UiAccessParameter;
	p->m_text = text;
	p->m_serverObj = this;
	m_ownerDlg->PostMessage(WM_SET_TITLE, (WPARAM)p);
}

/// Print log
/// 로그를 출력합니다.
void CServerBase::LogError( LPCWSTR text )
{
	if(!m_ownerDlg->m_hWnd)
	{
		return;
	}

	UiAccessParameter* p = new UiAccessParameter;
	p->m_text = text;
	p->m_serverName = GetDescription();
	m_ownerDlg->PostMessage(WM_LOG_ERROR, (WPARAM)p);
}

void CServerBase::LogEvent( LPCWSTR text )
{
	if(!m_ownerDlg->m_hWnd)
	{
		return;
	}

	UiAccessParameter* p = new UiAccessParameter;
	p->m_text = text;
	p->m_serverName = GetDescription();
	m_ownerDlg->PostMessage(WM_LOG_EVENT, (WPARAM)p);
}


CStringW CServerBase::LoadString( int stringResourceNum )
{
	CStringW loadString;
	if( !loadString.LoadStringW(stringResourceNum) )
	{
		LogError(L"Failed to load string");
	}
	return loadString;
}

