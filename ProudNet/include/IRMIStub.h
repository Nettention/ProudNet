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

#include "Enums.h"
#include "FakeClr.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	class CNetCoreImpl;
	class CReceivedMessage;
	class IRmiHost;
	class BeforeRmiSummary;
	class AfterRmiSummary;
	class RmiContext;

	/** 
	\~korean
	PIDL 컴파일러가 생성한 Stub 클래스의 베이스 클래스

	주의 사항
	- 이 클래스를 유저가 직접 구현하지 말 것. PIDL 컴파일러에서 구현한 것을 쓰도록 해야 한다.

	\~english
	Base class of Stub class created by PIDL compiler

	Note
	- User must not create this class. Must be realized by PIDL compiler.

	\~chinese
	PIDL编译器生成的Stub类的基本类。

	注意事象
	- 用户不要直接体现此类。在PIDL编译器要使用体现的东西。

	\~japanese
	PIDLコンパイラーが生成したStubクラスのベースクラス

	ご注意事項
	- このクラスをユーザーが直接実現しないこと。PIDLコンパイラーで実現したものを使うようにしなければなりません。
	\~
	*/
	class  IRmiStub
	{
	public:
		IRmiHost *m_core;
		bool m_internalUse; // true이면 ProudNet 전용. 사용자는 건들지 말것

		/** 
		\~korean
		true로 세팅하면 NotifyCallFromStub 을 호출받을 수 있다.
		그러나, 그 댓가로 실행 속도가 현저히 떨어진다. 디버깅을 할 때만 켜는
		것을 권장한다.

		\~english
		If set as true then calls NotifyCallFromStub.
		But in return, process speed will be lowered significantly. It is recommended to use this when debugging.

		\~chinese
		设置为true的话可以呼叫NotifyCallFromStub。
		但运行速度会明显下降。建议只在调试的时候打开。

		\~japanese
		trueに設定するとNotifyCallFromStubを呼び出してもらうことができます。
		しかし、その対価で実行速度が著しく落ちます。デバッギングをする時のみつけることをお勧めします。
		\~
		*/
		bool m_enableNotifyCallFromStub;

		/** 
		\~korean
		true로 설정하면 BeforeRmiInvocation,AfterRmiInvocation 를 콜백한다.
		그러나 그 댓가로 실행 속도가 약간 떨어진다. 성능 최적화를 위해 RMI 함수 종류별 실행
		시간을 체크할 때만 켜는 것을 권장한다.

		\~english
		If set as true then BeforeRmiInvocation and AfterRmiInvocation are called back.
		But in return, process speed will be lowered a little. It is recommended to use this when checking running time of RMI function of each type.

		\~chinese
		设置为true的话回调BeforeRmiInvocation,AfterRmiInvocation。
		但运行速度会明显下降。为了性能的优化，建议只在检查RMI函数种类运行时间的时候打开。

		\~japanese
		trueに設定すると、BeforeRmiInvocation,AfterRmiInvocationをコールバックします。
		しかし、その対価で実行速度が少し落ちます。性能の最適化のためにRMI関数の種類別実行時間をチェックする時のみつけることをお勧めします。
		\~
		*/
		bool m_enableStubProfiling;

		/** 
		\~korean
		이 함수를 구현하지 말 것. PIDL 컴파일러의 결과물이 override한다.

		\~english
		DO NOT realize this function. The outcome of PIDL compiler will override.

		\~chinese
		不要体现此函数。PIDL 编译器的产物会override。

		\~japanese
		この関数を実現しないこと。PIDLコンパイラーの結果物がオーバーライドされます。
		\~
		*/
		virtual RmiID* GetRmiIDList() = 0;
		/** 
		\~korean
		이 함수를 구현하지 말 것. PIDL 컴파일러의 결과물이 override한다.

		\~english
		DO NOT realize this function. The outcome of PIDL compiler will override.

		\~chinese
		不要体现此函数。PIDL 编译器的产物会override。

		\~japanese
		この関数を実現しないこと。PIDLコンパイラーの結果物がオーバーライドされます。
		\~
		*/
		virtual int GetRmiIDListCount() = 0;
		/** 
		\~korean
		이 함수를 구현하지 말 것. PIDL 컴파일러의 결과물이 override한다.

		\~english
		DO NOT realize this function. The outcome of PIDL compiler will override.

		\~chinese
		不要体现此函数。PIDL 编译器的产物会override。

		\~japanese
		この関数を実現しないこと。PIDLコンパイラーの結果物がオーバーライドされます。
		\~
		*/
		virtual bool ProcessReceivedMessage(CReceivedMessage& pa, void* hostTag) = 0;
		/** 
		\~korean
		RMI가 실행된 직후 호출된다.
		AfterRmiInvocation.m_rmiName은 USE_RMI_NAME_STRING을 define해야 문자열이 등장합니다.

		\~english
		Called right after RMI is run
		In AfterRmiInvocation.m_rmiName's case, a character string appears after you define USE_RMI_NAME_STRING.

		\~chinese
		RMI运行后立即被呼叫。
		AfterRmiSummary.m_rmiName需要USE_RMI_NAME_STRING定义后出现字符串.

		\~japanese
		RMIが実行された後に呼び出されます。
		AfterRmiInvocation.m_rmiNameはUSE_RMI_NAME_STRINGをdefineしたら文字がでます。

		\~
		*/
		 PROUD_API virtual void AfterRmiInvocation(const AfterRmiSummary& summary);
		/** 
		\~korean
		RMI가 실행되기 직전에 호출된다.
		BeforeRmiSummary.m_rmiName은 USE_RMI_NAME_STRING을 define해야 문자열이 등장합니다.

		\~english
		Called right after RMI is run
		In BeforeRmiSummary.m_rmiName's case, a character string appears after you define USE_RMI_NAME_STRING.

		\~chinese
		RMI运行之前被呼叫。
		BeforeRmiSummary.m_rmiName需要USE_RMI_NAME_STRING定义后出现字符串.

		\~japanese
		RMIが実行される前に呼び出されます。
		BeforeRmiSummary.m_rmiNameはUSE_RMI_NAME_STRINGをdefineしたら文字がでます。

		\~
		*/
		 PROUD_API virtual void BeforeRmiInvocation(const BeforeRmiSummary& summary);

		/** 
		\~korean
		유저가 이 함수를 override하면, RMI가 실행되면서 받은 파라메터를 문자열로 모두 표시할 수 있게 해준다.
		단, 성능이 매우 떨어지게 되므로 주의해서 쓰도록 하자.

		\~english
		If user override this function then it lets parameter received from RMI be displayed as text string.
		But in return, process speed will be lowered significantly so be careful when you use this.

		\~chinese
		用户override此函数的话，可以把RMI运行的时候接收的所有参数显示为字符串。
		但是，性能会下降，所以使用时要注意。

		\~japanese
		ユーザーがこの関数をオーバーライドすれば、RMIが実行されながら受けたパラメーターを文字列で全て表示できるようにします。
		但し、性能が非常に落ちるようになるので注意して使ってください。
		\~
		*/
		 PROUD_API virtual void NotifyCallFromStub(HostID remote, RmiID RMIId, String methodName, String parameters);

		/** RMI stub에서, 수신된 데이터를 deserialize를 하기 전에 호출됩니다.
		여러분은 여기서 받은 데이터와 RMI ID를 미리 열람하실 수 있습니다.
		이 함수는 기본적으로 true를 리턴합니다. 
		만약 여러분이 이 함수를 오버라이드해서 false를 리턴하게 하면 해당 RMI 함수는 호출되지 않을 것입니다.
		\param remote 송신자
		\param rmiContext 수신한 추가 정보
		\param message deserialize되기 직전의, 받은 데이터 */
		 PROUD_API virtual bool BeforeDeserialize(HostID remote, RmiContext& rmiContext, CMessage& message);

		 PROUD_API void ShowUnknownHostIDWarning(HostID remoteHostID);

		 PROUD_API IRmiStub();
		 PROUD_API virtual ~IRmiStub();
	};

	 extern const PNTCHAR* DecryptFailedError;

	/**  @} */
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
