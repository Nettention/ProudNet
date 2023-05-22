#include "stdafx.h"

// Marshaling class
class A
{
public:
	int m_x;
	String m_y;
	double m_z;
};

class B:public A
{
public:
	int m_x2;
	String m_y2;	
};

// Function override for marshaling
namespace Proud 
{
	CMessage& operator<<(CMessage &msg, const A& obj)
	{
		msg<<obj.m_x<<obj.m_y<<obj.m_z;
		return msg;
	}

	CMessage& operator>>(CMessage &msg, A& obj)
	{
		msg>>obj.m_x>>obj.m_y>>obj.m_z;
		return msg;
	}

	void AppendTextOut(String& str, A& obj)
	{
		// It is ok to print member variable itself instead of this
		str += L"<A>";
	}

	// Marshaling function about class B which inherit A
	CMessage& operator<<(CMessage &msg, const B& obj)
	{
		// Members of B
		msg<<obj.m_x2<<obj.m_y2;

		// Excution marshaling about base class
		msg<<(*(A*)&obj);

		return msg;
	}

	CMessage& operator>>(CMessage &msg, B& obj)
	{
		// Members of B
		msg>>obj.m_x2>>obj.m_y2;

		// Excution marshaling about base class
		msg>>(*(A*)&obj);

		return msg;
	}

	void AppendTextOut(String& str, B& obj)
	{
		// It is ok to print member variable itself instead of this
		str += L"<A>";
	}
}

void TestA()
{
	// Create object
	A a;
	a.m_x = 3;
	a.m_y = L"abc";
	a.m_z = 4;

	// Put into message object
	CMessage msg;
	msg.UseInternalBuffer();
	msg<<a;

	// Reset reading point of message object then read object information from message object
	msg.SetReadOffset(0);

	A a2;
	msg>>a2;
	if(a.m_x != a2.m_x || a.m_y != a2.m_y || a.m_z != a2.m_z)
	{
		// Failed to marshaling about class A!
		puts("Marshaling is failed for Class A!");
	}
}

void TestB()
{
	B a;
	a.m_x = 3;
	a.m_y = L"abc";
	a.m_z = 4;
	a.m_x2 = 5;
	a.m_y2 = L"xyz";

	// Put into message object
	CMessage msg;
	msg.UseInternalBuffer();
	msg<<a;

	// Reset reading point of message object then read object information from message object
	msg.SetReadOffset(0);

	B a2;
	msg>>a2;
	if(a.m_x != a2.m_x || a.m_y != a2.m_y || a.m_z != a2.m_z ||
		a.m_x2 != a2.m_x2 || a.m_y2 != a2.m_y2)
	{
		// Failed to marshaling about class B!
		puts("Marshaling is failed for Class B!");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//puts("This sample program tests marshaling function in ProudNet.");
	puts("This sample program tests marshaling function in ProudNet.");
	TestA();
	TestB();

	puts("Test end.");

	return 0;
}
