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

#include "BasicTypes.h"
#include "FakeClr.h"
#include "ThreadPool.h"
#include "IRmiHost.h"
#include "P2PGroup.h"

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
서버가, 서버에 접속하는 클라이언트와 통신할 때 사용할 UDP 소켓을 배정하는 정책입니다.
자세한 사항은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_udp_assign" >서버의 UDP 포트 사용 방식</a> 를 참고하십시오.

\~english
This is a policy allocating UDP sockets to be used by the serverwhen communicating with the clients.
Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#server_udp_assign" >Method of using Server UDP Port</a> for more details.

\~chinese
分配服务器与连接服务器的client通信的时候使用的UDP socket的政策。
详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_udp_assign" >服务器UDP端口使用方法</a>%。

\~japanese
サーバーが、サーバーに接続するクライアントと通信する時に使うUDPソケットを割り当てるポリシーです。
詳しくは、\ref server_udp_assign をご参照ください。
\~
*/
	enum ServerUdpAssignMode
	{
		/**
		\~korean
		지정안됨. 사용자는 이 값을 쓰지 마십시오.

		\~english
		Not designated. Users are not to use this value.

		\~chinese
		没被指定。用户不要使用此值。

		\~japanese
		未指定。ユーザーはこの値を使ってはいけません。
		\~
		*/
		ServerUdpAssignMode_None,

		/**
		\~korean
		접속이 들어오는 각 클라이언트에 대해서 서로 다른 UDP 포트를 사용합니다.
		- 본 값은 기본값이며, 권장되는 값입니다.
		- 이 값은 Proud.CStartServerParameter.m_udpPorts 에도 영향을 미칩니다.

		\~english
		For each clients that coonects, it is to use different UDP port for each of them.
		- This value is a default and recommended value.
		- This value affects Proud.CStartServerParameter.m_udpPorts also.

		\~chinese
		对连接的client使用不同的UDP端口。
		- 此值是默认值，而且是被建议的值。
		- 此值也对 Proud.CStartServerParameter.m_udpPorts%产生影响。

		\~japanese
		接続が入ってくる各クライアントに対して相互違うUDPポートを使用します。
		- この値は基本値で、勧奨値です。
		- この値は、Proud.CStartServerParameter.m_udpPortsにも影響を及ぼします。
		\~
		*/
		ServerUdpAssignMode_PerClient,

		/**
		\~korean
		접속이 들어오는 모든 클라이언트는 미리 준비되어 있는 고정된 갯수의 UDP 포트를 재사용합니다.
		이 설정을 사용할 경우 UDP의 특성상 서버와 통신을 하기 어려운 클라이언트들이 생길 수 있습니다.
		- 본 옵션은 ProudNet의 하위 호환성 때문에 준비된 기능이며, 권장하지 않는 기능입니다.
		- 본 옵션 사용시 방화벽 설정이 필요합니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#block_icmp" >ICMP 관련 방화벽 설정</a> 를 참고하십시오.
		이 값은 Proud.CStartServerParameter.m_udpPorts 에도 영향을 미칩니다.

		\~english
		All the clients that connect reuse the UDP ports that are already prepared.
		When using this option, it is possible to cause some clients hard to communicate with the server due to UDP characteristics.
		- This is option is prepared for the lower compatibility of Proudnet and it is not recommended to use.
		- It is necessary to use a firewall setup to use this feature. Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#block_icmp" >Firewall setup for ICMP related
</a> for more details.
		This value affects Proud.CStartServerParameter.m_udpPorts also.

		\~chinese
		连接的client再次使用已经准备好的固定个数的UDP端口。
		使用此设置的时候，因UDP的特性，可能会发生与服务器难以通信的client。
		- 此选项因为是为ProudNet的下位兼容性而准备的功能，是不推荐的功能。
		- 使用此选项时需要防火墙设置。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#block_icmp" >ICMP 相关的防火墙设置</a>%。
		此值也对 Proud.CStartServerParameter.m_udpPorts%产生影响。

		\~japanese
		接続が入ってくる全てのクライアントは前もって準備されている固定数のUDPポートを再使用します。
		この設定を使用する場合、UDPの特性からサーバーとの通信が難しいクライアントが発生することがあります。
		- このオプションはProudNetの下位互換性のため準備された機能で、勧奨しない機能です。
		- このオプションを使う時はファイアウォール設定が必要です。詳しくは、\ref block_icmpをご参照ください。
		この値は、Proud.CStartServerParameter.m_udpPortsにも影響を及ぼします。
		\~
		*/
		ServerUdpAssignMode_Static,
	};

	/**  @} */

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
