/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
/*
ProudNet 1.8.00002-master


이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의 : 저작물에 관한 위의 명시를 제거하지 마십시오.


This program is soley copyrighted by Nettention.
Any use, correction, and distribution of this program are subject to the terms and conditions of the License Agreement.
Any violated use of this program is prohibited and will be cause of immediate termination according to the License Agreement.

** WARNING : PLEASE DO NOT REMOVE THE LEGAL NOTICE ABOVE.


此程序的版权归Nettention公司所有。
与此程序的修改、使用、发布相关的事项要遵守此程序的所有权者的协议。
不遵守协议时要原则性的禁止擅自使用。
擅自使用的责任明示在与此程序所有权者的合同书里。

** 注意：不要移除关于制作物的上述明示。


このプログラムの著作権はNettentionにあります。
このプログラムの修正、使用、配布に関する事項は本プログラムの所有権者との契約に従い、
契約を遵守しない場合、原則的に無断使用を禁じます。
無断使用による責任は本プログラムの所有権者との契約書に明示されています。

** 注意：著作物に関する上記の明示を除去しないでください。

*/

#pragma once

#include "PnTime.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
	/** \addtogroup minidump_group
	*  @{
	*/

	/** 
	\~korean
	덤프 서버가 요구하는 delegate

	\~english
	delegate that is requested by dump server

	\~chinese
	转储服务器需要的delegate。

	\~japanese
	ダンプサーバーが要求するdelegate
	\~
	*/
	class IDumpServerDelegate
	{
	public:
		virtual ~IDumpServerDelegate() {}

		/** 
		\~korean
		서버 실행 파라메터를 설정하는 메서드.
		서버가 시작되는 순간 콜백된다. 사용자는 이 메서드를 통해 서버에게 서버 실행 옵션을 설정해야 한다.

		\param refParam 서버 실행 옵션. 이 함수에서 사용자는 Proud.CStartServerParameter.m_tcpPort는 반드시 설정해야 한다.
		Proud.CStartServerParameter.m_localNicAddr,Proud.CStartServerParameter.m_serverAddrAtClient는 필요시 설정하도록 한다.
		나머지 파라메터는 설정하지 않아도 된다.
		주의! DumpServer는 UDP 사용을 하지 않기때문에 m_udpPorts, m_udpAssignMode를 설정해도 UDP 통신이 되지 않는다.

		\~english
		Method that sets server execution parameter
		Called back at the moment server is started. User must set server execution option through this method.

		\param refParam server execution option. Within this function, user must set Proud.CStartServerParameter.m_tcpPort at least.
		Proud.CStartServerParameter.m_localNicAddr and Proud.CStartServerParameter.m_serverAddrAtClient can be set as needed.
		Other parameter, they do not have to be set.
		**Note** Since DumpServer does not use UDP, UDP communication will be not available even if m_udpPorts and/or m_udpAssignMode are set.

		\~chinese
		设置服务器实行参数的方法。
		服务器启动的瞬间回调。用户要通过此方法往服务器设置服务器实行选项。

		\param refParam 服务器实行选项。在此函数用户必须设置 Proud.CStartServerParameter.m_tcpPort%。
		有必要的时候设置 Proud.CStartServerParameter.m_localNicAddr, Proud.CStartServerParameter.m_serverAddrAtClient%。
		剩下的参数不用设置。
		注意！DumpServer 是不使用UDP的， 所以即使设置m_udpPorts, m_udpAssignMode 也不会进行UDP通信。

		\~japanese
		サーバー実行パラメーターを設定するメソッド
		サーバーが開始される瞬間コールバックされます。ユーザーはこのメソッドを通じサーバーにサーバー実行オプションを設定しなければなりません。
		\param refParam サーバー実行オプション。この関数でユーザーは、Proud.CStartServerParameter.m_tcpPortを必ず設定しなければなりません。
		Proud.CStartServerParameter.m_localNicAddr,Proud.CStartServerParameter.m_serverAddrAtClientは必要によって設定するようにします。
		残りのパラメーターは設定しなくても良いです。
		ご注意！DumpServerはUDP使用をしないため、m_udpPorts、m_udpAssignModeを設定してもUDP通信が行われません。
		\~
		*/
		virtual void OnStartServer(CStartServerParameter &refParam) = 0;

		/** 
		\~korean
		서버가 종료해야 하는 상황(유저의 요청 등)이면 이 함수가 true를 리턴하면 된다.

		\~english
		In case when server must be terminated(i.e. upon user request), it can be done if this function returns true.

		\~chinese
		服务器要终止的情况（用户的邀请等）的话，此函数返回true即可。

		\~japanese
		サーバーが終了する状況（ユーザーの要請など）であればこの関数がtrueをリターンすれば良いです。
		\~
		 */
		virtual bool MustStopNow() = 0;
		
		/** 
		\~korean
		Critical section 객체를 리턴한다. 개발자는 이 함수를 통해 이미 서버가 사용중인 critical section이나
		별도로 준비한 critical section 객체를 공급해야 한다.

		\~english
		Returns critical section object. Developer must provide either the critical section that is already used by server through this function or oterwise prepared critical section object.

		\~chinese
		返回Critical section对象。开发者通过此函数要提供服务器已经在使用中的critical section或者另外准备的critical section对象。

		\~japanese
		Critical section オブジェクトをリターンします。開発者はこの関数を通じ既にサーバーが使用しているcritical sectionとか別に準備したcritical sectionオブジェクトを供給しなければなりません。
		\~
		 */
		virtual CriticalSection* GetCriticalSection() = 0;
		
		/** 
		\~korean
		서버 시작이 완료됐음을 알리는 이벤트
		\param err 서버 시작이 성공했으면 NULL이, 그렇지 않으면 ErrorInfo 객체가 들어있다.

		\~english
		Evnet notifies that server start is completed
		\param err NULL if server start is successful, if not then it contains ErrorInfo object.

		\~chinese
		告知服务器启动完成的event。
		\param err 服务器启动成功的话NULL，要不然有ErrorInfo对象。

		\~japanese
		サーバー開始が完了されたことを知らせるイベント
		\~
		 */
		virtual void OnServerStartComplete(Proud::ErrorInfo *err) = 0;
		
		/** 
		\~korean
		기록할 덤프 파일의 이름을 개발자가 이 함수를 통해 공급해야 합니다. 입력 파라메터를 근거로 만들어주면 좋습니다.
		\param clientHostID 덤프 클라이언트의 HostID.
		\param clientAddr 클라이언트의 외부 인터넷 주소 
		\param dumpTime 덤프가 남는 시간

		\~english
		Developer must provide the name of dump file to be recorded through this function. It is ideal if it is based on input parameter.
		\param clientHostID HostID of dump client
		\param clientAddr external internet address of client 
		\param dumpTime time that dump stays

		\~chinese
		开发者通过此函数提供要记录的转储文件的名称。最好以输入参数为依据创建。
		\param clientHostID 转储玩家的Host ID。
		\param clientAddr 玩家的外部互联网地址。
		\param dumpTime 转储剩下的时间。

		\~japanese
		記録するダンプファイルの名前を開発者がこの関数を通じ供給しなければなりません。入力パラメーターを根拠にして作れば良いです。
		\param clientHostID ダンプクライアントのHostID.
		\param clientAddr クライアントの外部インターネットアドレス
		\param dumpTime ダンプが残る時間
		\~
		*/
		virtual String GetDumpFilePath(Proud::HostID clientHostID, const Proud::AddrPort& clientAddr, const ::timespec& tp) = 0;

		/** 
		\~korean
		일정 시간마다 호출된다.

		\~english
		Periodically called

		\~chinese
		每一定时间都会呼叫。

		\~japanese
		所定時間ごとに呼び出されます。
		\~
		 */
		virtual void OnFrameMove() {}
	};

	/** 
	\~korean
	덤프 서버

	일반적 용도
	- 생성은 Create()로 한다.
	- RunMainLoop()를 실행하면 덤프 서버가 종료할 때까지 역할을 수행한다.

	\~english
	dump server

	General usage
	- To create, use Create().
	- If run RunMainLoop() then it runs until dump server is terminated.

	\~chinese
	转储服务器。

	一般用途
	- 生成通过Create()。
	- 实行RunMainLoop()的话，实行其作用直到转储服务器终止。

	\~japanese
	ダンプサーバー

	一般的な用途
	- 生成はCreate()で行います。
	- RunMainLoop()を実行するとダンプサーバーが終了する時まで役割を遂行します。
	\~
	 */
	class CDumpServer
	{
	public:
		virtual ~CDumpServer(void) {}
		/** 
		\~korean
		이 메서드를 실행하면 덤프 서버가 활성화된다. 이 메서드는 서버가 작동을 중지하라는 요청이 IDumpServerDelegate에
		의해 오기 전까지 리턴하지 않는다.

		\~english
		Once this method is run, dump server is activated. This method does not return until it receives server down request from IDumpServerDelegate

		\~chinese
		实行此方法的话会激活转储服务器。终止服务器运转的邀请没有被IDumpServerDelegate传来之前，此方法不会返回。

		\~japanese
		このメソッドを実行するとダンプサーバーが活性化されます。このメソッドはサーバーが作動を中止しろといいう要請がIDumpServerDelegateによって来る前までリターンしません。
		\~
		 */
		virtual void RunMainLoop() = 0;

		/** 
		\~korean
		CDumpServer 객체를 생성한다.
		\param dg IDumpServerDelegate를 상속받은 이벤트 Callback을 받을 class 포인터
		\return 생성된 CDumpServer 객체 포인터

		\~english TODO:translate needed.
		Generates a CDumpServer object. 
		\param dg The class pointer to receive the event callback that inherited IDumpServerDelegate
		\return Generated CDumpServer object pointer

		\~chinese
		生成 CDumpServer%对象。
		\param dg 接收继承IDumpServerDelegate的Callbackevent的class pointer。
		\return 生成的 CDumpServer%对象pointer。

		\~japanese
		CDumpServerオブジェクトを生成します。
		\param dg IDumpServerDelegateを相続してもらったイベントコールバックを受けるクラスポインター
		\return 生成されたCDumpServerオブジェクトポインター
		\~
		 */
		 static CDumpServer* Create(IDumpServerDelegate* dg);
	};
	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
