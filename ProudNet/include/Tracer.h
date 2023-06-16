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
#include "PNString.h"
#include "Enums.h"

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
	Tracer.h 파일을 Include해야 합니다.
	파일에 로그를 기록하는 모듈입니다. ( <a target="_blank" href="http://guide.nettention.com/cpp_ko#write_pidl" >PIDL 파일 작성하기</a>  참고)

	일반적 용도
	- CLogWriter.New 를 써서 이 객체를 생성합니다. 생성시 저장할 로그 파일 이름과 새로운 파일을 만들 라인수를 지정합니다.
	- WriteLine, WriteLine를 써서 로그를 기록합니다. 저장된 로그는 비동기로 파일에 저장됩니다.

	\~english
	 Module that records log to a file (Refer <a target="_blank" href="http://guide.nettention.com/cpp_en#write_pidl" >Making a PIDL file</a> )

	General usage
	- Use CLogWriter.New to create this object. Designates log file name when created.
	- Use WriteLine, WriteLine to record log. Saved log is saved in file as asynchronous.

	\~chinese
	往文件记录log的模块。（参考 <a target="_blank" href="http://guide.nettention.com/cpp_zh#write_pidl" >创建PIDL文件</a>）

	一般用途
	- 使用 CLogWriter.New%生成此对象。生成的时候指定要保存的log文件名和要创建新文件的行数。
	- 使用WriteLine, WriteLine记录log。保存的log以非同步形式保存到文件。

	\~japanese
	Tracer.hファイルをIncludeしなければなりません。
	ファイルにログを記録するモジュールです。（\ref logwriter_file参考）

	一般的な用途
	- CLogWriter.Newを使ってこのオブジェクトを生成します。生成時に保存するログファイル名と新しいファイルを作るライン数を指定します。
	- WriteLine, WriteLineを使ってログを記録します。保存されたログは非同期でファイルに保存します。
	\~
	*/
	class CLogWriter
	{
	public:
		enum eLogType
		{
			eLogTypeNewFile = 0,
			eLogTypeDefault,
			eLogTypeUserDefine
		};
	
		/** 
		\~korean
		\brief 새 CLogWriter 객체를 생성합니다.
		\param logFileName 로그 파일 이름
		\param newFileForLineLimit 로그 줄 수가 여기서 지정한 값 이상으로 커지면 새 로그 파일에 기록하기 시작합니다. 0이면 무제한입니다.
		\param putFileTimeString true를 지정하면 파일명에 현재 시간정보(연월일시분초)를 붙여 파일명 중복을 방지합니다.

		\~english TODO:translate needed.
		\brief Generate a new CLogWriter object.
		\param logFileName log file name. 
		\param newFileForLineLimit If the number of the log lines gets bigger than the designated value her, then it starts recording in a new log file. It is limitless if 0.
		\param putFileTimeString If set as “true”, the current time information (year/month/date/hour/minute/second) is added to prevent duplication of file names. 

		\~chinese
		\brief 生成新的 CLogWriter%对象。
		\param logFileName log文件名。
		\param newFileForLineLimit log行数比在这里指定值更多的话，开始记录到新文件里。

		\~japanese
		\brief 新しいCLogWriterオブジェクトを生成します。
		\param logFileName ログファイル名
		\param newFileForLineLimit ログ行数がここで指定した値以上に大きくなると新しいログファイルに記録し始めます。0であれば無制限です。
		\param putFileTimeString trueを指定するとファイル名に現在の時間情報(年月日時分秒)をつけてファイル名重複を防止します。
		\~
		*/
		 PROUD_API static CLogWriter* New(const String& logFileName, unsigned int newFileForLineLimit = 0, bool putFileTimeString = true);

	protected:
		CLogWriter(); // CLogWriter::New()를 대신 사용하세요!

	public:
		virtual ~CLogWriter();

		/** 
		\~korean
		로그 파일을 다른 이름으로 바꿉니다. 기록 중이던 파일은 닫히고 다른 이름으로 바뀐 파일로 새로 열립니다. 
		- 이 함수는 비동기로 실행됩니다.  즉, 즉시 리턴됩니다.
		- 만약 로그 파일을 다른 이름으로 바꾸는 것이 실패하면 이 함수는 오류를 리턴하지 않습니다. 그 대신 로그를 기록하는 함수를 
		사용할 때 Proud.Exception 예외가 발생할 것입니다. 
		\param logFileName 새 로그 파일 이름 

		\~english
		The log file is changed to another name. The file being recorded is closed and opens newly as a file changed to a different name.
		- This function is executed asynchronously. In other words, it is returned immediately. 
		- If the log file fails to change to a different name, this function does not return the error. Instead, Proud.Exception exception will occur when the function to record the log is used. 
		\param logFileName New log file name

		\~chinese
		把日志文件修改为其他名称。记录中的文件将被关闭，同时打开其他名称的文件。 
		- 此函数非同步运行。即，立即返回。
		- 如果日志文件的名称更改失败，将不返回错误。 但在使用日志记录函数会发生Proud.Exception例外。 
		\paramlogFileName 新日志文件名称 

		\~japanese
		ログファイルの名前を変更します。記録していたファイルは閉じて、変更された名前のファイルが新たに開かれます。
		- この関数は非同期で実行されます。即ち、直ちにリターンされます。
		- もし、ログファイルの名前変更に失敗したら、この関数はエラーをリターンしません。その代わりにログを記録する関数を使う時、Proud.Exception例外が発生します。
		\param logFileName 新しいログファイル名
		\~
		*/
		virtual void SetFileName(const String& logFileName) = 0;

		/** 
		\~korean
		로그 한줄을 ProudNet 양식에 맞춰 파일에 기록합니다.
		- 이 함수는 비동기로 실행됩니다. 즉, 즉시 리턴됩니다.
		 -이 함수는 아래 예제처럼 최종 출력 됩니다. 
		  logWriteLineTime: logLevel / logCategory(logHostID): logMessage {logFunction(logLine)}
		\param logLevel 로그 내용의 심각도
		\param logCategory 어떤 종류의 로그인지?
		\param logHostID 이 로그를 전송한 호스트
		\param logMessage 로그 메시지
		\param logFunction 로그를 호출한 함수 이름
		\param logLine 로그를 호출한 라인

		\~english
		One line of the log is recorded in the file in the ProudNet format. 
		- This function is executed asynchronously. In other words, it is returned immediately.
		- This function is finally output as the examples below.
		logWriteLineTime: logLevel / logCategory(logHostID): logMessage {logFunction(logLine)}
		\param logLevel Severity of the log content
		\param logCategory What type of a log is this?
		\param logHostID The host transmitting this log. 
		\param logMessage Log message
		\param logFunction Name of the function that has called the log
		\param logLine Line that has called the log


		\~chinese
		把一行日志按照ProudNet样式记录到文件。
		- 此函数非同步运行，即，立即返回。
		-此函数最终被输出，如下例子。 
		logWriteLineTime: logLevel / logCategory(logHostID): logMessage {logFunction(logLine)}
		\param logLevel 日志内容的重要度
		\param logCategory 何种类型的日志?
		\param logHostID 传输此日志的主机
		\param logMessage 日志消息
		\param logFunction 调用日志的函数名称
		\param logLine 调用日志的line

		\~japanese
		ログ1行をProudNetの様式に合わせてファイルに記録します。
		- この関数は非同期で実行されます。即ち、直ちにリターンされます。
		- この関数は下の例題のように最終出力されます。
		logWriteLineTime: logLevel / logCategory(logHostID): logMessage {logFunction(logLine)}
		\param logLevel ログ内容の深刻度
		\param logCategory どんな種類のログなのか？
		\param logHostID このログを転送したホスト
		\param logMessage ログメッセージ
		\param logFunction ログを呼び出した関数名
		\param logLine ログを呼び出したライン
		\~
		*/
		virtual void WriteLine(int logLevel, LogCategory logCategory, HostID logHostID, const String& logMessage, const String& logFunction = _PNT(""), int logLine = 0) = 0;

		/**
		\~korean
		로그 한줄을 파일에 기록합니다.
		- 이 함수는 비동기로 실행됩니다. 즉, 즉시 리턴됩니다.
		- 단, void WriteLine(TraceID TID, LPCWSTR text) 와 달리 아무런 헤더를 포함하지 않습니다.
		\param logMessage 로그를 찍을 문자열. 
		- String 형태 또는 WriteLine(String::NewFormat(L"Test Log %d", 10)) 형태로 사용 가능합니다.

		\~english
		Writes a line of log to the file
		- This function is asynchronous so returned immediately.
		- But not like void WriteLine(TraceID TID, LPCWSTR text), does not have any header.
		\param logMessage text string to record log.
		- You are able to use string type or WriteLine(String::NewFormat(L"Test Log %d", 10)) type

		\~chinese
		把一行log记录到文件。
		- 此函数以非同步进行。即，会立即返回。
		- 但是，与void WriteLine(TraceID TID, LPCWSTR text)不同并不会包括任何header。
		\param logMessage 记录log的字符串。
		- You are able to use string type or WriteLine(String::NewFormat(L"Test Log %d", 10)) type

		\~japanese
		ログ1行をファイルに記録します。
		- この関数は非同期で実行されます。即ち、直ちにリターンされます。
		- 但し、void WriteLine(TraceID TID, LPCWSTR text)とは違って、何らのヘッダーを含めません。
		\param logMessage ログを残す文字列 
		- String 形態またはWriteLine(String::NewFormat(L"Test Log %d", 10))形態で使用可能です。
		\~
		*/
		virtual void WriteLine(const String& logMessage) = 0;

		/**
		\~korean
		- CLogWriter 객체를 제거(종료) 할 때 미처 출력하지 못한 로그들을 마저 출력할지 또는 무시할지 여부를 결정합니다.
		- 기본값은 false입니다.
		- true로 설정되면 출력하지 못한 로그가 있어도 무시하고 즉시 종료합니다.
		- false로 설정된 경우 출력하지 못한 로그들을 모두 출력 할 때 까지 종료가 지연됩니다.

		\param flag 이 옵션을 사용할지 여부입니다.
		
		\~english 
		- This determines whether to output the rest of the logs not yet output when the object is terminated (ended) or ignore them. 
		- The default value is false. 
		- If set true, it terminates immediately even if there is a log not yet output. 
		- If set false, termination is delayed until not yet output logs are all output. 

		\param flag Whether to use this option or not. 

		\~chinese
		- 对清除（结束）CLogWriter 对象时未及时输出的日志进行继续输出或忽略操作做出决定。
		- 默认值为false。
		- 如果是true，即便存在未输出完的日志也会忽略并立即结束。
		- 如果是false，结束操作将被延迟，直到未输出的日志完成输出。

		\param flag 是否使用此选项。

		\~japanese
		- CLogWriter オブジェクトを除去（終了）する時、まだ出力できなかったログを最後まで出力するか、または無視するかを決定します。
		- 基本値はfalseです。
		- trueに設定すると出力できなかったログがあっても無視して直ちに終了します。
		- falseに設定した場合、出力できなかったログを全て出力する時まで終了が遅延されます。
		\param flag このオプションを使用するかどうかです。
		
		\~
		*/
		virtual void SetIgnorePendedWriteOnExit(bool flag) = 0;
	};

	/**  @} */

	PROUD_API String GetFileTimeString();
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
