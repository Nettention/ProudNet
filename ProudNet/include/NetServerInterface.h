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

#include "BasicTypes.h"
#include "HostIDArray.h"
#include "AddrPort.h"
#include "Enums.h"
#include "EncryptEnum.h"
#include "RMIContext.h"
#include "ErrorInfo.h"
#include "ServerParam.h"
#include "NetServerStats.h"
#include "NetClientInterface.h"
#include "LambdaEventHandler.h"
#include "acr.h"

#define PN_DEPRECATED /*__declspec(deprecated) */

#ifdef _MSC_VER
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
	class CNetClientInfo;
	class CP2PGroup;
	class CP2PGroups;
	class CSendFragRefs;
	class ErrorInfo;
	class INetClientEvent;
	class INetServerEvent;
	class IRmiProxy;
	class IRmiStub;
	class ReliableUdpHostStats;
	struct SendOpt;





	/**
	\~korean
	\brief GetSuitableSuperPeerRankListInGroup에서 채워지는 배열의 각 원소입니다.

	\~english
	\brief Each element of array filled at GetSuitableSuperPeerRankListInGroup

	\~chinese
	\brief 在GetSuitableSuperPeerRankListInGroup填充的数据各元素。

	\~japanese
	\brief GetSuitableSuperPeerRankListInGroupで満たされる配列の各元素です。
	\~
	*/
	struct SuperPeerRating
	{
		/**
		\~korean
		P2P 그룹 내의 피어 ID 입니다.

		\~english
		Peer ID in P2P group

		\~chinese
		P2P组内的peerID。

		\~japanese
		P2Pグループ内のPeer IDです。
		\~
		*/
		HostID m_hostID;

		/**
		\~korean
		이 피어 ID가 수퍼피어 적격 검사에서 얻은 점수입니다.
		이 항목이 포함된 배열의 다른 원소와 비교했을때 값이 클수록 수퍼피어로서의 적격 수준이 높음을 의미합니다.

		\~english
		Result from superpeer qualified testing by this peer ID.
		If value is bigger than others, it means, it is close to qualification of superpeer

		\~chinese
		此peer ID在superpeer的合格检查中获得的分数。
		与包含此项目的数组的其他元素相比较的话，此值越大意味着以superpeer的资格水准越高。

		\~japanese
		このPeer IDがSuper Peer適格検査から得た点数です。
		この項目が含まれた配列の他の元素と比較した時、値が大きいほどSuper Peerとしての適格レベルが高いことを意味します。

		\~
		*/
		double m_rating;

		/**
		\~korean
		서버와의 UDP 통신이 성사되고 있으면 true입니다. 서버와의 UDP가 되어야 원활한 P2P 통신을 기대할 수 있습니다.

		\~english
		If it communicates to server with UDP, it is true. You can expect reliable P2P if it communicates with UDP

		\~chinese
		与服务器的UDP通信成功的话true。与服务器的UDP成功才能期待顺利的P2P通信。

		\~japanese
		サーバーとのUDP通信に成功したらtrueです。サーバーとのUDPができてこそ円滑なP2P通信を期待することができます。
		\~
		*/
		bool m_realUdpEnabled;

		/**
		\~korean
		공유기 뒤에 있는 호스트이면 true입니다. 공유기가 없는 호스트인 경우 원활한 수퍼피어 역할을 기대할 수 있습니다.

		\~english
		If it locates behind of router then it is true. You can expect relialbe superpeer when host does not use router

		\~chinese
		路由器后面的主机的话是true。没有路由器的主机的话不能期待superpeer的作用。

		\~japanese
		ルーターの後ろにあるホストならtrueです。ルーターのないホストの場合、円滑なSuper Peer役割を期待することができます。
		\~
		*/
		bool m_isBehindNat;

		/**
		\~korean
		최근 클라이언트-서버간 레이턴시입니다. 밀리초 단위입니다. 클수록 수퍼피어로서의 역할을 기대하기 어렵습니다.

		\~english
		Latency between client to server. It is a second unit. If it is high, do not except working as superpeer

		\~chinese
		最近client-服务器之间latency。毫秒单位。越大越难期待作为superpeer的作用。

		\~japanese
		最近、クライアントサーバー間のレーテンシーです。ミリ秒単位です。大きいほどSuper Peerとしての役割を期待しにくいです。
		\~
		*/
		int m_recentPingMs;

		/**
		\~korean
		타 피어와의 평균 레이턴시입니다. 밀리초 단위입니다. 클수록 수퍼피어로서의 역할을 기대하기 어렵습니다.

		\~english
		Average latency with other peers. It is a second unit. If it is high, do not except working as superpeer

		\~chinese
		与其他peer的平均latency。毫秒单位。越大越难期待作为superpeer的作用。

		\~japanese
		他のPeerとの平均レーテンシーです。ミリ秒単位です。大きいほどSuper Peerとしての役割は期待し難いです。
		\~
		*/
		int64_t m_P2PGroupTotalRecentPingMs;

		/**
		\~korean
		송신 최대 가능 속도입니다.
		부동소수점 타입이지만, 바이트 단위입니다. (1 미만의 통신량도 체크하기 위해서) 클수록 수퍼피어로서의 역할을 기대할 수 있습니다.

		\~english
		Maximum sending speed.
		It is a floating-point type but byte. (to check traffic that under 1) If it is high, you can except working as superpeer

		\~chinese
		传送信息最大可能速度。
		浮点小数点类型，byte单位。（为了也要检查未满1的通信量）越大可以期待作为superpeer的作用。

		\~japanese
		送信最大可能速度です。
		不動小数点タイプですが、バイト単位です。(1未満の通信量もチェックするために)大きいほどSuper Peerとしての役割を期待することができます。

		\~
		*/
		double m_sendSpeed;

		/**
		\~korean
		P2P 그룹에 들어온 후 지난 시간입니다.
		이미 다른 피어가 수퍼피어일때 이 피어가 수퍼피어로 전환되는건 성급할 수 있습니다.

		\~english
		Past time that after joined P2P group.
		If other peer is already superpeer it may hasty to change superpeer

		\~chinese
		进入P2P组后经过的时间。
		已经是其他peer为superpeer的时候，此peer转换为superpeer可能会有点性急。

		\~japanese
		P2Pグループに入ってから過ぎた時間です。
		既に他のPeerがSuper Peerである時、このPeerがSuper Peerに切り替わることはちょっと速いかも知れません。

		\~
		*/
		int64_t m_JoinedTime;

		/**
		\~korean
		사용자에 의해 입력된 초당 프레임 갯수(frame rate)입니다. Proud.CNetClient.SetApplicationHint 에서 입력됩니다.

		\~english
		Frame rate (second) that entered by user. It enters at Proud.CNetClient.SetApplicationHint

		\~chinese
		被用户输入的每秒帧的个数（frame rate）。在 Proud.CNetClient.SetApplicationHint%里输入。

		\~japanese
		ユーザーによって入力された秒当たりフレーム数(frame rate)です。Proud.CNetClient.SetApplicationHintで入力されます。
		\~
		*/
		double m_frameRate;
	};

	/**
	\~korean
	\brief 수퍼 피어 선정 정책

	\~english
	\brief super peer selection policy

	\~chinese
	\brief superpeer选定政策。

	\~japanese
	\brief Super Peer選定ポリシー
	\~
	*/
	class CSuperPeerSelectionPolicy
	{
	public:

		/**
		\~korean
		서버와의 UDP 통신이 켜져있을 때 가산점되는 가중치. 압도적으로 가중치가 매우 커야 한다.

		\~english
		The weight added when UDP communication to server is on. This weight must be overwhelming.

		\~chinese
		与服务器的UDP通信开启的时候加分的加重值。加重值要压倒性的大。

		\~japanese
		サーバーとのUDP通信が大きくなっている時、加算点になる加重値。圧倒的に加重値が大きくなければなりません。
		\~
		*/
		double m_realUdpWeight;
		/**
		\~korean
		인터넷 공유기 없는, 즉 공인 IP를 직접 사용하는 클라이언트에 가산점되는 가중치. 홀펀칭 유지율이 높으므로 가중치를 높게 잡는 편이 좋다.

		\~english
		The weight added to client that directly uses public IP - which means using no internet router. It is recommended to set this as relatively high since hole-punching rate is kept high.

		\~chinese
		没有路由器的，即给直接使用公认IP的client成为加分的加重值。打洞维持率高，所以设置加重值高一点为好。

		\~japanese
		インターネットルーターのない、即ちパブリックIPを直接使うクライアントに加算点になる加重値。ホールパンチングの維持率が高いので加重値を高く取った方が良いです。
		\~
		*/
		double m_noNatDeviceWeight;
		/**
		\~korean
		서버와의 레이턴시 1초 분량에 대해 감점되는 가중치. 수퍼피어가 서버와의 통신을 자주 하는 경우 가중치를 크게 잡는 것이 좋다.

		\~english
		The weight added for each second of time of latency to server. If a super peer is communicating with server often then this must be set high.

		\~chinese
		与服务器的latency一秒分量减分的加重值。如果superpeer经常与服务器通信的话设置加重值高一点为好。

		\~japanese
		サーバーとのレーテンシー1秒分量に対して減点になる加重値。Super Peerがサーバーとの通信をよくやる場合、加重値を大きく取った方が良いです。
		\~
		*/
		double m_serverLagWeight;
		/**
		\~korean
		타 피어 1개와의 레이턴시 1초 분량에 대해 감점되는 가중치. m_serverLagWeight 보다 작게 잡더라도 피어의 갯수가 증가할수록 대 서버 가중치보다 증가한다.
		이 값은 CStartServerParameter.m_enablePingTest 값을 true로 설정하지 않으면, 계산되지 않습니다.

		\~english
		The weight to be deducted for each second of time of latency to other peer. This increases as the number of peers increase even if set smaller than m_serverLagWeight.
		It is not calculated, if you do not set CStartServerParameter.m_enablePingTest as true.

		\~chinese
		对其他peer一个的latency一秒分量减分的加重值。即使比m_serverLagWeight设置的小，peer 的个数越增加也会比大服务器加重值增加。
		不把 CStartServerParameter.m_enablePingTest%值设置成true的话，此值不会被计算。

		\~japanese
		他のPeer 1個とのレーテンシー1秒分量に対して減点される加重値。 m_serverLagWeightより小さく取ってもPeerの数が増加するほど対サーバー加重値より増加します。
		CStartServerParameter.m_enablePingTest 値をtrueに設定しなければ計算されません。

		\~
		*/
		double m_peerLagWeight;
		/**
		\~korean
		송신 속도 10Mbps에 대해 가산점되는 가중치. 가정용 인터넷에서는 업로드 속도가 느린 편이기 때문에 이 가중치가 필요할 수도 있다.

		\~english
		The weight added to transmission speed of 10Mbps. This weight may be needed as home internet connection has relatively low spees for uploading.

		\~chinese
		对传送速度10Mbps加分的加重值。因为家庭用互联网上传速度偏慢，可能会需要此加重值。

		\~japanese
		送信速度10Mbpsに対して加算点になる加重値。家庭用インターネットではアップロード速度が遅いので、この加重値が必要になるかも知れません。
		\~
		*/
		double m_sendSpeedWeight;
		/**
		\~korean
		사용자가 측정한 프레임레이트와의 가중치입니다.
		- 프레임 레이트 값 1에 비례해서 이 값이 증가합니다. 예를 들어 60프레임을 잘 유지하면 수퍼피어로서 적격이고 20프레임 이하이면
		수퍼피어로서 부적합하게 평가하고자 한다면, 4를 넣을 경우 20프레임 * 4 = 80, 60프레임*4 = 240, 즉 160의 점수차가 나게 됩니다.
		- 측정된 프레임 레이트는 Proud.CNetClient.SetApplicationHint 를 통해 입력하십시오.

		\~english
		The weight to the frame rate measured by user
		- This value increases proportional to frame rate value 1. For an example, if assumed that sustaining 60 fps is regarded as an ideal super peer and lower than 20 fps as to be avoided,
		then entering 4 gives 20 fps * 4 = 80, 60 fps * 4 = 240 resulting 160 points in difference.
		- Measured frame rate is to be entered via Proud.CNetClient.SetApplicationHint.

		\~chinese
		用户测定的与帧率的加重值。
		- 与帧率值1相比，此值会增加。例如想评价为很好的维持60帧的话，作为superpeer是合格的，20帧一下的话是不合格的话，输入4的话20帧*4=80，60帧*4=240，即会相差160分。
		- 测定的帧率请通过 Proud.CNetClient.SetApplicationHint%输入。

		\~japanese
		ユーザーが測定したフレームレートとの加重値です。
		- フレームレート値1に比例してこの値が増加します。例えば、60フレームをよく維持すればSuper Peerとして適格で、20フレーム以下であればSuper Peerとして不適合に評価するとしたら、4を入れる場合、20フレーム*4=80、60フレーム*4=240、即ち、160の点数差が出るようになります。
		- 測定されたフレームレートは、Proud.CNetClient.SetApplicationHintを通じ入力してください。

		\~
		*/
		double m_frameRateWeight;

		/**
		\~korean
		P2P 그룹에 참가된 클라이언트가 참가한 후 지난 시간이 여기서 지정한 시간 이하이면 수퍼피어 선정에서 거의 배제한다. 밀리초 단위다.
		- 0으로 지정하면 이러한 배제를 하지 않는다.
		- 통상적으로 잦은 수퍼피어 변경을 막기 위해 값을 지정한다.

		\~english
		To be disregarded as super peer when the time from the moment that the client participated to the P2P group to now is less than the time designated here. This is in millisecond.
		- Set as 0 then the disregard will not occur.
		- Usually set to prevent super peer eing changed too often.

		\~chinese
		参与P2P组的client参加后经过的时间是在这里指定时间以下的话，从superpeer选定里排除。毫秒单位。
		- 指定为0的话不会做这种排除。
		- 一般为了避免频繁的superpeer改变而指定此值。

		\~japanese
		P2Pグループに参加したクライアントが参加し後の経過時間がここで指定した時間以下であればSuper Peer選定でほぼ排除します。ミリ秒単位です。
		- 0に指定すればこのような排除をしません。
		- 通常、頻繁なSuper Peerの変更を防ぐために値を指定します。

		\~
		*/
		int64_t m_excludeNewJoineeDurationTimeMs;

		PROUDSRV_API 		 CSuperPeerSelectionPolicy();

		inline bool operator==(const CSuperPeerSelectionPolicy& rhs) const
		{
			return m_realUdpWeight == rhs.m_realUdpWeight &&
				m_noNatDeviceWeight == rhs.m_noNatDeviceWeight &&
				m_serverLagWeight == rhs.m_serverLagWeight &&
				m_peerLagWeight == rhs.m_peerLagWeight &&
				m_sendSpeedWeight == rhs.m_sendSpeedWeight &&
				m_frameRateWeight == rhs.m_frameRateWeight;
		}

		/**
		\~korean
		일반적으로 쓰이는 유형으로 값을 미리 설정해서 리턴한다.

		\~english
		Previously prepared type are set and then returned.

		\~chinese
		根据一般使用的类型，事先设置值后返回。

		\~japanese
		一般的に使われる類型で値を前もって設定してリターンします。
		\~
		*/
		PROUDSRV_API static CSuperPeerSelectionPolicy GetOrdinary();

		/**
		\~korean
		쓰지 말것

		\~english
		DO NOT USE THIS.

		\~chinese
		不要使用。

		\~japanese
		使わないこと。
		\~
		*/
		PROUDSRV_API 		static CSuperPeerSelectionPolicy GetNull();
	};

	/**
	\~korean
	\brief P2P 연결에 대한 통계입니다.

	\~english
	\brief Statistics to P2P connection

	\~chinese
	\brief 对P2P连接的统计。

	\~japanese
	\brief P2P接続に対する統計です。
	\~
	*/
	class CP2PConnectionStats
	{
	public:
		/**
		\~korean
		전체 연결 P2P쌍의 갯수입니다.

		\~english
		Number of all connected P2P pairs

		\~chinese
		全部P2P连接的一对个数。

		\~japanese
		全体接続P2Pペアの数です。
		\~
		*/
		uint32_t m_TotalP2PCount;

		/**
		\~korean
		직접 연결 P2P쌍의 갯수입니다.

		\~english
		Number of directly connected P2P pairs

		\~chinese
		直接P2P连接的一对个数

		\~japanese
		直接接続P2Pペアの数です。
		\~
		*/
		uint32_t m_directP2PCount;

		/**
		\~korean
		해당 클라이언트가 다른 클라이언트에게 Udp packet 전송을 시도한 총 횟수
		- CNetServer 에서만 값이 채워집니다. ( CLanServer 에서는 udp를 사용안하므로. )
		- group의 CP2PConnectionStats 라면 모든 그룹원의 횟수가 합산됩니다.

		\~english
		Total attempted number of Udp packet sending from client to other client.
		- Value will fill at only CNetServer (Because CLanServer does not use udp.)
		- All group members attempt will be added, if it is CP2PConnectionStats of group.

		\~chinese
		从相关client往其他client试图传送UDP packet的总次数。
		- 只在 CNetServer%值被填充。（因为在 CLanServer%不使用UDP）
		- group的 CP2PConnectionStats%的话所有组员的次数都会被合算。

		\~japanese
		該当クライアントが他のクライアントにUDPパケットの転送を試した総回数
		- CNetServer でのみ値が満たされます。 (CLanServerではUDPを使用しませんので)
		- groupのCP2PConnectionStatsなら全てのグループメンバーの回数が合算されます。

		\~
		*/
		uint32_t m_toRemotePeerSendUdpMessageTrialCount;

		/**
		\~korean
		해당 클라이언트가 다른 클라이언트에게 Udp packet 전송을 성공한 총 횟수
		- CNetServer 에서만 값이 채워집니다. ( CLanServer 에서는 udp를 사용안하므로. )
		- group의 CP2PConnectionStats 라면 모든 그룹원의 횟수가 합산됩니다.

		\~english
		Total successful number of Udp packet sending from client to other client
		- Value will fill at only CNetServer (Because CLanServer does not use udp.)
		- All group members attempt will be added, if it is CP2PConnectionStats of group.

		\~chinese
		从相关client往其他client成功传送UDP packet的总次数。
		- 只在 CNetServer%值被填充。（因为在 CLanServer%不使用UDP）
		- group的 CP2PConnectionStats%的话所有组员的次数都会被合算。

		\~japanese
		該当クライアントが他のクライアントにUDPパケットの転送に成功した総回数
		- CNetServer でのみ値が満たされます。(CLanServerではUDPを使用しませんので)
		- groupのCP2PConnectionStatsであれば全てのグループメンバーの回数が合算されます。

		\~
		*/
		uint32_t m_toRemotePeerSendUdpMessageSuccessCount;
	};

	/**
	\~korean
	\brief RemoteA 와 RemoteB 사이의 Udp 메시징 시도와 성공갯수입니다.

	\~english
	\brief Number of attempt and success Udp messaging between RemoteA and Remote B.

	\~chinese
	\brief RemoteA和RemoteB之间UDP messaging试图和成功个数。

	\~japanese
	\brief RemoteAとRemoteB間のUDPメッセージングの試しと成功数です。
	\~
	*/
	class CP2PPairConnectionStats
	{
	public:
		/**
		\~korean
		A가 B에게 시도한 Udp 갯수

		\~english
		Number of attempted Udp from A to B

		\~chinese
		A给B试图的UDP个数。

		\~japanese
		AがBに試したUDP数
		\~
		*/
		uint32_t m_toRemoteBSendUdpMessageTrialCount;

		/**
		\~korean
		A가 B에게 성공한 Udp 갯수

		\~english
		Number of succeed Udp  from A to B

		\~chinese
		A给B成功的UDP个数。

		\~japanese
		AがBに成功したUDP数
		\~
		*/
		uint32_t m_toRemoteBSendUdpMessageSuccessCount;

		/**
		\~korean
		B가 A에게 시도한 Udp 갯수

		\~english
		Number of attempted Udp from B to A

		\~chinese
		B给A试图的UDP个数。

		\~japanese
		BがAに試したUDP数
		\~
		*/
		uint32_t m_toRemoteASendUdpMessageTrialCount;

		/**
		\~korean
		A가 B에게 시도한 Udp 갯수

		\~english
		Number of attempted Udp from A to B

		\~chinese
		A给B试图的UDP个数。

		\~japanese
		AがBに試したUDP数
		\~
		*/
		uint32_t m_toRemoteASendUdpMessageSuccessCount;

		/**
		\~korean
		릴레이 통신을 하고 있으면 true입니다.

		\~english
		true if relayed mode.

		\~chinese
		如果已是进行Relay通信的状态则为true。

		\~japanese
		リレー通信をしていればtrueです。
		\~
		*/
		bool m_isRelayed;
	};

	/**
	\~korean
	\brief 게임용 네트워크 서버

	게임용 네트워크 서버입니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_overview" >ProudNet 서버</a> 에 있습니다.
	- MMO 게임에도 쓸 수 있다.
	- CNetClient 의 연결을 받는다.
	- 클라이언트에게 HostID 를 배정해주는 역할을 한다.
	- HostID 를 매개체로 하는 RMI를 가능하게 한다.
	- 일반적인 게임 서버에 필요한 기본적 옵션이 설정되어 있다.
	- 클라이언트간 P2P 연결을 지원한다. 이 서버 객체는 클라이언트간 P2P 연결이 실패할 경우
	릴레이 서버 역할도 지원한다.

	일반적 용법
	- 이 객체를 생성한다. Proud.CNetServer.Create() 사용.
	- Start()로 서버를 시작한다.
	- INetServerEvent 를 구현한다. 가령 INetServerEvent.OnClientJoin() 등을 구현한다.
	이것들은 클라이언트 접속 등에 대한 이벤트 핸들러이다.
	- 시작시 SetEventSink()로 이벤트 핸들러를 엮어주고,
	AttachProxy(), AttachStub()를 써서 1개 이상의 송신 또는
	수신용 RMI 인터페이스를 연계한다.

	P2P 연결 그룹 만들기
	- CNetServer 에 연결된 클라이언트는 1개 이상의 P2P 그룹을 만들 수 있다.
	P2P 그룹으로 묶여있는 클라이언트끼리는 HostID 를 인자로 RMI 를 직접 주고 받을 수 있다.
	- 새 P2P group을 생성/제거하기: 서버만이 권한이 있다.
	CreateP2PGroup(),DestroyP2PGroup() 를 통해서. 클라이언트에서는 OnP2PMemberJoin 등이 호출된다.
	- peer 를 기 존재 P2P group에 추가/제거하기:
	JoinP2PGroup(), LeaveP2PGroup(). 클라이언트에서는 OnP2PMemberJoin 등이 호출된다.

	Relay server로서의 역할
	- HostID 를 쓰는한 P2P 연결은 가상으로도 이루어진다. 예컨대 P2P 연결이 실제로
	아직 성공하지 못한 peer간이라도 상대가 P2P group에 속해있으면 상대의 HostID
	로 보내는 RMI는 성공적으로 상대에게 간다. relay server가 중도 역할을 담당한다.
	같은 RMI 수신 메시지라도 그것이 relay server를 경유했는지 여부를 얻으려면 RMI 수신부에서
	RmiContext.m_relayed 를 읽으면 된다.

	스레드 풀링
	- 기본적으로 스레드 풀을 사용한다. 따라서 서버 시작시 스레드 수를 최소 1개를 지정해야 한다.
	- 스레드 풀링 기능을 끄는게 유리한 경우: 1개 CPU이면서 CPU time 처리가 전부인 서버(예컨대 릴레이 서버)
	- 스레드 풀링 기능을 켜는게 유리한 경우: 멀티 CPU 또는 device time 처리가 잦은 서버(예컨대 MMO 게임 서버)
	- 스레드 풀 함수는 CoInitialize()가 준비된 상태에서 호출되므로 또 CoInitialize()나 CoUninitialize() 호출 불필요.

	UDP 호스트 포트 갯수 관련 서버 성능
	- CNetServer.Start 의 서버 포트 파라메터로 UDP 호스트 포트 갯수를 지정할 수 있다.
	클라이언트는 서버로 연결할 때 파라메터에 있는 포트 중 하나로 서버에 연결된다.

	P2P 직접 통신 성공 가능성 노트
	- P2P간 한쪽이 real IP, Full-cone NAT인 경우 direct P2P 통신이 성공한다.
	- 게임 서버가 NAT 뒤에서 작동할 경우, Start()에 서버 주소를 넣어야 한다. (자세한 것은 Start()의 설명 참조)
	단, 이렇게 한다 하더라도 게임 서버 앞의 NAT 뒤에 있는 또 다른 호스트에서 실행되는 클라이언트와 외부 인터넷
	의 클라이언트간 직접 P2P 통신은 실현되지 못한다. 서버에 있는 NAT 장치에 의해 외부 인터넷 주소 대신 NAT의 내부
	IP가 인식되기 때문이다.

	\~english
	\brief Gaming network server

	A network server for gaming. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#server_overview" >ProudNet Server</a>.
	- Can be used for a MMO title
	- Receives CNetClient connection
	- Allocates HostID to client
	- Allows RMI requires HostID
	- Basic options are set in order to support what a general game server needs
	- Supports P2P connection between clients. This server object supports to be a rela server when P2P connection fails between clients.

	General usage
	- Creates this object. Use Proud.CNetServer.Create().
	- Use Start() to start server.
	- Realize INetServerEvent, such as realizing INetServerEvent.OnClientJoin() and so on.
	Those are event handlers for client connections and so on.
	- Intertwines SetEventSink() to event andlers when started and links 1 or more of transmission or reception RMI interface using AttachProxy(), AttachStub().

	Creating P2P connection group
	- The client connected to CNetServer can create 1 or more P2P group.
	Among the clients linked as P2P group can transmit/receive RMI as HostID being index.
	- Creating/removing a new P2P group: Only server has rights.
	OnP2PMemberJoin and so on will be called at client via CreateP2PGroup(),DestroyP2PGroup().
	- Adding/removig peer to/from  existing P2P group: JoinP2PGroup(), LeaveP2PGroup(). OnP2PMemberJoin and so on will be called at client.

	Roles as relay server
	- As long as using HostID, P2P connection occurs virtually as well.
	For an example, even if the opponent is a member of P2P group, between clients that P2P connection was not successful in real then RMI is successfully sent to opponent's HostID.
	Relay server manages the carrying.
	Tough for same RMI reception message, it is to read RmiContext.m_relayed at RMI read part to get whether went through relay server.

	Thread pooling
	- Basically uses thread pool. So there must be at least 1 or more thread when server starts.
	- When turning thread pooling off is better: server with 1 CPU and all it does is CPU time process (e.g. relay server)
	- When turning thread pooling on is better: server with multip CPU or server that has often device time process (e.g. MMO game server)
	- Since thread pool function is called when CoInitialize()is ready there is no need to call CoInitialize() or CoUninitialize().

	Server performance related to number of UDP host port
	- Can designate number of UDP host port as server port parameter of CNetServer.Start
	Client will be connected to one of the ports among parameter when connected to server.

	Success rate of P2P direct communication note
	- Direct P2P communication will be successful when an end of P2P is real IP, Full-cone NAT.
	- When game server operatedbehind NAT it is necessary to enter server address to Start(). (Please refer Start() manual.)
	However, there will be no direct P2P communication between the client run by other host that is in front of game server and behind of NAT and the client of outer internet.
	This is because the internal IP of NAT will be recognized other than outer internet address by NAT device in server.

	\~chinese
	\brief 游戏用网络服务器。

	游戏用网络服务器。详细的在<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_overview" >ProudNet 服务器</a>%里。
	- 也可以在MMO游戏使用。
	- 接收 CNetClient%的连接。
	- 起着给client分配Host ID的作用。
	- 让RMI可以以Host ID为媒体。
	- 设置着一般游戏服务器需要的基本选项。
	- 支持client之间P2P连接。此服务器对象在client之间P2P连接失败的时候还支持relay服务器的作用。

	一般使用方法
	- 生成此对象。使用 Proud.CNetServer.Create()。
	- 用Start()开始服务器。
	- 体现INetServerEvent。例如体现 INetServerEvent.OnClientJoin()等。
	这些是对client连接等的event handler。
	- 开始时用SetEventSink()连event handler，使用AttachProxy(), AttachStub()连接一个以上的传送或者收信用RMI界面。

	创建P2P连接组。
	- 连接 CNetServer%的client可以创建一个以上的P2P组。
	组成P2P组的client之间可以以Host ID为因子直接交换RMI。
	- 生成/删除新P2P group：只有服务器有权力。
	通过CreateP2PGroup(),DestroyP2PGroup()，在client呼叫OnP2PmemberJoin等。
	- 把peer往已存在P2P group添加/删除：
	JoinP2PGroup(), LeaveP2PGroup()。在client呼叫OnP2PMemberJoin等。

	Relay server的作用
	- 既然使用Host ID，P2P连接会虚拟实现。例如即使是P2P连接实际上还没有成功的Peer之间，对方属于P2P group的话发送到对方Host ID的RMI会成功到传送给对方。Relay server在中间担任作用。即使是相同的接收信息，想知道它是否经由relay server的话在RMI收信部读 RmiContext.m_relayed%即可。

	线程pooling
	- 一般都使用线程pool。所以服务器开始时至少要指定一个线程数。
	- 关闭线程pool功能为有利的情况：是一个CPU而且CPU time处理是全部的服务器（例如relay服务器）。
	- 开启线程pool功能为有利的情况：多CPU或者device time处理频繁的服务器（例如MMO游戏服务器）。
	- 线程pool函数在CoInitialize()准备的状态下呼叫，所以不需要CoInitialize()或者CoUninitialize()

	UDP主机端口个数相关服务器性能。
	- 用 CNetServer.Start%的服务器端口参数可以指定UDP主机端口个数。
	Client与服务器连接的时候用参数里存在的端口之一与服务器进行连接。

	P2P直接通信成功可能性记录。
	- P2P之间一方real IP, Full-cone NAT 的时候direct P2P通信成功。
	- 游戏服务器在NAT后面运转的时候要往Start()输入服务器地址。（详细请参考Start()说明）
	但即使这样，游戏服务器前面的NAT在其后面的其他主机中实行的client和外部互联网client之间的直接通信却不会实现。 因为根据服务器的NAT装置，被识别的不是外部互联网地上，而是内部IP。

	\~japanese
	\brief ゲーム用ネットワークサーバー
	ゲーム用ネットワークサーバーです。詳しくは、\ref server_overviewにあります。
	- MMO ゲームにも使えます。
	- CNetClient からの接続を受けます。
	- クライアントにHostIDを割り当てる役割をします。
	- HostIDを媒介体とするRMIを可能にします。
	- 一般的なゲームサーバーに必要な基本的なオプションが設定されています。
	- クライアント間のP2P接続をサポートします。このサーバーオブジェクトはクライアント間のP2P接続に失敗した場合、リレーサーバー役割をサポートします。

	一般的な用法
	- このオブジェクトを生成します。Proud.CNetServer.Create() 使用
	- Start()でサーバーを開始します。
	- INetServerEventを実現します。仮にINetServerEvent.OnClientJoin()などを実現します。
	これらはクライアント接続などに対するイベントハンドラーです。
	- 開始時にSetEventSink()でイベントハンドラーを組んで、AttachProxy()、AttachStub()を使って1個以上の送信または受信用RMIインターフェースを連携します。

	P2P接続グループ生成
	- CNetServer に接続されたクライアントは1個以上のP2Pグループを作ることができます。
	P2Pグループで縛られているクライアント間ではHostIDを因子にRMIをやり取りできます。
	- 新しいP2P groupを生成/除去する：サーバーのみが権限があります。
	CreateP2PGroup(),DestroyP2PGroup()を通じ、クライアントではOnP2PMemberJoinなどが呼び出されます。
	- peerを既に存在するP2P groupに追加・除去する：
	JoinP2PGroup(), LeaveP2PGroup(). クライアントではOnP2PMemberJoinなどが呼び出されます。

	Relay serverとしての役割
	- HostIDを使う限り、P2P接続は仮想でも行われます。例えば、P2P接続が実際にまだ成功しなかったPeer間でも相手がP2P groupに属していれば相手のHostIDに送るRMIは成功的に相手に行きます。relay serverが中途役割を担当します。
	同じRMI受信メッセージでもそれがrelay serverを景夕したのかどうかを得るためには、RMI受信部でRmiContext.m_relayedを読めばいいです。

	スレッドプーリング
	- 基本的にスレッドプールを使用します。よって、サーバー開始時にスレッド数を最小１個を指定しなければなりません。
	- スレッドプーリング機能をオフするのが有利な場合：1個のCPUでありながらCPU time処理が全部であるサーバー(例えば、リレーサーバー)
	- スレッドプーリング機能をオンするのが有利な場合：マルチCPUまたはdevice time処理が頻繁なサーバー(例えば、MMOゲームサーバー)
	- スレッドプール関数は、CoInitialize()が準備された状態で呼び出されますので、また、CoInitialize()とかCoUninitialize()の呼び出しが不要。

	UDPホストポート数に関連するサーバー性能
	- CNetServer.StartのサーバーポートパラメーターでUDPホストポート数を指定することができます。
	クライアントはサーバーに接続する時にパラメーターにあるポートのうち一つでサーバーに接続します。

	P2P直接通信成功可能性ノード
	- P2Pの片側がreal IP, Full-cone NATである場合、direct P2P通信が成功します。
	- ゲームサーバーがNATの後ろで作動する場合、Start()にサーバーアドレスを入れる必要があります。(詳しくはStart()の説明をご参照ください。)
	但し、このようにするとしても、ゲームサーバーの前のNATの後ろにある他のホストで実行されるクライアントと外部インターネットのクライアント間の直接P2P通信は実現されません。サーバーにあるNAT装置によって外部インターネットアドレスの替わりにNATの内部IPが認識されるからです。

	\~
	*/
	class CNetServer : public IRmiHost, public HasCoreEventFunctionObjects
	{
	public:
		virtual ~CNetServer() {}

	private:
		PROUDSRV_API static CNetServer* Create_Internal();
	public:

		/**
		\~korean
		새 인스턴스를 생성합니다.

		\~english
		Creates new instance.

		\~chinese
		生成新的实例。

		\~japanese
		新しいインスタンスを生成します。
		\~
		*/
		inline static CNetServer* Create()
		{
			// 여기서 PROUDNET_H_LIB_SIGNATURE를 건드린다. 이렇게 함으로 이 전역 변수가 컴파일러에 의해 사라지지 않게 한다.
			PROUDNET_H_LIB_SIGNATURE++;
			return Create_Internal();
		}

		/**
		\~korean
		클라이언트 또는 채팅 그룹에 속한 모든 클라이언트를 쫓아낸다.
		\return 해당 클라이언트가 없으면 false.

		\~english
		Kicks out all clients in client group and/or chatting group
		\return false if there is no realted client.

		\~chinese
		踢出属于Client或者聊天组的所有client。
		\return 没有相关client的话false。

		\~japanese
		クライアントまたはチャットグループに属した全てのクライアントを追い出します。
		\return 該当クライアントがなければfalse.

		\~
		*/
		virtual bool CloseConnection(HostID clientHostID) = 0;

		/**
		\~korean
		모든 클라이언트를 추방한다.

		\~english
		Kicks out all clients

		\~chinese
		踢出所有client。

		\~japanese
		全てのクライアントを追い出します。
		\~
		*/
		virtual void CloseEveryConnection() = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 을 생성합니다. 
		- 이렇게 생성한 그룹 내의 피어들끼리는 즉시 서로간 메시징을 해도 됩니다.  (참고: <a target="_blank" href="http://guide.nettention.com/cpp_ko#robust_p2p" >ProudNet의 P2P 통신 성능</a>)
		- 클라이언트 목록(clientHostIDs,clientHostIDNum)을 받을 경우 이미 멤버가 채워져 있는 P2P group을 생성한다. 
		텅 빈 배열을 넣을 경우 멤버 0개짜리 P2P 그룹을 생성할 수 있다. (단, AllowEmptyP2PGroup 에 따라 다르다.) 
		- 이 메서드 호출 후, CNetClient 에서는 P2P 그룹의 각 멤버에 대해(자기 자신 포함) 
		INetClientEvent.OnP2PMemberJoin 이 연이어서 호출된다.
		- HostID_Server 가 들어가도 되는지 여부는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_as_p2pgroup_member" >P2P 그룹 멤버로서 서버를 포함시키기</a> 를 참고하십시오. 

		\param clientHostIDs 만들 P2P 그룹의 멤버 리스트. HostID_Server 는 멤버로 불가능하다. count=0일때 이 값은 무시된다. 
		\param count clientHostIDs의 배열 요소 갯수. 0을 넣을 경우 멤버가 없는 P2P group 을 생성함을 의미한다. 
		\param message P2P 그룹을 추가하면서 서버에서 관련 클라이언트들에게 보내는 메시지. 생략하셔도 됩니다. 
		- INetClientEvent.OnP2PMemberJoin 에서 그대로 받아집니다. 
		\param option P2P 그룹 생성 과정에서 사용자가 지정하고 싶은 추가 옵션입니다. 생략하셔도 됩니다. 
		\param assignedHostID 사용자가 원하는 HostID 를 가진 P2P 그룹을 생성할 수 있습니다. 기본값은 None입니다. None을 지정하면 자동 할당됩니다. 만약 이미 사용중인 HostID 를 입력할 경우 P2P group 생성은 실패하고 None을 리턴할 수 있습니다.
		\return 만들어진 P2P 그룹의 Host ID 입니다. 타입은 HostID 입니다.

		\~english 
		Generate a <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>. 
		- The peers in the group generated in this way can do messaging with one another immediately (<a target="_blank" href="http://guide.nettention.com/cpp_en#robust_p2p" >P2P performance of ProudNet</a>)
		- After calling this method, for each member of P2P group(including itself), INetClientEvent.OnP2PMemberJoin will be called one after another to CNetClient.
		When you insert an empty array, you can generate a P2P group with 0 members (however, it depends on AllowEmptyP2PGroup).
		- After calling this method, in CNetClient, INetClientEvent.OnP2PMemberJoin is called continuously to each member of the P2P group (including itself).
		- To find out whether HostID_Server can enter or not, please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#server_as_p2pgroup_member" >How to include a server as a peer - to - peer group member</a> .

		\param clientHostIDs The member list of the P2P group to make clientHostIDs. HostID_Server is not possible as a member. When count=0, this value is ignored. 
		\param count The number of array elements of clientHostIDs. If you input 0, it means it will generate a P2P group without a member. 
		\param The message to be sent to the related clients from the server when a P2P group is added. You can skip it. 
		- It is received in INetClientEvent.OnP2PMemberJoin as it is. 
		\param This is an additional option that users want to designate during creation of a P2P group. You can skip it. 
		\param You can create a P2P group with the HostID users want. The default value is None. If you select None, it is automatically allocated. If you enter a HostID already being used, P2P group creation will fail and None can be returned. 
		\return HostID of the created P2P group. The type is HostID.  

		\~chinese
		生成<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%。
		- 这样生成的组内peer之间可以立即进行相互之间的messaging。（参考：<a target="_blank" href="http://guide.nettention.com/cpp_zh#robust_p2p" >ProudNet 的P2P通信功能</a>）
		- 收到client目录（clientHostIDs,clientHostIDNum）的话生成已经填充成员的P2P group。
		- 输入空数组的话能生成0个成员的P2P组。（但根据AllowEmptyP2Pgroup会有所不同）
		- 呼叫此方法后对P2P组的各个成员（包括自己本身）在 CNetClient%连续呼叫 INetClientEvent.OnP2PmemberJoin。
		- HostID_Server 是否能进入请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_as_p2pgroup_member" >以P2P的组成员函括服务器</a>%。

		\param clientHostIDs 要创建的P2P组成员list。 HostID_Server 不可能成为成员。Count=0时被忽视。
		\param count clientHostIDs的数组因素个数。输入0的话意味着生成没有成员的P2Pgroup
		\param message 添加P2P组，并可以省略服务器往相关client发送的信息。
		- 在 INetClientEvent.OnP2PMemberJoin%如实的被接收。

		\param option 在P2P组的生成过程中用户想要指定的添加选项。可以省略。
		\param assignedHostID 可以生成持有用户希望的HostID的P2P组。基本值是None。如果指定为None则会自动进行分配。如果输入已在使用中的 HostID%，则生成P2P group将会失败，之后Return None。
		\return 创建的P2P组Host ID。类型是Host ID。

		\~japanese
		\ref p2p_group を生成します。
		- このように生成したグループ内のPeer間は直ちにサーバー間でメッセージングをしても良いです。(参考： \ref robust_p2p)
		- クライアントリスト(clientHostIDs,clientHostIDNum)を受ける場合、既にメンバーが満たされているP2P groupを生成します。
		空の配列を入れる場合、メンバー0個のP2Pグループを生成することができます。(但し、AllowEmptyP2PGroup によって違います。)
		- このメソッドを呼び出し後、CNetClientではP2Pグループの各メンバーに対して(自分自身を含む)
		INetClientEvent.OnP2PMemberJoinが続けられて呼び出されます。
		- HostID_Server が入っても良いのかどうかは、\ref server_as_p2pgroup_memberをご参照ください。
		\param clientHostIDs 作るP2Pグループのメンバーリスト。HostID_Serverはメンバーで不可能です。count=0である時、この値は無視されます。
		\param count clientHostIDsの配列要素数。0を入れる場合、メンバーがないP2P groupを生成することを意味します。
		\param message P2Pグループを追加しながら、サーバーで関連クライアントに送るメッセージ。省略しても良いです。
		- INetClientEvent.OnP2PMemberJoin でそのまま受けられます。
		\param option P2P グループの生成過程でユーザーが指定したい追加オプションです。省略しても良いです。
		\param assignedHostID ユーザーが望むHostIDを持つP2Pグループを生成することができます。基本値はNoneです。Noneを指定すれば自動割り当てられます。もし、既に使用中であるHostIDを入力する場合、P2P groupの生成は失敗して、Noneをリターンできます。
		\return 作られたP2PグループのHost IDです。タイプはHostIDです。

		\~
		*/
		virtual HostID CreateP2PGroup(const HostID* clientHostIDs, int count, ByteArray message = ByteArray(), CP2PGroupOption &option = CP2PGroupOption::Default, HostID assignedHostID = HostID_None) = 0;

		/**
		\~korean
		멤버가 없는 P2P 그룹을 만듭니다.
		- AllowEmptyP2PGroup 설정에 따라 그룹 생성이 실패할 수도 있다.
		\param message P2P 그룹을 추가하면서 서버에서 관련 클라이언트들에게 보내는 메시지입니다. 생략하셔도 됩니다.
		\param assignedHostID 사용자가 원하는 HostID 를 가진 P2P 그룹을 생성할 수 있습니다. 기본값은 None입니다. None을 지정하면 자동 할당됩니다. 만약 이미 사용중인 HostID 를 입력할 경우 P2P group 생성은 실패하고 None을 리턴할 수 있습니다.
		\return 만들어진 P2P 그룹의 Host ID 입니다. 타입은 HostID 입니다.

		\~english 
		Create a P2P group without a member. 
		- Group creation may fail depending on the AllowEmptyP2PGroup configuration. 
		\param message This is the message sent to related clients from the server when adding a P2P group. You can skip it. 
		\param You can create a P2P group with the HostID users want. The default value is None. If you select None, it is automatically allocated. If you enter a HostID already being used, P2P group creation will fail and None can be returned.
		\return HostID of the created P2P group. The type is HostID.

		\~chinese
		创建没有成员的P2P组。
		- 根据AllowEmptyP2PGroup设置，组的生成可能会失败。
		\param message 添加P2P组，并可以省略服务器往相关client发送的信息。
		\return 创建的P2P组的Host ID。类型是HostID。

		\~japanese
		メンバーのないP2Pグループを作ります。
		- AllowEmptyP2PGroup 設定によってグループ生成に失敗することもあります。
		\param message P2P グループを追加しながらサーバーで関連クライアントに送るメッセージです。省略しても良いです。
		\param assignedHostID ユーザーが望むHostIDを持つP2Pグループを生成することができます。基本値はNoneです。Noneを指定すれば自動割り当てられます。もし、既に使用中であるHostIDを入力する場合、P2P groupの生成は失敗してNoneをリターンできます。
		\return 作られたP2PグループのHost IDです。タイプはHostIDです。

		\~
		*/
		inline HostID CreateP2PGroup(ByteArray message = ByteArray(), CP2PGroupOption &option = CP2PGroupOption::Default, HostID assignedHostID = HostID_None)
		{
			return CreateP2PGroup(NULL, 0, message, option, assignedHostID);
		}

		/**
		\~korean
		1개 멤버로 구성된 P2P 그룹을 만듭니다.
		- 이렇게 생성한 그룹 내의 피어들끼리는 즉시 서로간 메시징을 해도 됩니다. (참고: \ref robust_p2p) 

		\param memberID 그룹에 처음 들어가 있을 멤버의 HostID 입니다.
		\param message P2P 그룹을 추가하면서 서버에서 관련 클라이언트들에게 보내는 메시지입니다. 생략하셔도 됩니다.
		\param assignedHostID 사용자가 원하는 HostID 를 가진 P2P 그룹을 생성할 수 있습니다. 기본값은 None입니다. None을 지정하면 자동 할당됩니다. 만약 이미 사용중인 HostID 를 입력할 경우 P2P group 생성은 실패하고 None을 리턴할 수 있습니다.
		\return 만들어진 P2P 그룹의 Host ID 입니다. 타입은 HostID 입니다.

		\~english 
		You can create a P2P group consisted of one member
		- The peers in the group generated in this way can do messaging with one another immediately (refer to: \ref robust_p2p)

		\param memberID The HostID of the member when joining the group for the first time. 
		\param message This is the message sent to related clients from the server when adding a P2P group. You can skip it.
		\param You can create a P2P group with the HostID users want. The default value is None. If you select None, it is automatically allocated. If you enter a HostID already being used, P2P group creation will fail and None can be returned.
		\return HostID of the created P2P group. The type is HostID.

		\~chinese
		创建由一个成员组成的P2P组。
		- 这样生成的组内peer之间可以立即进行相互的messaging。（参考：<a target="_blank" href="http://guide.nettention.com/cpp_en#robust_p2p" >P2P performance of ProudNet</a>）
		\param memberID 第一次进入组的成员Host ID。
		\param message 添加P2P组，并可以省略服务器往相关client发送的信息。
		\param assignedHostID 可以生成持有用户希望的HostID的P2P组。基本值是None。如果指定为None则会自动进行分配。如果输入已在使用中的 HostID%，则生成P2P group将会失败，之后Return None。
		\return 创建的P2P组的Host ID。类型是HostID。

		\~japanese
		1個のメンバーから構成されたP2Pグループを作ります。
		- このように生成したグループ内のPeer間では、直ちにお互いメッセージングをしても良いです。(参考：\ref robust_p2p)
		\param memberID グループに初めて入っているメンバーのHostIDです。
		\param message P2P グループを追加しながら、サーバーで関連クライアントに送るメッセージです。省略しても良いです。
		\param assignedHostID ユーザーが望むHostIDを持つP2Pグループを生成することができます。基本値はNoneです。Noneを指定すれば自動割り当てられます。もし、既に使用中であるHostIDを入力する場合、P2P group生成は失敗して、Noneをリターンできます。
		\return 作られたP2PグループのHost IDです。タイプはHostIDです。

		\~
		*/
		inline HostID CreateP2PGroup(HostID memberID, ByteArray message = ByteArray(), CP2PGroupOption &option = CP2PGroupOption::Default, HostID assignedHostID = HostID_None)
		{
			return CreateP2PGroup(&memberID, 1, message, option, assignedHostID);
		}

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 을 파괴한다.
		- 안에 있던 모든 member들은 P2P 그룹에서 탈퇴된다는 이벤트(Proud.INetClientEvent.OnP2PMemberLeave)를 노티받는다.
		물론, 서버와의 연결은 유지된다.
		\param groupHostID 파괴할 P2P 그룹의 ID
		\return 해당 P2P 그룹이 없으면 false.

		\~english
		Destructs <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>
		- All members within will receive an event(Proud.INetClientEvent.OnP2PMemberLeave) that they will be withdrawn from P2P group. Of course, the connection to server will be sustained.
		\param groupHostID ID of P2P group to be destructed
		\return false if there is no P2P group related

		\~chinese
		破坏<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%。
		- 在里面的所有member被通知从P2P组退出的event（Proud.INetClientEvent.OnP2PMemberLeave）。
		当然，继续维持与服务器的连接。
		\param groupHostID 要破坏的P2P组ID。
		\return 没有相关P2P组的话false。

		\~japanese
		\ref p2p_group を破壊します。
		- 内部にあった全てのメンバーは、P2Pグループより脱退されるというイベント(Proud.INetClientEvent.OnP2PMemberLeave)を通知してもらいます。
		勿論、サーバーとの接続は維持されます。
		\param groupHostID 破壊するP2PグループのID
		\return 該当P2Pグループがなければfalse.

		\~
		*/
		virtual bool DestroyP2PGroup(HostID groupHostID) = 0;

		/**
		\~korean
		서버에 생성되어 있는 모든 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 을 뒤져서, 멤버가 전혀 없는 P2P 그룹만 골라서 모두 파괴한다.

		\~english
		Search all <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> that is created within server then destructs all P2P groups consist of 0 member.

		\~chinese
		查找在服务器生成的所有 <a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%，只选择没有成员的P2P组后全部破坏。

		\~japanese
		サーバーに生成されている全ての\ref p2p_groupを探して、メンバーが全くないP2Pグループだけを選んで全て破壊します。
		\~
		*/
		virtual void DestroyEmptyP2PGroups() = 0;

		/**
		\~korean
		내부 상황을 문자열로 리턴한다. 디버그용으로 쓸만하다.

		\~english
		Returns internal status as text string. Useful when debugging.

		\~chinese
		用字符串返回内部情况。可用于调试用。

		\~japanese
		内部状況を文字列でリターンします。デバッグ用で使う価値があります。
		\~
		*/
		virtual String DumpGroupStatus() = 0;

		/**
		\~korean
		접속한 클라이언트 갯수를 얻는다.

		\~english
		Gets the number of connected clients

		\~chinese
		获得连接的client个数。

		\~japanese
		接続したクライアント数を得ます。
		\~
		*/
		virtual int GetClientCount() = 0;

		/**
		\~korean
		서버 처리량 통계를 얻는다.

		\~english
		Gets server process statistics

		\~chinese
		获得服务器处理量统计。

		\~japanese
		サーバー処理量の統計を得ます。
		\~
		*/
		virtual void GetStats(CNetServerStats &outVal) = 0;

		/**
		\~korean
		클라이언트 HostID 목록을 얻는다.
		- 이 메서드로 얻는 목록은 호출할 시점의 스냅샷일 뿐이다. 이 함수가 리턴된 후에도
		클라이언트 목록의 내용이 실제 서버의 상태와 동일함을 보장하지는 않는다.
		따라서 정확한 클라이언트 리스트를 얻고자 하면 INetServerEvent.OnClientJoin,
		INetServerEvent.OnClientLeave 에서 유저가 필요로 하는 클라이언트 리스트를 따로
		만들어 관리하는 것을 권장한다.
		\param output HostID 목록이 채워질 곳
		\param outputLength 목록의 최대 크기
		\return 실제 채워진 항목의 갯수

		\~english
		Gets client HostID list
		- The list acquired by this method is only a snapshot. After this function is returned, there is no guarantee that the contents in the list matche the status of real server.
		Therefore, in order to get precise client list, it is recommended to create and manage a separate client list needed by user at INetServerEvent.OnClientJoin, INetServerEvent.OnClientLeave.
		\param output Where HostID list to be filled
		\param outputLength Maximum size of the list
		\return The number of clause really filled

		\~chinese
		获得client Host ID目录。
		- 用此方法获得的目录只是要呼叫的时点的snapshot而已。此函数返回以后不保证client目录的内容与实际服务器状态相同。
		所以想获得正确的client list的话，建议在 INetServerEvent.OnClientJoin,
		INetServerEvent.OnClientLeave%另外创建并管理用户需要的client list。
		\param output HostID目录填充的地方。
		\param outputLength 目录的最大大小。
		\return 实际填充的项目个数。

		\~japanese
		クライアントHost IDリストを得ます。
		- このメソッドで得るリストは呼び出す時点のスナップショットであるだけです。この関数がリターンされた後にもクライアントリストの内容が実際サーバーの状態と同じであることを保障することはありません。
		よって、正確なクライアントリストを得たい場合は、INetServerEvent.OnClientJoin, INetServerEvent.OnClientLeaveでユーザーが必要とするクライアントリストを別に作って管理することをお勧めします。
		\param output HostID リストが満たされる所
		\param outputLength リストの最大サイズ
		\return 実際満たされた項目の数

		\~
		*/
		virtual int GetClientHostIDs(HostID* output, int outputLength) = 0;

		/**
		\~korean
		clientHostID가 가리키는 peer가 참여하고 있는 P2P group 리스트를 얻는다.

		\~english
		Gets P2P group list of peer that is pointed by clientHostID

		\~chinese
		获得clientHostID所指的peer参与的P2P group list。

		\~japanese
		clientHostIDが指すPeerが参加しているP2P group リストを得ます。
		\~
		*/
		virtual bool GetJoinedP2PGroups(HostID clientHostID, HostIDArray& output) = 0;

		/**
		\~korean
		(이 함수 대신에 StartRoundTripLatencyTest와 GetRoundTripLatency를 대신 사용하십시오.)
		클라이언트의 마지막 레이턴시를 얻습니다.
		\param clientID 찾으려는 클라이언트의 HostID.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		Gets the last latency of client.		
		\param clientID HostID of client to find
		\return returns ping time in millisecond. Returns -1 when there is none.

		\~chinese
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		获得client的最后latency。
		\param clientID 要找的client的Host ID。
		\return ping把time以毫秒单位返回。但没有的话返回-1。

		\~japanese
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		クライアントの最後のレーテンシーを得ます。
		\param clientID 探したいクライアントのHostID.
		\return ping timeをミリ秒単位でリターンします。但し、なければ-1をリターンします。
		\~
		*/
		inline double GetLastPingSec(HostID clientID, ErrorType* error = NULL)
		{
			int ms = GetLastUnreliablePingMs(clientID, error);

			if (ms < 0)
				return (double)ms;

			return ((double)ms) / 1000;
		}

		/**
		\~korean
		클라이언트의 마지막 레이턴시를 얻는다.
		\param clientID 찾으려는 클라이언트의 HostID.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		Gets the last latency of client
		\param clientID HostID of client to find
		\return Returns ping time in millisecond.Returns -1 when there is none.

		\~chinese
		获得client的最后latency。
		\param clientID 要找的client的Host ID。
		\return ping把time以毫秒单位返回。但没有的话返回-1。

		\~japanese
		クライアントの最後のレーテンシーを得ます。
		\param clientID 探したいクライアントのHostID.
		\return ping timeをミリ秒単位でリターンします。但し、なければ-1をリターンします。

		\~
		*/
		virtual int GetLastUnreliablePingMs(HostID clientID, ErrorType* error = NULL) = 0;

		/**
		\~korean
		(이 함수 대신에 StartRoundTripLatencyTest와 GetRoundTripLatency를 대신 사용하십시오.)
		클라이언트의 최근 레이턴시를 얻는다.
		\param clientID 찾으려는 클라이언트의 HostID.
		\return ping time을 초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		Gets the recent latency of client.
		\param clientID HostID of client to find
		\return Returns ping time in millisecond. Returns -1 when there is none.

		\~chinese
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		获得client的最近latency。
		\param clientID 要找的client的Host ID。
		\return ping把time以毫秒单位返回。但没有的话返回-1。

		\~japanese
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		クライアントの最近レーテンシーを得ます。
		\param clientID 探したいクライアントのHostID.
		\return ping timeを秒単位でリターンします。但し、なければ-1をリターンします。
		\~
		*/
		inline double GetRecentPingSec(HostID clientID, ErrorType* error = NULL)
		{
			int ms = GetRecentUnreliablePingMs(clientID, error);

			if (ms < 0)
				return (double)ms;

			return ((double)ms) / 1000;
		}

		/**
		\~korean
		(이 함수 대신에 StartRoundTripLatencyTest와 GetRoundTripLatency를 대신 사용하십시오.)
		클라이언트의 최근 레이턴시를 얻는다.
		\param clientID 찾으려는 클라이언트의 HostID.
		\return ping time을 밀리초단위로 리턴한다. 단 없으면 -1을 리턴한다.

		\~english
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		Gets the recent latency of client
		\param clientID HostID of client to find
		\return Returns ping time in millisecond.Returns -1 when there is none.

		\~chinese
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		获得client的最近latency。
		\param clientID 要找的client的Host ID。
		\return ping把time以毫秒单位返回。但没有的话返回-1。

		\~japanese
		(We recommend you to use StartRoundTripLatencyTest and GetRoundTripLatency rather than this function.)
		クライアントの最近レーテンシーを得ます。
		\param clientID 探したいクライアントのHostID.
		\return ping timeをミリ秒単位でリターンします。但し、なければ-1をリターンします。
		\~
		*/
		virtual int GetRecentUnreliablePingMs(HostID clientID, ErrorType* error = NULL) = 0;

		/**
		\~korean
		두 피어간의 최근 핑 정보를 받습니다.

		\~english
		Gets recent ping information between two peers

		\~chinese
		接收两个peer之间的最近ping信息。

		\~japanese
		量Peer間の最近ping情報を受けます。
		\~
		*/
		virtual int GetP2PRecentPingMs(HostID HostA, HostID HostB) = 0;

		/**
		\~korean
		P2P group 1개의 정보를 얻는다.

		\~english
		Gets the information of 1 P2P group

		\~chinese
		获得P2P group一个信息。

		\~japanese
		P2P group 1個の情報を得ます。
		\~
		*/
		virtual bool GetP2PGroupInfo(HostID groupHostID, CP2PGroup &output) = 0;

		/**
		\~korean
		HostID 가 유효한지 확인한다.
		\param id 유효한지 확인할 HostID.
		\return true이면 유효한 클라이언트, 서버 혹은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 의 HostID임을 의미한다.

		\~english
		Checks if HostID is valid
		\param id HostID to be used to check if valid
		\return true then valid HsstID of client or server or <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>

		\~chinese
		确认Host ID是否有效。
		\param id 要确认是否是有效的Host ID。
		\return true的话有效的client，意味着是服务器或者<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%的Host ID。

		\~japanese
		HostID が有効なのかを確認します。
		\param id 有効なのかを確認する HostID.
		\return trueであれば有効なクライアント、サーバーまたは\ref p2p_groupのHostIDを意味します。

		\~
		*/
		virtual bool IsValidHostID(HostID id) = 0;

		/**
		\~korean
		모든 P2P group list를 얻는다.
		- 사본 카피를 한다.

		\~english
		Gets all P2P group list
		- Makes a back up copy

		\~chinese
		获得所有P2P group list。
		- 复印副本。

		\~japanese
		全てのP2P group listを得ます。
		- コピーします。

		\~
		*/
		virtual void GetP2PGroups(CP2PGroups& output) = 0;

		/**
		\~korean
		개설되어 있는 P2P group의 총 갯수를 얻는다.

		\~english
		Gets the total number of opened P2P group.

		\~chinese
		获得开设的P2P group的总个数。

		\~japanese
		開設されているP2P groupの総数を得ます。
		\~
		*/
		virtual int GetP2PGroupCount() = 0;

		/**
		\~korean
		이 객체에 연결된 peer 1개의 정보를 얻는다.
		\param peerHostID 찾으려는 peer의 HostID
		\param output 찾은 peer의 정보. 못 찾으면 아무것도 채워지지 않는다.
		\return true이면 찾았다는 뜻

		\~english
		Gets the information of 1 peer connected to this object
		\param peerHostID HostID of peer to find
		\param output Infoof foudn peer. Nothing will be filled when found nothing.
		\return true when found

		\~chinese
		获得连接与此对象的一个peer信息。
		\param peerHostID 要找的peer的HostID。
		\param output 找到的peer信息。没有找到的话不填充任何东西。
		\return true是找到了的意思。

		\~japanese
		このオブジェクトに接続されたPeer 1個の情報を得ます。
		\param peerHostID 探したいPeerのHostID
		\param output 探したPeerの情報。探せなかったら何も満たされません。
		\return trueであれば探したという意味

		\~
		*/
		virtual bool GetClientInfo(HostID clientHostID, CNetClientInfo& output) = 0;

		/**
		\~korean
		clientHostID가 가리키는 클라이언트가 접속중인지 체크한다.
		\return 접속중인 클라이언트이면 true를 리턴한다.

		\~english
		Checks if the client pointed by clientHostID is logged on
		\return if found to be logged on client then returns true

		\~chinese
		检查clientHostID所指的client是否是连接中。
		\return 连接中的client的话返回true。

		\~japanese
		clientHostIDが指すクライアントが接続中なのかをチェックします。
		\return 接続中のクライアントであればtureをリターンします。

		\~
		*/
		virtual bool IsConnectedClient(HostID clientHostID) = 0;

		/**
		\~korean
		GetClientInfo와 같은 역할을 하며, 이제 더 이상 안쓰는 함수다. 대신 GetClientInfo를 사용하는 것을 권장한다.

		\~english
		Performs roles as GetClientInfo but not to be used any more. It is recommended to use GetClientInfo instead.

		\~chinese
		起着与GetClientInfo一样的作用，而且是不再使用的函数。建议使用GetClientInfo来代替。

		\~japanese
		GetClientInfoと同じ役割をして、これ以上使わない関数です。代わりにGetClientInfoを使うことをお勧めします。
		\~
		*/
		PN_DEPRECATED inline bool GetPeerInfo(HostID clientHostID, CNetClientInfo& output)
		{
			return GetClientInfo(clientHostID, output);
		}

		/**
		\~korean
		이 호스트에 연결된 다른 호스트(서버의 경우 클라이언트들, 클라이언트의 경우 피어들)들 각각에의 tag를 지정합니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#host_tag" >각 호스트에 대한 사용자 정의 데이터(Tag)</a> 기능입니다.
		- 자기 자신(서버 혹은 클라이언트)에 대한 tag 도 지정할 수 있습니다.

		\~english
		Designates tag to each of other hosts(clients for servers, peers for clients) that are connected to this host. A <a target="_blank" href="http://guide.nettention.com/cpp_en#host_tag" >User-defined data(Tag) for each host</a> function.
		- Can designate tag to itself(server or client)

		\~chinese
		指定往这个主机连接的其他主机（服务器的湖client，client的话peer）的tag。<a target="_blank" href="http://guide.nettention.com/cpp_zh#host_tag" >对各个主机的用户定义数据（Tag）</a>%功能。
		- 也可以指定自己本身（服务器或者client）的tag。

		\~japanese
		このホストに接続された他のホスト(サーバーの場合はクライアント、クライアントの場合はPeer)のそれぞれにtagを指定します。\ref host_tag 機能です。
		- 自分自身(サーバーまたはクライアント)に対するtagも指定することができます。

		\~
		*/
		virtual bool SetHostTag(HostID hostID, void* hostTag) = 0;

		/**
		\~korean
		서버의 현재 시간을 얻는다. CNetClient.GetServerTimeMs()과 거의 같은 값이다.

		\~english
		Gets current time of server. Has almost same value as CNetClient.GetServerTimeMs().

		\~chinese
		获得服务器的现在时间。与 CNetClient.GetServerTimeMs()基本相同的值。

		\~japanese
		サーバーの現在時間を得ます。CNetClient.GetServerTimeMs()とほぼ同じ値です。
		\~
		*/
		virtual int64_t GetTimeMs() = 0;

		/**
		\~korean
		클라이언트 memberHostID가 기 존재 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a>  groupHostID에 중도 참여하게 합니다.
		- 참여 시도된 피어들과 기존에 참여되어 있던 피어들 끼리는 즉시 서로간 메시징을 해도 됩니다.  (참고: <a target="_blank" href="http://guide.nettention.com/cpp_ko#robust_p2p" >ProudNet의 P2P 통신 성능</a>)

		\param memberHostID P2P 그룹에 참여할 멤버의 Host ID. HostID_Server 가 들어가도 되는지 여부는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_as_p2pgroup_member" >P2P 그룹 멤버로서 서버를 포함시키기</a> 를 참고하십시오.
		\param groupHostID 이미 만들어진 P2P 그룹의 Host ID입니다.
		\param message P2P 그룹을 추가하면서 서버에서 관련 클라이언트들에게 보내는 메시지입니다. INetClientEvent.OnP2PMemberJoin 에서 그대로 받아집니다.
		\return 성공시 true를 리턴합니다.

		\~english
		Lets client memberHostID enter into existing <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> groupHostID
		- Messaging can do between existed peer and new peer. (Reference: <a target="_blank" href="http://guide.nettention.com/cpp_en#robust_p2p" >P2P performance of ProudNet</a>)

		\param memberHostID HostID of member to join P2P group. Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#server_as_p2pgroup_member" >How to include a server as a peer-to- peer group member</a> to find out if HostID_Server can join.
		\param groupHostID HostID of existing P2P group
		\param message Message to be sent to related clients from server when adding P2P group. Will be accepted as it is at INetClientEvent.OnP2PMemberJoin.
		\return returns true if successful

		\~chinese
		让Client的memberHostID往已存在的 <a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a> groupHostID中途参与。
		- 试图参与的peer和之前参与的peer之间可以立即进行messaging（参考 <a target="_blank" href="http://guide.nettention.com/cpp_zh#robust_p2p" >ProudNet 的P2P通信功能</a>）。

		\param memberHostID 参与P2P组的成员Host ID。 HostID_Server 是否也能进入请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_as_p2pgroup_member" >以P2P的组成员函括服务器</a>%。
		\param groupHostID 已经创建的P2P组的Host ID。
		\param message 添加P2P组的时候服务器往相关client发送的信息。在 INetClientEvent.OnP2PMemberJoin%原封不动的被接收。
		\return 成功的时候返回true。

		\~japanese
		クライアントmemberHostIDが既に存在。\ref p2p_group  groupHostIDに中途参加するようにします。
		- 参加を試したPeerと既存に参加していたPeer間では、直ちにメッセージングをしても良いです。(参考：\ref robust_p2p)
		\param memberHostID P2P グループに参加するメンバーのHost ID。HostID_Serverが入ってもいいかは、\ref server_as_p2pgroup_memberをご参照ください。
		\param groupHostID 既に作られたP2PグループのHost IDです。
		\param message P2P グループを追加しながら、サーバーより関連クライアントに送るメッセージです。 INetClientEvent.OnP2PMemberJoin よりそのまま受け取られます。
		\return 成功時にtrueをリターンします。

		\~
		*/
		virtual bool JoinP2PGroup(HostID memberHostID, HostID groupHostID, ByteArray message = ByteArray()) = 0;

		/**
		\~korean
		클라이언트 memberHostID가 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a>  groupHostID 를 나가게 한다.
		- 추방되는 클라이언트에서는 P2P 그룹의 모든 멤버에 대해(자기 자신 포함) INetClientEvent.OnP2PMemberLeave 이 연속으로 호출된다.
		한편, P2P group의 나머지 멤버들은 추방되는 클라이언트에 대한 INetClientEvent.OnP2PMemberLeave 를 호출받는다.
		- 멤버가 전혀 없는, 즉 비어있는 P2P group가 될 경우 AllowEmptyP2PGroup의 설정에 따라 자동 파괴되거나 잔존 후 재사용 가능하다.

		\param memberHostID 추방할 멤버의 Host ID
		\param groupHostID 어떤 그룹으로부터 추방시킬 것인가
		\return 성공적으로 추방시 true를 리턴

		\~english
		Client memberHostID lets <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> groupHostID go out
		- At the client being kicked out, INetClientEvent.OnP2PMemberLeave will be called one after another for all members of P2P group(including itself).
		On the other hand, INetClientEvent.OnP2PMemberLeave will be called for other members of P2P group that are to be kicked out.
		- For those of no members, in other words for an empty P2P group, they will be either auto-destructed by AllowEmptyP2PGroup setting or can be re-used after its survival.

		\param memberHostID HostID of member to be kicked out
		\param groupHostID to be kicked out from which group?
		\return returns true if successfully kicked out

		\~chinese
		Client 的memberHostID让 <a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a> groupHostID退出。
		- 在被踢出去的client对P2P组的所有成员（包括自己本身）INetClientEvent.OnP2PmemberLeave%连续呼叫。
		一方面P2P group的剩下成员会被呼叫对踢出的client的 INetClientEvent.OnP2PMemberLeave。
		- 没有成员的话，即成为空的P2P group的时候根据AllowEmptyP2Pgroup的设置可以自动破坏或者残留后再使用。

		\param memberHostID 要踢出的Host ID。
		\param groupHostID 要从哪个组踢出。
		\return 踢出成功的话返回true。

		\~japanese
		クライアントmemberHostIDが\ref p2p_group  groupHostID を出て行くようにします。
		- 追い出されるクライアントでは、P2Pグループの全てのメンバーに対して(自分自身を含む) INetClientEvent.OnP2PMemberLeaveが連続で呼び出されます。
		一方、P2P groupの残りのメンバーは、追い出されるクライアントに対するINetClientEvent.OnP2PMemberLeaveを呼び出してもらいます。
		- メンバーが全くない、すなわち空いているP2P groupになる場合、AllowEmptyP2PGroupの設定によって自動破壊されるか、残存後に再使用が可能です。
		\param memberHostID 追い出されるメンバーのHost ID
		\param groupHostID どのグループから追い出させるか？
		\return 追い出しに成功した時、trueをリターン

		\~
		*/
		virtual bool LeaveP2PGroup(HostID memberHostID, HostID groupHostID) = 0;

		/**
		\~korean
		이벤트를 받을 수 있는 객체를 설정한다.
		- 하나만 설정 가능하다.
		- 이 메서드는 클라이언트가 서버에 연결을 시도하기 전에만 호출할 수 있다. 안그러면 exception이 throw된다.

		\~english
		Sets an object that can receive event
		- Can set only 1 object
		- This method can only be called before client attempts to connect to server. Otherwise, an execption will be 'throw'n.

		\~chinese
		设置可以接收event的对象。
		- 只能设置一个。
		- 此方法在client试图连接服务器之前呼出。否则会exception이 throw。

		\~japanese
		イベントを受けられるオブジェクトを設定します。
		- このメソッドはクライアントがサーバー接続を試す前のみ呼び出すことができます。そうしなければ、exceptionがthrowされます。

		\~
		*/
		virtual void SetEventSink(INetServerEvent* eventSink) = 0;

		// 주의: 실존 객체이므로 SUPPORTS_LAMBDA_EXPRESSION로 감싸지 말 것.

		/// See Proud.INetServerEvent.OnClientJoin for details.
		LambdaBaseHolder_Param1<void, CNetClientInfo*> OnClientJoin;
		/// See Proud.INetServerEvent.OnClientLeave for details.
		LambdaBaseHolder_Param3<void, CNetClientInfo *, ErrorInfo *, const ByteArray& > OnClientLeave;
		/// See Proud.INetServerEvent.OnClientHackSuspected for details.
		LambdaBaseHolder_Param2<void, HostID, HackType > OnClientHackSuspected;
		/// See Proud.INetServerEvent.OnClientOffline for details.
		LambdaBaseHolder_Param1<void, CRemoteOfflineEventArgs &> OnClientOffline;
		/// See Proud.INetServerEvent.OnClientOnline for details.
		LambdaBaseHolder_Param1<void, CRemoteOnlineEventArgs &> OnClientOnline;
		/// See Proud.INetServerEvent.OnConnectionRequest for details.
		LambdaBaseHolder_Param4<void, AddrPort, ByteArray&, ByteArray&, bool*> OnConnectionRequest;
		/// See Proud.INetServerEvent.OnP2PGroupJoinMemberAckComplete for details.
		LambdaBaseHolder_Param3<void, HostID, HostID, ErrorType > OnP2PGroupJoinMemberAckComplete;
		/// See Proud.INetServerEvent.OnP2PGroupRemoved for details.
		LambdaBaseHolder_Param1<void, HostID > OnP2PGroupRemoved;
		/// See Proud.INetServerEvent.OnUserWorkerThreadBegin for details.
		LambdaBaseHolder_Param0<void> OnUserWorkerThreadBegin;
		/// See Proud.INetServerEvent.OnUserWorkerThreadEnd for details.
		LambdaBaseHolder_Param0<void> OnUserWorkerThreadEnd;


	public:
		/**
		\~korean
		서버를 시작합니다. 서버 시작을 하지 못할 경우 Proud.Exception이 throw 됩니다.
		\param 서버 시작을 위한 설정 내용입니다. TCP listen port 등을 지정합니다.

		\~english
		The server starts. If the server does not start, Proud.Exception will throw.
		Setting for the initiating the \param server. Able to indicate TCP listen port.

		\~chinese
		开始启动服务器，如果无法启动则 Proud.Exception 将被 throw。
		\param 服务器启动相关设置内容，指定 TCP listen port 等。


		\~japanese
		サーバーを起動します。 サーバが起動していない場合はProud.Exceptionがthrowされます。
		\Param サーバを起動するための確立の内容です。 TCP listen port等を指定します。
		*/
		virtual void Start(CStartServerParameter &param) = 0;

		/**
		\~korean
		서버를 시작합시다. 서버 시작을 하지 못할 경우 outError에 에러 내용이 들어갑니다.
		자세한 것은 Proud.CNetServer.Start(CStartServerParameter&)를 참고하십시오.

		\~english
		The server starts. If the server does not start, error information goes in to outError.
		For detail information please refer to Proud.CNetServer.Start(CStartServerParameter&).

		\~chinese
		开始启动服务器，如果无法启动则错误内容将进入 outError。
		详细内容请参考 Proud.CNetServer.Start(CStartServerParameter&)。

		\~japanese
		サーバーを起動します。サーバが起動していない場合はoutErrorにエラーの内容が入ります。
		詳細については Proud.CNetServer.Start(CStartServerParameter&)を参照してください。
		*/
		virtual void Start(CStartServerParameter &param, ErrorInfoPtr& outError) = 0;

		/**
		\~korean
		서버를 종료한다.
		-주의!! 이 함수는 rmi등 UserWorkerThread에서 호출하는 함수내에서는 호출해서는 안됩니다.

		\~english
		Stop server.
		- Warning!! This function never call in function that call from UserWorkerThread such as rmi.

		\~chinese
		终止服务器。
		- 注意！不能把此函数在rmi等UserWorkerThread呼叫的函数内呼叫。

		\~japanese
		サーバーを終了します。
		-ご注意!! この関数はrmiなどUserWorkerThreadで呼び出す関数内では呼び出してはいけません。

		\~
		*/
		virtual void Stop() = 0;

		/**
		\~korean
		멤버가 전혀 없는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 이 유지될 수 있도록 설정하는 옵션. 기본적으로는 true로 설정되어 있다.
		- true로 설정될 경우: CreateP2PGroup 호출시 멤버 0개짜리 P2P 그룹을 생성해도 성공한다. 멤버가 0개짜리 P2P 그룹이 계속 존재한다.
		이러한 경우 사용자는 멤버가 0개인 P2P 그룹이 계속해서 서버에 남지 않도록, 필요한 경우 DestroyP2PGroup 또는 DestroyEmptyP2PGroups 을 호출해야 한다.
		- false로 설정될 경우: CreateP2PGroup 호출시 멤버 0개짜리 P2P 그룹을 생성할 수 없다. P2P 그룹의 멤버가 0개가 되면 그 P2P 그룹은 자동 파괴된다.

		\~english
		Option to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained. Default is true.
		- When set as true: Creating P2P group of 0 member will succeed when CreateP2PGroup is called. The P2P group of 0 member will sustain.
		In this case, user must call DestroyP2PGroup or DestroyEmptyP2PGroups to prevent the P2P group of 0 member to be left in server.
		- When set as false: Creating P2P group of 0 member will fail when CreateP2PGroup is called. If the number of P2P group becomes 0 then the P2P group will be destructed automatically.

		\~chinese
		设置成<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%没有成员时也能维持的选项。默认设置为true。
		- 设置为true的话：呼叫CreateP2Pgroup的话，即使生成0个成员的P2P组也会成功。0个成员的P2P组继续存在。
		这时候用户不要让0个成员的P2P组继续存在于服务器，并且需要的话呼叫DestroyP2PGroup或者DestroyEmptyP2PGroups。
		- 设置为false的话：呼叫CreateP2PGroup时不能生成0个成员的P2P组。P2P 组的成员变成0个的话那个P2P组会自动破坏。

		\~japanese
		メンバーの全くない\ref p2p_groupが維持されるように設定するオプション。基本的にはtrueに設定されています。
		- trueに設定される場合：CreateP2PGroupを呼び出す時、メンバー0個のP2Pグループを生成しても成功します。メンバーが0個のP2Pグループが続けて存在します。
		このような場合、ユーザーはメンバーが0個であるP2Pグループが続けてサーバーの残らないように、必要な場合、DestroyP2PGroupまたはDestroyEmptyP2PGroupsを呼び出さなければなりません。
		- falseに設定される場合：CreateP2PGroupを呼び出す時、メンバー0個のP2Pグループを生成することはできません。P2Pグループのメンバーが0個になるとそのP2Pグループは自動破壊されます。

		\~
		*/
		virtual void AllowEmptyP2PGroup(bool enable) = 0;

        /**
        \~korean
        멤버가 전혀 없는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 이 유지될 수 있도록 설정하는 옵션값을 얻습니다.
        \return 멤버가 전혀 없는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 이 유지될 수 있도록 설정하는 옵션값입니다.

        \~english
        Gets option value to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained.
        \return Option value to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained.

        \~chinese
        Gets option value to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained.
        \return Option value to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained.

        \~japanese
        Gets option value to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained.
        \return Option value to set <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> without member can be sustained.

        \~
        */
        virtual bool IsEmptyP2PGroupAllowed() const = 0;

		/**
		\~korean
		클라이언트에서 인식 가능한 서버의 리스닝 주소 리스트를 얻는다.
		- 서버 시작시 받은 서버 주소를 토대로 리턴한다. NAT나 2개 이상의 랜카드가 있는 경우 유용하다.

		\~english
		Gets listening port address of the server that can be recognized by client
		- Returns values based on the server addresses that are received when server started. This is useful when using NAT or 2 or more LAN cards.

		\~chinese
		获得在client可以识别的服务器listening地址list。
		- 开启服务器时基于服务器地址返回。有NAT或者2个以上LAN卡的时候有用。

		\~japanese
		クライアントで認識可能なサーバーのリスニングアドレスを得ます。
		- サーバーを開始時に受けたサーバーアドレスを土台にリターンします。NATとか2個以上のLANカードがある場合有用です。

		\~
		*/
		virtual void GetRemoteIdentifiableLocalAddrs(CFastArray<NamedAddrPort> &output) = 0;

		/**
		\~korean
		TCP listener port의 로컬 주소 리스트를 얻는다. 예를 들어 서버 포트를 동적 할당한 경우 이것을 통해 실제 서버의 리스닝 포트 번호를 얻을 수 있다.

		\~english
		Gets local address of TCP listener port. For an example, it is possible to acquire the listening port number of real server through this when the server ports are dynamically allocated.

		\~chinese
		获得TCP listener port的本地地址list。例如动态分配服务器端口的时候，通过这个可以获得实际服务器的listening端口号码。

		\~japanese
		TCP listener portのローカルアドレスリストを得ます。例えば、サーバーポートをダイナミックに割り当てた場合、これを通じ実際サーバーのリスニングポート番号を得ることができます。
		\~
		*/
		virtual void GetTcpListenerLocalAddrs(CFastArray<AddrPort> &output) = 0;

		/**
		\~korean
		UDP listener port의 로컬 주소 리스트를 얻는다. 예를 들어 서버 포트를 동적 할당한 경우 이것을 통해 실제 서버의 리스닝 포트 번호를 얻을 수 있다.

		\~english
		Gets local address of UDP listener port. For an example, it is possible to acquire the listening port number of real server through this when the server ports are dynamically allocated.

		\~chinese
		获得UDP listener port本地地址list。例如动态分配服务器端口的时候，通过这个可以获得实际服务器的listening端口号码。

		\~japanese
		UDP listener portのローカルアドレスリストを得ます。例えば、サーバーポートをダイナミックに割り当てた場合、これを通じ実際サーバーのリスニングポート番号を得ることができます。
		\~
		*/
		virtual void GetUdpListenerLocalAddrs(CFastArray<AddrPort> &output) = 0;

		/**
		\~korean
		너무 오랫동안 서버와 통신하지 못하면 연결을 해제하기 위한 타임 아웃 임계값(밀리초)를 지정한다.
		- CNetServer.Start 호출 전에만 사용할 수 있다. 그 이후부터 호출할 경우 무시된다.
		- 무한으로 잡는 것은 추천하지 않는다. 비정상 연결 해제시 무한히 남는 연결로 자리잡기 때문이다. 그러느니 차라리 1시간을 거는 것이 낫다.
		반면, 1초 등 너무 작은 값을 잡으면 정상적인 클라이언트도 쫓겨날 위험이 있으므로 피해야 한다.
		- 기본값은 ProudNetConfig.DefaultNoPingTimeoutTime 이다.
		- 참고: <a target="_blank" href="http://guide.nettention.com/cpp_ko#debug_pause_problem" >디버거에 의해 일시정지하면 서버와의 연결이 끊어지는 문제 해결법</a>

		\~english
		Sets the critical value of time out(millisecond) to disconnect the connection with server when it has been long not communicating with it
		- Can only be used before calling CNetServer.Start. Caliing after that point will be ignored.
		- It is not recommended setting it as permanent since it will remain as an infinite connection after unexpected disconnection. It is better setting it as an hour than the case.
		- Default is ProudNetConfig.DefaultNoPingTimeoutTime.
		- Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#debug_pause_problem" >How to maintain the connection with server when the program stops due to debugger</a>.

		\~chinese
		过长时间不能与服务器通信的话，指定为了解除连接的超时临界值（millisecond）。
		- 只能在 CNetServer.Start t 呼叫之前使用。之后开始呼叫的话会被忽视。
		- 不建议设为无限。因为非正常连接时会成为无限剩余的连接。所以不如设置为1小时。
		反面，设太小的值的话，client 可能会有被赶出去的危险。
		- 默认值是 ProudNetConfig.DefaultNoPingTimeoutTime%。
		- 参考：<a target="_blank" href="http://guide.nettention.com/cpp_zh#debug_pause_problem" >因Debug一时终止而与服务器断开连接时的解决方法</a>。

		\~japanese
		とても長い間サーバーと通信できなかったら、接続を解除するためのタイムアウト臨界値(秒)を指定します。
		- CNetServer.Start を呼び出す前のみ使うことができます。その後から呼び出す場合は無視されます。
		- 無限にすることはお勧めしません。異常接続解除時に無限に残る接続になるからです。なので、むしろ1時間にした方が良いです。
		その反面、1秒など小さすぎる値を取ると正常なクライアントも追い出される危険がありますので避けるべきです。
		- 基本値はProudNetConfig.DefaultNoPingTimeoutTimeです。
		- 参考：\ref debug_pause_problem

		\~
		*/
		virtual void SetDefaultTimeoutTimeMs(int newValInMs) = 0;

		/**
		\~korean
		너무 오랫동안 서버와 통신하지 못하면 연결을 해제하기 위한 타임 아웃 임계값(밀리초)를 지정합니다.
		SetDefaultTimeoutTimeMs 와 다른점은 특정 호스트에 대해서만 런타임중에도 타임아웃을 변경 할 수 있습니다.
		- 서버의 시작 전, 후 상관 없이 사용 할 수 있습니다.
		- 특정 호스트(클라이언트)에만 적용 시킵니다.
		- 변경된 타임아웃 값이 적용되는 데는 몇 초간 걸릴 수 있습니다.

		\~english
		Specify timeout threshold(msec.) for disconnection in case of disconnected from server too long.
		Difference from SetDefaultTimeoutTimeMS is that you can change timeout during runtime for specific host.
		-You can use it regardless of before or after starting the server.
		-Applied to specific host(client).
		-It may take some seconds to apply the time out value change.


		\~chinese TODO:translate needed.
		Specify timeout threshold(msec.) for disconnection in case of disconnected from server too long.
		Difference from SetDefaultTimeoutTimeMS is that you can change timeout during runtime for specific host.
		-You can use it regardless of before or after starting the server.
		-Applied to specific host(client).
		-It may take some seconds to apply the time out value change.


		\~japanese TODO:translate needed.
		Specify timeout threshold(msec.) for disconnection in case of disconnected from server too long.
		Difference from SetDefaultTimeoutTimeMS is that you can change timeout during runtime for specific host.
		-You can use it regardless of before or after starting the server.
		-Applied to specific host(client).
		-It may take some seconds to apply the time out value change.
		*/
		virtual void SetTimeoutTimeMs(HostID host, int newValInMs) = 0;

		/**
		\~korean
		m_enableAutoConnectionRecovery가 true이면 SetDefaultTimeoutSec이나 SetDefaultTimeouMs으로 지정한 타임아웃 시간이 유효하지 않게 됩니다.
		따라서 ACR의 연결 유지 시간을 대신 지정해줘야 하는데, 이 함수를 통하여 지정해줄 수 있습니다.
		서버 시작 전에 설정할 수 있습니다.
		- 기본값은 60 * 1000 입니다. 즉, 1분입니다.

		\~english
		If m_enableAutoConnectionRecovery is true, The time of TimeOut is not valid.
		So, You should set the ACR time by using this method.
		- Default is 60 * 1000. ( 1 minute )

		\~chinese TODO:translate needed.

		If m_enableAutoConnectionRecovery is true, The time of TimeOut is not valid.
		So, You should set the ACR time by using this method.
		- Default is 60 * 1000. ( 1 minute )

		\~japanese TODO:translate needed.

		If m_enableAutoConnectionRecovery is true, The time of TimeOut is not valid.
		So, You should set the ACR time by using this method.
		- Default is 60 * 1000. ( 1 minute )

		*/
		virtual void SetDefaultAutoConnectionRecoveryTimeoutTimeMs(int newValInMs) = 0;

		/**
		\~korean
		m_enableAutoConnectionRecovery가 true이면 SetDefaultTimeoutSec이나 SetDefaultTimeouMs으로 지정한 타임아웃 시간이 유효하지 않게 됩니다.
		따라서 ACR의 연결 유지 시간을 대신 지정해줘야 하는데, 이 함수를 통하여 지정해줄 수 있습니다.
		- 서버의 시작 전, 후 상관 없이 사용 할 수 있습니다.
		- 특정 호스트(클라이언트)에만 적용 시킵니다.
		- 변경된 타임아웃 값이 적용되는 데는 몇 초간 걸릴 수 있습니다.

		\~english
		If m_enableAutoConnectionRecovery is true, The time of TimeOut is not valid.
		So, You should set the ACR time by using this method.
		- It may take some seconds to apply the time out value change.
		- You can use it regardless of before or after starting the server.
		- Applied to specific host(client).
		- It may take some seconds to apply the time out value change.

		\~chinese TODO:translate needed.

		If m_enableAutoConnectionRecovery is true, The time of TimeOut is not valid.
		So, You should set the ACR time by using this method.
		- It may take some seconds to apply the time out value change.
		- You can use it regardless of before or after starting the server.
		- Applied to specific host(client).
		- It may take some seconds to apply the time out value change.


		\~japanese TODO:translate needed.

		If m_enableAutoConnectionRecovery is true, The time of TimeOut is not valid.
		So, You should set the ACR time by using this method.
		- It may take some seconds to apply the time out value change.
		- You can use it regardless of before or after starting the server.
		- Applied to specific host(client).
		- It may take some seconds to apply the time out value change.

		*/
		virtual void SetAutoConnectionRecoveryTimeoutTimeMs(HostID host, int newValInMs) = 0;

		/**
		\~korean
		클라이언트가 UDP 통신을 어느 정도까지만 쓰도록 제한할 것인지를 설정하는 메서드다.
		- 사용자의 의도적인 이유로 클라이언트가 서버와의 또는 peer간의 UDP 통신을 차단하고자 할 때
		이 메서드를 쓸 수 있다.
		- 극악의 상황, 예를 들어 UDP 통신 장애가 심각하게 발생하는 서버에서 임시방편으로 이 옵션을 켜서
		문제를 일시적으로 줄일 수 있다. 하지만 UDP 통신은 게임 서비스에서 중요하기 때문에
		최대한 빨리 문제를 해결 후 이 옵션을 다시 켜는 것이 좋다.

		- 이 값을 변경한 후 새로 접속하는 클라이언트에 대해서만 적용된다.
		- 초기 상태는 FallbackMethod_None 이다.

		\param newValue 제한 정책. Proud.FallbackMethod_CloseUdpSocket 는 쓸 수 없다.

		\~english
		The method to set limits to how far the client uses UDP communication
		- This method can be used to block UDP communication between client and server or client and peer due to intentional reason by user.
		- The worst case, for an example, if there occurs a serious UDP communication hurdle at a server then the problem can be temporarily reduced by turning this option up.
		However, since UDP communication is crucial to game service so it is recommended to solve the problems ASAP then thrun this option back on.

		- Only applied to those newly connected clients after this value is changed
		- The initial status is FallbackMethod_None.

		\param newValue a limitation policy. Proud.FallbackMethod_CloseUdpSocket cannot be used.

		\~chinese
		设置client要限制UDP通信至什么程度的方法。
		- 因用户故意想要中断client和服务器或者peer的UDP通信的时候，可以使用此方法。
		- 最坏的情况，例如在UDP通信障碍严重发生的服务器里以应急方法开启此选项的话可以临时减少此问题。但是UDP通信在游戏服务里很重要，尽快把问题解决后重新开启此选项为好。

		- 只对修改此值以后重新连接的client适用。
		- 初始状态是FallbackMethod_None。

		\param newValue 限制政策。不能使用 Proud.FallbackMethod_CloseUdpSocket%。

		\~japanese
		クライアントがUDP通信をどれほどまで使うように制限するかを設定するメソッドです。
		- ユーザーの意図的な理由でクライアントがサーバーとの、またはPeer間のUDP通信を遮断しようとする時、このメソッドを使うことができます。
		- 最悪の状況、例えば、UDP通信障害が深刻に発生するサーバーで臨時方便としてこのオプションをつけて問題を一時的に減らさせることはできます。しかし、UDP通信はゲームサーバーで重要なので、最大限に速く問題を解決後、このオプションを再び付けた方が良いです。
		- この値を変更した後、新たに接続するクライアントに対してのみ適用されます。
		- 初期状態はFallbackMethod_Noneです。
		\param newValue 制限ポリシー。Proud.FallbackMethod_CloseUdpSocketは使うことができません。

		\~
		*/
		virtual void SetDefaultFallbackMethod(FallbackMethod newValue) = 0;

		/**
		\~korean
		내부 실행 로그를 파일로 남기는 기능을 켠다.
		- 이 기능이 필요한 경우는 별로 없다. Nettention에서 문제를 수집하기 위한 목적으로 사용된다.
		- 이미 EnableLog 를 한 상태에서 이 함수를 또 호출하면 이 함수는 아무것도 하지 않는다. 만약 다른 파일명으로
		로그를 계속해서 쌓고 싶으면 DisableLog를 호출 후 EnableLog를 호출해야 한다.
		\param logFileName 로그 파일 이름

		\~english
		Turns on the option that leaves internal execution log as file
		- There are not many cases that require this function. Used to collect problems at Nettention.
		- Calling this function again while already 'EnableLog'ed, this function will not do anything. If you need to keep stacking the log with other file name the you must call DisableLog then call EnableLog.
		\param logFileName log file name

		\~chinese
		开启把内部实行log保留为文件的功能。
		- 一般没有需要此功能的情况。 Nettention 以收集问题为目的使用此功能。
		- 已EnableLog的状态下呼叫此函数的话，此函数不会做任何事情。如果想用其他文件名继续堆积log的话，要呼叫DisableLog以后再呼叫EnableLog。
		\param logFileName log文件名。

		\~japanese
		内部実行ログをファイルで残す機能を付けます。
		- この機能が必要な場合は別にありません。Nettentionで問題を収集するための目的で使われます。
		- 既にEnableLogをした状態でこの関数をまた呼び出すとこの関数は何もしません。もし、他のファイル名でログを続けて保存したい場合はDisableLogを呼び出した後、EnableLogを呼び出してください。
		\param logFileName ログファイル名

		\~
		*/
		virtual void EnableLog(const PNTCHAR* logFileName) = 0;

		/**
		\~korean
		EnableLog로 켠 로그를 끈다.

		\~english
		Turns off the log that is thrned on by EnableLog

		\~chinese
		关闭EnableLog的log。

		\~japanese
		EnableLogでつけたログをオフします。
		\~
		*/
		virtual void DisableLog() = 0;

		/**
		\~korean
		스피드핵 탐지 기능의 신중함의 정도를 설정합니다.
		- 0~100 사이 값으로 설정합니다. 100에 가까울수록 신중하게 스피드핵을 탐지한다.
		- ProudNet의 스피드핵 탐지 기능은 클라이언트와 서버간의 ping과 타임스탬프 값으로 작동합니다.
		0에 가까운 값을 설정할 수록 스피드핵을 쓰는 클라이언트를 탐지하는 데 걸리는 시간은 짧아지지만
		스피드핵을 쓰지 않음에도 불구하고 통신 회선의 불량으로 인해 ping이 불규칙하게 도달하는 클라이언트마저도
		스피드핵을 쓰는 클라이언트로 오인될 가능성이 높아집니다.
		- 초기값은 1입니다.
		- 신중함의 정도를 조절한 후 이후부터 접속하는 클라이언트에 한해 변경된 값이 적용됩니다.

		\~english
		Set the intensity of speed hack detection
		- Sets between the value of 0 and 100. Closer to 100 detects with more intensity.
		- The speed hack detection function of ProudNet operates based on ping among servers and time stamp values.
		Using the value closer to 0 provides less time taken for detection but those clients with inconsistant ping can be detected as a speed hack.

		- Initial value is 100.
		- Changed value will be applied to clients connected after the intensity is changed.

		\~chinese
		设置speed hack探知功能的慎重程度。
		- 设置成0~100之间值。越接近1越慎重探知speed hack。
		- ProuNet的speed hack探知功能运转为client和服务器之间ping和time stamp值。
		设置为越接近0的值，探知使用speed hack的client的时间越会缩短，即使不使用speed hack，因为通信线路的不良，ping 不规则到达的client被误会为是使用speed hack的client的可能性会增加。
		- 默认值是1。
		- 调整慎重的程度以后，限制在已连接的client，适用修改的值。

		\~japanese
		スピード核探知機能の慎重さの程度を設定します。
		- 0~100の間の値に設定します。100に近いほど慎重にスピード核を探知します。
		- ProudNetのスピード核探知機能は、クライアントとサーバー間のpingとタイムスタンプ値で作動します。
		0に近い値を設定するようにスピード核を使うクライアントを探知するにかかる時間は短くなりますが、スピード核を使わないにも関わらず、通信回線の不良によってpingが不規則に到達するクライアントさえもスピード核を使うクライアントに誤認される可能性が高くなります。
		- 初期値は1です。
		- 慎重さの程度を調節した後、以降から接続するクライアントに限って変更された値が適用されます。

		\~
		*/
		virtual void SetSpeedHackDetectorReckRatioPercent(int newValue) = 0;

		/**
		\~korean
		스피드핵 탐지 기능을 켜거나 끈다.
		\param clientID 스피드핵 탐지 기능을 끄거나 켤 클라이언트의 HostID
		\param enable 켜려면 true.
		\return 유효한 클라이언트를 찾았으면 true. 적용할 클라이언트가 없는 경우 false.

		\~english
		Turns on/off speed hack detection function
		\param clientID HostID of client that speed hack detection to be turned on/off
		\param enable must set true to turn on
		\return True if found valid client, false if no related clients.

		\~chinese
		开启或者关闭speed hack探知功能。
		\param clientID 要关闭或者开启speed hack探知功能的client Host ID。
		\param enable 开启的话true。
		\return 找到有效的client的话true。没有想适用的client的话false。

		\~japanese
		スピード核探知機能をオン・オフします。
		\param clientID スピード核探知機能をオン・オフするクライアントのHostID
		\param enableつけるためにはtrue.
		\return 有効なクライアントを探したらtrue。適用するクライアントがない場合はfalse

		\~
		*/
		virtual bool EnableSpeedHackDetector(HostID clientID, bool enable) = 0;

		/**
		\~korean
		해킹된 클라이언트에서 망가뜨려놓은 메시지 헤더를 보낼 경우(특히 메시지 크기 부분)
		이를 차단하기 위해서는 서버측에서 받는 메시지 크기의 상한선을 지정해야 한다.
		이 메서드는 그 상한선을 지정하는 역할을 한다.
		초기값은 64KB이다.
		\param value_s serverMessageMaxLength
		\param value_c clientMessageMaxLength

		\~english
		To stop the damaged message header sent by a hacked client(especially the size part), it is crucial to set the maximum size of messages received by server.
		This method sets the limit and the default value is 64KB.
		\param value_s serverMessageMaxLength
		\param value_c clientMessageMaxLength

		\~chinese
		从被黑的client发送毁坏的信息header的时候（特别是信息大小部分），为了中断这个要指定在服务器方接收的信息大小的上限。
		此方法起着指定那个上限的作用。
		初始值是64KB。
		\param value_s serverMessageMaxLength
		\param value_c clientMessageMaxLength

		\~japanese
		ハッキングされたクライアントで壊しておいたメッセージヘッダーを送る場合(特に、メッセージサイズ部分)
		このメソッドはその上限線を指定する役割をします。
		初期値は64kbです。
		\param value_s serverMessageMaxLength
		\param value_c clientMessageMaxLength
		

		\~
		*/
		virtual void SetMessageMaxLength(int value_s, int value_c) = 0;

		/**
		\~korean
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#delayed_send" >TCP 지연 송신 기능과 Nagle 알고리즘</a> 상태를 얻는다.

		\~english
		Gets <a target="_blank" href="http://guide.nettention.com/cpp_en#delayed_send" >TCP delayed transmitting function and Nagle algorism</a> condition.

		\~chinese
		获得<a target="_blank" href="http://guide.nettention.com/cpp_zh#delayed_send" >TCP 延迟送信功能和Nagle算法</a>%状态。

		\~japanese
		\ref delayed_send  状態を得ます。
		
		\~
		*/
		virtual bool IsNagleAlgorithmEnabled() = 0;

		// ProudNet의 여러가지 unit test를 시행한다. DLL 버전의 경우 세부 API를 export가 불가능하므로 이렇게 구현되어 있다.
		// virtual void TEST_SomeUnitTests() = 0;

		/**
		\~korean
		지정한 클라이언트가 맺을 수 있는 최대 Direct P2P 연결 갯수를 지정합니다.
		- 여기서 지정한 값을 초과한 나머지 P2P 연결은 항상 Relayed가 됩니다.
		- Direct P2P의 실제 갯수는 여기서 지정한 값 이하가 나올 수 있는데, 이는 ProudNet의 <a target="_blank" href="http://guide.nettention.com/cpp_ko#jit_p2p" >JIT(Just-in-time) P2P 연결</a> 기능 때문이며, 정상적인 상황입니다.

		\~english
		Designates the maximum number of Direct P2P connections that the desinated clients can be connected to
		- Those other P2P connections that have values that exceed the designated value will always be relayed.
		- Actual number of P2P connections may be less than newVal due to <a target="_blank" href="http://guide.nettention.com/cpp_ko#jit_p2p" >JIT(Just-in-time) P2P connection</a>, but it is not a problem.

		\~chinese
		指定被指定的client可以连接的最大Direct P2P连接个数。
		- 在这里超过指定个数的P2P连接一直会Relayed。
		- Direct P2P的实际个数可能会是在这里指定的值以下，这是因为ProudNet的<a target="_blank" href="http://guide.nettention.com/cpp_zh#jit_p2p" >JIT(Just-in-time) P2P连接</a> %功能。是正常情况。

		\~japanese
		指定したクライアントが結べる最大Direct P2P接続数を指定します。
		- ここで指定した値を超過した残りのP2P接続はいつもRelayedされます。
		- Direct P2Pの実際数はここで指定した値以下になることがありますが、これはProudNetの\ref jit_p2p機能のためで、正常な状況です。

		\~
		*/
		virtual void SetMaxDirectP2PConnectionCount(HostID clientID, int newVal) = 0;

		/**
		\~korean
		클라이언트간 P2P 홀펀칭을 시작하기 위한 조건을 지정합니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#jit_p2p" >JIT(Just-in-time) P2P 연결</a> 를 참고하십시오.
		- 이 메서드를 호출한 이후에 접속한 클라이언트부터 본 값이 적용됩니다.
		- 기본값은 JIT 입니다.

		\param newVal 클라이언트간 P2P 통신을 위한 홀펀칭을 시작하는 조건입니다.
		\return 성공적으로 반영시 true를 리턴합니다.

		\~english

		\todo retranslate required.

		Designates the condition to begin P2P hole-punching among clients <a target="_blank" href="http://guide.nettention.com/cpp_en#jit_p2p" >JIT(Just-in-time) P2P connection</a>
		- The clients that the designated value will be applied to are the client that connected after this method is called.
		- Default is the value that is designated at Proud.CNetConfig.DefaultDirectP2PStartCondition.

		\param newVal the condition to begin P2P hole-punching among clients
		\return returns true if successfully applied

		\~chinese
		为了开始P2P打洞而指定条件。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#jit_p2p" >JIT(Just-in-time) P2P连接</a> %。
		- 呼叫此方法以后连接的client开始使用此值。
		- 默认值是JIT。

		\param newVal 开始为了client之间P2P通信的条件。
		\return 反映成功时返回true。

		\~japanese
		クライアント間P2Pホールパンチングを開始するための条件を指定します。詳しくは、\ref jit_p2pをご参照ください。
		- このメソッドを呼び出した後に接続したクライアントより本値が適用されます。
		- 基本値はJITです。
		\param newVal クライアント間のP2P通信のためのホールパンチングを開始する条件です。
		\return 反映に成功時にtrueをリターンします。

		\~
		*/
		virtual bool SetDirectP2PStartCondition(DirectP2PStartCondition newVal) = 0;

		/**
		\~korean
		수퍼 피어로서 가장 적격의 역할을 할 클라이언트를 찾는다.
		<a target="_blank" href="http://guide.nettention.com/cpp_ko#super_peer" >Super Peer(수퍼피어 혹은 호스트) 중심의 P2P 네트워킹</a> 형식의 게임을 개발중인 경우, 이 메서드는 groupID가 가리키는 P2P그룹 에 있는 멤버들 중
		가장 최적의 수퍼 피어를 찾아서 알려준다.
		- 이 메서드는 P2P 그룹을 생성하거나 변경한 직후 바로 얻을 경우 제대로 찾지 못할 수 있다.
		- 처음 이 메서드를 호출한 후부터 약 2~5초 정도 지난 후에 다시 호출하면 더 정확한 수퍼피어를 찾는다.
		\param groupID 이 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 에서 찾는다
		\param policy 수퍼 피어를 선정하는 정책. 자세한 설명은 CSuperPeerSelectionPolicy 참고.
		\param exclusdees groupID가 가리키는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 의 멤버 중 exclusdees에 들어있는 멤버들은 제외하고 선별한다.
		예를 들어 이미 사용하던 수퍼피어가 자격 박탈된 경우 다시 이것이 재 선발됨을 의도적으로 막고자 할 때 유용하다.
		\return 수퍼피어로서 가장 적격인 클라이언트의 HostID. P2P 그룹을 찾지 못하거나 excludees에 의해 모두가 필터링되면 HostID_None 을 리턴합니다.

		\~english
		Finds the clients that can perform best as super peer
		When developing game of <a target="_blank" href="http://guide.nettention.com/cpp_en#super_peer" >P2P Networking for Super Peer (Super Peer or host)</a> format, this method finds and notifies the best possible super peer among members of the P2P group that is pointed by groupID.
		- There are possibilities that this method cannot find properly right after P2P group is eiter created or modified.
		- It will find more suitable super peer when called about 2 to 5 seconds after this method is called.
		\param groupID finds within this <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>
		\param policy policy that designates super peer. Please refer CSuperPeerSelectionPolicy.
		\param exclusdees Selects excluding the members within excludees among <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a> that is pointed by groupID
		For an example, it is useful to prevent intentionally the super peer to be re-selected when the super peer wass once disqualified.
		\return HostID of the best possible client as super peer. Returns HostID_None either when no P2P group was found or all filtered by excludees.

		\~chinese
		寻找作为super peer起着最合适作用的client。
		正在开发<a target="_blank" href="http://guide.nettention.com/cpp_zh#super_peer" >Super Peer(或主机) 中心的P2P通信</a>%形式游戏的话，此方法会找到在groupID所指的P2P组成员中最适合的superpeer并告知。
		- 此方法在生成P2P组或者修改后立即获得的时候可能会找得不够准确。
		- 开始呼叫此方法以后大概2~5秒后重新呼叫的话能找到正确的superpeer。
		\param groupID 在此<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%寻找。
		\param policy 选拔superpeer的政策。详细请参考 CSuperPeerSelectionPolicy%。
		\param exclusdees groupID所指的<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%成员中把exclusdees里的成员除外之后选拔。
		例如已使用的superpeer被剥夺资格以后，想故意阻止它重新被选定的时候有用。
		\return 作为superpeer最适合的clientHost ID。找不到P2P组或者被excludees全部过滤的话返回HostID_None。

		\~japanese
		スーパーピーアとして一番適格の役割をするクライアントを探します。
		\ref super_peer  形式のゲームを開発中の場合、このメソッドはgroupIDが指すP2Pグループにあるメンバーのうち一番最適なスーパーピーアを探してくれます。
		- このメソッドはP2Pグループを生成したり変更した後、すぐ得られる場合、きちんと探すことができない場合があります。
		- 初めてこのメソッドを呼び出した後から約2~5秒ほどが過ぎた後、再び呼び出すともっと正確なスーパーピーアを探します。
		\param groupID この\ref p2p_groupで探します。
		\param policy スーパーピーアを選定するポリシー。詳しくは、CSuperPeerSelectionPolicy をご参照ください。
		\param exclusdees groupIDが指す\ref p2p_groupのメンバーのうち、exclusdeesに入っているメンバーは除外して選別します。
		例えば、既に使っていたスーパーピーアの資格が剥奪された場合、再びこれが再先発されることを意図的に防ぎたい時に有用です。
		\return スーパーピーアとして一番適格なクライアントのHostID。P2Pグループを探すことができないとか、excludeesによって全てがフィルタリングされればHostID_Noneをリターンします。

		\~
		*/
		virtual HostID GetMostSuitableSuperPeerInGroup(HostID groupID, const CSuperPeerSelectionPolicy& policy = CSuperPeerSelectionPolicy::GetOrdinary(), const HostID* excludees = NULL, intptr_t excludeesLength = 0) = 0;

		/**
		\~korean
		동명 메서드 설명을 참고바람

		\~english
		Please refer same name method.

		\~chinese
		请参考同名方法说明。

		\~japanese
		同名メソッドの説明をご参照ください。
		\~
		*/
		virtual HostID GetMostSuitableSuperPeerInGroup(HostID groupID, const CSuperPeerSelectionPolicy& policy, HostID excludee) = 0;

		/**
		\~korean
		GetMostSuitableSuperPeerInGroup는 가장 적격의 <a target="_blank" href="http://guide.nettention.com/cpp_ko#super_peer" >Super Peer(수퍼피어 혹은 호스트) 중심의 P2P 네트워킹</a> 를 얻는 기능입니다.
		이 함수는 가장 적격의 수퍼피어 뿐만 아니라 차순위 적격 수퍼피어들의 목록을 제공합니다.
		이 함수가 리턴하는 리스트의 첫번째 항목은 GetMostSuitableSuperPeerInGroup에서 리턴하는 수퍼피어와 동일합니다.

		\param groupID 이 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 에서 적격 수퍼피어 목록을 제공합니다.
		\param ratings 여기에 적격 수퍼피어 목록이 채워집니다. 가장 적격인 수퍼피어 순으로 정렬되어 채워집니다.
		\param ratingsBufferCount rating 의 배열 항목 갯수입니다. 이 함수가 리턴하는 배열의 크기는 이 크기 이상은 채우지 않습니다.
		\param policy GetMostSuitableSuperPeerInGroup 에서 같은 이름의 파라메터를 참고하십시오.
		\param excludees GetMostSuitableSuperPeerInGroup 에서 같은 이름의 파라메터를 참고하십시오.
		\return ratings 에 채워진 항목의 갯수입니다. P2P 그룹을 찾지 못하거나 excludees 에 의해 모두가 필터링되면 0을 리턴합니다.

		\~english
		GetMostSuitableSuperPeerInGroup is the function to get the best possible <a target="_blank" href="http://guide.nettention.com/cpp_en#super_peer" >P2P Networking for Super Peer (Super Peer or host)</a>.
		Provides not only the best possible super peer but also the list of the next to the best possible super peer as well
		The first clause of the list that this function returns is same as the super peer returned by GetMostSuitableSuperPeerInGroup.

		\param groupID Provides the list of the best possible super peer among this <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>
		\param ratings This is where the list of super peer will be filled. The best super peer is to be at the top as sorted.
		\param ratingsBufferCount The number of array clause of rating. The size of the array returned by this function does not fill more than this size.
		\param policy Please refer the same name parameter at	GetMostSuitableSuperPeerInGroup.
		\param excludees Please refer the same name parameter at	GetMostSuitableSuperPeerInGroup.
		\return the number of clause that is filled at ratings. Returns 0 either when no P2P group was found or filtered all of them by excludees.

		\~chinese
		GetMostSuitableSuperPeerInGroup 是获得<a target="_blank" href="http://guide.nettention.com/cpp_zh#super_peer" >Super Peer(或主机) 中心的P2P通信</a>%最适合的功能。
		此函数不仅提供superpeer，而且还提供次顺序的适合superpeer的目录。
		此函数返回的list的第一个项目与在GetMostSuitableSuperPeerInGroup返回的superpeer相同。
		\param groupID 在<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%提供合适的superpeer的目录。
		\param ratings 在这里填充合适的superpeer目录。以最适合的superpeer顺序排序填充。
		\param ratingsBufferCount rating数组项目个数。此函数返回的数组不会填充此大小以上的大小。
		\param policy 在GetMostSuitableSuperPeerInGroup参考相同名字的参数。
		\param excludees 在GetMostSuitableSuperPeerInGroup参考相同名字的参数。
		\return 往ratings填充的项目个数。找不到P2P组或者被excludees全部过滤的话返回0。

		\~japanese
		GetMostSuitableSuperPeerInGroupは一番適格な\ref super_peer を得る機能です。
		この関数は一番適格なスーパーピーアだけでなく、次順位の適格スーパーピーアのリストを提供します。
		この関数がリターンするリストの最初項目は、GetMostSuitableSuperPeerInGroupがリターンするスーパーピーアと同じです。
		\param groupID この\ref p2p_groupで適格スーパーピーアリストを提供します。
		\param ratings ここに適格スーパーピーアリストが満たされます。一番適格なスーパーピーアの順で整列されて満たされます。
		\param ratingsBufferCount rating の配列項目数です。この関数がリターンする配列のサイズはこれ以上は満たしません。
		\param policy GetMostSuitableSuperPeerInGroup で同じ名前のパラメーターをご参照ください。
		\param excludees GetMostSuitableSuperPeerInGroup で同じ名前のパラメーターをご参照ください。
		\return ratings に満たされた項目の数です。P2Pグループを探すことができないとか、excludeesによって全てがフィルタリングされれば0をリターンします。

		\~
		*/
		virtual int GetSuitableSuperPeerRankListInGroup(HostID groupID, SuperPeerRating* ratings, int ratingsBufferCount, const CSuperPeerSelectionPolicy& policy = CSuperPeerSelectionPolicy::GetOrdinary(), const CFastArray<HostID> &excludees = CFastArray<HostID>()) = 0;

		/**
		\~korean
		서버에서 준비된 UDP 소켓 주소들을 얻는다.

		\~english
		Gets UDP socket addresses that are prepared by server

		\~chinese
		获得在服务器准备的UDP socket地址。

		\~japanese
		サーバーで準備されたUDPソケットアドレスを得ます。
		\~
		*/
		virtual void GetUdpSocketAddrList(CFastArray<AddrPort> &output) = 0;

		/**
		\~korean
		엔진 프로토콜 버전을 얻는다. 이 값이 클라이언트에서 얻는 엔진 프로토콜 버전과 다르면 접속이 불허된다.

		\~english
		Gets engine protocol version. Connection will be rejected if this value is different to engine protocol version from client.

		\~chinese
		获得引擎协议版本。此值与从client获得的引擎协议版本不同的话不允许连接。

		\~japanese
		エンジンプロトコルバージョンを得ます。この値がクライアントより得るエンジンプロトコールバージョンと違えば接続は不許になります。
		\~
		*/
		virtual int GetInternalVersion() = 0;

		/**
		\~korean
		지정한 대상과 관련된 모든 P2P 연결의 상태 통계를 얻습니다.
		가령 Direct P2P와 relayed P2P의 비율을 얻을 때 이 메서드가 유용합니다.
		\param remoteHostID 클라이언트 또는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 의 HostID 입니다.
		- 1개 클라이언트의 HostID 인 경우, 이 메서드는 그 클라이언트가 P2P 통신하는 다른 피어들과의 연결 통계를 반환합니다.
		- 1개 P2P 그룹의 HostID 인 경우, 이 메서드는 그 P2P 그룹 내의 모든 클라이언트끼리의 P2P 통신에 대한 통계를 반환합니다.
		\param status P2P커넥션 관련 변수들이 여기에 채워집니다.
		\return remoteHostID가 유효하면 true,유효하지 않으면 false를 리턴 합니다.

		\~english
		Gets status statistics of all P2P connections related to designated target
		This method is useful to get the proportion of Direct P2P and relayed P2P.
		\param remoteHostID HostID of client or <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>
		- If this is HostID of 1 client then this method returns connection statistics of other peers that the client is P2P communicating with.
		- If this is HostID of 1 P2Pgroup then this method returns the statistics of all P2P communications among all clients in the P2P group.
		\param status P2P connection related vairables are to be filled in here.
		\return Returns true if remoteHostID is valid, false if not valid

		\~chinese
		获得与指定的对象相关的所有P2P连接状态统计。
		如果获得Direct P2P和relayed P2P的比率的时候此方法有用。
		\param remoteHostID client或者<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%的Host ID。
		- 一个client的Host ID的话，此方法返回那个client与其他peer进行P2P通信的连接统计。
		- 一个P2P组的Host ID的话，此方法返回对那个P2P组内的所有client之间P2P通信的统计。
		\param status 这里填充P2P connection相关变数。
		\return remoteHostID有效的话true，无效的话返回false。

		\~japanese
		指定した対象に関わる全てのP2P接続の状態統計を得ます。
		もし、Direct P2Pとrelayed P2Pの比率を得る時、このメソッドが有用です。
		\param remoteHostID クライアントまたは\ref p2p_groupのHostIDです。
		- 1クライアントのHostIDである場合、このメソッドはそのクライアントがP2P通信する他のPeerとの接続統計を返還します。
		- 一P2PグループのHostIDの場合、このメソッドはそのP2Pグループ内の全てのクライアントのP2P通信に対する統計を返還します。
		\param status P2Pコネクション関連変数がここに満たされます。
		\return remoteHostIDが有効であればtrue、有効では無ければfalseをリターンします。

		\~
		*/
		virtual bool GetP2PConnectionStats(HostID remoteHostID,/*output */CP2PConnectionStats& status) = 0;

		/**
		\~korean
		remoteA 와 remoteB 사이의 udp message 시도횟수와 성공횟수를 반환해줍니다.
		- 사용 자는 이를 바탕으로 udp 손실율 등을 계산할수있습니다.

		\~english
		Return attempted number and succeed number of udp message between remoteA and remoteB
		- User can calculate udp loss rate based on it.

		\~chinese
		返回remoteA与remoteB之间udp message试图次数和成功次数。
		- 用户可以以此为基础计算UDP损失率等。

		\~japanese
		remoteA とremoteB間のudp messageの実行回数と成功階数を返還します。
		- ユーザーはこれを土台にUDP損失率などを計算することができます。

		\~
		*/
		virtual bool GetP2PConnectionStats(HostID remoteA, HostID remoteB, CP2PPairConnectionStats& status) = 0;

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
		\ref thread_pool の各スレッドの情報を得ます。
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
		\param output this is where the information is filled.

		\~chinese
		获得网络线程的信息。
		ProudNet 与<a target="_blank" href="http://guide.nettention.com/cpp_zh#thread_pool" >线程池(Thread pool)</a>%不同，内置着担任网络I/O处理的线程。获得它的信息。
		\param output 往这里填充信息。

		\~japanese
		ネットワーキングスレッドの情報を得ます。
		ProudNetは、\ref thread_poolとは別にネットワークI/O処理を担当するスレッドが内蔵されています。これの情報を得ます。
		\param output ここに情報が満たされます。

		\~
		*/
		virtual void GetNetWorkerThreadInfo(CFastArray<CThreadInfo> &output) = 0;

		/**
		\~korean
		사용자 정의 메시지를 전송합니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#send_user_message" >RMI를 전혀 안쓰고 사용자 정의 메시지를 주고 받기</a> 를 참고하십시오.
		\param remotes 수신자 배열입니다. RMI와 마찬가지로, 클라이언트, 서버(HostID_Server), <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a>  등을 넣을 수 있습니다.
		\param remoteCount 수신자 배열 크기
		\param rmiContext 송신 부가정보. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#protocol_overview" >ProudNet의 프로토콜 종류</a> 를 참고하십시오.
		\param payload 보낼 메시지 데이터
		\param payloadLength 보낼 메시지 데이터의 길이. 바이트 단위입니다.

		\~english
		Send user defined message. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#send_user_message" >Exchanging user-defined message without using RMI at all</a> for more detail
		\param remotes Receiver array. You can put client, server(HostID_Server), <a target="_blank" href="http://guide.nettention.com/cpp_en#p2p_group" >P2P Group</a>, etc like RMI.
		\param remoteCount Size of receiver array
		\param rmiContext Additional information of sending. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#protocol_overview" >Protocol type of ProudNet</a>.
		\param payload Message data for sending.
		\param payloadLength Length of sending message data. It is byte.

		\~chinese
		传送用户定义信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#send_user_message" >完全不使用RMI交换用户定义信息</a>%。
		\param remotes 收信者数组。与RMI相同可以输入client，服务器（HostID_Server），<a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%等。
		\param remoteCount 收信者数组大小。
		\param rmiContext 传送信息的附加信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#protocol_overview" >ProudNet 的网络协议种类</a>%。
		\param payload 要发送的信息数据。
		\param payloadLength 要发送的信息数据长度。Byte单位。

		\~japanese
		ユーザー定義メッセージを転送します。詳しくは、\ref send_user_messageをご参照ください。
		\param remotes 受信者配列です。RMIと同様に、クライアント、サーバー(HostID_Server)、 \ref p2p_group  などを入れることができます。
		\param remoteCount 受信者配列サイズ
		\param rmiContext 送信に関する付加情報。詳しくは、\ref protocol_overview をご参照ください。
		\param payload 送りメッセージデータ
		\param payloadLength 送るメッセージデータ長さ。バイト単位です。

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
		\param remote 收信者。
		\param rmiContext 传送信息的附加信息。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#protocol_overview" >ProudNet 的网络协议种类</a>%。
		\param payload 要发送的信息数据。
		\param payloadLength 要发送的信息数据长度。Byte单位。

		\~japanese
		ユーザー定義メッセージを転送します。詳しくは、\ref send_user_messageをご参照ください。
		\param remote 受信者
		\param rmiContext 送信に関する付加情報。詳しくは、\ref protocol_overview をご参照ください。
		\param payload 送るメッセージデータ
		\param payloadLength 送るメッセージデータの長さ。バイト単位です。

		\~
		*/
		inline bool SendUserMessage(HostID remote, RmiContext &rmiContext, uint8_t* payload, int payloadLength)
		{
			return SendUserMessage(&remote, 1, rmiContext, payload, payloadLength);
		}

		/** ProudNet2 모듈에서 호출됩니다. 여러분은 사용하지 마세요.*/
	/*	virtual bool IsEventCallbackPaused() = 0;
		virtual void PauseEventCallback() = 0;
		virtual void ResumeEventCallback() = 0;*/

		/** @copydoc Proud::CNetClient::SetCoalesceIntervalMs() */
		virtual ErrorType SetCoalesceIntervalMs(HostID remote, int intervalMs) = 0;
		/** @copydoc Proud::CNetClient::SetCoalesceIntervalToAuto() */
		virtual ErrorType SetCoalesceIntervalToAuto(HostID remote) = 0;

		virtual void TEST_SetOverSendSuspectingThresholdInBytes(int newVal) = 0;

		virtual void TEST_SetTestping(HostID hostID0, HostID hostID1, int ping) = 0;

		// TCP 연결이 들어오는 것을 무조건 거부한다. 연결이 들어오자마자 쫓겨나는 클라이언트를 테스트하기 위함.
		// 클라측에서 연결 거부를 미리 걸고 들어오는 것도 방법이지만, 서버에서 TCP 연결이 들어오자마자 그것을 체크하기에는 너무 늦다.
		virtual void TEST_ForceDenyTcpConnection() = 0;

		/**
		\~korean
		일반적인 온라인 게임에서는 unreliable 메시징이 전체 트래픽의 대부분을 차이지함에 따라 기존 unreliable 메시징 기능에 트래픽 로스율을 얻는 기능을 추가합니다.
		패킷 로스율 측정기
		\param remoteHostID 어느 remote 와의 통신에 대한 로스율을 얻을 걷인지. 자기 자신 peer, server 뭐든지 입력 가능합니다. 자기 자신이면 0% 입니다
		\param outputPercent 패킷 로스율을 %단위로 채워짐(즉 0~100)

		\~english 
		As unreliable messaging takes most of the entire traffic in common on-line games, we add the feature to get the traffic loss rate in the existing unreliable messaging feature.
		Packet loss rate measuring device

		\param remoteHostID Which loss rate with communication with which remote would be obtained. You can input anything, including your own peer, server, etc. If it is itself, it is 0%.
		\param outputPercent The packet loss rate is filled up in % unit (i.e., 0 to 100)

		\~chinese
		在一般在线游戏中当Unreliable信息的总体通信量占据绝大部分时，在原有Unreliable信息功能上添加获得通信量Loss率的功能。
		数据包Loss率测定仪。
		\param remoteHostID 要获得对哪一个remote间的通信Loss率，可以输入如自身Peer，server等。如果是自己的话则为0%。
		\param outputPercent  数据包的Loss率将以%单位填充，（即0~100）

		\~japanese
		一般的なオンラインゲームでは、unreliableメッセージングが全体トラフィックのほとんどを占めることによって、既存のunreliableメッセージング機能にトラフィックロス率を得る機能を追加します。
		パケットロス率測定機
		\param remoteHostID どのリモートとの通信に対するロス率を得るか？自分自身のpeer、server なんでも入力可能です。自分自身であれば0%です。
		\param outputPercent　パケットロス率が%単位で満たされます。(すなわち、0~100)

		\~
		*/
		virtual ErrorType GetUnreliableMessagingLossRatioPercent(HostID remotePeerID, int *outputPercent) = 0;

		//		virtual void SetTimerCallbackIntervalMs(int newVal) = 0;
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
