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

//#include <string> 사용자에게 노출되는 헤더 파일은 STL을 사용할 수 없으므로
#include "BasicTypes.h"
#include "atomic.h"
#include "PNString.h"
#include "pnmutex.h"
#include "ClassBehavior.h"

#if !defined(_WIN32)
    #include <pthread.h>
    #include <time.h>
#endif

#if defined(_MSC_VER) && defined(_WIN32)
#pragma comment(lib,"dbghelp.lib") // Lock_Interval에서 MiniDumpWriteDump를 쓰기 때문에.
#endif

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
	class CriticalSection_BottleneckDetector;
	class CriticalSection_Standard;

	uint32_t GetAppropriateSpinCount();

	/** parameters for critical section initialization.
	*/
	class CriticalSectionSettings
	{
	public:
		/// spin count. will be set to appropriate value by default.
		/// This value will be set to 0 if only one CPU core is being used.
		uint32_t m_spinCount;

		/**
		\~korean
		병목 발생시 덤프 파일을 저장할 이름을 설정합니다.
		(ex. L"example_dump_file_name/")
		\~english
		Select the name to save the dump file when bottleneck occurs. 
		(ex. L"example_dump_file_name/")

		\~chinese

		\~japanese
		ボットルネックが発生時、ダンプファイルを保存する名前を設定します。
		(ex. L"example_dump_file_name/")

		\~
		*/
		String m_bottleneckWarningDumpFileName;

		/**
		\~korean
		병목 발생시 덤프 파일을 저장할 경로를 설정합니다.
		(ex. L"C:/")
		\~english
		Set the path to save the dump file when bottleneck occurs. 
		(ex. L"C:/")

		\~chinese

		\~japanese
		ボットルネックが発生時、ダンプファイルを保存する経路を設定します。
		(ex. L"C:/")


		\~
		*/
		String m_bottleneckWarningDumpFilePath;

		/**
		\~korean
		경고가 발생할때의 최대 시간값을 설정합니다.
		해당값이 0 이외의 값으로 세팅되면 NetServer 내부에서 해당 값 만큼 병목 발생시 경고와 덤프 파일을 남깁니다.
		\~english
		Set the maximum time value when a warning occurs. 
		When the value is set for a value other than 0, it leaves a warning and a dump file when bottleneck occurs equal to the value inside NetServer.  

		\~chinese

		\~japanese
		警告が発生する時の最大時間値を設定します。
		該当値が0以外の値で設定されれば、NetServer内部で該当値だけボットルネックが発生時に警告とダンプファイルを残します。

		\~
		*/
		uint32_t m_bottleneckWarningThresholdMs;

		/** If you this value to true,
		Proud.CriticalSection.GetLastLockedThreadID() is valid. */
		bool m_updateLastLockedThreadID;

		 PROUD_API CriticalSectionSettings();
	};


	class CriticalSection_BottleneckDetector;
	// _BottleneckDetectorAwareLock 반환값
	class LockBottleneckDetectorResult
	{
	public:
		LockResult m_lockResult;
		uint64_t m_owningThread; // lock을 성공시킨 스레드의 ID. pthread_t or win32 thread ID다.
	};



	/** \addtogroup util_group
	*  @{
	*/
	/**
	\~korean
	Critical section 클래스
	- MFC의 CCriticalSection보다 기능이 더 보강되어 있다.
	- 특히 (SMP 환경에서) thread sleep state를 최대한 줄이기 위해 기본적으로 spin lock이 설정되어 있다.
	일반적 용도
	- CriticalSection 객체를 먼저 만든 후, CriticalSectionLock 객체를 통해 critical section lock & unlock을 할 수 있다.

	\~english
	Critical section (class)
	- More fortified features than CCriticalSection of MFC environment
	- A spin lock is set as default in order to minimize thread sleep state under SMP environment.
	General usage
	- It is possible to perform critical section lock/unlock through CriticalSectionLock object after creating it.

	\~chinese
	Critical section类
	- 比MFC的 CCriticalSection%功能加强了。
	- 特别是（SMP环境下）为了把thread sleep state最大程度减少，基本都设置了spin lock。
	一般用途
	先把CriticalSection对象创建后，通过CriticalSectionLock对象可以critical section lock & unlock。

	\~japanese
	Critical section クラス
	- MFCのCCriticalSectionより機能がもっと補強されています。
	- 特に(SMP環境で)、thread sleep stateを最大限に減らすために基本的にspin lockが設定されています。
	一般的な用途
	- CriticalSection オブジェクトを先に作った後、CriticalSectionLock オブジェクトを通じcritical section lock & unlockを行うことができます。

	\~
	 */
	class CriticalSection
	{
		//String m_name;
		uint32_t m_validKey;

		CriticalSectionSettings m_settings;

		CriticalSection_Standard* m_standard;

		// 0: 덤프 안 남기는 중, 1: 덤프 남기고 있는 중
		CriticalSection_BottleneckDetector* m_bottleneckDetector;
	public:
		/** Initializes a critical section object. */
		PROUD_API CriticalSection();

		/** Initializes a critical section object. */
		PROUD_API CriticalSection(CriticalSectionSettings& settings);

	private:
		void _Initialize(CriticalSectionSettings& settings);
		void _Uninitialize();

		void _BottleneckDetectorAwareLock(int32_t timeout, LockBottleneckDetectorResult& result);

		void Lock_internal();
		inline bool IsBottleneckWarningEnabled()
		{
			return (bool)(m_settings.m_bottleneckWarningThresholdMs > 0);
		}

		bool IsOwningThreadAlive() const;

	public:
		/**
		\~korean
		파괴자
		- 파괴하기 전에, 이 critical section을 점유하고 있는 스레드가 없어야 한다!
		\~english
		Destroyer
		- There must be no thread occupying the critical section before the desruction.
		\~chinese
		破坏者
		- 破坏之前，不能有占有此critical section的线程！
		\~japanese
		破壊子
		- 破壊する前に、このcritical sectionを占有しているスレッドがあってはいけません!
		\~
		  */
		PROUD_API ~CriticalSection(void);

		/**
		\~korean
		critical section을 이 메서드를 호출하는 스레드가 점유한다.
		잘못된 호출이 일어났을 시 MessageBox를 띄워줍니다.
		\~english
		The critical section is to be occupied by the thread calling this method.
		\~chinese
		呼叫此方法的线程占有critical section。
		发生错误呼叫时打开MessageBox。
		\~japanese
		\~
		 */
		PROUD_API void Lock();

		/**
		\~korean
		critical section을 이 메서드를 호출하는 스레드가 점유한다.
		잘못된 호출이 일어났을 시 아무 처리하지 않고 넘어갑니다.
		\~english TODO:translate needed.
		The critical section is to be occupied by the thread calling this method.
		\~chinese
		呼叫此方法的线程占有critical section。
		发生错误的呼时不做任何处理。
		\~japanese
		critical sectionをこのメソッドを呼び出すスレッドが占有します。
		間違った呼び出しが発生した時、MessageBoxを表示します。
		\~
		 */
		void UnsafeLock();

		/**
		\~korean
		critical section을 이 메서드를 호출하는 스레드가 점유 해제한다.
		\~english
		The critical section is to be relieved by the thread calling this method.
		\~chinese
		呼叫此方法的线程解除占有critical section。
		\~japanese
		critical sectionをこのメソッドを呼び出すスレッドが占有解除します。
		\~
		 */
		PROUD_API void Unlock();

		/**
		\~korean
		EnterCriticalSection 대신 TryEnterCriticalSection을 사용한다.
		\return 성공적으로 잠금 했으면 true.
		\~english
		Use TryEnterCriticalSection rather than EnterCriticalSection.
		\return If it locked successfully, it is true.
		\~chinese
		TryEnterCriticalSection代替EnterCriticalSection使用。
		\return 锁定成功的话true。
		\~japanese
		EnterCriticalSectionの代わりにTryEnterCriticalSectionを使用します。
		\return ロックに成功したらtrue。
		\~
		 */
		PROUD_API bool TryLock();
		bool IsValid();

#ifdef _WIN32
		/**
		\~korean
		Windows 전용 함수입니다. LockCount나 OwningThread를 얻기 위해 CRITICAL_SECTION 구조체 포인터를 리턴합니다.
		\~english
		Returns CRITICAL_SECTION object for Windows platform. Useful for getting LockCount or OwningThread value.
		\~chinese
		Returns CRITICAL_SECTION object for Windows platform. Useful for getting LockCount or OwningThread value.
		\~japanese
		Returns CRITICAL_SECTION object for Windows platform. Useful for getting LockCount or OwningThread value.
		\~
		*/
		inline CRITICAL_SECTION* GetWin32Object() const
		{
			return m_platformSpecificObject;
		}
#endif

#ifndef _WIN32
		/** Returns pthread_mutex_t object. */
		inline pthread_mutex_t* GetMutexObject() const
		{
			return m_platformSpecificObject;
		}
#endif

#ifdef PN_LOCK_OWNER_SHOWN
		/** Returns true if this is locked by current thread. */
		bool IsLockedByCurrentThread() const;

		/** Returns true if this is locked by any thread. */
		bool IsLocked() const;
#endif

		// 이 값이 true이면 파괴자에서 내부 객체를 파괴 안함.
		// 이 값 다루기가 필요한 경우: 이 타입의 전역 객체이고, 프로세스 종료시, 객체 파괴 순서와 무관하게 다루고자 할 때, 누수를 감수하더라도.
		bool m_neverCallDtor;

		// Critical Section 의 Setting 값을 바꾸고 싶을때
		PROUD_API void Reset(CriticalSectionSettings& settings);

		void ShowErrorOnInvalidState();

		//////////////////////////////////////////////////////////////////////////
		// lock을 할 때 context switch가 발생했는지를 알 수 있다.
		// 성능 예: object pool을 위한 경우로 사용되는 경우 보통 700:1 정도의 contention을 일으킨다.

		// try-lock 성공 회수
		int m_tryLockSuccessCount;

		// try-lock 실패 횟수. 즉 일반 lock 횟수.
		int m_tryLockFailCount;

		NO_COPYABLE(CriticalSection)


	public:
		/** returns ID of the thread where the last acquire (lock) is done.

		You must set Proud.CriticalSectionSettings.m_updateLastLockedThreadID to true before using this function.

		NOTE: on unix, this is pthread_t value. This may differ from thread ID shown in debugger.

		NOTE: This value does not set to zero if last unacquire (unlock) is done.
		*/
		inline uint64_t GetLastLockedThreadID()
		{
			return m_lastLockedThreadID;
		}

	private:
		uint64_t m_lastLockedThreadID;


		/////////////////////////
		// PLATFORM SPECIFIC 멤버 변수들이다. 이것들은 이 클래스의 맨 뒤에 선언되어있도록 하자. 
	public:

		/** 여러분이 디버거에서 이 내부 상태값을 보고 싶으시면 이 멤버 변수의 값을 확인하십시오.
		*/
#ifdef _WIN32
		CRITICAL_SECTION* m_platformSpecificObject;
#else
		/** 여러분이 디버거에서 이 내부 상태값을 보고 싶으시면 이 멤버 변수의 값을 확인하십시오.
		*/
		pthread_mutex_t* m_platformSpecificObject;
#endif
	};

	//typedef RefCount<CriticalSection,false> CriticalSectionPtr;

	/**
	\~korean
	\brief CriticalSection 객체를 lock access하는 객체입니다.
	- 일반적으로 로컬 변수로서 사용됩니다. 
	- 이 객체가 파괴될 때 자동으로 lock하고 있던 critical section을 unlock합니다.
	\~english
	 \brief The object that 'lock accesses' CriticalSection object.
	- Usually created and used as a local variable.
	- When this object is destroyed, it automatically unlocks the critical section that was locked by this.
	\~chinese
	\brief CriticalSection 把对象lock access的对象。
	- 主要生成本地变数后使用。
	- 此对象被破坏的时候自动unlock正lock的critical section。
	\~japanese
	\brief CriticalSection オブジェクトをlock accessするオブジェクト
	- 必ずローカル変数で生成して一つのスレッド内でのみ使います。
	- このオブジェクトが破壊される時に自動ロックしていたcritical sectionをunlockします。
	\~

	\code
	// Example
	void foo()
	{
		CriticalSectionLock lock(critSec, true);
		... 
		
		// automatically unlocked here.
	}
	\endcode
	*/
	class CriticalSectionLock
	{
		CriticalSection* m_cs;

		// 몇 번의 Lock을 호출했는가?
		// 주의: m_cs의 실제 lock 횟수와 다를 수 있다.
		// 이 값은 이 CriticalSectionLock에 의해서 Lock한 것들만 카운팅한다.
		int m_recursionCount;

	public:
		/**
		\~korean
		critical section 객체를 생성자에서 바로 lock할 수 있다.
		\param cs 사용할 critical section 객체
		\param initialLock true이면 생성자에서 바로 lock한다.
		\~english
		It is possible to directly lock the critical section object at constructor.
		\param cs The critical section object to use
		\param initialLock If it is true then the constructor immediately locks it.
		\~chinese
		在生成者可以立即lock critical section对象。
		\param cs 要使用的critical section对象
		\param initialLock true的话在生成者立即lock。
		\~japanese
		critical section オブジェクトを生成子ですぐロックできます。
		\param cs 使用する critical section オブジェクト
		\param initialLock trueであれば生成子ですぐロックします。
		\~
		 */
		inline CriticalSectionLock(CriticalSection& cs, bool initialLock)
		{
			SetCriticalSection(cs, initialLock);
		}

		/**
		\~korean
		critical section을 나중에 세팅할때에 사용할 생성자입니다.
		\~english
		This is constructor that using set critical section later.
		\~chinese
		以后设置critical section的时候要使用的生成者。
		\~japanese
		critical sectionを後で設定する時に使う生成子です。
		\~
		*/
		inline CriticalSectionLock()
		{
			m_cs = NULL;
			m_recursionCount = 0;
		}
		inline void SetCriticalSection(CriticalSection &cs,bool initialLock)
		{
			m_cs = &cs;
			m_recursionCount = 0;
			if (initialLock)
			{
				Lock();
			}
		}
		// 쓰일 일이 없어 보이지만, 현재 스레드가 이 critsec을 잠그고 있는 상태인지 검사를 해야 할 때가 있어서
		// 이 함수는 존재함.
		inline CriticalSection* GetCriticalSection() const
		{
			return m_cs;
		}

		/**
		\~korean
		잠금이 되어있나?
		\~english
		Is it locked?
		\~chinese
		锁定了吗？
		\~japanese
		ロックされていますか？
		\~
		 */
		inline bool IsLocked() const
		{
			assert(m_recursionCount >= 0);
			return m_recursionCount > 0;
		}

		/**
		\~korean
		파괴자
		- 이미 이 객체가 점유하고 있던 CriticalSection이 있을 경우 점유 해제를 자동으로 한다.
		\~english
		Destructor
		- If there is a Critical Section occupied by this object then it automatically relieves.
		\~chinese
		破坏者
		- 此对象已经占有CriticalSection的时候自动解除。
		\~japanese
		破壊子
		- 既にこのオブジェクトが占有していたCriticalSectionがある場合、占有解除を自動で行います。
		\~
		 */
		inline ~CriticalSectionLock()
		{
			assert(m_recursionCount >= 0);
			for ( ; m_recursionCount > 0; --m_recursionCount )
			{
				m_cs->Unlock();
			}
		}

		/**
		\~korean
		critical section을 점유한다.
		\~english
		This occupies the critical section.
		\~chinese
		占有critical section。
		\~japanese
		critical sectionを占有します。
		\~
		 */
		inline void Lock()
		{
			m_cs->Lock();
			++m_recursionCount;
		}

		/**
		\~korean
		Try Lock을 수행한다.
		\return CriticalSection.TryLock()과 같은 값
		\~english
		Perform Try Lock.
		\return Same value as CriticalSection.TryLock()
		\~chinese
		执行Try Lock。
		\return 与CriticalSection.TryLock()一样的值。
		\~japanese
		Try Lockを遂行します。
		\return CriticalSection.TryLock()と同じ値
		\~
		 */
		inline bool TryLock()
		{
			bool r = m_cs->TryLock();
			if ( r )
			{
				++m_recursionCount;
			}
			return r;
		}

		// 내부적 용도로만
		inline void UnsafeLock()
		{
			m_cs->UnsafeLock();
			++m_recursionCount;
		}

		/**
		\~korean
		critical section을 점유 해제한다.
		\~english
		This relieves occupied critical section.
		\~chinese
		解除占有critical section。
		\~japanese
		critical sectionを占有解除します。
		\~
		 */
		inline void Unlock()
		{
			if ( IsLocked() )
			{
				--m_recursionCount;
				m_cs->Unlock();
			}
		}

		/**
		\~korean
		락이 중첩된 횟수를 조회한다.

		\~english
		Inquire the time that the lock is over

		\~chinese

		\~japanese
		ロックが重なった回数を照会します。
		\~
		*/
		inline int GetRecursionCount()
		{
			assert(m_recursionCount >= 0);
			return m_recursionCount;
		}

		NO_COPYABLE(CriticalSectionLock)
	};

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
};

#ifdef _MSC_VER
#pragma pack(pop)
#endif
