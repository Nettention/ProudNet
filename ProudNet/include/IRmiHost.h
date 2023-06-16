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

//#define TEST_DISCONNECT_CLEANUP 테스트 용도


#include "BasicTypes.h"
//#include "ConnectParam.h"
#include "HostIDArray.h"
#include "AddrPort.h"
#include "Enums.h"
#include "EncryptEnum.h"
#include "RMIContext.h"
#include "ErrorInfo.h"
#include "LambdaEventHandler.h"

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

	class CMessage;	
	class CSendFragRefs;
	class ErrorInfo;
	class IRmiProxy;
	class IRmiStub;
	class RmiContext;
	class MessageSummary;

	struct SendOpt;

	/** 
	\~korean
	\brief ProudNet RMI가 참조하는 네트워크 호스트 인터페이스입니다.
	유저는 이 클래스를 직접 사용할 필요가 없습니다. 

	\~english
	\brief ProudNet Network host interface that is referred by RMI.
	User does not have to use this class directly.

	\~chinese
	\brief ProudNet RMI参照的联网主机界面。
	用户不用直接使用此类。

	\~japanese
	\brief ProudNet RMIが参照するネットワークホストインターフェースです。
	ユーザーはこのクラスを直接使う必要がありません。
	\~
	*/
	class  IRmiHost
	{
		friend class IRmiProxy;
	public:
		virtual ~IRmiHost() {}

		/** 
		\~korean
		PIDL 컴파일러의 결과물 중 proxy를 이 객체에 등록한다. 

		\~english
		Registers proxy among the results of PIDL compiler to this object

		\~chinese
		PIDL 编译器的产物中把proxy登录到此对象。

		\~japanese
		PILDコンパイラーの結果物のうちproxyをこのオブジェクトに登録します。
		\~
		*/
		virtual void AttachProxy(IRmiProxy* proxy) = 0;
		// no-throw version for UE4, etc.
		void AttachProxy(IRmiProxy *proxy, ErrorInfoPtr& outError);

		/** 
		\~korean
		PIDL 컴파일러의 결과물 중 stub을 이 객체에 등록한다. 

		\~english
		Registers stub among the results of PIDL compiler to this object

		\~chinese
		PIDL 编译器产物中把stub登录到此对象。

		\~japanese
		PILDコンパイラーの結果物のうちstubをこのオブジェクトに登録します。
		\~
		*/
		virtual void AttachStub(IRmiStub* stub) = 0;
		// no-throw version for UE4, etc.
		void AttachStub(IRmiStub* stub, ErrorInfoPtr& outError);
		/** 
		\~korean
		AttachProxy 의 반대로서, 이 객체에서 등록 해제를 한다. 

		\~english
		As opposite to AttachProxy, this object cancels the registration.

		\~chinese
		是AttachProxy的相反，解除在此对象的登录。

		\~japanese
		AttachProxyの反対として、このオブジェクトで登録解除をします。
		\~
		*/
		virtual void DetachProxy(IRmiProxy* proxy) = 0;
		// no-throw version for UE4, etc.
		void DetachProxy(IRmiProxy* proxy, ErrorInfoPtr& outError);
		/** 
		\~korean
		AttachStub 의 반대로서, 이 객체에서 등록 해제를 한다. 

		\~english
		As opposite to AttachStub, this object cancels the registration.

		\~chinese
		是AttachStub的相反，解除在此对象的登录。

		\~japanese
		AttachStubの反対として、このオブジェクトで登録解除をします。
		\~
		*/
		virtual void DetachStub(IRmiStub* stub) = 0;
		// no-throw version for UE4, etc.
		void DetachStub(IRmiStub* stub, ErrorInfoPtr& outError);

		// Set any value you wish. For example, owner of this object.
		virtual void SetTag(void* value) = 0;
		virtual void* GetTag() = 0;

#ifdef SUPPORTS_LAMBDA_EXPRESSION
	public:
		/**
		
		사용자 정의 함수나 람다식을 비동기로 실행합니다. 자세한 것은 \page run_async를 참고하십시오.

		\param taskOwner 어떤 Host를 위한 비동기 실행인지를 가리킵니다.
		\param func 함수 혹은 람다식입니다. 일반적으로 람다식의 사용을 권장합니다.
		\return taskOwner가 유효하면 true를 리턴합니다. false를 리턴할 경우 func 는 실행되지 않습니다.
		*/
		inline bool RunAsync(HostID taskOwner, const std::function<void()> &func)
		{
			return RunAsyncInternal(taskOwner, new Lambda_Param0<void>(func)); // new 로 생성한 것은 구현부 내부에서 delete하게 만들어져 있다. 따라서 안전하다.
		}

#endif
		// 람다식 미지원 컴파일러에서는 못 쓰지만, 가상 함수는 실존 멤버이므로, 
		// 조건부 컴파일 블럭 바깥에 있어야 한다.
		virtual bool RunAsyncInternal(HostID taskOwner, LambdaBase_Param0<void>* func) = 0;

	protected:
		// 내부 용도 함수이다. 사용자는 호출하지 말 것.
		// 이 함수에 대한 설명은 CNetClientImpl에 있음
		virtual bool Send(const CSendFragRefs &sendData,
			const SendOpt& sendContext,
			const HostID *sendTo, int numberOfsendTo, int &compressedPayloadLength) = 0;

	public:

		/** 
		\~korean
		simple packet mode를 사용하고 있는지 확인한다.

		\~english
		Check to see whether simple packet mode is being used. 

		\~chinese
		确认是否使用simple packet mode。

		\~japanese
		simple packet modeを使っているかを確認します。
		\~
		*/
		virtual bool IsSimplePacketMode() = 0;

		/** 
		\~korean
		내부 함수. 사용자는 호출 금지. 

		\~english
		Internal function. User must not call this.

		\~chinese
		内部函数。用户禁止呼叫。

		\~japanese
		内部関数。ユーザーは呼び出し禁止
		\~
		*/
		virtual void ShowError_NOCSLOCK( ErrorInfoPtr errorInfo ) = 0;
		virtual void ShowNotImplementedRmiWarning(const PNTCHAR* RMIName) = 0;
		virtual void PostCheckReadMessage(CMessage& msg, const PNTCHAR* RMIName) = 0;

		virtual CriticalSection& GetCriticalSection() = 0;

#ifdef VIZAGENT
		virtual void EnableVizAgent(const PNTCHAR* vizServerAddr, int vizServerPort, const String &loginKey) = 0;

		virtual void Viz_NotifySendByProxy(const HostID* remotes, int remoteCount, const MessageSummary &summary, const RmiContext& rmiContext) = 0;
		virtual void Viz_NotifyRecvToStub(HostID sender, RmiID RmiID, const PNTCHAR* RmiName, const PNTCHAR* paramString) = 0;
#endif
	};

	/** 
	\~korean
	\brief clientWorker의 정보를 담는 구조체. 

	\~english
	\brief Structure that contain information of clientWorker

	\~chinese
	\brief 包含clientWorker信息的构造体。

	\~japanese
	\brief clientWorkerの情報を入れる構造体 
	\~
	*/
	class CClientWorkerInfo
	{
	public:

		//bool m_isWorkerThreadEnded;

		int m_connectCallCount;

		int m_disconnectCallCount;

		ConnectionState m_connectionState;

		int m_finalWorkerItemCount;

//		int64_t m_lastTcpStreamTimeMs;

		int64_t m_currentTimeMs;

		int m_peerCount;
#if !defined(_WIN32)
		pthread_t m_workerThreadID;
#else
		uint32_t m_workerThreadID;
#endif
	};

	/** 
	\~korean
	\brief socket의 정보를 담는 구조체입니다.

	\~english
	\brief Structure that contain information of socket

	\~chinese
	\brief 包含socket信息的构造体。

	\~japanese
	\brief socketの情報を入れる構造体です。
	\~
	*/
	class CSocketInfo
	{
	public:
		/** 
		\~korean
		TCP socket handle입니다. 서버와의 연결을 위한 socket입니다. 

		\~english
		It is TCP socket handle. This socket is for connecting to server

		\~chinese
		是TCP socket handle。为了与服务器连接的socket。

		\~japanese
		TCP socket handleです。サーバーとの接続のためのsocketです。
		\~
		*/
		SOCKET m_tcpSocket;

		/** 
		\~korean
		UDP socket handle입니다. 서버 혹흔 P2P peer와의 연결을 위한 socket입니다. 

		\~english
		It is UDP socket handle. This socket is for connecting to server or P2P peer.

		\~chinese
		是UDP socket handle。为了与服务器或者P2P peer的连接的socket。

		\~japanese
		UDP socket handleです。サーバーまたはP2P peerとの接続のためのsocketです。 
		\~
		*/
		SOCKET m_udpSocket;

		inline CSocketInfo()
		{
			m_tcpSocket = InvalidSocket;
			m_udpSocket = InvalidSocket;
		}
	};

	/** 
	\~korean
	\brief 1개의 스레드에 대한 정보가 담겨져 있는 구조체. 

	\~english
	\brief Construct contains information of 1 thread

	\~chinese
	\brief 包含对一个线程信息的构造体。

	\~japanese
	\brief 1個のスレッドに関する情報が入っている構造体 
	\~
	*/
	class CThreadInfo
	{
	public:
		/**
		\~korean
		Thread의 ID 

		\~english
		ID of thread

		\~chinese
		Thread的ID。

		\~japanese
		ThreadのID 
		\~
		*/
		uint64_t m_threadID;

		/**
		\~korean
		Thread의 Handle 

		\~english
		Handle of thread

		\~chinese
		Thread的Handle。

		\~japanese
		ThreadのHandle 
		\~
		*/
#if defined(_WIN32)
		HANDLE m_threadHandle;
#else
		pthread_t m_threadHandle;
#endif
	};

	/** 
	\~korean
	\brief 사용자 프로그램에서 엔진에 전달하는 입력 파라메터 

	\~english
	Input parameter delivered from \brief user program to engine

	\~chinese
	\brief 在用户程序中，往引擎传达的输入参数。

	\~japanese
	\brief ユーザープログラムでエンジンに伝達する入力パラメーター 
	\~
	*/
	class CApplicationHint 
	{
	public:
		/** 
		\~korean
		사용자가 측정한, 응용 프로그램의 프레임 레이트입니다. 

		\~english
		Frame rate of application program measured by user

		\~chinese
		用户测定的，应用程序的帧速率。

		\~japanese
		ユーザーが測定したアプリケーションプログラムのフレームレートです。 
		\~
		*/
		double m_recentFrameRate; 

		CApplicationHint()
		{
			m_recentFrameRate = 0;
		}
	};

	/** 
	\~korean
	\brief 통계 정보. 디버깅이나 성능 측정을 위함 

	\~english
	\brief statistics information. For debugging or performance test

	\~chinese
	\brief 统计信息。为了调试或性能测定。

	\~japanese
	\brief 統計情報。デバッギングとか性能測定のためのものです。
	\~
	*/
	class ReliableUdpHostStats
	{
	public:
		// 수신 윈도에 들어있는 프레임 갯수, 아직 뽑아내지 않은 스트림 크기, 여지껏 받은 스트림 크기, 여지껏 받은 프레임 갯수, 최근에 프레임 받는 갯수의 속도
		int m_receivedFrameCount, m_receivedStreamCount, m_totalReceivedStreamLength, m_totalAckFrameCount;
		int m_senderWindowLength;
		int m_senderWindowMaxLength;
		int m_senderWindowWidth;	// first~last의 number 차이
		int m_expectedFrameNumberToReceive;
		int m_nextFrameNumberToSend;

		/** 
		\~korean
		송신큐에 쌓여있는 스트림 크기

		\~english
		Stream size that stacked at sending queue

		\~chinese
		堆积在传送queue的流大小。

		\~japanese
		送信キューに積もってあるストリームサイズ
		\~
		*/
		int m_sendStreamCount;

		/** 
		\~korean
		초송신 윈도에 들어있는 프레임 갯수

		\~english
		Number of frame that located in first sending window

		\~chinese
		在初始传送窗的帧个数。

		\~japanese
		初送信ウィンドウに入っているフレーム数
		\~
		*/
		int m_senderWindowDataFrameCount;
		
        /**
		\~korean
		재송신 윈도에 들어있는 프레임 갯수

		\~english
		Number of frame that located in resend window

		\~chinese
		在再次传送窗的帧个数。

		\~japanese
		再送信ウィンドウに入っているフレーム数
		\~
		*/
		int m_resendFrameCount;

		/** 
		\~korean
		여지껏 보낸 스트림 크기

		\~english
		Total size of sent stream

		\~chinese
		至今发送的流大小。

		\~japanese
		これまで送ったストリームサイズ
		\~
		*/
		int m_totalSendStreamLength;

		/** 
		\~korean
		여지껏 초송신한 프레임 갯수
		- 초송신한 프레임 갯수에 비해 여지껏 재송신한 프레임 갯수가 지나치게 증가하면 P2P Reliable 메시징이 잘 이루어지지 않음을 의미한다.

		\~english
		Total number of first send frame
		- If number of resent frame is dramarically increased compare to number of first send frame, it mean, it does not working P2P Reliable messaing unstably

		\~chinese
		至今初始传送的帧个数。
		- 比起初始传送的帧个数，至今传送的帧个数过分增加的话，意味着P2P Reliable messaging没有很好的完成。

		\~japanese
		これまで初送信したフレーム数
		- 初送信したフレーム数に比べ、これまで再送信したフレーム数が増加し過ぎたら、P2P Reliableメッセージングがよく行われないことを意味します。
		\~
		*/
		int m_totalFirstSendCount;

		/** 
		\~korean
		여지껏 재송신한 프레임 갯수

		\~english
		Total number of resent frame

		\~chinese
		至今再次传送的帧个数。

		\~japanese
		これまで再送信したフレーム数
		\~
		*/
		int m_totalResendCount;

		/// if zero, it means nothing to show, or the number is zero indeed.
		int m_senderWindowLastFrame;

		int m_totalReceiveDataCount;
		int m_lastReceivedAckNumber;
		String ToString();
	};

	// 주의: 이걸 여러 스레드에서 너무 자주 접근할 시,멀티코어 환경에서 메인보드상에서의 병목이 유발되더라.
	class CTestStats
	{
	public:
		static double TestAllowedMaxSendSpeed;
		static double TestRecentReceiveSpeed;
		static double TestRecentSendSpeedAtReceiverSide;
	};

	// 주의: 이걸 여러 스레드에서 너무 자주 접근할 시,멀티코어 환경에서 메인보드상에서의 병목이 유발되더라.
	class CTestStats2
	{
	public:
		bool m_C2SReplyRecentReceiveSpeedDone;
		bool m_C2CRequestRecentReceiveSpeedDone;
		bool m_C2CReplyRecentReceiveSpeedDone;

		CTestStats2()
		{
			m_C2SReplyRecentReceiveSpeedDone = false;
			m_C2CRequestRecentReceiveSpeedDone = false;
			m_C2CReplyRecentReceiveSpeedDone = false;
		}
	};

	/** 
	\~korean
	Disconnect()에서 사용하는 인자입니다.

	\~english TODO:translate needed.
	This is the factor used in Disconnect()

	\~chinese
	Disconnect()使用的因子。
	\~japanese
	Disconnect()で使う因子です。
	\~
	*/
	class CDisconnectArgs
	{
	public:
		/**
		\~korean
		graceful disconnect를 수행하는데 걸리는 최대 시간입니다.
		이 시간을 넘어서면 Proud.CNetClient.Disconnect()는 무조건 return하게 되고, 서버에서는 클라이언트의 연결 해제를 
		즉시 인식하지 못합니다. 
		꼭 필요한 경우가 아니면 이 값을 변경하지 마십시오. 

		\~english TODO:translate needed.
		If you exceed this time, Proud.CNetClient.Disconnect() unconditionally returns and the server cannot immediately recognize the disconnection of the client. 
		Unless absolutely necessary, do not change this value. 

		\~chinese
		执行graceful disconnect所消耗的最长时间。
		如果超出此时间，则 Proud.CNetClient.Disconnect()%将会随机return，服务器将无法立刻识别客户端的连接解除。
		如不是必要情况，请匆变更此值。

		\~japanese
		graceful disconnectを遂行するにかかる最大時間です。
		この時間が経過したらProud.CNetClient.Disconnect()は無条件でリターンされて、サーバーではクライアントの接続解除を直ちに認識できなくなります。 
		必ず必要な場合ではなければ、この値を変更しないでください。
		\~
		*/
		int64_t m_gracefulDisconnectTimeoutMs;

		/**
		\~korean
		이것으로 Disconnect시의 대기하는 Sleep 시간을 조절할 수 있습니다.

		\~english TODO:translate needed.
		With this, you can adjust the sleep time waiting during disconnection. 

		\~chinese TODO:translate needed.
		通过这个可以调整断开时等待的Sleep时间。

		\~japanese TODO:translate needed.
		これでDisconnect時の待機するSleep時間を調節することができます。
		\~
		*/
		uint32_t m_disconnectSleepIntervalMs;

		/**
		\~korean
		서버에서 클라이언트 연결 해제 사유를 이것을 통해 전송할 수 있습니다.

		\~english TODO:translate needed.
		In the server, you can transmit the client disconnection reason through this. 

		\~chinese
		服务器将通过此传送客户端连接解除原因。
		\~japanese
		サーバーでクライアント接続解除の事由を、これを通じ転送することができます。
		\~
		*/
		ByteArray m_comment;

		PROUD_API 	CDisconnectArgs();
	};

	

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
