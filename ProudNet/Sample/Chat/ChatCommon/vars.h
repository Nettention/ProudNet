#pragma once

using namespace Proud;

extern int gServerPort;
extern Guid gProtocolVersion;

#define MESSAGE_LOG WM_USER+10
const int lineLength = 60;

void MoveWindowALittle(CWnd* window);

namespace Proud{

	inline void AppendTextOut(String &a, const Proud::HostIDArray &b)
	{
		String f;
		f.Format(L"<Proud::HostIDArray>");
		a += f;
	}
}
