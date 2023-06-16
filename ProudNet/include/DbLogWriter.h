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

#if defined(_WIN32)

#include "AdoWrap.h"

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

	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	LogWriter에서 오류가 났을 때 오류 메시지를 받을 클래스 입니다. 상속 하여 함수를 정의해 주시면 됩니다.
	- CDbLogWriter 생성시 인자로 넣어주게 됩니다.

	\~english
	This class is to receive error message when error occur at LogWriter. You can define function that using inherit.
	- When you creates CDbLogWriter, put it as a factor.

	\~chinese
	在LogWriter出错时，接收错误信息的类。上诉并定义函数即可。
	- CDbLogWriter%生成时放入为因子。

	\~japanese
	LogWriterでエラーになった時、エラーメッセージを受けるクラスです。相続した後、関数を定義してください。
	- CDｂLogWriterを生成時に因子で入れてください。
	\~
	*/
	class ILogWriterDelegate
	{
	public:
		virtual void OnLogWriterException(Proud::AdoException &Err) = 0;	//LogWriterException(exception객체를 파라미터로)
	};
	
	/** 
	\~korean
	CDbLogWriter 에서 사용되는 구조체

	\~english
	Structure used in CDbLogWriter 

	\~chinese
	在 CDbLogWriter%使用的构造体。

	\~japanese
	CDbLogWriterで使われる構造体
	\~
	*/
	class  CDbLogParameter
	{
	public:
		/** 
		\~korean
		로그 기록이 남을 데이터베이스로 접근하는 DBMS Connection String입니다. 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#logwriter_db" >데이터베이스에 로그를 기록하기</a> 를 참고하십시오.

		\~english
		DBMS Connection String accesses to DB that log record will be written on. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#logwriter_db" >Recording a log in database</a>.

		\~chinese
		接近log记录留下的数据库的DBMS Connection String。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#logwriter_db" >在数据库中记录log</a>%。

		\~japanese
		ログ記録が残るデータベースをアクセスするDBMS Connection Stringです。詳しくは、\reflogwriter_dbをご参照ください。
		\~
		*/
		String m_dbmsConnectionString;
		
		/** 
		\~korean
		로그를 기록하는 주체의 이름입니다. 예를 들어 이 로그를 기록하는 프로세스의 이름, 가령 "BattleServer #33"을 넣으시면 됩니다.

		\~english
		Name of the subject that writes log. For an example, the name of the process that writes this log, such as "BattleServer #33"

		\~chinese
		记载log的主体的名称。例如，记载此log的程序的名称，放入"BattleServer #33"即可。

		\~japanese
		ログを記録する主体名です。例えば、このログを記録するプロセスの名前、仮に"BattleServer #33"を入れてください。
		\~
		*/
		String m_loggerName;

		/** 
		\~korean
		로그를 기록하는 Dbms타입입니다.MsSql,MySql등을 지원합니다.

		\~english
		DBMS type that writes log. Supports MSSQL, MySQL and etc.

		\~chinese
		记载log的Dbms类型。支援MsSql,MySql 等。

		\~japanese
		ログを記録するDbmsタイプです。MsSql、MySqlなどをサポートします。
		\~
		*/
		DbmsType m_dbmsType;

		/**
		\~korean
		로그 기록을 할 DB에 있는 로그테이블명 입니다. default 는 DbLog 입니다.

		\~english 
		This is the log table name in the DB to record the log. The default is DbLog. 

		\~chinese
		记载log的时候存在与DB的log table名。Default是DbLog。

		\~japanese
		ログ記録をするDBにあるログテーブル名です。DefaultはDblogです。
		\~
		*/
		String m_dbLogTableName;

		PROUDSRV_API 		CDbLogParameter();
	};


	

	/** 
	\~korean
	DBMS 에 로그를 기록합니다. ( <a target="_blank" href="http://guide.nettention.com/cpp_ko#logwriter_db" >데이터베이스에 로그를 기록하기</a> 참고)

	일반적 용도
	- CDbLogWriter 를 사용하기 전에 ProudNet/Sample/DbmsSchema/LogTable.sql 을 실행하여 DbLog 테이블을 생성해야 합니다.
	- CDbLogWriter.New 를 써서 이 객체를 생성합니다. 
	- WriteLine, WriteLine 를 써서 로그를 기록합니다. 저장된 로그는 비동기로 저장됩니다.
	- 기본적으로 LoggerName, LogText, DateTime 이 기록됩니다. 유저가 원하는 컬럼을 넣으려면 WriteLine의 CPropNode 를 사용하면 됩니다.

	\~english
	 Write log to DBMS (Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#logwriter_db" >Recording a log in database</a>)

	General usage
	- Before using CDbLogWriter, you must create Dblog table by running ProudNet/Sample/DbmsSchema/LogTable.sql
	- Create this object by using CDbLogWriter.New
	- Record log by using WriteLine, WriteLine. (Note: maybe a typo) The recorded log will be regarded as asynchronous.
	- Basically LoggerName, LogText and DateTime will be recorded. In order to add a column that user wants, use CPropNode of WriteLine

	\~chinese
	往DBMS记载log（参考 <a target="_blank" href="http://guide.nettention.com/cpp_zh#logwriter_db" >在数据库中记录log</a>）
	
	一般的用途
	- 使用 CDbLogWriter%之前执行ProudNet/Sample/DbmsSchema/LogTable.sql，要生成DbLogtable。
	- 使用 CDbLogWriter.New%生成此对象。
	- 使用WriteLine, WriteLine记载log。储存的log会保存为异步。
	- 基本记载为LoggerName, LogText, DateTime。想放入用户需要的专栏的话使用WriteLine的 CPropNode%即可。

	\~japanese
	DBMSにログを記録します。( \ref logwriter_db 参考)

	一般的な用途
	- CDbLogWriterを使用する前にProudNet/Sample/DbmsSchema/LogTable.sqlを実行してDbLogテーブルを生成しなければなりません。
	- CDbLogWriter.New を使ってこのオブジェクトを生成します。 
	- WriteLine、WriteLineを使ってログを記録します。保存されたログは非同期で保存されます。
	- 基本的にLoggerName, LogText, DateTimeが記録されます。ユーザーが望むカラムを入れるためには、WriteLineのCPropNode を使ってください。
	\~
	*/
	class  CDbLogWriter
	{
	protected:
		inline		CDbLogWriter() {} // use CDbLogWriter::New() instead.
	public:
		/** 
		\~korean
		CDbLogWriter 인스턴스를 생성합니다.
		\param logParameter 로그 기록기의 시작을 위한 설정값들입니다.
		\param pDelegate 로그 기록기가 실행 중 필요로 하는 콜백을 구현하는 delegate입니다.

		\~english
		Create CDbLogWriter instance.
		\param logPramerter Setup value to start log writer.
		\param pDelegate It is delegate for realizing callback that requires during log writer running.

		\~chinese
		生成 CDbLogWriter%实例。
		\param logParameter 为了log记录仪的开始的设定值。
		\param pDelegat 体现log记录仪执行中需要的回调的delegate。

		\~japanese
		CDbLogWriter インスタンスを生成します。
		\param logParameter ログ記録器の開始のための設定値です。
		\param pDelegate ログ記録器が実行中に必要とするコールバックを実現するdelegateです。
		\~
		*/
		PROUDSRV_API 		static CDbLogWriter* New(CDbLogParameter& logParameter, ILogWriterDelegate *pDelegate);

		virtual ~CDbLogWriter() {}

		/** 
		\~korean
		한 개의 로그를 기록합니다. 
		- 이 함수는 비동기로 실행됩니다. 즉, 즉시 리턴됩니다.
		\param logText 찍을 로그 문자열
		\param pProperties 사용자가 추가한 필드에 들어갈 값들입니다. 사용 예는 <a target="_blank" href="http://guide.nettention.com/cpp_ko#logwriter_db" >데이터베이스에 로그를 기록하기</a> 를 참고하십시오.

		\~english
		Records 1 log 
		- this function runs as asynchronous. In other words, it will be returned immediately.
		\param logText log text string
		\param pProperties Values to be entered to the fields that were added by user. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#logwriter_db" >Recording a log in database</a>.

		\~chinese
		记录一个log。
		- 此函数异步执行。也就是说可以立即返回。
		\param logText log字符串。
		\param pProperties 进入用户添加的领域的值。使用例请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#logwriter_db" >在数据库中记录log</a>%。

		\~japanese
		1個のログを記録します。
		- この関数は非同期で実行されます。即ち、直ちにリターンされます。
		\param logText 残すログ文字列
		\param pProperties ユーザーが追加したフィールドに入る値です。使用例は、\ref logwriter_db をご参照ください。
		\~
		*/
		virtual void WriteLine( String logText, CProperty* const pProperties=NULL) = 0;
	};

	/**  @} */

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif //_WIN32
