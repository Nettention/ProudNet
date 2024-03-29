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

#include "sysutil.h"
#include "Singleton.h"
#include "MiniDumpAction.h"

#if defined(_WIN32)
#include <DbgHelp.h>
#pragma comment(lib,"dbghelp.lib")
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
	/** \addtogroup minidump_group
	*  @{
	*/

#if defined(_WIN32)

	/**
	\~korean
	미니 덤프를 사용하기 위해 제공해야 하는 파라메터

	\~english
	It is a parameter that needs to be provided for using mini dump.

	\~chinese
	为了使用Mini dump而需提供的参数。

	\~japanese
	ミニダンプを使用するために提供するパラメータ
	*/
	class CMiniDumpParameter
	{
	public:
		/**
		\~korean
		미니 덤프가 생성할 파일 이름의 경로를 채워넣는다.

		\~english
		Please input the directory of file name that mini dump is going to create.

		\~chinese
		填充Mini dump创建的文件名的路径。

		\~japanese
		ミニダンプが生成するファイル名の経路を入れる。
		*/
		Proud::String m_dumpFileName;

		/**
		\~korean
		미니 덤프 타입을 지정한다.

		\~english
		Sets the type of mini dump

		\~chinese
		指定微型转储类型。

		\~japanese
		ミニダンプタイプを指定する。
		*/
		MINIDUMP_TYPE m_miniDumpType;

		PROUD_API CMiniDumpParameter();
	};

#define SmallMiniDumpType MiniDumpNormal // MiniDumpWithDataSegs가 가장 쓸만하다곤 하지만 6MB나 되니까 유저들이 잘 안보낸다.

	/**
	\~korean
	미니 덤프 모듈

	특징
	- 이 객체는 서버 크래쉬 발생시 .DMP 파일을 만든다.
	- .DMP 파일은 visual studio.net에서 직접 열어서 오류 위치를 추적하는데 도움이 된다.
	- 동시 다발적으로 여러 스레드에서 unhandled exception이 발생해도 최초 1회에 대한 덤프 파일만 생성된다.
	나머지 스레드는 덤프가 끝날때까지 대기한다. 그래야 덤프중에 프로세스가 딴 문제가 생기지 않는다.

	일반적 용도
	- 이 객체는 싱글톤이다. 일단 이 객체의 Delegate를 지정함으로 JIT creation이 된다. Delegate 지정은
	Init()로 한다. 이후부터는 프로그램 오류시 덤프 프로그램이 작동한다.
	- 수동 미니 덤프를 생성은 Dump()로 한다.

	\~english
	Mini dump module

	Characterictics
	- This object creates .DMP files when server crash occurs.
	- .DMP can be opened by visual studio.net directly and helps tracking where error occurred.
	- The first cump file will be created even if there are multiple unhandled exceptionals occured from many thread simultaneously.
	Other thread will standby until dump ends. There will be no other problems while dumping the process by doing so.

	General usage
	- This object is Singleton. This will become JIT creation by designating Delegate of this object. Delegate is designated by using Init(). From this point and then, dump program will automatically run when error occurs.
	- Dump() is used to create manual mini dump.

	\~chinese
	微型转储模块。

	特征
	- 此对象在发生服务器crash的时候创建.DMP。
	- .DMP文件在visual  studio.net%直接打开并追踪位置会有帮助。
	- 即使同时在多发性的多个线程上发生unhandled exception，只生成对最初一次的转储文件。
	剩下的线程等到转储结束。这样才能在转储中程序不会发生其他问题。

	一般用途
	- 此对象是singleton。先指定此对象的Delegate之后会JIT creation。用Init()进行Delegate的指定。之后如果程序发生错误，转储程序就会运转。
	- 手动生成微型转储是用Dump()。

	\~japanese
	ミニダンプモジュール

	特徴
	-このオブジェクトはさーばクラッシュ発生時、.DMP ファイルを作ります。
	-.DMPファイルはvisual studio.netで直接開いてエラーの位置を追跡するのに役に立ちます。
	-同時多発的に多様なスレッドでunhandled exceptionが発生しても最初１回に対するダンプファイルだけ生成されます。
	残りのスレッドはダンプが終わるまで待機します。そうしないとダンプ中、プロセスに他の問題が発生します。

	一般的な用途
	-このオブジェクトはシングルトンです。一旦このオブジェクトのDelegateを指定することでJIT creationになります。 Delegate指定は
	Init()にします。それからはプログラムのエラーの際ダンププログラムが作動します。
	-手動ミニダンプの生成は Dump()にします。
	*/
	class CMiniDumper : public DllSingleton<CMiniDumper>
	{
		/**
		\~korean
		여러 스레드에서 덤프를 쌓으면 꼬인다. 따라서 이걸로 직렬화한다.

		\~english
		Will cras if many threads stack dump. So must be serialized by using this.

		\~chinese
		在几个线程堆积转储的话不好。所以用这个串联化。

		\~japanese
		多様なスレッドでダンプファイルを残すとこじれます。ですのでこれで直列化します。
		*/
		CriticalSection m_filterWorking;

		/**
		\~korean
		TopLevelFilter 실 작동을 1회로 제한하는 목적

		\~english
		To limit the real execution of TopLevelFilter by once.

		\~chinese
		TopLevelFilter 限制实运转为1回的目的。

		\~japanese
		TopLevelFilter実作動を一回と制限する目的
		*/
		volatile LONG m_hitCount;

		 void SetUnhandledExceptionHandler();

		 uint32_t CreateProcessAndWaitForExit(const PNTCHAR *args, MINIDUMP_TYPE miniDumpType, _EXCEPTION_POINTERS *pExceptionInfo = NULL);

		static LONG WINAPI TopLevelFilter(_EXCEPTION_POINTERS *pExceptionInfo);
		LONG TopLevelFilter_(_EXCEPTION_POINTERS *pExceptionInfo);

#if defined(_WIN64)
		bool DumpWithFlags(const PNTCHAR *dumpFileName, const uint32_t processID, const MINIDUMP_TYPE minidumpFlags, const uint32_t threadID, const uint64_t exceptionInfoAddr);
#else
		bool DumpWithFlags(const PNTCHAR *dumpFileName, const uint32_t processID, const MINIDUMP_TYPE minidumpFlags, const uint32_t threadID, const uint32_t exceptionInfoAddr);
#endif

	private:
		/**
		\~korean
		미니 덤프를 사용하기 위한 파라메터

		\~english 
		The parameter to use the mini dump. 

		\~chinese 
		用于mini dumper的参数
		\~

		\~japanese 
		ミニダンプを使うためのパラメーター
		\~
		*/
		CMiniDumpParameter m_parameter;

	public:
		PROUD_API CMiniDumper();
		PROUD_API ~CMiniDumper();

		/**
		\~korean
		초기화 함수입니다.
		\param cmdLine 앱의 Command-Line argument의 인자를 넣어주시면 됩니다.

		ProudNet은 앱에서 오류 발생 시, 앱 자신을 새로운 프로세스로 실행합니다.
		이 때, Startup 함수에서 인식할 수 있는 특수 구문을 Commad Line Argument에 추가하여 새로운 프로세스를 실행하게 됩니다.

		이 함수는 Win32 응용 프로그램 및 MFC 응용프로그램에서 호출하시면 됩니다.
		\code
		void main(int argc, char* argv[])
		{
		...

		CMiniDumpParameter parameter;
		switch(CMiniDumper::GetSharedPtr()->Startup(parameter))
		{
		case MiniDumpAction_AlarmCrash:
		// 오류 발생으로 새로운 프로세스에서 덤프 파일을 생성한 후, 이 값이 return이 됩니다.
		// 생성된 덤프 파일을 메일로 보내거나 에러 창을 보이는 등 유저가 덤프 파일 생성 후, 처리해야할 작업을 처리해주시면 됩니다.
		...
		break;

		case MiniDumpAction_DoNothing:
		// 유저 호출로 새로운 프로세스에서 덤프 파일을 생성한 후, 이 값이 반환됩니다.
		// 이 경우에는 아무것도 하지 말아야합니다.
		break;

		default: // MiniDumpAction_None
		// 일반적으로 앱 실행 시, 이 값이 반환됩니다.
		// 여기서는 일반적으로 처리해야할 일을 처리해주시면 됩니다.
		...
		break;
		}
		}
		\endcode

		\~english
		It is an initialized function.
		\param cmdLine Please insert Command-Line argument factor of application.

		When an error occurs in application, ProudNet executes the application with new process.
		\code
		void main(int argc, char* argv[])
		{
		...

		CMiniDumpParameter parameter;
		switch(CMiniDumper::GetSharedPtr()->Startup(parameter))
		{
		case MiniDumpAction_AlarmCrash:
		// After dump file creation at a new process due to error occurrence, this value will return.
		// Please proceed with works (such as sending created dump file by email & error window management) that need to be done right after dump file creation.
		...
		break;

		case MiniDumpAction_DoNothing:
		// After dump file creation at a new process by user calling, this value will return.
		// In this case, you should not do anything.
		break;

		default: // MiniDumpAction_None
		// Generally, this value returns when executing the application.
		// At this time, you should proceed with the relevant works.
		...
		break;
		}
		}
		\endcode

		\~chinese
		初始化函数。
		\param cmdLine 输入App的Command-Line argument 参数即可。

		在App当中如果发生错误，ProudNet会用新的Process运行App。
		此时将在Command-Line Argument中添加Startup函数可以认证的特殊语句后运行新的Process。.
		\code
		void main(int argc, char* argv[])
		{
		...

		CMiniDumpParameter parameter;
		switch(CMiniDumper::GetSharedPtr()->Startup(parameter))
		{
		case MiniDumpAction_AlarmCrash:
		// 因错误发生在新的Process中生成Dump文件后此值将被return。
		// 处理如用邮件发送已生成的Dump文件、显示错误窗口等用户生成Dump文件后所要处理的事件即可。
		...
		break;

		case MiniDumpAction_DoNothing:
		// 用用户呼出在新的Process中生成Dump文件后将返还此值。
		// 此时请不要进行任何操作。
		break;

		default: // MiniDumpAction_None
		// 通常在运行App时将返还此值。
		// 在这里处理一般需要进行的操作即可。
		...
		break;
		}
		}
		\endcode

		\~japanese
		初期化の関数です。
		\param cmdLine App(Application)のCommand-Line argumentのパラメーターを入れてください。

		ProudNetはAppでエラーが発生する際,App自身を新しいプロセス(Process)で実行します。
		この際、Startup 関数で認識できる特殊構文を Commad Line Argumentに追加して新しいプロセスを実行します。
		\code
		void main(int argc, char* argv[])
		{
		...

		CMiniDumpParameter parameter;
		switch(CMiniDumper::GetSharedPtr()->Startup(parameter))
		{
		case MiniDumpAction_AlarmCrash:
		// エラーの発生で新しいプロセスでダンプファイルを生成後、この値が returnされます。
		// 生成されたダンプファイルをメールで送ったりエラーの画面を見せるなどユーザがダンプファイルを生成後、処理しなければならない作業を処理してください。
		...
		break;

		case MiniDumpAction_DoNothing:
		// ユーザの呼び出しで新しいプロセスにダンプファイルを生成後、この値が返還されます。
		// この場合には何もしないでください。
		break;

		default: // MiniDumpAction_None
		// 一般的にはApp実行時,この値が返還されます。
		// ここでは一般的に処理しなければならないことを処理してください。
		...
		break;
		}
		}
		\endcode
		*/
		PROUD_API static MiniDumpAction Startup(const CMiniDumpParameter &parameter);
	private:
		MiniDumpAction Startup_(const CMiniDumpParameter &parameter);
	public:

		/***
		\~korean
		유저 호출에 의해 미니 덤프 파일을 생성한다. 단, 메모리 전체 덤프를 하므로 용량이 큰 파일이 생성된다.

		\~english
		Create mini dump file by user calling, however, the capacity of file becomes larger because the whole memory is dumped.

		\~chinese
		被用户呼叫生成微型转储文件。但是，因为进行内存全部转储，会生成容量大的文件。

		\~japanese
		ユーザの呼び出しによりミニダンプファイルを生成します。ただし、メモリー全体をダンプするので容量の大きいファイルが生成されます。
		*/
		PROUD_API  void ManualFullDump();

		/**
		\~korean
		유저 호출에 의해 미니 덤프 파일을 생성한다.

		\~english
		Create mini dump file by user calling.

		\~chinese
		被用户呼叫生成微型转储文件。

		\~japanese
		ユーザの呼び出しによりミニダンプファイルを生成します。
		*/
		 PROUD_API  void ManualMiniDump();

		/**
		\~korean
		이 함수를 호출한 시점에서의 프로세스 상태를 덤프 파일로 남긴다.
		\param fileName 덤프 파일 이름. 확장자는 dmp로 지정할 것.

		\~english
		Process status of calling this function as dump file
		\param fileName Name of dump file. Its extension must be .dmp.

		\~chinese
		从呼叫此函数的时点的程序状态留为转储文件。
		\param filename 转储文件名。扩展者指定为dmp。

		\~japanese
		この関数を呼び出した時点でのプロセス状態をダンプファイルで残します。
		\param fileNameダンプファイルの名前。拡張子はdmpで指定すること。
		*/
		PROUD_API static void WriteDumpFromHere(const PNTCHAR* fileName, bool fullDump = false);
		//		 static void WriteDumpAtExceptionThrower(const PNTCHAR* fileName, bool fullDump = false);
	};

	/**
	\~korean
	CExceptionLogger 에서 필요로하는 delegate 객체이다. 개발자는 이 객체를 오버라이드한 후 CExceptionLogger 가
	존재하는 동안 이 객체를 유지해야 한다.

	\~english
	Delegate object that is needed by CExceptionLogger. Developer must sustain this object while CExceptionLogger exists after overriding this object.

	\~chinese
	在 CExceptionLogger%需要的delegate对象。开发者在把此对象覆盖后， CExceptionLogger%存在的期间要维持此对象。

	\~japanese
	CExceptionLoggerで必要とする delegateオブジェクトです。開発者はこのオブジェクトをオーバーライドした後、 CExceptionLoggerが
	存在する間、このオブジェクトを維持しなればなりません。

	*/
	class IExceptionLoggerDelegate
	{
	public:
		/**
		\~korean
		Dump 파일들이 저장될 폴더를 리턴한다. 상대 경로 또는 절대 경로를 지정할 수 있다.
		개발자는 이 메서드를 오버라이드해야 한다.

		\~english
		Returns the folder where Dump files are to be stored. Either relative or absolute path can be designated.
		Developer must override this method.

		\~chinese
		返回保存dump文件的文件夹。可以指定相对路径或者绝对路径。开发者要把此方法覆盖。

		\~japanese
		Dumpファイルたちが保存されるフォルダーをリターンします。相対経路または絶対経路を指定できます。
		開発者はこのメソッドをオーバーライドしなければなりません。
		*/
		virtual String GetDumpDirectory() = 0;
	};

	/**
	\~korean
	크래쉬 등 프로그램에서 예외가 발생할 경우 예외가 발생한 지점을 DMP 파일로서 로그를 남기고
	계속해서 프로그램을 실행시키는 역할을 하는 객체
	- 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#exception_logger" >중단없는 오류덤프시스템(Exception Logger)</a> 를 참고할 것
	- Windows XP 또는 2003 Server 이상을 필요로 한다.
	- Singleton 객체로만 사용할 수 있다. 자세한 것은 CSingleton 참고.

	\~english
	The object that records the point where exception occurred as DMP file when crash occurs then has a role to keep running the program
	- Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#exception_logger" >Pauseless error dump system (Exception Logger)</a>.
	- Requires Windows XP or 2003 Server or later version
	- Can be only used as Singleton object. Please refer CSingleton.

	\~chinese
	在crash等程序中发生例外的时候，把发生例外的地点用DMP文件留log，起着继续执行程序的作用的对象。
	- 详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#exception_logger" >不中断的错误log系统(Exception Logger)</a>%。
	- 需要Windows XP或者2003 Server以上。
	- 只能用于Singleton对象。详细请参考 CSingleton%。

	\~japanese
	クラッシュなどプログラムで例外が発生する場合、例外が発生した地点をDMPファイルでログを残し、
	続けてプログラムを実行させる役割をするオブジェクト
	- 詳しくは<a target="_blank" href="http://guide.nettention.com/cpp_jp#exception_logger" >无间断错误转储系统(Exception Logger)</a>を参考してください。
	- Windows XPまたは2003 Server以上を必要とする。
	- Singletonオブジェクトだけで使用できます。詳しくは CSingletonを参考してください。
	*/
	class CExceptionLogger : public DllSingleton<CExceptionLogger>
	{
		CriticalSection m_cs;

		// 덤프 파일이 저장될 디렉터리
		String m_directory;
		// 덤프 파일의 이름
		String m_dumpName;
		// 덤프 파일에 새겨질 일련 번호
		int m_dumpSerial;
		// 가장 마지막에 덤프를 남긴 시간
		uint32_t m_lastLoggedTick;

		IExceptionLoggerDelegate* m_dg;

		static LONG CALLBACK ExceptionLoggerProc(PEXCEPTION_POINTERS pExceptionInfo);
	private:
		LONG ExceptionLoggerProc_(PEXCEPTION_POINTERS pExceptionInfo);

	public:
		/**
		\~korean
		본 로그 시스템을 준비한다.
		\param dg 이 시스템이 필요로 하는 추가적인 기능을 제공할 delegate 객체의 포인터

		\~english
		Prepares this log system
		\param dg Pointer of the delegate object that will provide additional functions required by this system

		\~chinese
		准备本log系统。
		\param dg 提供此系统需要的附加技能的delegate对象的指针。

		\~japanese
		本ログシステムを準備します。
		\param dg　このシステムが必要とする追加的な機能を提供する delegateオブジェクトのポインター
		*/
		void Init(IExceptionLoggerDelegate* dg, Proud::String dumpName = _PNT("dump"));


		inline int GetDumpSerial() { return m_dumpSerial; }
	};

	DECLARE_DLL_SINGLETON(PROUD_API, CExceptionLogger);
	DECLARE_DLL_SINGLETON(PROUD_API, CMiniDumper);

#endif

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
