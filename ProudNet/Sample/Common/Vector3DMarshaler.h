#pragma once

#include "Vector3D.h"
#include "../../include/Message.h"

template <typename FloatType>
inline Proud::CMessage& operator>>(Proud::CMessage& a, Vector3T<FloatType>& b)
{
	a.Read(b.x);
	a.Read(b.y);
	a.Read(b.z);
	return a;
}

template <typename FloatType>
inline Proud::CMessage& operator<<(Proud::CMessage& a, const Vector3T<FloatType>& b)
{
	a.Write(b.x);
	a.Write(b.y);
	a.Write(b.z);
	return a;
}

namespace Proud
{
	inline void AppendTextOut(Proud::String & a, const FloatVector3 & b)
	{
		Proud::String f;
		f.Format(_PNT("{x=%f,y=%f,z=%f}"), b.x, b.y, b.z);
		a += f;
	}

	inline void AppendTextOut(Proud::String & a, const Vector3 & b)
	{
		Proud::String f;
		f.Format(_PNT("{x=%lf,y=%lf,z=%lf}"), b.x, b.y, b.z);
		a += f;
	}
}