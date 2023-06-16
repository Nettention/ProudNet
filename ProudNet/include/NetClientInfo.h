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
	Information structure of other host

	\~chinese
	对方主机的信息构造体。

	\~japanese
	相手ホストの情報構造体
	\~
	*/
	class CNetClientInfo
	{
	public:
		/**
		\~korean
		서버에서 바라본 peer의 TCP socket의 주소. 즉 공인 인터넷 주소(external addr)입니다.
		- 서버와 연결됐지만 아직 홀펀칭이 성사되지 않은 클라이언트도 어쨌거나 주소는 얻을 수 있어야 하므로 이 필드가 존재합니다.
		- 보안을 위해, 클라이언트에서는 타 remote peer에 대한 이 값을 얻지 못합니다.

		\~english
		TCP socket address of peer from server side. It is official internet address (external addr).
		- Client is connected to server but it does not success hole-punching yet. Anyway it need to get address so that is why this field exists.
		- Other remote peer can not get this value as security reason.

		\~chinese
		在服务器里所看到的peer的TCP socket地址。即公认互联网地址（external addr）。
		- 虽然已经与服务器连接，但因还没有打洞的玩家也要获得地址，所以需要这个领域。
		- 为了安全，玩家不能获得其他remote peer的值。

		\~japanese
		サーバーから見たPeerのTCP socketのアドレス。即ち、パブリックインターネットアドレス(external addr)です。
		- サーバーにつながりましたが、まだホールパンチングに成功していないクライアントもどうしてもアドレスは得る必要がありますので、このフィールドが存在します。
		- セキュリティのために、クライアントでは他のremote peerに対するこの値を得られません。
		\~
		*/
		AddrPort m_TcpAddrFromServer;

		/**
		\~korean
		서버에서 바라본 클라이언트의 UDP socket의 주소. 즉, 클라이언트가 서버와의 홀펀칭이 완료된 후의 인식된 주소
		- <a target="_blank" href="http://guide.nettention.com/cpp_ko#get_holepunched_info" >클라이언트의 홀펀칭된 주소를 얻기</a>  및 <a target="_blank" href="http://guide.nettention.com/cpp_ko#use_alternative_p2p" >ProudNet을 쓰지 않고 ProudNet의 홀펀칭 정보를 토대로 별도 UDP 소켓으로 통신하기</a>  선결 이해를 권장합니다.
		- 서버와 연결이 갓 끝난 클라이언트는 AddrPort.Unassigned 이다가 수 초 이내에 홀펀칭된 값으로 바뀝니다.
		따라서 공유기의 종류 또는 공유기 설정에 따라 홀펀칭이 영구적으로 성공하지 못할 수도 있습니다. 이러한 경우 이 값은
		계속해서 AddrPort.Unassigned 일 수도 있습니다.
		- 반드시 이 값이 P2P 홀펀칭된 클라이언트의 주소를 의미하지는 않습니다. 그 값은 엔진 내부에서 관리됩니다.

		\~english
		Client's UDP socket address from server side. It is recognized address once client finished hole-punching with server.
		- We recommend to understand <a target="_blank" href="http://guide.nettention.com/cpp_en#get_holepunched_info" >Earning hole-punched address of client</a> and <a target="_blank" href="http://guide.nettention.com/cpp_en#use_alternative_p2p" >Communicating with a separated UDP socket based on ProudNet hole-punched information without using ProudNet.</a> first.
		- Client that just finished connecting to server is AddrPort.Unassigned then it changes to hole-punched value within second. So hole-punching never success depends on type or setting of router.
		This case, value can be AddrPort.Unassigned.
		- Sometimes, this value does not mean address of P2P hole-punched client. This value will manange from internal of engine.

		\~chinese
		在服务器所视的client UDP socket地址。即client与服务器的打洞成功后被识别的地址。
		- 建议预先理解<a target="_blank" href="http://guide.nettention.com/cpp_zh#get_holepunched_info" >获取客户端已进行打洞的地址</a>%及<a target="_blank" href="http://guide.nettention.com/cpp_zh#use_alternative_p2p" >不使用ProudNet，只基于ProudNet的打洞情报用另外的UDPSocket进行通信</a>%。
		- 与服务器的连接刚结束的玩家先是 AddrPort.Unassigned%，在数秒以内会改成已被打洞的值。
		因此，随着路由器的种类或者路由器的设置，打洞可能不会永久性的成功。这时候此值可能一直会是 AddrPort.Unassigned%。
		- 这个值不是必须意味着P2P被打洞的client地址。那个值在引擎内部管理。

		\~japanese
		サーバーから見たクライアントのUDP socketのアドレス。即ち、クライアントがサーバーとのホールパンチングが完了した後の認識アドレス
		- \ref get_holepunched_info 及び\ref use_alternative_p2pを先に理解することをお勧めします。
		- サーバーとの接続が終わったままのクライアントは、AddrPort.Unassignedであるが、数秒以内にホールパンチングされた値に変わります。
		よって、ルーターの種類またはルーター設定によってホールパンチングが永久的に成功できないかも知れません。このような場合、この値は続けて AddrPort.Unassignedになることもあります。
		- 必ずこの値がP2Pホールパンチングされたクライアントのアドレスを意味することはありません。その値はエンジン内部で管理されます。
		\~
		*/
		AddrPort m_UdpAddrFromServer;

		/**
		\~korean
		peer 내부에서의 UDP socket의 주소
		- 서버와의 UDP 홀펀칭이 아직 안끝난 클라이언트, 즉 TCP로만 통신중인 클라이언트는 이 값이 Unassigned일 수 있습니다. 그러나 비정상 상황은 아닙니다.

		\~english
		UDP socket address of inside of peer
		- Client that has not finished UDP hole-punching with server. If client communicate wiht only TCP, it can be Unassigned. However it is not wrong situation.

		\~chinese
		在peer内部的UDP socket地址。
		- 与服务器的UDP打洞还没有结束的client。即对于只用TCP通信的client，此值可能会unassigned。但不是非正常情况。

		\~japanese
		peer内部でのUDP socketのアドレス。
		- サーバーとのUDPホールパンチングがまだ終わっていないクライアント、即ち、TCPへのみ通信しているクライアントはこの値がUnassignedになることがあります。しかし、異常状況ではありません。

		\~
		*/
		AddrPort m_UdpAddrInternal;

		/**
		\~korean
		CNetServer 에서 할당해준 peer 의 int.

		\~english
		int of peer that assigned from CNetServer

		\~chinese
		在 CNetServer%分配的peer的int。

		\~japanese
		CNetServerで割り当てたpeerのint.  
		\~
		*/
		HostID m_HostID;

		/**
		\~korean
		true이면 이 클라이언트 peer로의 RMI는 서버를 경유하는 P2P relay를
		함을 의미합니다.
		- CNetClient 에서만 유효한 값.

		\~english
		If it is ture, it means this client peer is doing P2P relay with RMI through a server.
		- Available value on CNetClient

		\~chinese
		True的话意味着此client peer的RMI进行经由服务器的P2P relay。
		- 只有在 CNetClient%是有效值。

		\~japanese
		Trueであれば、このクライアントpeerへのRMIはサーバーを経由するP2P relayを行うことを意味します。
		- CNetClientでのみ有効な値 
		\~
		*/
		bool m_RelayedP2P;

		/**
		\~korean
		이 client가 참여하고 있는 P2P 그룹의 리스트
		- CNetServer, CNetClient 모두에서 유효합니다.

		\~english
		P2P group list that this client joined with
		- It is available at CNetServer, CNetClient.

		\~chinese
		此client参与的P2P组的列表。
		- 在 CNetServer, CNetClient%全部有效。

		\~japanese
		このクライアントが参加しているP2Pグループのリスト
		- CNetServer, CNetClientの両方で有効です。 
		\~
		*/
		HostIDSet m_joinedP2PGroups;

		/**
		\~korean
		true인 경우 이 클라이언트는 NAT 장치 뒤에 있음을 의미합니다.
		- CNetServer, CNetClient 모두에서 유효합니다.

		\~english
		If it is true, it means this client locates behind of NAT device.
		- It is available at CNetServer, CNetClient.

		\~chinese
		True的话意味着此client在NAT装置后面。
		- 在 CNetServer, CNetClient%全部有效。

		\~japanese
		Trueの場合、このクライアントはNAT装置の裏にあることを意味します。
		- CNetServer, CNetClientの両方で有効です。 
		\~
		*/
		bool m_isBehindNat;

		/**
		\~korean
		true인 경우 이 클라이언트는 서버와의 UDP 통신이 유지되고 있음을 의미합니다.
		false인 경우 UDP 통신이 불가능하기 때문에 TCP가 UDP 대신 사용되고 있음을 의미합니다.

		\~english
		If it is ture, it means UDP communication between this client and server is still up and running.
		If it is false, it means TCP is being used because UDP communication is impossible.

		\~chinese
		True的话意味着此client维持着与服务器的UDP通信。
		False 的话意味着因为无法进行UDP通信，TCP 代替UDP所使用。

		\~japanese
		Tureの場合、このクライアントはサーバーとのUDP通信が維持されていることを意味します。
		Falseの場合、UDP通信ができないので、TCPがUDPの代わりに使っていることを意味します。
		\~
		*/
		bool m_realUdpEnabled;

		/**
		\~korean
		이 클라이언트가 작동중인 NAT 장치의 이름
		- CNetServer 에서만 얻을 수 있습니다.
		- 실제로 NAT device 뒤에 있는 클라이언트라 하더라도, 이름을 얻는데는 어느 정도의 시간이 필요합니다. 따라서
		바로 접속이 들어온 클라이언트의 정보를 얻을 경우 이 값이 아직 채워지지 않을 수 있습니다. 이러한 경우 잠시 뒤에
		다시 클라이언트의 정보를 얻어오면 NAT 장치의 이름을 얻을 수 있습니다.

		\~english
		Name of NAT device which operated by this client
		- You can get it from CNetServer only.
		- If client actually locate behind of NAT device, it will take some time to get a name. So if you get client information that just loged on, it may empty.  This case you can get name of NAT device by reloading client information in a few minutes.

		\~chinese
		此client运转中的NAT装置名称。
		- 只能在 CNetServer%中获得。
		- 即使是在实际NAT device后面的client，获得名字需要一定的时间。因此，获取随即连接的client信息的时候，此值可能没有被填充。这时候，稍后重新获得client信息的话，可以获得NAT装置的名字。

		\~japanese
		このクライアントが作動しているNAT装置名
		- CNetServerでのみ得られます。
		- 実際にNAT deviceの裏にあるクライアントとしても、名前を得ることにある程度の時間が必要です。よって、すぐ接続が入ってきたクライアントの情報を得る場合、この値がまだ満たされないことがあります。このような場合、しばらく後で再びクライアントの情報を得るとNAT装置の名前を得ることができます。

		\~
		*/
		String m_natDeviceName;

		/**
		\~korean
		최근에 측정된 ping의 평균 시간 (초단위)
		- CNetClient.GetPeerInfo 에서 얻은 경우: 해당 P2P peer 의 ping입니다.
		- CNetServer.GetClientInfo 에서 얻은 경우: 서버-클라간의 ping입니다.

		\~english
		Average ping time that measure recently (Second)
		- When it get from CNetClient.GetPeerInfo: Ping of P2P peer
		- When it get from CNetServer.GetClientInfo: Ping between server-client

		\~chinese
		最近检测的ping的平均时间（秒单位）。
		- 在 CNetClient.GetPeerInfo%获得的情况：相关P2P peer的ping。
		- 在 CNetServer.GetClientInfo%获得的情况：服务器-client 之间的ping。

		\~japanese
		最近に測定したpingの平均時間(秒単位)
		- CNetClient.GetPeerInfoから得た場合：該当P2P peerのpingです。 
		- CNetServer.GetClientInfoから得た場合：サーバークライアント間のpingです。 
		\~
		*/
		int m_recentPingMs;

		/**
		\~korean
		이 peer로의 송신 대기중인 메시지의 총량(바이트 단위) 입니다.
		- 서버에서 peer에 대해 얻는 경우 서버=>클라이언트 송신에 대한 총량입니다.
		- 클라이언트에서 peer에 대해 얻는 경우 클라이언트=>클라이언트 송신에 대한 총량(단, relay되는 메시지에 대해서는 제외)

		\~english
		Total amount(byte) of waiting message to this peer.
		- When it get about peer from server, it is total sending amount of server to client
		- When it get about peer from client, it is total sending amount of client to client (except message that relayed)

		\~chinese
		用此peer的等待传送的信息总量（byte 单位）。
		- 在服务器对peer获得的情况：服务器=>client 传送信息的总量。
		- 在client对peer获得的情况：client=>client 传送信息的总量（但，对relay的信息例外）。

		\~japanese
		このpeerへの送信を待っているメッセージの総量(バイト単位)です。
		- サーバーでpeerに対して得る場合、サーバー=>クライアント送信に対する総量です。
		- クライアントでpeerに対して得る場合、クライアント=>クライアント送信に対する総量(但し、relayされるメッセージに対しては除外)
		\~
		*/
		int m_sendQueuedAmountInBytes;

		/**
		\~korean
		사용자가 지정한 tag의 포인터입니다.
		- CNetServer.SetHostTag, CNetClient.SetHostTag 을 통해 지정한 값입니다.
		- 주의! : tag는 네트웍 동기화가 되지 않는 값입니다.

		\~english
		Pointer of user defined tag.
		- It designates through CNetServer.SetHostTag and CNetClient.SetHostTag
		- Warning!! : tag value does not synchronize with network.

		\~chinese
		用户指定的tag指针。
		- 通过 CNetServer.SetHostTag, CNetClient.SetHostTag%指定的值。
		- 注意！：tag 是没有联网同步的值。

		\~japanese
		ユーザーが指定したtagのポインターです。
		- CNetServer.SetHostTag, CNetClient.SetHostTag を通じ指定した値です。 
		- ご注意!：tagはネットワーク同期化がされない値です。
		\~
		*/
		void* m_hostTag;

		/**
		\~korean
		클라이언트의 Frame Rate 입니다.
	    - CNetClient.SetApplicationHint 에서 사용자가 입력한 값입니다.
		- 각 피어에는 전달되지 않으며, 서버에서 각 클라이언트의 Frame Rate 를 확인 하고자 할 때 사용합니다.
		- Frame rate는 통신량 절감을 위하여 전송시 float값으로 변환되어 송수신 됩니다.

		\~english
		Frame Rate of client.
		- User enter this value on CNetClient.SetApplicationHint.
		- It does not send to each peer, it only use to check Frame Rate of client at server.
		- To reduce traffic, frame rate will send/receive as float value.

		\~chinese
		Client 的Frame Rate。
		- 在 CNetClient.SetApplicationHint%用户输入的值。
		- 不往各peer传达，在服务器想确认各client的Frame Rate的时候使用。
		- Frame rate 为了节省通信量，传送的时候转换为float值后发收信息。

		\~japanese
		クライアントのFrame Rateです。
		- CNetClient.SetApplicationHintでユーザーが入力した値です。
		- 各peerには伝達されなく、サーバーで各クライアントのFrame Rateを確認したい時に使用します。
		- Frame rateは通信料節減のために転送時にFloat値に変換されて送受信されます。
		\~
		*/
		double m_recentFrameRate;

		/**
		\~korean
		해당 클라이언트가 서버에게 Udp packet 전송을 시도한 총 횟수

		\~english
		Total number of attempt to send Udp packet to server from client

		\~chinese
		相关client给服务器试图UDP packet传送的总次数。

		\~japanese
		該当クライアントがサーバーにUDP packet転送を行った総数
		\~
		*/
		uint32_t m_toServerSendUdpMessageTrialCount;

		/**
		\~korean
		해당 클라이언트가 서버에게 Udp packet 전송을 성공한 총 횟수

		\~english
		Total number of attempt to success sending Udp packet to server from client

		\~chinese
		相关client给服务器成功传送UDP packet的总次数。

		\~japanese
		該当クライアントがサーバーにUDP packet転送に成功した総数
		\~
		*/
		uint32_t m_toServerSendUdpMessageSuccessCount;

		/**
		\~korean
		클라이언트가 현재 사용하고 있는 HostID 의 재사용 횟수입니다.
		- StartParameter.m_HostIDGenerationPolicy 로 재사용을 할지를 결정할수 있습니다.
		- 재사용하지 않는 옵션이면 무조건 0이 나옵니다.

		\~english
		Total reused count of HostID that client is using currently.
		- You can decide reusing with StartParameter.m_HostIDGenerationPolicy.
		- If you select non-recyle option, value will be 0

		\~chinese
		Client 现在使用的Host ID的再使用次数。
		- 可以决定是否以 StartParameter.m_HostIDGenerationPolicy%再次使用。
		- 不是再次使用的选项的话必为0。

		\~japanese
		クライアントが現在使っているHostIDの再使用回数です。
		- StartParameter.m_HostIDGenerationPolicyで再使用をするかどうかを決めることができます。
		- 再使用しないオプションであれば無条件0が出ます。
		\~
		*/
		uint32_t m_hostIDRecycleCount;

		int64_t m_unreliableMessageRecentReceiveSpeed;

		/**
		\~korean
		클라이언트가 접속한 서버의 주소입니다.

		\~english
		Address that client is connected with.
		*/
		AddrPort m_TcpLocalAddrFromServer;

		PROUD_API CNetClientInfo();

		PROUD_API String ToString(bool atServer);


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
