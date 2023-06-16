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
#include "Enums.h"
#include "AddrPort.h"
//#include "NetPtr.h"
#include "Ptr.h"
#include "ProcHeap.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	class ErrorInfo;

	/**
	\~korean
	에러 정보를 담는 객체의 스마트 포인터
	- ErrorInfoPtr 은 자주 생기는 객체도 아니므로 fast alloc을 안쓴다.

	\~english
	Smart pointer of the object that contains error info
	- ErrorInfoPtr does not use fast alloc since it does not exist often

	\~chinese
	装载出错信息的智能pointer。
	- 因为ErrorInfoPtr不是经常生成的对象，不用fast alloc。

	\~japanese
	エラー情報を入れるオブジェクトのスマートポインター
	- ErrorInfoPtrはよく発生するオブジェクトでもないので、fast allocを使いません。
	\~
	*/
	typedef RefCount<ErrorInfo> ErrorInfoPtr;

	/**
	\~korean
	에러(또는 성공) 정보를 담은 객체입니다.

	\~english
	Object contains error(or success) info

	\~chinese
	装载出错（或者成功）信息的对象。

	\~japanese
	エラー（または成功）情報を入れたオブジェクトです。
	\~
	*/
	class ErrorInfo
	{
	public:
		/**
		\~korean
		에러의 종류입니다. 에러가 없으면 ErrorType_Ok 입니다.

		\~english
		Error type. If there is no error then ErrorType_Ok.

		\~chinese
		错误的种类。没有错误的话ErrorType_Ok。

		\~japanese
		エラーの種類です。エラーがなければErrorType_Okです。
		\~
		*/
		ErrorType m_errorType;
		/**
		\~korean
		m_errorType 의 세부 값입니다.

		INetClientEvent.OnLeaveServer 와 INetServerEvent.OnClientLeave 에서
		m_detailType 은 연결 해제의 더 자세한 경위를 나타냅니다.
		예를 들어 클라이언트 프로그램이 종료하면서 연결이 해제되면 ErrorType_TCPConnectFailure 를,
		클라이언트의 전원이나 회선이 끊어져서 해제된 경우면 ErrorType_ConnectServerTimeout 가 들어갑니다.

		\~english
		Value for m_errorType

		m_detailType indicates how disconnection occurs with more details at INetClientEvent.OnLeaveServer and INetServerEvent.OnClientLeave
		For an example, it enters ErrorType_TCPConnectFailure when disconnection occurs as client program terminates
		or enters ErrorType_ConnectServerTimeout when disconnection occurs due to power failure and/or physical cable disconnection.

		\~chinese
		m_errorType 的详细值。

		在 INetClientEvent.OnLeaveServer%和 INetServerEvent.OnClientLeave%中m_detailType显示解除连接的更详细内容。
		例如玩家的程序终止时断开连接的话，如果 ErrorType_TCPConnectFailure%解除是因为玩家的电源或者线路断开造成的，会进入ErrorType_ConnectServerTimeout。

		\~japanese
		m_errorType の細部値です。

		INetClientEvent.OnLeaveServerとINetServerEvent.OnClientLeaveでm_detailTypeは、連結解除のもっと詳しい経緯を表します。
		例えば、クライアントプログラムが終了しながら接続が解除されたらErrorType_TCPConnectFailure を、クライアントの電源や回線が切れて解除された場合ならErrorType_ConnectServerTimeoutが入ります。

		\~
		*/
		ErrorType m_detailType;

		/**
		\~korean
		에러가 소켓 에러인 경우 소켓 에러 코드

		\~english
		Error code for socket error

		\~chinese
		错误是socket错误时的socket错误代码。

		\~japanese
		エラーがソケットエラーである場合、ソケットエラーコード
		\~
		*/
		SocketErrorCode m_socketError;

		/**
		\~korean
		에러의 발생 호스트

		\~english
		Host that occur error

		\~chinese
		发生错误主机。

		\~japanese
		エラーの発生ホスト
		\~
		*/
		HostID m_remote;

		/**
		\~korean
		추가 문자열 정보

		\~english
		Information of additional string

		\~chinese
		添加字符串信息。

		\~japanese
		追加文字列情報
		\~
		*/
		String m_comment;

		// 서버 연결 시도중 에러 등에서 유효한 값으로, 상대방 주소
		AddrPort m_remoteAddr;

		/**
		\~korean
		ErrorType_InvalidPacketFormat 의 경우 잘못된 마지막 메세지를 저장한다.

		\~english
		Save wrong last message when case of ErrorType_InvalidPacketFormat

		\~chinese
		ErrorType_InvalidPacketFormat 的时候储存错误的最后信息。

		\~japanese
		ErrorType_InvalidPacketFormat の場合、間違った最後のメッセージを保存します。
		\~
		*/
		ByteArray m_lastReceivedMessage;

#if defined(_WIN32)    

		/**
		\~korean
		DB에서 사용될 HRESULT값입니다.

		\~english
		HRESULT value using for DB

		\~chinese
		在DB要使用的HRESULT值。

		\~japanese
		DBで使われるHRESULT値です。
		\~
		*/
		HRESULT m_hResult;
#endif

		/**
		\~korean
		DB에서 사용될 소스입니다.

		\~english
		Source using for DB

		\~chinese
		在DB要使用的源（source）。

		\~japanese
		DBで使われるソースです。
		\~
		*/
		String m_source;

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif 


		PROUD_API ErrorInfo();

		/**
		\~korean
		ErrorInfo::FromSocketError() 을 통하여 좀 더 간편하게 ErrorInfo 를 만듭니다.
		\param code ErrorType
		\param se 소켓 에러 type
		\return 생성된 ErrorInfoPtr

		\~english TODO:translate needed.
		Through ErrorInfo::FromSocketError(), you can make ErrorInfo more conveniently.
		\param code ErrorType
		\param se Socket error type
		\return generated ErrorInfoPtr

		\~chinese
		通过 ErrorInfo::FromSocketError()%，更方便的制作ErrorInfo。
		\param code ErrorType
		\param se socket错误type。
		\return 生成的ErrorInfoPtr。

		\~japanese
		ErrorInfo::FromSocketError()を通じもっと簡単にErrorInfoを作ります。
		\param code ErrorType
		\param se ソケットエラー type
		\return 生成された ErrorInfoPtr

		\~
		*/
		static ErrorInfoPtr FromSocketError(ErrorType code, SocketErrorCode se);

		/**
		\~korean
		ErrorInfo::From() 을 통하여 좀 더 간편하게 ErrorInfo 를 만듭니다.
		\param errorType ErrorType 입니다.
		\param remote 상대측의 HostID
		\param comment Error에 관련한 첨부 String
		\param lastReceivedMessage 마지막 받은 Message
		\return 생성된 ErrorInfoPtr

		\~english TODO:translate needed.
		Through ErrorInfo::From(), you can make ErrorInfo more more conveniently.
		\param errorType This is ErrorType
		\param remote HostID of the remote correspondent
		\param comment String Attached string related to the error
		\param lastReceivedMessage Message Last received message
		\return Generated ErrorInfoPtr

		\~chinese
		通过 ErrorInfo::From()%，更方便的制作ErrorInfo。
		\param errorType 是ErrorType。
		\param remote 对方的HostID。
		\param comment 关于Error的附加String。
		\param lastReceivedMessage 最后接收的Message。
		\return 生成的ErrorInfoPtr。

		\~japanese
		ErrorInfo::From() を通じもっと簡単に ErrorInfoを作ります。
		\param errorType ErrorType です。
		\param remote 相手側の HostID
		\param comment Errorに関連する添付String
		\param lastReceivedMessage 最後に受けたMessage
		\return 生成されたErrorInfoPtr
		\~
		*/
		PROUD_API static ErrorInfoPtr From(ErrorType errorType, HostID remote = HostID_None, const String &comment = String(), const ByteArray &lastReceivedMessage = ByteArray());

		/**
		\~korean
		ErrorInfo 에 저장된 모든 정보를 String 으로 리턴해줍니다.

		\~english
		Change error contents to string

		\~chinese
		把所有储存到ErrorInfo的信息返回至String。

		\~japanese
		ErrorInfoに保存された全ての情報をStringにリターンします。
		\~
		*/
		PROUD_API String ToString() const;

		/**
		\~korean
		객체 자신을 새 객체에 사본을 뜹니다.

		\~english TODO:translate needed.
		The object itself is copied in a new object.

		\~chinese
		把对象本身复制到新的对象里。

		\~japanese
		オブジェクト自体を新しいオブジェクトにコピーします。
		\~
		*/
		PROUD_API ErrorInfo* Clone();

		/**
		\~korean
		에러 내용을 문자열로 변환한다.

		\~english
		Change error contents to string

		\~chinese
		把错误信息转换成字符串。

		\~japanese
		エラー内容を文字列に変換します。
		\~
		*/
		PROUD_API static const PNTCHAR* TypeToString(ErrorType e);
		PROUD_API static const PNTCHAR* TypeToStringByLangID(ErrorType e, int languageID);

		PROUD_API static const PNTCHAR* TypeToPlainString(ErrorType e);


		/**
		\~korean
		ErrorType 값을 문자열로 변환한다. (한국어)

		\~english
		Change ErrorType value to string (Korean)

		\~chinese
		把ErrorType值转换成字符串。（韩文）

		\~japanese
		ErrorType値を文字列に変換します。（韓国語）
		\~
		*/
		PROUD_API  static const PNTCHAR* TypeToString_Kor(ErrorType e);

		/**
		\~korean
		ErrorType 값을 문자열로 변환한다. (영어)

		\~english
		Change ErrorType value to string (English)

		\~chinese
		把ErrorType值转换成字符串。（英文）

		\~japanese
		ErrorType値を文字列に変換します。（英語）
		\~
		*/
		PROUD_API  static const PNTCHAR* TypeToString_Eng(ErrorType e);

		/**
		\~korean
		ErrorType 값을 문자열로 변환한다. (간체 중국어)

		\~english
		Change ErrorType value to string (Simplified Chinese)

		\~chinese
		把ErrorType值转换成字符串。（简体中文）

		\~japanese
		ErrorType値を文字列に変換します。（簡体中国語）
		\~
		*/
		PROUD_API  static const PNTCHAR* TypeToString_Chn(ErrorType e);

		/** ErrorType 값을 문자열로 변환한다. (일본어) */
		PROUD_API  static const PNTCHAR* TypeToString_Jpn(ErrorType e);
	};

	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
