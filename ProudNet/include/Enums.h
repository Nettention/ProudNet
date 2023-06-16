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

#if ((defined(_MSC_VER) && _MSC_VER >= 1700) || (__cplusplus > 199711L))
#include "ErrorType.h"
#else 
#include "ErrorTypeOldSpec.h"
#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	// 주의: 이것도 바꾸면 ProudClr namespace의 동명 심볼도 수정해야 한다.


	/**
	\~korean
	메시지 송신 우선순위
	- <a target="_blank" href="http://guide.nettention.com/cpp_ko#message_priority" >메시지 송신 우선순위 기능</a> 참고.

	\~english 
	Message transmission priority.
	Refer to \ref message_priority.

	\~chinese
	信息传送优先顺序。
	- 参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#message_priority" >信息传送的优先顺序功能。</a>%。

	\~japanese
	メッセージ送信プライオリティ
	- \ref message_priority 参考 
	\~
	*/
	enum MessagePriority
	{
	// Most priority. Ping, for example. ProudNet internal. Do not use it.
		MessagePriority_Ring0 = 0,

	// ProudNet internal. Do not use it.
		MessagePriority_Ring1,

		/**
		\~korean
		높은 우선순위

		\~english 
		High priority

		\~chinese
		高的优先顺序。

		\~japanese
		高いプライオリティ
		\~
		*/
		MessagePriority_High,

		/**
		\~korean
		보통 우선순위. 가장 많이 사용됨.

		\~english 
		Medium priority. Used most frequently

		\~chinese
		一般优先顺序。用的最多。

		\~japanese
		普通のプライオリティ。一番多く使われます。
		\~
		*/
		MessagePriority_Medium,

		/**
		\~korean
		낮은 우선순위

		\~english 
		Low priority

		\~chinese
		低的优先顺序。

		\~japanese
		低いプライオリティ
		\~
		*/
		MessagePriority_Low,

// Lowest priority. For ProudNet internal use.
		MessagePriority_Ring99,

		// For internal use. Do not use this.
		MessagePriority_LAST,
	};

	// 홀펀칭 패킷은 많을 수 있다. 이것이 트래픽을 방해하면 안되므로 우선순위를 최하위로 둔다.
	static const MessagePriority MessagePriority_Holepunch = MessagePriority_Ring99;

	// 주의: 이것도 바꾸면 ProudClr namespace의 동명 심볼도 수정해야 한다.

	enum MessageReliability
	{
		MessageReliability_Unreliable = 0,
		MessageReliability_Reliable,
		MessageReliability_LAST,
	};

	/**
	\~korean
	ProudNet 호스트 식별자

	\~english 
	ProudNet host identifier 

	\~chinese
	ProudNet 主机识别者。

	\~japanese
	ProudNetホスト識別子
	\~
	*/
	enum HostID
	{
		/**
		\~korean
		없음

		\~english
		None

		\~chinese
		无

		\~japanese
		なし
		\~
		*/
		HostID_None = 0,

		/**
		\~korean
		서버

		\~english
		Server

		\~chinese
		服务器

		\~japanese
		サーバー
		\~
		*/
		HostID_Server = 1,

		/**
		\~korean
		(사용금지)

		\~english
		(Do not use it)

		\~chinese
		（禁止使用）

		\~japanese
		(使用禁止)
		\~
		*/
		HostID_Last = 2
	};

	inline int CompareHostID(HostID lhs, HostID rhs)
	{
		return (int)lhs - (int)rhs;
	}

	typedef uint16_t RmiID;

	enum _RmiID
	{
		RmiID_None = 0,
		RmiID_Last = 65535,
	};

	/** \~korean 모듈 내에서 발생하는 로그의 범주 타입입니다.
	\~english It is a category type of the log occurred within the module.
	\~chinese 在模块内发生的Log范畴类
	\~japanese モジュール内で発生するログのカテゴリータイプです。
	\~ */
	enum LogCategory
	{
		/** \~korean 기본 시스템
		\~english Basic System
		\~chinese 基础系统
		\~japanese 基本システム
		\~ */
		LogCategory_System,
		/** \~korean TCP 통신 관련
		\~english Relevant to TCP communication
		\~chinese 相关 TCP 通信
		\~japanese TCP通信関連
		\~ */
		LogCategory_Tcp,
		/** \~korean UDP 통신 관련
		\~english Relevant to UDP communication
		\~chinese 相关 UDP 通信
		\~japanese UDP通信関連
		\~ */
		LogCategory_Udp,
		/** \~korean P2P 통신 관련
		\~english Relevant to P2P communication
		\~chinese 相关 P2P 通信
		\~japanese P2P通信関連
		\~ */
		LogCategory_P2P
	};

	/**
	\~korean
	해킹의 종류

	\~english
	Type of hacking

	\~chinese
	黑客的种类。

	\~japanese
	ハッキング種類
	\~
	*/
	enum HackType
	{
		/**
		\~korean
		해킹 아님

		\~english
		No hacking

		\~chinese
		不是黑客。

		\~japanese
		ハッキングではない
		\~
		*/
		HackType_None,

		/**
		\~korean
		스피드핵

		\~english
		Speek hack

		\~chinese
		速度hack。

		\~japanese
		スピード核
		\~
		*/
		HackType_SpeedHack,

		/**
		\~korean
		패킷 조작

		\~english
		Handle packet

		\~chinese
		数据包操作。

		\~japanese
		パケット操作
		\~
		*/
		HackType_PacketRig,
	};

	/**
	\~korean
	타 호스트와의 연결 상태

	\~english
	Connection status with other hosts

	\~chinese
	与其他主机的连接状态。

	\~japanese
	他のホストとの接続状態
	\~
	*/
	enum ConnectionState
	{
		/**
		\~korean
		연결이 끊어진 상태

		\~english
		Disconnected condition

		\~chinese
		连接中断的状态。

		\~japanese
		接続が切れた状態
		\~
		*/
		ConnectionState_Disconnected,

		/**
		\~korean
		연결 시도를 했지만 아직 결과를 알 수 없는 상태

		\~english
		Tried connecting but result is unknown

		\~chinese
		虽已试图连接，但无法知道结果的状态。

		\~japanese
		接続を試しましたが、まだ結果が知らない状態
		\~
		*/
		ConnectionState_Connecting,

		/**
		\~korean
		연결 과정이 성공한 상태

		\~english
		Succeed to connect

		\~chinese
		连接过程成功的状态。

		\~japanese
		接続過程に成功した状態
		\~
		*/
		ConnectionState_Connected,

		/**
		\~korean
		연결 해제 과정이 진행중인 상태

		\~english
		Disconnecting is in progress

		\~chinese
		在进行连接解除过程的状态。

		\~japanese
		接続解除過程が進行中の状態
		\~
		*/
		ConnectionState_Disconnecting,
	};

	 PROUD_API const PNTCHAR* ToString(LogCategory logCategory);
	 PROUD_API const PNTCHAR* ToString(ConnectionState val);

	/**
	\~korean
	TCP fallback을 의도적으로 시행할 때의 방법

	\~english
	How to intentially use TCP fallback

	\~chinese
	故意实行TCP fallback时的方法。

	\~japanese
	TCP Fallbackを意図的に施行する時の方法
	\~
	*/
	enum FallbackMethod // 강도가 낮은 순으로 enum 값을 정렬할 것
	{
		/**
		\~korean
		Fallback을 안함. 즉 서버 및 peer와의 UDP 통신을 모두 사용함.

		\~english
		No Fallback. In other words, UDP communication to both server and peer are in use.

		\~chinese
		不做Fallback。即使用全部的服务器及与peer的UDP通信。

		\~japanese
		Fallbackをしない。即ち、サーバーおよびPeerとのUDP通信を全て使用します。
		\~
		*/
		FallbackMethod_None,
		/**
		\~korean
		서버와의 UDP 통신은 유지하되 타 Peer들과의 UDP 통신만이 차단된 것으로 처리한다. 일시적 포트매핑 실패와 유사한 상황을 재현한다. 가장 강도가 낮다.

		\~english
		Regards that UDP with server is sustained but UDP with other peers to be disconnected. Reproduce a circumstance similar to a temporary port mapping failure. This is the lowest option with weakest impact.

		\~chinese
		维持与服务器的UDP通信，处理为只与其他peer的UDP通信中断。再现一时的端口映像失败和类似的情况。强度最低。

		\~japanese
		サーバーとのUDP通信は維持するが、他のPeerたちとのUDP通信のみが遮断されたものと処理します。一時的にポートマッピング失敗と類似した状況を再現します。一番強度が低いです。
		\~
		*/
		FallbackMethod_PeersUdpToTcp,
		/**
		\~korean
		서버와의 UDP 통신을 차단된 것으로 처리한다. 일시적 포트매핑 실패와 유사한 상황을 재현한다. 아울러 Peer들과의 UDP 통신도 차단된다. 중간 강도다.

		\~english
		 Regards that UDP with server is disconnected. Reproduce a circumstance similar to a temporary port mapping failure. On top of that, it also disconnects UDP with peers. Intermediate impact.


		\~chinese
		处理为只与其他peer的UDP通信中断。再现一时的端口映像失败和类似的情况。与peer的UDP通信也被中断。强度一般。

		\~japanese
		サーバーとのUDP通信を遮断されたものと処理します。一時的にポートマッピング失敗と類似した状況を再現します。ちなみに、PeerたちとのUDP通信も遮断されます。中間強度です。
		\~
		*/
		FallbackMethod_ServerUdpToTcp,
		/**
		\~korean
		클라이언트의 UDP 소켓을 아예 닫아버린다. 영구적인 UDP 복구를 못하게 한다. 가장 강도가 높다.
		- Proud.CNetServer.SetDefaultFallbackMethod 에서는 사용할 수 없다.

		\~english
		 All UDP sockets of client will be shut down. UDP restoration will never be possible. Strongest impact.
		- Unable to use in Proud.CNetServer.SetDefaultFallbackMethod

		\~chinese
		直接关闭玩家的UDPsocket。不让做永久性的UDP修复。强度最高。

		\~japanese
		クライアントのUDPソケットを閉じます。永久的なUDP復旧ができなくします。一番強度が高いです。
		- Proud.CNetServer.SetDefaultFallbackMethod では使用できません。
		\~
		*/
		FallbackMethod_CloseUdpSocket,
	};

	/**
	\~korean
	클라이언트간 직접 P2P 통신을 위한 홀펀칭을 시작하는 조건

	\~english
	Conditions to start hole-punching for direct P2P communication among clients

	\~chinese
	为了玩家之间开始直接P2P通信的打洞条件。

	\~japanese
	クライアント間の直接P2P通信のためのホールパンチングを開始する条件
	\~
	*/
	enum DirectP2PStartCondition
	{
		/**
		\~korean
		꼭 필요한 상황이 아닌 이상 홀펀칭을 하지 않는다. 웬만하면 이것을 쓰는 것이 좋다.

		\~english
		Unless really needed, it is recommended not to do hole-punching. Using this is strongly recommended.

		\~chinese
		不是必要的情况的话不进行打洞。最好用这个。

		\~japanese
		必ず必要な状況ではない以上、ホールパンチングはしません。なぜなら、これを使った方が良いです。
		\~
		*/
		DirectP2PStartCondition_Jit,
		/**
		\~korean
		CNetServer.CreateP2PGroup 이나 CNetServer.JoinP2PGroup 등에 의해 클라이언트간 P2P 통신이 허용되는 순간에 무조건
		홀펀칭 과정을 시작한다. 예를 들어 <a target="_blank" href="http://guide.nettention.com/cpp_ko#super_peer" >Super Peer(수퍼피어 혹은 호스트) 중심의 P2P 네트워킹</a>  에서 수퍼 피어를 게임 플레이 도중 종종 바꿔야 한다면 이것이
		필요할 수도 있을 것이다.

		\~english
		This forcefully begins hole-punching at the moment when P2P communication among clients is allowed by CNetServer.CreateP2PGroup or CNetServer.JoinP2PGroup or others.
		For an example, if there is a need to change super peer at <a target="_blank" href="http://guide.nettention.com/cpp_en#super_peer" >P2P Networking for Super Peer (Super Peer or host)</a> during game play, this may be needed.

		\~chinese
		被 CNetServer.CreateP2PGroup%或者 CNetServer.JoinP2PGroup ，玩家之间P2P通信允许的瞬间一定要开始打洞过程。例如在<a target="_blank" href="http://guide.nettention.com/cpp_zh#super_peer" >Super Peer(或主机) 中心的P2P通信</a>%，游戏进行中要把super peer改变时可能需要这个。

		\~japanese
		CNetServer.CreateP2PGroupとかCNetServer.JoinP2PGroupなどによってクライアント間のP2P通信が許容される瞬間に無条件ホールパンチング過程を開始します。例えば、\ref super_peerでスーパーピーアをゲームプレイ途中に偶に変えなければならないとしたら、これが必要になるかもしれません。

		\~
		*/
		DirectP2PStartCondition_Always,
		DirectP2PStartCondition_LAST,
	};
	/**  @} */

	/**
	\~korean
	사용자 실수로 인한 에러를 ProudNet이 보여주는 방법

	\~english
	The way Proudnet shows error caused by user

	\~chinese
	ProudNet 显示由用户的失误造成的错误的方法。

	\~japanese
	ユーザー間違いによるエラーをProudNetが表示する方法
	\~
	*/
	enum ErrorReaction
	{
		/**
		\~korean
		대화 상자를 보여준다. 개발 과정에서는 요긴하지만 엔드유저 입장에서는 풀스크린인 경우 프리징으로 보일 수 있어서 되레 문제 찾기가 더 어려울 수 있다.

		\~english
		Shows chat box. This may be useful during development process but it can also be very troublesome since the chat box can be seen to end users as full screen freezing.

		\~chinese
		显示对话框。虽在开发过程中非常重要，但对于最终用户在全屏中可能会视其为freezing，查找问题可能反而更加困难。

		\~japanese
		ダイアログボックスを表示します。開発過程では必要ですが、エンドユーザーの立場ではプールスクリーンの場合、Freezingに見えることがあり、かえって問題を探すのが大変になることもあります。

		\~
		*/
		ErrorReaction_MessageBox,
		/**
		\~korean
		크래시를 유발한다. 엔드유저 입장에서는 크래시로 나타나므로 릴리즈된 경우 문제를 더 쉽게 찾을 수 있다.
		단, <a target="_blank" href="http://guide.nettention.com/cpp_ko#minidump_main" >MiniDump (오류덤프시스템)</a>  등을 혼용해야 그 가치가 있다.

		\~english
		Causes a crash. From end users' point of view, this is definitely a crash so it can be rather easier to find what the problem is when released.
		But, it is more effective when used along with <a target="_blank" href="http://guide.nettention.com/cpp_en#minidump_main" >Error Dump System</a>.

		\~chinese
		诱发crash。在最终用户显示为crash，可能更容易找到release的原因。
		但要混用 <a target="_blank" href="http://guide.nettention.com/cpp_zh#minidump_main" >错误转储系统</a>

		\~japanese
		クラッシュを誘発します。エンドユーザーの立場ではクラッシュで現れますので、リリースされた場合、問題をもっと探しやすいかもしれません。但し、\ref minidumpなどを混用してその価値があります。
\~ */
		ErrorReaction_AccessViolation,
		/**
		\~korean
		디버거 출력창에서만 보여집니다. 크래시나 대화상자를 띄워줄만한 상황이 아니라면 이옵션을 사용하십시오.

		\~english
		It shows only debugger output screen. If you can not pop up crash or message box, please use this option.

		\~chinese
		只显示在调试输出窗。不是显示crash或者对话框的情况的话，请使用此选项。

		\~japanese
		デバッガー出力画面でのみ表示されます。クラッシュやダイアログボックスを表示する状況ではなければこのオプションを使ってください。
		\~
		*/
		ErrorReaction_DebugOutput,
		/**
		\~korean
		디버거 브레이크를 겁니다. 해당옵션은 디버그 모드에서만 사용하십시오.
		\~english
		Put on the debugger brake. Please use this option only in the debugging mode. 

		\~japanese
		デバッガーブレーキをかけます。該当オプションはデバッグモードでのみ使ってください。
		\~
		 */
		ErrorReaction_DebugBreak
	};

	/**
	\~korean
	연산 종류

	\~english
	Operation type

	\~chinese
	运算种类。

	\~japanese
	演算種類
	\~
	*/
	enum ValueOperType
	{
		/**
		\~korean
		덧셈

		\~english
		Addition

		\~chinese
		加法。

		\~japanese
		足し算
		\~
		*/
		ValueOperType_Plus,
		/**
		\~korean
		뺄셈

		\~english
		Subtraction

		\~chinese
		减法

		\~japanese
		引き算
		\~
		*/
		ValueOperType_Minus,
		/**
		\~korean
		곱셈

		\~english
		Multiplication

		\~chinese
		乘法。

		\~japanese
		掛け算
		\~
		*/
		ValueOperType_Multiply,
		/**
		\~korean
		나눗셈

		\~english
		Division

		\~chinese
		除法。

		\~japanese
		割り算
		\~
		*/
		ValueOperType_Division
	};

	/**
	\~korean
	비교 종류

	\~english
	Comparison type

	\~chinese
	比较种类。

	\~japanese
	比較種類
	\~
	*/
	enum ValueCompareType
	{
		/**
		\~korean
		크거나 같으면

		\~english
		Greater or equal

		\~chinese
		大或等于。

		\~japanese
		大きいか等しければ
		\~
		*/
		ValueCompareType_GreaterEqual,

		/**
		\~korean
		크면

		\~english
		Greater

		\~chinese
		大的话。

		\~japanese
		大きければ
		\~
		*/
		ValueCompareType_Greater,

		/**
		\~korean
		작거나 같으면

		\~english
		Less or equal

		\~chinese
		小或等于。

		\~japanese
		小さいか等しければ
		\~
		*/
		ValueCompareType_LessEqual,

		/**
		\~korean
		작으면

		\~english
		Less

		\~chinese
		小的话。

		\~japanese
		小さければ
		\~
		*/
		ValueCompareType_Less,

		/**
		\~korean
		같으면

		\~english
		Equal

		\~chinese
		等于。

		\~japanese
		等しければ
		\~
		*/
		ValueCompareType_Equal,

		/**
		\~korean
		다르면

		\~english
		Not equal

		\~chinese
		不同的话。

		\~japanese
		違えば
		\~
		*/
		ValueCompareType_NotEqual
	};

	/**
	\~korean
	암호화 수준

	\~english
	Encryption Level

	\~chinese
	加密水平。

	\~japanese
	暗号化水準
	\~
	*/
	enum EncryptLevel
	{
		/**
		\~korean
		AES 암호화 수준 하

		\~english
		AES encryption level Low

		\~chinese
		AES 加密水平低。

		\~japanese
		AES暗号化水準　下
		\~
		*/
		EncryptLevel_Low = 128,
		/**
		\~korean
		AES 암호화 수준 중

		\~english
		AES encryption level Middle

		\~chinese
		AES 加密水平中。

		\~japanese
		AES暗号化水準　中
		\~
		*/
		EncryptLevel_Middle = 192,
		/**
		\~korean
		AES 암호화 수준 상

		\~english
		AES encryption level High

		\~chinese
		AES 加密水平上。

		\~japanese
		AES暗号化水準　上
		\~
		*/
		EncryptLevel_High = 256,
	};

	/**
	\~korean
	Fast 암호화 수준

	\~english
	Fast encryption level

	\~chinese
	Fast 加密水平。

	\~japanese
	Fast暗号化水準
	\~
	*/
	enum FastEncryptLevel
	{
		/**
		\~korean
		Fast 암호화 수준 하

		\~english
		Fast encryption level Low

		\~chinese
		Fast加密水平低。

		\~japanese
		Fast暗号化水準　下
		\~
		*/
		FastEncryptLevel_Low = 512,
		/**
		\~korean
		Fast 암호화 수준 중

		\~english
		Fast encryption level Middle

		\~chinese
		Fast加密水平中。

		\~japanese
		Fast暗号化水準　中
		\~
		*/
		FastEncryptLevel_Middle = 1024,
		/**
		\~korean
		Fast 암호화 수준 상

		\~english
		Fast encryption level High

		\~chinese
		Fast加密水平上。

		\~japanese
		Fast暗号化水準　上
		\~
		*/
		FastEncryptLevel_High = 2048,
	};

	enum HostType
	{
		HostType_Server,
		HostType_Peer,
		HostType_All,
	};

	/**
	\~korean
	HostID 재사용 기능 옵션입니다.

	\~english
	It is an option for HostID reuse function.

	\~chinese
	HostID是再使用技能选项。

	\~japanese
	HostID再使用機能オプションです。

	\~
	*/
	enum HostIDGenerationPolicy
	{
		/**
		\~korean
		HostID 재사용 기능을 사용합니다.
		-기본값입니다.

		\~english
		It uses a HostID reuse function.
		-It is a default value.

		\~chinese
		HostID 使用再使用技能。
		-为基本值。.

		\~japanese
		HostID再使用機能を使います。
		‐デフォルト値です

		\~
		*/
		HostIDGenerationPolicy_Recycle = 1,

		/**
		\~korean
		HostID 재사용 기능을 사용하지 않습니다.

		\~english
		It does not use a HostID reuse function.

		\~chinese
		HostID 不使用再使用技能。.

		\~japanese
		HostID再使用機能を使いません。

		\~
		*/
		HostIDGenerationPolicy_NoRecycle,
	};

	/**
	\~korean
	스레드 모델입니다.

	\~english
	Thread model.

	\~chinese
	线程模型。

	\~japanese
	スレッドモデルです。

	\~
	*/
	enum ThreadModel
	{
		/**
		\~korean
		단일 스레드 모델입니다. 사용자가 만든 스레드 위에서 작동함을 의미합니다.
		따로 스레드가 없으므로 사용자는 Proud::CNetClient::FrameMove를 최소 매 1/60초마다 지속적으로 호출해 주어야 합니다.

		\~english
		Single thread model. Runs code on threads created by user.
		As there is no networker thread, You must call Proud::CNetClient::FrameMove for every 1/60 seconds.

		\~chinese
		单线程模型。运行在用户创建的线程池。
		由于没有独立的线程，用户需要最少 1/60 秒间隔持续调用 Proud::CNetClient::FrameMove。

		\~japanese
		シングルスレッドモデルです。ユーザーが作成したスレッド上で動作することを意味します。
		別にスレッドがないため、ユーザーはProud :: CNetClient :: FrameMoveを最小枚1/60秒ごとに継続的に呼び出す必要があります。

		\~
		*/
		ThreadModel_SingleThreaded = 1,

		/**
		\~korean
		멀티 스레드 모델입니다. Thread pool을 따로 만들고 그 위에서 작동함을 의미합니다.

		\~english
		Multi thread model. Runs code on the separated thread pool.

		\~chinese
		多线程模型。单独创建线程池并运行在此上。

		\~japanese
		マルチスレッドモデルです。 Thread poolを別々に作成し、その上で動作することを意味します。
		\~
		*/
		ThreadModel_MultiThreaded,

		/**
		\~korean
		외장 스레드 모델입니다. 사용자가 만든 Proud.CThreadPool 위에서 작동함을 의미합니다.

		\~english
		Use-external-thread model. Runs code on Proud.CThreadPool instance create by user.

		\~chinese
		外部线程模型。运行在用户创建的 Proud.CThreadPool。

		\~japanese
		外装スレッドモデルです。ユーザーが作成したProud.CThreadPool上で動作することを意味します。

		\~
		*/
		ThreadModel_UseExternalThreadPool,
	};

#if defined(SUPPORTS_CPP11) || defined(SWIG)
	/** 웹소켓의 종류입니다. */
	enum WebSocketType
	{
		/// 웹소켓을 사용하지 않습니다.
		WebSocket_None,

		/// 웹소켓을 사용합니다.
		WebSocket_Ws,

		/// 웹소켓을 사용하며 보안 네트워킹(SSL)를 합니다.
		WebSocket_Wss,
	};
#endif


	namespace Retained
	{
		/**
		\typedef EntityID

		EntityReplica 기능의 Entity가 가지고 있는 고유 식별자입니다.
		*/
		enum class EntityID :int
		{
			None = 0,
		};


		inline int CompareEntityID(EntityID lhs, EntityID rhs)
		{
			static_assert(sizeof(EntityID) == sizeof(int), "Do you really need >2 billion Entities in a Room?");
			return (int)lhs - (int)rhs;
		}
	}

}

CPNElementTraits_FOR_ENUM(::Proud::Retained::EntityID);
CPNElementTraits_FOR_ENUM(::Proud::HostID);

#ifdef _MSC_VER
#pragma pack(pop)
#endif
