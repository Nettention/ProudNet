#include "StdAfx.h"
#include "Clientbase.h"
#include "world.h"
#include "battleclient.h"
#include "mainwindow.h"
#include "BattleWaitingForm.h"
#include "Bullet_C.h"
#include "resource.h"
#include "MainWindow.h"
#include "../GCCommon/Battle_common.cpp"
#include "../GCCommon/Battle_proxy.cpp"
#include "../GCCommon/Battle_stub.cpp"

CBattleClient::CBattleClient()
{
	m_localHero = NULL;

	m_NetClientName = L"BattleClient";
	// Creates client object
	// 클라 객체 생성
	m_NetClient.Attach(CNetClient::Create());
	m_NetClient->SetEventSink(this);

	// RMI 초기화
	// Initialize RMI
	m_NetClient->AttachStub((BattleS2C::Stub*)this);
	m_NetClient->AttachStub((BattleC2C::Stub*)this);
	m_NetClient->AttachProxy(&m_c2sProxy);
	m_NetClient->AttachProxy(&m_c2cProxy);

	m_startPlayModeUnderProgress = false;
}

CBattleClient::~CBattleClient(void)
{
	ClearWorld();

	// Close client network module. It does disconnect with server.
	// 클라이언트 네트워크 모듈을 종료합니다. 즉 서버와의 연결을 끊는 역할도 합니다.
	m_NetClient.Free();

	// 이 객체를 참조하는 다른 폼 데이터도 청소합니다.
	// Clean up other form data which refer to this object.
	CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();
}

/// Handler that notice result about success or fail of connecting after connecting attempt with server
/// 서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 Handler
void CBattleClient::OnJoinServerComplete( ErrorInfo *info, const ByteArray &replyFromServer )
{
	if (info->m_errorType != ErrorType_Ok)
	{
		// When it failed to connect with server, shows error then go to close mode.
		// 서버와의 연결이 실패했으면 에러를 표시한 후 종료 모드로 돌입.
		CStringW txt;
		txt.Format(L"%s\n%s", CWorld::GetSharedPtr()->LoadString(IDS_STRING109), info->ToString().GetString());

		CWorld::GetSharedPtr()->GotoErrorForm(txt);
	}
	else
	{
		ATLTRACE("%s\n",__FUNCTION__); 
		// Starts logon process. Of course, game room entering attemption is doing at the same time.
		// 로그온 절차를 시작합니다. 물론, 게임방 진입 시도도 동시에 진행합니다.
		RmiContext rmi = RmiContext::ReliableSend;
		rmi.m_encryptMode = EM_Secure;
		m_c2sProxy.RequestNextLogon(HostID_Server, rmi,
									CWorld::GetSharedPtr()->m_roomToJoin.m_guid, CWorld::GetSharedPtr()->m_credential);

		// Initialize old data and state at game waiting room form
		// 게임방 대기실 폼의 구 데이터와 상태를 초기화합니다.
		CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_LOG)->RemoveAllItems();
		CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();
	}
}

/// When it closed connection with server
/// 서버와의 접속이 종료된 경우
void CBattleClient::OnLeaveServer( ErrorInfo *errorInfo )
{
	// Close program
	// 프로그램 종료
	PostQuitMessage(0);
}

/// Try to connect with server.
/// 서버와의 연결을 시도합니다.
bool CBattleClient::Connect()
{
	m_p2pGroupID = HostID_None;

	CNetConnectionParam param;

	param.m_protocolVersion = CSettings::GetSharedPtr()->GetBattleVersion() ;
	param.m_serverIP = CWorld::GetSharedPtr()->m_roomToJoin.m_serverAddr.m_addr;
	param.m_serverPort = CWorld::GetSharedPtr()->m_roomToJoin.m_serverAddr.m_port;

	return m_NetClient->Connect(param);
}

/// Processing each frame
/// 매 프레임마다의 처리
void CBattleClient::FrameMove( float fElapsedTime )
{
	// Call CNetClient.FrameMove
	// CNetClient.FrameMove를 호출.
	m_NetClient->FrameMove();

	for each(CGameObject_C* obj in m_gameObjects)
		{
		obj->FrameMove(fElapsedTime);
	}
}

/// Express character information which indicate on the screen with string.
/// 화면에 표시할 캐릭터 정보를 문자열로 표현합니다.
CStringW CBattleClient::GetHeroSlotText(const CHeroPublishInfo& heroSlot)
{
	CStringW ret;
	if (m_roomInfo.m_masterHeroGuid == heroSlot.m_guid)
		ret.Format(L"%s [%I64d] - Master", heroSlot.m_name, heroSlot.m_score);
	else
		ret.Format(L"%s [%I64d] - %s", heroSlot.m_name, heroSlot.m_score, GamingStateToString(heroSlot.m_gamingState));

	return ret;
}

/// Gets Hero guid, then find hero information (hero slot) which is synchronizing from server.
/// Hero guid를 입력받아, 서버로부터 동기화되고 있는 hero 정보( hero slot)을 찾습니다.
CBattleHeroSlotPtr CBattleClient::GetHeroSlotByGuid( Guid heroGuid )
{
	CBattleHeroSlotPtr output;
	if (m_heroSlots.TryGetValue(heroGuid, output))
		return output;
	else
		return CBattleHeroSlotPtr();
}

/// Find out hero slot but use HostID of client who control that hero.
/// hero slot을 찾되 입력값으로 그 hero를 조종하는 클라이언트의 HostID를 씁니다.
CBattleHeroSlotPtr CBattleClient::GetHeroSlotByHostID( HostID clientID )
{
	for (BattleHeroSlots ::iterator i = m_heroSlots.begin();i != m_heroSlots.end();i++)
	{
		CBattleHeroSlotPtr hero = i->second;
		if (hero->m_HostID == clientID)
			return hero;
	}
	return CBattleHeroSlotPtr();
}

/// Handler when P2P group is created or added new member at group
/// P2P 그룹이 생성되거나 그룹에 새 멤버가 추가되면 오는 Handler
void CBattleClient::OnP2PMemberJoin( HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField )
{
	// Save P2P group ID.
	// P2P 그룹 ID를 저장합니다.
	m_p2pGroupID = groupHostID;
}

/// Is player of this client room master?
/// 이 클라의 플레이어가 방장인가?
bool CBattleClient::IsLocalHeroRoomMaster()
{
	CBattleHeroSlotPtr localheroSlot = GetLocalHeroSlot();
	if (localheroSlot && localheroSlot->m_info.m_guid == m_roomInfo.m_masterHeroGuid)
		return true;
	else
		return false;
}

/// Find out hero slot that opposite player of this client.
/// 이 클라의 플레이어에 대응하는 hero slot을 찾습니다.
CBattleHeroSlotPtr CBattleClient::GetLocalHeroSlot()
{
	for (BattleHeroSlots ::iterator i = m_heroSlots.begin();i != m_heroSlots.end();i++)
	{
		CBattleHeroSlotPtr slot = i->second;
		if (slot->m_HostID == m_NetClient->GetLocalHostID())
			return slot;
	}

	return CBattleHeroSlotPtr();
}

/// Check all gamer click "READY" in game room. Room master consider to clicked.
/// 게임방 내 모든 게이머가 READY를 찍었나 체크합니다. 방장은 무조건 찍은걸로 간주합니다.
bool CBattleClient::IsEveryHeroPlayReady()
{
	if (m_heroSlots.Count == 0)
		return false;

	for (BattleHeroSlots ::iterator i = m_heroSlots.begin();i != m_heroSlots.end();i++)
	{
		CBattleHeroSlotPtr slot = i->second;
		if (slot->m_info.m_guid != m_roomInfo.m_masterHeroGuid && slot->m_info.m_gamingState != GamingState_Ready)
			return false;
	}

	return true;
}

DEFRMI_BattleS2C_NotifyNextLogonFailed(CBattleClient)
{
	ATLTRACE("::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT , 0, 0);");
	// Return to lobby.
	// 로비로 돌아갑니다.
	::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT , 0, 0);

	return true;
}

DEFRMI_BattleS2C_NotifyNextLogonSuccess(CBattleClient)
{
	// Save received Guid.
	// 받은 Guid를 저장해둡니다.
	CWorld::GetSharedPtr()->m_gamerguid = Gamerguid;

	// If there are any possibility of changing selected character with cracking then it goes through error. Therefore use official method which is renewal it.
	// 만약 해킹등으로 인하여 중간에 선택된 케릭터가 바뛰는 경우가 존재할 수 있습니다면 바로 오류로 이어지기 때문에 정보를 다시 갱신해 주는 정식 방법을 사용합니다.
	CWorld::GetSharedPtr()->m_localHeroInfo = selectedHeroInfo;

	// Move to waiting room form because it succeed logon.
	// 로그온이 성공했으므로 게임방 대기실 폼으로 이동합니다.
	CWorld::GetSharedPtr()->GotoBattleWaitingForm();
	return true;

}

DEFRMI_BattleS2C_ShowError(CBattleClient)
{
	CWorld::GetSharedPtr()->GotoErrorForm(errTxt.GetString());
	return true;
}

DEFRMI_BattleS2C_NotifyUnauthedAccess(CBattleClient)
{
	// "L"Error: Trying to play without logon!"
	// "L"Error: 로그온하지 않고 게이밍을 시도했습니다!"
	CStringW logString;
	logString.LoadStringW(IDS_STRING102);
	CWorld::GetSharedPtr()->GotoErrorForm(logString);

	return true;
}
DEFRMI_BattleS2C_HeroSlot_Appear(CBattleClient)
{
	// Add to waiting list.
	// 대기자 목록에 추가합니다.
	CBattleHeroSlotPtr hero(new CBattleHeroSlot);
	hero->m_info = heroInfo;
	hero->m_HostID = clientHostID;

	CStringW heroTxt = GetHeroSlotText(heroInfo);

	m_heroSlots.Add(heroInfo.m_guid, hero);
	CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_HERO_LISTBOX)->AddItem(heroTxt, hero.get()); 

	BattleWaitingForm_RefreshButtons();

	return true;

}

DEFRMI_BattleS2C_HeroSlot_Disappear(CBattleClient)
{
	// Remove from waiting list.
	// 대기자 목록에서 제거합니다.
	CBattleHeroSlotPtr hero = GetHeroSlotByGuid(heroGuid);
	if (!hero)
		return true;

	CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_HERO_LISTBOX);
	for (int i = 0;i < lbox->GetSize();i++)
	{
		if (lbox->GetItem(i)->pData == hero.get())
		{
			lbox->RemoveItem(i);
			break;
		}
	}

	m_heroSlots.Remove(heroGuid);

	BattleWaitingForm_RefreshButtons();

	return true;
}

DEFRMI_BattleS2C_HeroSlot_ShowState(CBattleClient)
{
	// Renew waiting list.
	// 대기자 목록을 갱신합니다.
	CBattleHeroSlotPtr hero = GetHeroSlotByGuid(heroInfo.m_guid);
	if (!hero)
		return true;

	hero->m_info = heroInfo;

	CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_HERO_LISTBOX);
	for (int i = 0;i < lbox->GetSize();i++)
	{
		if (lbox->GetItem(i)->pData == hero.get())
		{
			wcscpy(lbox->GetItem(i)->strText, GetHeroSlotText(heroInfo));
			break;
		}
	}

	BattleWaitingForm_RefreshButtons();

	return true;
}
DEFRMI_BattleC2C_P2P_Chat(CBattleClient)
{
	// Spread chatting string.
	// 채팅 문자열을 뿌립니다.
	CDXUTListBox* ctl = CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_LOG);

	CBattleHeroSlotPtr heroSlot = GetHeroSlotByHostID(remote);
	if (heroSlot)
	{
		CStringW txt;
		txt.Format(L"[%s] %s", heroSlot->m_info.m_name, chatText);
		ctl->AddItem(txt, NULL);
		ctl->SelectItem(ctl->GetSize() - 1);
	}

	return true;
}

DEFRMI_BattleS2C_NotifyGameRoomInfo(CBattleClient)
{
	CStringW loadString;
	loadString.LoadString(IDS_STRING132);
	m_roomInfo = info;
	CStringW title;
	title.Format(L"%s - %s", loadString, info.m_name);
	CWorld::GetSharedPtr()->m_battleWaitingForm.GetStatic(IDC_TITLE)->SetText(title);

	// Renew gamer list becasue room master may be changed.
	// 게이머 목록도 갱신합니다. 방장이 바뀌는 경우가 있으므로.
	CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_battleWaitingForm.GetListBox(IDC_HERO_LISTBOX);
	for (int i = 0;i < lbox->GetSize();i++)
	{
		CBattleHeroSlot* hero = (CBattleHeroSlot*)lbox->GetItem(i)->pData;
		wcscpy(lbox->GetItem(i)->strText, GetHeroSlotText(hero->m_info));
	}

	// Changes state of button.
	// 버튼 상태도 바꿉니다.
	BattleWaitingForm_RefreshButtons();

	return true;
}

DEFRMI_BattleS2C_NotifyStartPlayModeFailed(CBattleClient)
{
	m_startPlayModeUnderProgress = false;
	BattleWaitingForm_RefreshButtons();

	return true;
}

DEFRMI_BattleS2C_GotoLoadingMode(CBattleClient)
{
	// TODO: Start game data file loading from here.
	// This sample does not have loading process so notice finished loading to server instantly at this function.
	// TODO: 여기에서 게임 데이터 파일 로딩을 시작해야 합니다.
	// 본 샘플에서는 딱히 로딩하는 과정이 없으므로 이 함수에서 즉시 로딩 완료를 서버에 notify하도록 합니다.
	m_c2sProxy.NotifyLoadFinished(HostID_Server, RmiContext::ReliableSend);

	return true;
}

DEFRMI_BattleS2C_GotoPlayMode(CBattleClient)
{
	// 로컬 플레이어 캐릭터 생성
	if(m_localHero == NULL)
	{
		CGameObject_C *localHero(new CGameObject_C);
		m_localHero = localHero;
		m_localHero->m_owner = this;
		m_gameObjects.Add(localHero);	
	}
	m_localHero->m_type = GameObjectType_Hero;
	m_localHero->m_ownerHostID = m_NetClient->GetLocalHostID();
	m_localHero->m_name = CWorld::GetSharedPtr()->m_localHeroInfo.m_name;

	//Notice game start to server.(hero & bullet)
	//게임시작을 서버에 알립니다.
	m_c2sProxy.RequestLocalHeroSpawn(HostID_Server,RmiContext::ReliableSend,m_localHero->m_name,m_localHero->m_position,m_localHero->m_yaw);	

	// Start game play mode.
	// 게임 플레이 모드를 시작합니다.
	CWorld::GetSharedPtr()->GotoBattleForm();

	return true;
}

DEFRMI_BattleS2C_GotoWaitingMode(CBattleClient)
{
	// 게임 플레이 모드를 시작합니다.
	ClearWorld();

	CWorld::GetSharedPtr()->GotoBattleWaitingForm();

	return true;
}
DEFRMI_BattleS2C_NotyfyGotoLobby(CBattleClient)
{
	ATLTRACE("::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT , 0, 0);");
	// Return to lobby.
	// 로비로 돌아갑니다.
	::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT , 0, 0);

	return true;
}


/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CBattleClient::OnError(ErrorInfo *errorInfo) 
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

void CBattleClient::OnException(const Proud::Exception &e) 
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

DEFRMI_BattleC2C_P2P_LocalHero_Move(CBattleClient)
{
	CGameObject_C* remoteHero = GetRemoteHeroByClientID(remote);
	if (remoteHero)
	{
		// Renew position of character.
		// 캐릭터의 위치를 갱신합니다.
		remoteHero->m_positionFollower.SetTargetPosition(position);
		remoteHero->m_positionFollower.SetTargetVelocity(velocity);
		remoteHero->m_yawFollower.SetTargetAngle(yaw);

		// Marking that received chracter position with direct P2P.
		// 캐릭의 위치를 direct P2P로 받았는지 여부를 마킹합니다.
		bool oldValue = remoteHero->m_synchedThruDirectP2P;
		remoteHero->m_synchedThruDirectP2P = rmiContext.m_relayed ? false : true;
		if (oldValue != remoteHero->m_synchedThruDirectP2P)
		{
			remoteHero->m_synchedThruDirectP2PChangeShowCoolTimeMs = CGameObject_C::SynchedThruDirectP2PChangeShowInterval;
		}
	}

	return true;
}

DEFRMI_BattleS2C_NotifyLocalHeroViewersGroupID(CBattleClient)
{
	if(m_localHero != NULL)
	{
		m_localHero->m_viewersGroupID = localHeroViewersGroupID;
	}
	else
	{
		ASSERT(0);
	}
	return true;
}

// Find out chracter object of other client with HostID of other client.
// 타 클라의 HostID를 갖고 타 클라의 캐릭터 객체를 찾습니다.
CGameObject_C *CBattleClient::GetRemoteHeroByClientID( HostID remoteClientID )
{
	CGameObject_C *ret;
	if (m_remoteHeroes.TryGetValue(remoteClientID, ret))
		return ret;
	else
		return NULL;
}

DEFRMI_BattleS2C_RemoteHero_Appear(CBattleClient)
{
	// Ignore that received about character itself.
	// 자기 캐릭터에 대해서 날아온 것은 무시합니다.
	if (remote != m_NetClient->GetLocalHostID())
	{
		// Find or create instance about other chracter.
		// 타 캐릭터에 대한 인스턴스를 찾거나 만듭니다.
		CGameObject_C *remoteHero = GetRemoteHeroByClientID(remote);
		if (remoteHero == NULL)
		{
			CGameObject_C *newRemoteHero = new CGameObject_C;
			newRemoteHero->m_owner = this;
			newRemoteHero->m_type = GameObjectType_Hero;
			newRemoteHero->m_ownerHostID = heroOwnerID;
			newRemoteHero->m_name = heroName;

			m_remoteHeroes.Add(heroOwnerID, newRemoteHero);
			m_gameObjects.Add(newRemoteHero);

			remoteHero = newRemoteHero;
		}

		// Sets defualt chracter value.
		// 초기 캐릭터 값을 세팅합니다.
		remoteHero->m_positionFollower.SetTargetPosition(position);
		remoteHero->m_positionFollower.SetTargetVelocity(velocity);
		remoteHero->m_yawFollower.SetTargetAngle(yaw);

		// Set this because it just appeared.
		// 이것도 설정해야 합니다. 왜냐하면 바로 갓 등장한 것이기 때문입니다.
		remoteHero->m_positionFollower.EnableAutoFollowDuration(true);

		remoteHero->m_positionFollower.SetFollowerPosition(position);
		remoteHero->m_positionFollower.SetFollowerVelocity(velocity);

		// Sets value about angle.
		// 각도에 대한 값도 세팅합니다.
		remoteHero->m_yawFollower.SetFollowerAngleVelocity(D3DX_PI / 180 * 120);
		remoteHero->m_yawFollower.SetFollowerAngle(yaw);
		remoteHero->m_yawFollower.SetTargetAngle(yaw);
	}

	return true;
}

DEFRMI_BattleS2C_RemoteHero_Disappear(CBattleClient)
{
	// Remove opposited chracter of other client.
	// 대응하는 타 클라의 캐릭터를 제거합니다.
	CGameObject_C* remoteHero;
	if(m_remoteHeroes.TryGetValue(heroOwnerID, remoteHero))
	{
		remoteHero->Dispose();
		m_remoteHeroes.RemoveKey(heroOwnerID);
		m_gameObjects.Remove(remoteHero);
		delete remoteHero;
	}

	return true;
}

DEFRMI_BattleS2C_NotifyHeroScore(CBattleClient)
{
	if (m_localHero != NULL)
	{
		m_localHero->m_score = score;
	}
	return true;
}

// 어깨너머 뷰를 설정합니다.
void CBattleClient::SetCamera()
{
	if(m_localHero != NULL)
{
		// 카메라는 플레이어 살짝 뒤에서, 플레이어가 바라보는 방향 끝을 가리켜야.
		D3DXMATRIX cameraRotTM;
		D3DXMATRIX tranTM;

		// 캐릭터 발 중심이 아닌, 캐릭터 총 대강 위치 중심
		D3DXVECTOR3 playerGunCenterPos = m_localHero->m_position + D3DXVECTOR3(0, 0.5, 0);
	
		D3DXMATRIX yawMat;
		D3DXMatrixRotationY(&yawMat, m_localHero->m_yaw);
	
		D3DXMATRIX pitchMat;
		D3DXMatrixRotationZ(&pitchMat, m_localHero->m_pitch);
	
		D3DXVECTOR3 offset(-2, 0.5f, -0.5f); // 카메라와 플레이어간 거리. 기어오브워처럼 약간 옆으로 서 있게.
		D3DXVec3TransformCoord(&offset, &offset, &pitchMat);
		D3DXVec3TransformCoord(&offset, &offset, &yawMat);

		D3DXVECTOR3 eyePos;
		eyePos = playerGunCenterPos + offset; 
	
		// 플레이어가 바라보는 방향을 기준으로 저 멀리가 look at position
		D3DXVECTOR3 lookatOffset(500, 0, 0); 
		D3DXVec3TransformCoord(&lookatOffset, &lookatOffset, &pitchMat);
		D3DXVec3TransformCoord(&lookatOffset, &lookatOffset, &yawMat);

		D3DXVECTOR3 atPos = playerGunCenterPos + lookatOffset;

		D3DXMatrixLookAtLH(&g_mView, &eyePos, &atPos, &D3DXVECTOR3(0, 1, 0));
	}

}

void CBattleClient::ClearWorld()
{
	for each(CGameObject_C* obj in m_gameObjects)
	{
		obj->Dispose();
		delete obj;
		}
		
	m_gameObjects.Clear();
}

void CBattleClient::Render()
{
	D3DXMATRIX mWorld;

	// 터레인을 그립니다.
	D3DXMatrixIdentity(&mWorld);
	RenderMesh(mWorld, g_terrainMesh);

	for each(CGameObject_C* obj in m_gameObjects)
	{
		obj->Render();
	}
}