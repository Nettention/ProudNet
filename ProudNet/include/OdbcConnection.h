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

#include "OdbcEnum.h"
#include "OdbcException.h"
#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class COdbcHandle;
	class COdbcRecordset;
	class COdbcConnectionImpl;

	/**
	 \~korean
	 Connection 관리와 Transaction 제어, DirectExecute를 실행할 수 있는 클래스 입니다.
	 ODBC 매니저를 통해 사용하시는 데이터베이스의 드라이버를 등록하여 해당 DSN(Data Source Name)을 통해 접근하시면 됩니다.

	 \~english
	 It is a class that can manage Connection, control Transaction and execute DirectExecute.
	 Register a driver of database that you are using by the ODBC manager and access through DSN(Data Source Name).

	 \~chinese
	 可实行Connection 管理， Transaction 制御, DirectExecute 的class。
	 通过 ODBC管理，注册使用的数据库driver，通过该DSN(Data Source Name)访问即可。

	 \~japanese
	 Connection 管理と Transaction コントロール、DirectExecuteを実行できるクラスです。
	 ODBCマネージャーを通じて使用するデータベースのドライバーを登録し、該当DSN(Data Source Name)を通じて接近してください。

	 \~
	 */
	class COdbcConnection
	{
	public:
		PROUDSRV_API COdbcConnection();
		PROUDSRV_API ~COdbcConnection();

		/**
		 \~korean
		 데이터베이스에 연결할 때 사용합니다.

		 \~english
		 It is used when connecting to the database. 

		 \~chinese
		 连接数据库时使用。

		 \~japanese
		 データベースに連結するときに使用します。

		 \~
		 */
		 PROUDSRV_API void Open(const String& dsn, const String& id, const String& passwd, COdbcWarnings* odbcWarnings = NULL);

		/**
		 \~korean
		 데이터베이스에 연결할 때 사용합니다.
		 ex> conn.Open("DSN=MsSqlDsn;UID=root;PWD=1234")

		 \~english
		 It is used when connecting to the database. 
		 ex> conn.Open("DSN=MsSqlDsn;UID=root;PWD=1234")

		 \~chinese
		 连接数据库时使用。
		 ex> conn.Open("DSN=MsSqlDsn;UID=root;PWD=1234").

		 \~japanese
		 データベースに連結するときに使用します。
		 ex> conn.Open("DSN=MsSqlDsn;UID=root;PWD=1234")

		 \~
		 */
		 PROUDSRV_API void Open(const String& connectionString, COdbcWarnings* odbcWarnings = NULL);

		/**
		 \~korean
		 연결을 종료할 때 사용합니다.
		 이 커넥션을 통해 생성된 모든 COdbcCommand, COdbcRecordset 객체에게 영향을 미칩니다.

		 \~english
		 It is used when disconnection.
		 It has an effect on all COdbcCommand objects and COdbcRecordset objects that have been created by this connection. 

		 \~chinese
		 连接终了时使用。
		 会影响到所有通过此连接生成的 COdbcCommand, COdbcRecordset 对象。

		 \~japanese
		 連結を終了するときに使用します。
		 このコネクションを通じて生成された全てのCOdbcCommand, COdbcRecordsetオブジェクトに影響を与えます。

		 \~
		 */
		 PROUDSRV_API void Close();

		/**
		 \~korean
		 트랜잭션을 시작할 때 사용합니다.
		 RollbackTrans() 이나 CommitTrans()을 실행하여 트랜잭션을 종료하면 다시 호출해야 적용 됩니다.

		 \~english
		 It is used when starting transaction. 
		 In case transaction ends by executing RollbackTrans() or CommitTrans(), it is applied only when re-calling. 

		 \~chinese
		 Transaction开始时使用。
		 通过实行 RollbackTrans() 或 CommitTrans()终了Transaction时，需重新呼出才能被适用。

		 \~japanese
		 トランザクションを始まる時に使用します。
		 RollbackTrans() や CommitTrans()を実行し、トランザクションを終了した場合、適用のためにはもう一度呼び出す必要があります。

		 \~
		 */
		 PROUDSRV_API void BeginTrans();

		/**
		 \~korean
		 트랜잭션을 Rollback할 때 사용합니다.

		 \~english
		 It is used when transaction rollback. 

		 \~chinese
		 Rollback Transaction时使用。

		 \~japanese
		 トランザクションをRollbackする時に使用します。

		 \~
		 */
		 PROUDSRV_API void RollbackTrans();

		/**
		 \~korean
		 트랜잭션을 Commit할 때 사용합니다.

		 \~english
		 It is used when committing transaction. 

		 \~chinese
		 Transaction Commit 时使用。

		 \~japanese
		 トランザクションをCommitする時に使用します。

		 \~
		 */
		 PROUDSRV_API void CommitTrans();

		/**
		 \~korean
		 데이터베이스에 현재 연결중인지 확인할 때 사용합니다.

		 \~english
		 It is used when checking if connection to the database is normal. 

		 \~chinese
		 确认现在是否与数据库连接时使用。

		 \~japanese
		 現在データベースに連結中であるか確認するときに使用します。

		 \~
		 */
		 PROUDSRV_API bool IsOpened();

		/**
		 \~korean
		 다이렉트 쿼리를 실행합니다.
		 쿼리 실행으로 영향을 받은 Row 개수를 반환합니다.
		 \param query 쿼리문

		 \~english
		 It executed a direct query.
		 \param query It is a query.

		 \~chinese
		 实行 direct query。
		 \param query query文件。

		 \~japanese
		 ダイレクトクエリーを実行します。
		 \param query クエリー文

		 \~
		 */
		 PROUDSRV_API int Execute(const String& query, COdbcWarnings* odbcWarnings = NULL);

		/**
		 \~korean
		 다이렉트 쿼리를 실행합니다.
		 쿼리 실행으로 영향을 받은 Row 개수를 반환합니다.
		 \param recordset SELECT 쿼리 실행 시 받아지는 Recordset 객체
		 \param query 쿼리문

		 \~english
		 It executed a direct query.
		 \param recordset It is a Recordset object that is received when executing SELECT query.
		 \param query It is a query.

		 \~chinese
		 实行 direct query 。
		 \param recordset SELECT query 实行时，接收的 Recordset 对象。
		 \param query query文件

		 \~japanese
		 ダイレクトクエリーを実行します。
		 \param recordset SELECT クエリー実行しロードされるRecordsetオブジェクト
		 \param query クエリー文

		 \~
		 */
		 PROUDSRV_API int Execute(COdbcRecordset &recordset, const String& query, COdbcWarnings* odbcWarnings = NULL);

		// WARNING::내부에서 사용되는 함수입니다. 사용하지 마시기 바랍니다.
		// WARNING::Internal use only. Don't use this function.
		 void AllocStmtHandle(COdbcHandle* hstmt, OdbcPrepareType typem, COdbcWarnings* odbcWarnings = NULL);

	private:
		COdbcConnectionImpl* m_pImpl;

		PROUDSRV_API COdbcConnection(const COdbcConnection& other);
		COdbcConnection& operator=(const COdbcConnection& other);
	};

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
