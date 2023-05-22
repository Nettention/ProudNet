#pragma once

#include "../../../include/FakeClrBase.h"


/**
\~korean
FirstLogon 혹은 NextLogon에서 어느 클라이언트인지 구분하기위한 CustomField

\~english
CustomField that classify client at FirstLogon or NextLogon

\~chinese
在FirstLogon或NextLogon中区分客户端的CustomField

\~japanese
FirstLogonやNextLogonでどのクライアントであるかを区別するためのCustomField
\~
*/
#define ReturnToEntry 0x11      
#define NewEntryClient 0x22
#define NewLobbyClient 0x33
#define NewBattleClient 0x44

enum ServerMode;
class CGCServerDlg;

/**
\~korean
각 서버 인스턴스의 공통 베이스 클래스입니다.

\~english
Common base class of each server instance

\~chinese
每个服务器实例的共用基类。

\~japanese
各サーバインスタンスの共通ベースのクラスです。
\~
*/
class CServerBase
{
public:

	/**
	\~korean
	이 서버 객체를 관장하는 어플리케이션 폼

	\~english
	Application form that manage this server object

	\~chinese
	管理该服务器对象的应用表单

	\~japanese
	このサーバのオブジェクトを管理するアプリケーションフォーム

	\~
	*/
	CGCServerDlg* m_ownerDlg;

	/**
	\~korean
	이 서버가 곧 파괴되어야 하는지 나타냅니다.

	\~english
	Is this server destroyed soon?

	\~chinese
	该服务器是否要立即毁掉？

	\~japanese
	このサーバはすぐに破壊されるべきであるか。

	\~
	*/
	volatile bool m_disposed;

	ServerMode m_serverMode;

	CServerBase(void);
	virtual ~CServerBase(void);

	virtual void Start() = 0;
	virtual String GetDescription() = 0;

	virtual void FrameMove() {}


	/**
	\~korean
	이 서버의 부수적 상황을 표시하는 문구를 갱신합니다.

	\~english
	Renew text that indicate subsidiary state of this server.

	\~chinese
	更新显示该服务器辅助状态的文字。

	\~japanese
	このサーバの付随的な状況を表示するフレーズを更新します。

	\~
	*/
	void SetStatusText( LPCWSTR text );

	/**
	\~korean
	이 서버의 표시되는 이름(제목)을 설정합니다.

	\~english
	Set indicated name (title) of this server.

	\~chinese
	设置该服务器的显示名称(标题)。

	\~japanese
	このサーバに表示される名前（タイトル）を設定します。

	\~
	*/
	void SetTitle( LPCWSTR text );

	/**
	\~korean
	로그를 출력합니다.

	\~english
	Print log

	\~chinese
	打印日志。

	\~japanese
	ログを出力します。
	\~
	*/
	void LogError(LPCWSTR text);
	void LogEvent(LPCWSTR text);
	CStringW LoadString( int stringResourceNum );
};

typedef RefCount<CServerBase> CServerBasePtr;