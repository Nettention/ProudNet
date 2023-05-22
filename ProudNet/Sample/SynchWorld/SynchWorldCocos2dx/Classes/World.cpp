#include "stdafx.h"

#include "World.h"
#include "LocalHero.h"


CWorld* g_World = NULL;

// 기본 글꼴 크기
// Default Font Size
const float CWorld::DefaultFontSize = 20.0;

// 월드의 가로세로 길이
// Length and breadth of world
const float CWorld::ArenaLength = 500;

const float AdjustDesiredClientCountInterval = 0.5f;

CWorld::CWorld(void)
{
	m_clientIDSerial = 0;
	m_sceneState = ConnectScene;
	m_clientUnderUserControl = -1;
	m_desiredClientCount = 1;
	m_adjustDesiredClientCountCoolTime = AdjustDesiredClientCountInterval;

	// 서버 연결 폼에서 받은 정보
	// Information that received from server connection form
	m_serverAddr = "localhost";
	m_userName = "hahaha";

	// 사용자가 입력한 방향
	m_upMenuItemSelected = false;
	m_leftMenuItemSelected = false;
	m_rightMenuItemSelected = false;

	m_mainLayerObjects = CCNode::create();
	m_mainLayerObjects->retain();
}


CWorld::~CWorld(void)
{
	m_mainLayerObjects->release();
	m_mainLayerObjects = NULL;
}

// 사용자가 입력한 '원하는 클라이언트 갯수'에 따라 클라이언트 인스턴스를 증가 또는 증감을 한다.
// Do increase or decrease client instance depend on 'Wanted client number' that entered by user.
void CWorld::AdjustDesiredClientCount()
{
	// 최소한 1개의 클라는 건재해야 하므로
	// At least 1 client lives
	CC_ASSERT(m_desiredClientCount >= 1);

	// 아직 서버와 프라이머리 클라와의 연결 시도를 시작이라도 한 상태이어야 한다.
	// It has to started connecting attempt with server and primary client.
	if (m_clients.GetCount() > 0) 
	{
		if (m_desiredClientCount > m_clients.GetCount())
		{
			// 클라를 1개 더 추가한다.
			// Adds 1 more client.
			CClient* newCli = AddNewClient();
			newCli->Connect();
		}
		else if (m_desiredClientCount < m_clients.GetCount())
		{
			// 클라를 1개 제거함을 표기해둔다.
			// Mark remove 1 client.
			RemoveLastOneClient();
		}
	}
}

void CWorld::AdjustDesiredClientCountOnNeed(float fElapsedTime)
{
	// 희망하는 클라 갯수에 따라 점층적으로 클라를 새로 만들거나 이미 있는 클라를 폐기해 나간다.
	// Create new client or dispose existed client depends on number of client that you wish.
	m_adjustDesiredClientCountCoolTime -= fElapsedTime;
	if (m_adjustDesiredClientCountCoolTime < 0)
	{
		m_adjustDesiredClientCountCoolTime = AdjustDesiredClientCountInterval;

		AdjustDesiredClientCount();
	}
}

// 폐기하라는 표식이 된 클라들을 제거.
// Remove makred client as "dispose".
void CWorld::DisposeMarkedClient()
{
	for (int i = m_clients.GetCount() - 1;i >= 0;i--)
	{
		CClient* client = m_clients[i];
		if (client->m_markDisposed)
		{
			// 보조 클라이언트 %s가 제거됐습니다.
			// Sub client %s was removed
			CCString logString;
			logString.initWithFormat("Sub client %s has removed", client->m_localHero ? client->m_localHero->m_name.getCString() : "<None>");
			Log(logString.getCString());

			// 폼에서도 제거한다.
			// Remove from form.
			if (m_sceneState == MainScene)
			{
				CCArray* listArray = m_mainGUILayer->m_clientControlTableViewDataSoruce.m_clientControlList;
				CCString clientID;
				clientID.initWithFormat("%d", client->m_ID);

				for (int j = 0; j < listArray->count(); j++)
				{
					CCString* listArrayClientID = (CCString*)listArray->objectAtIndex(j);

					if (clientID.compare(listArrayClientID->getCString()) == 0)
					{
						if (m_clientUnderUserControl == j)
						{
							// 다른 것을 선택하게 한다.
							// Make select another one.
							m_clientUnderUserControl = j-1;
						}

						listArray->removeObjectAtIndex(j);
						m_mainGUILayer->m_clientControlTableView->reloadData();

						UpdateClientCountCtl();
						break;
					}
				}
			}

			// 실제 클라 제거
			// Remove actual client
			m_clients.RemoveAt(i);
		}
	}
}

// 클라이언트 갯수 관련 UI를 업데이트한다.
// Update client number related UI.
void CWorld::UpdateClientCountCtl()
{
	if (m_sceneState == MainScene)
	{
		CCString txt;
		txt.initWithFormat("Client Count: %d", m_mainGUILayer->m_clientControlTableViewDataSoruce.m_clientControlList->count());
	
		m_mainGUILayer->m_clientCountLabel->setString(txt.getCString());
	}
}

void CWorld::SetClientUnderUserControlVisible(bool visible)
{
	CClient* client = GetClientUnderUserControl();

	if (client)
	{
		if (client->m_localHero)
		{
			client->m_localHero->m_localHeroSprite->setVisible(visible);
			client->m_localHero->m_localHeroLabel->setVisible(visible);
		}

		for (Proud::Position iRH = client->m_remoteHeroes.GetStartPosition(); iRH;)
		{
			CRemoteHeroPtr_C remoteHero = client->m_remoteHeroes.GetNextValue(iRH);

			remoteHero->m_remoteHeroSprite->setVisible(visible);
			remoteHero->m_remoteHeroLabel->setVisible(visible);
		}
	}
}

// 현재 유저가 컨트롤하겠다고 선택한 캐릭터를 조종한다.
// Control selected character by current user.
void CWorld::LocalHeroes_FrameMove(float fElapsedTime)
{
	for (int i = 0; i < m_clients.GetCount(); i++)
	{
		CClient* client = m_clients[i];
		if (client->m_localHero)
		{
			if (client->m_localHero == GetLocalHeroUnderUserControl())
			{
				// 키 입력에 따른 이동 처리를 한다.
				// Movement process by key entering.
				if (m_upMenuItemSelected)
				{
					client->m_localHero->Accel(fElapsedTime);
				}
				else
				{
					client->m_localHero->Brake(fElapsedTime);
				}
				if (m_rightMenuItemSelected)
				{
					client->m_localHero->TurnRight(fElapsedTime);
				}
				if (m_leftMenuItemSelected)
				{
					client->m_localHero->TurnLeft(fElapsedTime);
				}
			}
			else
			{
				// 계속 우회전 전진을 하고 있는 것처럼 처리한다.
				// Process like keep turn right then move straight.
				client->m_localHero->Accel(fElapsedTime);
				client->m_localHero->TurnLeft(fElapsedTime);
			}
			client->m_localHero->FrameMove(fElapsedTime);
		}
	}
}

// 유저가 직접 키보드로 제어하고 있는 캐릭터의 클라이언트를 얻는다.
// Get client of character that controlling by user with keyboard.
CClient* CWorld::GetClientUnderUserControl()
{
	if (m_clientUnderUserControl < 0)
		return NULL;
	return m_clients[m_clientUnderUserControl];
}

// 현재 유저가 선택해서 조종하고 있는 캐릭터 객체를 얻는다.
// Get character object that selected by current user and under control.
CLocalHero* CWorld::GetLocalHeroUnderUserControl()
{
	if (m_clientUnderUserControl < 0)
		return NULL;
	return m_clients[m_clientUnderUserControl]->m_localHero;
}

// 현재 유저가 선택한 캐릭터의 객체 인덱스를 지정한다.
void CWorld::SetClientUnderUserControl(int ID)
{
	if (ID < 0)
		return ;
	
	// 현재 보여지는 객체들은 다시 보이지않도록 한다.
	SetClientUnderUserControlVisible(false);

	m_clientUnderUserControl = ID;

	if (m_sceneState == MainScene)
	{
		// 유저가 선택한 객체를 따라다니기위해 CCFollow객체를 생성한다.
		CCFollow* followTarget = CCFollow::create(GetLocalHeroUnderUserControl()->m_localHeroSprite,
			CCRectMake(0, 0, CWorld::ArenaLength, CWorld::ArenaLength));
		// 생성한 객체를 실행한다.
		m_mainLayer->runAction(followTarget);
	}
}

int CWorld::FindClientByValue(CClient* client)
{
	for (int i = 0;i < m_clients.GetCount();i++)
	{
		if (m_clients[i] == client)
			return i;
	}
	return -1;
}

// 1회 프레임 처리
// 1 frame process
void CWorld::FrameMove(float fElapsedTime)
{
	// 각 클라에 대한 진행 처리를 한다.
	// Do process about each client.
	LocalHeroes_FrameMove(fElapsedTime);

	for (int i = 0;i < m_clients.GetCount();i++)
	{
		CClient* client = m_clients[i];
		client->FrameMove(fElapsedTime);
	}

	DisposeMarkedClient();

	AdjustDesiredClientCountOnNeed(fElapsedTime);

	SetClientUnderUserControlVisible(true);
}

// 에러가 나서 더 이상 플레이 불능 상태의 모드로 전환한다.
// Turn to unplayable mode because of error.
void CWorld::GoToErrorScene(const char* text)
{
	if (m_sceneState != ErrorScene)
	{
		m_sceneState = ErrorScene;

		m_errorMsg = text;

		// 'scene' is an autorelease object
		cocos2d::CCScene* scene = cocos2d::CCScene::create();

		// 'layer' is an autorelease object
		ErrorLayer* errorLayer = ErrorLayer::create();

		// add layer as a child to scene
		scene->addChild(errorLayer);

		cocos2d::CCDirector::sharedDirector()->replaceScene(scene);
	}
}

// 메인 모드로 전환한다.
// Turn to main scene mode.
void CWorld::GoToMainScene()
{
	if (m_sceneState != MainScene)
	{
		m_sceneState = MainScene;

		// 'scene' is an autorelease object
		cocos2d::CCScene* scene = cocos2d::CCScene::create();

		// 'layer' is an autorelease object
		m_mainLayer = MainLayer::create();
		m_mainGUILayer = MainGUILayer::create();

		// add layer as a child to scene
		scene->addChild(m_mainLayer);
		scene->addChild(m_mainGUILayer);

		cocos2d::CCDirector::sharedDirector()->replaceScene(scene);

		// 현재 조정하는 Client를 설정.
		SetClientUnderUserControl(0);
	}
}

void CWorld::GoToConnectScene()
{
	m_sceneState = ConnectScene;

	// 'scene' is an autorelease object
	cocos2d::CCScene* scene = cocos2d::CCScene::create();

	// add layer as a child to scene
	scene->addChild(ConnectLayer::create());

	cocos2d::CCDirector::sharedDirector()->replaceScene(scene);

	// 현재 조정하는 Client를 설정.
	SetClientUnderUserControl(0);
}

// 메인 폼에서, 여러가지 로그를 출력한다.
// Print several log at main form.
void CWorld::Log(const char* text)
{
	if (m_sceneState == MainScene)
	{
		m_mainGUILayer->m_eventLogTableViewDataSource.m_eventLogList->addObject(CCString::create(text));
		m_mainGUILayer->m_eventLogTableView->reloadData();
	}
}

// 새 클라이언트 객체를 만들어 등록시킨다.
// Creates new client object then apply it.
CClient* CWorld::AddNewClient()
{
	m_clientIDSerial++;

	CClientPtr newCli(new CClient(m_clientIDSerial) );
	m_clients.Add(newCli);
	CCString txt;
	txt.initWithFormat("%d", newCli->m_ID);

	if (m_sceneState == MainScene)
	{
		m_mainGUILayer->m_clientControlTableViewDataSoruce.m_clientControlList->addObject(CCString::create(txt.getCString()));
		m_mainGUILayer->m_clientControlTableView->reloadData();
	}
	UpdateClientCountCtl();

	m_clientUnderUserControl = kmMin(m_clients.GetCount() - 1, m_clientUnderUserControl);
	m_clientUnderUserControl = kmMax(0, m_clientUnderUserControl);

	return newCli;
}

void CWorld::RemoveLastOneClient()
{
	m_clients[ m_clients.GetCount()-1]->m_markDisposed = true;
}
