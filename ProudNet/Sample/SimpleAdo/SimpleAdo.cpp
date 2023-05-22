// SimpleAdo.cpp 2009.10.20 create by ulelio
//

#include "stdafx.h"
#include "../../include/AdoWrap.h"
#include "../../include/ProudNetServer.h"
#include "../../include/CoInit.h"

using namespace Proud;
// DB에 연결한다.
// Connect to DB.
void DBConnect( CAdoConnection& conn )
{
	try
	{
		conn.Open(L"Data Source=localhost;Database=ProudAdo-Test;Trusted_Connection=yes");
		wprintf( L"Succeed to connect DB!!\n");
	}
	catch(AdoException &e)
	{
		wprintf( L"DB Exception : %s\n", Proud::StringA2W(e.what()).GetString());
	}

}

// UserData 에 값을 삽입한다. ( 쿼리실행 예 )
// Insert value to UserData. ( Example of query excution )
void InsertUserTable( CAdoConnection& conn, const wchar_t* szUserID, const wchar_t* szPassword, int nCountry )
{
	try
	{
		// 트랜잭션 시작 
		// Start transaction 
		conn.BeginTrans();

		wprintf( L"Execute Query : Insert into UserData (UserID, Password, Country ) VALUES(%s, %s, %d)....\n", szUserID, szPassword, nCountry );

		// 쿼리를 실행한다.
		// Excute query.
		conn.Execute(String::NewFormat(L"insert into UserData (UserID, Password, Country) VALUES(%s, %s, %d)", szUserID, szPassword, nCountry));

		wprintf(( L"Succeed to execute Query~!!\n") );

		// 트랜잭션 Commit
		// transaction Commit
		conn.CommitTrans();
	}
	catch (AdoException &e)
	{
		wprintf( L"DB Exception : %s\n", Proud::StringA2W(e.what()).GetString());
	}
}

// UserData 의 FieldName을 출력한다.
// Print FieldName of UserData.
void PrintFieldNameToUserTable( CAdoRecordset& rec )
{
	wprintf( L"===========================================================\n" );

	wprintf( L"FieldName1 : %s, FieldName2 : %s, FieldName3 : %s\n", 
			((String)rec.GetFieldNames(0)).GetString(), 
			((String)rec.GetFieldNames(1)).GetString(),
			((String)rec.GetFieldNames(2)).GetString() );

}

// UserData의 모든 Field의 값을 출력한다.
// Print all Field value of UserData.
void PrintFieldValueToUserTable( CAdoRecordset& rec )
{

	StringA strID, strPassword, strCountry;

	wprintf( L"===========================================================\n" );

	while( rec.IsEOF() == false )
	{
		strID = StringA((char*)((_bstr_t)rec.FieldValues[L"UserID"].m_val));
		strID = CW2A(((String)rec.FieldValues[L"UserID"]).GetString());
		strPassword  = CW2A(((String)rec.FieldValues[ L"Password"]).GetString());
		strCountry = CW2A(((String)rec.FieldValues[ L"Country"]).GetString());

		printf( "UserID : %s, Password : %s, Country : %s\n", strID.GetString(), strPassword.GetString(), strCountry.GetString() );
		
		// 커서를 다음행으로 이동시킨다.
		// Move a cursor to next line.
		rec.MoveNext();	
	}
}

// AddUserData stored procedure를 실행해보자.
// Excute AddUserData stored procedure.
void Excute_AddUserData( CAdoConnection& conn, CAdoRecordset& rec, const wchar_t* szUserID, const wchar_t* szPassword, int nCountry )
{
	CAdoCommand cmd;
	StringA szUserIDA = (LPSTR)CW2A(szUserID);
	cmd.Prepare( conn, L"AddUserData" );
	cmd.Parameters[1] = szUserIDA.GetString();
	cmd.Parameters[2] = szPassword;
	cmd.Parameters[3] = nCountry;

	cmd.Execute(rec);
	long ret = cmd.Parameters[0];

	// 프로시져가 실패하면 -1을 리턴함.
	// Return -1 when procedure is failed.
	if( ret < 0 )
	{
		wprintf( L"Stored Procedure is failed!!\n" );
	}
	else
	{
		try
		{
			if(!rec.IsOpened())
			{
				rec.Open( conn, OpenForRead, L"select * from UserData" ); // 리턴받은  recordset 객체를 먼저 열어야 접근가능
			}
		}
		catch (AdoException &e)
		{
			wprintf( L"DB Exception : %s\n", Proud::StringA2W(e.what()).GetString());
		}

		// 다시 출력해보자
		// Print again
		PrintFieldValueToUserTable( rec );
	}
}

/*void TestTimeField(CAdoConnection &conn)
{
	CAdoRecordset rs;
	rs.Open(conn, OpenForRead, _PNT("select * from TimeTest"));
	auto d1 = rs.GetFieldValue(_PNT("TimeType1"));
	auto d2 = rs.GetFieldValue(_PNT("TimeType2"));
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	// Coinitialize가 호출되있어야한다. Proud::CCoInitializer를 사용하면 편리하다.
	// Coinitialize has to called. It is convenience if you use Proud::CCoInitializer.
	CCoInitializer coi;

	// ADO 연결객체를 생성한다.
	// Create ADO connect object.
	CAdoConnection conn;

	DBConnect( conn );

//	TestTimeField(conn);

	wchar_t szUserID[20] = L"'ulelio'";
	wchar_t szPassword[20] = L"'1234'";
	int nCountry = 11;

	InsertUserTable( conn, szUserID, szPassword, nCountry );

	// recordset을 얻어오기 위한 객체 생성
	// Create object to get recordset
	CAdoRecordset rec;

	// UserData을 Select 한다.
	// Select UserData
	try
	{
		rec.Open( conn, OpenForReadWrite, L"select * from UserData" );
	}
	catch (AdoException &e)
	{
		wprintf( L"DB Exception : %s\n", Proud::StringA2W(e.what()).GetString());
	}

	// FieldName 출력
	// Print FieldName
	PrintFieldNameToUserTable( rec );

	// FieldValue 출력
	// Print FieldValue
	PrintFieldValueToUserTable( rec );

	rec.Close();

	wcscpy_s( szUserID, L"ulelio1" );
	wcscpy_s( szPassword, L"1234a" );
	nCountry = 11;

	// 프로시져 실행
	// Execute procedure
	Excute_AddUserData(conn, rec, szUserID, szPassword, nCountry);

	system("PAUSE");

	return 0;
}

