#pragma once

#include "NetCoreStats.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/**
	\~korean
	클라이언트의 상황 통계 정보입니다.
	성능 측정 등에서 사용됩니다.
	
	\~english
	Client status information.
	Used for performance check.
	
	\~chinese
	客户端状态统计信息。
	用于性能检测。
	
	\~japanese
	クライアントの状況の統計情報です。
	パフォーマンスの測定等で使われています。
	*/
	class CNetClientStats :public CNetCoreStats
	{
	public:

		/**
		\~korean
		현재 연결되어있는 remote peer의 갯수

		\~english
		Number of remote peer that in currently connected

		\~chinese
		现在已连接的remote peer的个数。

		\~japanese
		現在つながっているremote peerの数
		\~
		*/
		uint32_t m_remotePeerCount;

		/**
		\~korean
		true이면 서버와의 UDP 통신이 정상임을 의미한다.
		false이면 UDP를 쓰지 못하며, 서버와의 unreliable 메시징도 TCP로 주고받아지고 있음을 의미한다.

		\~english
		true means the UDP communication with server is ok
		Cannot use UDP when false and also means the unreliable messaging with server is done via TCP.

		\~chinese
		True的话意味着与服务器的UDP通信正常。
		False的话不能使用UDP，意味着与unreliable messaging也不能用TCP传接。

		\~japanese
		TrueであればサーバーとのUDP通信が正常であることを意味します。
		FalseであればUDPを使えず、サーバーとのunreliableメッセージングもTCPでやり取りすることを意味します。
		\~
		*/
		bool m_serverUdpEnabled;

		/**
		\~korean
		Direct P2P가 되어있는 remote peer의 갯수

		\~english
		Number of remote peer that consisted Direct P2P

		\~chinese
		Direct P2P的remote peer个数。

		\~japanese
		Direct P2Pになっているremote peerの数
		\~
		*/
		uint32_t m_directP2PEnabledPeerCount;


		/**
		\~korean
		TCP, UDP Send Queue에 남아있는 총 크기, Send Queue 총 크기

		\~english
		Total size left in TCP and UDP Send Queue. Total size of the Send Queue


		\~chinese
		TCP, UDP Send Queue中剩余的总的大小, Send Queue的总大小

		\~japanese
		TCP, UDP Send Queueに残っているサイズ合計、Send Queueのサイズ合計
		\~
		*/
		uint32_t m_sendQueueTotalBytes;
		uint32_t m_sendQueueTcpTotalBytes;
		uint32_t m_sendQueueUdpTotalBytes;


		 PROUD_API CNetClientStats();

		 PROUD_API virtual String ToString() const;
	};
}
#ifdef _MSC_VER
#pragma pack(pop)
#endif
