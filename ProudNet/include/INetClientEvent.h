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

#include "INetCoreEvent.h"
#include "acr.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	/**
	\~korean
	CNetClient 용 event sink

	용법
	- 이 객체를 구현한다.
	- CNetClient 에 파라메터로 전달한다.

	\~english
	Event sink for CNetClient

	Usage
	- Realize this object.
	- Passed as a parameter to CNetClient

	\~chinese
	CNetClient 用event sink

	用法
	- 体现此对象。
	- 用参数传送给 CNetClient%。

	\~japanese
	CNetClient 用 event sink

	用法
	- このオブジェクトを実現します。
	- CNetClientにパラメーターで伝達します。
	\~
	*/
	class INetClientEvent : public INetCoreEvent
	{
	public:
		INetClientEvent() {}
		virtual ~INetClientEvent() {}

		/**
		\~korean
		CNetServer.Connect 를 통해 서버 연결을 시도한 결과가 도착하면 발생하는 이벤트이다.

		\param info 서버 연결 결과를 다음 객체. 서버와의 연결이 성공한 경우 ErrorInfo.m_type 이 success value를 갖는다.
		서버와의 연결이 실패한 경우 이 값을 열람하면 된다. 자세한 것은 ErrorInfo 클래스 설명을 참고.
		\param replyFromServer 서버로부터 받은 추가 메시지이다. 이 값은
		INetServerEvent.OnConnectionRequest 에서 보낸 값이다.

		\~english
		Event that occurs after arrival of the result of attempt to connect to server through CNetServer.Connect

		\param info object that contains the result of connection to server. ErrorInfo.m_type has success value when it is successful.
		View this value when connection attempt to server failed. Please refer ErrorInfo class description.
		\param replyFromServer Additional message from server. This value is from INetServerEvent.OnConnectionRequest.

		\~chinese
		通过 CNetServer.Connect%尝试服务器连接的结果到达的话发生的event。

		\param info 包含服务器连接的结果的对象。与服务器的连接成功的话， ErrorInfo.m_type%拥有success value。
		与服务器的连接失败的话，查看此值即可。详细的请参考ErrorInfo类说明。
		\param replyFromServer 从服务器接收的附加信息。此值是从 INetServerEvent.OnConnectionRequest%发送的值。

		\~japanese
		CNetServer.Connectを通じサーバー接続を試した結果が到着すると発生するイベントです。

		\param infoサーバー接続の結果を次のオブジェクト。サーバーとの接続が成功した場合、ErrorInfo.m_typeがsuccess valueを持ちます。
		サーバーとの接続に失敗した場合、この値を閲覧すれば良いです。詳しくは、ErrorInfoクラスの説明をご参照ください。
		\param replyFromServer サーバーから受けた追加メッセージです。この値は、
		INetServerEvent.OnConnectionRequestより送られた値です。 
		\~
		*/
		virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray& replyFromServer) = 0;

		/**
		\~korean
		서버 연결 해제시 발생 이벤트입니다.
		- 클라이언트에서 먼저 연결 해제(Disconnect 호출)를 한 경우에는 이 이벤트가 발생하지 않습니다.
		그러나, NetClient.FrameMove를 호출하는 스레드가 NetClient.Disconnect 또는 NetClient.Dispose를 호출하는 스레드가 같으면 콜백될 수 있습니다.
		\param errorInfo 어떤 이유로 서버와의 연결이 해제되었는지를 담고 있습니다. 자세한 것은 ErrorInfo 도움말에 있습니다.

		\~english
		Event that occurs when server connection is terminated
		- This event also occurs when client terminates the connection to server.
		However, it will be called back if the thread where NetClient.FrameMove is called and NetClient.Disconnect is called are same.
		\param errorInfo Contains the reason why the connection was terminated. Please refer ErrorInfo.

		\~chinese
		服务器解除连接时发生的事件。
		- 玩家先解除连接的（呼叫Disconnect）时候不会发生。
		However, it will be called back if the thread where NetClient.FrameMove is called and NetClient.Disconnect is called are same.
		\param errorInfo 记载着与服务器的连接解除的理由。详细的在ErrorInfo帮助里。

		\~japanese
		サーバー接続を解除時に発生するイベントです。
		- クライアントで先に接続解除(Disconnect呼び出し)をした場合には、このイベントが発生しません。
		しかし、 NetClient.FrameMoveを呼び出すスレッドがNetClient.DisconnectまたはNetClient.Disposeを呼び出すスレッドが同じであればコールバックされることがあります。
		\param errorInfo 何が理由でサーバーとの接続が解除されたのかを持っています。詳しくは、ErrorInfoのヘルプにあります。
		\~
		*/
		virtual void OnLeaveServer(ErrorInfo *errorInfo) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 이 생성되거나 P2P그룹에 새 멤버가 추가되는 경우 이 메서드가 콜백됩니다.
		로컬 호스트 자신에 대해서도 이것이 호출됩니다.

		예를 들어 클라이언트 A가 이미 들어가 있는 그룹 G에 B가 새로 들어오면
		A는 (B,G)를 받고, B는 (A,G), (B,G)를 받게 됩니다.

		\param memberHostID 자기 또는 타 peer의 HostID입니다.
		\param groupHostID P2P 그룹의 HostID입니다.
		\param memberCount 처리된 후 멤버 수 입니다.
		\param message CNetServer.CreateP2PGroup 또는 CNetServer.JoinP2PGroup 에서 사용자가 입력한 커스텀 데이터가 여기에서 그대로 전달됩니다.

		\~english
		This method is to be called back either when <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> is created or when a new member is added to P2P group.
		Also called for local host iteself.

		For an example, if B enters into group G which already has client A in it, then A receives (B,G) and B recieves (A,G) and (B,G).
		\param memberHostID HostID of itself or other peer
		\param groupHostID HostID of P2P group
		\param memberCount The number of menebers after being processed
		\param message The custom data entered by user via either CNetServer.CreateP2PGroup or CNetServer.JoinP2PGroup will be passed onto here as they are.

		\~chinese
		发生<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%，或者往P2P组添加新成员的时候回调此方法。
		这个也对本地主机本身呼叫。

		例如，B 进入玩家A已经进入的组G的话，A 收到(B,G)，B 收到(A,G)，(B,G)。

		\param memberHostID 自己或者其他peer的HostID。
		\param groupHostID P2P组的HostID。
		\param memberCount 被处理后的成员数。
		\param message 在 CNetServer.CreateP2PGroup%或者 CNetServer.JoinP2PGroup ，用户输入的custom数据会如实地发送至这里送。

		\~japanese
		\ref p2p_groupが生成されたり、P2Pグループに新しいメンバーが追加される場合、このメソッドがコールバックされます。
		ローカルホストの自身に対してもこれが呼び出されます。
		例えば、クライアントAが既に入っているグループGにBが新たに入ったらAは(B,G)を受けて、Bは(A,G), (B,G)を受けることになります。

		\param memberHostID 自分または他のpeerのHostIDです。
		\param groupHostID P2PグループのHostIDです。
		\param memberCount 処理された後のメンバー数です。
		\param message CNetServer.CreateP2PGroupまたはCNetServer.JoinP2PGroupでユーザーが入力したカスタムデータがここでそのまま伝達されます。
		\~
		*/
		virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray& message) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 이 생성되거나 기존 P2P그룹에서 피어가 탈퇴시 이 메서드가 콜백됩니다.
		로컬 호스트 자신에 대해서도 이것이 호출됩니다.

		\param memberHostID 자기 또는 타 peer의 HostID입니다.
		\param groupHostID P2P 그룹의 HostID입니다.
		\param memberCount 처리된 후 멤버 수입니다.

		\~english
		This method is to be called back either when <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> is created or when a peer withdraws.
		Also called for local host iteself.

		\param memberHostID HostID of itself or other peer
		\param groupHostID HostID of P2P group
		\param memberCount The number of members after processed

		\~chinese
		<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%被生成或者peer从之前的P2P组退出时此方法会被回调。
		对本地主机本身，这个也会被呼叫。

		\param memberHostID 自己或者其他peer的HostID。
		\param groupHostID P2P组的HostID。
		\param memberCount 被处理后的成员数。

		\~japanese
		\ref p2p_groupが生成されるか、既存のP2Pグループでピーアが脱退時にこのメソッドがコールバックされます。
		ローカルホストの自身に対してもこれが呼び出されます。
		\param memberHostID の自分または他のpeerのHostIDです。
		\param groupHostID P2PグループのHostIDです。
		\param memberCount 処理された後のメンバー数です。
		\~
		*/
		virtual void OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount) = 0;

		/**
		\~korean
		타 클라이언트와의 P2P 통신 경로(릴레이 혹은 직접)가 바뀌는 순간 이 메서드가 호출됩니다.
		- 바뀐 후 호출되는 것이므로 CNetClient.GetPeerInfo 를 호출해서 CNetPeerInfo.Proud.CNetPeerInfo.m_RelayedP2P 의 값을 열람해도 됩니다.
		- 이것이 콜백되는 것과 상관없이 P2P간 서로 통신은 항상 가능합니다. 다만, 피어간 메시징이 릴레이를 하느냐 직접 이루어지느냐의 차이가 있을 뿐입니다.
		- ProudNet의 P2P 통신 경로에 대한 자세한 내용은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#robust_p2p" >ProudNet의 P2P 통신 성능</a> 를 참고하십시오.

		\param remoteHostID P2P 연결이 되어 있는 타 peer의 HostID
		\param reason P2P 연결이 성공한 경우에는 ErrorType_Ok 가 들어있다. 여타의 값인 경우는 '왜 연결이 relay로 바뀌었는지'를 담는다.

		\~english
		This method is called at the moment the connection status to other client via P2P (relay or not) changes.
		Since it is called after the change so it is ok to call CNetClient.GetPeerInfo to view the value of CNetPeerInfo.Proud.CNetPeerInfo.m_RelayedP2P.
		<a target="_blank" href="http://guide.nettention.com/cpp_en#robust_p2p" >P2P performance of ProudNet</a>

		\param remoteHostID HostID of other peer that is connected via P2P
		\param reason If the P2P connection is successful then contains ErrorType_Ok. For other values it contain 'why the connection changed to relay'.

		\~chinese
		与其他玩家的P2P通信路径（relay 或者直接）改变的瞬间，此方法会被呼叫。
		- 因为是改变以后被呼叫的，可以呼叫 CNetClient.GetPeerInfo%以后查看 CNetPeerInfo.Proud.CNetPeerInfo.m_RelayedP2P%的值。
		- 这个与回调无关，P2P 之间的通信是一直可行的。只是存在peer之间的messaging进行relay还是直接进行的区别。
		- 对ProudNet的P2P通信路径的详细内容请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#robust_p2p" >ProudNet 的P2P通信功能</a>%。

		\param remoteHostID P2P连接后的其他peer的Host ID。
		\param reason P2P连接成功的话，有ErrorType_Ok。其他值的话包含‘连接为什么改变成relay了’。

		\~japanese
		他のクライアントとのP2P通信経路(リレーまたは直接)が変わる瞬間、このメソッドが呼び出されます。
		- 変わった後に呼び出されますので、CNetClient.GetPeerInfoを呼び出してCNetPeerInfo.Proud.CNetPeerInfo.m_RelayedP2Pの値を閲覧しても良いです。
		- これがコールバックされるのと関係なくP2P間のお互いの通信はいつも可能です。但し、Peer間のメッセージングがリレーをするか、直接行われるのかの差があるだけです。
		- ProudNetのP2P通信経路に対する詳しい内容は、\ref robust_p2pをご参照ください。

		\param remoteHostID P2P接続ができている他のpeerのHostID
		\param reason P2P 接続に成功した場合には、ErrorType_Okが入っています。他の値の場合は、「どうして接続がリレーに変わったのか」を持ちます。
		\~
		*/
		virtual void OnChangeP2PRelayState(HostID remoteHostID, ErrorType reason) = 0;

		/**
		\~korean
		서버와의 연결이 끊겼을 때 콜백 됩니다.
		해당 이벤트가 콜백이 된 뒤, 서버에 재 접속이 이루어지지 않으면
		OnLeaveServer 가 콜백 됩니다.
		(해당 함수는 NetClient 의  NetConnectionParam.m_enableAutoConnectionRecovery 가 true 로 설정 되어야 합니다.)

		\param args 연결이 끊겼을 때 여러가지 정보를 담고 있습니다.

		\~english
		When connection to the server is lost, a callback is called. After the callback is called for a particular event, if reconnection to the server is not established, a callback occurs for OnLeaveServer.
		(The particular function shall have NetConnectionParam.m_enableAutoConnectionRecovery of NetClient as “True”)

		\param args When connection is lost, it has a variety of information.

		\~chinese
		与服务器连接中断时可以回调。相关事件回调后如果不能重新连接服务器 OnLeaveServer（离线服务器）则进行回调。
		（相关函数 NetClient（网络客户端）的 NetConnectionParam.m_enableAutoConnectionRecovery（网络连接程序_能够恢复自动连接）应该设定为 true（准确地）

		\param args (参量）在连接中端时包含了各种信息

		\~japanese
		サーバとの接続が切断されたときにコールバックされます。そのイベントがコールバックされた後、サーバに再接続が行われない場合はOnLeaveServerがコールバックされます。
		(その関数はNetClientのNetConnectionParam.m_enableAutoConnectionRecoveryをtrueに設定する必要があります。)

		\param args接続が切断されたとき、いくつかの情報を含んでいます。

		\~
		*/
		virtual void OnServerOffline(CRemoteOfflineEventArgs& /*args*/) {}

		/**
		\~korean
		서버와의 연결이 끊긴 뒤 재 접속이 이루어지면 콜백 됩니다.
		(해당 함수는 NetClient 의  NetConnectionParam.m_enableAutoConnectionRecovery 가 true 로 설정 되어야 합니다.)

		\param args 재 연결에 대한 정보를 담고 있습니다.

		\~english
		After connection to the server is lost and then established again, a callback occurs.
		(The particular function shall have NetConnectionParam.m_enableAutoConnectionRecovery of NetClient as “True”)

		\param args It has information on reconnection.

		\~chinese
		与服务器连接中断以后会进行重新连接并进行回调
		（相关函数 NetClient（网络客户端）的 NetConnectionParam.m_enableAutoConnectionRecovery（网络连接程序_能够恢复自动连接）应该设定为 true（准确地）

		\param args (参量）重新连接的相关信息

		\~japanese
		サーバとの接続が切断された後、再接続が行われるとコールバックされます。
		(その関数はNetClientのNetConnectionParam.m_enableAutoConnectionRecoveryをtrueに設定する必要があります。)

		\param args再接続に関する情報を含んでいます。

		\~
		*/
		virtual void OnServerOnline(CRemoteOnlineEventArgs& /*args*/) {}

		/**
		\~korean
		자신이 속한 P2P Group 내의 타 피어가 서버와의 연결이 끊겼을 때 콜백 됩니다.
		(해당 함수는 NetClient 의  NetConnectionParam.m_enableAutoConnectionRecovery 가 true 로 설정 되어야 합니다.)

		\param args 연결이 끊긴 피어의 정보가 담겨 있습니다.

		\~english
		When other peers within the P2P group where it belongs to lost connection to the server, a callback occurs.
		(The particular function shall have NetConnectionParam.m_enableAutoConnectionRecovery of NetClient as “True”)

		\param args It has information on the peers that have lost connection.

		\~chinese
		在自身所属的 P2P Group（P2P工作组）中如果其他同等 PEER（软件开发平台）与服务器连接中断时可以进行回调
		（相关函数 NetClient（网络客户端）的 NetConnectionParam.m_enableAutoConnectionRecovery（网络连接程序_能够恢复自动连接）应该设定为 true（准确地）

		\param args (参量）连接中断的PEER（软件开发平台）的相关信息

		\~japanese
		自分が属しているP2P Group内の他のピアがサーバとの接続が切断されたとき、コールバックされます。
		(その関数はNetClientのNetConnectionParam.m_enableAutoConnectionRecoveryをtrueに設定する必要があります。)

		\param args接続が切断されたピアの情報が含まれています。

		\~
		*/
		virtual void OnP2PMemberOffline(CRemoteOfflineEventArgs& /*args*/) {}

		/**
		\~korean
		자신이 속한 P2P Group 내의 타 피어가 서버와의 연결이 끊긴 뒤 재 접속이 이루어지면 콜백 됩니다.
		(해당 함수는 NetClient 의  NetConnectionParam.m_enableAutoConnectionRecovery 가 true 로 설정 되어야 합니다.)

		\param args 재 연결이 된 피어의 정보가 담겨 있습니다.

		\~english
		When other peers within the P2P group where it belongs to lost connection to the server and then gains reconnection, a callback occurs.
		(The particular function shall have NetConnectionParam.m_enableAutoConnectionRecovery of NetClient as “True”)

		\param args It contains information of the peer reconnected.

		\~chinese
		在自身所属的 P2P Group（P2P工作组）中如果其他同等 PEER（软件开发平台）与服务器连接中断时可以进行回调
		（相关函数 NetClient（网络客户端）的 NetConnectionParam.m_enableAutoConnectionRecovery（网络连接程序_能够恢复自动连接）应该设定为 true（准确地）

		\param args(参量）重新连接的PEER（软件开发平台）的相关信息

		\~japanese
		自分が属しているP2P Group内の他のピアがサーバとの接続が切断された後、再接続される場合にコールバックされます。
		(その関数はNetClientのNetConnectionParam.m_enableAutoConnectionRecoveryをtrueに設定する必要があります。)

		\param args再接続が行われたピアの情報が含まれています。

		\~
		*/
		virtual void OnP2PMemberOnline(CRemoteOnlineEventArgs& /*args*/) {}


		/**
		\~korean
		서버와의 UDP 통신이 정상적이냐의 여부가 바뀌는 순간 이 메서드가 호출된다.
		\param reason ErrorType_Ok 인 경우 서버와의 UDP 통신이 정상적으로 수행중이며 서버와의 unreliable RMI 메시징이 UDP를 경유한다.
		여타 값인 경우 서버와의 UDP 통신이 불가능하게 됐음을 의미하며 이 기간 동안 서버와의 unreliable RMI 메시징은 TCP를 경유한다.

		\~english
		This method is called at the moment that the communication status of UDP communication with server change.
		\param reason If ErrorType_Ok then UDP communication with server is normal and unreliable RMI messaging goes to server via UDP.
		Having other values means UDP communication witer server is unavailable and during this state, unreliable RMI messaging goes to server via TCP.

		\~chinese
		与服务器的UDP通信正常与否改变的瞬间，此方法被呼叫。
		\param reason ErrorType_Ok 的话是与服务器的UDP通信执行正常，与服务器的unreliable RMI messaging经由UDP。
		其他值的话意味着无法与服务器的UDP进行通信，这期间与服务器的unreliable RMI messaging经由TCP。

		\~japanese
		サーバーとのUDP通信が正常なのかどうかが変わる瞬間、このメソッドが呼び出されます。
		\param reason ErrorType_Ok の場合、サーバーとのUDP通信が正常に遂行中なので、サーバーとのunreliable RMIメッセージングがUDPを経由します。
		他の値の場合、サーバーとのUDP通信ができなくなったことを意味して、この期間中にサーバーとのunreliable RMIメッセージングはTCPを経由します。
		\~
		*/
		virtual void OnChangeServerUdpState(ErrorType /*reason*/) {}

		/**
		\~korean
		서버와의 시간이 동기화 될 때마다 호출된다.
		서버 시간 동기화는 이 메서드의 호출 횟수가 증가할수록 점차 정확도가 증가한다.
		이 메서드 내에서도 GetServerTimeMs()을 호출해도 된다.

		\~english
		Called every time when time is synchronized with server
		Server time sybchronization becomes more accurate with as frequency of calling this method increases.
		It is also possible to call GetServerTimeMs() within this metod.

		\~chinese
		每当与服务器的时间同步的时候被呼叫。
		服务器时间的同步化随着此方法的呼出次数增加而增加。
		在此方法内也可以呼出GetServerTime()。

		\~japanese
		サーバーとの時間が同期化される時ごとに呼び出されます。
		サーバー時間の同期化は、このメソッドの呼び出し回数が増加するほど徐々に正確度が増加します。
		このメソッド内でも GetServerTimeMs()を呼び出しても良いです。
		\~
		*/
		virtual void OnSynchronizeServerTime() = 0;
	};

	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
