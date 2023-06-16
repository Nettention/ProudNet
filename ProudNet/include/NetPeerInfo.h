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
#include "AddrPort.h"
#include "Enums.h"
#include "HostIDArray.h"

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
	상대 호스트의 정보 구조체

	\~english
	Information construct of opponent host

	\~chinese
	对方主机的信息构造体。

	\~japanese
	相手ホストの情報構造体
	\~
	*/
	class CNetPeerInfo
	{
	public:
		/**
		\~korean
		서버에서 바라본 클라이언트의 UDP socket의 주소. 즉, 클라이언트가 서버와의 홀펀칭이 완료된 후의 인식된 주소
		- <a target="_blank" href="http://guide.nettention.com/cpp_ko#get_holepunched_info" >클라이언트의 홀펀칭된 주소를 얻기</a>  및 <a target="_blank" href="http://guide.nettention.com/cpp_ko#use_alternative_p2p" >ProudNet을 쓰지 않고 ProudNet의 홀펀칭 정보를 토대로 별도 UDP 소켓으로 통신하기</a>  선결 이해를 권장합니다.
		- 서버와 연결이 갓 끝난 클라이언트는 AddrPort.Unassigned 이다가 수 초 이내에 홀펀칭된 값으로 바뀝니다.
		따라서 공유기의 종류 또는 공유기 설정에 따라 홀펀칭이 영구적으로 성공하지 못할 수도 있습니다. 이러한 경우 이 값은
		계속해서 AddrPort.Unassigned 일 수도 있습니다.
		- 반드시 이 값이 P2P 홀펀칭된 클라이언트의 주소를 의미하지는 않습니다. 그 값은 엔진 내부에서 관리됩니다.

		\~english
		Address of TCP socket of peer from server perspective. In other words, recognized address after finished t hole-punching from client to server.
		- We recommend to understand <a target="_blank" href="http://guide.nettention.com/cpp_en#get_holepunched_info" >Earning hole-punched address of client</a> and <a target="_blank" href="http://guide.nettention.com/cpp_en#use_alternative_p2p" >Communicating with a separated UDP socket based on ProudNet hole-punched information without using ProudNet.</a> first.
		- The client jsut completed connecting to server has AddrPort.Unassigned value and changed to have hole-punched value within seconds.
		  That means depending ontype and/or settings of router, the hole-punching can never be successful. If this happens, the value can remain as the value of AddrPort.Unassigned.
		- This value does not necessarily mean the address of P2P hole-punched client address. The value is managed internally by engine.

		\~chinese
		在服务器里所看到的client的UDP socket地址。即，client 与服务器的打洞完成后的识别地址。
		- 建议<a target="_blank" href="http://guide.nettention.com/cpp_zh#get_holepunched_info" >获取客户端已进行打洞的地址</a>%及<a target="_blank" href="http://guide.nettention.com/cpp_zh#use_alternative_p2p" >不使用ProudNet，只基于ProudNet的打洞情报用另外的UDPSocket进行通信</a>%的预先理解。
		- 与服务器的连接刚结束的玩家先是 AddrPort.Unassigned ，数秒以后改成已被打洞的值。
		因此，随着路由器的种类或者路由器的设置，打洞可能不会永久性的成功。这时候此值可能一直会是 AddrPort.Unassigned%。
		- 这个值不是必须意味着P2P被打洞的client地址。那个值在引擎内部管理。


		\~japanese
		サーバーから見たクライアントのUDP socketのアドレス。即ち、クライアントがサーバーとのホールパンチングが完了された後の認識アドレス
		- \ref get_holepunched_info 及び \ref use_alternative_p2p を先に理解することをお勧めします。
		- サーバーとの接続が終わったままのクライアントはAddrPort.Unassignedですが、数秒以内にホールパンチングされた値に変わります。よって、ルーターの種類またはルーター設定によってホールパンチングが永久的に成功できないこともあります。このような場合、この値は続けてAddrPort.Unassignedになることもあります。
		- 必ず、この値がP2Pホールパンチングされたクライアントのアドレスを意味することはありません。その値はエンジン内部で管理されます。
		\~
		*/
		AddrPort m_UdpAddrFromServer;

		/**
		\~korean
		peer 내부에서의 UDP socket의 주소
		- 서버와의 UDP 홀펀칭이 아직 안끝난 클라이언트, 즉 TCP로만 통신중인 클라이언트는 이 값이 Unassigned일 수 있습니다. 그러나 비정상 상황은 아닙니다.

		\~english
		Address of UDP socket inside of peer
		- The client that has not comp[leted UDP hole-punching with server, in other words the client communicating only through TCP can have this value as unassigned. But this is not abnormal situation.

		\~chinese
		在peer内部的UDP socket地址。
		- 与服务器的UDP打洞还没有结束的client。即对于只用TCP通信的client，此值可能会unassigned。但不是非正常情况。

		\~japanese
		peer 内部でのUDP socketのアドレス
		- サーバーとのUDPホールパンチングがまだ終わってないクライアント、即ち、TCPへのみ通信しているクライアントは、この値がUnassignedの場合もあります。しかし、異常状況ではありません。 
		\~
		*/
		AddrPort m_UdpAddrInternal;

		/**
		\~korean
		CNetServer 에서 할당해준 peer 의 int.

		\~english
		Int of peer that allocated by CNetServer.

		\~chinese
		在 CNetServer%分配的peer的int。

		\~japanese
		CNetServerより割り当てられたpeerのint.
		\~
		*/
		HostID m_HostID;

		/**
		\~korean
		true이면 이 클라이언트 peer로의 RMI는 서버를 경유하는 P2P relay를
		함을 의미합니다.
		- CNetClient 에서만 유효한 값.

		\~english
		If true then RMI towards to peer of tis client P2P relayes.
		- Only valid in CNetClient

		\~chinese
		True的话意味着此client peer的RMI进行经由服务器的P2P relay。
		- 只有在 CNetClient%是有效值。

		\~japanese
		TrueであればこのクライアントのpeerへのRMIはサーバーを経由するP2P relayを意味します。
		- CNetClientでのみ有効な値 
		\~
		*/
		bool m_RelayedP2P;

		/**
		\~korean
		이 client가 참여하고 있는 P2P 그룹의 리스트
		- CNetServer, CNetClient 모두에서 유효합니다.

		\~english
		P2P group list that this client is participating
		- Valid in both CNetServer and CNetClient

		\~chinese
		此client参与的P2P组的列表。
		- 在 CNetServer, CNetClient%全部有效。

		\~japanese
		このクライアントが参加しているP2Pグループのリスト
		- CNetServer, CNetClient の両方で有効です。 
		\~
		*/
		HostIDArray m_joinedP2PGroups;

		/**
		\~korean
		true인 경우 이 클라이언트는 NAT 장치 뒤에 있음을 의미합니다.
		- CNetServer, CNetClient 모두에서 유효합니다.

		\~english
		If true then this client is behind NAT device
		- Valid in both CNetServer and CNetClient

		\~chinese
		True 的话意味着此client在NAT装置后面。
		- 在 CNetServer, CNetClient%全部有效。

		\~japanese
		trueの場合、このクライアントはNAT装置の後にあることを意味します。
		- CNetServer, CNetClient の両方で有効です。 
		\~
		*/
		bool m_isBehindNat;

		/**
		\~korean
		최근에 측정된 ping의 평균 시간 (밀리초단위)
		- CNetClient.GetPeerInfo 에서 얻은 경우: 해당 P2P peer의 ping입니다.
		- CNetServer.GetClientInfo 에서 얻은 경우: 서버-클라간의 ping입니다.

		\~english
		Recently measured the average time of ping (in Millisecond)
		- If from CNetClient.GetPeerInfo: ping of the P2P peer
		- If from CNetServer.GetClientInfo: ping between server and client

		\~chinese
		最近检测的ping的平均时间（毫秒单位）。
		- 在 CNetClient.GetPeerInfo%获得的情况：相关P2P peer的ping。
		- 在 CNetServer.GetClientInfo%获得的情况：服务器-client 之间的ping。

		\~japanese
		最近に測定されたpingの平均時間（ミリ秒単位）
		- CNetClient.GetPeerInfo より得た場合：該当P2P peerのpingです。 
		- CNetServer.GetClientInfoより得た場合：サーバークライアント間のpingです。 
		\~
		*/
		int m_recentPingMs;

		/**
		\~korean
		이 peer로의 송신 대기중인 메시지의 총량(바이트 단위) 입니다.
		- 서버에서 peer 에 대해 얻는 경우 서버=>클라이언트 송신에 대한 총량입니다.
		- 클라이언트에서 peer 에 대해 얻는 경우 클라이언트=>클라이언트 송신에 대한 총량(단, relay되는 메시지에 대해서는 제외)

		\~english
		Total amount(in byte) of waiting messages to be sent to this peer
		- If getting from peer by server: Total amount of transmission by client
		- If getting from peer by client: Total amount of transmission by client (but excluding relayed messages)

		\~chinese
		用此peer的等待传送的信息总量（byte 单位）。
		- 在服务器对peer获得的情况：服务器=>client 传送信息的总量。
		- 在client对peer获得的情况：client=>client 传送信息的总量（但，对relay的信息例外）。

		\~japanese
		このpeerへの送信を待っているメッセージの総量（バイト単位）です。
		- サーバーよりpeerに対して得る場合、サーバー=>クライアント送信に対する総量です。
		- クライアントよりpeerに対して得る場合、クライアント=>クライアント送信に対する総量（但し、relayされないメッセージに対しては除外）
		\~
		*/
		int m_sendQueuedAmountInBytes;

		/**
		\~korean
		사용자가 지정한 tag의 포인터입니다.
		- CNetServer.SetHostTag, CNetClient.SetHostTag 을 통해 지정한 값입니다.
		- 주의! : tag는 네트웍 동기화가 되지 않는 값입니다.

		\~english
		Pointer of user defined tag
		- A value set by CNetServer.SetHostTag and CNetClient.SetHostTag
		- Attention!: tag is a value that cannot be network synchronized.

		\~chinese
		用户指定的tag指针。
		- 通过 CNetServer.SetHostTag,  CNetClient.SetHostTag%指定的值。
		- 注意！：tag 是没有联网同步的值。

		\~japanese
		ユーザーが指定したtagのポインターです。
		- CNetServer.SetHostTag, CNetClient.SetHostTagを通じ指定した値です。 
		- ご注意!：tagはネットワーク同期化が取れない値です。 
		\~
		*/
		void* m_hostTag;

		/**
		\~korean
		클라이언트의 Frame Rate 입니다.
		- CNetClient.SetApplicationHint 에 사용자가 입력한 값입니다.
		- P2P 그룹을 맺은 각 피어들의 Frame Rate 를 확인하고자 할 때 사용합니다.
		- 핑과 함께 수퍼피어 선정에 사용할 수 있습니다.
		- Frame rate는 통신량 절감을 위하여 전송시 float값으로 변환되어 송수신 됩니다.

		\~english
		Frame Rate of client
		- User input value at CNetClient.SetApplicationHint
		- It uses to check Frame Rate that each peers in P2P group
		- You can use to select super peer with ping
		- To reduce traffic, it sends/receives with float value

		\~chinese
		Client 的Frame Rate。
		- 用户在 CNetClient.SetApplicationHint%输入的值。
		- 要确定连接P2P组的各peer的Frame Rate的时候使用。
		- 与ping一起可以使用于super peer选定。
		- Frame rate 为了减少通信量，传送时转换为float值后发送信息。

		\~japanese
		クライアントのFrame Rateです。
		- CNetClient.SetApplicationHintにユーザーが入力した値です。
		- P2Pグループを結んだ各ピーアのFrame Rateを確認したい時に使います。
		- pingとともにsuper peerの設定に使うことができます。
		- Frame rateは通信料の節減のために、転送時にfloat値に変換して送受信されます。 
		\~
		*/
		double m_directP2PPeerFrameRate;

		/**
		\~korean
		자신(CNetClient)이 해당 클라에게 Udp packet을 전송 시도한 총 갯수

		\~english
		Total attempted number of sending Udp packet from CNetClient to other client

		\~chinese
		自身（CNetClient）试图给相关client传送UDP packet的总个数。

		\~japanese
		自分(CNetClient)が該当クライアントにUDP packetの送信を実行した総数
		\~
		*/
		uint32_t m_toRemotePeerSendUdpMessageTrialCount;

		/**
		\~korean
		자신(CNetClient)이 해당 클라에게 Udp packet을 전송해서 성공한 총 갯수

		\~english
		Total succeed number of sending Udp packet from CNetClient to other client

		\~chinese
		自身（CNetClient）给相关client成功传送UDP packet的总个数。

		\~japanese
		自分(CNetClient)が該当クライアントにUDP packetの送信を成功した総数
		\~
		*/
        uint32_t m_toRemotePeerSendUdpMessageSuccessCount;

		int64_t m_unreliableMessageReceiveSpeed;

		 PROUD_API CNetPeerInfo();

		 PROUD_API String ToString(bool atServer);

		/**
		\~korean
		UDP Data 전송되는 전체 Byte 수

		\~english
		The number of total bytes that transmits the UDP data

		\~chinese
		UDP 方式传输的所有字节数


		\~japanese
		UDP Data 送信される全体バイト数
		\~
		*/
		int64_t m_udpSendDataTotalBytes;



#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않습니다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif
	};

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
