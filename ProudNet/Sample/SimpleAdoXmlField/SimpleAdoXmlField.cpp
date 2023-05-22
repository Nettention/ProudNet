// SimpleAdoXmlField.cpp : Define entrance point about console application program.
//

#include "stdafx.h"

#include "../../include/ProudNetCommon.h"

#include "../../include/CoInit.h"
#include "../../include/AdoWrap.h"

using namespace Proud;

// Connect to DB.
void DBConnect(CAdoConnection& conn)
{
	try
	{
		conn.Open(L"Data Source=localhost;Database=ProudAdo-Test;Trusted_Connection=yes");
		wprintf(L"Succeed to connect DB!!\n");

	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}
}

void PrintRecordset(CAdoRecordset& rs)
{
	while (!rs.IsEOF())
	{
		for (int i = 0; i < rs->Fields->Count; ++i)
		{
			String strName = (LPCWSTR)_bstr_t(rs.FieldNames[i]);
			String strValue = rs.FieldValues[i];


			wprintf(L"%s - %s\n", strName.GetString(), strValue.GetString());
		}

		rs.MoveNext();
	}
}

void PrintByQuery(CAdoConnection& conn)
{
	CAdoRecordset rs;

	try
	{
		// Get under Cats.
		rs.Open(conn, OpenForRead, L"select id,xmldat.query('//Cats') as ret from XMLData where id=1");
	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception :%s\n", StringA2W(e.what()).GetString());
	}
	PrintRecordset(rs);

	rs.Close();
}

void PrintByProcedure(CAdoConnection& conn)
{
	CAdoRecordset rs;
	CAdoCommand cmd;
	try
	{
		cmd.Prepare(conn, L"GetXmlData");
		cmd.Parameters[1] = (int)1;
		cmd.Execute(rs);
	}
	catch (AdoException& e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}

	PrintRecordset(rs);

	rs.Close();
}
void PrintAll(CAdoConnection& conn)
{
	CAdoRecordset rs;

	try
	{
		// Get all.
		rs.Open(conn, OpenForRead, L"select * from XMLData where id=1");
	}
	catch (AdoException &e)
	{
		wprintf(L"DB Exception :%s\n", StringA2W(e.what()).GetString());
	}
	PrintRecordset(rs);

	rs.Close();
}

void AddCatsInfo(CAdoConnection& conn, String strValue)
{
	String strQuery;
	try
	{
		strQuery.Format(L"update XMLData set xmldat.modify('insert %s as first into (/Root/Cats)[1]') where id=1", strValue);
		// Add to lower factor of Cats.
		conn.Execute(strQuery);
	}
	catch (AdoException& e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}
}

void DeleteCatsInfo(CAdoConnection& conn, String strValue)
{
	String strQuery;
	try
	{
		strQuery.Format(L"update XMLData set xmldat.modify('delete /Root/Cats/%s') where id=1", strValue);
		// Delete lower factor of Cats.
		conn.Execute(strQuery);
	}
	catch (AdoException& e)
	{
		wprintf(L"DB Exception : %s\n", StringA2W(e.what()).GetString());
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	CCoInitializer coi;

	// Create ADO connection object.
	CAdoConnection conn;

	DBConnect(conn);

	wprintf(L"id=1 Select All");
	PrintAll(conn);

	wprintf(L"Puma Element Add\n");
	AddCatsInfo(conn, L"<Puma>Shoes</Puma>");

	wprintf(L"Print Cats by Query \n");
	PrintByQuery(conn);

	wprintf(L"Puma Element Delete \n");
	DeleteCatsInfo(conn, L"Puma");

	wprintf(L"Print Cats by Procedure\n");
	PrintByProcedure(conn);

	system("PAUSE");

	return 0;
}

