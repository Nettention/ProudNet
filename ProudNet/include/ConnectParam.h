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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class CThreadPool;

	/** \addtogroup net_group
	*  @{
	*/

	/**
	\~korean
	서버에 연결하기 위한 정보 구조체

	\~english
	Informtion structure of making server connection

	\~chinese
	为了连接服务器的信息构造体。

	\~japanese
	サーバーに接続するための情報構造体
	\~
	 */
	class CNetConnectionParam
	{
	public:
		/**
		\~korean
		연결할 서버의 주소
		- 예: 111.222.111.222(IP 주소식) 혹은 game.mydomain.net(호스트 이름식)
		- 스트레스 테스트를 하는 경우에 한꺼번에 많은 클라이언트가 서버에 접속해야 합니다. 이러한 경우
		서버 주소가 호스트 이름식인 경우 접속 속도가 매우 느릴 수 있습니다. 따라서 스트레스 테스트를 할 경우
		서버 주소로 IP 주소식을 쓰는 것을 권장합니다.

		\~english
		Address of server to coneec to
		- ex: 111.222.111.222(IP address) or game.mydomain.net(host name)
		- During a stress test, lots of clients need to be connected to the server at the same time.
		If the server address is a type of a host name, it can be seriously laggy.
		It is strongly recommended to use IP address format for server address.

		\~chinese
		要连接的服务器地址。
		- 例：111.222.111.222（IP 地址式）或 game.mydomain.net（主机名式）。
		- 做压力测试的情况一次要连接多个客户端。这种情况服务器地址是主机名式的话，连接速度可能会很慢。因此做压力测试的时候建议把IP地址式用为服务器地址。

		\~japanese
		接続するサーバーのアドレス
		- 例: 111.222.111.222(IPアドレス形式)または game.mydomain.net(ホスト名形式)
		- ストレステストをする場合に一度に多いクライアントがサーバーに接続しなければなりません。このような場合
		サーバーアドレスがホスト名形式である場合、接続速度が非常に遅い場合があります。よって、ストレステストをする場合、サーバーアドレスとしてIPアドレス形式を使うことをお勧めします。
		\~
		 */
		String m_serverIP;

		/**
		\~korean
		연결할 서버의 TCP 포트입니다.

		\~english
		The TCP port of the server to be connected.

		\~chinese
		要连接的服务器TCP端口。

		\~japanese
		接続するサーバーのTCPポートです。
		\~
		 */
		uint16_t m_serverPort;

		///**
		//\~korean
		//여러분이, 서버 주소로 FQDN(예: game1.mygame.com)을 사용할 수 없는 상황이지만, IPv6 클라이언트에서 IPv4 서버로 접속을 해야 하는 상황이신가요? 이 때를 위한 응급 기능입니다.
		//서버 주소로 IPv4 literal (예: 11.22.33.44)를 넣으시고, 이 두 멤버 변수에다가 IPv4 서버 주소의 FQDN을 넣으십시오. 다음은 예입니다.
		//serverIP: 11.22.33.44
		//publicDomainName1: www.naver.com
		//publicDomainName2: www.nts.go.kr
		//단, 위와 같은 상황이 아니면 본 기능을 사용하지 않는 것을 권장합니다. 대신 서버 주소로 FQDN을 사용하십시오.

		//자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#apple_ipv6_workaround" >Apple의 IPv6 정책 우회책</a> 를 참고하십시오.

		//\~english
		//It is the variable that enables connection from IPv6 network client even if connects to server by IPv4 literal string.
		//For more details, please refer to tips for resolving <a target="_blank" href="http://guide.nettention.com/cpp_en#apple_ipv6_workaround" >Apple's IPv6 enforcement.</a>

		//\~chinese
		//It is the variable that enables connection from IPv6 network client even if connects to server by IPv4 literal string.
		//For more details, please refer to tips for resolving <a target="_blank" href="http://guide.nettention.com/cpp_zh#apple_ipv6_workaround" >Apple's IPv6 enforcement.</a>

		//\~japanese
		//It is the variable that enables connection from IPv6 network client even if connects to server by IPv4 literal string.
		//For more details, please refer to tips for resolving <a target="_blank" href="http://guide.nettention.com/cpp_jp#apple_ipv6_workaround" >Apple's IPv6 enforcement.</a>
		//*/
		//String m_publicDomainName1;

		///** \copydoc m_publicDomainName1 */
		//String m_publicDomainName2;

 		/**
		\~korean
		클라이언트에서 생성하는 UDP socket이 사용할 local port 번호들입니다.

		이 모듈은 서버나 peer와의 연결 각각을 위해 1개의 UDP port를 사용합니다. 이때 m_localUdpPortPool 에서 지정한
		포트 번호들을 local port로 삼는 UDP socket들을 생성하게 됩니다. 만약 m_localUdpPortPool에서 시정한
		포트 번호가 없거나 m_localUdpPortPool에서 지정한 포트 번호들이 모두 다른데서 사용중인 경우 임의의
		포트 번호가 지정되어 UDP socket이 생성됩니다.

		일반적으로는 m_localUdpPortPool 값은 그냥 두시는 것이 좋습니다. 하지만 의도적으로 local UDP socket의
		port 번호를 강제로 지정하고자 할 때 m_localUdpPortPool 안에 원하시는 값들을 넣으십시오.

		\~english
		Numbers of UDP ports for newly created UDP sockets.

		This host module uses an UDP port for each server or peer connection. Values in m_localUdpPortPool are
		used for binding local UDP port to every UDP socket created by this module. Arbitrary UDP port number
		will be taken if m_localUdpPortPool is empty or no available UDP port corresponding to m_localUdpPortPool
		exists.

		In ordinary case, m_localUdpPortPool should be left unchanged. You should add values into m_localUdpPortPool
		if you want to bind some local UDP ports to UDP sockets created by this module.

		\~chinese
		在客户端生成的UDP socket要使用的local port号码。
		此模块为了与服务器或者peer的连接，各使用一个UDP port。这时会生成在m_localUdpPortPool指定的端口号码为local port的UDP socket。
		如果没有在m_localUdpPortPool指定的端口号码，或者在m_localUdpPortPool指定的端口号码都在别的地方使用中的时候，任意端口号码会被指定并生成UDP socket。
		一般的情况下，最好不要动m_localUdpPortPool值。但是想强制指定local UDP socket的port的时候，往m_localUdpPortPool里放所需的值。

		\~japanese
		クライアントで生成するUDP socketが使用するLocal port番号です。
		このモジュールはサーバーとかpeerとの連結のためにそれぞれ1個のUDP portを使用します。この時、m_localUdpPortPoolで指定したポート番号をLocal portとするUDP socketを生成するようになります。もし、m_localUdpPortPoolで是正したポート番号がないとか、m_localUdpPortPoolで指定したポート番号が全て違う所で使用中の場合、任意のポート番号が指定されUDP socketが生成されます。
		一般的にm_localUdpPortPool値はそのまま置いた方が良いです。しかし、意図的にlocal UDP socketのport番号を強制に指定したい場合、m_localUdpPortPool内に希望する値を入れてください。
		\~
		 */
		CFastArray<int> m_localUdpPortPool;

		/**
		\~korean
		서버에 연결하기 전에, 서버와의 프로토콜 매칭을 위한 값입니다.
		- CNetServer.Start에서 입력했던 protocol version과 서버와의 연결이 성공합니다. 그렇지 않을 경우
		ErrorType_ProtocolVersionMismatch가 서버 연결 후 응답으로 옵니다.

		\~english
		This is the value to match the protocol with servers before connecting to the servers.
		- The connection to the server with the protocol version that was input at CNetServer.Start.
		If not, ErrorType_ProtocolVerionMismatch is to be returned after connected to the server.

		\~chinese
		连接服务器之前，为了与服务器protocol匹配的值。
		- 在 CNetServer.Start%输入的protocol version和服务器连接成功了。否则连接服务器之后会得到ErrorType_ProtocolVersionMismatch。

		\~japanese
		サーバーに接続する前に、サーバーとのプロトコルマッチングのための値です。
		- CNetServer.Startで入力したprotocol versionとサーバーとの接続が成功します。そうではない場合、ErrorType_ProtocolVersionMismatchがサーバー接続後に応答として来ます。
		\~
		 */
		Guid m_protocolVersion;
		/**
		\~korean
		서버에 보내는 추가 연결 정보입니다. INetServerEvent.OnConnectionRequest()에서 이 데이터가 받아집니다.

		\~english
		This is an additional connection info to be sent to the server.
 		This data is received at INetServerEvent.OnConnectRequest().

		\~chinese
		在服务器发送额外附加连接信息。
		在 INetServerEvent.OnConnectionRequest()接收数据。

		\~japanese
		サーバーに送る追加接続情報です。INetServerEvent.OnConnectionRequest()でこのデータが受け取られます。
		\~
		 */
		ByteArray m_userData;

		/**
		\~korean
		기본값은 false 입니다.
		true 로 설정할 경우 Reliable P2P 의 전송속도는 1MB/sec를 감당할 수 없습니다.
		그러나 처리 성능이 가벼워집니다. 더미 클라이언트 테스트를 할 때에만 true 로 설정하십시오.

		\~english
		Default is false.
		If setting it as true, Reliable P2P transmission speed cannot support 1MB/sec.
		But performance will be improved, so set it as true only for dummy client test.

		\~chinese
		基本值为false。
		如果将此设置为true，Reliable P2P的传送速度将无法承载1MB/sec。
		但会提高处理性能，请只在进行测试时将此值设置为true。

		\~japanese
		デフォルト値は falseです。
		true に設定する場合Reliable P2Pの転送速度は 1MB/secを耐えることはできません。
		しかし処理性能がよくなります。ダミークライアントテストをする時だけ true に設定してください。

		\~
		*/
		bool m_slowReliableP2P;

		// coalesce interval. 테스트용이므로 평소에는 손대지 말 것. 0이면 기본값 인터벌 값으로 대체됨을 의미하며, 이 값 자체의 기본값은 0이다.
		int m_tunedNetworkerSendIntervalMs_TEST;

		/**
		\~korean
		\brief Simple network protocol mode.

		기본값은 false 입니다.
		패킷 캡쳐 및 복제 방식으로 더미 클라이언트 테스트를 가능하게 하기 위해서 이 값을 true로 설정하십시오.
		단, 서비스가 해커의 공격에 취약해 지며 UDP networking과 direct P2P 통신을 사용할 수 없습니다. (대신 relay 로 전송합니다.)
		라이브 서비스를 위해서는 false로 설정하십시오.

		패킷 캡쳐와 리플레이 테스트관련 내용:
		각각의 더미 클라이언트는 자신의 HostID를 확인할 수 없습니다.
		CreateP2PGroup() 과 같이 P2P 그룹 관련 함수를 호출할 경우 예상치 못한 상황이 발생할 수 있습니다.

		\~english
		\brief Simple network protocol mode.

		Default is false.
		Setting this to true allows dummy client test via packet capture and replication method.
		However, it will make service vulnerable to hackers, and does not allow UDP networking
		and direct P2P communication (will be relayed instead.)
		You should set this to false for live service.

		Notice for packet capture and replay test:
		- Each dummy client cannot identify self HostID.
		Unexpected behavior may occur if you call P2P group functions such as CreateP2PGroup().

		\~chinese
		\brief Simple network protocol mode.

		基本值为false。
		是数据包截取及复制的方式，为能够进行客户端测试请将此值设置为true。
		但这时可能会容易受到黑客攻击且无法使用UDP networking和Direct P2P通信。（但这时会用relay传送）。
		为能够提供实时服务请将此值设置为false。

		数据包截取与Replay测试的相关内容：
		各测试客户端无法确认自己的HostID。
		如CreateP2PGroup()，呼叫P2P组相关函数时可能会发生没有预测到的状况。

		\~japanese
		\brief Simple network protocol mode.

		デフォルト値は false です。
		Packet capture及び複製方式でdummy client テストを可能にするためにこの値を trueに設定してください。
		ただし、サービスがHackerの攻撃に脆弱になり、 UDP networkingと direct P2P通信を使うことができません。 (代わりに relayで転送します。)
		Liveサービスのためには falseに設定してください。

		Packet captureとリプレーテスト関連内容
		各々の dummy client は自分の HostIDの確認ができません。
		CreateP2PGroup() のように P2Pグループ関連関数を呼び出す場合、予想外の状況が発生する恐れがあります。

		\~
		*/
		bool m_simplePacketMode;

		/**
		\~korean
		- OnException 콜백 기능을 사용할 지 여부를 선택합니다.
		- 기본값은 true입니다.
		- 만약 false를 지정하면 유저 콜백에서 예상치 못한 Exception 발생 시 OnException이 호출되지 않고 크래시가 발생합니다.

		\~english
		- Decide whether it uses OnException callback function or not.
		- Default value is true.
		- If setting it as false, when exception occurs during  user callback, crash will occur as OnException is not called.

		\~chinese
		- 选择是否使用 OnException Callback功能。
		- 基本值为 true。
		- 如果指定false，用户在Callback过程中发生意想不到的 Exception 时，OnException将不被呼出，且发生Crash。

		\~japanese
		- OnException Callback機能を使用するかどうかを選択します。
		- デフォルト値はtrueです。
		- もしfalseを指定するとユーザーCallbackから予想できないExceptionが発生した時、OnExceptionが呼び出されずにクラッシュが発生します。
		\~
		*/
		bool m_allowExceptionEvent;

		/**
		\~korean
		서버와 클라이언트가 NAT 라우터 뒤에 있는 상태에서 다른 클라이언트와 P2P 통신을 해야 할 경우 필요할 수 있습니다.
		가령 클라이언트가 게임 플레이를 처리하는 super peer가 되어야 하는 경우,
		그리고 super peer와 서버가 같은 host인 경우,
		super peer 안에서는 여기에 super peer의 public 주소를 넣으시기 바랍니다.

		\~english
		It is variable used when trying to set the clients' external address by force.
		It might be necesaary to make a P2P communication with other clients when a server and client is behind the NAT router.
		For example, if it should be the super peer that the client deal with game play and if it is the host that has a same server with the super peer, put the public address of the super peer here in the super peer.

		\~chinese
		是为了强制设置客户端的外部地址的时候使用的变量。
		服务器和客户端在NAT路由器后面的状态下跟其他客户端P2P通信的时候可能需要。
		即使客户端（client）需要处理游戏的super peer, 而且super peer 和服务器一样是host的情况下，super peer要配置super peer的public地址。
		平时不要设置价格。

		\~japanese
		クライアントの外部アドレスを強制的に設定しようとする際に使用される変数です。
		サーバーとクライアントがNATルーターの後ろにある状態で、他のクライアントとP2P通信をしなければならない場合、必要になるかもしれません。
		仮にクライアントがゲームプレイを処理するsuper peerにならなければならない場合、
		そしてsuper peerとサーバーが同じhostの場合、
		super peee内では、ここにsuper peerのpublicアドレスを入力してください。
		普段は値を設定しないでください。

		\~
		*/
		String m_clientAddrAtServer;

		/** <a target="_blank" href="http://guide.nettention.com/cpp_ko#acr" >연결 유지 기능 사용하기</a> 를 켜거나 끕니다. 기본적으로 꺼져 있습니다. */
		bool m_enableAutoConnectionRecovery;

		/** 클라이언트-서버간에 TCP 핑퐁을 주고 받으며, 이 핑퐁이 너무 오랫동안 되지 못하면 연결을 끊어버리는 기능입니다.
		
		기본값은 true입니다. 하위호환성을 위해 true일 뿐이며, 여러분은 가급적이면 이 값을 false로 바꾸는 것을 권장합니다.

		일반적인 경우 이것을 켜지 않으셔도, NetClient와 NetServer는 TCP 연결 해제를 즉시 혹은 20초 정도 지나서 감지합니다.

		이보다 더 빠른 시간 안에 TCP 연결 해제를 감지하고 싶다면 이 값을 true로 만들고 SetDefaultTimeoutTime이나 SetTimeoutTime을 추가적으로 사용하십시오.

		그러나 주의가 필요합니다. 이것을 true로 설정하는 경우 평소 인터넷 품질이 나쁜 나라나 무선 신호가 약한 네트워킹에서 의도치 않은 연결해제가 일어날 수 있습니다.
		인터넷 환경이 나쁘지만 그래도 조금이나마 통신이 되는 것을, 통신 불가능으로 오판하기 때문입니다. 
		
		이 값이 true이면, 모바일 기기에서 NetClient가 사용중일 때 주의사항이 있습니다. 프로그램이 오랫동안 백그라운드에 있을 때 즉 일시정지 상태에서 수십초 정도의
		오랜 시간이 지나면, 서버에서는 연결해제 즉 OnClientLeave나 OnClientOffline이 발생할 수 있습니다. 
		
		*/
		bool m_closeNoPingPongTcpConnections;

		/**
		\ref
		RMI, 이벤트 를 콜백 받을 쓰레드 모델을 지정합니다.

		- SingleThreaded 지정시 사용자는 NetClient.FrameMove 함수를 호출 할 때 RMI, 이벤트가 콜백 됩니다.
		- MultiThreaded 지정시 RMI, 이벤트 콜백이 멀티 쓰레드로 콜백 됩니다.
		- UseExternalThreadPool 지정시 m_externalUserWorkerThreadPool 에 지정 된 사용자 정의 쓰레드 풀로 작동 됩니다.
		기본 옵션은 SingleThreaded 입니다.

		(주의! SingleThreaded 옵션 사용시 사용자는 NetClient.FrameMove 함수를 반드시 호출 해야 합니다.)
		*/
		ThreadModel m_userWorkerThreadModel;

		/**
		\ref
		네트워크 I/O 처리 작업에 대한 쓰레드 모델을 지정합니다.

		- SingleThreaded 지정시 사용자는 NetClient.FrameMove 함수를 호출 할 때 I/O 처리를 합니다.
		- MultiThreaded 지정시 전역 공유 쓰레드풀에서 멀티 쓰레드로 처리가 됩니다.
		- UseExternalThreadPool 지정시 m_externalNetWorkerThreadPool 에 지정 된 사용자 정의 쓰레드 풀로 작동 됩니다.
		 (전역 공유 쓰레드풀이란, NetClient 객체가 여러개가 되더라도 엔진 내부의 하나의 쓰레드풀에서 처리가 됩니다.)

		기본 옵션은 MultiThreaded 입니다.

		(주의! SingleThreaded 옵션 사용시 사용자는 NetClient.FrameMove 함수를 반드시 호출 해야 합니다.)
		*/
		ThreadModel m_netWorkerThreadModel;

		/**
		\ref
		RMI, 이벤트 콜백 등 다양한 작업을 사용자의 쓰레드 풀에서 작동 하도록 합니다.
		m_userWorkerThreadModel 에서 ThreadModel_UseExternalThreadPool 지정 시 반드시 이 값을 세팅 해야 합니다.
		*/
		CThreadPool* m_externalUserWorkerThreadPool;

		/**
		\ref
		네트워크 I/O 처리를 사용자의 쓰레드 풀에서 작동 하도록 합니다.
		m_netWorkerThreadModel 에서 ThreadModel_UseExternalThreadPool 지정 시 반드시 이 값을 세팅 해야 합니다.
		*/
		CThreadPool* m_externalNetWorkerThreadPool;

		/**
		\~korean
		Timer callback 주기 입니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_timer_callback" >서버에서 타이머 루프 RMI 이벤트 처리하기</a>  기능입니다.
		이것을 세팅하면 milisec단위로 한번씩 INetServerEvent.OnTick 가 호출됩니다.
		- 0이면 콜백하지 않습니다.
		- 기본값은 0입니다.

		\~english
		This sets the period of timer callback. A function described in <a target="_blank" href="http://guide.nettention.com/cpp_en#server_timer_callback" >Managing Timer loop RMI Event from server</a>.
		When this is activated, INetServerEvent.OnTimerCallback will be called at every period set by millisecond unit.
		- Ther is no callback when this is set as 0.
		- The default value is 0

		\~chinese
		Timer callback周期。<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_timer_callback" >在服务器处理Timer Loop，RMI，Event</a> 功能。
		设置这个的话， INetServerEvent.OnTick%会以milisec单位呼叫一次。
		- 0的的话不会回拨。
		- 默认值是0。

		\~japanese
		Timer callback 周期です。\ref server_timer_callback  機能です。
		これを設定すると、ミリ秒単位で一度ずつINetServerEvent.OnTickが呼び出されます。
		- 0であればコールバックしません。
		- 基本値は0です。
		\~
		*/
		uint32_t m_timerCallbackIntervalMs;

		/**
		\~korean
		Timer callback이 동시 몇 개의 user worker thread에서 호출될 수 있는지를 정합니다.
		기본값은 1입니다. 모든 user worker thread를 사용하고 싶다면 INT_MAX를 입력해도 됩니다.

		\~english TODO:translate needed.

		\~chinese
		决定Timer callback同时能在几个user worker thread被呼叫。
		默认值是1。想使用所有user worker thread的话也可以输入INT_MAX。

		\~japanese
		Timer callbackが同時にいくつのuser worker threadで呼び出されることができるのかを決めます。
		基本値は1です。全てのuser worker threadを使用したい場合は、INT_MAXを入力しても良いです。
		\~
		*/
		int32_t m_timerCallbackParallelMaxCount;

		/**
		\~korean
		서버에서 일정주기에 한번씩 콜백을 할시에 인자로 사용되는 유저 데이터입니다.
		Proud.INetServerEvent.OnTick 가 호출될시에 인자값으로 들어갑니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_timer_callback" >서버에서 타이머 루프 RMI 이벤트 처리하기</a> 기능입니다.
		- 기본값은 NULL입니다.

		\~english
		This is a user data used as an index when the server calls back each time in every period.
		This value is to be input as an index when Proud.INetServerEvent.OnTimerCallback is called. A function described in <a target="_blank" href="http://guide.nettention.com/cpp_en#server_timer_callback" >Managing Timer loop RMI Event from server</a>.
		- The default value is NULL.

		\~chinese
		在服务器每一定周期回拨一次的时候用于因素的用户数据。
		Proud.INetServerEvent.OnTick%被呼叫的时候输入成因素值。是<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_timer_callback" >在服务器处理Timer Loop，RMI，Event</a>%功能。
		- 默认值是NULL。

		\~japanese
		サーバーで所定周期に一度ずつコールバックをする時に因子として使われるユーザーデータです。
		Proud.INetServerEvent.OnTickが呼び出される時に因子値として入ります。\ref server_timer_callback 機能です。
		- 基本値はNULLです。
		\~
		*/
		void* m_timerCallbackContext;

		 PROUD_API CNetConnectionParam();
	};

	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
