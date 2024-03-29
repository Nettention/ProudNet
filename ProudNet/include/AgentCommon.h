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

#include "ProudNetServer.h"
#include "ProudNetClient.h"

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


	/**
	\~korean
	일정 시간 혹은 OnReportStatsCommand 가 왔을때 Agent 로 보고할 Report 클래스
	- 사용자가 임의로 값을 넣어서 사용할수 있습니다.
	- 여기서 입력한 값은 PNServerAgentConsole 에서 확인가능합니다.

	\~english TODO:translate needed.
	The report class to report with Agent after a certain amount of time or when OnReportStatsCommand has come. 
	- Users can input values randomly to use
	- The value input here can be identified in PNServerAgentConsole


	\~chinese
	一定的时间或者OnReportStatsCommand的时候用Agent报告的Report类。
	- 使用者可以输入任意值使用。
	- 在这里输入的值在PNServerAgentConsole可以确认。

	\~japanese
	所定時間またはOnReportStatsCommandが来た時、Agentに報告するReportクラス
	- ユーザーが任意に値を入れて使うことができます。
	- ここで入力した値はPNServerAgentConsoleで確認できます。
	\~
	*/
	class CReportStatus
	{
	public:
		/** 
		\~korean
		이값에 따라서 PNLicenseAgentConsole 에서 글자색이 달라집니다. 

		\~english TODO:translate needed.
		Depending on this value, the color of the letters change in PNLicenseAgentConsole


		\~chinese
		随着此值，在PNLicenseAgentConsole的字体颜色会改变。

		\~japanese
		この値によって、 PNLicenseAgentConsoleで文字色が違ってきます。
		\~
		*/
		enum StatusType { StatusType_OK, StatusType_Warning, StatusType_Error };

		/** 
		\~korean
		AgentConsole 로 보낼 정보의 타입

		\~english TODO:translate needed.
		Type of the information to be sent to AgentConsole


		\~chinese
		往AgentConsole发送信息的类型。

		\~japanese
		AgentConsole に送る情報のタイプ
		\~
		*/
		StatusType m_statusType;

		/** 
		\~korean
		정보의 내용. 이 String이 비면  Data를 보내지 않습니다.

		\~english TODO:translate needed.
		Content of the information. If this string is empty, data is not transmitted


		\~chinese
		信息的内容。如果此String空的话不发送Data。

		\~japanese
		情報の内容。このStringが空くとデータを送りません。
		\~
		*/
		String m_statusText;
		
		typedef CFastMap<String, String> KeyValueList;
		
		/**
		\~korean
		추가적으로 필요한 Data들의 정보 List
		- AgentConsole의 property View의 Log를 통하여 확인하실 수 있습니다.
		- AgentConsole의 property View에서 Key : Value로 출력됩니다.

		\~english TODO:translate needed.
		List List of the data needed additionally
		- You can check with the log of the property view of AgentConsole.
		- It is output as Key : Value of the property view of AgentConsole.


		\~chinese
		额外需要的Data信息List。
		-AgentConsole 的property可以通过View Log可以确认.
		-AgentConsole 的property在Key : Value输出.

		\~japanese
		追加的に必要なデータ情報リスト
		- AgentConsoleのproperty ViewのLogを通じ確認することができます。.
		- AgentConsoleのproperty ViewでKey : Valueで出力されます。
		\~
		*/
		KeyValueList m_list;
	};

	/**
	\~korean
	CAgentConnector 가 요구하는 delegate 인터페이스. CAgentConnector 를 생성할 때 인자로 필요하다.

	\~english TODO:translate needed.
	The delegate interface required by CAgentConnector. It is required as a factor when generating CAgentConnector


	\~chinese
	CAgentConnector 需要的delegate界面。作为因子，生成 CAgentConnector%的时候需要。

	\~japanese
	CAgentConnectorが要求するdelegateインターフェース。CAgentConnectorを生成する時に因子として必要です。
	\~
	*/
	class IAgentConnectorDelegate
	{
	public:
		/** 
		\~korean
		Agent connect의 인증이 성공된 경우 callback됩니다.

		\~english TODO:translate needed.
		It is called back when Agent connect’s authentication is successful


		\~chinese
		Agent connect 的认证成功时会callback。

		\~japanese
		Agent connectの認証に成功した場合、callbackされます。
		\~
		*/
		virtual void OnAuthentication(ErrorInfo* errorinfo) = 0;

		/** 
		\~korean
		Server app을 종료시키라는 명령이 온경우 callback됩니다.

		\~english TODO:translate needed.
		It is called back when the command to terminate the server app is received


		\~chinese
		接到结束Server app的命令时会callback。

		\~japanese
		Server appを終了させろという命令が来た場合、callbackされます。
		\~
		*/
		virtual void OnStopCommand() = 0;

		/**
		\~korean
		Agent Server 로부터 Report 요청 명령이 온경우 callback됩니다.

		\~english TODO:translate needed.
		It is called back when the report request command has come from the agent server


		\~chinese
		从Agent Server接到Report邀请命令时会callback。

		\~japanese
		Agent ServerからReportリクエスト命令が来た場合、callbackされます。
		\~
		*/
		virtual void OnReportStatsCommand() = 0;

		/** 
		\~korean
		Agent를 사용자가 잘못된 방법으로 사용하였을 시에 호출됩니다.

		\~english TODO:translate needed.
		It is called when the agent is used in a wrong way by the user


		\~chinese
		用户以错误的方法使用Agent的时候呼出。

		\~japanese
		Agentをユーザーが間違った方法で使った時に呼び出されます。
		\~
		*/
		virtual void OnWarning(ErrorInfo* errorinfo) = 0;
	};

	/**
	\~korean
	Agent와 연결하여 PNServerAgentConsole 에서 사용자의 서버앱을 원격으로 관리할수 있습니다.

	\~english TODO:translate needed.
	By connecting with Agent, the server app of the user can be controlled remotely from PNServerAgentConsole


	\~chinese
	连接Agent以后在PNServerAgentConsole可以远程管理用户的server app。

	\~japanese
	Agentと連結してPNServerAgentConsoleでユーザーのサーバーアプリをリモートで管理することができます。
	\~
	*/
	class CAgentConnector
	{
	public:
		virtual ~CAgentConnector() {}

		 static CAgentConnector* Create(IAgentConnectorDelegate* dg);

		/** 
		\~korean
		Start 후 객체를 삭제하기 전까지 Agent와 계속 연결을 유지합니다.
		- ServerAgent에 의하여 실행 된 것이 아닌 경우 Start되지 않습니다.

		\~english TODO:translate needed.
		Before deleting an object after start, it maintains connection with the agent continuously
		- It does not start if it is not executed by ServerAgent


		\~chinese
		Start 以后，把对象删除之前会继续维持与Agent的连接。
		- 不是被ServerAgent实行的情况不Start。

		\~japanese
		Start後にオブジェクトを削除する前までAgentと続けて連結を維持します。
		- ServerAgentによって実行されたのではない場合はStartされません。
		\~
		*/
		virtual bool Start() = 0;

		/**
		\~korean
		일정 시간 혹은 IAgentConnectorDelegate::OnReportStatsCommand 가 왔을때 이 함수를 이용하여 CReportStatus 를 Agent로 보낼수 있습니다.
		\return ServerAgent에 의하여 실행되지 않아 Start 되지 않았거나 reportStatus의 Data가 잘못되었을 시에 false 

		\~english TODO:translate needed.
		When a certain time or IAgentConnectorDelegate::OnReportStatsCommand has come, CReportStatus can be sent to the agent using this function
		- False when it has not started because it is not executed by \return ServerAgent or when the data of reportStatus is wrong


		\~chinese
		一定时间或者IAgentConnectorDelegate::OnReportStatsCommand 的时候，利用此函数可以把 CReportStatus%发送到Agent。
		\return 没有被ServerAgent实行而没有Start的或者reportStatus的Data出错的时候fals。

		\~japanese
		所定時間またはIAgentConnectorDelegate::OnReportStatsCommandが来た時、この関数を利用してCReportStatusをAgentに送ることができます。
		\return ServerAgentによって実行されずStartされなかったとか、reportStatusのDataが間違っている時にfalse 
		\~
		*/
		virtual bool SendReportStatus(CReportStatus& reportStatus) = 0;

		/** 
		\~korean
		Agent로 Log를 보내는 기능 
		- 상세한 데이터가 아닌 간단한 로그를 보내고자 할 때 SendReportStatus를 사용하지 않고 EventLog를 통하여 간편이 이용하실 수 있습니다.
		\return text가 비었거나 ServerAgent가 Start되지 못하였을 때 false

		\~english TODO:translate needed.
		Feature to send a log to Agent
		- When you wish to send a simple log instead of detailed data, you can conveniently use through an EventLog instead of using SendReportStatus. 
		\False when \return text is empty or ServerAgent does not start


		\~chinese
		把Log发送到Agent的功能
		- 想发送不是详细数据而是简单log的时候，不用SendReportStatus，而是可以通过EventLog简单利用。
		\return text空着或者ServerAgent没Start的时候false。

		\~japanese
		AgentにLogを送る機能
		- 詳しいデータではない簡単なログを送りたい時、SendReportStatusを使わなくEventLogを通じ簡単にご利用できます。
		\return textが空いているか、ServerAgentがStartされなかった時にfalse
		\~
		*/
		virtual bool EventLog(CReportStatus::StatusType logType, const PNTCHAR* text) = 0;

		/**
		\~korean
		일정 시간에 한번씩 해주어야 하는 일들을 처리합니다.
		- 연결이 되어 있지 않다면 일정 시간마다 자동으로 연결을 시도 합니다.

		\~english TODO:translate needed.
		It processes tasks that need to be done every once in a certain period of time.
		- If not connected, it attempts connection automatically every certain time.


		\~chinese
		处理在一定时间每进行一次的事情。
		- 没有连接的话会每一定时间自动尝试连接。

		\~japanese
		所定時間に1回ずつ行うべきことを処理します。
		- 連結されなかったら所定時間ごとに自動に連結を試みます。
		*/
		virtual void FrameMove() = 0;

		/**
		\~korean
		FrameMove 내의 실시간 상태 정보 전송에 대한 Delay Time을 정합니다. 
		- 1/1000초 단위입니다. 
		- default 는 1000입니다.

		\~english TODO:translate needed.
		It sets the delay time for transmitting the real-time information transmission within FrameMove
		- It is in 1/1000 second unit
		- The default is 1000


		\~chinese
		决定对FrameMove内的实时状态信息传送的Delay Time。
		- 是1/1000秒单位。 
		- default是1000。


		\~japanese
		FrameMove内のリアルタイム状態情報の転送に対するDelay Timeを決めます。
		- 1/1000秒単位です。
		- defaultは1000です。
		\~
		*/
		virtual void SetDelayTimeAboutSendAgentStatus(uint32_t delay) = 0;

	};


#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
