#pragma once

/**
\~korean
캐주얼 게임 유저(게이머)에 대한 여러가지 유틸리티 클래스입니다.

\~english
Several utility classes about casual game user (gamer)

\~chinese
休闲游戏用户(玩家)的各种工具类。

\~japanese
カジュアルゲームのユーザー（ゲーマー）に対する様々なユーティリティのクラスです。

\~
*/
class CCasualGamer
{
public:
	CCasualGamer(void);
	~CCasualGamer(void);

	/**
	\~korean
	CLoadedGamerDataPtr에서, 게이머가 가장 최근에 사용한 플레이어 캐릭터 정보를 얻습니다.

	\~english
	Gets recent used player character information by gamer at CLoadedGamerDataPtr.

	\~chinese
	CLoadedGamerDataPtr中获得玩家最近使用的玩家人物信息。

	\~japanese
	CLoadedGamerDataPtrからゲーマーが一番最近使ったプレイヤーキャラクターの情報を取得します。

	\~
	*/
	static Guid GetSelectedHeroGuid(CLoadedData2Ptr gamerData);

	/**
	\~korean
	CLoadedGamerDataPtr에서, 게이머가 가장 최근에 사용한 플레이어 캐릭터 정보를 얻습니다.

	\~english
	Gets recent used player character information by gamer at CLoadedGamerDataPtr. 

	\~chinese
	CLoadedGamerDataPtr中获得玩家最近使用的玩家人物信息。

	\~japanese
	CLoadedGamerDataPtrからゲーマーが一番最近使ったプレイヤーキャラクターの情報を取得します。

	\~
	*/
	static String GetSelectedHeroName(CLoadedData2Ptr gamerData);

	/**
	\~korean
	CLoadedGamerDataPtr에서, 게이머가 가장 최근에 사용한 플레이어 캐릭터 정보를 얻습니다.

	\~english
	Gets recent used player character information by gamer at CLoadedGamerDataPtr.

	\~chinese
	CLoadedGamerDataPtr中获得玩家最近使用的玩家人物信息。

	\~japanese
	CLoadedGamerDataPtrからゲーマーが一番最近使ったプレイヤーキャラクターの情報を取得します。

	\~
	*/
	static CPropNodePtr GetSelectedHero(CLoadedData2Ptr gamerData);
};
