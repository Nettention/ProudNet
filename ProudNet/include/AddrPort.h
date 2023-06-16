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

#if defined(__unix__)
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <winsock2.h>
#include <ws2ipdef.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <errno.h>
#endif

#if defined(__ORBIS__)
#include "OrbisIPv6.h"
#endif

#ifdef PF_MAX
#undef PF_MAX
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/
	class CFastSocket;

	struct ExtendSockAddr;
	struct NamedAddrPort;

	/**

	\~korean
	소켓 에러 코드

	\~english
	Socket Error Code

	\~chinese
	Socket 错误代码

	\~japanese
	ソケットエラーコード
	\~
	*/
	enum SocketErrorCode
	{
		SocketErrorCode_Ok = 0,
		// EINTR와 WSAEINTR은 서로 다른 값이다. 따라서 Win32에서는 EINTR을 쓰면 안된다.
		// 따라서, 어쩔 수 없이 아래와 같이 서로 다르게 구별될 수밖에 없다.
#if !defined(_WIN32)
		SocketErrorCode_Error = -1,
		SocketErrorCode_Timeout = ETIMEDOUT,
		SocketErrorCode_ConnectionRefused = ECONNREFUSED,
		SocketErrorCode_ConnectResetByRemote = ECONNRESET,
		SocketErrorCode_AddressNotAvailable = EADDRNOTAVAIL,
		SocketErrorCode_NotSocket = ENOTSOCK,
		SocketErrorCode_WouldBlock = EWOULDBLOCK,
		SocketErrorCode_AccessError = EACCES,
		SocketErrorCode_InvalidArgument = EINVAL,
		SocketErrorCode_Intr = EINTR, // EINTR in linux
		SocketErrorCode_InProgress = EINPROGRESS, // operation이 이미 진행중이다.
		SocketErrorCode_Again = EAGAIN, // win32에서는 would-block이 대체. unix에서는 wouldblock과 again이 다른 값이지만 같은 의미라고 한다. POSIX.1-2001에 의함.
		SocketErrorCode_AlreadyIsConnected = EISCONN,
		SocketErrorCode_AlreadyAttempting = EALREADY, // The socket is nonblocking and a previous connection attempt has not yet been completed.
		SocketErrorCode_NetUnreachable = ENETUNREACH,
#else
		SocketErrorCode_Error = SOCKET_ERROR,
		SocketErrorCode_Timeout = WSAETIMEDOUT,
		SocketErrorCode_ConnectionRefused = WSAECONNREFUSED,
		SocketErrorCode_ConnectResetByRemote = WSAECONNRESET,
		SocketErrorCode_AddressNotAvailable = WSAEADDRNOTAVAIL,
		SocketErrorCode_NotSocket = WSAENOTSOCK,
		SocketErrorCode_ShutdownByRemote = WSAEDISCON,	// FD_CLOSE or FD_SEND에서의 이벤트
		SocketErrorCode_WouldBlock = WSAEWOULDBLOCK,
		SocketErrorCode_IoPending = WSA_IO_PENDING,
		SocketErrorCode_AccessError = WSAEACCES,
		SocketErrorCode_OperationAborted = ERROR_OPERATION_ABORTED,
		SocketErrorCode_InvalidArgument = WSAEINVAL,
		SocketErrorCode_Intr = WSAEINTR, // EINTR in linux
		SocketErrorCode_InProgress = WSAEINPROGRESS, // operation이 이미 진행중이다.		
		SocketErrorCode_AlreadyIsConnected = WSAEISCONN,
		SocketErrorCode_AlreadyAttempting = WSAEALREADY,
		SocketErrorCode_Cancelled = WSAECANCELLED,
		SocketErrorCode_NetUnreachable = WSAENETUNREACH,
#endif
	};

	enum ShutdownFlag
	{
#if !defined(_WIN32)
		ShutdownFlag_Send = SHUT_WR,
		ShutdownFlag_Receive = SHUT_RD,
		ShutdownFlag_Both = SHUT_RDWR,
#else
		ShutdownFlag_Send = SD_SEND,
		ShutdownFlag_Receive = SD_RECEIVE,
		ShutdownFlag_Both = SD_BOTH,
#endif
	};

	static const int PN_IPV6_ADDR_LENGTH = 16;

	// 무효한 소켓 핸들값.
#ifdef _WIN32
	// 윈도에서는 invalid fd = INVALID_SOCKET = 0이다.
	const int InvalidSocket = 0; // INVALID_SOCKET
#else 
	// unix에서는 음수가 무효 소켓 fd 값이다.
	const int InvalidSocket = -1;
#endif

	/**
	\~korean
	IP, Port 식별자

	\~english
	IP, Port Identifier

	\~chinese
	IP, Port识别者

	\~japanese
	IP、ポート識別子
	\~
	*/
	struct AddrPort
	{

		/* PN_ALIGN(1) 즉,  __declspec(align) 을 1바이트로 사용 했을 때 VC++에서는 "4 미만은 무시한다"는 warning이 뜬다.
		따라서 align 4를 하되, 선언되는 변수들이 4배수 크기로 지정하도록 한다.
		어떤 컴파일러는 기본 align 8이므로 align 4를 해주어야 한다.
		*/
		union PN_ALIGN(4) ExtendAddr
		{
			// 절대 이 공용체 내의 변수를 함부로 추가 하지 마십시오.
			struct PN_ALIGN(4)
			{
				uint8_t  __dummy[PN_IPV6_ADDR_LENGTH - sizeof(uint32_t)]; // 12바이트 => 4배수 만족
				uint32_t v4; // 4바이트 => 4배수 만족
			};

			uint16_t v6Short[PN_IPV6_ADDR_LENGTH / 2]; // 16바이트 => 4배수 만족
			uint8_t  v6Byte[PN_IPV6_ADDR_LENGTH]; // 16바이트 => 4배수 만족
		} m_addr;

		/**
		\~korean
		포트 번호
		- native endian이고, network endian가 아니다. 즉 socket 함수 htons나 ntohs를 통해 변환할 필요가 없이 그대로 사용해도 된다.

		\~english
		Port Number
		- It's native endian, not network endian. So it can be used as-is without converting it through htons or ntohs.

		\~chinese
		端口编号
		- 是native endian，而不是network endian。也就是说，没有必要通过socket函数htons或ntohs转变，也可以直接使用。

		\~japanese
		ポート番号
		⁻ native endianで、network endianではありません。即ち、socket関数のhtonsやntohsを通じ変換する必要がなくそのまま使っても良いです。		

		\~
		*/
		uint16_t m_port;

	private:
		bool IsFFFFAddress() const;
		bool Is0000Address() const;
		bool IsLocalhostAddress() const;
		
	public:

		// #TODO 아래 주석들은 code review 후 doxygen 형태로 정리 해야 된다.

		/**
		IPv4 주소가 들어있으면, true를 리턴합니다.
		RFC 4291 에서 정의 하고 있는, 표준 IPv4 표현 방식을 일컫습니다.
		예: 11.22.33.44는 ::ffff:11.22.33.44 형식으로 저장되며, 이때 이 함수는 true를 리턴합니다.
		*/
		PROUD_API bool IsIPv4MappedIPv6Addr() const;

		/*
		RFC 4291 에서 정의 하고 있는 표준 IPv4 표현 방식으로 세팅 합니다.
		(::1.2.3.4 와 ::FFFF:1.2.3.4 두개가 있지만, 프라우드넷에서는 뒤의 방식으로 사용 합니다.)
		*/
		PROUD_API  void SetIPv4MappedIPv6Address(uint32_t ipv4Address);

		/*
		IPv6 주소로 세팅 합니다.
		addr 인자가 v4 mapped v6 주소로 저장 되어 있어도 됩니다.
		*/
		PROUD_API  void SetIPv6Address(const in6_addr& addr);

		PROUD_API  void SetIPv6Address(const uint8_t* src, const size_t length);

		// 내부 전용 함수. 도움말 불필요.
		PROUD_API void Synthesize(const uint8_t* pref, const size_t prefLength, const uint32_t v4BinaryAddress);

		/* IPv4 주소를 32비트 바이너리로서 얻는다.
		AddrPort 는 RFC 6052, RFC 4291 에서 정의 하고 있는 v4 표현 방식을 따르고 있어야 합니다.

		예: 11.22.33.44 => ::ffff:11.22.33.44

		만약 NAT64 주소이거나, IPv6 호스트를 가리키는 주소인 경우, 이 함수는 실패합니다.

		\param outIPv4Address 얻어낸 IPv4 주소값.
		\return 성공적으로 얻어내면 true.
		*/
		PROUD_API  bool GetIPv4Address(uint32_t* outIPv4Address) const;

		inline bool IsAddressEqualTo(const AddrPort &a) const
		{
			return memcmp(m_addr.v6Byte, a.m_addr.v6Byte, PN_IPV6_ADDR_LENGTH) == 0 ? true : false;
		}

		/**
		\~korean
		생성자

		\~english
		Generator

		\~chinese
		生成者

		\~japanese
		生成子
		\~
		*/
		 PROUD_API AddrPort();

		 PROUD_API  bool ToNativeV4(ExtendSockAddr& output, ErrorInfo& outErrorInfo) const;
		 PROUD_API  void ToNativeV6(ExtendSockAddr& out) const;


		/**
		\~korean
		Socket API 파라메터 sockaddr_in 구조체에게 값을 준다.

		\~english
		This method passes a value to Socket API Parameter, sockaddr_in structure.

		\~chinese
		给予Socket API参数sockaddr_in构造体赋值。

		\~japanese
		Socket APIパラメーターのsockaddr_in構造体に値を与えます。

		\~
		*/
		 PROUD_API  void FromNativeV4(const sockaddr_in& in);

		 PROUD_API  void FromNativeV6(const sockaddr_in6& in);

		 PROUD_API  void FromNative(const ExtendSockAddr& in);

		/**
		\~korean
		xxx.xxx.xxx.xxx:xxxx 문자열 추출

		\~english
		Extract xxx.xxx.xxx.xxx:XXXX string

		\~chinese
		抽出字符串 xxx.xxx.xxx.xxx:XXXX

		\~japanese
		xxx.xxx.xxx.xxx:xxxx 文字列エクスポート
		\~
		*/
		 PROUD_API virtual String ToString() const;

		/**
		\~korean
		xxx.xxx.xxx.xxx 문자열 추출

		\~english
		Extract xxx.xxx.xxx.xxx string

		\~chinese
		抽出字符串 xxx.xxx.xxx.xxx

		\~japanese
		xxx.xxx.xxx.xxx 文字列エクスポート
		\~
		*/
		 PROUD_API virtual String IPToString() const;

		/**
		\~korean
		IP address 문자열과 port를 입력받아, AddrPort 객체를 리턴합니다.
		host name은 처리할 수 없습니다. 대신 FromHostNamePort()를 사용하세요.
		\param ipAddress IP 주소 값입니다. 예를 들어 "11.22.33.44"입니다.
		\param port 포트 값입니다.

		\~english
		Input an IP address string and a port to get the return value from AddrPort object.
		Host name can’t be processed. Use FromHostNamePort() instead.
		\param ipAddress IP is the address value. Ex. “11.22.33.44”
		\param port is the port value.

		\~chinese
		通过输入IP地址字符串和端口返回AddrPort对象。
		host name无法处理，请使用FromHostNamePort()。
		\param ipAddress IP地址值。例如"11.22.33.44"。
		\param port 端口值。

		\~japanese
		\param ipAddress IPアドレス値です。例えば、"11.22.33.44"です。
		\param port ポート値です。

		\~
		*/
		 PROUD_API  static AddrPort FromIPPortV4(const String& ipAddress, uint16_t port);

		 PROUD_API  static AddrPort FromIPPortV6(const String& ipAddress, uint16_t port);

		 PROUD_API  static AddrPort FromIPPort(const int32_t& af, const String& ipAddress, uint16_t port);

		/** Set to Any:port. */
		 PROUD_API  static AddrPort FromAnyIPPort(const int32_t& af, uint16_t port);

		 PROUD_API static bool FromHostNamePort(AddrPort* outAddrPort, SocketErrorCode& errorCode, const String& hostName, uint16_t port);

		/**
		\~korean
		NamedAddrPort 객체로부터 값을 가져온다.
		- NamedAddrPort의 host name이 "my.somename.net" 형태인 경우 이를 IP address로 변환해서 가져온다.

		\~english
		This method gets a value from NamedAddrPort object.
		- If host name of NamedAddrPort is in "my.somename.net" format, then this method converts the host name to IP address and returns it.

		\~chinese
		从NamedAddrPort对象取值。
		- 当NamedAddrPort的host name是"my.somename.net"形态的时，把它转换成IP address后取回。

		\~japanese
		NamedAddrPort オブジェクトから値を取り込みます。
		- NamedAddrPortのhost nameが"my.somename.net"形態である場合、これをIP addressに変換して取り込みます。
		\~
		*/
		 PROUD_API  static AddrPort From(const NamedAddrPort& src);

		/**
		\~korean
		미지정 IP 객체. 디폴트 값이다.

		\~english
		Undesignated IP object. It is default value.

		\~chinese
		未指定的IP对象，是默认值。

		\~japanese
		未指定IPオブジェクト。デフォルト値です。
		\~
		*/
		 PROUD_API static AddrPort Unassigned;

		/**
		\~korean
		브로드캐스트 주소도 아니고, null 주소도 아닌, 1개 호스트의 1개 포트를 가리키는 정상적인 주소인가?

		\~english
		Is it correct address that point 1 port of 1 host instead of broadcast address, null address?

		\~chinese
		不是broadcast地址，也不是null地址，是否是指一个主机上一个端口的正常地址？

		\~japanese
		ブロードキャストアドレスでもなく、nullアドレスでもない、1ホストの1ポートを指す正常なアドレスですか？
		\~
		*/
		 PROUD_API bool IsUnicastEndpoint() const;

		 PROUD_API bool IsAnyOrUnicastEndpoint() const;

		static inline bool IsEqualAddress(const AddrPort& a, const AddrPort& b)
		{
			return a.IsAddressEqualTo(b);
		}

		inline uint32_t Hash() const
		{
			assert(PN_IPV6_ADDR_LENGTH == 16); // 본 함수는 IPv6 or IPv4 mapped IPv6 주소만 처리할 수 있다.

			uint32_t* p = (uint32_t*)&m_addr;
			uint32_t v = 0;
			for (int i = 0; i < 4; i++)
			{
				v ^= *p;
				p++;
			}
			v ^= m_port;

			return v;
		}

		inline bool operator!=(const AddrPort &rhs) const
		{
			assert(PN_IPV6_ADDR_LENGTH == 16); // 본 함수는 IPv6 or IPv4 mapped IPv6 주소만 처리할 수 있다.

			if (CompareInt32Array<4>(m_addr.v6Byte, rhs.m_addr.v6Byte) != 0 ||
				m_port != rhs.m_port)
			{
				return true;
			}

			return false;
		}
		inline bool operator==(const AddrPort &b) const
		{
			assert(PN_IPV6_ADDR_LENGTH == 16); // 본 함수는 IPv6 or IPv4 mapped IPv6 주소만 처리할 수 있다.

			if (CompareInt32Array<4>(m_addr.v6Byte, b.m_addr.v6Byte) == 0 &&
				m_port == b.m_port)
			{
				return true;
			}

			return false;
		}

		inline bool operator<(const AddrPort& b) const
		{
			int compareResult = CompareInt32Array<4>(m_addr.v6Byte, b.m_addr.v6Byte);
			if (compareResult < 0)
				return true;
			if (compareResult > 0)
				return false;
			return m_port - b.m_port < 0;
		}
	};

	/**
	\~korean
	AddrPort와 비슷하지만 m_addr에 문자열이 들어간다.
	- 문자열에는 111.222.111.222 형태 또는 game.aaa.com 형식의 이름이 들어갈 수 있다.
	- AddrPort은 111.222.111.222 형태만이 저장될 수 있는 한계 때문에 이 구조체가 필요한거다.

	\~english
	It's similar to AddrPort but string can be inserted in m_addr.
	- At string, either 111.222.111.222 format or game.aaa.com format can be added.
	- This structure is needed since AddrPort can only store 111.222.111.222 format.

	\~chinese
	与AddrPortt相似，但是m_addr里有字符串。
	- 字符串里会有111.222.111.222格式，或者game.aaa.com格式的名称。
	- 因为AddrPort只能存储111.222.111.222格式，所以需要此构造。

	\~japanese
	AddrPortと類似していますが、m_addrに文字列が入ります。
	- 文字列には111.222.111.222形態またはgame.aaa.com形式の名前が入ることができます。
	- AddrPortは111.222.111.222形態のみ保存できるという限界のため、この構造体が必要です。
	\~
	*/
	struct NamedAddrPort
	{
		/**
		\~korean
		호스트 이름
		- 문자열에는 111.222.111.222 형태 또는 game.aaa.com 형식의 이름이 들어갈 수 있다.

		\~english
		Host name
		- At string, either 111.222.111.222 format or game.aaa.com format can be added.

		\~chinese
		主机名
		- 字符串里可以添加111.222.111.222或者game.aaa.com格式的名称。

		\~japanese
		ホスト名
		- 文字列には111.222.111.222形態またはgame.aaa.com形式の名前が入ることができます。
		\~
		*/
		String m_addr;

		/**
		\~korean
		포트 값

		\~english
		Port Value

		\~chinese
		端口值

		\~japanese
		ポート値
		\~
		*/
		uint16_t m_port;

		/**
		\~korean
		빈 주소

		\~english
		Empty Address

		\~chinese
		空地址

		\~japanese
		空アドレス
		\~
		*/
		PROUD_API static NamedAddrPort Unassigned;

		/**
		\~korean
		특정 hostname, 포트 번호로부터 이 객체를 생성한다.

		\~english
		Generate this object from a specific hostname and port number.

		\~chinese
		特定的hostname，从端口号码生成此对象。

		\~japanese
		特定hostname、ポート番号からこのオブジェクトを生成します。
		\~
		*/
		 PROUD_API static NamedAddrPort  FromAddrPort(String addr, uint16_t port);

		/**
		\~korean
		AddrPort 객체로부터 이 객체를 생성한다

		\~english
		Generate this object from AddrPort object

		\~chinese
		从AddrPort对象生成这个对象。

		\~japanese
		AddrPortオブジェクトからこのオブジェクトを生成します。
		\~
		*/
		 PROUD_API static NamedAddrPort  From(const AddrPort &src);

		/**
		\~korean
		hostname이 들어있는 문자열인 경우 들어있는 문자열로 새 호스트 이름을 지정한다.

		\~english
		Appoint a new host name with the string inside of hostname.

		\~chinese
		有hostname字符串的情况，使用内在字符串指定新的主机名。

		\~japanese
		hostnameが入る文字列の場合、入っている文字列に新しいホスト名を指定します。
		\~
		*/
		PROUD_API void OverwriteHostNameIfExists(const String &hostName);

		/**
		\~korean
		비교 연산자

		\~english
		Comparison Operator

		\~chinese
		比较运算符

		\~japanese
		比較演算子
		\~
		*/
		inline bool operator==( const NamedAddrPort &rhs ) const
		{
			return m_addr == rhs.m_addr && m_port == rhs.m_port;
		}

		/**
		\~korean
		비교 연산자

		\~english
		Comparison Operator

		\~chinese
		比较运算符

		\~japanese
		比較演算子
		\~
		*/
		inline bool operator!=( const NamedAddrPort &rhs ) const
		{
			return !( m_addr == rhs.m_addr && m_port == rhs.m_port );
		}

		/**
		\~korean
		내용물을 문자열로 만든다.

		\~english
		This method makes the contents into string.

		\~chinese
		把内容改成字符串。

		\~japanese
		内容物を文字列にします。
		\~
		*/
		PROUD_API  String  ToString() const;

		/**
		\~korean
		브로드캐스트 주소도 아니고, null 주소도 아닌, 1개 호스트의 1개 포트를 가리키는 정상적인 주소인가?

		\~english
		Is it correct address that point 1 port of 1 host instead of broadcast address, null address?

		\~chinese
		不是broadcast地址，也不是null地址，是否是指定一个主机上一个端口的正常地址？

		\~japanese
		ブロードキャストアドレスでもなく、nullアドレスでもない、1ホストの1ポートを指す正常なアドレスですか？
		\~
		*/
		 PROUD_API bool IsUnicastEndpoint();
	};

	inline void AppendTextOut(String& a, const AddrPort &b)
	{
		a += b.ToString();
	}

	inline void AppendTextOut(String& a, const NamedAddrPort &b)
	{
		a += b.ToString();
	}

	/**
	\~korean
	네트워크 프로그래밍 관련 유틸리티 클래스

	\~english
	Network programming related utility class

	\~chinese
	网络编程相关的应用程序类

	\~japanese
	ネットワークプログラミング関連ユーティリティークラス
	\~
	*/
	class CNetUtil
	{
	public:
//#if !defined __FreeBSD__
		/**
		\~korean
		호스트가 갖고 있는 로컬 IP 주소를 모두 얻어냅니다.

		\~english
		Gets every local IP addresses that host has.

		\~chinese
		取得主机拥有的所有本地IP地址。

		\~japanese
		ホストが持っているローカルIPアドレスを全て取り持ちます。
		\~
		*/
		PROUD_API static void GetLocalIPAddresses(CFastArray<String> &output);

		PROUD_API static int GetIPVersionFromString(const String& rhs);

		/** Returns true if address is filled with 255 or 0xffff. */
		  PROUD_API static bool IsAddressAny(const String& address);

		/** Returns true if address is filled with 0. */
		 PROUD_API static bool IsAddressUnspecified(const String& address);

		/** Returns true if address is not Any, Unspecified nor Loopback. */
		 PROUD_API static bool IsAddressPhysical(const String& address);

		/** Returns true if addres is Loopback. For example, "127.0.0.1", "::1" or "localhost". */
		 PROUD_API  static bool IsAddressLoopback(const String& address);

#if defined(_WIN32)

		/**
		\~korean
		일부 온보드 저가형 네트워크 아답타의 경우 하드웨어에서 지원하는 TCP offload 기능에 문제가 있을 수 있습니다.
		이 메서드는 윈도우 레지스트리에서 해당 기능을 제어하며 재부팅을 필요로 합니다.
		\return ErrorType_Ok 이면 성공적으로 적용했으며, 재부팅을 요합니다. Windows Vista 이상의 OS 에서 관리자 권한을 획득하지 않으면 ErrorType_PermissionDenied 에러가 발생할 수 있습니다. 기타 오류의 경우 ErrorType_Unexpected 가 리턴됩니다.

		\~english
		TCP offload function will occur problem with some cheap network adapter that intergrated on the mother board.
		This method is controlled by Windows registry and it require restart a machine.
		It successfully applied, if \return ErroType_Ok is true. and it require restart. ErrorType_PermissionDenied will occur, if you do not get aminitrator permission on Windows Vista or above version. ErrorType_Unexpected will return with other errors

		\~chinese
		一些廉价的网络适配器集成，可能会在硬件支持的TCP offload技能产生问题。这个方法会在Windows注册表上控制有关技能，并需要重新启动。、
		\return ErrorType_Ok的话表示应用成功，需重新启动。在Windows Vista以上的OS里不获得管理权限，会发生ErrorType_PermissionDenied错误。其他错误的时候会返回ErrorType_Unexpected。

		\~japanese
		一部オンボード低下型ネットワークアダプターの場合、ハードウェアでサポートするTCP offload機能に問題がある場合があります。
		このメソッドはウィンドウレジストリーで該当機能を制御して再起動を必要とします。
		\return ErrorType_Okであれば適用済みですので再起動してください。Windows Vista以上のOSで管理者の権限を獲得しなければErrorType_PermissionDeniedエラーが発生する可能性があります。その他エラーの場合、ErrorType_Unexpectedが取り戻されます。
		\~
		*/
		PROUD_API static ErrorType EnableTcpOffload(bool enable);
#endif
	};

	/**  @}  */
}

template<>
class CPNElementTraits < Proud::AddrPort >
{
public:
	typedef const Proud::AddrPort& INARGTYPE;
	typedef Proud::AddrPort& OUTARGTYPE;

	inline static uint32_t Hash(INARGTYPE a)
	{
		uint32_t sumVal = 0;
		for (int32_t i = 0; i < Proud::PN_IPV6_ADDR_LENGTH; i++)
		{
			sumVal += a.m_addr.v6Byte[i];
		}

		return sumVal ^ a.m_port;
	}

	inline static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
	{
		return element1 == element2;
	}

	inline static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
	{
 		if (element1 < element2)
 			return -1;
 		if (element1 == element2)
 			return 0;
		return 1;
	}
};

// template<>
// class CPNElementTraits<Proud::AddrPort>
// {
// public:
// 	typedef const Proud::AddrPort& INARGTYPE;
// 	typedef Proud::AddrPort& OUTARGTYPE;
//
// 	inline static ULONG Hash( INARGTYPE element )
// 	{
// 		assert(sizeof(element.m_binaryAddress) == 4);
// 		return element.m_binaryAddress ^ element.m_port;
//
// 		return ret;
// 	}
//
// 	inline static bool CompareElements( INARGTYPE element1, INARGTYPE element2 ) throw()
// 	{
// 		return ( element1 == element2 ) ? true:false;
// 	}
//
// 	inline static int CompareElementsOrdered( INARGTYPE element1, INARGTYPE element2 ) throw()
// 	{
// 		if(element1.m_binaryAddress < element2.m_binaryAddress)
// 			return -1;
// 		else if(element1.m_binaryAddress < element2.m_binaryAddress)
// 			return 1;
// 		if(element1.m_port < element2.m_port)
// 			return -1;
// 		else if(element1.m_port > element2.m_port)
// 			return 1;
// 		return 0;
// 	}
// };

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

