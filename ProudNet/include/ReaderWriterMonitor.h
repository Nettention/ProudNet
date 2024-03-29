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

#include "./CriticalSect.h"
#include "./Event.h"

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

#if defined(_WIN32)    
	class CReaderLock_NORECURSE;
	class CWriterLock_NORECURSE;

	// 내부적으로만 쓰는 클래스고, 성능을 위해 범용성을 희생한다
	// 따라서 재귀 잠금 불허
	// *주의* 재귀 접근시 감지하지 않고 데드락으로 이어진다!
	class CReaderWriterMonitor_NORECURSE
	{
		friend CReaderLock_NORECURSE;
		friend CWriterLock_NORECURSE;
		CriticalSection m_cs; // 추후에는 lock free 알고리즘으로 구현해야 하겠다. 일단은 이렇게 만든다.

		int m_readerLockCount;
		int m_writerLockCount;
		uint32_t m_owningThread;
		Event m_noReaderLocked;
		Event m_noWriterLocked;

		PROUD_API int ReaderLock();
		PROUD_API void ReaderUnlock();
		PROUD_API void WriterLock();
		PROUD_API void WriterUnlock();

	public:
		 PROUD_API CReaderWriterMonitor_NORECURSE(void);
		 PROUD_API ~CReaderWriterMonitor_NORECURSE(void);

	};

	class CReaderLock_NORECURSE
	{
		CReaderWriterMonitor_NORECURSE* m_cs;
		bool m_locked;
		int m_multipleReaderCount;
	public:
		/** 
		\~korean
		reader lock 객체를 생성자에서 바로 lock할 수 있다.
		\param cs 적용할 reader lock 객체
		\param initialLock true이면 생성자에서 바로 lock한다. 

		\~english
		Reader lock object can be directly locked at constructor.
		\param cs reader lock object to be applied
		\param initialLock if true then constructor locks directly

		\~chinese
		能把reader lock对象在生成者直接lock。
		\param cs 要适用的reader lock对象。
		\param initialLock true的话从生成者直接进行lock。

		\~japanese
		reader lock オブジェクトを生成子よりすぐロックできます。
		\param cs 適用するreader lockオブジェクト
		\param initialLock trueであれば生成子よりすぐロックします。 
		\~
		*/
		inline CReaderLock_NORECURSE(CReaderWriterMonitor_NORECURSE& cs, bool initialLock)
		{
			m_multipleReaderCount = 0;
			m_cs = &cs;
			m_locked = false;
			if (initialLock == true)
			{
				m_multipleReaderCount = m_cs->ReaderLock();
				m_locked = true;
			}
		}

		/** 
		\~korean
		파괴자 
		- 이미 이 객체가 점유하고 있던 CriticalSection 이 있을 경우 점유 해제를 자동으로 한다. 

		\~english
		Destructor 
		- If there is CriticalSection that is occupied by this object already then disoccupation will be performed automatically.

		\~chinese
		破坏者
		- 存在此对象已经占有的CriticalSection的时候，自动解除占有。

		\~japanese
		破壊子
		- 既にこのオブジェクトが占有していたCriticalSectionがある場合、占有解除を自動にします。 
		\~
		*/
		inline ~CReaderLock_NORECURSE()
		{
			if (m_locked)
				Unlock();
		}

		/** 
		\~korean
		reader lock을 점유한다.
		\return 잠그고 나서 얻은 multiple reader의 갯수 

		\~english
		Occupies reader lock
		\return number of multiple reader required after locking

		\~chinese
		占有reader lock。
		\return 锁定以后获得的multiple reader个数。

		\~japanese
		reader lockを占有します。
		\return ロックしてから得たmultiple readerの数
		\~
		*/
		inline void Lock()
		{
			m_locked = true;
			m_multipleReaderCount = m_cs->ReaderLock();
		}

		inline int GetMultipleReaderCount()
		{
			return m_multipleReaderCount;
		}

		/** 
		\~korean
		reader lock을 점유 해제한다. 

		\~english
		Withdraws reader lock

		\~chinese
		解除占有reader lock。

		\~japanese
		reader lockを占有解除します。 
		\~
		*/
		inline void Unlock()
		{
			m_cs->ReaderUnlock();
			m_locked = false;
		}
	};

	class CWriterLock_NORECURSE
	{
		CReaderWriterMonitor_NORECURSE* m_cs;
		bool m_locked;
	public:
		/** 
		\~korean
		Writer lock 객체를 생성자에서 바로 lock할 수 있다.
		\param 적용할 Writer lock 객체
		\param initialLock true이면 생성자에서 바로 lock한다. 

		\~english
		Writer lock object can be directly locked at constructor.
		\param to_be_applied Writer lock object
		\param initialLock if true then constructor locks directly

		\~chinese
		Writer lock 可以把对象从生成者直接lock。
		\param 适用的Writer lock对象。
		\param initialLock true的话从生成者直接进行lock。

		\~japanese
		Writer lock オブジェクトを生成子よりすぐロックできます。
		\param 適用する Writer lockオブジェクト
		\param initialLock trueであれば生成子よりすぐロックします。 
		\~
		*/
		inline CWriterLock_NORECURSE(CReaderWriterMonitor_NORECURSE& cs, bool initialLock)
		{
			m_cs = &cs;
			m_locked = false;
			if (initialLock == true)
			{
				m_cs->WriterLock();
				m_locked = true;
			}
		}

		/** 
		\~korean
		파괴자 
		- 이미 이 객체가 점유하고 있던 CriticalSection 이 있을 경우 점유 해제를 자동으로 한다. 

		\~english
		Destructor
		- If there is CriticalSection that is occupied by this object already then disoccupation will be performed automatically.

		\~chinese
		破坏者
		- 存在此对象已经占有的CriticalSection的时候，自动解除占有。

		\~japanese
		破壊者
		- 既にこのオブジェクトが占有していたCriticalSectionがある場合、占有解除を自動にします。 
		\~
		*/
		inline ~CWriterLock_NORECURSE()
		{
			if (m_locked)
				Unlock();
		}

		/** 
		\~korean
		Writer lock을 점유한다. 

		\~english
		Occupies writer lock

		\~chinese
		占有Writer lock。

		\~japanese
		Writer lockを占有します。
		\~
		*/
		inline void Lock()
		{
			m_locked = true;
			m_cs->WriterLock();
		}

		/** 
		\~korean
		Writer lock을 점유 해제한다.

		\~english
		Withdraws writer lock

		\~chinese
		解除Writer lock占有。

		\~japanese
		Writer lockを占有解除します。
		\~
		*/
		inline void Unlock()
		{
			m_cs->WriterUnlock();
			m_locked = false;
		}
	};

	enum eAccessMode
	{
		eAccessMode_None = 0,
		eAccessMode_Read = 1,
		eAccessMode_Write = 2
	};


	class CReaderWriterAccessChecker
	{
#ifdef _DEBUG
		volatile uint32_t	m_AccessThreadID;
		volatile eAccessMode m_AccessMode;
#endif // _DEBUG
	public:
		 PROUD_API CReaderWriterAccessChecker();
		 PROUD_API void AssertThreadID(eAccessMode eMode) const;
		 PROUD_API void ClearThreadID() const;
	};
#endif // __GNUC__

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
