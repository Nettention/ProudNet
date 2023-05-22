#include "StdAfx.h"
#include "CasualGamer.h"

CCasualGamer::CCasualGamer(void)
{
}

CCasualGamer::~CCasualGamer(void)
{
}

/** Gets recent used player character information by gamer at CLoadedGamerDataPtr. */
/** CLoadedGamerDataPtr에서, 게이머가 가장 최근에 사용한 플레이어 캐릭터 정보를 얻습니다. */
Proud::CPropNodePtr CCasualGamer::GetSelectedHero( CLoadedData2Ptr gamerData )
{
	if(gamerData->GetRootNode()->GetChild()->GetCount() == 0)
		return CPropNodePtr();
	Guid heroGuid = gamerData->GetRootNode()->Fields[L"SelectedHeroGuid"];
	return gamerData->GetNode(heroGuid);
}

/** Gets recent used player character information by gamer at CLoadedGamerDataPtr. */
/** CLoadedGamerDataPtr에서, 게이머가 가장 최근에 사용한 플레이어 캐릭터 정보를 얻습니다. */
Proud::Guid CCasualGamer::GetSelectedHeroGuid( CLoadedData2Ptr gamerData )
{
	CPropNodePtr hero = GetSelectedHero(gamerData);
	if (!hero)
	{
		return Guid();
	}
	else
	{
		return hero->GetUUID();
	}
}

/** Gets recent used player character information by gamer at CLoadedGamerDataPtr. */
/** CLoadedGamerDataPtr에서, 게이머가 가장 최근에 사용한 플레이어 캐릭터 정보를 얻습니다. */
String CCasualGamer::GetSelectedHeroName( CLoadedData2Ptr gamerData )
{
	CPropNodePtr hero = GetSelectedHero(gamerData);
	if (!hero)
	{
		return String();
	}
	else
	{
		return (String)hero->Fields[L"Name"];
	}



	
}