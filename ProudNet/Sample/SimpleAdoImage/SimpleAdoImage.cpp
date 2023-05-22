

//////////////////////////////////////////////////////////////////////////
//SimpleAdoImage
//Written Date:2010.01.04
//Writer:rekfkno1
//Written Purpose: To show, getting and saving Image field of MSSQL by AdoWrap and ByteArray.
//Detail: It currently drawing with GDI_ and only support PNG file format.
//However, You can use other format with assigning rule because Image field is just like buffer.

//작성 날짜:2010.01.04
//작성인:rekfkno1
//작성 목적:MSSQL의 Image필드를 AdoWrap와 ByteArray를 통해 얻고 저장하는것을 보여주기 위함.
//특이점:현재 GDI+를 사용하여 그리기를 수행하고 있으며,PNG파일 포맷만 지원하게 해놓았다.
//하지만, Image필드는 그냥 버퍼나 다름없는 필드이므로 다른 포맷도 약속만 정한다면, 별 문제없이 수행할수 있겠다.
//////////////////////////////////////////////////////////////////////////

#include <map>
#include "../../include/ProudNetCommon.h"
#include "../../include/ProudDB.h"
using namespace Proud;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCWSTR lpszClass = L"SimpleAdoImage";
String strSelect = L"";

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus")
class CGdiPlusStarter
{
private:
	ULONG_PTR m_gpToken;

public:
	bool m_bSuccess;
	CGdiPlusStarter() {
		GdiplusStartupInput gpsi;
		m_bSuccess = (GdiplusStartup(&m_gpToken, &gpsi, NULL) == Ok);
	}
	~CGdiPlusStarter() {
		GdiplusShutdown(m_gpToken);
	}
};
CGdiPlusStarter g_gps;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	if (g_gps.m_bSuccess == FALSE) {
		MessageBox(NULL, L"GDI+ Failed to reset library.",
			L"Caution", MB_OK);
		return 0;
	}

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = L"";
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

typedef std::map<Proud::String, Image*> ImageMap;
ImageMap ImageList;

void AllDeleteImage()
{
	for (ImageMap::iterator i = ImageList.begin(); i != ImageList.end(); ++i)
	{
		if (i->second)
		{
			delete i->second;
		}
	}

	ImageList.clear();
}
void SelectImage()
{
	if (ImageList.size() <= 0)
	{
		strSelect = L"";
		return;
	}

	if (strSelect == L"")
	{
		strSelect = ImageList.begin()->first;
	}
	else
	{
		ImageMap::iterator itr = ImageList.begin();

		for (; itr != ImageList.end(); ++itr)
		{
			if (strSelect == itr->first)
			{
				++itr;
				if (itr == ImageList.end())
				{
					strSelect = ImageList.begin()->first;
					break;
				}

				strSelect = itr->first;
			}
		}
	}


	SetWindowText(hWndMain, strSelect.GetString());
	InvalidateRect(hWndMain, NULL, TRUE);
}
void OnPaint(HDC hdc)
{
	Graphics G(hdc);

	if (strSelect != L"") {
		G.DrawImage(ImageList[strSelect], 0, 0);
	}
}

void OpenFile()
{
	//파일을 열어 'AddImageData'프로시져를 통해 저장하고 있다.
	// Open file and save it through 'AddImageData' procedure.
	OPENFILENAME OFN;
	WCHAR lpstrFile[MAX_PATH] = L"";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWndMain;
	OFN.lpstrFilter = L"PNG file\0*.png\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;

	if (GetOpenFileName(&OFN) != 0)
	{
		HANDLE hFile = CreateFile(lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD filesize = GetFileSize(hFile, NULL);

		BYTE* pByte = new BYTE[filesize];
		DWORD dwRead;
		ReadFile(hFile, pByte, filesize, &dwRead, NULL);

		CloseHandle(hFile);

		AllDeleteImage();

		Proud::ByteArray Array;

		Array.AddRange(pByte, filesize);

		//DB에 저장.
		// Save to DB.
		CAdoConnection conn;
		CAdoCommand cmd;
		conn.Open(L"Data Source=localhost;Database=ProudAdo-Test;Trusted_Connection=yes");

		cmd.Prepare(conn, L"AddImageData");
		cmd.Parameters[1] = lpstrFile;
		cmd.Parameters[2] = Array;

		cmd.Execute();

		long ret = cmd.Parameters[0];

		if (ret < 0)
		{
			MessageBox(NULL, L"Stored Procedure is failed", L"error", MB_OK);
		}

		conn.Close();
	}

}

void ReadImage()
{
	//DB에서 읽어들여 List를 구축한다.
	// Build List reading from DB.
	CAdoConnection conn;
	CAdoRecordset rs;
	conn.Open(L"Data Source=localhost;Database=ProudAdo-Test;Trusted_Connection=yes");

	rs.Open(conn, OpenForReadWrite, L"select * from ImageData");

	AllDeleteImage();

	while (rs.IsEOF() == false)
	{
		Proud::String strName = rs.FieldValues[L"ImageName"];
		ByteArrayPtr Array = rs.FieldValues[L"TestImage"];


		HGLOBAL hBuf = ::GlobalAlloc(GMEM_MOVEABLE, Array.Count);
		void* pBuffer = ::GlobalLock(hBuf);

		if (pBuffer)
		{
			CopyMemory(pBuffer, Array.GetData(), Array.Count);
			IStream* pStream = NULL;

			if (::CreateStreamOnHGlobal(hBuf, FALSE, &pStream) == S_OK)
			{
				Image* pImage = Image::FromStream(pStream);
				//pStream->Release();

				ImageList[strName] = pImage;
			}
			::GlobalUnlock(hBuf);
		}
		::GlobalFree(hBuf);

		rs.MoveNext();
	}

	conn.Close();

	strSelect = L"";
	SelectImage();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
			OpenFile();
			break;
		case VK_F2:
			ReadImage();
			break;
		case VK_RIGHT:
			SelectImage();
			break;
		default:
			break;
		}
		return 0;
	case WM_DESTROY:
		AllDeleteImage();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
