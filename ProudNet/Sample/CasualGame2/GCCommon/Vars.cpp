#include "StdAfx.h"
#include ".\vars.h"

CSettings::CSettings()
{
	static PNGUID ver = { 0x683addc6, 0x6740, 0x485b, { 0xaf, 0x8d, 0x88, 0x18, 0xc1, 0xcc, 0x4, 0x3e } };
	m_EntryVersion = Guid(ver);
	m_EntryServerPort = 33555;

}

Proud::Guid CFarmSettings::GetFarmVersion()
{
	return m_FarmVersion;
}

int CFarmSettings::GetFarmServerPort()
{
	return m_FarmServerPort;
}

CFarmSettings::CFarmSettings()
{
	static PNGUID ver = { 0x683addc6, 0x6722, 0x4a5b, { 0xbf, 0x8d, 0x88, 0x18, 0xaa, 0xdd, 0x4, 0x3e } };
	m_FarmVersion = Guid(ver);
	m_FarmServerPort = 33000;
	m_FarmName = L"ProudNet Sample Farm";
	m_dbAuthKey = L"ProudNet Sample Farm Key should be something!";
	
	m_dbmsConnectionString = L"Data Source=.;Database=ProudDB2-Test;Trusted_Connection=yes"; // .을 써야지, localhost를 쓰지 말 것. 일부 컴에서 연결 실패 오류로 이어짐.
	m_logdbConnectionString = L"Data Source=.;Database=Log-Test;Trusted_Connection=yes"; // .을 써야지, localhost를 쓰지 말 것. 일부 컴에서 연결 실패 오류로 이어짐.

}

int CFarmSettings::GetDbServerPort()
{
	return m_FarmServerPort + 1;
}
Proud::Guid CSettings::GetEntryVersion()
{
	Guid ret = m_EntryVersion;
	ret.Data1++;
	return ret;
}

int CSettings::GetEntryServerPort()
{
	return m_EntryServerPort + 1;
}

Proud::Guid CSettings::GetLobbyVersion()
{
	Guid ret = m_EntryVersion;
	ret.Data1 += 2;
	return ret;
}

int CSettings::GetLobbyServerPort()
{
	return m_EntryServerPort + 2;
}

Proud::Guid CSettings::GetBattleVersion()
{
	Guid ret = m_EntryVersion;
	ret.Data1 += 3;
	return ret;
}

int CSettings::GetBattleServerPort()
{
	return m_EntryServerPort + 3;
}

CHeroPublishInfo::CHeroPublishInfo()
{
	m_type = 0;
	m_score = 10;
	m_gamingState = GamingState_None;
}

LPCWSTR GamingStateToString(GamingState s)
{
	switch (s)
	{
	case GamingState_None:
		return L"<NONE>";
		break;
	case GamingState_NotReady:
		return L"Waiting";
	case GamingState_Ready:
		return L"Ready";
	case GamingState_Loading:
		return L"Loading";
	case GamingState_PlayWaiting:
		return L"PlayWaiting";
	case GamingState_Playing:
		return L"Playing";
	}
	return L"";
}

CBullet::CBullet(void)
{
	m_speed = 0;
	m_velocity = D3DXVECTOR3(0,0,0);

	m_BroadCastBulletStateInterval = 0.2f;

	m_broadcastStateCoolTime = (float)(Proud::CRandom::StaticGetFloat() * m_BroadCastBulletStateInterval);

	// Set first position...
	//초기위치 정하기...
	float ran_val = (float)Proud::CRandom::StaticGetFloat();

	if(ran_val< 0.4f)//s,n,t
		m_position.x = (float)Proud::CRandom::StaticGetFloat()*299;
	else if(ran_val>= 0.4f && ran_val < 0.8f)
		m_position.x = -20;
	else
		m_position.x = 320;
	
	ran_val = (float)Proud::CRandom::StaticGetFloat();
	if (ran_val < 0.5f) 
		m_position.y = 0; 
	else 
		m_position.y = 5;
	
	if(m_position.x != -20 && m_position.x != 320)//n,s,t
	{
		ran_val = (float)Proud::CRandom::StaticGetFloat();
		if(ran_val < 0.4f)
		{
			m_position.z = -20;
			m_direction = D3DX_PI / 180 * 270;
			m_directionState = SOUTH;
		}
		else if(ran_val >= 0.4f && ran_val < 0.8f)
		{
			m_position.z = 320;
			m_direction = D3DX_PI /180 * 90;
			m_directionState = NORTH;
		}
		else
		{
			m_position.y = 100;
			m_position.z = (float)Proud::CRandom::StaticGetFloat()*300;
			m_directionState = TOP;
			m_direction = 3.0f;
		}
	}
	else if(m_position.x == -20)
	{
		m_position.z = (float)Proud::CRandom::StaticGetFloat()*300;
		m_direction = 0;
		m_directionState = WEST;
	}
	else 
	{
		m_position.z = (float)Proud::CRandom::StaticGetFloat()*300;
		m_direction = D3DX_PI;
		m_directionState = EAST;
	}

	m_speed = 0;
	m_maxSpeed = (float)Proud::CRandom::StaticGetInt();
	m_maxSpeed = max(50,m_maxSpeed);
	m_maxSpeed = min(m_maxSpeed,80);
	m_accelspeed = (float)((Proud::CRandom::StaticGetFloat() + 0.1)*70);
}

CBullet::~CBullet(void)
{
}

CItem::CItem(void)
{
	m_position.x = (float)Proud::CRandom::StaticGetFloat()*300;
	m_position.y = 0;
	m_position.z = (float)Proud::CRandom::StaticGetFloat()*300;

	m_position.x = max(0,m_position.x);
	m_position.x = min(m_position.x,300);
	
	m_position.z = max(0,m_position.z);
	m_position.z = min(m_position.z,300);
	
	float ran_val = (float)Proud::CRandom::StaticGetFloat();
	
	if (ran_val<0.2f)
		m_type = LIFEINCREASE;
	else if (ran_val >= 0.2f && ran_val < 0.5f)
		m_type = LIFEDECREASE;
	else if (ran_val >= 0.5f && ran_val < 0.7f)
		m_type = DISAPPEARBULLET;
	else
		m_type = RAPID;
}
CItem::~CItem(void)
{

}

CStringW NoRoomNameText = L"<Enter room title.>";
// For synchronization character position, limited visible area to user (circle area)
// 캐릭 위치 동기화를 위해, 유저에게 제한된 가시 거리(원의 반경)
const float SynchAreaRange = 10;

// Minimum distance between bullet and object. If it is smaller than this area or same then crash it.
//총알과 물체사이의 최소거리. 이범위보다 작거나 같아지면 충돌
const int distanceHeroFromToBullet = 5;
const int distanceHeroFromToItem = 10;
