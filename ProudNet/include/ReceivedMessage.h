﻿/*
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

#include "Message.h"
#include "AddrPort.h"
#include "ProcHeap.h"
#include "CompressEnum.h"
#include "ClassBehavior.h"

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
	/* 
	\~korean
	수신된 메시지. 내부 용도다. 

	\~english
	Received messge. Internal purpose.

	\~chinese
	收到的信息是内部用。

	\~japanese
	受信したメッセージ。内部用途です。
	\~
	*/
	class CReceivedMessage
	{
		// 복사 비용이 크다. CFinalUserWorkItem을 쓰던지 해서 복사 안되게 한다.
		NO_COPYABLE(CReceivedMessage)

	public:
		// 과거 버전에서 fast heap을 외부 참조하기 때문에 객체 파괴시 main 객체를 잠그고 그 안의 fast heap access를 해야 했지만
		// 이제는 그런 기능이 사라지고 전역 obj-pool 방식을 쓰기 때문에 unsafe가 아님.
		// 주의: Final user work item인 경우 이것은 RMI ID부터 시작하는 내용이다. 즉 MessageType_XXX가 제거된 상태다.
		CMessage m_unsafeMessage;

		// 송신자
		HostID m_remoteHostID;

		// UDP 수신인 경우에 유효한 멤버 변수
		AddrPort m_remoteAddr_onlyUdp;

		// 릴레이로 왔니?
		bool m_relayed;

		// ACR
		int m_messageID; // m_hasMessgeID=true일때만 유효 
		bool m_hasMessageID;

		//double m_actionTime;

		EncryptMode m_encryptMode;
		CompressMode m_compressMode;

		// 측정 결과 호출 횟수가 매우 많은 메서드인지라 inline으로 둔다.
		inline CReceivedMessage()
		{
			Clear();
		}

		inline CMessage &GetWriteOnlyMessage()
		{
			return m_unsafeMessage;
		}
		inline CMessage &GetReadOnlyMessage()
		{
			return m_unsafeMessage;
		}
		inline AddrPort GetRemoteAddr()
		{
			return m_remoteAddr_onlyUdp;
		}
		inline HostID GetRemoteHostID()
		{
			return m_remoteHostID;
		}
		inline bool IsRelayed()
		{
			return m_relayed;
		}

		// PIDL compiler output에서 호출하므로, getter method를 의도로 둠.
		inline EncryptMode GetEncryptMode()
		{
			return m_encryptMode;
		}

		// PIDL compiler output에서 호출하므로, getter method를 의도로 둠.
		inline CompressMode GetCompressMode()
		{
			return m_compressMode;
		}

		// called by BiasManagedPointer.
		inline void Clear()
		{
			m_relayed = false;
			m_remoteHostID = HostID_None;
			m_remoteAddr_onlyUdp = AddrPort::Unassigned;

			m_messageID = 0;
			m_hasMessageID = false;

			m_encryptMode = EM_None;
			m_compressMode = CM_None;

			m_unsafeMessage.Clear();
		}

	};


#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
