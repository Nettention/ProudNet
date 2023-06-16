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

#include "ProudNetServer.h"

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

	/** 
	\~korean
	EmergencyLog 서버가 요구하는 delegate

	\~english TODO:translate needed.
	The delegate requested by EmergencyLog server

	\~chinese
	EmergencyLog 服务器需要的delegate。

	\~japanese
	EmergencyLog サーバーが要求する delegate
	\~
	*/
	class IEmergencyLogServerDelegate
	{
	public:
		virtual ~IEmergencyLogServerDelegate() {}

		/**
		\~korean
		서버 실행 파라메터를 설정하는 메서드.
		서버가 시작되는 순간 콜백된다. 사용자는 이 메서드를 통해 서버에게 서버 실행 옵션을 설정해야 한다.

		\param refParam 서버 실행 옵션. 이 함수에서 사용자는 Proud.CStartServerParameter.m_tcpPort 는 반드시 설정해야 한다.
		Proud.CStartServerParameter.m_localNicAddr,Proud.CStartServerParameter.m_serverAddrAtClient 는 필요시 설정하도록 한다.
		나머지 파라메터는 설정하지 않아도 된다.
		주의! CEmergencyLogServer 는 UDP 사용을 하지 않기때문에 m_udpPorts, m_udpAssignMode 를 설정해도 UDP 통신이 되지 않는다.

		\~english TODO:translate needed.
		The method to set server run parameters. 
		This is called back when the server starts. Users must set the server running option through this method. 
		\param refParam Server running option. In this function, the user must configure Proud.CStartServerParameter.m_tcpPort. 
		Proud.CStartServerParameter.m_localNicAddr,Proud.CStartServerParameter.m_serverAddrAtClient shall be configured when necessary. 
		Other parameters need not be configured. 
		Caution! As CEmergencyLogServer does not use UDP, UDP communication is not possible even if m_udpPorts and m_udpAssignMode are selected. 

		\~chinese
		设置服务器的运行参数方法。
		服务器启动瞬间会回拨。用户要利用此方法给服务器设置服务器运行选项。

		\param refParam 服务器运行选项。此函数的用户必须设置 Proud.CStartServerParameter.m_tcpPort%。

		必要时设置 Proud.CStartServerParameter.m_localNicAddr, Proud.CStartServerParameter.m_serverAddrAtClient%。
		剩下的参数不用设置。
		注意！因为 CEmergencyLogServer%不使用UDP，即使设置了m_udpPorts, m_udpAssignMode 也不会进行UDP通信。

		\~japanese
		サーバー実行パラメーターを設定するメソッド
		サーバーが開始される瞬間コールバックされます。ユーザーはこのメソッドを通じサーバーにサーバー実行オプションを設定しなければなりません。
		\param refParam サーバー実行オプション。この関数でユーザーはProud.CStartServerParameter.m_tcpPort を必ず設定しなければなりません。
		Proud.CStartServerParameter.m_localNicAddr,Proud.CStartServerParameter.m_serverAddrAtClientは必要によって設定してください。
		残りのパラメーターは設定しなくても良いです。
		ご注意! CEmergencyLogServerはUDP使用をしないため、m_udpPorts, m_udpAssignMode を設定してもUDP通信ができません。
		\~
		*/
		virtual void OnStartServer(CStartServerParameter &refParam) = 0;

		/** 
		\~korean
		서버가 종료해야 하는 상황(유저의 요청 등)이면 이 함수가 true를 리턴하면 된다. 

		\~english TODO:translate needed.
		If this is a situation where the server must be terminated (user request, etc), then this function shall return true. 

		\~chinese
		如需终止服务器（因用户邀请等原因），此函数返回true即可。

		\~japanese
		サーバーが終了しなければならない状況（ユーザーの要請など）であればこの関数がtrueをリターンすればいいです。
		\~
		*/
		virtual bool MustStopNow() = 0;
		
		/** 
		\~korean
		Critical section 객체를 리턴한다. 개발자는 이 함수를 통해 이미 서버가 사용중인 critical section이나
		별도로 준비한 critical section 객체를 공급해야 한다.  

		\~english TODO:translate needed.
		A critical section object is returned. Developers must supply either a critical section where the server is already being used or a critical section object additionally prepared.  

		\~chinese
		返回Critical section对象。开发者要通过此函数提供服务器正在使用的critical section，或另外准备的critical section。

		\~japanese
		Critical section オブジェクトをリターンします。開発者はこの関数を通じ既にサーバーが使用しているcritical sectionとか別に準備したcritical sectionオブジェクトを供給しなければなりません。 
		  
		\~
		*/
		virtual CriticalSection* GetCriticalSection() = 0;
		
		/** 
		\~korean
		서버 시작이 완료됐음을 알리는 이벤트
		\param err 서버 시작이 성공했으면 NULL이, 그렇지 않으면 ErrorInfo 객체가 들어있다. 

		\~english TODO:translate needed.
		The event that informs the server start is complete. 
		If the server start is successful, NULL, otherwise the ErrorInfo object is in there. 
		

		\~chinese
		告知服务器启动完毕的的事件。
		\param err 服务器启动成功的话有NULL，否则会有ErrorInfo对象。

		\~japanese
		サーバー開始が完了されたことを知らせるイベント
		\param err サーバー開始に成功したらNULLが、失敗したらErrorInfoオブジェクトが入っています。 
		
		\~
		*/
		virtual void OnServerStartComplete(Proud::ErrorInfo *err) = 0;

		/** 
		\~korean
		일정 시간마다 호출된다. 

		\~english TODO:translate needed.
		It is called a fixed interval. 
		 
		\~chinese
		每隔一段时间进行传呼。

		\~japanese
		所定時間ごとに呼び出されます。
		\~
		*/
		virtual void OnFrameMove() {}
	};

	/** 
	\~korean
	EmergencyLog 서버
	일반적 용도
	- 클라이언트는 따로 실행할 필요없음 CNetClient.SendEmergencyLog 를 호출하면 알아서 로그서버로 보냄
	- 생성은 Create()로 한다.
	- RunMainLoop()를 실행하면 로그 서버가 종료할 때까지 역할을 수행한다.

	\~english TODO:translate needed.
	EmergencyLog server
	General use
	- The client needs not be executed additionally. If you call CNetClient.SendEmergencyLog, it sends to the log server on its own. 
	- Creation is done with Create()
	- If you run RunMainLoop(), it performs its role until the log server ends. 

	\~chinese
	EmergencyLog 服务器。
	一般用途
	- client不需另外实行。传呼 CNetClient.SendEmergencyLog%将会自动发送至log服务器。
	- 生成用Create()。
	- 实行RunMainLoop()的话会发挥作用至log服务器结束。

	\~japanese
	EmergencyLog サーバー
	一般的な用途
	- クライアントは別に実行する必要がありません。CNetClient.SendEmergencyLogを呼び出しながらログサーバーに送ります。
	- 生成はCreate()とします。
	- RunMainLoop()を実行するとログサーバーが終了する時まで役割を遂行します。
	\~
	*/
	class CEmergencyLogServer
	{
	public:
		virtual ~CEmergencyLogServer(void) {}

		/** 
		\~korean
		이 메서드를 실행하면 로그 서버가 활성화된다. 이 메서드는 서버가 작동을 중지하라는 요청이 IEmergencyLogServerDelegate 에
		의해 오기 전까지 리턴하지 않는다. 

		\~english TODO:translate needed.
		- If you run this method, the log server is activated. This method does not return until the request to stop running the server is relayed by IEmergencyLogServerDelegate. 

		\~chinese
		实行此方法的话会激活log服务器。在接到通过IEmergencyLogServerDelegate服务器发出的终止运行邀请之前，此方法不会返回。

		\~japanese
		このメソッドを実行するとログサーバーが活性化されます。このメソッドはサーバーが作動を中止しろという要請がIEmergencyLogServerDelegateによって来る前までリターンしません。 
		\~
		*/
		virtual void RunMainLoop() = 0;

		/** 
		\~korean
		CEmergencyLogServer 객체를 생성한다. 

		\~english TODO:translate needed.
		Generate CEmergencyLogServer object. 

		\~chinese
		生成 CEmergencyLogServer%对象。

		\~japanese
		CEmergencyLogServer オブジェクトを生成します。
		\~
		*/
		 static CEmergencyLogServer* Create(IEmergencyLogServerDelegate* dg);
	};

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
