#pragma once

#include "../../../../include/ProudNetClient.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace Proud;

#include "Client.h"
#include "ConnectLayer.h"
#include "MainLayer.h"
#include "MainGUILayer.h"
#include "ErrorLayer.h"


// 메인 모듈
// 1개 이상의 클라이언트 객체들을 가진다.
// Main module
// Own more than 1 client object.
class CWorld
{
public:
	enum SceneState
	{
		ConnectScene,		// Server connecting scene
		MainScene,			// Playing a game
		ErrorScene,			// Unplayable form with error
	};

/*	enum SceneState
	{
		ConnectScene,		// 서버에 연결을 묻는 씬
		MainScene,			// 게임 플레이 중
		ErrorScene,			// 에러가 나서 더 이상 진행이 불능한 상태에서 표시되는 씬
	};*/

	// 현재 활성화된 폼
	// Currently activated form
	SceneState m_sceneState;

	// 로컬 클라이언트 목록
	// Local client list
	CFastArray<CClientPtr> m_clients;

	// 서버 연결 폼에서 받은 정보
	// Information that received from server connection form
	CCString m_serverAddr, m_userName, m_errorMsg;

	// MainScene에 필요한 CCLayer
	MainLayer* m_mainLayer;
	MainGUILayer* m_mainGUILayer;

	// 새로 추가되는 클라에게 할당하는 일련번호
	// Serial number that assign to new adding client
	int m_clientIDSerial;

	// 현재 유저가 직접 조종하는 캐릭터의 소유 클라이언트의 인덱스
	// Own client index of controlling character by current user
	int m_clientUnderUserControl;

	int m_desiredClientCount;

	// 기본 글꼴 크기
	// Default Font Size
	static const float DefaultFontSize;
	// 월드의 크기
	// Size of world
	static const float ArenaLength;

	// 추가 클라이언트들을 생성할 때, 한꺼번에 생성하면 과부하가 걸리므로, 천천히 클라이언트를 증가시키며 서버에 접속시키거나
	// 서버로부터 접속 해지를 시키기 위해서 쓰이는 변수
	// This function use for if you create many additional client at the same time, it occurs overload so increase client number slowly then connect or disconnect from server
	float m_adjustDesiredClientCountCoolTime;

	// 사용자가 입력한 방향
	bool m_upMenuItemSelected;
	bool m_leftMenuItemSelected;
	bool m_rightMenuItemSelected;

	// 메인 레이어에서 오브젝트를 가지고 있는 노드.
	CCNode* m_mainLayerObjects;

	CWorld(void);
	~CWorld(void);

private:
	void AdjustDesiredClientCount();
	void AdjustDesiredClientCountOnNeed( float fElapsedTime );
	void DisposeMarkedClient();
	void LocalHeroes_FrameMove(float fElapsedTime);
	void UpdateClientCountCtl();
	void SetClientUnderUserControlVisible(bool visible);
public:
	CClient* GetClientUnderUserControl();
	void SetClientUnderUserControl(int ID);
	CLocalHero* GetLocalHeroUnderUserControl();
	int FindClientByValue(CClient* client);
	void FrameMove(float fElapsedTime);
	void GoToErrorScene(const char* text);
	void GoToMainScene();
	void GoToConnectScene();
	void Log(const char* text);

	CClient* AddNewClient();
	void RemoveLastOneClient();
};

extern CWorld* g_World;
