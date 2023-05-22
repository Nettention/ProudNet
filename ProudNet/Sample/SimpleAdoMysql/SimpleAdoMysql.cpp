// SimpleAdo.cpp 2009.12.31 create by rekfkno1
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//MySQL ADOWrap 예제.
//기본적으로 ODBC드라이버를 깔아야 사용 할수 있으며,
//AdoCommand를 사용하는 방식이 약간 다르다.(MsSQL은 Procedure를 Prepare하는 순간 파라메터를 가져오지만,MySQL은 지원하지 않는기능이다.)
//MSSQL과 달리 MySQL은 Procedure에서 Output return을 지원하지 않는다.
//만일 OutPut을 지원 받고 싶다면 Ado.Net를 이용해보시길...

//MySQL ADOWrap Sample.
//Basically you need to install ODBC driver to use it.
//And it is bit different using method instead of AdoCommand.(MsSQL take parameter when it does Prepare Procedure, but MySQL does not support that.)
//MySQL does not support Output return at Procedure rather than MSSQL.
//If you want to get support OutPut then please try Ado.Net...
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "../../include/ProudNetCommon.h"

#include "../../include/CoInit.h"
#include "../../include/AdoWrap.h"

using namespace Proud;

// DB에 연결한다.
// Connect to DB.
void DBConnect(CAdoConnection& conn)
{
	try
	{
		conn.Open(L"Driver={MySQL ODBC 5.3 Unicode Driver}; Server=localhost; port=3306; Database=proudado_test; User ID=root; Password=234234;", MySql);
		wprintf(L"DB Connect Succeed!!\n");

	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}
}

// UserTable 에 값을 삽입한다. ( 쿼리실행 예 )
// Insert value to UserTable. ( Example of query excution )
void InsertUserTable(CAdoConnection& conn, const wchar_t* szUserID, const wchar_t* szPassword, int nCountry)
{
	try
	{
		// 트랜잭션 시작
		// Start transaction
		conn.BeginTrans();

		wprintf(L"Execute Query : insert into UserTable (UserID, Password, Country ) VALUES(%s, %s, %d)....\n", szUserID, szPassword, nCountry);

		// 쿼리를 실행한다.
		// Excute query.
		conn.Execute(String::NewFormat(L"insert into UserTable (UserID, Password, Country) VALUES('%s', '%s', %d)", szUserID, szPassword, nCountry));

		wprintf((L"Succeed to execute query~!!\n"));

		// 트랜잭션 Commit
		// Transaction Commit
		conn.CommitTrans();
	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}
}

// UserTable 의 FieldName을 출력한다.
// Print FieldName of UserTable.
void PrintFieldNameToUserTable(CAdoRecordset& rec)
{
	wprintf(L"===========================================================\n");

	wprintf(L"FieldName1 : %s, FieldName2 : %s, FieldName3 : %s\n",
		((String)rec.GetFieldNames(0)).GetString(),
		((String)rec.GetFieldNames(1)).GetString(),
		((String)rec.GetFieldNames(2)).GetString());

}

// UserTable의 모든 Field의 값을 출력한다.
// Print all Field value of UserTable.
void PrintFieldValueToUserTable(CAdoRecordset& rec)
{

	String strID, strPassword, strCountry;

	wprintf(L"===========================================================\n");

	while (rec.IsEOF() == false)
	{
		strID = rec.FieldValues[L"UserID"];
		strPassword = rec.FieldValues[L"Password"];
		strCountry = rec.FieldValues[L"Country"];

		wprintf(L"UserID : %s, Password : %s, Country : %s\n", strID.GetString(), strPassword.GetString(), strCountry.GetString());

		// 커서를 다음행으로 이동시킨다.
		// Move a cursor to the next line.
		rec.MoveNext();
	}
}

// AddUserData stored procedure를 실행해보자.
// Excute AddUserData stored procedure.
void Excute_AddUserData(CAdoConnection& conn, CAdoRecordset& rec, const wchar_t* szUserID, const wchar_t* szPassword, int nCountry)
{
	CAdoCommand cmd;
	cmd.Prepare(conn, L"AddUserData");
	// MSSQL과 달리 AppendParameter을 호출해야만 한다.
	// Need to call AppendParameter instead of MSSQL.
	cmd.AppendParameter(L"InUserID", ADODB::adVarWChar, ADODB::adParamInput, szUserID, wcslen(szUserID));
	cmd.AppendParameter(L"InPassword", ADODB::adVarWChar, ADODB::adParamInput, szPassword, wcslen(szPassword));
	cmd.AppendParameter(L"InCountry", ADODB::adInteger, ADODB::adParamInput, nCountry);

	cmd.Execute();

	// MySql은 리턴을 지원하지 않음.output파라메터 또한 지원하지 않음.
	// MySql does not support return. Also does not support output parameter too.
	try
	{
		if (!rec.IsOpened())
		{
			// 리턴받은  recordset 객체를 먼저 열어야 접근가능
			// Has to open recordset object to access it
			rec.Open(conn, OpenForRead, L"select * from UserTable");
		}
	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}

	// 다시 출력해보자
	// Print again
	PrintFieldValueToUserTable(rec);

	rec.Close_NoThrow();

}

// GetUserData stored procedure를 실행해보자.
// Excute GetUserData stored procedure.
void Excute_GetUserData(CAdoConnection& conn, CAdoRecordset& rec, const wchar_t* szUserID)
{
	CAdoCommand cmd;
	Proud::String strID, strPassword, strCountry;

	cmd.Prepare(conn, L"GetUserData");
	cmd.AppendParameter(L"InUserID", ADODB::adVarWChar, ADODB::adParamInput, szUserID, wcslen(szUserID));
	cmd.Execute(rec);

	// 다시 출력해보자
	// Print again
	PrintFieldValueToUserTable(rec);

	rec.Close_NoThrow();
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 다국어 출력을 위해 locale를 설정합니다.
	setlocale(LC_ALL, "");

	// Coinitialize가 호출되있어야한다. Proud::CCoInitializer를 사용하면 편리하다.
	// Has to call Coinitialize. It is convenience if you use Proud::CCoInitializer.
	CCoInitializer coi;

	// ADO 연결객체를 생성한다.
	// Create ADO connection object.
	CAdoConnection conn;

	DBConnect(conn);

	wchar_t szUserID[20] = L"nettention";
	wchar_t szPassword[20] = L"1234";
	int nCountry = 11;

	InsertUserTable(conn, szUserID, szPassword, nCountry);

	// recordset을 얻어오기 위한 객체 생성
	// Create object to get recordset
	CAdoRecordset rec;

	// UserTable을 Select 한다.
	// Select UserTable.
	try
	{
		rec.Open(conn, OpenForReadWrite, L"select * from UserTable");
	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}

	// FieldName 출력
	// Print FieldName
	PrintFieldNameToUserTable(rec);

	// FieldValue 출력
	// Print FieldValue
	PrintFieldValueToUserTable(rec);

	rec.Close();

	wcscpy_s(szUserID, L"넷텐션");
	wcscpy_s(szPassword, L"1991pn");
	nCountry = 11;

	// 프로시져 실행
	// Execute procedure
	Excute_AddUserData(conn, rec, szUserID, szPassword, nCountry);

	Excute_GetUserData(conn, rec, szUserID);

	system("PAUSE");

	return 0;
}

