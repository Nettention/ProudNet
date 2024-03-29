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
#include "Enums.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	밀리초 단위의 정밀도를 가지는 타이머입니다.
	- CMilisecTimer::New 를 통해 생성하고, delete로 삭제 합니다.
	- 내부적으로 QueryPerformanceCounter을 사용합니다.
	- 정밀도의 문제가 없으나, windows 버젼,CPU에 따라 잘못된 연산이 일어 날 수 있습니다.
	- 컴퓨터를 켜고 81706년까지 사용할수 있습니다.

	\~english
	It is a timer whose degree of precision is millisecond. 
	Create by “CMilisecTimer::New” and delete by “delete”.
	QueryPerformanceCounter is internally used. 
	No problem with precision but wrong calculation might occur depending on Windows version & CPU.
	Can use it up to  81706 after turning on the computer. 

	\~chinese
	精确度为毫秒单位的定时器。
	-通过 CMilisecTimer::New 生成，用delete删除。
	-内部使用 QueryPerformanceCounter。
	-精确度没有问题，但有时会根据Windows版本，CPU等产生错误演算。
	-在开启电脑后可一直使用至81706年。

	\~japanese
	ミリ秒単位の精密度を持つタイマーです。
	- CMilisecTimer::Newを使用して生成し、deleteで削除します。
	-内部的にはQueryPerformanceCounterを使用します。
	-精密度の問題はないが、 windowsバージョンのCPUによって間違った演算が起こる可能性があります。
	-コンピューターの電源を入れて81706年まで使用できます。

	\~
	*/
	class  CMilisecTimer
	{
	public:
		virtual ~CMilisecTimer(){}

		/**
		\~korean
		타이머를 초기화한다.

		\~english
		Initialize the timer

		\~chinese
		初始化Timer。

		\~japanese
		Timerを初期化する。

		\~
		*/
		 virtual void Reset() = 0;

		/**
		\~korean
		타이머를 시작한다.

		\~english
		Start the timer

		\~chinese
		开始Timer。

		\~japanese
		Timerをはじめる。

		\~
		*/
		 virtual void Start() = 0;

		/**
		\~korean
		타이머를 일시 정지한다.

		\~english
		Temporarily stop the timer. 

		\~chinese
		暂时终止Timer。

		\~japanese
		Timerを一時停止する。

		\~
		*/
		 virtual void Stop() = 0;

		/**
		\~korean
		타이머를 0.1초만큼 진행되게 한다.

		\~english
		Move the timer 0.1 second forward

		\~chinese
		让Timer只进行0.1秒。

		\~japanese
		Timerを0.1秒だけ進行させる。

		\~
		*/
		 virtual void Advance() = 0;

		/** 
		\~korean
		현재 시간을 얻는다. 

		\~english
		Gets current time

		\~chinese
		获取现在的时间。

		\~japanese
		現在の時間を得ます。

		\~
		*/
		 virtual int64_t GetTimeMs() = 0; 
		
		/** 
		\~korean
		전에 GetElapsedTimeMs() 호출로 부터 지난시간을 얻습니다.
		- MMTimer 의 경우, 정밀도에 문제가 있을 수 있습니다.
		- QPC의 경우, 정확한 시간을 얻긴 하지만, GetTickCount()보다 50배 정도 느리다.

		\~english TODO:translate needed.
		Get the elapsed time from the previous GetElapsedTimeMs() call. 
		- In the case of MMTimer, there may be a problem with the accuracy. 
		- In the case of QPC, you get accurate time, but it is 50 times slower than GetTickCount(). 

		\~chinese
		获取从之前GetElapsedTimeMs()呼叫开始经过的时间。
		- MMTimer的话精度可能会有问题。
		- QPC的话可以获得正确的时间，但比GetTickCount()慢大概50倍左右。

		\~japanese
		前のGetElapsedTimeMs()呼び出しから経過した時間を得ます。
		- MMTimerの場合、精密度に問題があり得ます。
		- QPCの場合、正確な時間を得ることはありますが、GetTickCount()より50倍ほど遅いです。
		\~
		*/
		 virtual int64_t GetElapsedTimeMs() = 0; 

		/** 
		\~korean
		타이머가 정지 상태이면 true를 리턴한다. 

		\~english
		Returns true when timer is paused

		\~chinese
		计时器是停止状态的话返回true。

		\~japanese
		タイマーが停止状態であればtrueをリターンします。
\~
		*/
		 virtual bool IsStopped() = 0; 


		/**
		\~korean
		type을 지정하여 timer을 생성합니다.

		\~english TODO:translate needed.
		Designate the type to generate the timer. 

		\~chinese
		指定type，生成timer。

		\~japanese
		Typeを指定してtimerを生成します。
\~
		*/
		 PROUD_API static CMilisecTimer* New();
	};

	/** 
	\~korean
	매우 정밀한 "현재 시간"을 얻는다. 정밀도는 1ms이며, 값을 얻어오는 부하는 critical section lock 1회이다.

	\~english
	Gets very precise "current time". 1ms resolution. the time cost of this function is a critical section lock.

	\~chinese
	获得非常精确的“现时间”。精确度为1ms，获取值的负荷为1次 critical section lock。

	\~japanese
	とても精密な"現在時間"を得ます。精度は1msで、値を取り入れる負荷はcritical section lock 1回です。
	\~
	*/
	 PROUD_API int64_t GetPreciseCurrentTimeMs();

	 /** epoch 즉 1970-1-1부터의 지난 시간(밀리초)를 구합니다. */
	 PROUD_API int64_t GetEpochTimeMs();


	/**  @} */
	}

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif
