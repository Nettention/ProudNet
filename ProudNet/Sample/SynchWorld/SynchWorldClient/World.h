#pragma once

#include "DXUTForm.h"

#include "LocalHero.h"
#include "Client.h"

// 메인 모듈
// 1개 이상의 클라이언트 객체들을 가진다.
// Main module
// Own more than 1 client object.
class CWorld
{
public:
	enum FormState
	{
		ConnectForm,		// Server connecting form
		MainForm,			// Playing a game
		ErrorForm,			// Unplayable form with error
	};

/*	enum FormState
	{
		ConnectForm,		// 서버에 연결을 묻는 폼
		MainForm,			// 게임 플레이 중
		ErrorForm,			// 에러가 나서 더 이상 진행이 불능한 상태에서 표시되는 폼
	};*/

	// 현재 활성화된 폼
	// Currently activated form
	FormState m_formState;

	// 로컬 클라이언트 목록
	// Local client list
	Proud::CFastArray<CClientPtr> m_clients;

	// 서버 연결 폼에서 받은 정보
	// Information that received from server connection form
	CStringW m_serverAddr, m_userName;

	// 서버 연결을 묻는 폼
	// Server connecting form
	CDXUTDialog m_connectForm, m_errorForm, m_mainForm;

	// 새로 추가되는 클라에게 할당하는 일련번호
	// Serial number that assign to new adding client
	int m_clientIDSerial;

	// 현재 유저가 직접 조종하는 캐릭터의 소유 클라이언트의 인덱스
	// Own client index of controlling character by current user
	int m_clientUnderUserControl;

	int m_desiredClientCount;

	// 뷰포트 카메라
	// View port camera
	CPositionFollower		m_cameraPosition;

	// 월드의 크기
	// Size of world
	static const float ArenaLength;

	// 추가 클라이언트들을 생성할 때, 한꺼번에 생성하면 과부하가 걸리므로, 천천히 클라이언트를 증가시키며 서버에 접속시키거나
	// 서버로부터 접속 해지를 시키기 위해서 쓰이는 변수
	// This function use for if you create many additional client at the same time, it occurs overload so increase client number slowly then connect or disconnect from server
	float m_adjustDesiredClientCountCoolTime;

	CWorld(void);
	~CWorld(void);

private:
	void AdjustDesiredClientCount();
	void AdjustDesiredClientCountOnNeed( float fElapsedTime );
	void DisposeMarkedClient();
	void LocalHeroes_FrameMove(float fElapsedTime);
	void MoveCamera( float fElapsedTime );
	void UpdateClientCountCtl();
public:
	CClient* GetClientUnderUserControl();
	CLocalHero* GetLocalHeroUnderUserControl();
	bool FormMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	int FindClientByValue(CClient* client);
	void FrameMove(float fElapsedTime);
	void GoToErrorForm(LPCWSTR text);
	void GoToMainForm();
	void Log(LPCWSTR text);
	void Render();
	void RenderForm(float elapsedTime);

	CClient* AddNewClient();
	void RemoveLastOneClient();

};

extern CWorld *g_World;