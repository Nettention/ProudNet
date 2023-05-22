
// stdafx.cpp : Source file that include standard include file.
// SimpleDB2Client.pch become pre-compiled header.
// stdafx.obj is included pre-compiled type information.

#include "stdafx.h"


LPCWSTR SampleNames[] =
{
	L"Edward",
	L"Edwin",
	L"Enoch",
	L"Eugene",
	L"Evelyn",
	L"Ferdinand",
	L"Frances",
	L"Frederick",
	L"Gabriel",
	L"Geoffrey",
	L"George",
	L"Gilbert",
	L"Gregory",
	L"Harold",
	L"Henry",
	L"Herbert",
	L"Harace",
	L"Humphrey",
	L"Issac",
	L"Jacob",
	L"Jerome",
	L"John",
	L"Kenneth",
	L"Lawrence",
	L"Leonard",
	L"Leslie",
	L"Lewi",
	L"Martin",
	L"Matthew",
	L"Nicholas",
	L"Noel",
	L"Oliver",
	L"Oscar",
	L"Oswald",
	L"Owen",
	L"Patricia",
	L"Paul",
	L"Peter",
	L"Philip",
	L"Richard",
	L"Robert",
	L"Roland",
	L"Samuel",
	L"Sabastian",
	L"Theodore",
	L"Thomas",
	L"Vincent",
	L"Vivian",
	L"Wallace",
	L"William",
	L"Agatha",
	L"Agnes",
	L"Angela",
	L"Aileen",
	L"Alice",
	L"Amy",
	L"Beatrice",
	L"Bridget",
	L"Catherine",
	L"Cecil",
	L"Cordelia",
	L"Dorothy",
	L"Elizabeth",
	L"Edith",
	L"Emery",
};

// determine number of elements in an array (not bytes)
#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))


LPCWSTR GetRandomUserName()
{
	int index = CRandom::StaticGetInt() % _COUNTOF(SampleNames);
	return SampleNames[index];
}

