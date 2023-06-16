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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** 
	\~korean
	사용자는 이 메서드도 구현해야 합니다.
	
	참고
	- C++ 이외 버전에서는 본 메서드는 delegate callback 형태일 수 있습니다. 
	
	\~english TODO:translate needed.
	Users must implement this method. 
	
	Note
	- In versions other than C++, this method may be in the form of delegate callback. 

	\~chinese
	用户也要体现这个方法。

	参考
	- C++以外版本中此方法可能是delegate callback形式。

	\~japanese
	ユーザーはこのメソッドも実現しなければなりません。

	参考
	- C++ 以外のバージョンでは、このメソッドはdelegate callback形態になることがあります。 

	*/
	class IHlaDelegate_Common
	{
	public:
		virtual ~IHlaDelegate_Common() {}
		
		/** 
		\~korean
		HLA 내부 모듈에 의해 잠금 보호가 요청되면 이 메서드가 콜백됩니다. 사용자는 이를 구현해야 합니다.
		예를 들어, 당신은 이 루틴에서 메모리를 보호할 critical section 잠금을 하실 수 있겠습니다. 
		
		\~english TODO:translate needed.
		If lock protection is requested by an HLA internal module, this method is called back. Users must implement this. 
		For example, you may do critical section lock that would protect the memory from this routine. 
		
		\~chinese
		被HLA内部模块邀请锁定保护的话此方法会回拨。用户要体现这个。
		例如，你可以在此routine进行保护内存的critical section锁定。

		\~japanese
		HLA内部モジュールによってロック保護が要請されたらこのメソッドがコールバックされます。ユーザーはこれを実現しなければなりません。
		例えば、あなたはこのルーチンでメモリーを保護するcritical sectionロックをすることができます。

		*/
		virtual void HlaOnLockCriticalSection() = 0;

		/** 
		\~korean
		HlaOnLockCriticalSection()의 반대 역할을 합니다. 
		
		\~english TODO:translate needed.
		It performs the opposite role of HlaOnLockCriticalSection()
		
		\~chinese
		起着HlaOnLockCriticalSection()的相反作用。

		\~japanese
		HlaOnLockCriticalSection()の反対役割をします。 

		*/
		virtual void HlaOnUnlockCriticalSection() = 0;
	};
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
