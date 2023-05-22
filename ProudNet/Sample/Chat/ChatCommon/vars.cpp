#include "StdAfx.h"
#include ".\vars.h"

int gServerPort = 17788;
PNGUID gProtocolVersion0 =
{ 0xafa3c0c, 0x77d7, 0x4b74, { 0x9d, 0xdb, 0x1c, 0xb3, 0xd2, 0x5e, 0x1e, 0x64 } };

Guid gProtocolVersion = Guid(gProtocolVersion0);



// Shift a window to a little to right and down, for easier access while running multiple instances.
void MoveWindowALittle(CWnd* window)
{
	CWnd* desktop = CWnd::GetDesktopWindow();
	CRect desktopRC;
	desktop->GetWindowRect(&desktopRC);

	CRect rc;
	window->GetWindowRect(&rc);
	int rightShift = rand() % (desktopRC.Width() - rc.Width());
	int downShift = rand() % (desktopRC.Height() - rc.Height());
	rc -= CPoint(rc.left, rc.top);
	rc += CPoint(rightShift, downShift);

	window->MoveWindow(rc);
}
