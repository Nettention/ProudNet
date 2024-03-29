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

#include <winsvc.h>
#include "NTServiceStartParameter.h"
#include "Singleton.h"
#include "FastArray.h"

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
	/** \addtogroup ntservice_group
	*  @{
	*/

	/**
	\~korean
	CNTService 가 필요로 하는 event sink 객체

	이 객체는 CNTService 가 요구하는 다음 기능을 제공하는 역할을 한다. 유저는 이 클래스를 구현해야 한다.
	- 서버 메인 루틴
	- 로그 남기기 함수 등 추가 기능

	\~english
	Event sink that CNTService needs

	This object provides the following functions requested by CNTService. User must realize this class.
	- Server main routine
	- Addtional functions such as function that leaves log

	\~chinese
	CNTService 需要的event sink对象。

	此对象起着提供 CNTService%需要的下一个功能的作用。用户要体现此类。
	- 服务器主routine。
	- 留log函数等添加的功能。

	\~japanese
	CNTService が必要とするevent sinkオブジェクト

	このオブジェクトはCNTServiceが要求する次の機能を提供する役割をします。ユーザーはこのクラスを具現する必要があります。
	- サーバーメインルーチン
	- ログ残し関数などの追加機能
	*/
	class INTServiceEvent
	{
	public:
		virtual ~INTServiceEvent() {}

		/**
		\~korean
		이벤트 로그를 남긴다.
		\param type EVENTLOG_ERROR_TYPE,EVENTLOG_WARNING_TYPE,EVENTLOG_INFORMATION_TYPE 중 하나
		\param text 남길 문자열

		\~english
		Leaves event log
		\param type Either EVENTLOG_ERROR_TYPE or EVENTLOG_WARNING_TYPE or EVENTLOG_INFORMATION_TYPE
		\param text Text string to be left

		\~chinese
		留有事件log。
		\param type EVENTLOG_ERROR_TYPE,EVENTLOG_WARNING_TYPE,EVENTLOG_INFORMATION_TYPE 中之一。
		\param text 要留的字符串。

		\~japanese
		イベントログを残します。
		\param type EVENTLOG_ERROR_TYPE,EVENTLOG_WARNING_TYPE,EVENTLOG_INFORMATION_TYPE 中一つ
		\param text 残す文字列
		*/
		virtual void Log(int type, const PNTCHAR* text) = 0;

		/**
		\~korean
		서비스 모듈 메인 함수
		이 함수는 서비스 모듈에 의해 호출되는 메인 함수이다. 이 함수에 서버 프로그램 메인 루틴이 호출되어야 한다.
		이 함수가 리턴할 때 서비스가 종료한다.

		요구 사항
		- 이 함수 내부에서 키 입력에 의한 종료 또는 Win32 메시지 프로세스 루틴이 반드시 들어가야 한다.
		왜냐하면 SCM에 의해 서비스 종료 명령이 주어질 경우 WM_QUIT을 수신하기 때문이다. 아래의 예제 코드를 참고하라.

		참고
		- argc,argv는 CNTService::Instance() 로부터 얻을 수 있다. 그밖에 CNTService::Instance() 로부터 여러가지 옵션을
		얻을 수 있다.

		예제 소스
		\code
			while(1)
			{
				Sleep(100);
				if(_kbhit())
				{
					switch(getch())
					{
					case VK_ESCAPE:
						break;
					}
				}

				MSG msg;
				// 최대 일정 짧은 시간동안, 콘솔 입력, 윈도 메시지 수신, 메인 스레드 종료 중 하나를 기다린다.
				MsgWaitForMultipleObjects(0,0,TRUE,100,QS_ALLEVENTS);
				if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				{
					if(!GetMessage(&msg,NULL,NULL,NULL))
						break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		\endcode

		\~english
		Service module main function
		    This function is the main function that is called by service module. Server program main routine must be called to this function.
		    Service is terminated when this function is returned.

		Requirements
		- Either a termination by keyboard input or Win32 message process routine must be entered to this function.
		  This is since when service termination order is given by SCM then it recieves WM_QUIT. Please refer the example code shown below.

		Reference
		- argc,argv can be acquired from CNTService::Instance(). And other various options can be acquired from CNTService::Instance().

		Example source
		\code
			while(1)
			{
				Sleep(100);
				if(_kbhit())
				{
					switch(getch())
					{
					case VK_ESCAPE:
						break;
					}
				}

				MSG msg;
				// Waits for one of console input, Windows message reception, mina thread termination for max shcedule and short time.
				MsgWaitForMultipleObjects(0,0,TRUE,100,QS_ALLEVENTS);
				if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				{
					if(!GetMessage(&msg,NULL,NULL,NULL))
						break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		\endcode

		\~chinese
		服务模块主函数。
		此函数是被服务模块呼叫的主函数。要往此函数呼叫服务器程序主routine。
		此函数返回的时候服务终止。

		要求事项
		- 在此函数内部必须要进入用key输入的终止或者Win32信息程序routine。
		因为从SCM收到服务终止命令的时候会收到WM_QUIT。请参考以下例代码。

		参考
		- argc,argv 能从 CNTService::Instance()%获得。此外能从 CNTService::Instance()%获得各种选项。

		例source
		\code
			while(1)
			{
				Sleep(100);
				if(_kbhit())
				{
					switch(getch())
					{
					case VK_ESCAPE:
						break;
					}
				}

				MSG msg;
				// 最大一定短时间内，等待console输入、window信息的接收、主要线程终止中的一个。
				MsgWaitForMultipleObjects(0,0,TRUE,100,QS_ALLEVENTS);
				if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				{
					if(!GetMessage(&msg,NULL,NULL,NULL))
						break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		\endcode

		\~japanese
		-	サービスモジュールメイン関数
		この関数はサービスモジュールにより呼び出されるメイン関数です。この関数にサーバープログラムメインルーチンが呼び出される必要があります。
		この関数がリターンする時、サービスが終了します。

		要求事項
		- この関数の内部にキー入力による終了、またはWin32メッセージプロセスルーチンが必ず入れてください。
		何故ならSCMによりサービス終了命令が与えられる場合WM_QUITを受信するからです。以下のコード例をご参考ください。

		参考
		- argc,argvはCNTService::Instance()から得られます。その他にCNTService::Instance() から様々なオプションを得られます。

		ソース例
		\code
			while(1)
			{
				Sleep(100);
				if(_kbhit())
				{
					switch(getch())
					{
					case VK_ESCAPE:
						break;
					}
				}

				MSG msg;
				// 最大日程短い時間の間、コンソール入力、ウィンドーメッセージ受信、メインスレッド終了中一つを待ちます。
				MsgWaitForMultipleObjects(0,0,TRUE,100,QS_ALLEVENTS);
				if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				{
					if(!GetMessage(&msg,NULL,NULL,NULL))
						break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		\endcode
		*/
		virtual void Run() = 0;

		virtual void Stop() = 0;

		virtual void Pause() = 0;

		virtual void Continue() = 0;
	};

	class CNTServiceInternal;

	/**
	\~korean
	Windows 서비스 (daemon) 모듈 클래스
	- CNTService 는 singleton 이다. 그냥 Instance()로 접근하면 된다.

	일반적 용법
	- ::WinMain 이나 ::main()에서 WinMain()을 호출한다. 파라메터는 ::WinMain 이나 ::main()에서 받은 것을 넘기면 된다.
	이때 미리 준비한 INTServiceEvent 객체를 attach해야 한다.
	- IsInstalled(), IsStartedBySCM() 등을 통해 서비스 실행에 관련된 여러가지 설정 상태를 얻을 수 있다.

	프로그램 실행 파라메터
	- -AR : SCM에 등록 시 이 옵션을 추가하면 서비스가 크래쉬 시 몇분 후에 자동 시작하게 한다. 편리한 기능이지만
	디버깅중이라면 오히려 짜증을 유발함에 주의.
	- -install : SCM에 등록한다.
	- -uninstall : SCM에서 등록 해제한다.
	- -console : 이 옵션은 서비스를 일반 EXE 형태로 실행하게 한다. 디버깅을 해야 하는 상황 등
	SCM에서의 실행을 원하지 않는 경우 이 옵션을 써야 한다.

	\~english
	 Windows service (daemon) module class
	- CNTService is Singleton. It can be accessed by Instance().

	General usage
	- Calls WinMain() either from ::WinMain or ::main(). Can pass parameters from ::WinMain or ::main().
	  At this time, the previously prepared INTServiceEvent obkect must be attached.
	- Various setting status related to service execution can be acquired through IsInstalled(), IsStartedBySCM() and others.

	Program execution parameter
	- -AR : If this option is added when registered to SCM, servive will auto-start after facing a crash. A handy function but can cause major irritation while debugging.
	- -install : Registers at SCM
	- -uninstall : Withdraws from SCM
	- -console : This option allows service to run as a general EXE form. When not wanted running it at SCM, you must use this option.

	\~chinese
	Windows服务（daemon）模块类。
	- CNTService%是singleton。就用Instance()连接即可。

	一般使用方法
	- 在::WinMain 或者 ::main()呼叫WinMain()。给在::WinMain 或者 ::main()接收的参数即可。
	这时要attach事先准备的INTServiceEvent对象。
	- 通过IsInstalled(), IsStartedBySCM()可以获得服务实行相关几个设置状态。

	程序实行参数
	- 往-AR : SCM登陆时添加此选项的话，服务crash时几分钟后会自动开始。虽然是个便利的功能，如果是调试中的话返回诱发恼怒。
	- -install : 登录到SCM。
	- -uninstall : 在SCM解除登录。
	- -console : 此选项把服务实行为一般EXE形式。不想在SCM中实行的话要使用此选项。

	\~japanese
	Windows サービス (daemon) モジュールクラス
	- CNTService は singleton です。Instance()で接近すれば良いです。

	一般的用法
	- ::WinMainや ::main()から WinMain()を呼び出します。パラメータは ::WinMain や ::main()から貰ったのを渡せば良いです。
	この時、既に用意した INTServiceEvent オブジェクトを attachする必要があります。
	- IsInstalled(), IsStartedBySCM() などを通じてサービス実行に関連された様々な設定状態を得られます。

	プログラム実行パラメータ
	- -AR : SCMに登録時、このオプションを追加するとサービスがクラッシューされた何分後に自動スタートします。便利な機能ですが
	デバッギング中ならむしろ腹が立つかも知りません。
	- -install : SCMに登録する。
	- -uninstall : SCMから登録解除する。
	- -console : このオプションはサービスを一般EXE形態で実行するようにします。デバッギングをする必要があるシチュエーションなど
	SCMでの実行を望まない場合、このオプションを使用する必要があります。
	*/
	class CNTService: public DllSingleton<CNTService>
	{
		CNTServiceInternal* m_internal;

		static void WINAPI _ServiceMain(DWORD dwArgc, PNTCHAR** lpszArgv);

		void SetServiceStatus(uint32_t dwState);
		void Run();
		void ServiceMain(int argc, PNTCHAR* argv[]);
		void Handler(uint32_t dwOpcode);
		static void WINAPI _Handler(DWORD dwOpcode);

		void WinMain_ActualService();
		void WinMain_Console();

		BOOL Install(CServiceParameter &param);
		BOOL Uninstall();

		void WinMain_Internal(CNTServiceStartParameter &param);
		void Log(int type, const PNTCHAR* fmt, ...);

	public:

		PROUDSRV_API CNTService(void);
		PROUDSRV_API ~CNTService(void);

		/*
		\~korean
		winmain이나 main에서 보내준 실행 파라메터를 얻는다.

		\~english
		Gets execution parameter either from winmain or main

		\~chinese
		获得从winmain或者main发送的实行参数。
		\~japanese
		winmainや mainから送られた実行パラメータを得ます。
		*/
		PROUDSRV_API void GetArgv_Internal(CFastArray<String>& output);


		/*
		\~korean
		winmain이나 main에서 보내준 실행 파라메터를 얻는다.

		\~english
		Gets execution parameter either from winmain or main

		\~chinese
		获得从winmain或者main发送的实行参数。
		\~japanese
		winmainやmainから送られた実行パラメータを得ます。
		*/
		PROUDSRV_API void GetEnvp_Internal(CFastArray<String>& output);

		/*
		\~korean
		SCM에서 보내준 실행 파라메터를 얻는다.

		\~english
		Gets execution parameter from SCM

		\~chinese
		获得从SCM发送的实行参数。

		\~japanese
		SCMから送られた実行パラメータを得ます。
		*/
		PROUDSRV_API void GetArgvFromSCM_Internal(CFastArray<String>& output);

		/**
		\~korean
		INTServiceEvent 에서 얻은 이름을 리턴한다.

		\~english
		Returns the name acquired from INTServiceEvent

		\~chinese
		返回INTServiceEvent获得的名字。

		\~japanese
		INTServiceEventから得られた名をリターンします。
		*/
		PROUDSRV_API const PNTCHAR* GetName();

		/**
		\~korean
		SCM에 의해 실행된 상태인가?

		\~english
		Is it run by SCM?

		\~chinese
		是被SCM实行的状态吗？

		\~japanese
		SCMにより実行された状態か？
		*/
		PROUDSRV_API bool IsStartedBySCM() const;

		PROUDSRV_API bool FindArg(const PNTCHAR* name);
		PROUDSRV_API String CreateArg();
		PROUDSRV_API BOOL IsInstalled();

		/**
		\~korean
		서비스 모듈 메인 함수. main()이나 ::WinMain()에서 이 메서드를 호출해야 한다. 다음 기능을 담당한다.
		- 실행 파라메터 처리: 서비스 등록/해제, 콘솔 모드 실행 등
		- 이벤트 싱크 객체 가져오기
		- 서비스 메인 함수(INTServiceEvent.Run()) 실행
		\param argc main()에서 받은 값
		\param argv main()에서 받은 값
		\param envp main()에서 받은 값
		\param param CNTService의 설정 값

		\~english
		This method must be called either by service mudule main function main() or ::WinMain(). Manages followings.
		- Execution parameter handling: service register/withdraw, running console more and so on.
		- Getting event sink object
		- Running service main function(INTServiceEvent.Run())
		\param argc value received from main()
		\param argv value received from main()
		\param envp value received from main()
		\param param Setting Value of CNTService

		\~chinese
		服务模块主函数。在main()或者 ::WinMain()呼叫此函数。担任以下功能。
		- 实行参数处理：服务登录/解除、console 模式实行等。
		- 获得event sink对象。
		- 服务主函数(INTServiceEvent.Run())实行。
		\param argc 在main()接收的值。
		\param argv 在main()接收的值。
		\param envp 在main()接收的值。
		\param param CNTService的设定值

		\~japanese
		サービスモジュールメイン関数. main()や ::WinMain()からこのメッソドを呼び出す必要があります。次の機能を担当します。
		- 実行パラメータ処理: サービス登録/解除、コンソールモード実行など
		- イベントシンクオブジェクト持ってくる
		- サービスメイン関数(INTServiceEvent.Run())実行
		\param argc main()から貰った値
		\param argv main()から貰った値
		\param envp main()から貰った値
		\param param CNTServiceの設定値
		*/
		PROUDSRV_API static void WinMain(int argc, char* argv[], char* envp[], CNTServiceStartParameter &param);

		/**
		\~korean
		서비스 모듈 메인 함수. main()이나 ::WinMain()에서 이 메서드를 호출해야 한다. 다음 기능을 담당한다.
		- 실행 파라메터 처리: 서비스 등록/해제, 콘솔 모드 실행 등
		- 이벤트 싱크 객체 가져오기
		- 서비스 메인 함수(INTServiceEvent.Run()) 실행
		\param argc main()에서 받은 값
		\param argv main()에서 받은 값
		\param envp main()에서 받은 값
		\param param CNTService의 설정 값

		\~english
		Service module main function. Must be called either by ::WinMain()main() or ::WinMain(). Manages followings.
		- Execution parameter handling: service register/withdraw, running console more and so on.
		- Getting event sink object
		- Running service main function(INTServiceEvent.Run())
		\param argc value received from main()
		\param argv value received from main()
		\param envp value received from main()
		\param param Setting Value of CNTService

		\~chinese
		服务模块主函数。在main()或者 ::WinMain()呼叫此函数。担任一下功能。
		- 实行参数处理：服务登录/解除、console 模式实行等。
		- 获得event sink对象。
		- 服务主函数(INTServiceEvent.Run())实行。
		\param argc 在main()接收的值。
		\param argv 在main()接收的值。
		\param envp 在main()接收的值。
		\param param CNTService的设定值

		\~japanese
		サービスモジュールメイン関数. main()や ::WinMain()からこのメッソドを呼び出す必要があります。次の機能を担当します。
		- 実行パラメータ処理: サービス登録/解除、コンソールモード実行など
		- イベントシンクオブジェクト持ってくる
		- サービスメイン関数(INTServiceEvent.Run())実行
		\param argc main()から貰った値
		\param argv main()から貰った値
		\param envp main()から貰った値
		\param param CNTServiceの設定値
		*/
		PROUDSRV_API static void WinMain(int argc, wchar_t* argv[], wchar_t* envp[], CNTServiceStartParameter &param);

		PROUDSRV_API void FrequentWarning(const PNTCHAR* text);
		PROUDSRV_API void FrequentWarningWithCallStack(const PNTCHAR* text);


	public:
		/*
		\~korean
		winmain이나 main에서 보내준 실행 파라메터를 얻는다.

		\~english
		Gets execution parameter either from winmain or main

		\~chinese
		获得从winmain或者main发送的实行参数。
		\~japanese
		winmainやmainから送られた実行パラメータを得ます。
		*/
		PROUDSRV_API static void GetArgv(CFastArray<String>& output);

		/*
		\~korean
		winmain이나 main에서 보내준 실행 파라메터를 얻는다.

		\~english
		Gets execution parameter either from winmain or main

		\~chinese
		获得从winmain或者main发送的实行参数。
		\~japanese
		winmainやmainから送られた実行パラメータを得ます。
		*/
		PROUDSRV_API static void GetEnvp(CFastArray<String>& output);

		/*
		\~korean
		SCM에서 보내준 실행 파라메터를 얻는다.

		\~english
		Gets execution parameter from SCM

		\~chinese
		获得从SCM发送的实行参数。

		\~japanese
		SCMから送られた実行パラメータを得ます。
		*/
		PROUDSRV_API static void GetArgvFromSCM(CFastArray<String>& output);
	};

	DECLARE_DLL_SINGLETON(PROUDSRV_API, CNTService);

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
