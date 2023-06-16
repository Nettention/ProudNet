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

#include "INetCoreEvent.h"
#include "AddrPort.h"
#include "NetClientInfo.h"
#include "acr.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	//class CUserWorkerThreadCallbackContext;
	/** \addtogroup net_group
	*  @{
	*/

	/** 
	\~korean
	CNetServer 용 event sink

	용법
	- 이 객체를 구현한다.
	- CNetServer 에 파라메터로 전달한다.

	\~english
	event sink for CNetServer

	Usage
	- Realize this object
	- Pass to CLanServer as a parmeter.

	\~chinese
	CNetServer 用event sink

	用法
	- 体现此对象。
	- 用参数传送给 CNetServer%。

	\~japanese
	CnetServer用event sink

	用法
	- このオブジェクトを実現します。
	- CNetServerにパラメーターで伝達します。
	\~
	*/
	class INetServerEvent : public INetCoreEvent
	{
	public:
		virtual ~INetServerEvent() {}

		/** 
		\~korean
		클라이언트가 연결을 들어오면 발생하는 이벤트
		- OnConnectionRequest에서 진입 허가를 한 클라이언트에 대해서만 콜백된다.
		\param clientInfo 연결이 들어온 클라이언트

		\~english
		Event that occurs when client attempts to connect
		- Only called back for the client that received access permission from OnConnectionRequest  
		\param clientInfo client requested to connect

		\~chinese
		出现玩家的连接的话发生的event。
		- 只对在OnConnectionRequest允许进入的玩家回调。
		\param clientInfo 出现连接的玩家。

		\~japanese
		クライアントが接続されると発生するイベント
		- OnConnectionRequestで進入許可をしたクライアントに対してのみコールバックされます。
		\param clientInfo 接続されたクライアント
		\~
		*/
		virtual void OnClientJoin(CNetClientInfo *clientInfo) = 0;

		/** 
		\~korean
		클라이언트가 연결을 해제하면 발생하는 이벤트입니다.
		\param clientInfo 연결이 해제된 클라이언트의 정보입니다.
		\param errorInfo 연결이 해제된 클라이언트의 연결 해제 정보입니다. 
		ErrorInfo.m_type 값은 연결 해제의 경위를, ErrorInfo.
		자세한 것은 ErrorInfo 도움말에 있습니다.
		\param comment 클라이언트가 CNetClient.Disconnect 의 파라메터 comment 를 통해 보내온 문자열입니다.

		\~english
		Event that occurs when client terminates connection
		\param clientInfo client info that is disconnected
		\param errorInfo disconnection info of client that is disconneced 
		ErrorInfo.m_type value shows how disconnection processed
		Please refer ErrorInfo help section.
		\param comment text string sent by client via parameter comment of CNetClient.Disconnect

		\~chinese
		玩家解除连接的话发生的event。
		\param clientInfo 解除连接的玩家信息。
		\param errorInfo 解除连接玩家的连接解除信息。
		ErrorInfo.m_type 值表示连接解除的经纬。
		详细的在ErrorInfo帮助里。
		\param comment 玩家通过 CNetClient.Disconnect%的参数comment发过来的字符串。

		\~japanese
		クライアントが接続を解除すると発生するイベントです。
		\param clientInfo 接続が解除されたクライアントの情報です。
		\param errorInfo 接続の解除されたクライアントの接続解除情報です。 
		ErrorInfo.m_type 値は接続解除の経緯を、ErrorInfo。
		詳しくは、ErrorInfoのヘルプをご参照ください。
		\param comment クライアントが CNetClient.Disconnect のパラメーターコメントを通じ送られた文字列です。
		\~
		*/
		virtual void OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorinfo, const ByteArray& comment) = 0;

		/**
		\~korean
		클라이언트가 끊겼을 때 콜백 됩니다.
		해당 이벤트가 콜백이 된 뒤, 재 접속이 이루어지지 않으면
		OnClientLeave 가 콜백 됩니다.
		(해당 함수는 NetClient 의  NetConnectionParam.m_enableAutoConnectionRecovery 가 true 로 설정 되어야 합니다.)

		\param args 연결이 끊긴 클라이언트의 정보를 담고 있습니다.

		\~english
		A callback occurs when the client is disconnected.
		A callback occurs for OnClientLeave if reconnection does not occur.
		(The particular function shall have NetConnectionParam.m_enableAutoConnectionRecovery of NetClient as “True”)

		\param ars It contains information on a disconnected client.

		\~chinese
		网络客户端中断时回调
		相关事件回调以后，如果重新连接不能实现时 OnClientLeave（客户端离线）则进行回调。
		（相关函数 NetClient（网络客户端）的 NetConnectionParam.m_enableAutoConnectionRecovery（网络连接程序_能够恢复自动连接）应该设定为 true（准确地）

		\param args(参量）连接中断的客户端相关信息

		\~japanese
		クライアントが切断されたときにコールバックされます。
		そのイベントがコールバックされた後、再接続が行われない場合は
		OnClientLeaveがコールバックされます。
		(その関数はNetClientのNetConnectionParam.m_enableAutoConnectionRecoveryをtrueに設定する必要があります。)

		\param args接続が切断されたクライアントの情報を含んでいます。

		\~
		*/
		virtual void OnClientOffline(CRemoteOfflineEventArgs& /*args*/) {}

		/**
		\~korean
		클라이언트가 끊긴 뒤 재 접속 되었을 때 콜백 됩니다.
		(해당 함수는 NetClient 의  NetConnectionParam.m_enableAutoConnectionRecovery 가 true 로 설정 되어야 합니다.)

		\param args 재 연결이 된 클라이언트의 정보를 담고 있습니다.

		\~english
		A callback occurs when the client is disconnected and then reconnected.
		(The particular function shall have NetConnectionParam.m_enableAutoConnectionRecovery of NetClient as “True”)

		\param args It contains information on a reconnected client.

		\~chinese
		客户端中断后重新连接时进行回调
		（相关函数 NetClient（网络客户端）的 NetConnectionParam.m_enableAutoConnectionRecovery（网络连接程序_能够恢复自动连接）应该设定为 true（准确地）

		\param args(参量）重新连接的客户端的相关信息

		\~japanese
		クライアントが切断された後、再接続したときにコールバックされます。
		(その関数はNetClientのNetConnectionParam.m_enableAutoConnectionRecoveryをtrueに設定する必要があります。)

		\param args再接続が行われたクライアントの情報を含んでいます。
		\~
		*/
		virtual void OnClientOnline(CRemoteOnlineEventArgs& /*args*/) {}


		/** 
		\~korean
		클라이언트가 서버로 처음 연결을 시도하면 콜백되는 이벤트
		- 이 이벤트에서는 서버가 클라의 연결 시도를 수용할 것인지 거절할 것인지를 결정한다.
		만약 거절할 경우 클라이언트는 서버와의 연결이 실패하며 클라이언트는
		ErrorType_NotifyServerDeniedConnection 를 받게 된다.
		만약 수용할 경우 클라이언트는 서버와의 연결이 성공하게 되며 클라이언트는 새 HostID 를
		할당받게 된다. 이때 서버는 OnClientJoin 이벤트가 콜백된다.

		주의사항
		- 절대! 여기에서 유저의 로그온 정보(아이디,비밀번호)를 주고받지 말아야 한다.
		왜냐하면 여기서 클라이언트가 서버로 보내는 커스텀 데이터는 비 암호화되어 있기
		때문이다.

		\param clientAddr 클라이언트의 인터넷 주소
		\param userDataFromClient 클라이언트에서 보낸 커스텀 데이터. 이 값은 클라이언트에서 서버로 연결시
		넣었던 파라메터 CNetConnectionParam::m_userData 에서 넣었던 값이다.
		\param reply 이 이벤트 콜백에서 이 필드에 값을 채우면 클라이언트는 서버와의 연결 결과
		INetClientEvent::OnJoinServerComplete 에서 받게 된다.
		\return 클라이언트로부터의 연결을 수용할 것이라면 이 함수는 true를 리턴하면 된다. 

		\~english
		 Event called back when client attempts to connect to server for the first time
		- This event is where to decide if server accepts the attempt or not
		If refused then client fails to connect to server and client receives ErrorType_NotifyServerDeniedConnection.
		If accepted then client succeeds to connect to server and a new HostID will be allocated to client.
   		This is when OnClientJoin event is called back to server

		Note
		- NEVER! to send/receive user logon info(user account name and/or password) in here.
	 	  That is because the custom data from cliet to server is not encrypted.

		\param clientAddr Internet address of client
		\param userDataFromClient The custom data from client
					    This value is the value that was entered at parameter CNetConnectionParam::m_userData when client was connected to server.
		\param reply If this event callback fills a value to this field then client will receive the result of connection to server from INetClientEvent::OnJoinServerComplete.
		\return If to accept the connection reqeust from client then this function should return true.

		\~chinese
		玩家首次试图连接服务器时回调的event。
		- 在此event决定服务器容纳玩家的试图连接还是拒绝。
		如果拒绝的话，玩家与服务器的连接会失败，玩家会收到ErrorType_NotifyServerDeniedConnection。
		如果收纳的话，玩家与服务器的连接会成功，玩家会分配到新的Host ID。这时候服务器回调OnClientJoin event。

		注意事象
		- 绝对！在这里不能有对用户的登录信息（账户，密码）的来往。
		因为在这里玩家往服务器发送的custom数据是非加密的。

		\param clientAddr 玩家的互联网地址。
		\param userDataFromClient 玩家发送的custom数据。这个值是从玩家连接到服务器的时候输入的在参数 CNetConnectionParam::m_userData%输入的值。
		\param reply 如果在event的回拨中填充这个领域的值的话，玩家与服务器的连接结果从INetClientEvent::OnJoinServerComplete 接收。
		\return 要收纳从玩家的连接话，把此函数返回true即可。

		\~japanese
		クライアントがサーバーへ初めて接続を試すとコールバックされるイベントです。
		- このイベントではサーバーがクライアントの接続を受け入れるか拒絶するかを決定します。
		もし、拒絶する場合、クライアントはサーバーとの接続に失敗して、クライアントは
		ErrorType_NotifyServerDeniedConnection を受けるようになります。
		もし、受け入れる場合、クライアントはサーバーとの接続に成功して、クライアントは新しいHostIDを割り当ててもらうことになります。この時、サーバーはOnClientJoinイベントがコールバックされます。

		ご注意事項
		- 絶対、ここでユーザーのログオン情報(ID、パスワード)をやり取りしてはいけません。
		なぜなら、ここでクライアントがサーバーへ送るカスタムデータは非暗号化されているからです。

		\param clientAddr クライアントのインターネットアドレス
		\param userDataFromClient クライアントから送ったカスタムデータ。この値はクライアントよりサーバーへ接続時に入れたパラメーターのCNetConnectionParam::m_userDataで入れた値です。
		\param reply このイベントコールバックでこのフィールドに値を入れると、クライアントはサーバーとの接続結果、INetClientEvent::OnJoinServerCompleteで受けるようになります。
		\return クライアントからの接続を受け入れるなら、この関数はtrueをリターンします。 
		\~
		*/
		virtual bool OnConnectionRequest(AddrPort /*clientAddr*/, ByteArray& /*userDataFromClient*/, ByteArray& /*reply*/) {
			return true;
		}

		/** 
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 에 새 멤버가 추가되는 과정이 모든 클라이언트에서도 완료되면 발생하는 이벤트입니다.
		- 만약 그룹 G에 기 멤버 M1,M2,M3가 있다고 가정할 경우 신규 멤버 M4를 넣는다고 합시다.
		이때 JoinP2PGroup()에 의해 M4가 추가되더라도 즉시 완료되는 것은 아닙니다. 각 클라이언트
		M1,M2,M3에서는 아직 'M4의 추가되었음' 보고(INetClientEvent.OnP2PMemberJoin)를 받지 않았고 M4에서도 M1,M2,M3,M4 모두에 대한 '추가되었음 보고'(INetClientEvent.OnP2PMemberJoin)를 받지 않았기 때문입니다.
		이 이벤트는 M4에 대해, M1,M2,M3,M4에서 이들을 모두 받은 것을 서버에서 확인한 후에야 발생됩니다.
		- CreateP2PGroup에 대해서도 마찬가지로 이 이벤트가 발생합니다. 
		
		\param groupHostID \ref 그룹의 HostID
		\param memberHostID 그룹 멤버의 HostID
		\param result ErrorType_Ok 이면 성공적으로 추가됐음을 의미하며,ErrorType_AlreadyExists 이면 이미 그룹내 들어가있는 멤버임을 의미 합니다.(실패 했다는 의미는 아님)
		그외 다른 값이면 추가가 실패했음을 의미합니다. 
		예를 들어 CNetServer.CreateP2PGroup 혹은 CNetServer.JoinP2PGroup 호출 후에 이 콜백이 있기 전에
		CNetServer.LeaveP2PGroup 혹은 CNetServer.DestroyP2PGroup 를 호출 하거나, 해당 클라이언트가 동시에 서버와의 연결을 끊고 있는 중이었다면
		다른 값이 여기에 들어갈 수 있습니다. 

		\~english
		The event that occurs when the process adding a new member to <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> is completed in all client
		- Assuming that there are existing member M1, M2 and M3 in group G and we try to add a new member M4.
		Even if M4 is added by JoinP2PGroup(), the process is not to be completed immediately.
		This is because each client M1, M2 and M3 did not yet receive the report(INetClientEvent.OnP2PMemberJoin) that 'M4 is added',
		and M4 did not yet receive the report(INetClientEvent.OnP2PMemberJoin) that M4 is added to M1, M2, M3 and M4. 
		This event, for M4, occurs only after server acknowledges that M1, M2, M3 and M4 received necesary info.
		- The event also occurs for CreateP2PGroup. 
		
		\param groupHostID HostId of \ref group
		\param memberHostID HostID of group member
		\param result If ErrorType_Ok then it means successfully added, if ErrorType_AlreadyExists then it is a member that already in group. (does not mean it falied to)
		If has other values then it means failed to add. 
		For an example,
		Case1. Calling CNetServer.LeaveP2PGroup or CNetServer.DestroyP2PGroup between the moment after calling CNetServer.CreateP2PGroup or CNetServer.JoinP2PGroup and the moment before this callback occurs, OR  
		Case2. the client was terminating its connection to server at the same time,
		For each case, an other value can enter here.

		\~chinese
		往<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%添加新成员的过程在所有玩家中也结束的话会发生的event。
		- 如果假设组G里有原来的成员M1、M2、M3，添加新成员M4.
		这时即使被JoinP2PGroup()添加M4，不会立即完毕。因为每个玩家M1,M2,M3 并没有收到‘M4的添加’的报告（INetClientEvent.OnP2PmemberJoin），在M4也没有收到对M1、M2、M3、M4所有人的‘已添加’的报告（INetClientEvent.OnP2PMemberJoin）。
		此event对M4，在服务器确认M1、M2、M3、M4全部接收以后才会发生。
		- 也对CreateP2PGroup发生此event。

		\param groupHostID \ref 组的Host ID。
		\param memberHostID 组成员的Host ID。
		\param result ErrorType_Ok 的话意味着添加成功，如果是ErrorType_AlreadyExists的话意味着已是进入组内的成员。（不是失败了的意思）
		此外如添加其他值的话意味着失败。
		例如，呼叫 CNetServer.CreateP2PGroup%或者 CNetServer.JoinP2PGroup%，相关玩家同时中断与服务器的连接时候，其他值可以进入这里。

		\~japanese
		\ref p2p_groupに新しいメンバーが追加される過程が全てのクライアントでも完了されると発生するイベントです。
		- もし、グループGに既存メンバーM1,M2,M3がいると仮定する場合、新規メンバーM4を入れるとしましょう。
		この時、JoinP2PGroup()によってM4が追加されるとしても直ちに完了されるわけではありません。各クライアントM1,M2,M3ではまだ「M4が追加された」という報告(INetClientEvent.OnP2PMemberJoin)を受けていなく、M4でもM1,M2,M3,M4の全てに対する「追加された」という報告(INetClientEvent.OnP2PMemberJoin)を受けていないからです。
		このイベントはM4に対して、M1,M2,M3,M4でこれらを全て受けたものをサーバーで確認してから発生されます。
		- CreateP2PGroupに対しても同様にこのイベントが発生します。

		\param groupHostID \ref グループの HostID
		\param memberHostID グループメンバーの HostID
		\param result ErrorType_Ok であれば追加に成功したことを意味して、ErrorType_AlreadyExistsであれば既にグループ内に入っているメンバーであることを意味します。(失敗したという意味ではありません)
		その外の値であれば追加に失敗したことを意味します。
		例えば、CNetServer.CreateP2PGroup またはCNetServer.JoinP2PGroupを呼び出した後、このコールバックがある前に、CNetServer.LeaveP2PGroupまたはCNetServer.DestroyP2PGroupを呼び出すか、該当クライアントが同時にサーバーとの接続を切っている間でしたら、他の値がここに入ることができます。 

		\~
		*/
		virtual void OnP2PGroupJoinMemberAckComplete(HostID groupHostID, HostID memberHostID, ErrorType result) = 0;

		/** 
		\~korean
		user work thread pool의 스레드가 시작할 때 이 메서드가 호출된다.

		\~english
		This method is called when thread of user work thread pool starts.

		\~chinese
		user work thread pool的线程开始的时候，此方法会被呼叫。

		\~japanese
		user work thread poolのスレッドが開始する時にこのメソッドが呼び出されます。
		\~
		*/
		virtual void OnUserWorkerThreadBegin() = 0;

		/** 
		\~korean
		user work thread pool의 스레드가 종료할 때 이 메서드가 호출된다.

		\~english
		This method is called when thread of user work thread pool terminates.

		\~chinese
		user work thread pool的线程终止的时候，此方法会被呼叫。

		\~japanese
		user work thread poolのスレッドが開始する時にこのメソッドが呼び出されます。
		\~
		*/
		virtual void OnUserWorkerThreadEnd() = 0;

		/** 
		\~korean
		클라이언트가 해킹당했다는 의혹이 있을 때 이 메서드가 호출된다.
		\param clinetID 의심되는 client의 HostID
		\param hackType 해킹의 종류 

		\~english TODO:translate needed.
		This method is called when there is suspicion that the client has been hacked. 
		\param clinetID HostID of the client under suspicion
		\param hackType Type of hacking. 

		\~chinese
		疑惑玩家被黑的时候，此方法被呼叫。
		\param clinetID 可疑的client的Host ID。
		\param hackType 黑客种类。

		\~japanese
		クライアントがハッキングされたという疑いがある時にこのメソッドが呼び出されます。
		\param clinetID 疑われるclientのHostID
		\param hackType ハッキングの種類
		\~
		*/
		virtual void OnClientHackSuspected(HostID /*clientID*/,HackType /*hackType*/) {}

		/** 
		\~korean
		P2P group이 제거된 직후 이 메서드가 호출됩니다.
		- P2P group이 제거되는 조건은 Proud.CNetServer.DestroyP2PGroup 를 호출하거나 P2P group 에 소속된 마지막 멤버가
		서버와의 접속을 종료할 때 등입니다.

		\~english
		This method is called right after P2P group is removed. 
		- The condition P2P froup is removed is EITHER calling Proud.CNetServer.DestroyP2PGroup OR the last member of P2P group terminates its connection to server.

		\~chinese
		P2P group被删除之后，此方法被呼叫。
		- P2P group被删除的条件是呼叫 Proud.CNetServer.DestroyP2PGroup%或者与所属于P2P group的最后成员的连接终止等的时候。

		\~japanese
		P2P groupが除去された直後にこのメソッドが呼び出されます。
		- P2P groupが除去される条件は、Proud.CNetServer.DestroyP2PGroupを呼び出すか、P2P groupに所属された最後のメンバーがサーバーとの接続を終了する時などです。
		\~
		*/
		virtual void OnP2PGroupRemoved(HostID /*groupID*/) {}

		/** 
		\~korean
		RMI 호출 또는 이벤트 발생으로 인해 user worker에서 callback이 호출되기 직전에 호출됩니다.
		프로그램 실행 성능 측정을 위해 사용하셔도 됩니다.

		\~english
		Called right before calling callback by either RMI calling or user worker due to event occur. Also can be used for performance test purpose.

		\~chinese
		因为RMI呼出或者event的发生，在user worker中callback呼叫之前呼叫。
		也可以在测定程序运行性能时使用。

		\~japanese
		RMI呼び出しまたはイベントの発生によってuser workerでコールバックが呼び出される直前に呼び出されます。
		プログラムの実行性能測定のために使っても良いです。
		\~
		*/
		//virtual void OnUserWorkerThreadCallbackBegin(CUserWorkerThreadCallbackContext* ctx){}
		/** 
		\~korean
		RMI 호출 또는 이벤트 발생으로 인해 user worker에서 callback이 리턴한 직후에 호출됩니다.
		프로그램 실행 성능 측정을 위해 사용하셔도 됩니다.

		\~english
		Called right before calling callback by either RMI calling or user worker due to event occur. Also can be used for performance test purpose.

		\~chinese
		因为RMI呼出或者event的发生，在user worker中callback返回之后呼叫。
		也可以为了程序运行性能测定而使用。

		\~japanese
		RMI呼び出しまたはイベントの発生によってuser workerでコールバックがリターンした直後に呼び出されます。
		プログラムの実行性能測定のために使っても良いです。
		\~
		*/
		//virtual void OnUserWorkerThreadCallbackEnd(CUserWorkerThreadCallbackContext* ctx){}

		/** 
		\~korean
		일정 시간마다 user worker thread pool에서 콜백되는 함수입니다.  <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_timer_callback" >서버에서 타이머 루프 RMI 이벤트 처리하기</a>기능입니다. 		
		\param context CStartServerParameter.m_timerCallbackContext 에서 입력된 값과 동일합니다.

		\~english
		Function called back at user worker thread pool periodically. This is a <a target="_blank" href="http://guide.nettention.com/cpp_en#server_timer_callback" >Managing Timer loop RMI Event from server</a> function.		
		\param context same as the value entered at CStartLanServerParameter.m_timerCallbackContext

		\~chinese
		每隔一段时间在user worker thread pool回调的函数。<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_timer_callback" >在服务器处理Timer Loop，RMI，Event</a> 功能。
		\param context 与在 CStartLanServerParameter.m_timerCallbackContext%输入的值相同。

		\~japanese
		所定時間ごとにuser worker thread poolでコールバックされる関数です。\ref server_timer_callback 機能です。 
		\param context CStartServerParameter.m_timerCallbackContext で入力された値と同じです。
		\~
		*/
		//virtual void OnTick(void* context){}

	};

	/** 
	\~korean
	스레드 관련 콜백 인자 구조체입니다.
	아직은 제공되는 변수가 없습니다만, 하위호환성을 위해 미리 준비되어 있을 뿐입니다.

	\~english
	Callback factor structure that related with thread
	It does not supply any variable yet. It exists for lower compatibility.

	\~chinese
	是关于线程回调因子构造体。
	现在还没有被提供的变数，只是为了下级兼容性，事先准备了而已。

	\~japanese
	スレッド関連コールバック因子構造体です。
	まだ提供されない変数はありませんが、下位互換性のために前もって準備されているだけです。
	\~
	*/
// 	class CUserWorkerThreadCallbackContext
// 	{
// 	};
	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
