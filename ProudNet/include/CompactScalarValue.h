/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
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

namespace Proud
{
	// srcclr에서 쓰므로 노출시킨 API이지만 사용자가 직접 쓰는건 비추.
	template<typename Type>
	class CCompactScalarValue
	{
		// 1바이트 처리.
		// 내부적으로 사용된다.
		inline void WriteByte(char a)
		{
			char* ptr = (char*)(m_filledBlock + m_filledBlockLength);
			// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka15414.html 에 의하면,
			// GCC는 char* 타입에 대해서 1byte align을 가정하고 작동하게 되어있다.
			// 즉, 컴파일러가 알아서 4byte align에 맞게 조작하게 작동된다.
			*ptr = a;
			m_filledBlockLength += sizeof(char);
		}

		// 1바이트 처리.
		// 내부적으로 사용된다.
		inline bool ReadByte(char &a)
		{
			if (m_extracteeLength + (int)sizeof(a) > m_srcLength)
				return false;

			// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka15414.html 에 의하면,
			// GCC는 char* 타입에 대해서 1byte align을 가정하고 작동하게 되어있다.
			// 즉, 컴파일러가 알아서 4byte align에 맞게 조작하게 작동된다. 
			a = *(char*)(m_src + m_extracteeLength);

			m_extracteeLength += sizeof(a);

			return true;
		}

		uint8_t* m_src;
		int m_srcLength;
	public:
		// MakeBlock에서 사용됨
		uint8_t m_filledBlock[100];
		int m_filledBlockLength;

		// ExtractValue에서 사용됨
		Type m_extractedValue;
		int m_extracteeLength; // read offset

		//////////////////////////////////////////////////////////////////////////
		// 읽기/쓰기 주요 함수

		// 스트림->값
		inline void MakeBlock(Type src)
		{
			m_filledBlockLength = 0;

			bool bNegative;	 // src가 음수인가?
			if (src < 0)
			{
				bNegative = true;
				src = ~src; // 예: -64 => 63
			}
			else
			{
				bNegative = false;
			}

			while (1)
			{
				char oneByte = 0;
				oneByte = src & 0x7f;  // 하위 7비트
				src >>= 7;	// 절대값에서 기록한 비트만큼 제거

				if (src != 0)  // 값을 저장하기 위해 더 많은 비트가 사용되고 있으면
				{
					WriteByte(oneByte | 0x80);  //  다음 내용이 있음을 의미하는 8th bit를 set 후 1바이트 기록. 그리고 다음 턴으로.
				}
				else // 이제 기록할 값이 6 or bit만 있는 경우
				{
					// 기록할 값이 7bit이 경우 1vvv vvvv 0s00 0000를, 6bit인 경우 0svv vvvv를 기록한다.
					if (oneByte & 0x40)
					{
						WriteByte((oneByte | 0x80)); // 7개 비트를 기록한다. 
						oneByte = 0;		// 이제 더 이상 기록할 bit가 없으므로 다음 byte에서는 0개의 zero bit가 들어간다.
					}

						if (bNegative)
						oneByte |= 0x40; // 6th bit는 sign bit이다.

					WriteByte(oneByte); // 기록하고 종료
					break;
				}

			}
		}
		
		// 값->스트림
		inline bool ExtractValue(uint8_t* src, int srcLength)
		{
			m_extracteeLength = 0;
			m_src = src;
			m_srcLength = srcLength;

			char oneByte = 0;
			Type fillee = 0; // 여기에 읽혀진 byte들이 채워짐
			int leftShiftOffset = 0;

			while (1)
			{
				if (m_extracteeLength == 10)
					return false; // int64에서 가능한 최대 크기는 10다. 이를 넘어갔다는 얘기는, 데이터가 뭔가 깨진 것이다.

				if (!ReadByte(oneByte))
					return false;  // 읽기 실패 처리

				if ((oneByte & 0x80) == 0) // 8th bit가 꺼져있으면 뒤에 더 읽을 것이 없다.
				{
					fillee |= (Type(oneByte & 0x3f) << leftShiftOffset); // 6개 하위 비트를 읽어서 다 채움

					if (oneByte & 0x40) // sign bit
						m_extractedValue = ~fillee;
					else
						m_extractedValue = fillee;

					return true;
				}
				else
				{
					// 7개 bit를 읽어 처리하고 나머지를 더 처리하자.
					fillee |= (Type(oneByte & 0x7f) << leftShiftOffset);
					leftShiftOffset += 7;
				}
			}
		}
	};
}