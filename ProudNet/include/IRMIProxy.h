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

#include "BasicTypes.h"
#include "FakeClr.h"
#include "Enums.h"
#include "IRmiHost.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	class RmiContext;
	class CNetCoreImpl;
	class CMessage;
	class MessageSummary;

	/** 
	\~korean
	PIDL 컴파일러가 생성한 Proxy 클래스의 베이스 클래스

	주의 사항
	- 이 클래스를 유저가 직접 구현하지 말 것. PIDL 컴파일러에서 구현한 것을 쓰도록 해야 한다.

	\~english
	Base class of Proxy class created by PIDL compiler

	Note
	- User must not create this class. Must be realized by PIDL compiler.

	\~chinese
	PIDL编译器生成的Proxy类的基本类。

	注意事象
	- 用户不要直接体现此类。在PIDL编译器要使用体现的东西。

	\~japanese
	PIDLコンパイラーが生成したProxyクラスのベースクラス

	ご注意事項
	- このクラスをユーザーが直接実現しないこと。PIDLコンパイラーによって実現したものを使うようにします。
	\~
	*/
	class IRmiProxy
	{
	public:
		IRmiHost* m_core;
		bool m_internalUse; // true이면 ProudNet 전용. 사용자는 건들지 말것
		int m_signature;

		/** 
		\~korean
		true로 세팅하면 NotifySendByProxy 을 호출받을 수 있다.
		- true로 설정하면 실행 성능이 조금 떨어진다. 하지만 이 성능 차이도 아까운 상황이라면 이 값을 false로 설정해주면 된다.
		- 기본값은 true이다.

		\~english
		If set as true then NotifySendByProxy can be called
		- When set as true, it lowers process performance  by a little. But, if you cannot afford to hasve the loss then set this as false.
		- Default is true.

		\~chinese
		设置为true的话可以呼叫NotifySendByProxy。
		- 设置为true的话运行性能会稍微下降。但对此性能之差感到不便的话，把此值设置为false即可。
		- 默认值是true。

		\~japanese
		trueで設定するとNotifySendByProxyを呼び出してもらえます。
		- trueで設定すると実行性能が少し落ちます。しかし、この性能差ももったいない状況ならこの値をfalseに設定してください。
		- 基本値はtureです。
		\~
		*/
		bool m_enableNotifySendByProxy;

		/** 
		\~korean
		사용자는 이 함수를 오버라이드하지 마십시오.

		\~english
		User must not override this function.

		\~chinese
		用户不要覆盖此函数。

		\~japanese
		ユーザーはこの関数をオーバーライドしないでください。
		\~
		*/
		virtual RmiID* GetRmiIDList() = 0;
		/** 
		\~korean
		사용자는 이 함수를 오버라이드하지 마십시오.

		\~english
		User must not override this function.

		\~chinese
		用户不要覆盖此函数。

		\~japanese
		-ユーザーはこの関数をオーバーライドしないでください。
		\~
		*/
		virtual int GetRmiIDListCount() = 0;
	
		/** 
		\~korean
		메시지 송신을 위해 RMI를 호출할 때(즉 proxy에서 호출하기)마다 이 함수가 callback됩니다.
		프로필러나 RMI 사용 로그를 남기고자 할 때 이 함수를 사용하시면 됩니다. 자세한 내용은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#monitor_rmi_proxy" >송신측(Proxy) 호출 시점을 접근하기</a> 를 참고하십시오.
		- 수신자가 여럿인 경우 여러번 호출됩니다. 
		- 기본 함수는 아무것도 하지 않습니다.
		
		또한 송신 직전에 RmiContext 를 최종 수정할 수 있는 기회를 제공합니다. 
		(기회를 주는 이유: 송신시 잘못된 값이 있으면 경고와 함께 최종 수정을 위함입니다.
		오픈베타시점에서는 문제 분석과 해결을 동시에 해야 하니까요. )
		\param sendTo 수신자
		\param summary 보내는 RMI 메시지의 요약 정보
		\param rmiContext 사용자가 호출한 RmiContext 값입니다.

		\~english
		This function is called back every time RMI is called for message send(e.g. calling from proxy).
		This function is used when there is a need to leave porfiler or RMI use log. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#monitor_rmi_proxy" >Approaching Transmission (Proxy) call</a>.
		- Multiple reciever cause multiple calling.
		- Base function does not do anything. 
		
		Plus, this provides a cance to finally modify RmiContext before sending.
		(Why the last chance is given: for the case there is an incorrect value and to notify, warn and modify finally. During OBT, we need to do prob analysis and solving at the same time.)
		\param sendTo reciever
		\param summary gist of RMI message to be sent
		\param rmiContext RmiContext value called by user

		\~chinese
		为了信息的传送，每当呼叫RMI的时候(即在proxy呼叫)，此函数会callback。
		想剩profiler或者RMI使用log的话可以使用此函数。详细内容请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#monitor_rmi_proxy" >送信方(Proxy) 接近呼出始点</a>%。
		- 收信者是几个人的时候呼叫几次。
		- 基本函数不做任何事情。

		而且在传送之前提供最终修改RmiContext的机会。
		（给机会的理由：为了传送时如果有错误值，要与警告一起最终修改。在公测时要同时进行问题分析和解决）
		\param sendTo 收信者。
		\param summary 发送的RMI信息的概要信息。
		\param rmiContext 用户呼叫的RmiContext值。

		\~japanese
		メッセージ送信のためにRMIを呼び出す時（即ち、proxyで呼び出す）ごとにこの関数がcallbackされます。
		プロフィーラーとかRMI使用ログを残したい時、この関数を使ってください。詳しくは、\ref monitor_rmi_proxyをご参照ください。
		- 受信者が複数である場合は複数回呼び出されます。
		- 基本関数は何もしません。

		また、送信の直前にRmiContextを最終修正する機会を提供します。
		（機会を与える理由：送信時に間違った値があれば警告とともに最終修正のためです。
		オープンベター時点では問題分析と解決を同時にしなければなりませんから）
		\param sendTo 受信者
		\param summary 送るRMIメッセージの要約情報
		\param rmiContext ユーザーが呼び出したRmiContext 値です。
		\~
		*/
		 PROUD_API virtual void NotifySendByProxy(const HostID* remotes, int remoteCount, const MessageSummary &summary, RmiContext &rmiContext, const CMessage &msg);
		
		/** 	
		\~korean
		내부 함수입니다. 사용자는 이 함수를 오버라이드하지 마십시오. 
		ProudClr 에서 오버라이드하기위해 public 으로 해야함

		\~english
		User must not override this function.
		Must be public in order to override at ProudClr 

		\~chinese
		内部函数。用户不要覆盖此函数。
		在ProudClr，为了覆盖要public。

		\~japanese
		内部関数です。ユーザーはこの関数をオーバーライドしないでください。
		ProudClrでオーバーライドするためにはpublicにしてください。
		\~
		*/
		 PROUD_API virtual bool RmiSend(const HostID* remotes, int remoteCount, RmiContext &rmiContext, const CMessage &msg, const PNTCHAR* RMIName, RmiID RMIId);

		 PROUD_API IRmiProxy();
		 PROUD_API virtual ~IRmiProxy();
	};

	 PROUD_API extern const PNTCHAR* ProxyBadSignatureErrorText;

	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
