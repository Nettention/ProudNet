#pragma once

#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \~korean 상황 통계 정보입니다. 
	\~english Status information.
	\~chinese 状态统计信息。
	\~japanese 状況の統計情報です。
	\~
	*/
	class CNetCoreStats
	{
	public:
		uint64_t m_totalTcpReceiveCount;

		/**
		\~korean
		총 수신된 TCP 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all TCP messages received. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的TCP信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたTCPメッセージの総量(byte)。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalTcpReceiveBytes;

		uint64_t m_totalTcpSendCount;

		/**
		\~korean
		총 송신된 TCP 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all TCP messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总传送的TCP信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたTCPメッセージの総量(byte)。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalTcpSendBytes;
		
		/**
		\~korean
		총 수신된 UDP 메시지의 갯수. RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Number of all UDP messages received. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的UDP信息个数。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたUDPメッセージの総数。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalUdpReceiveCount;
		/**
		\~korean
		총 수신된 UDP 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all UDP messages received. This includes RMI and the headers of all packets in ProudNet

		\~chinese
		总收信的UDP信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたUDPメッセージの総量（byte）。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalUdpReceiveBytes;

		/**
		\~korean
		총 송신된 UDP 메시지의 갯수. RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Number of all UDP messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总传送的UDP信息个数。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたUDPメッセージの総数。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalUdpSendCount;

		/**
		\~korean
		총 송신된 UDP 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all UDP messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总传送的UDP信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたUDPメッセージの総量（byte）。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalUdpSendBytes;

		/**
		\~korean
		총 수신된 WebSocket 메시지의 갯수. RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Number of all WebSocket messages received. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的WebSocket信息个数。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたWebSocketメッセージの総数。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalWebSocketReceiveCount;

		/**
		\~korean
		총 수신된 WebSocket 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all WebSocket messages received. This includes RMI and the headers of all packets in ProudNet

		\~chinese
		总收信的WebSocket信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたWebSocketメッセージの総量（byte）。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalWebSocketReceiveBytes;

		/**
		\~korean
		총 송신된 WebSocket 메시지의 갯수. RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Number of all WebSocket messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总传送的WebSocket信息个数。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたWebSocketメッセージの総数。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalWebSocketSendCount;

		/**
		\~korean
		총 송신된 WebSocket 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all WebSocket messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总传送的WebSocket信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたWebSocketメッセージの総量（byte）。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		uint64_t m_totalWebSocketSendBytes;

		inline uint64_t GetTotalReceiveBytes()
		{
			return m_totalTcpReceiveBytes + m_totalUdpReceiveBytes + m_totalWebSocketReceiveBytes;
		}

		/**
		\~korean
		총 수신된 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的信息量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたメッセージの総量（byte）。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
#if defined(_MSC_VER)
		__declspec(property(get = GetTotalReceiveBytes)) uint64_t m_totalReceiveBytes;
#endif


		inline uint64_t GetTotalSendCount()
		{
			return m_totalTcpSendCount + m_totalUdpSendCount + m_totalWebSocketSendCount;
		}
		inline uint64_t GetTotalReceiveCount()
		{
			return m_totalTcpReceiveCount + m_totalUdpReceiveCount + m_totalWebSocketReceiveCount;
		}
		inline uint64_t GetTotalSendBytes()
		{
			return m_totalTcpSendBytes + m_totalUdpSendBytes + m_totalWebSocketSendBytes;
		}
		/**
		\~korean
		총 송신된 메시지의 갯수. RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Number of all messages sent. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的信息个数。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたメッセージの総数。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
#if defined (_MSC_VER)
		__declspec(property(get = GetTotalSendCount)) uint64_t m_totalSendCount;

		/**
		\~korean
		총 수신된 메시지의 갯수. RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Number of all received messages. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的信息个数。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		受信されたメッセージの総数。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		__declspec(property(get = GetTotalReceiveCount)) uint64_t m_totalReceiveCount;

		/**
		\~korean
		총 송신된 메시지의 양(byte). RMI 뿐만 아니라 ProudNet 내부의 모든 패킷의 헤더까지 포함한다.

		\~english
		Amount(byte) of all sent messages. This includes RMI and the headers of all packets in ProudNet.

		\~chinese
		总收信的信息的量（byte）。除了RMI，也包括ProudNet内部的所有数据包header。

		\~japanese
		送信されたメッセージの総量（byte）。RMIだけではなく、ProudNet内部の全てのパケットのヘッダーまで含めます。
		\~
		*/
		__declspec(property(get = GetTotalSendBytes)) uint64_t m_totalSendBytes;
		
#endif

		PROUD_API CNetCoreStats();

		/**
		\~korean
		이 객체의 내용물을 표현하는 문자열을 리턴합니다.

		\~english
		Returns a string that represents this object.

		\~chinese
		返回表示该对象的字符串。

		\~japanese
		このオブジェクトの内容を表すストリングをリターンします。
		\~
		*/
		virtual String ToString() const = 0;
	};
}
#ifdef _MSC_VER
#pragma pack(pop)
#endif
