#pragma once

/**
\~korean
이 샘플 프로그램에서 사용되는 entry client, lobby client, battle client 등의 공통 베이스 클래스입니다.

\~english
Common base class of entry client, lobby client, battle client which are using at this sample program

\~chinese
该样本程序使用的entry client, lobby client, battle client等公用基类。

\~japanese
このサンプルプログラムで使われているentry client、lobby client、battle clientなどの共通ベースのクラスです。

\~
*/
class CClientBase
{
public:

	/**
	\~korean
	ProudNet 클라이언트 모듈

	\~english
	ProudNet client module

	\~chinese
	ProudNet客户端模块

	\~japanese
	ProudNetクライアントのモジュール
	
	\~
	*/
	CAutoPtr<CNetClient> m_NetClient;

	CClientBase(void);
	virtual ~CClientBase(void);

	virtual void FrameMove(float elapsedTime) = 0;

	/**
	\~korean
	camera transform을 설정합니다. g_mView를 설정해야 합니다. 상속 클래스들은 이를 구현해야 합니다.

	\~english
	Set a camera transform and g_m View. This should be realized by inheritance classes realize this.

	\~chinese
	设置camera transform，应设置g_mView。继承类应实现这些。

	\~japanese
	camera transformを設定します。 g_mViewを設定する必要があります。継承クラスはこれを実装する必要があります。

	\~
	*/
	virtual void SetCamera() = 0;

protected:
	CStringW m_NetClientName;

	/**
	\~korean
	오류 출력 메소드입니다.

	\~english
	Print Error

	\~chinese
	打印错误。

	\~japanese
	エラー出力のメソッドです。

	\~
	*/
	void GotoErrorForm(LPCWSTR text);

	/**
	\~korean
	에러 이벤트에 대한 Test를 해볼 수 있는 함수 입니다.

	\~english
	Function that testing about error event

	\~chinese
	可测试错误事件的函数。

	\~japanese
	エラーイベントに対してTestを試みることのできる関数です。

	\~
	*/
	ErrorInfoPtr GetTestErrorInfoPtr();
};
