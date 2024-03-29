/*
ProudNet 1.8.58849-master


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

//#define TEST_DISCONNECT_CLEANUP 테스트 용도

#include "IRmiHost.h"
#include "HlaHost_C.h"
#include "BasicTypes.h"
#include "ConnectParam.h"
#include "HostIDArray.h"
#include "AddrPort.h"
#include "Enums.h"
#include "ErrorInfo.h"
#include "EncryptEnum.h"
#include "RMIContext.h"
#include "ErrorInfo.h"
#include "NetClientStats.h"
#include "LambdaEventHandler.h"
#include "acr.h"
#include "RoundTripLatencyTest.h"

#define PN_DEPRECATED /*__declspec(deprecated) */

#ifdef _MSC_VER // pragma warning은 VC++전용
#pragma warning(push)
#pragma warning(disable:4324)
#endif

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

	class MessageSummary;
	class CTestStats2;
	class CMessage;
	class CNetConnectionParam;
	class CNetPeerInfo;
	class CP2PGroup;

	class CSendFragRefs;
	class ErrorInfo;
	class INetClientEvent;
	class IRmiProxy;
	class IRmiStub;
	class ReliableUdpHostStats;
	class CUserWorkerThreadCallbackContext;

	struct SendOpt;



	/**
	\~korean
	\brief 클라이언트 FrameMove 처리 결과
	- FrameMove에서 메시지와 이벤트가 얼마나 처리 되었는지 의 결과 입니다.

	\~english
	\brief client FrameMove process result
	- The result that shows how much of messages and events were processed by FrameMove

	\~chinese
	\brief clientFrameMove处理结果。
	- 在FrameMove中信息与event处理多少的结果。

	\~japanese
	\brief クライアントFrameMoveの処理結果
	- FrameMoveでメッセージとイベントがどれだけ処理されたのかの結果です。 

	\~
	*/
	class CFrameMoveResult
	{
	public:

		/**
		\~korean
		FrameMove 호출 이후 RMI 호출 횟수입니다.

		\~english
		Number of RMI caling after calling FrameMove

		\~chinese
		FrameMove呼叫以后RMI呼叫次数。

		\~japanese
		FrameMove 呼び出し以降にRMI呼び出し回数です。
		\~
		*/
		uint32_t	m_processedMessageCount;

		/**
		\~korean
		FrameMove 호출 이후 INetClientEvent 콜백 횟수입니다.

		\~english
		Number of INetClientEvent callback after calling FrameMove

		\~chinese
		FrameMove呼叫以后INetClientEvent回调次数。

		\~japanese
		FrameMove 呼び出し以降にINetClientEvent コールバック回数です。
		\~
		*/
		uint32_t	m_processedEventCount;

		CFrameMoveResult():
			m_processedMessageCount(0),
			m_processedEventCount(0)
		{

		}
	};

	/**
	\~korean
	\brief 서버와의 연결 상태를 얻은 결과

	\~english
	\brief Acquired result of the connection status to server

	\~chinese
	\brief 获得与服务器的连接状态的结果。

	\~japanese
	\brief サーバーとの接続状態を得た結果 
	\~
	*/
	class CServerConnectionState
	{
	public:
		/**
		\~korean
		서버와의 UDP 연결이 잘 되어 있는 상태이면 true이다.
		이게 false인 경우, unreliable 메시징은 TCP를 통해 시행된다.

		\~english
		True if UDP connection to server is well
		If false then unreliable messaging is done via TCP

		\~chinese
		与服务器的UDP连接状态良好的话true。
		这是false的话，unreliable messaging通过TCP实行。

		\~japanese
		サーバーとのUDP接続がよくできている状態ならtrueです。
		これがfalseである場合、unreiableメッセージングはTCPを通じ施行されます。

		\~
		*/
		bool m_realUdpEnabled;

		CServerConnectionState()
		{
			m_realUdpEnabled = false;
		}
	};

	/**
	\~korean
	\brief ProudNet 내부에서 Direct P2P 통신을 위해 보유하고 있는 주소 등의 정보입니다.
	CNetClient.GetDirectP2PInfo 등에서 사용됩니다.

	\~english
	\brief Information that contains addresses and others for internal P2P communication in ProudNet.
	Used at CNetClient.GetDirectP2PInfo and so on.

	\~chinese
	\brief 在ProudNet内部为了Direct P2P通信而保留的地址等信息。在 CNetClient.GetDirectP2PInfo%等使用。

	\~japanese
	\brief ProudNet 内部でDirect P2P通信のために保有しているアドレスなどの情報です。 
	CNetClient.GetDirectP2PInfo などで使われます。

	\~
	*/
	class CDirectP2PInfo
	{
	public:
		/**
		\~korean
		CNetClient가 상대 피어와의 Direct P2P 통신을 위해 갖고 있는 UDP 소켓의 로컬 주소
		- CNetClient.GetLocalUdpSocketAddr와 같은 값이다.

		\~english
		Local address of UDP socket possessed by CNetClient for it to use direct P2P communication with opponent peer
		- Has same value as CNetClient.GetLocalUdpSocketAddr

		\~chinese
		CNetClient 与对方peer的Direct P2P通信所拥有的UDP socket本地地址。
		- 与 CNetClient.GetLocalUdpSocketAddr%一样的值。

		\~japanese
		CNetClientが相手peerとのDirect P2P通信のために持っているUDPソケットのローカルアドレス 	
		- CNetClient.GetLocalUdpSocketAddrと同じ値です。 

		\~
		*/
		AddrPort m_localUdpSocketAddr;

		/**
		\~korean
		상대 피어에게 Direct P2P로 패킷을 송신시 상대 피어의 착신 주소이다.
		socket API sendto()에 쓰이기도 한다.

		\~english
		Reception address of opponent peer when packets sent through Direct P2P
		Can be used at socket API sendto()

		\~chinese
		给对方peer用Direct P2P发送数据包时对方peer的收信地址。
		也在socket API sendto()里使用。

		\~japanese
		相手PeerにDirect P2Pでパケットを送信時に相手peerの着信アドレスです。
		socket API sendto()に使われたりします。 

		\~
		*/
		AddrPort m_localToRemoteAddr;

		/**
		\~korean
		상대 피어로부터 Direct P2P 로 패킷을 수신시 상대 피어의 송신 주소이다.
		socket API recvfrom()에 쓰이기도 한다.

		\~english
		Transmission address of opponent peer when packets received through Direct P2P
		Can be used at socket API recvfrom()

		\~chinese
		从对方peer用Direct P2P接收数据包时对方peer的传送地址。
		也在socket API recvfrom()里使用。

		\~japanese
		相手peerからDirect P2Pでパケットを受信時に相手peerの送信アドレスです。
		socket API recvfrom()に使われたりします。 

		\~
		*/
		AddrPort m_remoteToLocalAddr;

		inline CDirectP2PInfo()
		{
			m_localToRemoteAddr = AddrPort::Unassigned;
			m_remoteToLocalAddr = AddrPort::Unassigned;
			m_localUdpSocketAddr = AddrPort::Unassigned;
		}

		/**
		\~korean
		P2P 홀펀칭이 성사되어서 확보된 정보인가?

		\~english
		Is this acquired after P2P hole-punching is completed?

		\~chinese
		是P2P打洞成功后确保的信息吗？

		\~japanese
		P2P ホールパンチングに成功して確保された情報なのか？ 
		\~
		*/
		inline bool HasBeenHolepunched()
		{
			return m_localUdpSocketAddr.IsUnicastEndpoint() && m_localToRemoteAddr.IsUnicastEndpoint() && m_remoteToLocalAddr.IsUnicastEndpoint();
		}
	};

	/**
	\~korean
	\brief 네트워크 클라이언트

	게임 클라이언트와 게임 서버의 네트워킹과 네트워크 클라이언트간의 P2P 통신을 위한 클라이언트입니다.

	자세한 내용은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#client_overview" >네트워크 클라이언트</a> 에 있습니다.

	\~english
	\brief Network Client

	A network client class for client-to-server networking and client-to-client (P2P) networking.

	<a target="_blank" href="http://guide.nettention.com/cpp_en#client_overview" >Network Client</a> explains the details.

	\~chinese
	\brief 联网client。

	为了游戏client和游戏服务器的联网和网络client之间P2P通信的client。

	详细内容在<a target="_blank" href="http://guide.nettention.com/cpp_zh#client_overview" >网络客户端</a>%里。

	\~japanese
	\brief ネットワーククライアント
	ゲームクライアントとゲームサーバーのネットワーキングとネットワーククライアント間のP2P通信のためのクライアントです。
	詳しくは、\ref client_overviewにあります。

	\~
	*/
	class CNetClient : public IRmiHost, public IHlaHost_C, public HasCoreEventFunctionObjects
	{
	public:
		virtual ~CNetClient() {}

	private:
		 PROUD_API static CNetClient* Create_Internal(); // PNTEST가 쓴다.
	public:

		/**
		\~korean
		새 인스턴스를 생성합니다.

		\~english
		Creates new instance.

		\~chinese
		生成新实例。

		\~japanese
		新しいインスタンスを生成します。
		\~
		*/
		inline static CNetClient* Create()
		{
			// 여기서 PROUDNET_H_LIB_SIGNATURE를 건드린다. 이렇게 함으로 이 전역 변수가 컴파일러에 의해 사라지지 않게 한다.
			PROUDNET_H_LIB_SIGNATURE++;
			return Create_Internal();
		}
		/*
		\~korean
		가상으로 랙 유발을 시킨다. 송신, 수신시에 모두 적용된다.
		통상적으로 300,100이 약간 심한 랙 환경을 흉내낸다.

		\~english
		Virtually causes laggy situation. Applies to both transmission and reception.	Usually, 300,100 causes an average laggy situation.

		\~chinese
		诱发虚拟lag。传送和接收信息时都能适用。
		一般300,100模仿较严重的lag环境。

		\~japanese
		仮想でラックを誘発させます。送受信時に全て適用されます。
		通常的に300,100が少しひどいラック環境を真似ます。

		\~
		*/
#ifdef DEPRECATE_SIMLAG
		virtual void SimulateBadTraffic(uint32_t minExtraPing, uint32_t extraPingVariance) = 0;
#endif // DEPRECATE_SIMLAG

		/**
		\~korean
		서버 연결 과정을 시작합니다.
		- 이 함수는 즉시 리턴합니다. 따라서, 이 함수가 리턴했다고 해서 서버와의 연결이 모두 끝난 것은 아닙니다.
		- 이 함수 호출 후 INetClientEvent.OnJoinServerComplete() 이벤트가 도착한 후에야 서버 연결의 성사 여부를 파악할 수 있습니다.

		\param param 연결할 서버 정보입니다.
		\return 이미 다른 서버에 연결된 상태이면 false를 리턴한다. 성공적으로 연결 요청을 시작했으면 true가 리턴됩니다.

		\~english

		Initialtes server connection request.

		- This function returns immediately. Being this function returned does not mean that the connection to server is completed.
		- It is possible to verify if connection to server is completed only after INetClientEvent.OnJoinServerComplete event arrives after calling this function.

		\param connectionInfo specifies server information to connect to.
		\return Returns false is already connected to other server. Returns true if successfully connected.

		\~chinese
		开始与服务器的连接过程。
		- 此函数立即返回。此函数返回不代表与服务器的连接全部结束。
		- 此函数呼叫后 INetClientEvent.OnJoinServerComplete() event到达后才可以知道服务器连接的成功与否。

		\param param 要连接的服务器信息。
		\return 已经是与其他服务器连接的状态的话返回false。成功开始了连接邀请的话返回true。

		\~japanese
		サーバー接続過程を開始します。
		この関数は直ちにリターンします。よって、この関数がリターンしたとしてサーバーとの接続が全て終わったのではありません。
		- この関数を呼び出した後、INetClientEvent.OnJoinServerComplete() イベントが到着した後になってサーバー接続の成功可否を把握することができます。
		\param param 接続するサーバー情報です。
		\return 既に他のサーバーに接続された状態であればfalseをリターンします。接続要請を開始したらtrueがリターンされます。

		\~
		*/
		virtual bool Connect(const CNetConnectionParam &connectionInfo) = 0;
		virtual bool Connect(const CNetConnectionParam &connectionInfo, ErrorInfoPtr &outError) = 0;

		/**
		\~korean
		서버와의 연결을 해제한다. 아울러 모든 P2P 그룹에서도 탈퇴한다.
		- 자세한 것은 동명 메서드 참조

		\~english
		Terminates the connection to server and withdraws from all P2P group.
		- Please refer to same name method for more detail.

		\~chinese
		解除与服务器的连接。之后退出所有P2P组。
		- 详细请参考同名方法。

		\~japanese
		サーバーとの接続を解除します。ちなみに、すべてのP2Pグループからも脱退します。
		- 詳しくは、同名メソッドをご参照ください。

		\~
		*/
		virtual void Disconnect() = 0;

		/**
		\~korean
		서버와의 연결을 해제한다. 아울러 모든 P2P 그룹에서도 탈퇴한다.
		\param gracefulDisconnectTimeout 서버와의 연결을 해제하는 과정을 처리하기 위해 클라이언트는 일정 시간의
		시간을 요구한다. 이 값은 서버와의 연결을 해제하는 데까지 허락하는 최대 시간(초)이다.
		이 값은 통상적으로 1 이내가 적당하지만, 너무 지나치게 작은 값을 잡는 경우, 클라이언트는 서버와의
		연결을 종료했지만 서버측에서 클라이언트의 연결 해제를 즉시 감지하지 못하는 경우가 있을 수 있다.
		\param comment 여기에 채워진 데이터는 INetServerEvent.OnClientLeave에서 받아진다.
		즉 클라이언트가 서버와의 연결을 해제하되 서버에게 마지막으로 데이터를 보내고자 할 때(예: 접속을 끊는 이유를 보내기) 유용하다.
		gracefulDisconnectTimeout가 너무 짧으면 못 갈수 있다.

		\~english
		Terminates the connection to server and withdraws from all P2P group.
		\param gracefulDisconnectTimeout Client requires a certain amount of time in order to process the steps to terminate the connection to server. This value is the maximum time(in second) that allowed to complete the termination.
		Usually 1 is reasonable for the value but if it is too small then there can be some cases that server cannot detect clinet's diconnection immediately after client terminated the connection.
		\param comment The data filled in here will be received at INetServerEvent.OnClientLeave.
		In other words, it is useful when client need to send its last data to server before terminating its connection to server.(e.g. sending why terminating the connection)
		If gracefulDisconnectTimeout is too short then there is a chance the sending fails.

		\~chinese
		解除与服务器的连接。之后退出所有P2P组。
		\param gracefulDisconnectTimeout 为了处理与服务器连接的解除过程，client 要求一定时间的时间。此值是解除与服务器的连接的允许的最大时间（秒）。
		此值一般1以内为适当，用过小的值的话可能会发生client与服务器的连接虽已终止，服务器方不能立即感知client的连接解除的情况。
		\param comment 往这里填充的数据在 INetServerEvent.OnClientLeave%接收。即client解除与服务器的连接，想最后给服务器传送数据的时候有用（例：发送断开连接的理由）。
		gracefulDisconnectTimeout 太短的话可能不能到达。

		\~japanese
		サーバーとの接続を解除します。ちなみに、すべてのP2Pグループからも脱退します。
		\param gracefulDisconnectTimeout サーバーとの接続を解除する過程を処理するために、クライアントは所定時間の時間を求めます。この値はサーバーとの接続解除までに許可する最大時間(秒)です。
		この値は通常的に1以内が適当ですが、あまりにも小さい値を取る場合、クライアントはサーバーとの接続を終了しましたが、サーバー側でクライアントの接続解除を直ちに感知できない場合があり得ます。
		\param comment ここに入れたデータは、INetServerEvent.OnClientLeaveより受け取られます。
		即ち、クライアントがサーバーとの接続を解除しますが、サーバーに最後にデータを送りたい時(例：接続を切る理由を送る)有用です。
		gracefulDisconnectTimeoutがあまりにも短いと届かない場合があります。 

		\~
		*/
		virtual void Disconnect(const CDisconnectArgs &args) = 0;

		/**
		\~korean
		서버와의 연결을 해제한다. 아울러 모든 P2P 그룹에서도 탈퇴한다.
		UE4/PS4 와 같이 사용자 단에서 try/catch 를 사용 할 수 없는 환경에서, 에러 메세지를 통보 받을 때 사용 하도록 한다.
		- 자세한 것은 동명 메서드 참조

		\~english
		It disconnects the connection to the server. Additionally, it withdraws from all P2P groups. 
		In an environment where try/catch cannot be used from the user side such as UE4/PS4, it shall be used when an error message is notified. 
		- For details, refer to the method with the same name. 

		\~chinese
		解除与服务器的连接。同时从所有P2P组退出。
		像UE4/PS4这种不能在客户端使用try/catch的环境中，获得错误消息时使用。
		- 详细的内容请参照同名方法

		\~japanese
		サーバーとの接続を解除します。ちなみに、すべてのP2Pグループからも脱退します。
		UE4/PS4のように、ユーザー側でtry/catchを使用できない環境で、エラーメッセージを通報してもらう時に使うようにします。
		- 詳しいことは同名のメソッドを参照してください。

		\~
		*/
		virtual void Disconnect(ErrorInfoPtr& outError) = 0;

		/**
		\~korean
		서버와의 연결을 해제한다. 아울러 모든 P2P 그룹에서도 탈퇴한다.
		UE4/PS4 와 같이 사용자 단에서 try/catch 를 사용 할 수 없는 환경에서, 에러 메세지를 통보 받을 때 사용 하도록 한다.
		- 자세한 것은 동명 메서드 참조

		\~english
		It disconnects the connection to the server. Additionally, it withdraws from all P2P groups. 
		In an environment where try/catch cannot be used from the user side such as UE4/PS4, it shall be used when an error message is notified. 
		- For details, refer to the method with the same name. 

		\~chinese
		解除与服务器的连接。同时从所有P2P组退出。
		像UE4/PS4这种不能在客户端使用try/catch的环境中，获得错误消息时使用。
		- 详细的内容请参照同名方法

		\~japanese
		サーバーとの接続を解除します。ちなみに、すべてのP2Pグループからも脱退します。
		UE4/PS4のようにユーザー側でtry/catchを使用できない環境で、エラーメッセージを通報してもらった時に使うようにします。
		- 詳しくは同名メソッドをご参照ください。

		\~
		*/
		virtual void Disconnect(const CDisconnectArgs &args, ErrorInfoPtr& outError) = 0;

		// Added for emergency use if Disconnect() never returns though it has automatic non-wait functionality.
		virtual void DisconnectAsync(const CDisconnectArgs &args) = 0;

		/**
		\~korean
		[디버깅용] 전체 P2P 그룹의 상태를 덤프로 남긴다.

		\~english
		[Debugging] Leaves the status of entire P2P group as a dump

		\~chinese
		[调试用] 把全部P2P组的状态留为转储。

		\~japanese
		[デバッギング用]全体P2Pグループの状態をダンプで残します。
		\~
		*/
		//virtual String DumpStatus() = 0;

		/**
		\~korean
		수신된 RMI나 이벤트를 처리합니다.
		가장 마지막에 FrameMove을 호출한 이후부터 지금까지 서버로부터 수신된 RMI나 INetClientEvent의 콜백 이벤트는 클라이언트 메모리에 누적되어 있습니다.
		그리고 그것들을 이 메서드에 의해 일제히 콜백이 발생하게 합니다.
		- 사용자는 일정 시간마다 이를 호출해서 RMI 수신 처리 및 발생 이벤트를 처리하도록 해야 합니다. 일반적인 경우 사용자는 이 메서드를 매 렌더 프레임마다 호출합니다.
		- 이 메서드를 장시간 호출 안한다고 해서 타 호스트나 서버와의 연결이 끊어지는 일은 없습니다.

		\param maxWaitTimeMs 처리할 이벤트나 수신 메시지가 있을 때까지, 얼마나 오래 기다릴 것인지에 대한 값입니다. 0이면 기다리지 않고 즉시 리턴합니다.
		게임 등 렌더링 루프 안에서는 0이 일반적이며, 렌더링 루프가 없는 일반 앱에서는 적당한 값 (가령 200ms)를 넣습니다.
		\param outResult FrameMove 호출시 처리 결과 보고를 얻습니다. 생략 가능한 파라메터입니다.

		\~english
		Handles received RMI or event
		All of RMI from server or callbcak event of INetClientEvent occurred since the last FrameMove call are stacked within memory.
		And those let callbacks happen by this method.
		- User is to call this periodically in order to handle RMI reception process and event occurred. Usually, user calls this method at every render frame.
		- By not calling this method for a long time does not cause the connection between server or other hosts terminated.

		\param maxWaitTimeMs  It is the value of how long it will wait until there is a processed event or in-coming message,  If it is 0, do not wait and return immediately.
		0 is general in the rendering loop including game, and etc.  Put the appropriate value (for example, 200ms) in the general app without the rendering loop.
		\param outResult Gets the report to process result when FrameMove is called. Ignorable parameter.

		\~chinese
		处理收信的RMI或者event。
		从最后呼叫FrameMove开始到现在从服务器收信的RMI或者INetClientEvent的回拨event累计在client内存里。
		然后利用此方法一齐发生回调。
		- 用户要在每一段时间呼叫它进行RMI收信处理及event发生处理。一般的情况下用户把此方法在每个render frame呼叫。
		- 不会发生因没有长时间呼叫此方法而与其他主机或者服务器的连接断开的事情。

		\param maxWaitTimeMs  是对要处理的活动或者到有收信信息的时候为止,以及需要等多长时间的价格。如果是 0 的话,则立即返回,不将等待。
		在游戏等绘制圈里,0是一般性的, 在没有绘制圈的应用软件里,放入适当的价格(例如200ms)。
		\param outResult FrameMove呼叫时获得处理结果报告。可以省略的参数。

		\~japanese
		受信されたRMIとかイベントを処理します。
		一番最後にFrameMoveを呼び出した後から、これまでサーバーより受信したRMIとかINetClientEventのコールバックイベントはクライアントメモリーに累積されています。
		そして、それらをこのメソッドによって一斉にコールバックが発生するようにします。
		- ユーザーは所定時間ごとにこれを呼び出してRMI受信処理及び発生イベントを処理するようにしなければなりません。一般的な場合、ユーザーはこのメソッドを毎回レンダーリングフレームごとに呼び出します。
		- このメソッドを長時間呼び出さないとして他のホストやサーバーとの接続が切れることはありません。
		\param maxWaitTimeMs 処理するイベントや受信メッセージがある時まで、どれだけ長く待つかに対する値です。0であれば待たずに直ちにリターンします。
		ゲームなどレンダーリングループ内では0が一般的で、レンダーリングループがない一般アプリでは適当な値(仮に200ms)を入れます。
		\param outResult FrameMove 呼び出し時に処理結果の報告を得ます。省略可能なパラメーターです。 


		\param maxWaitTimeMs  処理するイベントや受信メッセージがあるまで、どれほど長く待つのかについた値です。0には待たずに直ちにリターンします。
		ゲームなどレンダリングループの中では0が一般的であり、レンダリングループがない一般アプリでは適当な値(例えば、200ms)を入れます。

		\~
		*/
		virtual void FrameMove(int maxWaitTime = 0, CFrameMoveResult* outResult = NULL) = 0;
		/** No-throw function. Exception is stored to outError. Used for UE4, etc. */
		virtual void FrameMove(int maxWaitTime, CFrameMoveResult* outResult, ErrorInfoPtr& outError) = 0;

		/**
		\~korean
		이 client가 참여하고 있는 P2P group 중 하나인 groupHostID에 참여하고 있는
		다른 peer들의 HostID list를 얻는다.

		\~english
		Gets the HostID list of other peers participating groupHostID that is one of P2P groups that this client is participating

		\~chinese
		获得参与此client参与的P2P group中之一groupHost ID的其他peer HostID list。

		\~japanese
		このクライアントが参加しているP2Pグループの一つであるgroupHostIDに参加している他のPeerのHostID listを得ます。
		\~
		*/
		virtual void GetGroupMembers(HostID groupHostID, HostIDArray &output) = 0;

		/**
		\~korean
		다른 peer에서 가지고 있을 서버 시간을 구한다.

		\~english
		Gets the server time that other peer has

		\~chinese
		求其他peer会拥有的服务器时间。

		\~japanese
		他のPeerが持っているサーバー時間を求めます。
		\~
		*/
		virtual int64_t GetIndirectServerTimeMs(HostID peerHostID) = 0;

		/**
		\~korean
		이 호스트의 local HostID를 구한다. HostID_None이면 서버에 연결 안됐다는 뜻.

		\~english
		Gets local HostID of this host. If HostID_None then means not connected to server

		\~chinese
		求此主机的local Host ID。HostID_None 的话表示没有与服务器连接。

		\~japanese
		このホストのlocal HostIDを求めます。HostID_Noneであればサーバーに接続されなかったという意味。
		\~
		*/
		virtual HostID GetLocalHostID() = 0;

		/**
		\~korean
		이 클라이언트가 속한 네트워크에 작동중인 인터넷 공유기 장치(NAT device)의
		이름을 얻는다.

		\~english
		Gets the name of Internet router device(NAT device) that is active at the network that this client is participating.

		\~chinese
		给此client所属的网络获得运转中的互联网路由器装置（NAT device）的名字。

		\~japanese
		このクライアントが属したネットワークに作動中のインターネットルーター(NAT device)の名前を得ます。
		\~
		*/
		virtual String GetNatDeviceName() = 0;

		/**
		\~korean
		이 호스트가 가입한 모든 P2P 그룹 리스트를 얻는다.

		\~english
		Gets the entire P2P group list that this host is participating

		\~chinese
		获得此主机加入的所有P2P组列表。

		\~japanese
		このホストが加入した全てのP2Pグループリストを得ます。
		\~
		*/
		virtual void GetLocalJoinedP2PGroups(HostIDArray &output) = 0;

		/**
		\~korean
		시작 이래 수집된 처리량 통계를 얻는다.

		\~english
		Gets the statistics of process collected since beginning

		\~chinese
		获得开始依赖收集的处理量统计。

		\~japanese
		開始以来に収集された処理量の統計を得ます。
		\~
		*/
		virtual void GetStats(CNetClientStats &outVal) = 0;

		/**
		\~korean
		P2P group의 서버 시간을 얻는다.
		이는 groupHostID가 가리키는 P2P group의 모든 GetIndirectServerTime()값의 평균이다.

		\~english
		Gets server time of P2P group
		This is an average value of all GetIndirectServerTime() values within the P2P group that groupHostID points.

		\~chinese
		获得P2P group的服务器时间。
		这是groupHostID所指的P2P group的所有GetIndirectServerTime()值的平均。

		\~japanese
		P2Pグループのサーバー時間を得ます。
		これはgroupHostIDが指すP2Pグループの全てのGetIndirectServerTime()値の平均です。

		\~
		*/
		virtual int64_t GetP2PServerTimeMs(HostID groupHostID) = 0;

		/**
		\~korean
		이 호스트가 타 Peer와의 통신을 위해 내부적으로 갖고 있는 UDP socket의 로컬 주소를 얻는다.
		- <a target="_blank" href="http://guide.nettention.com/cpp_ko#use_alternative_p2p" >ProudNet을 쓰지 않고 ProudNet의 홀펀칭 정보를 토대로 별도 UDP 소켓으로 통신하기</a>  에서 사용되기도 한다.

		\param remotePeerID 이 호스트와 P2P 통신을 하고 있는 타 Peer의 HostID.
		\return CNetClient 내부에서 보유하고 있는 UDP socket의 포트번호 또는 (주소,포트번호). 사용자는 여기서 포트 번호를 가져다 쓰면 된다.
		- P2P 그룹에 상대측 peer가 들어온지 얼마 안된 경우 이 값은 AddrPort.Unassigned일 수 있다. 왜냐하면 ProudNet은 성능 향상을 위해
		UDP socket을 즉시 만들지 않기 때문이다. 따라서 이러한 경우 잠시 뒤에 다시 이 메서드를 호출해서 얻을 수 있다.

		\~english
		Gets the local address of UDP socket that this host has internally for communicating with other peer
		- Also used in <a target="_blank" href="http://guide.nettention.com/cpp_en#use_alternative_p2p" >Communicating with a separated UDP socket based on ProudNet hole-punched information without using ProudNet.</a>

		\param remotePeerID HostID of other peer that P2P communicates with this host
		\return CNetClient Port number or (address, port number) of UDP socket that is possessed internally. User can use this as port number.
		- This value can be AddrPort.Unassigned when the opponent peer entered P2P group recently.
		This is because ProudNet does not create UDP socket immediately for performance reason. So, if this happens then you must call this method again some moments later.

		\~chinese
		获得此主机为了与其他peer的通信，内部UDP socket的本地地址。
		-  也在<a target="_blank" href="http://guide.nettention.com/cpp_zh#use_alternative_p2p" >不使用ProudNet，只基于ProudNet的打洞情报用另外的UDPSocket进行通信</a>%使用。

		\param remotePeerID 此主机和P2P进行通信的其他peer的Host ID。
		\return CNetClient%内部保留的UDP socket的端口号码或者（地址端口号码）用户从这里拿端口号码即可。
		- 往P2P组对方peer进入没有多少时间的时候此值可能是 AddrPort.Unassigned%。因为ProudNet为了性能提高不会立即创建UDP socket。所以这样的情况下可以稍后再呼叫此方法获得。

		\~japanese
		このホストが他のPeerとの通信のために内部的に持っているUDPソケットのローカルアドレスを得ます。
		- \ref use_alternative_p2pで使われることもあります。 
		\param remotePeerID このホストとP2P通信をしている他のPeerのHostID.
		\return CNetClient 内部で保有しているUDPソケットのポート番号または(アドレス、ポート番号)。ユーザーはここでポート番号を取り込んで使えば良いです。
		- P2Pグループに相手側のPeerが入って間もない場合、この値はAddrPort.Unassignedであることもあります。なぜなら、ProudNetは性能向上のためにUDPソケットを直ちに作らないからです。よって、このような場合、しばらく後で再びこのメソッドを呼び出して得ることができます。

		\~
		*/
		virtual AddrPort GetLocalUdpSocketAddr(HostID remotePeerID) = 0;

		/**
		\~korean
		이 호스트가 remotePeerID가 가리키는 타 Peer와의 통신을 위해 홀펀칭된 정보를 얻는다.
		- <a target="_blank" href="http://guide.nettention.com/cpp_ko#use_alternative_p2p" >ProudNet을 쓰지 않고 ProudNet의 홀펀칭 정보를 토대로 별도 UDP 소켓으로 통신하기</a>  에서 사용되기도 한다.

		\param remotePeerID 타 피어의 ID
		\param outInfo 타 피어와의 통신을 위한 홀펀칭 정보가 채워질 곳
		\return 홀펀칭된 Peer인 경우 true, 그 외의 경우 false를 리턴한다. 만약 false를 리턴한 경우 아직 홀펀칭되지 않은 peer인 경우에는 0.3~1초 간격으로
		이 메서드를 지속적으로 호출하다 보면 true를 리턴할 때가 있다. 왜냐하면 홀펀칭이 성사되는 시간이 항상 다르기 때문이다.

		\~english
		Gets the hole-punched information for this host to communicate with other peer that remotePeerID points
		- Also used in <a target="_blank" href="http://guide.nettention.com/cpp_en#use_alternative_p2p" >Communicating with a separated UDP socket based on ProudNet hole-punched information without using ProudNet.</a>

		\param remotePeerID ID of other peer
		\param outInfo where the hole-punching information for communicating with other peer to be filled
		\return True if hole-punched peer, otherwise returns false.
		For the peer that is not hole-punched yet, there are some occasions that it returns true when calling this method in every 0.3 ~ 1 seconds.
		This happens since the moment when hole-punching is completed differs all the time.
		\~chinese
		此主机为了与remotePeerID所指的其他peer的通信获得打洞信息。
		- 也在<a target="_blank" href="http://guide.nettention.com/cpp_zh#use_alternative_p2p" >不使用ProudNet，只基于ProudNet的打洞情报用另外的UDPSocket进行通信</a>%使用。

		\param remotePeerID 其他peer的ID。
		\param outInfo 为了与其他peer的通信的打洞信息要被填充的地方。
		\return 打洞的peer的话true，之外的情况返回false。如果返回false的话，还没有打洞的peer的情况是以0.3~1间隔，持续呼叫此方法的话有返回true的时候。因为打洞成功的时间总是不同。

		\~japanese
		このホストがremotePeerIDが指す他のPeerとの通信のためにホールパンチングされた情報を得ます。
		- \ref use_alternative_p2pで使われることもあります。 		
		\param remotePeerID 他のPeerのID
		\param outInfo 他のPeerとの通信のためのホールパンチング情報が満たされる所
		\return ホールパンチングされたPeerの場合はtrue、その他の場合はfalseをリターンします。もし、falseをリターンした場合、まだホールパンチングされてないpeerの場合は0.3~1秒の間隔でこのメソッドを持続的に呼び出しながら見ると、trueをリターンする時があります。なぜなら、ホールパンチングに成功する時間はいつも違うからです。 

		\~
		*/
		virtual bool GetDirectP2PInfo(HostID remotePeerID, CDirectP2PInfo &outInfo) = 0;

		/**
		\~korean
		연결된 서버의 주소를 구한다.

		\~english
		Gets the address of connected server

		\~chinese
		求连接的服务器地址。

		\~japanese
		接続されたサーバーのアドレスを求めます。
		\~
		*/
		virtual AddrPort GetServerAddrPort() = 0;

		/**
		\~korean
		이 객체에 연결된 peer 1개의 정보를 얻는다.
		\param peerHostID 찾으려는 peer의 HostID
		\param output 찾은 peer의 정보. 못 찾으면 아무것도 채워지지 않는다.
		\return true이면 찾았다는 뜻

		\~english
		Gets the information of 1 peer connected to this object
		\param peerHostID HostID of peer to find
		\param output the information of peer found. Nothing will be filled when found nothing.
		\return True means found

		\~chinese
		获得连接此对象的一个peer信息。
		\param peerHostID 要找的peer的HostID。
		\param output 找到的peer的信息。没有找到的话不填充任何东西。
		\return true的话是找到的意思。

		\~japanese
		このオブジェクトにつながったPeer 1個の情報を得ます。
		\param peerHostID 探したいpeerのHostID
		\param output 探したpeerの情報。探すことができなかったら何も満たされません。
		\return trueであれば探したという意味

		\~
		*/
		virtual bool GetPeerInfo(HostID peerHostID, CNetPeerInfo& output) = 0;

		/**
		\~korean
		이 호스트에 연결된 다른 호스트(서버의 경우 클라이언트들, 클라이언트의 경우 피어들)들 각각에의 tag를 지정합니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#host_tag" >각 호스트에 대한 사용자 정의 데이터(Tag)</a> 기능입니다.
		- 자기 자신(서버 혹은 클라이언트)에 대한 tag도 지정할 수 있습니다.

		\~english
		Desigantes tag to each of other hosts(client for server, peer for client) aht are connected to this host. A <a target="_blank" href="http://guide.nettention.com/cpp_en#host_tag" >User-defined data(Tag) for each host</a> function.
		- Can designate tag to itself (server or client)

		\~chinese
		给连接此主机的其他主机（服务器的话client，client 的话peer）指定tag。<a target="_blank" href="http://guide.nettention.com/cpp_zh#host_tag" >对各个主机的用户定义数据（Tag）</a> %功能。
		- 也可以对自己本身（服务器或者client）指定tag。

		\~japanese
		このホストにつながった他のホスト(サーバーの場合はクライアント、クライアントの場合はピーア)のそれぞれにtagを指定します。\ref host_tag 機能です。
		- 自己自身(サーバーまたはクライアント)に対するtagも指定することができます。 

		\~
		*/
		virtual bool SetHostTag(HostID hostID, void* hostTag) = 0;

		/**
		\~korean
		서버의 현재 시간을 구한다. (초 단위)
		이 값은 일정 시간마다 레이턴시도 고려되어 계산되는 서버의 시간이다.

		\~english
		Gets current time of server (in second)
		This value is the server time that considered periodic latency

		\~chinese
		求服务器的现在时间（秒单位）。
		此值是每隔一段时间考虑latency而计算的服务器时间。

		\~japanese
		サーバーの現在時間を求めます。(秒単位)
		この値は所定時間ごとにレーテンシーも考慮されて計算されるサーバーの時間です。

		\~
		*/
		virtual int64_t GetServerTimeMs() = 0;

		/**
		\~korean
		클라이언트와 서버와의 시간 차를 구한다.

		\~english
		Gets the time difference between client and server

		\~chinese
		求client与服务器的时间差。

		\~japanese
		クライアントとサーバーとの時間差を求めます。
		\~
		*/
		virtual int64_t GetServerTimeDiffMs() = 0;

		/**
		\~korean
		서버와의 소켓 연결 상태를 리턴합니다.
		- 소켓의 연결 상태만 리턴합니다. 완전히 연결되었는지는 OnJoinServerComplete로 콜백됩니다.
		\param output 서버와의 연결 상태가 채워지는 공간
		\return 서버와의 연결 상태

		\~english
		Returns connection status to server
		\param output where the status is filled
		\return connection status to server

		\~chinese
		返回服务器和socket连接状态。
		- 值返回socket连接状态。是否完全连接用OnJoinServerComplete回调。
		\param output 填充与服务器的连接状态的空间。
		\return 与服务器的连接状态。

		\~japanese
		サーバーとのソケット接続状態をリターンします。
		- ソケットの接続状態のみをリターンします。完全につながっているかはOnJoinServerCompleteにコールバックされます。
		\param output サーバーとの接続状態が満たされる空間
		\return サーバーとの接続状態 

		\~
		*/
		virtual ConnectionState GetServerConnectionState(CServerConnectionState &output) = 0;


		virtual void GetWorkerState(CClientWorkerInfo &output) = 0;

		/**
		\~korean
		서버와의 소켓 연결 여부를 리턴합니다.
		- 서버와 소켓이 연결 되었는지의 여부만 리턴합니다.

		\~english
		Return connect socket to server or not.
		- Only return connected socket to server or not.

		\~chinese
		返回与服务器的socket连接与否。
		- 只返回服务器和socket连接与否。

		\~japanese
		サーバーとのソケット接続可否をリターンします。
		- サーバーとソケットが接続されているかどうかのみをリターンします。

		\~
		*/
		inline bool HasServerConnection()
		{
			CServerConnectionState stat;
			return GetServerConnectionState(stat)==ConnectionState_Connected;
		}

		/**
		\~korean
		이벤트를 받을 수 있는 객체를 설정한다.
		- 하나만 설정 가능하다.
		- 이 메서드는 클라이언트가 서버에 연결을 시도하기 전에만 호출할 수 있다. 안그러면 exception이 throw된다.

		\~english
		Sets the object can receive event
		- Only 1 object can be set.
		- This method can only be called before client attempts to connect to server. Otherwise, an execption will be 'throw'.

		\~chinese
		设置可以接收event的对象。
		- 只能设置一个。
		- 此方法只能在client试图连接服务器之前呼叫。否则会exception 이 throw。

		\~japanese
		イベントを受けられるオブジェクトを設定します。
		- 一つのみ設定可能です。
		- このメソッドはクライアントがサーバーに接続を試す前のみ呼び出すことができます。それとも、exceptionがthrowされます。 

		\~
		*/
		virtual void SetEventSink(INetClientEvent* eventSink) = 0;

		// rarely used for diagnostics.
		virtual INetClientEvent* GetEventSink() = 0;

	public:


		// 주의: 실존 객체이므로 SUPPORTS_LAMBDA_EXPRESSION로 감싸지 말 것.

		/// See Proud.INetClientEvent.OnJoinServerComplete for details.
		LambdaBaseHolder_Param2<void, ErrorInfo*, const ByteArray&> OnJoinServerComplete;
		/// See Proud.INetClientEvent.OnLeaveServer for details.
		LambdaBaseHolder_Param1<void, ErrorInfo*> OnLeaveServer;
		/// See Proud.INetClientEvent.OnP2PMemberJoin for details.
		LambdaBaseHolder_Param4<void, HostID, HostID, int, const ByteArray &> OnP2PMemberJoin;
		/// See Proud.INetClientEvent.OnP2PMemberLeave for details.
		LambdaBaseHolder_Param3<void, HostID, HostID, int> OnP2PMemberLeave;
		/// See Proud.INetClientEvent.OnChangeP2PRelayState for details.
		LambdaBaseHolder_Param2<void, HostID, ErrorType > OnChangeP2PRelayState;
		/// See Proud.INetClientEvent.OnChangeServerUdpState for details.
		LambdaBaseHolder_Param1<void, ErrorType> OnChangeServerUdpState;
		/// See Proud.INetClientEvent.OnP2PMemberOffline for details.
		LambdaBaseHolder_Param1<void, CRemoteOfflineEventArgs &> OnP2PMemberOffline;
		/// See Proud.INetClientEvent.OnP2PMemberOnline for details.
		LambdaBaseHolder_Param1<void, CRemoteOnlineEventArgs &> OnP2PMemberOnline;
		/// See Proud.INetClientEvent.OnServerOffline for details.
		LambdaBaseHolder_Param1<void, CRemoteOfflineEventArgs &> OnServerOffline;
		/// See Proud.INetClientEvent.OnServerOnline for details.
		LambdaBaseHolder_Param1<void, CRemoteOnlineEventArgs &> OnServerOnline;
		/// See Proud.INetClientEvent.OnSynchronizeServerTime for details.
		LambdaBaseHolder_Param0<void> OnSynchronizeServerTime;


	public:
		/**
		\~korean
		Remote peer의 마지막 레이턴시를 얻는다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the last latency of Remote peer
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in second). Returns -1 if there is none.

		\~chinese
		获得remote peer的最后latency。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最後のレイテンシを得る。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeを秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		inline double GetLastUnreliablePingSec(HostID remoteHostID, ErrorType* error = NULL)
		{
			int ret=GetLastUnreliablePingMs(remoteHostID, error);
			if(ret<0)
				return (double)ret;
			else
				return (double)(ret/1000);
		}
		/**
		\~korean
		Remote peer의 마지막 레이턴시를 얻는다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the last latency of Remote peer
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in millisecond). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最后latency。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以毫秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最後のレイテンシを得る。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeをミリ秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		virtual int GetLastUnreliablePingMs(HostID remoteHostID, ErrorType* error = NULL) = 0;

		/**
		\~korean
		Remote peer의 마지막 레이턴시를 얻는다. GetLastUnreliablePing값은 Unreliable로 핑값을 구하는 반면 해당함수는 ReliableUdp로 측정한다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the last latency of Remote peer. GetLastUnreliablePing value is found by Unreliable but the relevant function is measured by ReliableUdp.
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in millisecond). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最后Latency。GetLastUnreliablePing 值用Unreliable求ping值，但相应函数用ReliableUdp测定。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以毫秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最後のレイテンシを得る。 GetLastUnreliablePing値はUnreliableでPING値を求める反面、該当関数は ReliableUdpで測定する。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeをミリ秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		virtual int GetLastReliablePingMs(HostID remoteHostID, ErrorType* error = NULL) = 0;

		/**
		\~korean
		Remote peer의 마지막 레이턴시를 얻는다. GetLastUnreliablePing값은 Unreliable로 핑값을 구하는 반면 해당함수는 ReliableUdp로 측정한다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the last latency of Remote peer. GetLastUnreliablePing value is found by Unreliable but the relevant function is measured by ReliableUdp.
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in second). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最后Latency。GetLastUnreliablePing 值用Unreliable求Ping值，但相应函数用ReliableUDP测定。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。输入<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%的Host ID的话获得P2P组内的所有成员的平均latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最後のレイテンシを得る。 GetLastUnreliablePing値はUnreliableでPING値を求める反面、該当関数は ReliableUdpで測定する。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeを秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		inline double GetLastReliablePingSec(HostID remoteHostID, ErrorType* error = NULL)
		{
			int ret=GetLastReliablePingMs(remoteHostID, error);
			if(ret<0)
				return (double)ret;
			else
				return double(ret)/1000;
		}

		/**
		\~korean
		Remote peer의 최근 레이턴시를 얻는다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 의 HostID를 넣으면 P2P 그룹 내
		모든 멤버들의 평균 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the recent latency of Remote peer
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server. Gets the average latency of all members of P2P group when set HostID of <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in second). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最近latency。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。输入<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%的Host ID的话获得P2P组内的所有成员的平均latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最近のレイテンシを得る。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。 <a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a> のHostIDを入力すると P2Pグループ内の全てのメンバーの平均レイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeを秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		inline double GetRecentUnreliablePingSec(HostID remoteHostID, ErrorType* error = NULL)
		{
			int ret = GetRecentUnreliablePingMs(remoteHostID, error);
			if(ret<0)
				return (double)ret;
			else
				return (double)(ret/1000);
		}

		/**
		\~korean
		Remote peer의 최근 레이턴시를 얻는다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the recent latency of Remote peer
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in millisecond). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最近latency。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以毫秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最近のレイテンシを得る。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeをミリ秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		virtual int GetRecentUnreliablePingMs(HostID remoteHostID, ErrorType* error = NULL) = 0;

		/**
		\~korean
		Remote peer 의 최근 레이턴시를 얻는다. GetRecentUnreliablePing 값은 Unreliable로 핑값을 구하는 반면 해당함수는 ReliableUdp 로 측정한다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the recent latency of Remote peer. GetRecentUnreliablePing value is found by Unreliable but the relevant function is measured by ReliableUdp.
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in millisecond). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最近Latency。GetRecentUnreliablePing 值用Unreliable求Ping值，但相应函数则用ReliableUDP测定。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以毫秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最近のレイテンシを得る。 GetRecentUnreliablePing値はUnreliableでPING値を求める反面、該当関数は ReliableUdpで測定する。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeをミリ秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~
		*/
		virtual int GetRecentReliablePingMs(HostID remoteHostID, ErrorType* error = NULL) = 0;

		/**
		\~korean
		Remote peer 의 최근 레이턴시를 얻는다. GetRecentUnreliablePing 값은 Unreliable로 핑값을 구하는 반면 해당함수는 ReliableUdp 로 측정한다.
		\param remoteHostID 찾으려는 remote peer의 HostID. HostID_Server를 지정하면 서버와의 레이턴시를 얻는다.
		\param error 에러 여부를 리턴한다. 정상적인 경우 ErrorType_Ok, Ping을 얻지 못한 경우 ErrorType_ValueNotExist가 저장된다.
		\return ping time을 초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the recent latency of Remote peer. GetRecentUnreliablePing value is found by Unreliable but the relevant function is measured by ReliableUdp.
		\param remoteHostID HostID of remote peer to find. Gets the latency with server when set HostID_Server.
		\param error It returns whether an error occurs or not. In case of normal case, ErrorType_Ok will be saved, however, ErrorType_ValueNotExist will be saved in case Ping is not received.
		\return returns ping time(in second). Returns -1 if there is none.

		\~chinese
		获得Remote peer的最近Latency。GetRecentUnreliablePing 值用Unreliable求Ping值，但相应函数则用ReliableUDP测定。
		\param remoteHostID 要找的remote peer的HostID。指定HostID_Server的话获得与服务器的latency。
		\param error 返回错误与否。如果正常则为ErrorType_Ok，如果没有获得Ping则会储存ErrorType_ValueNotExist。
		\return ping把time以秒单位返回。没有的话返回-1。

		\~japanese
		Remote peerの最近のレイテンシを得る。 GetRecentUnreliablePing値はUnreliableでPING値を求める反面、該当関数は ReliableUdpで測定する。
		\param remoteHostID 探そうとするremote peerのHostID. HostID_Serverを指定するとサーバとのレイテンシを得る。
		\param error エラーがあるかないかをリターンする。正常的な場合はErrorType_Ok, Pingを得なかった場合にはErrorType_ValueNotExistが保存される。
		\return ping timeを秒単位でリターンする。ただし、 ping timeが測定されていなければ-1をリターンする。

		\~

		*/
		inline double GetRecentReliablePingSec(HostID remoteHostID, ErrorType* error = NULL)
		{
			int ret = GetRecentReliablePingMs(remoteHostID, error);
			if(ret<0)
				return (double)ret;
			else
				return double(ret)/1000;
		}

		/** 사용자 지정 주기로 핑퐁을 보내어 Roundtrip 레이턴시를 구하는 측정을 시작합니다.
		최근 레이턴시 값들을 최대 CNetConfig::RountTripLatencyTestMaxCount만큼 가지고 있게 됩니다.
		이 함수를 호출하고 원하는 시간이 흐른 뒤, 다음 두가지 방법으로 Roundtrip 레이턴시를 얻을 수 있습니다.
		(1) GetRoundTripLatency를 호출하여 아웃파라미터로 Roundtrip 레이턴시 얻습니다.
		(2) StopRoundTripLatencyTest를 호출하여 측정을 끝낸뒤, GetRoundTripLatency를 호출하여 아웃파라미터로 Roundtrip 레이턴시를 얻습니다.
		구해진 Latency는 측정 시간 동안 오간 핑퐁들의 평균입니다.
		\param peerID RTT 테스트 타겟 피어의 HostID.
		\param parameter 테스트가 어떻게 이루어지게 할 것인지에 대한 설정값들을 이 구조체에 담아보낸다.
		\return 성공하면 ErrorType_Ok, 등록되지 않은 피어이면 ErrorType_InvalidHostID를 반환한다.*/
		virtual ErrorType StartRoundTripLatencyTest(HostID peerID, const StartRoundTripLatencyTestParameter& parameter) = 0;

		/** 사용자 지정 주기로 핑퐁을 보내어 Roundtrip 레이턴시를 구하는 측정을 끝낸다.
		최근 레이턴시 값들을 최대 CNetConfig::RountTripLatencyTestMaxCount만큼 가지고 있게 됩니다.
		StartRoundTripLatencyTest 함수를 호출하고 원하는 시간이 흐른 뒤, 다음 두가지 방법으로 Roundtrip 레이턴시를 얻을 수 있다.
		(1) GetRoundTripLatency를 호출하여 아웃파라미터로 Roundtrip 레이턴시 얻는다.
		(2) 이 함수를 호출하여 측정을 끝낸뒤, GetRoundTripLatency를 호출하여 아웃파라미터로 Roundtrip 레이턴시 얻는다.
		구해진 Latency는 측정 시간 동안 오간 핑퐁들의 평균이다.
		\param peerID RTT 테스트 타겟 피어의 HostID. HostID_Server를 넘기면 서버를 대상으로 한다.
		\return 성공하면 ErrorType_Ok, 등록되지 않은 피어이면 ErrorType_InvalidHostID를 반환한다. */
		virtual	ErrorType StopRoundTripLatencyTest(HostID peerID) = 0;

		/** StartRoundTripLatencyTest 함수를 호출하고 원하는 시간이 흐른 뒤, 다음 두가지 방법으로 Roundtrip 레이턴시를 얻을 수 있다.
		가지고 있는 최대 CNetConfig::RountTripLatencyTestMaxCount만큼의 레이턴시들로 측정 결과를 산출합니다.
		(1) 이 함수를 바로 호출하여 아웃파라미터로 Roundtrip 레이턴시 얻는다.
		(2) StopRoundTripLatencyTest 함수를 호출하여 측정을 끝낸뒤, 이 함수를 호출하여 아웃파라미터로 Roundtrip 레이턴시 얻는다.
		구해진 Latency는 측정 시간 동안 오간 핑퐁들의 평균이다.
		\param peerID RTT 테스트 타겟 피어의 HostID. HostID_Server를 넘기면 서버를 대상으로 한다.
		\param result 테스트 결과가 이 아웃파라미터에 담긴다.
		\return 성공하면 ErrorType_Ok, 등록되지 않은 피어이면 ErrorType_InvalidHostID를 반환한다. */
		virtual	ErrorType GetRoundTripLatency(HostID peerID, RoundTripLatencyTestResult& result) = 0;

		/**
		\~korean
		내부적으로 갖고 있는 UDP 소켓을 닫아버리고 서버 및 P2P 통신을 bypass 상태로 강제 전환한다.
		- <a target="_blank" href="http://guide.nettention.com/cpp_ko#use_alternative_p2p" >ProudNet을 쓰지 않고 ProudNet의 홀펀칭 정보를 토대로 별도 UDP 소켓으로 통신하기</a>  에서 사용된다.
		- CNetClient는 P2P 통신을 하고 있는 Peer의 갯수만큼의 UDP 소켓을 갖고 있으며 각각의 UDP 소켓은 서로 다른 Peer와의 통신을
		전담하고 있다. 이 메서드는 1개 Peer와의 통신만을 bypass 상태로 강제 전환한다.
		- 이 메서드 호출 후 상대방도 direct P2P를 쓸 수 없게 되므로 상대방도 본 메서드를 호출해야 할 수도 있다.
		- direct P2P가 이미 아닌 경우에도 이 메서드는 과거 홀펀칭되었던 정보를 채워준다.

		\param peerID 강제 bypass할 상대측 peer의 HostID
		\param outDirectP2PInfo 강제 bypass된 peer와 이 CNetClient가 P2P 통신을 하기 위해 CNetClient가 내부적으로 갖고 있던 홀펀칭 정보가 채워질 곳.
		\return 상대 피어가 존재하며 상대 피어와의 홀펀칭이 된 적이 있었거나 direct P2P를 유지하고 있었다면 true를 리턴한다.

		\~english
		Shuts UDP socket that is possessed internally then forcefully converts server and P2P communication to bypass state
		- Used in <a target="_blank" href="http://guide.nettention.com/cpp_en#use_alternative_p2p" >Communicating with a separated UDP socket based on ProudNet hole-punched information without using ProudNet.</a>
		- CNetClient has the same number of UDP sockets as the number of peers that are P2P communiating with and each of UDP sockets manages the communication with each peer.
		This methos forcefully converts 1 communication with peer to bypass state.
		- Since the opponent cannot use direct P2P after calling this method, the opponent may call this method as well.
		- Also fills previously hole-punched info when not direct P2P

		\param peerID HostId of opponent peer that to be forcefully bypassed
		\param outDirectP2PInfo where the hole-punching information that was possessed by CNetClient in order to perform P2P communication between this CNetClient and forcefully bypassed peer.
		\return Returns true either if hole-punched with opponent peer while opponent peer exists OR sustained direct P2P.

		\~chinese
		关闭内部的UDP socket，强制转换服务器及P2P通信以bypass状态。
		- 在<a target="_blank" href="http://guide.nettention.com/cpp_zh#use_alternative_p2p" >不使用ProudNet，只基于ProudNet的打洞情报用另外的UDPSocket进行通信</a>%使用。
		- CNetClient%拥有着相当于进行P2P通信peer个数的UDP socket，每个UDP socket负责与其他peer的通信。此方法以bypass状态只强制转换与一个peer的通信。
		- 呼叫此方法后对方也不能使用direct P2P，所以对方也可能要呼叫此方法。
		- 不是direct P2P的情况，此方法也填充过去被打洞的信息。

		\param peerID 要强制bypass的对方peer的Host ID。
		\param outDirectP2PInfo 强制bypass的peer和此 CNetClient%为了P2P通信， CNetClient%在内部拥有的打洞信息被填充的地方。
		\return 存在对方peer而且有过与对方peer的打洞或者维持direct P2P的话返回true。

		\~japanese
		内部的に持っているUDPソケットを閉じて、サーバーおよびP2P通信をバイパス状態に強制切り替えます。
		- \ref use_alternative_p2p で使われます。
		- CNetClientはP2P通信をしているPeerの数だけのUDPソケットを持っていて、各々のUDPソケットはお互い違うPeerとの通信を専担しています。このメソッドは1個のPeerとの通信のみをバイパス状態に強制切り替えます。
		- このメソッドを呼び出した後、相手もdirect P2Pを使えなくなりますので、相手もこのメソッドを呼び出すこともできます。
		- direct P2Pではない場合にもこのメソッドは過去にホールパンチングされた情報を満たしてくれます。
		
		\param peerID 強制にバイパスする相手のPeerのHostID		
		\param outDirectP2PInfo 強制にバイパスされたpeerとこのCNetClientがP2P通信を行うため、CNetClientが内部的に持っていたホールパンチング情報が満たされる所。\return 相手Peerが存在して相手Peerとのホールパンチングがされたことがあったとか、direct P2Pを維持していたらtrueをリターンします。 

		\~
		*/
		virtual bool InvalidateUdpSocket(HostID peerID, CDirectP2PInfo &outDirectP2PInfo ) = 0;

		/**
		\~korean
		InvalidateUdpSocket으로 제거된 UDP socket을 다시 만든다.
		그리고 해당 상대측 peer와의 UDP 홀펀칭을 재개한다.
		- <a target="_blank" href="http://guide.nettention.com/cpp_ko#use_alternative_p2p" >ProudNet을 쓰지 않고 ProudNet의 홀펀칭 정보를 토대로 별도 UDP 소켓으로 통신하기</a>  에서 사용된다.
		- 이 메서드를 호출한다고 해서 즉시 UDP socket이 만들어지지는 않는다. 새로 만들어진 UDP 소켓의 주소를 얻으려면
		잠시 후에 GetLocalUdpSocketAddr을 호출해서 얻어와야 한다.
		- 이 메서드 호출 후 상대방도 direct P2P를 쓸 수 있어야 하므로 상대방도 본 메서드를 호출해야 할 수도 있다.

		\param peerID 일전에 InvalidateUdpSocket이 시행되었던 피어의 ID.
		\return 성공적으로 완료시 true. 해당 상대측 peer가 없으면 false.

		\~english
		Re-creates UDP socket removed by InvalidateUdpSocket
		And re-starts UDP hole-punching with opponent peer
		- Used at <a target="_blank" href="http://guide.nettention.com/cpp_en#use_alternative_p2p" >Communicating with a separated UDP socket based on ProudNet hole-punched information without using ProudNet.</a>
		- Calling this method does not mean immediate UDP socket creation. To get the address of newly created UDP socket, must call GetLocalUdpSocketAddr and get the address after a while.
		- Since the opponent cannot use direct P2P after calling this method, the opponent may call this method as well.

		\param peerID ID of peer that InvalidateUdpSocket was executed previously.
		\return true if successfully completed. False if there is no opponent peer.

		\~chinese
		重新创建用InvalidateUdpSocket删除的UDP socket。
		然后重新开始与相关对方peer的UDP打洞。
		- 在<a target="_blank" href="http://guide.nettention.com/cpp_zh#use_alternative_p2p" >不使用ProudNet，只基于ProudNet的打洞情报用另外的UDPSocket进行通信</a>%使用。
		- 即使呼叫此方法，也不会立即创建UDP socket。想获得重新创建的UDP socket的地址的话，稍后呼叫GetLocalUdpSocketAddr后获取
		- 此方法呼叫后对方也要使用direct P2P，所以对方也有可能会呼叫此方法。

		\param peerID 之前InvalidateUdpSocket被实行的peer的ID。
		\return 正式完成的话true。没有相关对方peer的话false。

		\~japanese
		InvalidateUdpSocketで除去されたUDP socketを再び作ります。 
		そして該当相手のPeerとのUDPホールパンチングを再開します。
		- \ref use_alternative_p2p で使われます。
		- このメソッドを呼び出すといって直ちにUDP socketが作られることはありません。新たに作られたUDPソケットのアドレスを得るためには、しばらく後でGetLocalUdpSocketAddrを呼び出して得てくる必要があります。
		- このメソッドを呼び出した後、相手もdirect P2Pを使うことができなければならないので、相手もこのメソッドを呼び出すこともあります。
		\param peerID この前にInvalidateUdpSocketが施行されたPeerのID
		\return 成功的に完了時にtrue。該当相手側のPeerがなければfalse。

		\~
		*/
		virtual bool RestoreUdpSocket(HostID peerID) = 0;

		// 사용자는 이걸 쓰지 말것
		virtual void TEST_FallbackUdpToTcp(FallbackMethod mode) = 0;

		virtual void TEST_EnableVirtualSpeedHack(int64_t multipliedSpeed)=0;

		virtual void TEST_SetPacketTruncatePercent(Proud::HostType hostType, int percent) = 0;

		/**
		\~korean
		P2P간의 Reliable 메시징 시스템의 작동 통계를 얻는다.
		- 성능 측정이나 디버깅 등을 위해 사용된다.

		\~english
		Gets operation statistics of reliable messaging system among P2P
		- Used for performance test or debugging

		\~chinese
		获得P2P之间的Reliable messaging系统的运转统计。
		- 为性能测定或者调试等而使用。

		\~japanese
		P2P間のReliableメッセージングシステムの作動統計を得ます。
		- 性能測定やデバッギングなどのために使用されます。 

		\~
		*/
		virtual bool GetPeerReliableUdpStats(HostID peerID,ReliableUdpHostStats &output) = 0;

		/**
		\~korean
		이 클라이언트가 공유기 뒤에 있는 클라이언트인가?
		\return 판단할 수 없으면 false 리턴. 이때 output은 안채워진다.
		\param output 공유기 뒤에 있는 클라이언트이면 true가 채워진다.

		\~english
		Is this client behind a router?
		\return returns false if cannot be verified. The output will not be filled at this time.
		\param output Fills true if the client is behind a router

		\~chinese
		此client是路由器后面的client吗？
		\return 不能判断的话返回false。这时候不填充output。
		\param output 路由器后面的client的话填充true。

		\~japanese
		このクライアントがルーターの後ろにあるクライアントなのか？
		\return 判断できなければfalseをリターン。この時、outputは満たされません。 
		\param output ルーターの後ろにあるクライアントであればtrueが満たされます。 

		\~
		*/
		virtual bool IsLocalHostBehindNat(bool &output) = 0;

		/**
		\~korean
		엔진 프로토콜 버전을 얻는다. 이 값이 클라이언트에서 얻는 엔진 프로토콜 버전과 다르면 접속이 불허된다.

		\~english
		Gets the version of engine protocol. If this value differes to engine protocol version from client then the connection will not be accepted.

		\~chinese
		获得引擎协议版本。此值与从client获得的引擎协议版本不同的话不允许连接。

		\~japanese
		エンジンプロトコルバージョンを得ます。この値がクライアントより得るエンジンプロトコルバージョンと違って接続が不許されます。
		\~
		*/
		virtual int GetInternalVersion() = 0;

		/**
		\~korean
		주고받을 수 있는 RMI 혹은 사용자 정의 메시지의 최대 크기다.

		\~english
		The maximum size of RMI or user defined message

		\~chinese
		能传接的RMI或者用户定义的信息的最大大小。

		\~japanese
		やり取りできるRMIまたはユーザー定義メッセージの最大サイズです。
		\~
		*/
		virtual int GetMessageMaxLength() = 0;

		/**
		\~korean
		서버에서 인식된, 이 클라이언트의 네트워크 주소를 얻는다. 즉, 즉 공인 인터넷 주소(external addr)이다.
		- 서버와 연결되어 있을때만 유효한 값을 리턴한다.

		\~english
		Gets the network address of this client that is recognized by server. In other words, this is a public Internet address(external address).
		- Returns effective values only when connected to server

		\~chinese
		获得从服务器识别的，此client的网络地址。即公认互联网地址（external addr）。
		- 只与服务器连接的时候返回有效值。

		\~japanese
		サーバーより認識されたこのクライアントのネットワークアドレスを得ます。即ち、パブリックインターネットアドレス(external addr)です。
		- サーバーと接続されている時のみ有効な値をリターンします。

		\~
		*/
		virtual AddrPort GetPublicAddress() = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#thread_pool" >스레드 풀(Thread pool)</a> 의 각 스레드의 정보를 얻습니다.
		\param output 여기에 정보가 채워집니다.

		\~english
		Gets the information of each thread of <a target="_blank" href="http://guide.nettention.com/cpp_en#thread_pool" >Thread pool</a>
		\param output this is where the information has filled in.

		\~chinese
		获得<a target="_blank" href="http://guide.nettention.com/cpp_zh#thread_pool" >线程池(Thread pool)</a>%的各线程信息。
		\param output 往这里填充信息。

		\~japanese
		\ref thread_poolの各スレッドの情報を得ます。
		\param output ここに情報が満たされます。

		\~
		*/
		virtual void GetUserWorkerThreadInfo(CFastArray<CThreadInfo> &output) = 0;

		/**
		\~korean
		네트워킹 스레드의 정보를 얻습니다.
		ProudNet은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#thread_pool" >스레드 풀(Thread pool)</a> 과 별개로 네트워크 I/O처리를 담당하는 스레드가 내장되어 있습니다. 이것의 정보를 얻습니다.
		\param output 여기에 정보가 채워집니다.

		\~english
		Gets the information of networking thread
		ProudNet has a thread that manages network I/O that is independent to <a target="_blank" href="http://guide.nettention.com/cpp_en#thread_pool" >Thread pool</a>. This gets the information of the thread.
		\param output This is where the information is filled.

		\~chinese
		获取网络线程信息。
		ProudNet是与<a target="_blank" href="http://guide.nettention.com/cpp_zh#thread_pool" >线程池(Thread pool)</a>不同，内置担任网络I/O处理的线程。获得它的信息。
		\param output 往这里填充信息。

		\~japanese
		ネットワーキングスレッドの情報を得ます。
		ProudNetは\ref thread_poolとは別にネットワークI/O処理を担当するスレッドが内臓されています。これの情報を得ます。
		\param output ここに情報が満たされます。 

		\~
		*/
		virtual void GetNetWorkerThreadInfo(CFastArray<CThreadInfo> &output) = 0;

		/**
		\~korean
		ProudNet에 의해 만들어진 socket handle 들을 얻습니다. 이 기능은 클라이언트 보안 소프트웨어(가령 nProtect나 Ahnlab Hack Shield 등)에서 필요로 할 경우 유용합니다.
		이 메서드를 통해 얻은 socket handle은 정보 획득 용도로만 사용하시고, I/O 처리를 수행하는 것은 금지합니다. 부작용이 있을 수 있기 때문입니다.

		\param remoteHostID 소켓 정보를 얻을 peer의 HostID입니다. HostID_Server일 경우 서버와 연결된 소켓 정보를 얻습니다.
		\param output 여기에 정보가 채워집니다.
		\return remoteHostID 값이 유효한 경우 true입니다.

		\~english
		Gets socket handles that made by ProudNet. This function is useful when it need at security software(e.g. nProtect or Ahnlab Hack Shield)
		Please use socket handle from this method for getting information. Do not excute I/O process. Because it may occur bed reaction.

		\param remoteHostID HostID of peer that getting socket information. If it is HostID_Server then it gets socket information from connected server.
		\param output Information fill in here
		\return remoteHostID It is true, if value is available.

		\~chinese
		获得被ProudNet创建的socket handle。此功能在client安全软件（例如nProtect或者Ahnlab Hack等）需要的时候有用。
		通过此方法获得的socket handle只适用于获得信息用途，禁止进行I/O 处理。因为可能有副作用。

		\param remoteHostID 获得socket的peer的Host ID。如果是HostID_Server的话获得与服务器连接的socket信息。
		\param output 往这里填充信息。
		\return remoteHostID值有效的话true。

		\~japanese
		ProudNetによって作られたsocket handleを得ます。この機能はクライアントセキュリティソフトウェア(仮に、nProtectとかAhnlab Hack Shieldなど)で必要とする場合に有用です。
		このメソッドを通じ得たsocket handleは情報獲得用途でのみ使って、I/O処理を遂行することは禁止です。副作用があり得るからです。
		\param remoteHostID ソケット情報を得るPeerのHostIDです。HostID_Serverの場合、サーバーに接続されたソケット情報を得ます。
		\param output ここに情報が満たされます。
		\return remoteHostID 値が有効な場合、trueです。 

		\~
		*/
		virtual bool GetSocketInfo(HostID remoteHostID,CSocketInfo& output) = 0;

		/**
		\~korean
		CNetClient을 사용하는 응용프로그램 상태를 CNetClient에게 전달합니다.
		CNetClient를 렌더링 루프와 다른 스레드에서 실행시키는 게임의 경우 프레임 레이트를 CNetClient가 직접 산출할 수 없어서 사용자가 굳이 측정해야 합니다.
		- 아무때나 호출해도 성능에 영향을 거의 안줍니다.
		- 이 메서드는 다양한 기능, 가령 <a target="_blank" href="http://guide.nettention.com/cpp_ko#super_peer" >Super Peer(수퍼피어 혹은 호스트) 중심의 P2P 네트워킹</a> 기능 등에서 사용합니다.

		\~english
		Passes the status of application program that uses CNetClient to CNetClient
		User must measure the frame rate of the game that runs CNetClient in diffrent thread to rendering loop since it cannot be acquired directly by CNetClient.

		- Does not affect performance even called at any time
		- Various functions of this method can be used, e.g. <a target="_blank" href="http://guide.nettention.com/cpp_en#super_peer" >P2P Networking for Super Peer (Super Peer or host)</a> function.

		\~chinese
		给 CNetClient%传达使用 CNetClient%的应用程序状态。
		在渲染loop和其他线程实行的游戏的时候， CNetClient%不能直接算出帧率，因此用户一定要进行测定。
		- 无论什么时候呼叫，对于性能几乎不产生影响。
		- 此方法是在各种功能，例如在<a target="_blank" href="http://guide.nettention.com/cpp_zh#super_peer" >Super Peer(或主机) 中心的P2P通信</a>%功能等里使用。
		\~
		\~japanese
		CNetClientを使用するアプリケーションの状態をCNetClientに伝達します。
		CNetClientをレンダーリングループと他のスレッドで実行させるゲームの場合、フレームレートをCNetClientが直接算出できず、ユーザーがあえて測定しなければなりません。 
		- いつ呼び出しても性能にほぼ影響を与えません。
		- このメソッドは多様な機能、仮に\ref super_peer 機能などで使用します。 
\~
		*/
		virtual void SetApplicationHint(const CApplicationHint &hint) = 0;

		/**
		\~korean
		사용자 정의 메시지를 전송합니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#send_user_message" >RMI를 전혀 안쓰고 사용자 정의 메시지를 주고 받기</a> 를 참고하십시오.
		\param remotes 수신자 배열입니다. RMI와 마찬가지로, 클라이언트, 서버(HostID_Server), <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 등을 넣을 수 있습니다.
		\param remoteCount 수신자 배열 크기
		\param rmiContext 송신 부가정보. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#protocol_overview" >ProudNet의 프로토콜 종류</a> 를 참고하십시오.
		\param payload 보낼 메시지 데이터
		\param payloadLength 보낼 메시지 데이터의 길이. 바이트 단위입니다.

		\~english
		Send user defined message. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#send_user_message" >Exchanging user-defined message without using RMI at all</a> for more detail
		\param remotes Receiver array. You can put client, server(HostID_Server), <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>, etc like RMI.
		\param remoteCount Size of receiver array
		\param rmiContext Additional information of sending. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#protocol_overview" >Protocol type of ProudNet</a>
		\param payload Message data for sending.
		\param payloadLength Length of sending message data. It is byte.

		\~chinese
		传送用户定义信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#send_user_message" >完全不使用RMI交换用户定义信息</a>%。
		\param remotes 收信者数组。与RMI一样，可以输入client，服务器（HostID_Server），<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%等。
		\param remoteCount 收信者数组大小。
		\param rmiContext 传送信息附加信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#protocol_overview" >ProudNet 的网络协议种类</a>%。
		\param payload 要发送的信息数据。
		\param payloadLength 要发送的信息数据长度。Byte 单位。
		\~
		\~japanese
		ユーザー定義メッセージを転送します。詳しくは、\ref send_user_messageをご参照ください。
		\param remotes 受信者配列です。RMIと同様に、クライアント、サーバー(HostID_Server)、\ref p2p_groupなどを入れることができます。
		\param remoteCount 受信者配列サイズ
		\param rmiContext 送信付加情報。詳しくは、\ref protocol_overviewをご参照ください。
		\param payload 送るメッセージデータ
		\param payloadLength 送るメッセージデータの長さ。バイト単位です。
\~
		*/
		virtual bool SendUserMessage(const HostID* remotes, int remoteCount, RmiContext &rmiContext, uint8_t* payload, int payloadLength) = 0;

		/**
		\~korean
		사용자 정의 메시지를 전송합니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#send_user_message" >RMI를 전혀 안쓰고 사용자 정의 메시지를 주고 받기</a> 를 참고하십시오.
		\param remote 수신자
		\param rmiContext 송신 부가정보. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#protocol_overview" >ProudNet의 프로토콜 종류</a> 를 참고하십시오.
		\param payload 보낼 메시지 데이터
		\param payloadLength 보낼 메시지 데이터의 길이. 바이트 단위입니다.

		\~english
		Sending user defined message. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#send_user_message" >Exchanging user-defined message without using RMI at all</a> for more detail.
		\param remote receiver
		\param rmiContext Additional information of sending. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#protocol_overview" >Protocol type of ProudNet</a>.
		\param payload Message data for sending.
		\param payloadLength Length of sending message data. It is byte.

		\~chinese
		传送用户定义信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#send_user_message" >完全不使用RMI交换用户定义信息</a>%。
		\param remote 收信者
		\param rmiContext 传送信息附加信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#protocol_overview" >ProudNet 的网络协议种类</a>%。
		\param payload 要发送的信息数据。
		\param payloadLength 要发送的信息数据长度。Byte 单位。

		\~japanese
		ユーザー定義メッセージを転送します。詳しくは、\ref send_user_messageをご参照ください。
		\param remote 受信者
		\param rmiContext 送信付加情報。詳しくは、\ref protocol_overviewをご参照ください。
		\param payload 送るメッセージデータ
		\param payloadLength 送るメッセージデータの長さ。バイト単位です。

		\~
		*/
		inline bool SendUserMessage(HostID remote, RmiContext &rmiContext, uint8_t* payload, int payloadLength)
		{
			return SendUserMessage(&remote,1,rmiContext,payload,payloadLength);
		}

		/** \~korean Coalesce interval을 수동으로 설정합니다.
		ProudNet 자체에서 설정되는 네트워크 품질 기반 데이터 뭉쳐 보내는 기능이 원하지 않는 품질이 나올때 사용하십시오.
		적절하지 않은 값을 입력할 경우 의도하지 않은 결과가 초래될 수 있으니 주의하십시오.
		\~english Manually set coalesce interval.
		Use this function if you do not satisfy to the quality of coalesce feature of ProudNet.
		Inapprpriate use of this function may cause unexpected behavior, so care must be taken.

		\~japanese
		Coalesce intervalを手動で設定します。
		ProudNet 自体で設定されるネットワーク品質基板のデータを送る機能が望まない品質になる時に使ってください。
		適切ではない値を入力する場合、意図しない結果が発生することがありますのでご注意ください。

		\~
		*/
		virtual ErrorType SetCoalesceIntervalMs(HostID remote, int intervalMs) = 0;

		/** \~korean Coalesce interval을 자동으로 설정합니다. 이 설정은 디폴트입니다.
		\~english Coalesce interval set to automatic. This is default.

		\~japanese
		Coalesce intervalを自動設定します。この設定はデフォルトです。
		\~
		*/
		virtual ErrorType SetCoalesceIntervalToAuto(HostID remote) = 0;

/**
		\~korean
		긴급 로그를 긴급 로그 수집 서버(CEmergencyLogServer)로 전송합니다. CEmergencyLogServer를 먼저 실행하셔야 해당기능을 쓸수 있습니다.
		- CStartServerParameter.m_clientEmergencyLogMaxLineCount의 갯수를 지정하셔야 합니다.
		- 의도되지 않게 서버와의 연결이 끊긴 경우, 해당함수를 호출하면 클라의 최근 상황과 현재 상태로그를 CEmergencyLogServer로 전송합니다.
		- logClient는 내부에서 새로운 thread를 생성하여 로그 클라이언트를 만들고 바로 서버로 로그가 전송됩니다. 따라서 이 메서드 call 후 바로 메인 스레드를 종료하지 말아야 합니다.
		\param serverAddr 로그서버 주소입니다.
		\param serverPort 로그서버의 포트번호입니다.
		\return 성공, 실패 여부입니다. m_clientEmergencyLogMaxLineCount 값이 0인 경우가 이에해당됩니다.

		\~english TODO:translate needed.
		The emergency log is transmitted to the emergency log collection server (CEmergencyLogServer). You have to run CEmergencyLogServer first in order to use its feature. 	
		- You have to designate the number of CStartServerParameter.m_clientEmergencyLogMaxLineCount. 
		- When the connection to the server is lost unintentionally, the current status log as well as the latest status of the client is transmitted to CEmergencyLogServer. 
		- logClient generates a new thread inside and create a log client and the log is directly transmitted to the server. Therefore, you must not end the main thread after calling this method. 
		\param serverAddr This is the address of the log server. 
		\param serverPort This is the port number of the log server. 
		\return This is either success or failure. The case when the value of m_clientEmergencyLogMaxLineCount is 0 falls under this case. 


		\~chinese
		把紧急log传送到紧急log收集服务器（CEmergencyLogServer）。要先实行 CEmergencyLogServer%才能使用此功能。
		- 要指定 CStartServerParameter.m_clientEmergencyLogMaxLineCount%的个数。
		- 非意图的断开连接的时候，呼叫相关函数的话，往 CEmergencyLogServer%传送client的最近状态和现在状态log。
		- logClient在内部生成新thread并创建log client，立即往服务器传送log。因此在呼叫此方法后，请不要立即终止主线程。
		\param serverAddr log服务器地址。
		\param serverPort log服务器端口号码。
		\return 成功与否。 m_clientEmergencyLogMaxLineCount 是0的情况属于此。

		\~japanese
		緊急ログを緊急ログ収集サーバー(CEmergencyLogServer)に転送します。CEmergencyLogServerを先に実行してから該当機能を使うことができます。		
		- CStartServerParameter.m_clientEmergencyLogMaxLineCountの数を指定しなければなりません。
		- 意図せずにサーバーとの接続が切れた場合、該当関数を呼び出すとクライアントの最近状況と現在状態ログをCEmergencyLogServerに転送します。
		- logClientは内部で新しいスレッドを生成してログクライアントを作ってすぐサーバーにログが転送されます。よって、このメソッドを呼び出した後、すぐメインスレッドを終了してはいけません。
		\param serverAddr ログサーバーアドレスです。
		\param serverPort ログサーバーのポート番号です。
		\return 成功、失敗可否です。m_clientEmergencyLogMaxLineCount 値が0である場合がこれに該当します。

		\~
		*/
		#if defined(_WIN32)
		virtual bool SendEmergencyLogData(String serverAddr, uint16_t serverPort) = 0;
		#endif

		virtual AddrPort GetTcpLocalAddr() = 0;
		virtual AddrPort GetUdpLocalAddr() = 0;

		virtual String TEST_GetDebugText() = 0;
		virtual void TEST_GetTestStats(CTestStats2 &output) = 0;

		// ACR test용
		virtual void TEST_FakeTurnOffSendAndReceive() = 0;
		virtual void TEST_SetAutoConnectionRecoverySimulatedDelay(int timeMs) = 0;

#ifdef TEST_DISCONNECT_CLEANUP
		virtual bool IsAllCleanup() = 0;
#endif
		/**
		\~korean
		사용자가 지정한 다른 P2P peer와의 통신을 강제로 relay로 되게 할지를 지정하는 함수입니다.

		이 기능이 요긴하게 쓰이는 경우는 다음과 같습니다.
		- 클라이언트측의 P2P 통신량이 과다해서, 몇몇 P2P peer와의 통신은 relay로 전환하고자 할 경우
		\param remotePeerID Relay 전환시킬 Peer 의 HostID값입니다.
		\param enable true이면 강제 relay 를 켭니다.

		\~english TODO:translate needed.
		This is the function to designate whether the communication with other P2P peers designated by the user should be forced to relay. The following are the cases where this feature comes handy. 
		- When the P2P traffic from the client side is excessive so you want to convert some communication with some P2P peers to relay;
		\param remotePeerID The HostID value of the peer to be converted to relay
		\param enable If true is enabled, forced relay is turned on. 

		\~chinese
		是指定是否将用户指定的与其他P2P Peer间的通信强制进行relay的函数。
		能够有效使用其功能的状况如下。
		客户端的P2P通信量过多时，想将几个P2P Peer间的通信转换成relay的时候。
		\param remotePeerID Relay要转换的Peer的HostID值。
		\param enable 如果是true则开启强制relay。

		\~japanese
		ユーザーが指定した他のP2P Peerとの通信を強制にrelayするかを指定する関数です。この機能が有効に使われる場合は次の通りです。
		- クライアント側のP2P通信料が多過ぎて、いくつかのP2P Peerとの通信はrelayに切り替えたい場合、
		\param remotePeerID Relay 切り替えるPeerのHostID値です。
		\param enable trueであれば強制relayをつけます。

		\~
		*/
		virtual ErrorType ForceP2PRelay(HostID remotePeerID, bool enable) = 0;

		/**
		\~korean
		일반적인 온라인 게임에서는 unreliable 메시징이 전체 트래픽의 대부분을 차이지함에 따라 기존 unreliable 메시징 기능에 트래픽 로스율을 얻는 기능을 추가합니다.
		패킷 로스율 측정기
		\param remoteHostID 어느 remote 와의 통신에 대한 로스율을 얻을 걷인지. 자기 자신 peer, server 뭐든지 입력 가능합니다. 자기 자신이면 0% 입니다
		\param outputPercent 패킷 로스율을 %단위로 채워짐(즉 0~100)

		\~english TODO:translate needed.
		As unreliable messaging takes most of the entire traffic in common on-line games, we add the feature to get the traffic loss rate in the existing unreliable messaging feature. 
		Packet loss rate measuring device
		\param remoteHostID Which loss rate with communication with which remote would be obtained. You can input anything, including your own peer, server, etc. If it is itself, it is 0%
		\param outputPercent The packet loss rate is filled up in % unit (i.e., 0 to 100) 

		\~chinese
		在一般在线游戏中当Unreliable信息的总体通信量占据绝大部分时，在原有Unreliable信息功能上添加获得通信量Loss率的功能。
		数据包Loss率测定仪。
		\param remoteHostID 要获得对哪一个remote间的通信Loss率，可以输入如自身Peer，server 等。如果是自己的话则为0%。
		\param outputPercent  数据包的Loss率将以%单位填充，（即0~100）
		
		\~japanese
		一般的にオンラインゲームではunreliableメッセージングが全体トラフィックのほとんどを占めますので、既存のunrelableメッセージング機能にトラフィックロス率を得る機能を追加します。
		パケットロス率測定機
		\param remoteHostID どのremoteとの通信に対するロス率を得るか。自己自身のpeer、server何でも入力可能です。自己自身であれば0%です。
		\param outputPercent パケットロス率が%単位で満たされます(即ち、0~100)

		\~
		*/
		virtual ErrorType GetUnreliableMessagingLossRatioPercent(HostID remotePeerID, int *outputPercent) = 0;

		/**
		\~korean
		CNetClient.FrameMove 를 호출하면 누적되어 있던 모든 이벤트와 수신 RMI가 일제히 호출됩니다.
		ProudNet은 사용자가 원하는 경우 누적되어 있던 이벤트와 수신 RMI를 1회의 CNetClient.FrameMove 호출 구간 내에서 모두 처리하다 말고 중간에 리턴하는 기능이 있습니다. 그리고 남아있는 누적 이벤트와 수신 RMI를 다음 CNetClient.FrameMove 호출에서 마저 처리할 수 있도록 해줍니다.
		남아있는 누적 이벤트와 수신 RMI를 중도에 끊으려면 RMI 수신 처리중인 루틴 또는 이벤트 수신처리중인 루틴에서 Proud.NetClient.HolsterMoreCallbackUntilNextFrameMove를 호출하면 됩니다.

		\code
		DEFRMI_TestS2C_Foo(CMyClient)
		{
			...
			// 이것을 호출하면 CNetClient.FrameMove는 남아있는 누적 이벤트와 RMI 수신을 무시하고 즉시 리턴합니다.
			HolsterMoreCallbackUntilNextFrameMove();
		}

		void CMyClient::OnClientLeave(...)
		{
			...
			// 이것을 호출하면 CNetClient.FrameMove는 남아있는 누적 이벤트와 RMI 수신을 무시하고 즉시 리턴합니다.
			HolsterMoreCallbackUntilNextFrameMove();
		}
		\endcode

		\~english
		When CNetClient.FrameMove is called, all accumulated events and reception RMIs get called at the same time.
		ProudNet allows its user to abort the operation of accumulated events and reception RMIs being managed within one time called CNetClient.FrameMove and return in the middle.And the remaining events and reception RMIs can be managed by CNetClient.FrameMove call.
		When you even wish to stop the operation of remaining events or reception RMIs, you can call either Proud.INetClientEvent.HolsterMoreCallbackUntilNextFrameMove from the routine managing RMI reception or event reception.

		\code
		DEFRMI_TestS2C_Foo(CMyClient)
		{
			...
			// CNetClient.FrameMove ignores the remaining accumulated events and reception RMIs then returns immediately.
			HolsterMoreCallbackUntilNextFrameMove();
		}

		void CMyClient::OnClientLeave(...)
		{
			...
			// When you call this, CNetClient.FrameMove ignores the remaining accumulated events and reception RMIs then returns immediately
			HolsterMoreCallbackUntilNextFrameMove();
		}
		\endcode

		\~chinese
		呼出 CNetClient.FrameMove ，则累积的所有Event和收信RMI将会一同呼出。如果用户愿意，在Proudnet具有可以在累积的Event和收信RMI在一次 CNetClient.FrameMove%呼出区间内终止处理，并在中途进行Return的功能。剩余的累积Event和收信RMI在下一次的 CNetClient.FrameMove%呼出当中进行处理。
		如果要中途终止剩余累积Event和收信RMI的处理的话可以在RMI收信处理中的Routine或Event收信处理中的Routine当中呼出 Proud.INetClientEvent.HolsterMoreCallbackUntilNextFrameMove。

		\code
		DEFRMI_TestS2C_Foo(CMyClient)
		{
			...
			// 如果呼出这个， CNetClient.FrameMove%将无视剩余的累积Event和RMI收信并立即Return。
			HolsterMoreCallbackUntilNextFrameMove();
		}

		void CMyClient::OnClientLeave(...)
		{
			...
			// 如果呼出这个， CNetClient.FrameMove%将无视剩余的累积Event和RMI收信并立即Return。
			HolsterMoreCallbackUntilNextFrameMove();
		}
		\endcode
		
		\~japanese
		CNetClient.FrameMoveを呼べば積もっていた全てのイベントと受信RMIが一切にきます。
		ProudNetはユーザーが望む場合積もっていた全てのイベントと受信RMIを一つのCNetClient.FrameMoveルーチンの中で処理をやめ途中にリターンする機能があります。そして残されてた全てのイベントと受信RMIを次のCNetClient.FrameMoveで処理できるようにします。
		残っていたイベントと受信RMIを途中で切るためにはRMI受信処理やイベント受信処理中のルーチンの中でProud.INetClientEvent.HolsterMoreCallbackUntilNextFrameMoveをよべばいいです。

		\code
		DEFRMI_TestS2C_Foo(CMyClient)
		{
			...
			// これを呼べばCNetClient.FrameMoveは残ってる全てのイベントとRMI수신を無視して即刻リターンします。
			HolsterMoreCallbackUntilNextFrameMove();
		}

		void CMyClient::OnClientLeave(...)
		{
			...
			// これを呼べばCNetClient.FrameMoveは残ってる全てのイベントとRMI수신を無視して即刻リターンします。
			HolsterMoreCallbackUntilNextFrameMove();
		}
		\endcode

		\~
		*/
		virtual void HolsterMoreCallbackUntilNextFrameMove() = 0;
	};

	 PROUD_API const PNTCHAR* ToString(ConnectionState cs);

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

	/**  @} */

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
