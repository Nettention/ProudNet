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

#include "CompactScalarValue.h"

namespace Proud
{
	//////////////////////////////////////////////////////////////////////////
	// 성능을 많이 먹는 것들을 일로 옮겼음.

	const int StringSerializeMaxLength = 1024 * 1024;	
	 PROUD_API extern const char* ReadOffsetAlignErrorText;
	 PROUD_API extern const char* NullAccessErrorText;


	inline void AdjustReadOffsetByteAlign(CMessage& msg)
	{
		// 비트 단위 읽기를 하던 중이라 읽기 포인트가 byte align이 아니면 조정한다.
		int octetOffset = msg.m_readBitOffset & 7; // mod 8

		msg.m_readBitOffset &= 0xfffffff8; // mod 8만큼을 빼 버림

		if (octetOffset != 0)
			msg.m_readBitOffset += 8;
	}

	inline void AssureReadOffsetByteAlign(CMessage& msg)
	{
		if ((msg.m_readBitOffset & 7) != 0)
		{
			CMessage::ThrowOnWrongLength(ReadOffsetAlignErrorText, (int)strlen(ReadOffsetAlignErrorText), StringSerializeMaxLength);
		}
	}

	inline bool CMessage::Read(uint8_t* data, int count)
	{
		// 아예 읽을게 없으면 splitter test 자체도 제낀다.
		if (count == 0)
			return true;

		AdjustReadOffsetByteAlign(*this);

		if (!Read_NoTestSplitter(data, count))
			return false;

#ifdef _DEBUG
		ReadAndCheckTestSplitterOnCase();
#endif

		return true;
	}

	template<typename POD>
	inline bool CMessage::Read_POD(POD* data)
	{
		AdjustReadOffsetByteAlign(*this);

		if (!Read_NoTestSplitter_POD<POD>(data))
			return false;

#ifdef _DEBUG
		ReadAndCheckTestSplitterOnCase();
#endif
		return true;
	}


	inline void CMessage::Write(const Guid &b)
	{
		const PNGUID* p = &b;
		Write((const uint8_t*)p, sizeof(*p));
	}

	inline bool CMessage::Read(Guid &b)
	{
		PNGUID* p = &b;
		return Read((uint8_t*)p, sizeof(*p));
	}

	inline bool CMessage::CanRead(int count)
	{
		// 아예 읽을게 없으면 splitter test 자체도 제낀다.
		if (count == 0)
			return true;

		int oldReadBitOffset = m_readBitOffset;

		AdjustReadOffsetByteAlign(*this);

		if (!CanRead_NoTestSplitter(count))
		{
			m_readBitOffset = oldReadBitOffset;
			return false;
		}
#ifdef _DEBUG
		ReadAndCheckTestSplitterOnCase();
#endif

		m_readBitOffset = oldReadBitOffset;

		return true;
	}

	inline void CMessage::Write(const AddrPort &b)
	{
		Write((uint8_t*)&b.m_addr, sizeof(b.m_addr));
		Write(b.m_port);
	}



	inline void CMessage::CopyFromArrayAndResetReadOffset(const uint8_t* src, int srcLength)
	{
		m_msgBuffer.Clear();
		m_msgBuffer.AddRange(src, srcLength);
		m_readBitOffset = 0;
	}

	inline CMessage::CMessage()
	{
		Clear();
	}

	// 복사 생성자
	inline CMessage::CMessage(const CMessage &src)
	{
		m_readBitOffset = src.m_readBitOffset;
#ifdef _DEBUG
		m_enableTestSplitter = src.m_enableTestSplitter;
#endif
		m_bitLengthInOneByte = src.m_bitLengthInOneByte;
		m_isSimplePacketMode = src.m_isSimplePacketMode;

		m_msgBuffer = src.m_msgBuffer;
	}

	/*void CMessage::ResetWritePointer()
	{
	m_msgBuffer.Clear();
	}*/

	inline int CMessage::GetWriteOffset()
	{
		return (int)m_msgBuffer.GetCount();
	}

	inline void CMessage::Clear()
	{
		m_readBitOffset = 0;
#ifdef _DEBUG
		m_enableTestSplitter = CNetConfig::EnableTestSplitter;
#endif
		m_bitLengthInOneByte = 0;
		m_isSimplePacketMode = false;

		m_msgBuffer.Clear(); // keeps buffer, however.
	}

	inline bool CMessage::Read(AddrPort &b)
	{
		b = AddrPort::Unassigned;
		return (Read((uint8_t*)&b.m_addr, sizeof(b.m_addr)) == true &&
			Read(b.m_port) == true);
	}


	inline bool CMessage::Read(HostID &b)
	{
		b = HostID_None;
		int x;
		bool r;
		r = Read(x);
		if (r == true)
			b = (HostID)x;
		return r;
	}



	inline void CMessage::Write(NamedAddrPort n)
	{
		WriteString(n.m_addr);
		Write(n.m_port);
	}

	inline bool CMessage::Read(NamedAddrPort &n)
	{
		return ReadString(n.m_addr) && Read(n.m_port);
	}



	inline void CMessage::SetReadOffset(int offset)
	{
		if (offset > (int)m_msgBuffer.GetCount())
		{
			ThrowReadOffsetOutOfRangeException(offset);
		}
		m_readBitOffset = offset * 8;
	}


	inline void CMessage::UseInternalBuffer()
	{
		/* Q: CMessage ctor를 냅두고 이 함수가 따로 있나요?
		A: global object pool에서 가져오는 과정이 무조건 수행되면 느리잖아요.t
		Q: 그러면, 최초의 write 가 발생하면 JIT로 하면 되잖아요?
		A: Aㅏ..... TODO로 남깁시다 ㅋㅋㅋ */
		m_msgBuffer.UseInternalBuffer();
		m_msgBuffer.SetGrowPolicy(GrowPolicy_HighSpeed);
		m_msgBuffer.SetMinCapacity(CNetConfig::MessageMinLength);
	}

	inline void CMessage::WriteString(const char* str)
	{
		WriteStringA(str);
	}

	inline void CMessage::WriteString(const wchar_t* str)
	{
		WriteStringW(str);
	}

	inline void CMessage::WriteString(const StringA &str)
	{
		WriteStringA(str.GetString());
	}

	inline void CMessage::WriteString(const StringW &str)
	{
		WriteStringW(str.GetString());
	}

	inline bool CMessage::ReadString(StringA &str)
	{
		return ReadStringA(str);
	}

	inline bool CMessage::ReadString(StringW &str)
	{
		return ReadStringW(str);
	}


	inline void CMessage::SetLength(int count)
	{
		if (m_readBitOffset > count * 8)
		{
			m_readBitOffset = count * 8; // 어쨌거나 크기 조절하기를 시행.
		}

		m_msgBuffer.SetCount(count);
	}



	/// 읽은 것처럼 구라친다.
	inline bool CMessage::SkipRead(int count)
	{
#ifdef _DEBUG
		ReadAndCheckTestSplitterOnCase();
#endif

		if (m_msgBuffer.IsNull())
		{
			ThrowOnWrongLength(NullAccessErrorText, (int)strlen(NullAccessErrorText), StringSerializeMaxLength);
		}

		if (GetReadOffset() + count > (int)m_msgBuffer.GetCount())
			return false;
		// memcpy(data,m_msgBuffer.GetData()+m_readOffset,count); 이것만 commented out.
		m_readBitOffset += count * 8;

		return true;

	}


	inline bool CMessage::Read_NoTestSplitter(uint8_t* data, int count)
	{
		if (m_msgBuffer.IsNull())
		{
			ThrowOnWrongLength(NullAccessErrorText, (int)strlen(NullAccessErrorText), StringSerializeMaxLength);
		}

		AssureReadOffsetByteAlign(*this);

		if (GetReadOffset() + count > (int)m_msgBuffer.GetCount())
			return false;

		// ikpil.choi 2016-11-07 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
		//UnsafeFastMemcpy(data, m_msgBuffer.GetData() + GetReadOffset(), count);
		memcpy_s(data, count, m_msgBuffer.GetData() + GetReadOffset(), count);
		m_readBitOffset += count * 8;

		return true;
	}

	template<typename POD>
	inline bool CMessage::Read_NoTestSplitter_POD(POD* data)
	{
		const int length = sizeof(POD); // 상수. 성능 위해.
		const int bitLength = sizeof(POD) * 8; // 상수. 성능 위해.

		if (m_msgBuffer.IsNull())
		{
			ThrowOnWrongLength(NullAccessErrorText, (int)strlen(NullAccessErrorText), StringSerializeMaxLength);
		}

		AssureReadOffsetByteAlign(*this);

		// cache it
		int readOffset = GetReadOffset();

		if (readOffset + length > (int)m_msgBuffer.GetCount())
			return false;

		// 하기 if에 대해서는 Write_NoTestSplitter_POD에 설명이 있다.
#if defined(__ANDROID__) || defined(__arm__) // RVCT를 쓰는 경우 __packed keyword를 쓰면 되지만 우리는 __packed 를 못 쓰는 컴파일러(gcc or clang)을 쓰고 있다.
		memcpy(data, m_msgBuffer.GetData() + readOffset, length);		// size값에 상수를 넣는다. 그리고 clang이 여기를 코드최적화를 제대로 해줘야 한다. 그래서 memcpy_s를 안쓴다.
#else
		*data = *((POD*)(m_msgBuffer.GetData() + readOffset));
#endif
		m_readBitOffset += bitLength;

		return true;
	}


	inline bool CMessage::CanRead_NoTestSplitter(int count)
	{
		if (m_msgBuffer.IsNull())
		{
			ThrowOnWrongLength(NullAccessErrorText, (int)strlen(NullAccessErrorText), StringSerializeMaxLength);
		}

		int oldReadBitOffset = m_readBitOffset;

		AssureReadOffsetByteAlign(*this);

		if (GetReadOffset() + count > (int)m_msgBuffer.GetCount())
		{
			m_readBitOffset = oldReadBitOffset;
			return false;
		}

		m_readBitOffset = oldReadBitOffset;

		return true;
	}

#ifdef _DEBUG
	inline void CMessage::ReadAndCheckTestSplitterOnCase()
	{
		if (m_enableTestSplitter)
		{
			uint8_t s;
			if (!Read_NoTestSplitter(&s, sizeof(s)) || s != Splitter)
			{
				ThrowWrongSplitterException();
			}
		}
	}

	inline void CMessage::EnableTestSplitter(bool enable)
	{
		m_enableTestSplitter = enable;
	}
#endif

	inline bool CMessage::ReadWithShareBuffer(CMessage& output, int length)
	{
		if (GetLength() < GetReadOffset() + length)
			return false;

		uint8_t* contentPtr = GetData() + GetReadOffset();

		//CMessage에 담긴 데이터 자체가 없을 때를 위한 방어 코드.
		//이 함수는 해당 객체의 메모리 버퍼를 output이 length 크기만큼 읽어가는 기능을 수행함. 읽을 데이터가 없는 상황에서 호출되면 안됨.
		if (GetData() == NULL)
			return false;

		// 포인터 연산이 잘못 되었을 경우의 방어 코드. 포인터 연산 후 결과는 절대로 null이 될 수 없다.
		if (contentPtr == NULL)
			return false;

		output.UseExternalBuffer(contentPtr, length);
		output.SetLength(length); // 이게 있어야 함!
		SkipRead(length);

		return true;
	}


	inline void CMessage::AppendByteArray(const uint8_t* fragment, int fragmentLength)
	{
		m_msgBuffer.AddRange(fragment, fragmentLength);
	}

	template<typename Type>
	inline bool Message_ReadScalar(CMessage& msg, Type &a)
	{
		if (msg.IsSimplePacketMode())
		{
			int64_t a0; // VLQ에 대해서는, 사용자가 어떤 타입을 넣을지 무시하고 쓰므로
			bool ok = msg.Read(a0); 
			if (ok)
			{
				a = (Type)a0; // 크기가 작더라도 무시하자.
			}
			return ok;
		}
		else
		{
			CCompactScalarValue<Type> val;
			int readOffset = msg.GetReadOffset();
			if (!val.ExtractValue(msg.GetData() + readOffset, msg.GetLength() - readOffset))
				return false;

			a = val.m_extractedValue;
			msg.SkipRead(val.m_extracteeLength);

			return true;
		}
	}

	inline bool CMessage::ReadScalar(int64_t &a)
	{
		return Message_ReadScalar<int64_t>(*this, a);
	}

	inline bool CMessage::ReadScalar(int32_t &a)
	{
		return Message_ReadScalar<int32_t>(*this, a);
	}

	inline bool CMessage::ReadScalar(int16_t &a)
	{
		return Message_ReadScalar<int16_t>(*this, a);
	}

	inline bool CMessage::ReadScalar(int8_t &a)
	{
		return Message_ReadScalar<int8_t>(*this, a);
	}

	template <typename Type>
	inline void Message_WriteScalar(CMessage& msg, Type a)
	{
		if (msg.IsSimplePacketMode())
		{
			msg.Write((int64_t)a); // VLQ에 대해서는, 사용자가 어떤 타입을 넣을지 무시하고 쓰므로
		}
		else
		{
			// 변환된 메모리 블럭 생성
			CCompactScalarValue<Type> comp;
			comp.MakeBlock(a);

			// 이를 메시지 객체에 넣기
			msg.Write(comp.m_filledBlock, comp.m_filledBlockLength);
		}
	}

	inline void CMessage::WriteScalar(int64_t a)
	{
		Message_WriteScalar<int64_t>(*this, a);
	}

	inline void CMessage::WriteScalar(int32_t a)
	{
		Message_WriteScalar<int32_t>(*this, a);
	}

	inline void CMessage::WriteScalar(int16_t a)
	{
		Message_WriteScalar<int16_t>(*this, a);
	}

	inline void CMessage::WriteScalar(int8_t a)
	{
		Message_WriteScalar<int8_t>(*this, a);
	}

	inline void CMessage::UninitBuffer()
	{
		m_readBitOffset = 0;
		m_bitLengthInOneByte = 0;
		m_msgBuffer.UninitBuffer();
	}

#ifdef _DEBUG
	inline CMessageTestSplitterTemporaryDisabler::CMessageTestSplitterTemporaryDisabler(CMessage& msg)
	{
		m_msg = &msg;
		m_oldVal = m_msg->IsTestSplitterEnabled();
		m_msg->EnableTestSplitter(false);
	}

	inline CMessageTestSplitterTemporaryDisabler::~CMessageTestSplitterTemporaryDisabler()
	{
		m_msg->EnableTestSplitter(m_oldVal);
	}
#endif

	// 이 함수를 호출하면 simple packet mode가 켜지거나 꺼진다.
	// read/write scalar 등을 비활성화하는데 사용된다.
	inline void CMessage::SetSimplePacketMode(bool isSimplePacketMode)
	{
		m_isSimplePacketMode = isSimplePacketMode;
	}

	// 다른 버퍼 객체와 포인터를 공유하기 시작하면서 읽기 위치를 리셋한다.
	inline void CMessage::ShareFromAndResetReadOffset(ByteArrayPtr data)
	{
		m_msgBuffer = data;
		m_readBitOffset = 0;
	}

	inline void CMessage::UseExternalBuffer(uint8_t* buf, int capacity)
	{
		// 		if (capacity > CNetConfig::MessageMaxLength)
		// 			throw Exception("UseExternalBuffer failed due to too large capacity");

		m_msgBuffer.UseExternalBuffer(buf, capacity);
	}





}
