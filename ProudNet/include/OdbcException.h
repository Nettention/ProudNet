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

#include <sqltypes.h>

#include "Exception.h"
#include "FastArray.h"
#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/**
	 \~korean
	 ODBC의 익셉션 처리를 위한 클래스입니다.
	 what() 함수를 이용하여 상세 내용을 확인할 수 있습니다.

	 \~english
	 It is a class that deals with ODBC exception. 
	 You can check the detailed information by what() function. 

	 \~chinese
	 处理ODBC异常的class。
	 通过what()函数可以确认详细内容。

	 \~japanese
	 ODBCのexception処理のためのクラスです。
	 what() 関数を利用して詳細内容を確認できます。

	 \~
	 */
	class COdbcException: public Exception
	{
	protected:
		SQLRETURN m_ret;

	public:
		PROUDSRV_API COdbcException();
		PROUDSRV_API COdbcException(const SQLRETURN ret, const StringA& errorString);

		/**
		 \~korean
		 sql.h나 sqlext.h에 정의된 return value를 반환 합니다.

		 \~english
		 It returns the return value that has been defined in sql.h or sqlext.h.

		 \~chinese
		 返还在sql.h或 sqlext.h定义的return value。

		 \~japanese
		 sql.hやsqlext.hに定義されたreturn valueを返還します。

		 \~
		 */
		PROUDSRV_API int GetSqlErrorCode();

		PROUDSRV_API bool IsConnectionTimeout();

		PROUDSRV_API COdbcException(const COdbcException& other);
		PROUDSRV_API COdbcException& operator=(const COdbcException& other);
	};

	/**
	 \~korean
	 SqlErrorCode가 SQL_SUCCESS_WITH_INFO일 경우 Warning Exception이 발생합니다.
	 해당 익셉션은 기본적으론 무시해도 상관없으나 중요한 정보를 담고있는 경우도 있으므로 주의해야 합니다.
	 ex> Warning 발생 예
	 1. MSSQL의 경우 사용하는 데이터베이스(스키마)가 변경되는 경우에 변경되었다는 알림을 위해 발생
	 2. Insert시 Primary Key가 중복된 데이터일 경우 발생(Primary Key Duplicate Error)

	 \~english
	 In case SqlErrorCode is SQL_SUCCESS_WITH_INFO, Warning Exception will occur. 
	 It does not matter if you ignore this exception, but it contains important information, so you should take extra caution. 
	 ex> Example of Warning Occurrence
	 1. In case of MSSQL, when the database (schema) is changed, warning will occur.
	 2. In case primary keys are duplicated when inserting, warning will occur. (Primary Key Duplicate Error)

	 \~chinese
	 SqlErrorCode是 SQL_SUCCESS_WITH_INFO的情况，发生 Warning Exception。
	 一般情况下无视该异常也没有太大问题，但也有重要信息的情况，所以需注意。
	 ex> Warning 发生 例
	 1. . MSSQL的情况，使用的数据库变更时，为提示变更而发生。
	 2. Insert시 Primary Key为重复数据时发生(Primary Key Duplicate Error)

	 \~japanese
	 SqlErrorCodeがSQL_SUCCESS_WITH_INFOの場合Warning Exceptionが発生します。
	 該当exceptionは基本的に無視しても構いませんが、重要な情報を含めている場合もあるためご注意ください。
	 ex> Warning 発生の例
	 1. MSSQLの場合、使用しているデータベース（スキーマ）が変更される場合に変更のお知らせのため発生
	 2. Insert時Primary Keyが重複されたデータの場合発生(Primary Key Duplicate Error)

	 \~
	 */
	class COdbcWarning: public COdbcException
	{
	public:
		PROUDSRV_API COdbcWarning();
		PROUDSRV_API COdbcWarning(const SQLRETURN ret, const StringA& errorString);

		PROUDSRV_API COdbcWarning(const COdbcWarning& other);
		COdbcWarning& operator=(const COdbcWarning& other);
	};

	typedef CFastArray<COdbcWarning> COdbcWarnings;
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
