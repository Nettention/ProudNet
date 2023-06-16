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

#include "EncryptEnum.h"
#include "CompressEnum.h"
#include "Enums.h"
#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class CMessage;

	/** \addtogroup net_group
	*  @{
	*/

	/** 
	\~korean
	전송되는 메시지의 요약 정보입니다. 예를 들어 RMI 로그를 추적할 때, 보낸 메시지가 어떤 형식인지 등을 요약하여 사용자에게 제공합니다. 

	\~english
	This is a quick summary of messages being sent. For instance, when tracking RMI log, this shows a summary of the sent messages to user such as which type they are in.

	\~chinese
	将传送的信息摘要。例如用RMI追踪log的时候，概述发送的信息是什么形式等提供给用户。

	\~japanese
	伝送されるメッセージの要約情報です。例えば、RMIログを追跡する時、送ったメッセージがどのような形式なのかなどを要約してユーザーに提供します。
	\~
	*/
	class MessageSummary
	{
	public:
		/** 
		\~korean
		메시지의 크기입니다. ProudNet의 메시지 계층 이하의 계층은 포함하지 않습니다. 

		\~english
		Size of message. This doesn't include other classes than the ProudNet message class.

		\~chinese
		信息的大小。不包含ProudNet的信息阶层以下的阶层。

		\~japanese
		メッセージのサイズです。ProudNetのメッセージ階層以下の階層は含めません。
		\~
		*/
		int m_payloadLength;

		/** 
		\~korean
		압축된 메시지의 크기입니다.압축이 되지 않았으면,0이 들어갑니다. ProudNet의 메시지 계층 이하의 계층은 포함하지 않습니다. 

		\~english
		Size of compressed message. This doesn't include other classes than the ProudNet message class.

		\~chinese
		压缩的信息大小。如果没有压缩的话是0。不包括ProudNet的信息阶层以下的阶层。

		\~japanese
		圧縮されたメッセージのサイズです。圧縮がされなければ0が入ります。ProudNetのメッセージ階層以下の階層は含めません。
		\~
		*/
		int m_compressedPayloadLength;

		/** 
		\~korean
		이 메시지가 RMI 메시지인 경우 RMI의 ID값입니다. 

		\~english
		RMI ID if a message is RMI.

		\~chinese
		如果此信息是RMI信息的话是RMI的ID值。

		\~japanese
		このメッセージがRMIメッセージである場合、RMIのID値です。
		\~
		*/
		RmiID m_rmiID;
		/** 
		\~korean
		이 메시지가 RMI 메시지인 경우 RMI의 함수명입니다. 

		\~english
		RMI function name if a message is RMI.

		\~chinese
		如果此信息是RMI信息的话是RMI的函数名。

		\~japanese
		このメッセージがRMIメッセージである場合、RMIの関数名です。
		\~
		*/
		const PNTCHAR* m_rmiName;
		/** 
		\~korean
		이 메시지에 동원된 암호화 기법입니다. 

		\~english
		The encrypted method of a message.

		\~chinese
		被这个信息调动的加密技术。

		\~japanese
		このメッセージに動員された暗号化技法です。
		\~
		*/
		EncryptMode m_encryptMode;
		/** 
		\~korean
		이 메시지에 동원된 압축방식입니다.

		\~english
		The compressed method of a message.

		\~chinese
		被这个信息调动的压缩方式。

		\~japanese
		このメッセージに動員された圧縮方式です。
		\~
		*/
		CompressMode m_compressMode;
	};

	/**  @} */

	/** 
	\~korean
	IRmiStub::BeforeRmiInvocation 에서 수신 메시지의 요약 정보입니다. 예를 들어 RMI 로그를 추적할 때, 보낸 메시지가 어떤 형식인지 등을 요약하여 사용자에게 제공합니다. 

	\~english
	Summary of received message at IRmiStub::BeforeRmiInvocation. For exmaple, it provide summary to user such as type of message when you tracking RMI log.

	\~chinese
	在IRmiStub::BeforeRmiInvocation 收信信息的摘要。例如用RMI追踪log的时候，概述发送的信息是以什么形式等提供给用户。

	\~japanese
	IRmiStub::BeforeRmiInvocation で受信メッセージの要約情報です。例えば、RMIログを追跡する時、送ったメッセージがどんな形式なのかなどを要約してユーザーに提供します。
	\~
	*/
	class BeforeRmiSummary
	{
	public:
		/** 
		\~korean
		이 메시지가 RMI 메시지인 경우 RMI의 ID값입니다. 

		\~english
		RMI ID if a message is RMI.

		\~chinese
		如果此信息是RMI信息的话是RMI的ID值。

		\~japanese
		このメッセージがRMIメッセージである場合、RMIのID値です。
		\~
		*/
		RmiID m_rmiID;
		/** 
		\~korean
		이 메시지가 RMI 메시지인 경우 RMI의 함수명입니다. 
		USE_RMI_NAME_STRING을 define해야 이 함수의 파라메터에서 문자열이 등장합니다.

		\~english
		RMI function name if a message is RMI.
		A character string will appear in the parameter of this function only when you've defined USE_RMI_NAME_STRING.

		\~chinese
		如果此信息是RMI信息的话是RMI的函数名。
		USE_RMI_NAME_STRING 被定义后此函数的参数中出现字符串.

		\~japanese
		このメッセージがRMIメッセージである場合、RMIの関数名です。
		USE_RMI_NAME_STRINGをdefineしたら文字がでます。
		\~
		*/
		const PNTCHAR* m_rmiName;
		/** 
		\~korean
		보낸 Host의 HostID 입니다. 

		\~english
		HostID of Host who sent

		\~chinese
		发送的Host的Host ID。

		\~japanese
		送ったHostのHostIDです。
		\~
		*/
		HostID m_hostID;
		/** 
		\~korean
		사용자가 지정한 hostTag의 포인터입니다. 

		\~english
		Pointer of user defined hostTag.

		\~chinese
		用户指定的hostTag的指针。

		\~japanese
		ユーザーが指定したhostTagのポインターです。
		\~
		*/
		void* m_hostTag;
	};

	/**  @} */

	/** \addtogroup net_group
	*  @{
	*/

	/** 
	\~korean
	IRmiStub::AfterRmiInvocation 에서 수신 메시지의 요약 정보입니다. 예를 들어 RMI 로그를 추적할 때, 보낸 메시지가 어떤 형식인지 등을 요약하여 사용자에게 제공합니다. 

	\~english
	Summary of received message at IRmiStub::AfterRmiInvocation. For exmaple, it provide summary to user such as type of message when you tracking RMI log.

	\~chinese
	在IRmiStub::AfterRmiInvocation 收信信息的摘要。例如用RMI追踪log的时候，概述发送的信息是以什么形式等提供给用户。

	\~japanese
	IRmiStub::AfterRmiInvocation で受信メッセージの要約情報です。例えば、RMIログを追跡する時、送ったメッセージがどんな形式なのかなどを要約してユーザーに提供します。
	\~
	*/
	class AfterRmiSummary
	{
	public:
		/** 
		\~korean
		이 메시지가 RMI 메시지인 경우 RMI의 ID값입니다. 

		\~english
		RMI ID if a message is RMI.

		\~chinese
		如果此信息是RMI的信息的话是RMI的ID值。

		\~japanese
		このメッセージがRMIメッセージである場合、RMIのID値です。
		\~
		*/
		RmiID m_rmiID;
		/** 
		\~korean
		이 메시지가 RMI 메시지인 경우 RMI의 함수명입니다.
		USE_RMI_NAME_STRING을 define해야 이 함수의 파라메터에서 문자열이 등장합니다.

		\~english
		RMI function name if a message is RMI.
		A character string will appear in the parameter of this function only when you've defined USE_RMI_NAME_STRING.

		\~chinese
		如果此信息是RMI的信息的话是RMI的函数名。
		USE_RMI_NAME_STRING 被定义后此函数的参数中出现字符串.

		\~japanese
		このメッセージがRMIメッセージである場合、RMIの関数名です。
		USE_RMI_NAME_STRINGをdefineしたら文字がでます。
		\~
		*/
		const PNTCHAR* m_rmiName;
		/** 
		\~korean
		보낸 Host의 HostID 입니다. 

		\~english
		HostID of Host who sent

		\~chinese
		发送的Host的Host ID。

		\~japanese
		送ったHostのHostIDです。
		\~
		*/
		HostID m_hostID;
		/** 
		\~korean
		사용자가 지정한 hostTag의 포인터입니다. 

		\~english
		Pointer of user defined hostTag.

		\~chinese
		用户指定的hostTag的指针。

		\~japanese
		ユーザーが指定したhostTagのポインターです。
		\~
		*/
		void* m_hostTag;
		/** 
		\~korean
		수신 RMI함수가 처리되는데 걸리는 시간 

		\~english
		Time to process received RMI function.

		\~chinese
		收信RMI函数被处理时所需要的时间。

		\~japanese
		受信RMI関数が処理されるにかかる時間
		\~
		*/
		uint32_t m_elapsedTime;
	};

	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif