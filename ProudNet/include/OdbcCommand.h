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

#include "OdbcEnum.h"
#include "OdbcException.h"
#include "OdbcVariant.h"
#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class COdbcCommandImpl;
	class COdbcConnection;
	class COdbcRecordset;

	/**
	 \~korean
	 Prepared Statement를 처리하기 위한 클래스 입니다.
	 COdbcConnection의 Execute와는 달리 한 번 설정된 쿼리를 파라미터 값만 변경하여 계속 사용할 때 유용합니다.
	 Input, Output, Input&Output 파라미터가 필요한 Stored Procedure를 실행할 때도 사용 됩니다.

	 \~english
	 It is a class that deals with Prepared Statement.
	 It is useful for continuous use by changing parameter value of a query, differently from Execute in COdbcConnection.
	 Also used when executing Stored Procedure that Input, Output and  Input&Output parameter need. 

	 \~chinese
	 处理Prepared Statement的class。
	 与COdbcConnection的 Execute 不同，只需变更已设定一次的query的参数值，则有利于继续使用。
	 在实行Input, Output, Input&Output 参数所需的 Stored Procedure时，也被使用。

	 \~japanese
	 Prepared Statementを処理するためのクラスです。
	 COdbcConnection의 Executeとは異なり、一度設定されたクエリーをパラメータ値だけ変更して使用し続ける場合に役立ちます。
	 Input, Output, Input&Output パラメータが必要なStored Procedureを実行するときにも使用されます。


	 \~
	 */
	class COdbcCommand
	{
	public:
		 PROUDSRV_API COdbcCommand();
		 PROUDSRV_API ~COdbcCommand();

		/**
		 \~korean
		 쿼리문을 입력할 때 사용합니다.
		 쿼리문의 Value는 ?로 입력합니다. (ex> INSERT INTO test(id, name) VALUES(?, ?))
		 값을 제외한 명령이나 테이블 이름, 필드 이름등은 ?로 입력할 수 없습니다.
		 \param conn COdbcConnection 객체
		 \param query 쿼리문

		 \~english
		 It is used when inputting a query.
		 Input the query as ?. (ex> INSERT INTO test(id, name) VALUES(?, ?)).
		 Table name and field name cannot be inputted as ?. 
		 \param conn COdbcConnection Object
		 \param query Query

		 \~chinese
		 在输入query文件时使用。
		 将query文件的Value输入为？。(ex> INSERT INTO test(id, name) VALUES(?, ?))
		 除了值以外的命令或table名称，field名称等不可输入为？。
		 \param conn COdbcConnection 对象
		 \param query query文件

		 \~japanese
		 クエリー文を入力する時に使用します。.
		 クエリー文の Valueは?と入力します。 (ex> INSERT INTO test(id, name) VALUES(?, ?))
		 値を除いた命令やテーブル名、フィールド名などは?と入力出来ません。
		 \param conn COdbcConnection オブジェクト
		 \param queryクエリー文

		 \~
		 */
		 PROUDSRV_API void Prepare(COdbcConnection &conn, const String& query);

		/**
		 \~korean
		 Input 파라미터를 입력할 때 사용됩니다.
		 변수의 값만 지속적으로 변경하여 실행하고자 할 경우 포인터 형식으로 데이터를 입력하면 됩니다.
		 포인터로 입력할 경우 해당 변수의 Scope에 의해 문제가 발생하지 않는지 확인해야 합니다.
		 1.	int a = 10;
		 2.	cmd.AppendInputParameter(a); // a의 값이 복사되어 Call by Value 형식으로 작동합니다.
		 3.	cmd.AppendInputParameter(&a); // a의 주소값을 사용하는 Call by Pointer 형식으로 작동합니다.

		 입력 데이터에 따라 sqlType이 자동적으로 결정되기 때문에 데이터베이스에 명시적으로 원하는 타입을 넣고 싶을 경우 OdbcSqlDataType을 입력하면 됩니다.
		 ex> int32_t 타입의 변수를 int64_t타입의 데이터로 데이터베이스에 넣고 싶을 경우
		 1.	int a = 10;
		 2.	cmd.AppendInputParameter(a, OdbcSqlDataType_BIGINT);

		 이진 데이터를 입력할 경우 Proud::COdbcByteData 클래스를 이용해야 합니다.
		 COdbcByteData는 포인터로만 입력할 수 있습니다.
		 1. unsigned char byte[10000] = "........................."; // 1000바이트 데이터 입력
		 2. COdbcByteData byteData(byte, 1000, 10000);
		 3. cmd.AppendInputParameter(&byteData);

		 \~english
		 It is used when inputting “Input parameter”.
		 You can just input data in the form of pointer in case you want to change variables only. 
		 In case of inputting in the form of pointer, you must check if there is problem occurred by scope of the relevant variable. 
		 1.         int a = 10;
		 2.         cmd.AppendInputParameter(a); // It is executed in the form of Call by Value as “a” is copied.
		 3.         cmd.AppendInputParameter(&a); // It is executed in the form of Call by Pointer that uses “a” address. 
		 
		 Depending on inputted data, sqlType is automatically decided, so in case you input a type you want in the database, you can input OdbcSqlDataType.
		 ex> In case of inputting variable whose type is int32_t as data whose type is int64_t, 
		 1.         int a = 10;
		 2.         cmd.AppendInputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 In case of inputting binary data, you must use Proud::COdbcByteData class.
		 COdbcByteData is only allowed to be inputted by the pointer. 
		 1. unsigned char byte[10000] = "........................."; // Input 1,000 byte data.
		 2. COdbcByteData byteData(byte, 1000, 10000);
		 3. cmd.AppendInputParameter(&byteData);

		 \~chinese
		 输入 Input参数时使用。
		 如果想一直只变更变数值实行的情况，以pointer的形式输入数据即可。
		 以pointer的形式输入时，需确认因该变数的Scope，是否会发生问题。
		 1.         int a = 10;
		 2.         cmd.AppendInputParameter(a); // a的值被复制，以 Call by Value 形式运行。.
		 3.         以使用cmd.AppendInputParameter(&a); // a地址值的Call by Pointer 形式运行。
		 
		 因随着输入数据的不同，sqlType将自动决定，所以如果想在数据库中明确地放入所需type时，输入OdbcSqlDataType即可。
		 如果想将ex> int32_t type的变数以int64_t type的数据放入数据库时
		 1.         int a = 10;
		 2.         cmd.AppendInputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 输入二进制数据时，应利用 Proud::COdbcByteData class。
		 COdbcByteData只能用pointer输入。
		 1. 输入unsigned char byte[10000] = "........................."; // 1000 byte 数据。
		 2. COdbcByteData byteData(byte, 1000, 10000);
		 3. cmd.AppendInputParameter(&byteData);

		 \~japanese
		 Input パラメータを入力する時に使用されます。
		 変数の値だけ持続的に変更して実行しようとする場合、ポインター形式でデータを入力してください。
		 ポインターで入力する場合、該当変数のScopeによる問題が発生しないか確認する必要があります。
		 1.         int a = 10;
		 2.         cmd.AppendInputParameter(a); // aの値がコピーされCall by Value形式に作動します。
		 3.         cmd.AppendInputParameter(&a); // aのアドレス値を使用するCall by Pointer形式で作動します。

		 入力データによりsqlTypeが自動的に決定されるため、データベースに明示的に望むタイプを入れたい場合にはOdbcSqlDataTypeを入力してください。
		 ex> int32_t タイプの変数をint64_tタイプのデータでデータベースに入れたい場合
		 1.         int a = 10;
		 2.         cmd.AppendInputParameter(a, OdbcSqlDataType_BIGINT);

		 パイナリデータを入力する場合 Proud::COdbcByteData クラスを利用してください。
		 COdbcByteDataはポインターでだけ入力できます。
		 1. unsigned char byte[10000] = "........................."; // 1000バイトデータ入力
		 2. COdbcByteData byteData(byte, 1000, 10000);
		 3. cmd.AppendInputParameter(&byteData);

		 \~
		 */
		 PROUDSRV_API void AppendInputParameter(const COdbcVariant &param, OdbcSqlDataType sqlType = OdbcSqlDataType_UNKNOWN);

		/**
		 \~korean
		 Output 파라미터를 지정할 때 사용됩니다.
		 데이터를 받아야 하므로 포인터 형식으로만 데이터를 입력할 수 있습니다.
		 포인터로 입력할 경우 해당 변수의 Scope에 의해 문제가 발생하지 않는지 확인해야 합니다.
		 1.	int a;
		 2.	cmd.AppendOutputParameter(&a);

		 입력 데이터에 따라 sqlType이 자동적으로 결정되기 때문에 데이터베이스에 명시적으로 원하는 타입을 넣고 싶을 경우 OdbcSqlDataType을 입력하면 됩니다.
		 ex> 데이터베이스로 부터 int64_t 타입의 데이터를 int32_t타입의 변수로 받고 싶을 경우
		 1.	int a;
		 2.	cmd.AppendOutputParameter(a, OdbcSqlDataType_BIGINT);

		 문자열을 받을 경우 반드시 Excute 전후에 Proud::String 클래스의 GetBuffer와 ReleaseBuffer를 사용해야 합니다.
		 1. String str;
		 2. str.GetBuffer(100);
		 3. cmd.AppendOutputParameter(&str);
		 4. cmd.Execute();
		 5. str.ReleaseBuffer();
		 6. str.GetBuffer(100);	// 2, 4, 5를 반복해야 합니다.
		 7. cmd.Execute();
		 8. str.ReleaseBuffer();

		 이진 데이터를 받을 경우 Proud::COdbcByteData 클래스를 사용해야 합니다.
		 COdbcByteData는 포인터로만 입력할 수 있습니다.
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();

		 \~english
		 It is used when designating “Output parameter”.
		 You can only input data in the form of pointer in order to receive data. 
		 In case of inputting in the form of pointer, you must check if there is problem occurred by scope of the relevant variable. 
		 1.         int a;
		 2.         cmd.AppendOutputParameter(&a);
		 
		 Depending on inputted data, sqlType is automatically decided, so in case you input a type you want in the database, you can input OdbcSqlDataType.
		 ex> In case of receiving data whose type is int64_t as variable whose type is int32_t, 
		 1.         int a;
		 2.         cmd.AppendOutputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 In case of receiving string, you must use GetBuffer와 ReleaseBuffer of Proud::String class before/after execution.
		 1. String str;
		 2. str.GetBuffer(100);
		 3. cmd.AppendOutputParameter(&str);
		 4. cmd.Execute();
		 5. str.ReleaseBuffer();
		 6. str.GetBuffer(100);       // You must repeat 2, 4 and 5.
		 7. cmd.Execute();
		 8. str.ReleaseBuffer();
		 
		 In case of receiving binary data, you must use Proud::COdbcByteData class. 
		 COdbcByteData is only allowed to be inputted by the pointer.
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();
		 
		 \~chinese
		 指定Output参数时使用。
		 因需接收数据，所以只能以pointer的形式输入数据。
		 以pointer的形式输入时，需确认因该变数的Scope，是否会发生问题。
		 1.         int a;
		 2.         cmd.AppendOutputParameter(&a);
		 
		 因随着输入数据的不同，sqlType将自动决定，所以如果想在数据库中明确地放入所需type时，输入OdbcSqlDataType即可。
		 如果想将ex> int32_t type的变数以int64_t type的数据放入数据库时
		 1.         int a;
		 2.         cmd.AppendOutputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 接收字符串时，必须在 Excute 前后使用Proud::String class的 GetBuffer和 ReleaseBuffer。
		 1. String str;
		 2. str.GetBuffer(100);
		 3. cmd.AppendOutputParameter(&str);
		 4. cmd.Execute();
		 5. str.ReleaseBuffer();
		 6. str.GetBuffer(100);       // 需反复2, 4, 5
		 7. cmd.Execute();
		 8. str.ReleaseBuffer();
		 
		 接收二进制数据时，需使用 Proud::COdbcByteData class。
		 COdbcByteData只能以pointer的形式输入。
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();

		 \~japanese
		 Output パラメータを指定するとき使用されます。
		 データを受けるためポインター形式でだけデータを入力できます。
		 ポインターで入力する場合、該当変数のScopeによる問題が発生しないか確認する必要があります。
		 1.         int a;
		 2.         cmd.AppendOutputParameter(&a);

		 入力データによりsqlTypeが自動的に決定されるためデータベースに明示的に望むタイプを入れたい場合、OdbcSqlDataTypeを入力してください。
		 ex> データベースからint64_tタイプのデータをint32_tタイプの変数で受けたい場合
		 1.         int a;
		 2.         cmd.AppendOutputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 文字列を受ける場合、必ずExcute前後にProud::StringクラスのGetBufferとReleaseBufferを使用してください。
		 1. String str;
		 2. str.GetBuffer(100);
		 3. cmd.AppendOutputParameter(&str);
		 4. cmd.Execute();
		 5. str.ReleaseBuffer();
		 6. str.GetBuffer(100);       // 2, 4, 5を繰り返してください。
		 7. cmd.Execute();
		 8. str.ReleaseBuffer();
		 
		 パイナリデータを受ける場合Proud::COdbcByteDataクラスを使用してください。
		 CodbcByteDataはポインターでだけ入力できます。
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();

		 \~
		 */
		 PROUDSRV_API void AppendOutputParameter(const COdbcVariant &param, OdbcSqlDataType sqlType = OdbcSqlDataType_UNKNOWN);

		/**
		 \~korean
		 Input&Output 파라미터를 지정할 때 사용됩니다.
		 데이터를 받아야 하므로 포인터 형식으로만 데이터를 입력할 수 있습니다.
		 포인터로 입력할 경우 해당 변수의 Scope에 의해 문제가 발생하지 않는지 확인해야 합니다.
		 1.	int a;
		 2.	cmd.AppendInputOutputParameter(&a);

		 입력 데이터에 따라 sqlType이 자동적으로 결정되기 때문에 데이터베이스에 명시적으로 원하는 타입을 넣고 싶을 경우 OdbcSqlDataType을 입력하면 됩니다.
		 ex> int32_t타입의 변수를 int64_t 타입의 데이터로 데이터베이스로 부터 입출력 하고 싶을 경우
		 1.	int a;
		 2.	cmd.AppendInputOutputParameter(a, OdbcSqlDataType_BIGINT);

		 문자열을 입출력 할 경우 반드시 Excute 전후에 Proud::String 클래스의 GetBuffer와 ReleaseBuffer를 사용해야 합니다.
		 1. String str;
		 2. str = "ABCD";	// 2, 3의 순서가 바뀌면 안됩니다.
		 3. str.GetBuffer(100);
		 4. cmd.AppendInputOutputParameter(&str);
		 5. cmd.Execute();
		 6. str.ReleaseBuffer();
		 7. str = "EFGH"; // 2, 3, 5, 6을 반복해야 합니다.
		 8. str.GetBuffer(100);
		 9. cmd.Execute();
		 10. str.ReleaseBuffer();

		 이진 데이터를 받을 경우 Proud::COdbcByteData 클래스를 사용해야 합니다.
		 COdbcByteData는 포인터로만 입력할 수 있습니다.
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();

		 \~english
		 It is used when designating “Input&Output parameter”.
		 You can only input data in the form of pointer in order to receive data. 
		 In case of inputting in the form of pointer, you must check if there is problem occurred by scope of the relevant variable. 
		 1.         int a;
		 2.         cmd.AppendInputOutputParameter(&a);
		 
		 Depending on inputted data, sqlType is automatically decided, so in case you input a type you want in the database, you can input OdbcSqlDataType.
		 ex> In case of inputting & outputting variable whose type is int32_t as data whose type is int64_t, 
		 1.         int a;
		 2.         cmd.AppendInputOutputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 In case of inputting & outputting string, you must use GetBuffer와 ReleaseBuffer of Proud::String class before/after execution.
		 1. String str;
		 2. str = "ABCD";  // 2 and 3 must be right order. 
		 3. str.GetBuffer(100);
		 4. cmd.AppendInputOutputParameter(&str);
		 5. cmd.Execute();
		 6. str.ReleaseBuffer();
		 7. str = "EFGH"; // You must repeat 2,3,5 and 6. 
		 8. str.GetBuffer(100);
		 9. cmd.Execute();
		 10. str.ReleaseBuffer();
		 
		 In case of receiving binary data, you must use Proud::COdbcByteData class. 
		 COdbcByteData is only allowed to be inputted by the pointer.
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();
		 
		 \~chinese
		 指定Input&Output参数时使用。
		 因需接收数据，所以只能以pointer的形式输入数据。
		 以pointer的形式输入时，需确认因该变数的Scope，是否会发生问题。
		 1.         int a;
		 2.         cmd.AppendInputOutputParameter(&a);
		 
		 因随着输入数据的不同，sqlType将自动决定，所以如果想在数据库中明确地放入所需type时，输入OdbcSqlDataType即可。
		 如果想将ex> int32_t type的变数以int64_t type的数据放入数据库时
		 1.         int a;
		 2.         cmd.AppendInputOutputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 接收字符串时，必须在 Excute 前后使用Proud::String class的 GetBuffer和 ReleaseBuffer。
		 1. String str;
		 2. str = "ABCD";  // 不可调换2, 3的顺序。
		 3. str.GetBuffer(100);
		 4. cmd.AppendInputOutputParameter(&str);
		 5. cmd.Execute();
		 6. str.ReleaseBuffer();
		 7. str = "EFGH"; // 需反复2, 3, 5, 6
		 8. str.GetBuffer(100);
		 9. cmd.Execute();
		 10. str.ReleaseBuffer();
		 
		 接收二进制数据时，需使用 Proud::COdbcByteData class。
		 COdbcByteData只能以pointer的形式输入。
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();

		 \~japanese
		 Input&Output パラメータを指定するときに使用されます。
		 データを受けるため、ポインター形式でだけデータを入力できます。
		 ポインターで入力する場合該当変数のScopeによる問題が発生しないか確認する必要があります。
		 1.         int a;
		 2.         cmd.AppendInputOutputParameter(&a);

		 入力データによりsqlTypeが自動的に決定されるためデータベースに明示的に望むタイプを入れたい場合OdbcSqlDataTypeを入力してください。
		 ex> int32_tタイプの変数をint64_t タイプのデータでデータベースから入出力したい場合
		 1.         int a;
		 2.         cmd.AppendInputOutputParameter(a, OdbcSqlDataType_BIGINT);
		 
		 文字列を入出力する場合必ずExcute前後にProud::StringクラスのGetBufferとReleaseBufferを使用してください。
		 1. String str;
		 2. str = "ABCD";  // 2, 3の順番が変わらないようにご注意ください。
		 3. str.GetBuffer(100);
		 4. cmd.AppendInputOutputParameter(&str);
		 5. cmd.Execute();
		 6. str.ReleaseBuffer();
		 7. str = "EFGH"; // 2, 3, 5, 6を繰り返してください。
		 8. str.GetBuffer(100);
		 9. cmd.Execute();
		 10. str.ReleaseBuffer();
		 
		 パイナリデータを受ける場合Proud::COdbcByteDataクラスを使用する必要があります。
		 COdbcByteDataはポインターでだけ入力できます。
		 1. unsigned char byte[10000];
		 2. COdbcByteData byteData(byte, 0, 10000);
		 3. cmd.AppendOutputParameter(&byteData);
		 4. cmd.Execute();

		 \~
		 */
		 PROUDSRV_API void AppendInputOutputParameter(const COdbcVariant &param, OdbcSqlDataType sqlType = OdbcSqlDataType_UNKNOWN);

		/**
		 \~korean
		 파라미터를 NULL 상태를 설정합니다.
		 DB에 데이터를 NULL로 입력하고 싶을 경우 이 함수를 이용해 NULL 상태를 TRUE로 변경 합니다.
		 한번 설정된 상태는 지속적으로 사용되므로 원 상태로 돌리기 위해선 Execute 후에 FALSE로 다시 세팅해야 합니다.
		 \param paramIndex 파라미터 Index (ex> 1, 2, 3, ...)
		 \param state true or false

		 \~english
		 Set a parameter as NULL.
		 In case you want to input data as NULL in the database, change the status of NULL to TRUE by using this function.
		 Once setting is done, the status is continuously used, so in case of restring the status, you must reset it as FALSE after execution. 
		 \param paramIndex parameter Index (ex> 1, 2, 3, ...)
		 \param state true or false
		 
		 \~chinese
		 将参数设置为 NULL状态。
		 如果想在DB输入NULL数据时，利用该函数将 NULL 状态变更为TRUE状态。
		 因设置一次的状态将一直被使用，所以为了还原，需在 Execute后以FALSE状态重新设置。
		 \param paramIndex 파라미터 Index (ex> 1, 2, 3, ...)
		 \param state true or false

		 \~japanese
		 パラメータを NULL状態を設定します。
		 DBにデータをNULLで入力したい場合、この関数を利用しNULL状態をTRUEに変更します。
		 一度設定された状態は持続的に使用されるため元の状態に戻すためにはExecute後にFALSEでもう一度セッティングする必要があります。
		 \param paramIndex パラメータIndex (ex> 1, 2, 3, ...)
		 \param state true or false

		 \~
		 */
		 PROUDSRV_API void SetNullParameter(unsigned int paramIndex, bool state);

		/**
		 \~korean
		 쿼리를 실행합니다.
		 쿼리 실행으로 영향을 받은 Row 개수를 반환합니다.

		 \~english
		 Execute the query.

		 \~chinese
		 实行query。

		 \~japanese
		 クエリーを実行します。

		 \~
		 */
		 PROUDSRV_API int Execute(COdbcWarnings* odbcWarnings = NULL);

		/**
		 \~korean
		 쿼리를 실행합니다.
		 쿼리 실행으로 영향을 받은 Row 개수를 반환합니다.
		 \param recordset 쿼리 실행후 받아지는 Recordset 객체

		 \~english
		 Execute the query.
		 \param recordset It is a Recordset object that is received after query execution.

		 \~chinese
		 实行query。
		 \param recordset query 实行后接收到的 Recordset 对象

		 \~japanese
		 クエリーを実行します。
		 \param recordset クエリー実行後受けるRecordsetオブジェクト

		 \~
		 */
		 PROUDSRV_API int Execute(COdbcRecordset &recordset, COdbcWarnings* odbcWarnings = NULL);

	private:
		COdbcCommandImpl* m_pImpl;

		// 복사 금지
		PROUDSRV_API COdbcCommand(const COdbcCommand& other);
		COdbcCommand& operator=(const COdbcCommand& other);
	};
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
