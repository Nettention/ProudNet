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

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	// C++ 언어에서 인식되는 데이터 타입.
	// SQL C Type 대체.
	enum OdbcDataType
	{
		OdbcDataType_NULL 			= 0,	/* = Basic OdbcSqlDataType 		*/
		OdbcDataType_BOOL 			= 1,	/* = OdbcSqlDataType_BIT 		*/
		OdbcDataType_TCHAR 			= 2,	/* = OdbcSqlDataType_CHAR(Linux) or OdbcSqlDataType_WCHAR(Windows) 		*/
		OdbcDataType_SINT8 			= 3,	/* = OdbcSqlDataType_TINYINT 	*/
		OdbcDataType_UINT8 			= 4,	/* = OdbcSqlDataType_TINYINT 	*/
		OdbcDataType_SINT16 		= 5,	/* = OdbcSqlDataType_SMALLINT 	*/
		OdbcDataType_UINT16 		= 6,	/* = OdbcSqlDataType_SMALLINT 	*/
		OdbcDataType_SINT32 		= 7,	/* = OdbcSqlDataType_INTEGER 	*/
		OdbcDataType_UINT32 		= 8,	/* = OdbcSqlDataType_INTEGER 	*/
		OdbcDataType_SINT64 		= 9,	/* = OdbcSqlDataType_BIGINT 	*/
		OdbcDataType_UINT64 		= 10,	/* = OdbcSqlDataType_BIGINT 	*/
		OdbcDataType_FLOAT 			= 11,	/* = OdbcSqlDataType_FLOAT 		*/
		OdbcDataType_DOUBLE 		= 12,	/* = OdbcSqlDataType_DOUBLE 	*/
		OdbcDataType_STRING 		= 13,	/* = OdbcSqlDataType_VARCHAR(Linux) or OdbcSqlDataType_WVARCHAR(Windows)	*/
		OdbcDataType_BYTE 			= 14,	/* = OdbcSqlDataType_VARBINARY 	*/
		OdbcDataType_TIMESTAMP 		= 15	/* = OdbcSqlDataType_TIMESTAMP 	*/
	};

	// DB 안에서 인식되는 데이터 타입.
	// SQLTYPE 대체.
	enum OdbcSqlDataType
	{
		// 주의: 아래 값들은 ODBC 함수에 직접 사용된다. 따라서 assign된 값을 임의로 바꾸지 말 것.
		OdbcSqlDataType_UNKNOWN 		= SQL_UNKNOWN_TYPE,
		OdbcSqlDataType_CHAR 			= SQL_CHAR,				/* = SQL_C_CHAR */
		OdbcSqlDataType_VARCHAR 		= SQL_VARCHAR,			/* = SQL_C_CHAR */
		OdbcSqlDataType_LONGVARCHAR 	= SQL_LONGVARCHAR,		/* = SQL_C_CHAR */
		OdbcSqlDataType_WCHAR			= SQL_WCHAR,			/* = SQL_C_WCHAR */
		OdbcSqlDataType_WVARCHAR 		= SQL_WVARCHAR,			/* = SQL_C_WCHAR */
		OdbcSqlDataType_WLONGVARCHAR 	= SQL_WLONGVARCHAR,		/* = SQL_C_WCHAR */
		OdbcSqlDataType_BIT 			= SQL_BIT,				/* = SQL_C_BIT */
		OdbcSqlDataType_TINYINT 		= SQL_TINYINT,			/* = SQL_C_TINYINT */
		OdbcSqlDataType_SMALLINT 		= SQL_SMALLINT,			/* = SQL_C_SHORT */
		OdbcSqlDataType_INTEGER 		= SQL_INTEGER,			/* = SQL_C_LONG */
		OdbcSqlDataType_BIGINT 			= SQL_BIGINT,			/* = SQL_C_SBIGINT */
		OdbcSqlDataType_REAL 			= SQL_REAL,				/* = SQL_C_FLOAT */
		OdbcSqlDataType_FLOAT 			= SQL_FLOAT,			/* = SQL_C_DOUBLE */
		OdbcSqlDataType_DOUBLE 			= SQL_DOUBLE,			/* = SQL_C_DOUBLE */
		OdbcSqlDataType_NUMERIC			= SQL_NUMERIC,			/* = SQL_C_DOUBLE */
		OdbcSqlDataType_DECIMAL			= SQL_DECIMAL,			/* = SQL_C_DOUBLE */
		OdbcSqlDataType_BINARY 			= SQL_BINARY,			/* = SQL_C_BINARY */
		OdbcSqlDataType_VARBINARY 		= SQL_VARBINARY,		/* = SQL_C_BINARY */
		OdbcSqlDataType_LONGVARBINARY 	= SQL_LONGVARBINARY,	/* = SQL_C_BINARY */
		OdbcSqlDataType_DATE 			= SQL_DATE,				/* = SQL_C_TIMESTAMP */
		OdbcSqlDataType_TIME 			= SQL_TIME,				/* = SQL_C_TIMESTAMP */
		OdbcSqlDataType_TIMESTAMP 		= SQL_TIMESTAMP			/* = SQL_C_TIMESTAMP */
	};

	enum OdbcPrepareType
	{
		OdbcPrepareType_QUERY 	= 0,
		OdbcPrepareType_PROC 	= 1
	};

	enum OdbcParamType
	{
		OdbcParamType_INPUT = SQL_PARAM_INPUT,
		OdbcParamType_OUTPUT = SQL_PARAM_OUTPUT,
		OdbcParamType_INPUT_OUTPUT = SQL_PARAM_INPUT_OUTPUT
	};
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif