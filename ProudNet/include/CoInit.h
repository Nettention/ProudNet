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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup db_group
	*  @{
	*/

	/** 
	\~korean
	CoInitialize, CoUninitialize 호출 쌍을 맞춘다.
	- 일반적으로 CoInitialize와 CoUninitialize는 호출 횟수가 균형을 맞춰야 하며, 각 스레드 별로 호출해야 하는 메서드이다.
	자세한 것은 각 API의 문서를 참고할 것.
	- 직접 CoInitialize(), CoUninitialize()를 호출하는 것보다 이 클래스를 로컬 변수로 선언하는 것이 더 안정적이다.
	이 클래스는 자동으로 CoUninitialize()를 필요할 때 호출하기 때문이다.
	
	\~english
	This method works in a set of CoInitialize and CoUninitialize call.
	- It needs to be called per each threads and normally the number of calls for CoInitialize and CoUninitialize should be balanced.
	Refer to API document for more detailed information.
	- It's safer to declare this class as a local variable rather than directly calling for CoInitialize() and CoUninitialize(),
	since it automatically calls for CoUninitialize() when needed.
	
	\~chinese
	CoInitialize, CoUninitialize呼叫对齐。
	- 一般来说CoInitialize和CoUninitialize的呼叫次数要对准平衡，以每线程呼叫的方式。
	详细的请参考各API文件。
	- 比起直接呼叫CoInitialize()，CoUninitialize()方法，把此类宣告为本地变数更稳定。
	因为此类需要CoUninitialize()的时候会自动呼叫。
	
	\~japanese
	CoInitialize, CoUninitialize 呼び出しペアを合わせます。
	- 一般的に、CoInitializeとCoUninitializeは呼び出し回数にバランスを取る必要があって、各スレッド別に呼び出さなければならないメソッドです。
	詳しくは各APIの文書をご参照ください。
	- 直接CoInitialize(), CoUninitialize()を呼び出すことよりは、このクラスをローカル変数で宣言した方がもっと安定的です。
	このクラスは自動にCoUninitialize()を必要とする時に呼び出すためです。
	\~
	*/
	class CCoInitializer
	{
	private:
		bool m_success;

	public:
		 PROUD_API CCoInitializer();
		 PROUD_API ~CCoInitializer();
	};

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
