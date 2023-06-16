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

#include "FakeClr.h"
#include "Enums.h"
#include "pnstdint.h"

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
	/** \addtogroup net_group
	*  @{
	*/

	/**
	\~korean
	ProudNet 관련 여러가지 설정 값들이다.
	- 따로 문서화되어 있지 않은 값들은 함부로 변경하지 말 것.

	\~english
	ProudNet related setting values
	- DO NOT change the values that are separately documented!

	\~chinese
	ProudNet 相关各种设定值。
	- 没有文书化的值不要擅自修改。

	\~japanese
	ProudNetに関連する様々な設定値です。
	- 別に文書化されていない値は、勝手に変更しないこと。
	\~

	*/
	class CNetConfig
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		// socket
		PROUD_API static int UdpIssueRecvLength;
		PROUD_API static int UdpRecvBufferLength_Client;
		PROUD_API static int UdpRecvBufferLength_Server;
		PROUD_API static int UdpRecvBufferLength_ServerStaticAssigned;
		PROUD_API static int TcpIssueRecvLength;
		PROUD_API static int TcpRecvBufferLength;
		PROUD_API static int TcpSendBufferLength;
		PROUD_API static int UdpSendBufferLength_Client;
		PROUD_API static int UdpSendBufferLength_Server;
		PROUD_API static int UdpSendBufferLength_ServerStaticAssigned;
		PROUD_API static bool EnableSocketTcpKeepAliveOption;
		PROUD_API static int64_t ReliableUdpHeartbeatIntervalMs_Real;
		PROUD_API static int64_t ReliableUdpHeartbeatIntervalMs_ForDummyTest;

		 PROUD_API static int64_t TcpSocketConnectTimeoutMs;
		 PROUD_API static int64_t ClientConnectServerTimeoutTimeMs;

		//////////////////////////////////////////////////////////////////////////
		// time
		PROUD_API static int64_t DefaultConnectionTimeoutMs;
		PROUD_API static int64_t MinConnectionTimeoutMs;
		PROUD_API static int64_t MaxConnectionTimeoutMs;
		PROUD_API static uint32_t WaitCompletionTimeoutMs;
		PROUD_API static uint32_t HeartbeatIntervalMs;
		 PROUD_API static uint32_t GarbageCollectIntervalMs;

		PROUD_API static int64_t ServerHolepunchIntervalMs;
		PROUD_API static int64_t UdpHolepunchIntervalMs;
		 PROUD_API static int64_t ServerUdpRepunchIntervalMs;
		PROUD_API static int ServerUdpRepunchMaxTrialCount;
		PROUD_API static int ServerUdpHolepunchMaxTrialCount;
		PROUD_API static int64_t P2PHolepunchIntervalMs;
		PROUD_API static int P2PShotgunStartTurn;
		PROUD_API static int P2PHolepunchMaxTurnCount;
		PROUD_API static int64_t PurgeTooOldUnmatureClientTimeoutMs;
		 PROUD_API static int64_t PurgeTooOldAddMemberAckTimeoutMs;
		 PROUD_API static int64_t DisposeGarbagedHostsTimeoutMs;
		PROUD_API static int64_t RemoveTooOldUdpSendPacketQueueTimeoutMs;
		PROUD_API static int64_t AssembleFraggedPacketTimeoutMs;
		 PROUD_API static bool EnablePacketDefragWarning;
		PROUD_API static int64_t GetP2PHolepunchEndTimeMs();
		PROUD_API static int ShotgunTrialCount;
		PROUD_API static int ShotgunRange;

		 PROUD_API static int64_t UnreliablePingIntervalMs;
		PROUD_API static int64_t ReliablePingIntervalMs;

		PROUD_API static bool UseReportRealUdpCount;
		 PROUD_API static int64_t ReportRealUdpCountIntervalMs;

		PROUD_API static uint32_t LanClientHeartbeatIntervalMs;

		PROUD_API static int64_t LanPeerConnectPeerTimeoutTimeMs;
		PROUD_API static int64_t PurgeTooOldJoiningTimeOutIntervalMs;
		PROUD_API static int64_t RemoveLookaheadMessageTimeoutIntervalMs;

		 PROUD_API static int RecyclePairReuseTimeMs;
		PROUD_API static int64_t GarbageTooOldRecyclableUdpSocketsIntervalMs;
		 PROUD_API static int ServerUdpSocketLatentCloseMs;

		PROUD_API static int64_t TcpInDangerThresholdMs;
		PROUD_API static int64_t TcpUnstableDetectionWaitTimeMs;
		PROUD_API static int64_t PauseUdpSendDurationOnTcpInDangerMs;

		PROUD_API static int64_t RecentAssemblyingPacketIDsClearIntervalMs;

		PROUD_API static int AutoConnectionRecoveryTimeoutTimeMs;
		 PROUD_API static int64_t AutoConnectionRecoveryTimeoutMinTimeMs;

		PROUD_API static int64_t AutoConnectionRecoveryRefreshServerAddrInfoTimeoutMs;

		/**
		\~korean
		모든 상대 Remote 들의 IssueSend를 모아서 송신한다.
		- 기본값은 3ms 이다.
		- 이값을 길게 잡으면 송신렉이 생길수 있으니 주의요망! (3ms ~ 10ms)사이가 적당하다.

		\~english
		It sends IssueSend of all other Remotes
		- Defualt value is 3ms
		- It may occur lag of sending, if you set too longe. It will suitable between 3ms ~ 10ms.

		\~chinese
		把所有对方Remote的IssueSend收集后传送信息。
		- 默认值是3ms。
		- 请注意！如果把这个值设置太长的话可能会发生传送缓慢现象。（3ms ~ 10ms）之间为恰当。

		\~japanese
		全ての相手RemoteのIssueSendを集めて送信します。
		- 基本値は3msです。
		- この値を長く取れば送信ラックが発生することがありますのでご注意ください!(3ms~10ms)間が適当です。
		\~
		*/
		 PROUD_API static int EveryRemoteIssueSendOnNeedIntervalMs;

		__forceinline static int64_t GetFallbackServerUdpToTcpTimeoutMs()
		{
			// 나중에, UdpPingInterval로 개명하자. C-S UDP가 증발하건 P2P UDP가 증발하건 한쪽이 증발하면 나머지도 불안한건 매한가지 이므로.
			assert(UnreliablePingIntervalMs > 0);

			// 연속 3번 보낸 것을 못 받는다는 것은 80% 이상 패킷 로스라는 의미. 이 정도면 홀펀칭 되어 있어도 막장이다.
			// 따라서 10->4 하향.
			return UnreliablePingIntervalMs * 4;
		}

		__forceinline static int64_t GetFallbackP2PUdpToTcpTimeoutMs()
		{
			// 나중에, UdpPingInterval로 개명하자. C-S UDP가 증발하건 P2P UDP가 증발하건 한쪽이 증발하면 나머지도 불안한건 매한가지 이므로.
			assert(UnreliablePingIntervalMs > 0);

			// 연속 3번 보낸 것을 못 받는다는 것은 80% 이상 패킷 로스라는 의미. 이 정도면 홀펀칭 되어 있어도 막장이다.
			// 따라서 10->4 하향.
			// GetFallbackServerUdpToTcpTimeoutMs과 GetFallbackP2PUdpToTcpTimeoutMs은 서로 같은 값이어야. 
			// C-S UDP가 증발하건 P2P UDP가 증발하건 한쪽이 증발하면 나머지도 불안한건 매한가지 이므로.
			return UnreliablePingIntervalMs * 4;
		}

		// TCP 핑 타임 아웃에 걸리는 시간
		// 참고: TCP 핑 타임 아웃을 감지하기 위해 핑을 주고 받는 주기는 CNetClientImpl::GetReliablePingTimerInterval 에서 결정.
		PROUD_API static int DefaultNoPingTimeoutTimeMs;

		/**
		\~korean
		선형계획법(?)
		- 너무 작게 잡으면 값의 변화가 작아서 기존 값의 오류를 쉽게 극복 못하고
		너무 크게 잡으면 값의 변화가 커서 대체적 값의 중간점을 유지하기 어렵다.

		\~english
		Linear planning(?)
		- If too small then hard to overcome error from exisiting value to its change being too small,
		If too big then hard to keep the mid-point value due to its change being too big.

		\~chinese
		线性计划法（？）
		- 设太小的话，因为值的变化小而不能容易克服之前值的错误，太大的话，值变化大而很难维持大体值的中间点。

		\~japanese
		線形計画法(?)
		- あまりにも小さく取ると値の変化が小さくて既存値のエラーを簡単に乗り越えることができず、あまりにも大きければ値の変化が大きくて大体の値の中間点を維持しにくいです。
		\~
		*/
		 PROUD_API static int LagLinearProgrammingFactorPercent;

		PROUD_API static int StreamGrowBy;

		 PROUD_API static int InternalNetVersion;

		PROUD_API static int InternalLanVersion;

		/** \~korean 본 함수는 ProudNet Version 정보를 반환합니다.
		\~english This function returns ProudNet Version information.
		\~chinese 此函数返回ProudNet Version信息
		\~japanese この関数は ProudNet Version 情報を返還します。
		\~ */
		PROUD_API static Proud::String GetVersion();

		 PROUD_API static int MtuLength;

		 PROUD_API static int64_t ElectSuperPeerIntervalMs;

		/**
		\~korean
		한 개의 메시지가 가질 수 있는 최대 크기
		- 고정된 값이 아니다. 개발자는 이 값을 언제든지 변경해도 된다.
		- 이 값은 CNetServer 나 CNetClient 객체를 생성하기 전에 미리 설정해야 한다.
		- 하지만 웬만해서는 변경하지 않는 것을 권장한다. 단, 서버간 통신에서 1개의 메시지가 1MB를 넘어가는 수준이라면 예외다.
		- 해킹된 클라이언트에서 잘못된 메시지 크기를 보내는 경우를 차단하려면 CNetServer.SetMessageMaxLength 를 쓰는 것을 권장한다.

		\~english
		The maximum size of a message can have
		- Not a fixed value. Developer can change this at any time.
		- This value must be set previously before creating the CNetServer object or CNetClient object.
		- However, it is not recommended to change this. But, there can be an exception when a message has a size around 1 MB among server communications.
		- To stop a hacked client sending incorrect message size, it is recommended to use CNetServer.SetMessageMaxLength.

		\~chinese
		一个信息能拥有的最大大小。
		- 不是固定值。开发者随时可以修改此值。
		- 此值要在生成 CNetServer%或者 CNetClient%对象之前设置。
		- 但是建议尽量不要修改。不过在服务器之间通信中信息超过1MB的水平的话例外。
		- 在被黑的client中想屏蔽发送错误信息大小的情况的话建议使用 CNetServer.SetMessageMaxLength%。

		\~japanese
		1個のメッセージが持つことができる最大サイズ
		- 固定された値ではありません。開発者はこの値をいつでも変更しても良いです。
		- この値は CNetServerとかCNetClientオブジェクトを生成する前に前もって設定しなければなりません。
		- しかし、できれば変更しないことをお勧めします。但し、サーバー間の通信で1個のメッセージが1MBを乗り越える水準なら例外です。
		- ハッキングされたクライアントで間違ったメッセージのサイズを送る場合を遮断するためには CNetServer.SetMessageMaxLengthを使うことをお勧めします。
		\~
		*/
		// 퇴역
		// static int MessageMaxLength;

		PROUD_API static const int MessageMaxLengthInOrdinaryCase = 64 * 1024;
		PROUD_API static const int MessageMaxLengthInServerLan = 1024 * 1024;

		// 대략, 고정 크기의 작은 메시지 헤더만 담는 경우 허용 가능한 크기. 크기가 매우 작으므로 조심해서 쓸 것!
		PROUD_API static const int MessageMinLength = 128;

		// 메시지 1개의 최대 크기 한계 (고정값)
		PROUD_API static int MessageMaxLength;

		PROUD_API static int64_t DefaultGracefulDisconnectTimeoutMs;

		/**
		\~korean
		서버에서 클라이언트로 P2P routed multicast를 할 때 최대 보낼 수 있는 갯수(router 역할을 하는 클라를 제외함)

		\~english
		The maximum number of P2P routed multicast from server to client (client that acts as router is excluded)

		\~chinese
		从服务器往client进行P2P routed multicast的时候，可以发送的最多个数（当router作用的client例外）

		\~japanese
		サーバーでクライアントへP2P routed multicastをする時、最大で送られる個数(ルーター役割をするクライアントは除外する)
		\~
		*/
		PROUD_API static int MaxS2CMulticastRouteCount;

		 PROUD_API static int UnreliableS2CRoutedMulticastMaxPingDefaultMs;

		PROUD_API static bool ForceCompressedRelayDestListOnly;

		/**
		\~korean
		서버당 최대 동접자수

		\~english
		Maximum number of con-current user per server

		\~chinese
		每个服务器最大同时连接者数。

		\~japanese
		サーバー当たり最大同時接続者数
		\~
		*/
		static const int MaxConnectionCount = 60000;

		/**
		\~korean
		서버에서 한번의 multicast를 할때 heap alloc이 불필요한 수준의 통상적인 최대 송신량

		\~english
		Maximum amount of normal communication that does not require heap alloc when server performs 1 multicast

		\~chinese
		在服务器进行一次multicast的时候，heap alloc 不必要水准的一般最大传送信息量。

		\~japanese
		サーバーで一度のマルチキャストをする時heap allocが不要な水準の通常的な最大送信量
		\~
		*/
		static const int OrdinaryHeavyS2CMulticastCount = 100;

		/** test splitter를 켤 것인가 말 것인가?
		켤 경우 통신량은 약간 증가하나 splitter test를 통해 깨진 serialization을 찾는데 도움이 된다. 
		주의: true로 설정하지 말 것. RMI encryption이 정상 작동 안한다. */
		static const bool EnableTestSplitter = false;

		static const uint32_t ClientListHashTableValue = 101;
	private:
		/**
		\~korean
		config의 값을 변경할 때만 이것을 걸어야 한다!

		\~english
		Use it only for changing config value

		\~chinese
		只在修改config值的时候使用。

		\~japanese
		Configの値を変更する時のみこれをかける必要があります!
		\~
		*/
		PROUD_API static CriticalSection m_writeCritSec;
	public:
		 PROUD_API static bool EnableSpeedHackDetectorByDefault;

		/**
		\~korean
		사용자는 이 값을 false로 바꾸어 메시지 우선순위 기능을 끌 수 있다. 테스트 목적으로만 쓰는 것을 권장한다.

		\~english
		User can turn off message priority function by changing it to false. We recommend to use it for testing

		\~chinese
		用户可以把此值改为false而关闭信息的优先顺序功能。建议只在以test为目的时使用。

		\~japanese
		ユーザーはこの値をfalseに変えてメッセージのプライオリティ機能をオフすることができます。テスト目的でのみ使うことをお勧めします。
		\~
		*/
		PROUD_API  static bool EnableMessagePriority;

		 PROUD_API static int64_t SpeedHackDetectorPingIntervalMs;

		PROUD_API  static CriticalSection& GetWriteCriticalSection();

		 PROUD_API static int DefaultMaxDirectP2PMulticastCount;

		 PROUD_API static bool UpnpDetectNatDeviceByDefault, UpnpTcpAddPortMappingByDefault;

		 PROUD_API static int64_t MeasureClientSendSpeedIntervalMs;

		PROUD_API  static int64_t MeasureSendSpeedDurationMs;

		PROUD_API  static DirectP2PStartCondition DefaultDirectP2PStartCondition;

		/**
		\~korean
		ProudNet 내부 또는 ProudNet에 의해 콜백되는 메서드(RMI 수신 루틴, 이벤트 핸들러 등)에서 unhandled exception(가령 메모리 긁기)이 발생했으며 그것을 사용자가 catch하지 않은 경우 roudNet이 대신 catch 처리한 후 OnUnhandledException으로 넘겨줄 것인지 아니면 그냥 unhandled exception을 냅둘 것인지를 결정하는 변수이다.
		- 기본값은 true이다.
		- 사용자는 언제든지 이 값을 바꿔도 된다.

		\~english
		The variable that decides whether:

		1. to pass as OnUnhandledException after Proudnet processes it as catch when unhandled exception(such as scratching memory) occurs at the method(RMI reception routine, event handler)
		called back by either ProudNet internal or ProudNet while user did not catch,

		2. or to leave unhandled exception as it is.
		- Default is true.
		- User can change this value at any time.

		\~chinese
		被ProudNet内部或者ProudNet返回的方法（RMI 收信routine，event handler 等）发生了unhandled exception而且用户没有catch的时候，决定ProudNet会catch处理后传给OnUnhandledException还是继续留着unhandled exception的变数。
		- 默认值是true。
		- 用户可以随时修改此值。

		\~japanese
		ProudNet内部またはProudNetによってコールバックされるメソッド(RMI受信ルーチン、イベントハンドラーなど)でunhandled exceptionが発生して、それをユーザーがcatchしない場合、roudNetが代わりにcatch処理した後、OnUnhandledExceptionに引き渡すか、それともそのままunhandled exceptionを置いて置くかを決定する変数です。
		- 基本値はtureです。
		- ユーザーはいつでもこの値を変更しても良いです。

		\~
		*/
		 PROUD_API static bool CatchUnhandledException;

		/**
		\~korean
		ProudNet 사용자가 실수를 한 경우 대화 상자로 보일 것인지 여타 방식으로 처리할 것인지를
		결정하는 변수다.
		- 사용자는 언제든지 이 값을 바꿔도 된다.
		- 기본값은 ErrorReaction_MessageBox 이다.

		\~english
		A variable to decide to display ProudNet user mistake as chat box or other way
		- User can change this value at any time.
		- Default is ErrorReaction_MessageBox.

		\~chinese
		决定ProudNet用户失误的时候视为对话框还是用其他方式处理的变数。
		- 用户可以随之修改此值。
		- 默认值是ErrorReaction_MessageBox。

		\~japanese
		ProudNet ユーザーが間違いをした場合、ダイアログボックスで表示するか、他の方式で処理するかを決定する変数です。
		- ユーザーはいつでもこの値を変更しても良いです。
		- 基本値はErrorReaction_MessageBoxです。
		\~
		*/
		 PROUD_API static ErrorReaction UserMisuseErrorReaction;

		/**
		\~korean
		CNetClient 의 내부 처리용 worker thread 의 우선순위를 통상보다 높게 설정하는지 여부
		- 기본값: true이다.
		- 클라이언트 어플리케이션에서 CNetClient 인스턴스를 사용하는 경우 렌더링 프레임이 떨어지는 경우
		이 값을 false로 세팅해서 문제 해결을 할 가능성이 있기도 하다. 하지만 드물다. 차라리 통신량을 줄이는 것이 낫다.
		- 최초의 CNetClient 인스턴스를 만들기 전에만 이 값을 설정할 수 있다.

		\~english
		To decide whether to set priority of internal worker thread of CNetClient to be higher than normal
		- Default: true
		- When using CNetClient instance at client application causes lowering renderinf frames, there is a possibility to solve that by setting this as false. But very rare. It is better to redice communication traffic.
		- This value can be set only, before creating the very first CNetClient.

		\~chinese
		是否把 CNetClient%的内部处理用worker thread的优先顺序设置成比一般高些。
		- 默认值：true。
		- 在client应用中使用 CNetClient%实例的时候，渲染帧下降的时候，可以把此值设置为false从而解决问题。但是很少罕见。还是减少通信量的更好。
		- 只在创建起初 CNetClient%实例之前可以设置此值。

		\~japanese
		CNetClientの内部処理用worker threadのプライオリティを通常より高く設定するかの可否
		- 基本値：trueです。
		-　クライアントアプリケーションでCNetClientインスタンスを使う場合、レンダリングフレームが落ちる場合、この値をfalseに設定して問題解決をする可能性もあります。しかし、稀です。むしろ、通信量を減らした方が増です。
		- 最初のCNetClinetインスタンスを作る前のみこの値を設定することができます。
		\~
		*/
		PROUD_API  static bool NetworkerThreadPriorityIsHigh;

		/**
		\~korean
		소켓의 Shutdown 작업을 abortive or hard으로 진행하고자 할 때 설정하는 변수입니다.
		테스트 목적으로만 쓰는 것을 권장합니다.
		- 기본값: false이다.
		\~english
		It is the variable which is setted when progressing shutdown of socket by aboritive or  hard.
		We recommend to use it for testing
		- Default: false
		\~chinese TODO:translate needed.
		It is the variable which is setted when progressing shutdown of socket by aboritive or  hard.
		We recommend to use it for testing
		- Default: false
		\~japanese TODO:translate needed.
		It is the variable which is setted when progressing shutdown of socket by aboritive or  hard.
		We recommend to use it for testing
		- Default: false
		\~
		*/
		PROUD_API  static bool EnableAbortiveSocketClose;

		/**
		\~korean
		프라우드넷 내부에서 사용되고 있는 Object Pooling 기능을 on/off 할 수 있는 변수 입니다.
		사용자는 이 값을 변경하지 않는 것을 권장합니다.
		- 기본값: true이다.
		\~english
		This variable can turn on or off Object Pooling function in ProudNet.
		We recommend not to change it.
		- Default: true
		\~chinese TODO:translate needed.
		This variable can on/off Object Pooling function in ProudNet.
		We recommend not to change it.
		- Default: true
		\~japanese TODO:translate needed.
		This variable can on/off Object Pooling function in ProudNet.
		We recommend not to change it.
		- Default: true
		\~
		*/
		 PROUD_API static bool EnableObjectPooling;

		/**
		\~korean
		서버 TCP ListenSocket 작업시에 SocketReuseAddress 소켓 옵션을 설정한다.
		- 기본값 : true이다.
		\~english
		\~chinese
		\~japanese
		\~
		*/
		 PROUD_API static bool EnableSocketReuseAddress;

		// static double ReportP2PGroupPingIntervalMs;
		PROUD_API  static int64_t ReportLanP2PPeerPingIntervalMs;
		PROUD_API  static int64_t ReportP2PPeerPingTestIntervalMs;
		 PROUD_API static int64_t ReportServerTimeAndPingIntervalMs;
		 PROUD_API static int64_t LongIntervalMs;

		PROUD_API  static int64_t MinSendSpeed;

		 PROUD_API static int DefaultOverSendSuspectingThresholdInBytes;
		PROUD_API  static bool ForceUnsafeHeapToSafeHeap;
		PROUD_API  static bool EnableSendBrake;
		PROUD_API  static int UdpCongestionControl_MinPacketLossPercent;
		PROUD_API  static int64_t VizReconnectTryIntervalMs;
		 PROUD_API static int64_t SuperPeerSelectionPremiumMs;

		/**
		\~korean
		ProudNet 에서 HostID 재사용 발급시 몇초가 지나야 발급할지를 결정하는 값입니다.

		\~english
		It will decide issue timing for issuing HostID re-use at ProudNet.

		\~chinese
		在ProudNet发放Host ID再次使用时，决定在几秒之后才会发放的值。

		\~japanese
		ProudNetで HostID 再使用発行時に何秒が過ぎて発行するかを決定する値です。
		\~
		*/
		 PROUD_API static int64_t HostIDRecycleAllowTimeMs;

		/**
		\~korean
		Sendqueue의 용량이 얼마나 초과되면 warning를 나오게 할것인지에 대한 값입니다.

		\~english
		It will decide warning message timing depends on amount of Sendqueue

		\~chinese
		对Sendqueue容量超过多少才出现warning的值。

		\~japanese
		Sendqueueの容量がどれだけ超過すればwarningを表示するかに対する値です。
		\~
		*/
		 PROUD_API static int SendQueueHeavyWarningCapacity;

		 PROUD_API static int64_t SendQueueHeavyWarningTimeMs;
		PROUD_API  static int64_t SendQueueHeavyWarningCheckCoolTimeMs;

		/**
		\~korean
		emergency log를 위한 NetClientStats 의 사본을 갱신할 coolTime 값입니다.

		\~english 
		This is the coolTime value to renew the copy of NetClientStats for the emergency log. 

		\~chinese
		为了emergency log，要更新NetClientStats副本的coolTime值。

		\~japanese
		emergency logのためのNetClientStatsのコピーを更新するcoolTime値です。
		\~
		*/
		PROUD_API  static int64_t UpdateNetClientStatCloneCoolTimeMs;

		PROUD_API  static bool EnableErrorReportToNettention;

		PROUD_API  static int64_t ManagerGarbageFreeTimeMs;
		PROUD_API  static unsigned int ManagerAverageElapsedTimeCollectCount;
		PROUD_API  static bool	EnableStarvationWarning;

		 PROUD_API static bool FraggingOnNeedByDefault;
		PROUD_API  static bool CheckDeadLock;
		PROUD_API  static bool UseIsSameLanToLocalForMaxDirectP2PMulticast;

		PROUD_API  static int TcpSendMaxAmount;

		/**
		\~korean
		Thread local storage (TLS) 함수를 사용할지 여부를 결정합니다.
		기본값은 true입니다.
		TLS 함수를 사용할 수 없는 환경에 한해서, 이 값을 false로 설정하십시오.
		(예를 들어 몇 해킹 방지 3rd party 소프트웨어 혼용을 하는 경우)

		\~english 
		This is to determine whether to use the Thread local storage (TLS) function. 
		The default value is true. 
		For the environment where the TLS function cannot be used only, set this value as false. 
		(for example, when an anti-hacking 3rd party software is used together)

		\~chinese
		决定是否使用Thread local storage (TLS)函数。
		基本值是true。
		限于在无法使用TLS函数时的环境下请将此值设置为false。
		（例如混用几种防盗3rd party软件的时候。）

		\~japanese
		Thread local storage (TLS)関数を使うかどうかを決定します。
		基本値はtrueです。
		TLS関数を使用できない環境に限って、この値をfalseに設定してください。
		(例えば、ハッキング防止3rd partyソフトウェアと混用する場合)
		\~
		*/
		PROUD_API static const bool AllowUseTls = true;

		 PROUD_API static bool ConcealDeadlockOnDisconnect;

		//kdh MessageOverload Warning 추가
		PROUD_API  static int MessageOverloadWarningLimit;
		PROUD_API  static int MessageOverloadWarningLimitTimeMs; //sec

		// 		 static int64_t LanServerMessageOverloadTimerIntervalMs;
		// 		 static int64_t LanClientMessageOverloadTimerIntervalMs;
		// 		 static int64_t NetServerMessageOverloadTimerIntervalMs;
		// 		 static int64_t NetClientMessageOverloadTimerIntervalMs;
		PROUD_API  static int64_t MessageOverloadTimerIntervalMs;
		PROUD_API  static int64_t LanRemotePeerHeartBeatTimerIntervalMs;

		PROUD_API  static void AssertTimeoutTimeAppropriate(int64_t ms);
		PROUD_API  static int P2PFallbackTcpRelayResendTimeIntervalMs;

		PROUD_API  static int CleanUpOldPacketIntervalMs;
		PROUD_API  static int NormalizePacketIntervalMs;
		 PROUD_API static void ThrowExceptionIfMessageLengthOutOfRange(int length);

		PROUD_API  static double MessageRecovery_MessageIDAckIntervalMs;

		PROUD_API  static bool ListenSocket_RetryOnInvalidArgError;
		 PROUD_API static bool AllowOutputDebugString;

		PROUD_API static bool DefensiveSendReadyListAdd;
		PROUD_API static bool DefensiveCustomValueEvent;

		PROUD_API static bool EnablePeriodicDetectingTransitionNetwork;

		PROUD_API static const int64_t WaitForSendingDummyPacketIntervalMs;

		/************************************************************************/
		/* ACR 관련 설정변수                                                     */
		/************************************************************************/
		 PROUD_API static bool EnableAutoConnectionRecoveryOnServer;

		 static const int RountTripLatencyTestMaxCount = 1000;
	};
	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
