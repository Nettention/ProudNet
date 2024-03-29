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

#include "OdbcEnum.h"
#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class COdbcVariant;
	class COdbcRecordsetImpl;
	class COdbcConnection;

	/**
	 \~korean
	 SELECT를 통해 얻어진 Recordset을 제어할 수 있는 클래스입니다.

	 \~english
	 It is a class that can control Recordset that has been received by SELECT.

	 \~chinese
	 可以防御通过SELECT获取的Recordset的class。

	 \~japanese
	 SELECTのため取得されたRecordsetをコントロールできるクラスです。

	 \~
	 */
	class COdbcRecordset
	{
	public:
		PROUDSRV_API COdbcRecordset();
		PROUDSRV_API ~COdbcRecordset();

		/**
		 \~korean
		 첫번째 레코드로 이동합니다.
		 성공/실패를 반환합니다.
		 Stored Procedure의 Recordset에서는 사용할 수 없습니다.

		 \~english
		 Move to the first record.
		 Return success/fail
		 You cannot use it in the Recordset of the stored procedure.

		 \~chinese
		 移动到第一个记录。
		 返回成/失败。
		 在Stored Procedure的Recordset中不能使用。

		 \~japanese
		 最初のレコードに移動します。
		 成功/失敗を返還します。
		 Stored ProcedureのRecordsetでは使用できません。

		 \~
		 */
		PROUDSRV_API  bool MoveFirst();
		/**
		 \~korean
		 마지막 레코드로 이동합니다.
		 성공/실패를 반환합니다.
		 Stored Procedure의 Recordset에서는 사용할 수 없습니다.

		 \~english
		 Move to the last record
		 Return success/fail
		 You cannot use it in the Recordset of the stored procedure.

		 \~chinese
		 移动到最后一个记录。
		 返回成/失败。
		 在Stored Procedure的Recordset中不能使用。

		 \~japanese
		 最後のレコードに移動します。
		 成功/失敗を返還します。
		 Stored ProcedureのRecordsetでは使用できません。

		 \~
		 */
		PROUDSRV_API  bool MoveLast();
		/**
		 \~korean
		 이전 레코드로 이동합니다.
		 성공/실패를 반환합니다.
		 Stored Procedure의 Recordset에서는 사용할 수 없습니다.

		 \~english
		 Move to the previous record.
		 Return success/fail
		 You cannot use it in the Recordset of the stored procedure.

		 \~chinese
		 移动到前一个记录。
		 返回成/失败。
		 在Stored Procedure的Recordset中不能使用。

		 \~japanese
		 以前のレコードに移動します。
		 成功/失敗を返還します。
		 Stored ProcedureのRecordsetでは使用できません。

		 \~
		 */
		PROUDSRV_API  bool MovePrev();

		/**
		 \~korean
		 다음 레코드로 이동합니다.
		 성공/실패를 반환합니다.
		 데이터를 얻을 때 이 함수를 먼저 호출해야만 얻을 수 있습니다.

		 \~english
		 Move to the next record.
		 Return success/fail
		 You cannot use it in the Recordset of the stored procedure.

		 \~chinese
		 移动到下一个记录。
		 返回成/失败。
		 只有事先调用函数才能获得数据。

		 \~japanese
		 次のレコードに移動します。
		 成功/失敗を返還します。
		 データを得る時は、この関数を先に呼び出してこそ得ることができます。

		 \~
		 */
		PROUDSRV_API bool MoveNext();

		/**
		 \~korean
		 여러개의 레코드셋을 가져오는 경우 이 함수를 통해 다음 레코드셋으로 전환할 수 있습니다.
		 성공/실패를 반환합니다.

		 \~english
		 When you bring many recordsets, you can change to the next recordset using this function.
		 Return success/fail

		 \~chinese
		 获取多个 RecordSet 时，可以通过该函数转换为下一个 RecordSet。
		 在获取多个记录集时可通过此函数返回成功/失败。

		 \~japanese
		複数のレコードセットを取り込む場合、この関数を通じ、次のレコードセットに切り替えられます。
		成功/失敗を返還します。

		 \~
		 */
		PROUDSRV_API  bool NextRecordSet();

		/**
		 \~korean
		 전체 Row 개수를 반환합니다.

		 \~english
		 Return the total number of Row.

		 \~chinese
		 返还所有 Row个数。

		 \~japanese
		 全体Row数を返還します。

		 \~
		 */
		PROUDSRV_API  SQLLEN GetRowCount();
		/**
		 \~korean
		 현재 Row Index를 반환합니다. (ex> 0, 1, 2, ...)
		 Row Index가 -1이면 BOF 상태이며, Row Count와 같다면 EOF 상태입니다.

		 \~english
		 Return the current Row Index (ex> 0, 1, 2, ...).
		 If Row Index is -1, its status is BOF and if it is same as Row Count, its status is EOF.

		 \~chinese
		 当前返还Row Index。(ex> 0, 1, 2, ...)
		 Row Index 若是 -1则是 BOF状态,若与Row Count相同则是 EOF 状态。

		 \~japanese
		 現在Row Indexを返還します。(ex> 0, 1, 2, ...)
		 Row Indexが-1ならBOF状態で、Row Countと同じならEOF状態です。

		 \~
		 */
		PROUDSRV_API  SQLLEN GetCurrentRowIndex();
		/**
		 \~korean
		 전체 Field 개수를 반환합니다.

		 \~english
		 Return the total number of Fields.

		 \~chinese
		 返还所有Field个数。

		 \~japanese
		 全体Field 個数を返還します。

		 \~
		 */
		PROUDSRV_API  SQLSMALLINT GetFieldCount();

		/**
		 \~korean
		 BOF는 Index == -1일 때 이며 아직 읽은 데이터가 없는 상태입니다.

		 \~english
		 BOF indicates Index == -1 and it means there is no data that has been read.

		 \~chinese
		 BOF为 Index == -1 时，是未读数据的状态。

		 \~japanese
		 BOFはIndex == -1の時で、まだ未読のデータがない状態です。

		 \~
		 */
		PROUDSRV_API  bool IsBof();
		/**
		 \~korean
		 EOF는 Index == GetRowCount()일 때 이며 데이터를 모두 읽은 상태입니다.

		 \~english
		 EOF indicates Index == GetRowCount() and it means all data has been read.

		 \~chinese
		 EOF为 Index == GetRowCount()时，是已读所有数据的状态。

		 \~japanese
		 EOFはIndex == GetRowCount()の時であり、全てのデータが既読の状態です。

		 \~
		 */
		PROUDSRV_API  bool IsEof();

		/**
		 \~korean
		 Field(Column)의 이름을 얻어 옵니다.
		 "SELECT COUNT(*) FROM TABLE"과 같이 컬럼명이 정해지지 않은 쿼리에서는 정상 작동하지 않을 수 있습니다.
		 그럴 땐 컬럼명에 Alias를 설정하여 "SELECT COUNT(*) AS TOTAL FROM TABLE"과 같이 사용하시면 됩니다.

		 \~english
		 Get the name of Field(Column).
		 It might not be executed normally at a query whose column name has not been defined, such as "SELECT COUNT(*) FROM TABLE".
		 In this case, set Alias in the column name  and use it like "SELECT COUNT(*) AS TOTAL FROM TABLE".

		 \~chinese
		 获取Field(Column)名称。
		 与"SELECT COUNT(*) FROM TABLE"相同，在Column Name未定的query中，有可能无法正常运行。
		 这时在Column Name设定Alias后，与"SELECT COUNT(*) AS TOTAL FROM TABLE"相同方法使用即可。

		 \~japanese
		 Field(Column)の名を取得します。
		 "SELECT COUNT(*) FROM TABLE"のように Column名が決められていないクエリーでは正常作動しない場合もあります。
		 その場合にはColumn名にAliasを設定し"SELECT COUNT(*) AS TOTAL FROM TABLE"のように使用してください。

		 \~
		 */
		PROUDSRV_API  String GetFieldName(int fieldIndex);

		/**
		 \~korean
		 Field(Column)의 SqlDataType을 얻어 옵니다.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~english
		 Get SqlDataType of Field(Column).
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~chinese
		 获取Field(Column)的 SqlDataType。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~japanese
		 Field(Column)のSqlDataTypeを取得します。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~
		 */
		PROUDSRV_API  OdbcSqlDataType GetFieldSqlType(int fieldIndex);
		/**
		 \~korean
		 Field(Column)의 SqlDataType을 얻어 옵니다.
		 \param fieldName Field(Column) 이름

		 \~english
		 Get SqlDataType of Field(Column).
		 \param fieldname It is the name of Field(Column).

		 \~chinese
		 获取Field(Column)的 SqlDataType
		 \param fieldName Field(Column) 名称。

		 \~japanese
		 Field(Column)のSqlDataTypeを取得します。
		 \param fieldName Field(Column) 名

		 \~
		 */
		PROUDSRV_API  OdbcSqlDataType GetFieldSqlType(const String& fieldName);

		/**
		 \~korean
		 Field(Column) 데이터의 현재 크기를 가져옵니다.
		 NULL일 경우 -1을 반환 합니다.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~english
		 Bring the current size of Field(Column) data.
		 In case of NULL, it returns -1.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~chinese
		 获取当前大小的Field(Column) 数据。
		 如果是NULL的情况，返还 -1。.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~japanese
		 Field(Column)データの現在サイズをロードします。
		 NULLの場合 -1を返還します。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~
		 */
		PROUDSRV_API  SQLLEN GetFieldSizeOrNull(int fieldIndex);
		/**
		 \~korean
		 Field(Column) 데이터의 현재 크기를 가져옵니다.
		 NULL일 경우 -1을 반환 합니다.
		 \param fieldName Field(Column) 이름

		 \~english
		 Bring the current size of Field(Column) data.
		 In case of NULL, it returns -1.
		 \param fieldname It is the name of Field(Column).

		 \~chinese
		 获取当前大小的Field(Column) 数据。
		 如果是NULL的情况，返还 -1。.
		 \param fieldName Field(Column)名称

		 \~japanese
		 Field(Column) データの現在サイズをロードします。
		 NULLの場合 -1を返還します。
		 \param fieldName Field(Column) 名

		 \~
		 */
		PROUDSRV_API  SQLLEN GetFieldSizeOrNull(const String& fieldName);

		/**
		 \~korean
		 Field(Column) 데이터의 최대 크기(데이터베이스 설정)를 가져옵니다.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~english
		 Bring the maximum size (Database setting) of Field(Column) data.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~chinese
		 获取Field(Column)数据的最大值（数据库设定）。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~japanese
		 Field(Column) データの最大サイズ（データベース設定）をロードします。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~
		 */
		PROUDSRV_API  SQLULEN GetFieldMaxSize(int fieldIndex);
		/**
		 \~korean
		 Field(Column) 데이터의 최대 크기(데이터베이스 설정)를 가져옵니다.
		 \param fieldName Field(Column) 이름

		 \~english
		 Bring the maximum size (Database setting) of Field(Column) data.
		 \param fieldname It is the name of Field(Column).

		 \~chinese
		 获取 Field(Column)数据的最大值（数据库设定）。
		 \param fieldName Field(Column) 名称

		 \~japanese
		 Field(Column) データの最大サイズ（データベース設定）をロードします。
		 \param fieldName Field(Column) 名

		 \~
		 */
		SQLULEN GetFieldMaxSize(const String& fieldName);

		/**
		 \~korean
		 Field(Column) 데이터의 소수점 아래 정밀도(자리수)를 가져옵니다.
		 Real, Float, Double, Timestamp와 같은 형식에서 사용합니다.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~english
		 Bring a decimal place of Field(Column) data.
		 It is used at types like Real, Float, Double and Timestamp.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~chinese
		 获取Field(Column)数据的小数点以下的精密度 （位数）。
		 在与Real, Float, Double, Timestamp相同的形式下使用。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~japanese
		 Field(Column) データの小数点以下精密度（数の桁）をロードします。
		 Real, Float, Double, Timestampのような形式で使用されます。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~
		 */
		PROUDSRV_API  SQLSMALLINT GetFieldPrecision(int fieldIndex);
		/**
		 \~korean
		 Field(Column) 데이터의 소수점 아래 정밀도(자리수)를 가져옵니다.
		 Real, Float, Double, Timestamp와 같은 형식에서 사용합니다.
		 \param fieldName Field(Column) 이름

		 \~english
		 Bring a decimal place of Field(Column) data.
		 It is used at types like Real, Float, Double and Timestamp.
		 \param fieldname It is the name of Field(Column).

		 \~chinese
		 获取Field(Column)数据的小数点以下的精密度 （位数）。
		 在与Real, Float, Double, Timestamp相同的形式下使用。
		 \param fieldName Field(Column)名称

		 \~japanese
		 Field(Column) データの小数点以下精密度（数の桁）をロードします。
		 Real, Float, Double, Timestampのような形式で使用します。
		 \param fieldName Field(Column) 名

		 \~
		 */
		SQLSMALLINT GetFieldPrecision(const String& fieldName);

		/**
		 \~korean
		 Field(Column) 데이터로 NULL을 입력할 수 있는지(데이터베이스 설정)를 반환합니다.
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~english
		 Return whether or not it can input NULL by Field(Column) data (Database setting).
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~chinese
		 返还Field(Column) 数据是否可以输入 NULL（数据库设定）
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~japanese
		 Field(Column) データでNULLを入力できるか（データベース設定）を返還します。
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~
		 */
		PROUDSRV_API  bool GetFieldNullable(int fieldIndex);
		/**
		 \~korean
		 Field(Column) 데이터로 NULL을 입력할 수 있는지(데이터베이스 설정)를 반환합니다.
		 \param fieldName Field(Column) 이름

		 \~english
		 Return whether or not it can input NULL by Field(Column) data (Database setting).
		 \param fieldname It is the name of Field(Column).

		 \~chinese
		 返还Field(Column) 数据是否可以输入 NULL（数据库设定）
		 \param fieldName Field(Column)名称

		 \~japanese
		 Field(Column) データでNULLを入力できるか（データベース設定）を返還します。
		 \param fieldName Field(Column) 名

		 \~
		 */
		bool GetFieldNullable(const String& fieldName);

		/**
		 \~korean
		 Field(Column) 데이터를 반환합니다.
		 데이터가 어떤 형인지 모를 경우 GetFieldSqlType()를 이용하여 알 수 있습니다.
		 OdbcSqlDataType과 매핑되는 Default OdbcDataType은 OdbcEnum.h를 참고 하십시오.
		 ex> int a = recordset.GetFieldValue(1);
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~english
		 Return Field(Column) data.
		 In case you do not know the type of data, use GetFieldSqlType() to get to know.
		 Refer to OdbcEnum.h for Default OdbcDataType that is mapped with OdbcSqlDataType.
		 ex> int a = recordset.GetFieldValue(1);
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~chinese
		 返还Field(Column)数据。
		 不知道数据是什么形式的情况下，可以利用 GetFieldSqlType()知道。
		 OdbcSqlDataType和 mapping Default OdbcDataType 请参考 OdbcEnum.h。
		 ex> int a = recordset.GetFieldValue(1);
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~japanese
		 Field(Column) データを返還します。
		 データ型を知らない場合GetFieldSqlType()を利用して知ることができます。
		 OdbcSqlDataTypeとマッピングできるDefault OdbcDataTypeはOdbcEnum.hをご参考ください。
		 ex> int a = recordset.GetFieldValue(1);
		 \param fieldIndex Field(Column) Index (ex> 1, 2, 3, ...)

		 \~
		 */
		PROUDSRV_API const COdbcVariant& GetFieldValue(int fieldIndex);
		/**
		 \~korean
		 Field(Column) 데이터를 반환합니다.
		 데이터가 어떤 형인지 모를 경우 GetFieldSqlType()를 이용하여 알 수 있습니다.
		 OdbcSqlDataType과 매핑되는 Default OdbcDataType은 OdbcEnum.h를 참고 하십시오.
		 ex> int a = recordset.GetFieldValue("name");
		 \param fieldName Field(Column) 이름

		 \~english
		 Return Field(Column) data.
		 In case you do not know the type of data, use GetFieldSqlType() to get to know.
		 Refer to OdbcEnum.h for Default OdbcDataType that is mapped with OdbcSqlDataType.
		 ex> int a = recordset.GetFieldValue("name");
		 \param fieldname It is the name of Field(Column).

		 \~chinese
		 返还Field(Column)数据。
		 不知道数据是什么形式的情况下，可以利用 GetFieldSqlType()知道。
		 OdbcSqlDataType和 mapping Default OdbcDataType 请参考 OdbcEnum.h。
		 ex> int a = recordset.GetFieldValue("name");
		 \param fieldName Field(Column) 名称

		 \~japanese
		 Field(Column) データを返還します。
		 データ型を知らない場合GetFieldSqlType()を利用して知ることができます。
		 OdbcSqlDataTypeとマッピングされるDefault OdbcDataTypeは OdbcEnum.hをご参考ください。
		 ex> int a = recordset.GetFieldValue("name");
		 \param fieldName Field(Column) 名

		 \~
		 */
		PROUDSRV_API const COdbcVariant& GetFieldValue(const String& fieldName);

		// WARNING::내부에서 사용되는 함수입니다. 사용하지 마시기 바랍니다.
		// WARNING::Internal use only. Don't use this function.
		PROUDSRV_API  void SetEnvironment(COdbcConnection* conn, void *hstmt, const SQLLEN &rowcount);

	private:
		COdbcRecordsetImpl * m_pImpl;

		PROUDSRV_API COdbcRecordset(const COdbcRecordset& other);
		COdbcRecordset& operator=(const COdbcRecordset& other);
	};
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
