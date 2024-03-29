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

#include <ctime>
#include "BasicTypes.h"
#include "PNString.h"
#include "Singleton.h"
#include "FastArray.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif

#ifdef _WIN32
#pragma comment(lib, "Shlwapi.lib") // PathCombine() 등을 쓰므로.
#endif

namespace Proud
{
	const int MaxCpuCoreCount = 4096; // 2050년이 넘어가면 이 값을 고쳐야 할지도 모른다.

	/** \addtogroup util_group
	*  @{
	*/

	/**
	\~korean
	Microsecond Sleep Function.

	\~englich TODO:suspend execution for microsecond intervals.

	\~chinese
	获得当前线程的状态。

	\~japanese
	\~
	*/
	PROUD_API void USleep(long uSec);

#if !defined(__ANDROID__) // glob는 안드로이드에서 못 쓴다.
	/** Find files that match fileNameWithWildcard(e.g. xxx\bb*.txt) to output. */
	PROUD_API void FindFiles(const String& directory, const String& searchFilter, CFastArray<String> &output);
#endif

	/**
	\~korean
	CPU갯수를 구한다.

	\~englich
	Get the number of CPUs.

	\~chinese
	求CPU个数。

	\~japanese
	CPU数を求めます。
	\~
	*/
	PROUD_API uint32_t GetNoofProcessors();

	/**
	\~korean
	프로세스의 이름을 얻어온다.

	\~english
	Get the name of the process.

	\~chinese
	获取区域名。

	\~japanese
	プルセス名を取り込みます。
	\~
	*/
	PROUD_API String GetProcessName();

	/** Returns current thread ID.
	- win32 kinds: same to ::GetCurrentThreadId().
	- unix kinds: same to pthread_self(). */
	PROUD_API uint64_t GetCurrentThreadID();

	/**
	 \~korean
	 ProudNet 사용자가 실수한 경우에만 보여주는 에러다.
	 - CNetConfig::UserMisuseErrorReaction 의 type에 따라서 MessageBox혹은 디버그뷰로 확인 할 수 있는 에러를 출력해준다.
	 \param text Error Comment

	 \~english
	This is an error shown only when a ProudNet user makes a mistake.
	- Depending on the type of CNetConfig::UserMisuseErrorReaction, it outputs the error that can be confirmed with either MessageBox or debugview.
	\param text Error Comment


	 \~chinese
	 ProudNet用户失误的时候显示的错误。
	 - 根据 CNetConfig::UserMisuseErrorReaction%的type，打印用MessageBox或者调试view可以确认的错误。
	 \param text Error Comment

	 \~japanese
	ProudNetユーザーが間違った場合のみ表示するエラーです。
	- CNetConfig::UserMisuseErrorReactionのtypeによってMessageBoxまたはデバッグビューで確認できるエラーを出力します。
	\param text Error Comment

	 \~
	 */
	PROUD_API void ShowUserMisuseError(const PNTCHAR* text);


	/**
	\~korean
	debug output or console에 디버깅용 문구를 출력합니다. newline은 추가되지 않습니다.

	\~english
	Prints text for debugging to debugger output window or console, without newline.

	\~chinese
	在debug output or console中输出调试用语句。不会添加newline。

	\~japanese
	debug output or consoleにデバッギング用文句を出力します。Newlineは追加されません。
	\~
	*/
	PROUD_API void TraceA(const char * format, ...);


#ifdef _WIN32
	/**
	\~korean
	물리적 메모리 점유율을 구한다.

	\~english
	Calculate the physical memory occupancy rate

	\~chinese
	求物理内存占有率。

	\~japanese
	物理的メモリー占有率を求めます。
	\~
	*/
	size_t GetOccupiedPhysicalMemory();

	/**
	\~korean
	총 메모리를 구한다.

	\~english
	Calculate the total memory

	\~chinese
	求总内存。

	\~japanese
	総メモリーを求めます。
	\~
	*/
	size_t GetTotalPhysicalMemory();

	/**
	\~korean
	컴퓨터 이름을 구한다.

	\~english
	Get the computer name.

	\~chinese
	求电脑名称。

	\~japanese
	コンピューター名を求めます。
	\~
	*/
	String GetComputerName();

	/**
	\~korean
	Windows XP, 2003에서만 지원하는 기능으로 low-fragmentation heap을 쓴다.
	- 이걸 쓰면 SMP 환경에서 heap 접근 속도가 대폭 향상된다. 물론 low-fragmentation의 이익도 얻는다.
	따라서 가능하면 이것을 쓰는 것이 강력 권장된다.
	- 이 메서드는 Windows 98 등에서는 지원되지 않는데, 이런 경우 이 메서드는 아무 것도 하지 않는다.
	하지만 Windows 98에서 필요 Win32 API를 못 찾아 에러가 발생하지는 않는다.

	\~english
	As the feature supported only in Window XP and 2003, it uses low-fragmentation heap.
	- When you use this, the heap access speed increases dramatically in SMP environment. Of course, you also get the benefits of low-fragmentation.
	Therefore, we strongly recommend you to use this if possible.
	- This method is not supported in Windows 98, etc. and in this case, this method does not do anything.
	However, errors do not occur when an essential Win32 API is not searched in Window 98.

	\~chinese
	是只在Windows XP, 2003支持的功能，使用low-fragmentation heap。
	- 使用这个的话在SMP环境下接近heap速度会大幅度提高。随之也会得到low-fragmentation 利益。所以可以的话强力推荐使用这个。
	- 此方法在Windows 98不支持，这时候此方法不会做任何事情。但是不会因Windows 98没有找到必要Win32 API而发生错误。

	\~japanese
	Windows XP, 2003でのみサポートする機能でlow-fragmentation heapを使います。
	- これを使うとSMP環境でheapのアクセス速度が大幅向上されます。勿論、low-fragmentationの利益も得ます。
	よって、できればこれを使うことを強力お勧めします。
	- このメソッドはWindows 98などではサポートしませんが、このような場合、このメソッドは何もしません。
	しかし、Windows 98で必要なWind 32 APIが見つからずエラーが発生することはありません。
	\~
	*/
	PROUD_API bool EnableLowFragmentationHeap();

	PROUD_API String GetCurrentProgramDirectory();

#endif

	/**
	\~korean
	이 프로그램이 실행되고 있는 폴더를 찾아서 current directory로 세팅한다.
	- 이렇게 하는 경우 다른 디렉토리에서 이 프로그램을 실행시켜도 media file을 제대로 찾는다.

	\~english
	Look for the folder where this program is being run and set as the current directory.
	- In this case, the media file is searched properly even this program is run in other directories.

	\~chinese
	找到此程序正在实行的文件后设置为current directory。
	- 这样的话，在其他directory运行此程序也会正确找到media file。

	\~japanese
	このプログラムが実行されているフォルダーを見つけてcurrent directoryで設定します。
	- このようにする場合、他のディレクトリーでこのプログラムを実行させてもmedia fileを正しく見つけます。
	\~
	*/
	PROUD_API void SetCurrentDirectoryWhereProgramExists();

	PROUD_API int SetCurrentDirectory(const PNTCHAR* directoryName);
	PROUD_API int64_t GetCurrentProcessId();


	/** 파일의 마지막 업데이트 시간을 리턴합니다.
	성공시 true 리턴 및 output에 값이 채워집니다. */
	PROUD_API bool GetFileLastWriteTime(const PNTCHAR* fileName, std::time_t& output);


	PROUD_API StringA UntabifyText(const StringA& text);
	PROUD_API StringA ReadTextFile(const char* fileName);


	PROUD_API bool CreateTempDirectory(const PNTCHAR* prefix, String& output);

	/** 파일이 존재하는지? */
	PROUD_API bool FileExists(const PNTCHAR* fileName);

#ifdef _WIN32

	/**
	\~korean
	현재 Process가 Service Mode로 실행중인지 여부를 검사합니다.
	\return 서비스모드면 1 아니면 0

	\~english
	Check whether the current process is being executed in service mode.
	\return  1 if service mode, otherwise 0.

	\~chinese
	检查现在的Process是否以Service Mode实行。
	\return 是服务模式的话是1不是的话0。

	\~japanese
	現在ProcessがService Modeで実行中なのかどうかを検査します。
	\return サービスモードであれば1、それとも0
	\~
	*/
	bool IsServiceMode();

	/**
	\~korean
	GetQueuedCompletionStatusEx 사용 가능 여부를 검사한다. Server2008이상만 사용할수 있다고 알려져 있다.
	\return 사용가능하면 1 아니면 0

	\~english
	Check whether it can be used. It is known that it can only be used in Server2008 or higher.
	\return 1 if usable, otherwise 0.

	\~chinese
	检查GetQueuedCompletionStatusEx是否可以使用。据说只在Server2008以上可以使用。
	\return 可以使用的话1，不是的话0。

	\~japanese
	GetQueuedCompletionStatusEx 使用可能可否を検査します。Server2008以上のみ使うことができると知られています。
	\return 使用可能であれば1、それとも0
	\~
	*/
	bool IsGqcsExSupported();

	/**
	\~korean
	HyperThreading를 지원하는지를 검사한다.

	\~english
	Check whether it supports HyperThreading.

	\~chinese
	检查是否支持HyperThreading。

	\~japanese
	HyperThreadingをサポートするかどうかを検査します。
	\~
	*/
	bool IsHyperThreading();

	/**
	\~korean
	비트가 1인 수를 체크합니다.
	\param bitMask 비트를 검사할 변수
	\return 비트가 1인 수

	\~english
	Check the number with the bit as 1.
	\param bitMask Parameter to check the bit.
	\return Number with the bit as 1.


	\~chinese
	打勾bit为1的数。
	\param bitmask 检查bit的变数。
	\return bit为1的数。

	\~japanese
	ビットが1である数をチェックします。
	\param bitMask ビットを検査する変数
	\return ビットが1である数
	\~
	*/
	uint32_t CountSetBits(uintptr_t bitMask);

	typedef
		/*__out_opt*/
		HANDLE
		(WINAPI* CreateIoCompletionPortProc)(
			/*__in*/     HANDLE FileHandle,
			/*__in_opt*/ HANDLE ExistingCompletionPort,
			/*__in*/     ULONG_PTR CompletionKey,
			/*__in*/     uint32_t NumberOfConcurrentThreads
			);

	typedef
		BOOL
		(WINAPI* HeapSetInformationProc) (
			/*__in_opt*/ HANDLE HeapHandle,
			/*__in*/ HEAP_INFORMATION_CLASS HeapInformationClass,
			/*__in_bcount_opt HeapInformationLength*/ PVOID HeapInformation,
			/*__in*/ size_t HeapInformationLength
			);

	typedef
		BOOL
		(WINAPI* QueueUserWorkItemProc) (
			/*__in*/     LPTHREAD_START_ROUTINE Function,
			/*__in_opt*/ PVOID Context,
			/*__in*/     uint32_t Flags
			);

	typedef
		BOOL
		(WINAPI* GetQueuedCompletionStatusProc) (
			/*__in*/  HANDLE CompletionPort,
			/*__out*/ LPDWORD lpNumberOfBytesTransferred,
			/*__out*/ PULONG_PTR lpCompletionKey,
			/*__out*/ LPOVERLAPPED *lpOverlapped,
			/*__in*/  uint32_t dwMilliseconds
			);

	// win32 api에서 복사해온 것임
	struct PN_OVERLAPPED_ENTRY {
		ULONG_PTR lpCompletionKey;
		LPOVERLAPPED lpOverlapped;
		ULONG_PTR Internal;
		DWORD dwNumberOfBytesTransferred;
	};

	typedef
		BOOL
		(WINAPI* GetQueuedCompletionStatusExProc) (
			/*__in  */HANDLE CompletionPort,
			/*__out_ecount_part(ulCount, *ulNumEntriesRemoved)*/ PN_OVERLAPPED_ENTRY* lpCompletionPortEntries,
			/*__in  */uint32_t ulCount,
			/*__out */PULONG ulNumEntriesRemoved,
			/*__in  */uint32_t dwMilliseconds,
			/*__in  */BOOL fAlertable
			);

	typedef
		PVOID
		(WINAPI* AddVectoredExceptionHandlerProc) (
			/*__in*/          uint32_t FirstHandler,
			/*__in*/          PVECTORED_EXCEPTION_HANDLER VectoredHandler
			);

	typedef
		BOOL
		(WINAPI* GetLogicalProcessorInformation)(
			/*__out */	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
			/*))inout */ PDWORD ReturnLength
			);

	/**
	\~korean
	파일의 절대 경로 이름을 구한다. 이미 절대 경로 이름이면 그대로 리턴한다.

	\~english
	Get the absolute pathname of the file. If it is already the absolute pathname, return as it is.

	\~chinese
	求文件的绝对路径名称。已经是绝对路径名称的话原样返回。

	\~japanese
	ファイルの絶対経路名を求めます。既に絶対経路名であればそのままリターンします。
	\~
	*/
	String GetFullPath(const PNTCHAR* fileName);

	/**
	\~korean
	디렉토리를 생성하고 풀패스를 구한다.

	\~english
	Generate a directory and get the full path.

	\~chinese
	生成directory，求fullpath。

	\~japanese
	ディレクトリーを生成してフルパスを求めます。
	\~
	*/
	bool CreateFullDirectory(const PNTCHAR* lpPathName, String& outCreatedDirectory);
#endif
	/**
	\~korean
	현재 디렉토리를 구한다.

	\~english
	Get the current directory.

	\~chinese
	求现在directory。

	\~japanese
	現在ディレクトリーを求めます。
	\~
	*/
	String GetCurrentDirectory();
#ifdef _WIN32
	/**
	\~korean
	타이머 유저 콜백 함수
	- 타이머 콜백 또는 등록된 대기 콜백을 위한 시작 주소를 제공하는 어플리케이션 정의 함수 이다.
	이 주소는 CreateTimerQueueTimer와 RegisterWaitForSingleObject 함수로 지정 한다.

	WaitOrTimerCallbackProc 타입은 콜백 함수로의 포인터를 정의한다. WaitOrTimerCallbackProc은 어플리케이션 정의 함수 이름을 위한 플레이스홀더 이다.

	\param [in] lpParameter 스레드 데이터는 CreateTimerQueueTimer 및 RegisterWaitForSingleObject 함수의 파라미터를 사용하는 함수로 보내진다.
	\param TimerOrWaitFired 만약 이 파라미터가 TRUE 라면 대기시같이 끝난다. 만약 이 파라미터가 FALSE 라면 대기 이벤트는 신호를 발생시킨다. (이 파라미터는 타이머 콜백을 위해 항상 TRUE 이다.)

	\~english
	Timer user callback function
	- An application-defined function that serves as the starting address for a timer callback or a registered wait callback.
	Specify this address when calling the CreateTimerQueueTimer, RegisterWaitForSingleObject function.

	The WaitOrTimerCallbackProc type defines a pointer to this callback function. WaitOrTimerCallback is a placeholder for the application-defined function name.

	\param [in] lpParameter The thread data passed to the function using a parameter of the CreateTimerQueueTimer or RegisterWaitForSingleObject function.
	\param TimerOrWaitFired If this parameter is TRUE, the wait timed out. If this parameter is FALSE, the wait event has been signaled. (This parameter is always TRUE for timer callbacks.)

	\~chinese
	Timer 用户回调函数
	- 为了timer回拨或者提供已登录的等待回拨的开始地址的应用定义函数。
	此地址是由CreateTimerQueueTimer和RegisterWaitForSingleObject函数指定。

	WaitOrTimerCallbackProc 类型定义以回拨函数的指针。 WaitOrTimerCallbackProc 是为了应用定义函数名称的占位符。

	\param [in] lpParameter 线程数据发往使用CreateTimerQueueTimer及RegisterWaitForSingleObject函数参数的函数。
	\param TimerOrWaitFired 如果此参数是TRUE的话等待时一起结束。如果此参数是FALSE的话，等待event会发生信号。（此参数为了timer回拨一直是TRUE。）

	\~japanese
	タイマーユーザーコールバック関数
	- タイマーコールバックまたは登録された待機コールバックのための開始アドレスを提供するアプリケーション定義関数です。
	このアドレスはCreateTimerQueueTimerとRegisterWaitForSingleObjectの関数で指定します。
	WaitOrTimerCallbackProcタイプはコールバック関数へのポインターを定義します。WaitOrTimerCallbackProcはアプリケーション定義関数名のためのプレイスホルダーです。
	\param [in] lpParameter スレッドデータは、CreateTimerQueueTimer及びRegisterWaitForSingleObject関数のパラメーターを使う関数に送られます。
	\param TimerOrWaitFired もし、このパラメーターがTRUEであれば待機時に一緒に終わります。もし、このパラメーターがFALSEであれば待機イベントは信号を発生します。(このパラメーターはタイマーコールバックのためにいつもTRUEです。)

	\~
	*/
	typedef VOID(NTAPI * WaitOrTimerCallbackProc) (PVOID lpParameter, BOOLEAN TimerOrWaitFired);

	typedef
		HANDLE
		(WINAPI* CreateTimerQueueProc) (
			VOID
			);

	typedef
		BOOL
		(WINAPI* CreateTimerQueueTimerProc) (
			/*__deref_out*/ PHANDLE phNewTimer,
			/*__in_opt   */ HANDLE TimerQueue,
			/*__in       */ WaitOrTimerCallbackProc Callback,
			/*__in_opt   */ PVOID Parameter,
			/*__in       */ uint32_t DueTime,
			/*__in       */ uint32_t Period,
			/*__in       */ uint32_t Flags
			);


	typedef
		BOOL
		(WINAPI* DeleteTimerQueueTimerProc) (
			/*__in_opt*/ HANDLE TimerQueue,
			/*__in    */ HANDLE Timer,
			/*__in_opt*/ HANDLE CompletionEvent
			);

	typedef
		BOOL
		(WINAPI* DeleteTimerQueueExProc) (
			/*__in    */ HANDLE TimerQueue,
			/*__in_opt*/ HANDLE CompletionEvent
			);

	typedef
		BOOL
		(WINAPI* InitializeCriticalSectionExProc) (
			/*__out*/ LPCRITICAL_SECTION lpCriticalSection,
			/*__in*/  uint32_t dwSpinCount,
			/*__in  */uint32_t Flags
			);

#if defined(_WIN32)
	bool GetWindowsVersion(DWORD& major, DWORD& minor, DWORD& servicePack);
#endif

	/**
	\~korean
	Windows NT 전용 API를 동적으로 얻어오는 클래스
	- Windows 98에서는 비록 호출은 못하지만 아예 해당 API가 없어서 exe 실행 조차도 못하는 문제는 해결할 필요가
	있을때 이 클래스가 유용하다.
	- 평소에는 쓸 필요가 없다. 다만, Windows 98에서도 작동해야 하는 모듈을 만든다면 유용하다.

	\~english
	Class that gets Windows NT custom API as dynamic
	- Though it cannot be called by Windows 98 but this can be very useful sovling troubles that even exe cannot be run.
	- Does not need to be used normally. But useful when creating a module that is to run on Windows 98.

	\~chinese
	把Windows NT专用API以动态获取的类。
	- 虽然在Windows 98不能呼叫，但需要解决因没有对应的API而根本无法实行exe的问题时，此类会有效果。
	- 平时没有必要使用。但是制造在Windows 98也能运转的模块时会有用。

	\~japanese
	Windows NT専用のAPIをダイナミックで得て来るクラス
	- Windows 98では、たとえ呼び出しはできませんが、当初該当APIがなくてexe実行さえできない問題は解決する必要がある時にこのクラスが有用です。
	- 通常には使う必要がありません。但し、Windows 98でも作動しなければならないモジュールを作るなら有用です。

	\~
	*/
	class CKernel32Api/*:protected CSingleton<CKernel32Api> 싱글톤 즐. 어차피 파괴자 콜이 없으므로. 게다가 CSingleton은 미래 업데이트에 따라 느려질 수 있으므로. */
	{
		//modify by rekfkno1 - 이렇게 멤버변수로 두는것은 생성자 호출이 늦는경우가 있더라.
		//그래서 static 함수내의 static 로컬변수로 싱글턴을 하는것이 좋을 듯하다.
		//static CKernel32Api g_instance;
	public:
		CKernel32Api();

		static CKernel32Api& Instance();

		// 함수 포인터 멤버 변수들
		HeapSetInformationProc HeapSetInformation;
		GetQueuedCompletionStatusExProc GetQueuedCompletionStatusEx;  // windows 2008 server 이후부터 쓰는 기능

		InitializeCriticalSectionExProc InitializeCriticalSectionEx;
		GetLogicalProcessorInformation GetLogicalProcessInformation;

		HINSTANCE m_dllHandle;               // Handle to DLL
	};

	class CLocale :public DllSingleton<CLocale>
	{
	public:
		int m_languageID;

		CLocale();

		static CLocale& GetUnsafeRef();
	};

	DECLARE_DLL_SINGLETON(PROUD_API, CLocale);

	class CSystemInfo :public DllSingleton<CSystemInfo>
	{
	public:
		SYSTEM_INFO m_info;

		CSystemInfo();
		inline uint32_t GetMemoryPageSize() { return m_info.dwPageSize; }

		static CSystemInfo& GetUnsafeRef();
	};

	DECLARE_DLL_SINGLETON(PROUD_API, CSystemInfo);

	/**
	\~korean
	현재 프로세스에서 사용되고 있는 총 스레드의 수를 얻어온다.

	\~english
	Gain the number of whole threads which is used in process now.

	\~chinese
	获得现在使用中的总线程数。

	\~japanese
	現在プロセスで使われている総スレッドの数を得ます。
	\~
	*/
	PROUD_API int GetTotalThreadCount();

	/**
	\~korean
	현재 스레드의 상태를 얻어온다.

	\~english
	Gain the condition of that thead currently.

	\~chinese
	Gain the condition of that thead currently.

	\~japanese
	現在スレッドの状態を得ます。
	\~
	*/
	DWORD GetThreadStatus(HANDLE const handle);

#endif //#ifdef _WIN32

	/**  @} */

#ifdef _MSC_VER
#if !defined(_NO_NTTNTRACE) && (defined(_DEBUG)||defined(IW_DEBUG))
#define NTTNTRACE Proud::TraceA
#else
#define NTTNTRACE __noop // 비 VC++에서는 이것을 사용 못하므로 아래 것이 사용됨
#endif
#else
#if !defined(_NO_NTTNTRACE) && (defined(_DEBUG)||defined(IW_DEBUG))
#define NTTNTRACE(...) Proud::TraceA(__VA_ARGS__)
#else
#define NTTNTRACE(...) //VC++에서는 C4390 경고가 나옴. 그래서 위의 것이 사용됨.
#endif
#endif

	// _MAX_PATH는 260밖에 안된다. 요즘 세대에서 이것은 너무 작다. 이것을 쓰자.
	const int LongMaxPath = 4000;

	// non-win32에 없는 것들을 유사 형태로 타 플랫폼을 위해 구현
	// 일부러 namespace Proud 안에 두었다. 없는 것을 global namespace에 구현하는 경우 타사가 구현한 것과 충돌할 수 있으니.
	// 게다가 이 클래스의 멤버로 둔 이유: 안그랬더니 ATL을 추가하는 경우 IsDebuggerPresent 함수를 쓰는 곳에서 컴파일 에러가 발생해서.
	class CFakeWin32
	{
	public:
		PROUD_API static unsigned int GetTickCount();
		static bool IsDebuggerPresent();
		static void OutputDebugStringA(const char *text);
		PROUD_API static void OutputDebugStringW(const wchar_t *text);
	};

#ifdef _PNUNICODE
#define OutputDebugStringT OutputDebugStringW
#else
#define OutputDebugStringT OutputDebugStringA
#endif


#ifdef _WIN32
	// error code를 사람이 읽을 수 있는 문장으로 만듭니다. 
	PROUD_API String GetMessageTextFromWin32Error(uint32_t errorCode);
#endif // _WIN32

#ifndef _WIN32
	PROUD_API bool fd_is_valid(int fd);
#endif

}

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif

