/*
ProudNet 1.8.58849-master


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

#if defined(_WIN32)
//#include <string>
#endif
#include <string>
#include "FastArray.h"
#include "FastArrayPtr.h"
#include "AddrPort.h"
#include "EncryptEnum.h"
#include "FakeClr.h"
#include "HostIDArray.h"
#include "NetConfig.h"
#include "Enums.h"
#include "BasicTypes.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
	/** \addtogroup net_group
	*  @{
	*/
	
	class CMessage;
	class NetVariant;
	struct AddrPort;

	/** 
	\~korean
	메시지 클래스. 자세한 내용은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#message_class" >메시지 객체(Proud.CMessage)</a>를 참고.

	\~english 
	Message class. For detailed information, refer to \ref message_class

	\~chinese
	信息类。详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#message_class" >信息客体(Proud.CMessage)</a>%。

	\~japanese
	メッセージクラス。詳しくは、\ref message_classをご参照ください。
	\~
	*/
	class  CMessage
	{
		// 현재 읽기 커서 위치. 바이트가 아니라 비트다!
		int m_readBitOffset;
		// 기록되어 있는 총 데이터 크기 중, 맨 마지막 바이트 중 몇번째 비트까지 기록되어있는지를 가리킨다. 
		// 비트단위 write를 할 때 증가한다.
		int m_bitLengthInOneByte; 

		// 이 값이 true이면 read/write scalar가 비활성되어 무조건 int64로 기재되는 등,
		// 패킷 구조는 사용자가 알 수 있도록 단순화되되 필요한 효율적인 기능들이 사라진다.
		bool m_isSimplePacketMode;
	public:
		inline bool IsSimplePacketMode() const { return m_isSimplePacketMode; }
	private:
		friend void AdjustReadOffsetByteAlign(CMessage& msg);
		friend void AssureReadOffsetByteAlign(CMessage& msg);

		 PROUD_API static void ThrowOnWrongLength(const char* where, int length, int maxLength);
	public:
		

		template<typename T, bool RAWTYPE, typename COLLT>
		static inline bool ReadArrayT(CMessage &a, COLLT &b)
		{
			int length;
			if (a.ReadScalar(length) == false)
				return false;
			
			// count가 해킹되어 말도 안되는 값이면 실패 처리하기
			// 물론 모든 경우를 잡지는 못하지만 (sizeof elem 무용지물) 그래도 최소 1바이트는 쓸테니.
			if (length < 0 || length > a.GetLength() - a.GetReadOffset())
				return false;

			b.SetCount(length);

			if(RAWTYPE)
			{
				return a.Read((uint8_t*)b.GetData(), sizeof(T) * length);
			}
			else 
			{
				for (int i = 0; i < length; i++)
				{
					if (a.Read(b[i]) == false)
						return false;
				}
				return true;
			}
		}

		template<typename T, bool RAWTYPE, typename COLLT>
		static inline void WriteArrayT(CMessage &a, const COLLT &b)
		{
			int length = (int)b.GetCount();
			a.WriteScalar(length);

			if(RAWTYPE)
				a.Write((uint8_t*)b.GetData(), sizeof(T) * length);
			else
			{
				for (int i = 0; i < length; i++)
					a.Write(b[i]);
			}
		}

	public:
		ByteArrayPtr m_msgBuffer;

		inline CMessage();
		inline CMessage(const CMessage &src);

		inline bool DataEquals(const CMessage& rhs)
		{
			return m_msgBuffer.Equals(rhs.m_msgBuffer);
		}
		/** 
		\~korean
		메시지로부터 데이터를 읽는다.
		\param data 읽은 데이터가 저장될 곳
		\param count data의 바이트 단위 크기 
		\return count만큼 성공적으로 읽었으면 true.

		\~english 
		Read data from the message. 
		\param data Where the read data will be stored. 
		\param count Byte unit size of the data
		\return True if successfully read as the count


		\~chinese
		从信息读取数据。
		\param data 所读取的数据储存的位置。
		\param count data的byte单位大小。 
		\return 读到最后的话true。

		\~japanese
		メッセージからデータを読み取ります。
		\param data読み取ったデータが保存される所です。
		\param count dataのバイト単位のサイズ
		\return countだけ読み取ることに成功したらtrueです。
		\~
		*/
		inline  bool Read( uint8_t* data, int count );

		template<typename POD>
		inline  bool Read_POD(POD* data);

		inline  bool CanRead( int count );
		
		/** 
		\~korean
		메시지에 데이터를 기록한다.

		\~english 
		Record data in the message
		

		\~chinese
		记录信息数据。

		\~japanese
		メッセージにデータを記録します。
		\~
		*/
		inline void Write(const uint8_t* data, int count)
		{
			// 아예 쓸게 없으면 splitter test 자체도 제낀다.
			if (count == 0)
				return;

			AdjustWriteOffsetByteAlign();

			Write_NoTestSplitter(data, count);

#ifdef _DEBUG
			WriteTestSplitterOnCase();
#endif
		}

		template<typename POD>
		inline void Write_POD(const POD* data)
		{
			AdjustWriteOffsetByteAlign();

			Write_NoTestSplitter_POD<POD>(data);
#ifdef _DEBUG
			WriteTestSplitterOnCase();
#endif
		}

		inline  void CopyFromArrayAndResetReadOffset(const uint8_t* src, int srcLength);
		// void ResetWritePointer();

		/** 
		\~korean
		읽기 지점을 강제 조정한다. 바이트 단위이다.

		\~english 
		Read point is forcibly adjusted. It is in bytes. 

		\~chinese
		强制调整读取地点。Byte 单位。

		\~japanese
		読み取り地点を強制に調整します。バイト単位です。
		\~
		*/
		inline  void SetReadOffset(int offset);

		// 사용자는 이 함수를 호출하지 말 것
		inline  void SetSimplePacketMode(bool isSimplePacketMode);

		/** 
		\~korean
		메시지의 현재 크기를 설정한다.

		\~english 
		Set the current size of the message. 

		\~chinese
		设置信息现在的大小。

		\~japanese
		メッセージの現在サイズを設定します。
		\~
		*/
		inline  void SetLength(int count);
		inline   int GetWriteOffset();

		//////////////////////////////////////////////////////////////////////////
		// Test splitter 기능
		// 성능에 민감하므로, release build에서는 제거되어 있음.
#ifdef _DEBUG
		inline  void EnableTestSplitter(bool enable);
		inline  bool IsTestSplitterEnabled() {
			return m_enableTestSplitter;
		}
	private:
		bool m_enableTestSplitter;
		enum { Splitter = 254 };

		inline void ReadAndCheckTestSplitterOnCase();
		inline void WriteTestSplitterOnCase()
		{
			if (m_enableTestSplitter)
			{
				uint8_t s = Splitter;
				Write_NoTestSplitter(&s, sizeof(s));
			}
		}
#endif

		//////////////////////////////////////////////////////////////////////////

		inline  bool Read_NoTestSplitter(uint8_t* data, int count);

		template<typename POD>
		inline  bool Read_NoTestSplitter_POD(POD* data);

		inline  bool CanRead_NoTestSplitter(int count);
		inline void Write_NoTestSplitter( const uint8_t* data, int count )
		{
			m_msgBuffer.MustNotNull();

			m_msgBuffer.AddRange(data, count);
		}

		template<typename POD>
		inline void Write_NoTestSplitter_POD(const POD* data)
		{
			int oldLength = m_msgBuffer.GetCount();
			m_msgBuffer.AddCount(sizeof(POD)); // 크기를 늘리고
#if defined(__ANDROID__) || defined(__arm__) // RVCT를 쓰는 경우 __packed keyword를 쓰면 되지만 우리는 __packed 를 못 쓰는 컴파일러(gcc or clang)을 쓰고 있다.
			// ARM에서는 4byte align이 아닌 데이터에 바로 = 를 쓰면 크래시가 난다. 
			// 따라서 memcpy를 써주어야 한다. 내부적으로 non-4byte align을 처리하기 때문이다.
			// 물론 느리다. 하지만 모바일 클라이언트가 이걸로 문제되지는 않는다.
			memcpy(m_msgBuffer.GetData() + oldLength, data, sizeof(POD));		// size값에 상수를 넣는다. 그리고 clang이 여기를 코드최적화를 제대로 해줘야 한다. 그래서 memcpy_s를 안쓴다.
#else
			// x86 계열에서는 하기 명령어가 non 4byte align이라도 잘 작동한다. 느려서 그렇지.
			*((POD*)(m_msgBuffer.GetData() + oldLength)) = *data; // 복사
#endif
		}

		inline void AdjustWriteOffsetByteAlign()
		{
			// 비트 단위 기록을 하던 중이라 쓰기 포인트가 byte align이 아니면, 조정한다.
			// 단, 이것을 호출 후에는 반드시 최소 1 바이트를 기록해야 한다. 안그러면 기록하던 비트들은 유실된다.
			m_bitLengthInOneByte = 0;
		}

	public:
		/** 
		\~korean
		이 객체의 메모리 버퍼를 output이 length 크기만큼 읽어가는 양상으로 만들되,
		output은 외부로서 이 객체의 버퍼를 참조하도록 만든다. 
		성능을 위해 사용되는 함수다.

		\~english 
		Make the memory buffer of this object as if the output reads as much as the size of the length, the output should be made to refer the buffer of this object. 
		This is the function used for performance.

		\~chinese
		把此对象的内存buffer制造成output读取相当于length大小的局面，但是output作为外部制造成参考此对象的buffer。
		为了性能使用的函数。

		\~japanese
		このオブジェクトのメモリーバッファーをoutputがlengthサイズだけ読み取るように作るが、outputは外部としてこのオブジェクトのバッファーを参照するように作ります。
		性能のために使われる関数です。
		\~
		*/
		inline  bool ReadWithShareBuffer(CMessage& output, int length);

		/** 
		\~korean
		메시지 객체에 데이터를 추가 기록한다.
		bool 타입 뿐만 아니라 다양한 타입을 지원한다.

		\~english 
		Additionally record the data in the message object. 
		It supports not only Boolean type but also various types. 

		\~chinese
		往信息对象添加记录数据。
		不只是bool类型，还支持各种类型。

		\~japanese
		メッセージオブジェクトにデータを追加記録します。
		Boolタイプだけではなく、多様なタイプをサポートします。
		\~
		*/
		inline void Write(bool b) {
			Write_POD<bool>(&b);
		}
		inline void Write(int8_t b) {
			Write_POD<int8_t>(&b);
		}
		inline void Write(uint8_t b) {
			Write_POD<uint8_t>(&b);
		}
		inline void Write(int16_t b) {
			Write_POD<int16_t>(&b);
		}
		inline void Write(uint16_t b) {
			Write_POD<uint16_t>(&b);
		}
		inline void Write(int32_t b) {
			Write_POD<int32_t>(&b);
		}
		inline void Write(uint32_t b) {
			Write_POD<uint32_t>(&b);
		}
		/* Q: 왜 long type에 대해서는 Write & Read가 없나요?
		A: long, unsigned long은 몇몇 컴파일러에서 크기가 서로 다릅니다. 
		VC++ x64에서는 32bit이지만, 다른 컴파일러 중 하나는 x64에서 64bit로 처리됩니다.
		이러면 네트워크로 주고받는 데이터 타입으로 부적절입니다.
		대신 int64_t, uint64_t를 쓰시기 바랍니다. 아니면 (unsigned)int or (u)int32_t를 쓰시던지.
		*/
		inline void Write(int64_t b) {
			Write_POD<int64_t>(&b);
		}
		inline void Write(uint64_t b) {
			Write_POD<uint64_t>(&b);
		}
		inline void Write(float b) {
			Write_POD<float>(&b);
		}
		inline void Write(double b) {
			Write_POD<double>(&b);
		}
		inline void Write(HostID b) {
			Write_POD<HostID>(&b);
		}

		inline  void Write(const AddrPort &b);
		inline  bool Read(AddrPort &b);
		
		bool Read(ByteArray &b)
		{
			return ReadArrayT<uint8_t, true, ByteArray>(*this,b);
		}
		void Write(const ByteArray &b)
		{
			WriteArrayT<uint8_t,true,ByteArray>(*this,b);
		}

		void Write(ByteArrayPtr b)
		{
			// b는 buffer usage를 미리 지정했어야 함
			WriteArrayT<uint8_t,true,ByteArrayPtr>(*this,b);
		}

		bool Read(ByteArrayPtr &b)
		{
			// b는 buffer usage를 미리 지정했어야 함
			return ReadArrayT<uint8_t, true, ByteArrayPtr>(*this,b);
		}


		/** 
		\~korean
		scalar compression 기법으로 값을 읽는다.

		\~english
		Reads with scalar compression technique

		\~chinese
		用scalar compression技术读入值。

		\~japanese
		scalar compression 技法で値を読み取ります。
		\~
		 */
		inline  bool ReadScalar(int64_t &a);
		inline  bool ReadScalar(int32_t &a);
		inline  bool ReadScalar(int16_t &a);
		inline  bool ReadScalar(int8_t &a);
		inline  bool ReadScalar(uint64_t &a) { return ReadScalar((int64_t&)a); }
		inline  bool ReadScalar(uint32_t &a) { return ReadScalar((int32_t&)a); }
		inline  bool ReadScalar(uint16_t &a) { return ReadScalar((int16_t&)a); }
		inline  bool ReadScalar(uint8_t &a) { return ReadScalar((int8_t&)a); }

		/** 
		\~korean
		scalar compression 기법으로 값을 기록한다. 

		\~english
		Writes with scalar compression technique

		\~chinese
		用scalar compression技术记录值。

		\~japanese
		scalar compression 技法で値を記録します。 
		\~
		*/
		inline  void WriteScalar(int64_t a);
		inline  void WriteScalar(int32_t a);
		inline  void WriteScalar(int16_t a);
		inline  void WriteScalar(int8_t a);
		inline  void WriteScalar(uint64_t a) { WriteScalar((int64_t)a); }
		inline  void WriteScalar(uint32_t a) { WriteScalar((int32_t)a); }
		inline  void WriteScalar(uint16_t a) { WriteScalar((int16_t)a); }
		inline  void WriteScalar(uint8_t a) { WriteScalar((int8_t)a); }

		//bool Read(out CFastArray<BYTE> b)
		//{
		//	b = new CFastArray<BYTE>();
		//	int length;
		//	if (Read(out length) == false)
		//		return false;

		//	for (int i = 0; i < length; i++)
		//	{
		//		BYTE p = 0;
		//		if (Read(out p) == false)
		//			return false;
		//		b.Add(p);
		//	}

		//	return true;
		//}

		//void Write(CFastArray<BYTE> b)
		//{
		//	int length = b.GetCount();
		//	Write(length);
		//	for (int i = 0; i < length; i++)
		//	{
		//		Write(b[i]);
		//	}
		//}

		// bool ReadFrameNumberArray(CFastArrayPtr<int,true> &b);
		// void WriteFrameNumberArray(const int* arr, int count);

		/** 
		\~korean
		메시지 객체에 데이터를 추가 기록한다.
		bool 타입 뿐만 아니라 다양한 타입을 지원한다. 
		\return 완전히 읽는데 성공하면 true 

		\~english
		Additionally writes data to message object
		Suuports various types including bool type 
		\return True if successful in thorough reading

		\~chinese
		往信息对象添加记录数据。
		不仅是bool类型，支持各种类型。
		\return 完全的读入成功的话true

		\~japanese
		メッセージオブジェクトにデータを追加記録します。
		boolタイプだけではなく、多様なタイプをサポートします。
		\return 完全に読み取ることに成功したらtrueです。
		\~
		*/
		inline bool Read(bool &b) {
			return Read_POD<bool>(&b);
		}
		inline bool Read(int8_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(uint8_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(int16_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(uint16_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(int32_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(uint32_t &b) {
			return Read_POD(&b);
		}
		//bool Read(unsigned &b) { return CMessage_RawRead(*this,b); }
		inline bool Read(int64_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(uint64_t &b) {
			return Read_POD(&b);
		}
		inline bool Read(float &b) {
			return Read_POD(&b);
		}
		inline bool Read(double &b) {
			return Read_POD(&b);
		}

		inline void Write(const Guid &b);
		inline bool Read(Guid &b);

		inline  bool Read(HostID &b);

		//void ClonedTo(CMessage_RawRead()
		//{
		//	BYTE[] o = new BYTE[m_buffer.GetCount()];
		//	m_buffer.CopyTo(o);
		//	return o;
		//}

		inline  void Write(NamedAddrPort n);
		inline  bool Read(NamedAddrPort &n);

// 		 void WriteMessageContent(const CMessage &msg);
// 		 bool ReadMessageContent(CMessage &msg);

		/** 
		\~korean
		메시지의 길이를 얻는다. 

		\~english
		Gets the length of message

		\~chinese
		获取信息的长度。

		\~japanese
		メッセージの長さを得ます。
		\~
		*/
		inline int GetLength() const
		{
			return (int)m_msgBuffer.GetCount();
		}

		/** 
		\~korean
		메시지의 현재 읽기 지점을 구한다. 리턴값은 바이트 단위다.

		\~english
		Calculates current read point of message. The return value is in byte. 

		\~chinese
		求信息的现在读取地点。返回值是byte单位。

		\~japanese
		メッセージの現在の読み取り地点を求めます。リターン値はバイト単位です。
		\~
		 */
		inline int GetReadOffset() const
		{
			return m_readBitOffset >> 3; // 나누기 8. signed도 >>연산자가 잘 작동하는구마이!
		}

		inline void AppendByteArray(const uint8_t* fragment, int fragmentLength);

		inline uint8_t* GetData()
		{
			return m_msgBuffer.GetData();
		}

		inline const uint8_t* GetData() const
		{
			return m_msgBuffer.GetData();
		}

		template<typename BYTEARRAY>
		inline void CopyToT(BYTEARRAY& dest)
		{
			dest.SetCount(GetLength());

			// ikpil.choi 2016-11-07 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
			// GetCount() 가 1 byte 의 것인지 어떻게 보장 할 것인가?
			//UnsafeFastMemcpy(dest.GetData(), GetData(), GetLength());
			memcpy_s(dest.GetData(), dest.TYPE_SIZE * dest.GetCount(), GetData(), GetLength());
		}

		inline void CopyTo(ByteArray& dest)
		{
			CopyToT<ByteArray>(dest);
		}
		inline void CopyTo(ByteArrayPtr& dest)
		{
			CopyToT<ByteArrayPtr>(dest);
		}
		inline void CopyTo(CMessage& dest)
		{
			CopyToT<ByteArrayPtr>(dest.m_msgBuffer);
			dest.m_readBitOffset = m_readBitOffset;
			dest.m_isSimplePacketMode = m_isSimplePacketMode;
		}

		/*		 void CloneTo(CMessage& dest);
		 ByteArrayPtr ToByteArray();
		 void ToByteArray(ByteArray &ret);
		 void CopyTo(ByteArray& dest); */

		// called by BiasManagedPointer.
		void Clear();

		inline  void ShareFromAndResetReadOffset(ByteArrayPtr data);

		/** 
		\~korean
		외부 버퍼를 사용하도록 선언한다.
		\param buf 외부 버퍼 포인터
		\param capacity 외부 버퍼의 크기

		\~english 
		Declare to use an external buffer
		\param buf External buffer pointer
		\param capacity Size of an external buffer

		\~chinese
		宣言使用外部buffer。
		\param buf 外部buffer指针。
		\param capacity 外部buffer大小。

		\~japanese
		外部バッファーを使うように宣言します。
		\param buf 外部バッファーポインター
		param capacity 外部バッファーサイズ
		\~
		 */
		inline  void UseExternalBuffer(uint8_t* buf, int capacity);
		
		/** 
		\~korean
		내부 버퍼를 사용하도록 선언한다.

		\~english
		Declares to use internal buffer

		\~chinese
		宣言使用内部buffer。

		\~japanese
		内部バッファーを使うように宣言します。
		\~
		 */
		inline  void UseInternalBuffer();

		/** 
		\~korean
		사용하던 버퍼를 완전히 리셋하고, 새로 외부 또는 내부 버퍼를 사용하도록 선언한다.
		- UseExternalBuffer, UseInternalBuffer를 재사용하려면 반드시 이것을 호출해야 한다.
		이러한 메서드가 따로 있는 이유는 사용자가 실수하는 것을 방지하기 위해서다. 
		- 이 메서드 호출 후에는 read offset과 메시지 크기가 초기화된다.

		\~english
		Completely resets the  void UseExternalBuffer(BYTE* buf, int capacity)r
		- This must be called in order to re-use UseExternalBuffer and UseInternalBuffer.
		  The reasone why this kind of method exists is to prevent for user to make mistakes.
		- After this method is called, the size of read offset and message will be initialized.

		\~chinese
		完全复位使用的buffer，宣言重新使用外部或者内部buffer。
		- 想再使用UseExternalBuffer, UseInternalBuffer的话必须呼叫这个。
		另有这种方法的理由是为了防止用户失误。
		- 呼叫此方法呼叫以后会初始化read offset和信息大小。

		\~japanese
		使っていたバッファーを完全にリセットして、新たに外部または内部バッファーを使うように宣言します。
		- UseExternalBuffer, UseInternalBufferを再使用するためには、必ずこれを呼び出してください。
		このようなメソッドが別にある理由は、ユーザーの間違いを防止するためです。
		- このメソッドを呼び出した後には、read offsetとメッセージサイズが初期化されます。

		 */
		inline  void UninitBuffer();

		/**
		\~korean
		현재 읽은 데이터 크기에 count를 더한다.

		\~english 
		Add count to the data size read currently. 

		\~chinese
		往现在所读数据大小上添加count。

		\~japanese
		現在読み取ったデータサイズにcountを加えます。
		\~
		*/
		inline  bool SkipRead(int count);

		/**
		\~korean
		문자열 넣기

		\~english
		Write string

		\~chinese
		输入字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  void WriteString(const char* str);

		/**
		\~korean
		문자열 넣기

		\~english
		Write string

		\~chinese
		输入字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  void WriteString(const wchar_t* str);

		/**
		\~korean
		문자열 넣기

		\~english
		Write string

		\~chinese
		输入字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  void WriteString(const Proud::StringA &str);

		/**
		\~korean
		문자열 넣기

		\~english
		Write string

		\~chinese
		输入字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  void WriteString(const Proud::StringW &str);
		
		/**
		\~korean
		문자열 넣기

		\~english
		Write string

		\~chinese
		输入字符串。

		\~japanese
		\~
		*/
		inline  void WriteString(const std::string &str)
		{
			WriteString(str.c_str());
		}

		/**
		\~korean
		문자열 넣기

		\~english
		Write string

		\~chinese
		输入字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  void WriteString(const std::wstring &str)
		{	
			WriteString(str.c_str());
		}

		/**
		\~korean
		문자열 꺼내기

		\~english
		Read string

		\~chinese
		导出字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  bool ReadString(Proud::StringA &str);

		/**
		\~korean
		문자열 꺼내기

		\~english
		Read string

		\~chinese
		导出字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  bool ReadString(Proud::StringW &str);
		
		/**
		\~korean
		문자열 꺼내기

		\~english
		Read string

		\~chinese
		导出字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline bool ReadString(std::string &str)
		{
			StringA str2;
			bool ret = ReadString(str2);
			if(ret)
			{
				str = str2;
			}
			return ret;
		}

		/**
		\~korean
		문자열 꺼내기

		\~english
		Read string 

		\~chinese
		导出字符串。

		\~japanese
		文字列を入れる
		\~
		*/
		inline  bool ReadString(std::wstring &str)
		{
			StringW str2;
			bool ret = ReadString(str2);
			if(ret)
			{
				str = str2;
			}
			return ret;
		}

		 PROUD_API bool ReadStringA(Proud::StringA &str);
		 PROUD_API bool ReadStringW(Proud::StringW &str);
		 PROUD_API void WriteStringA(const char* str);
		 PROUD_API void WriteStringW(const wchar_t* str);

		//////////////////////////////////////////////////////////////////////////

		 bool ReadVariant(NetVariant& var);
		 void WriteVariant(const NetVariant& var);

		////////////////////////////////////////////////////////////////////////////

	private:

		 PROUD_API void ThrowReadOffsetOutOfRangeException(int offset);
		 PROUD_API void ThrowWrongSplitterException();

	};

	//// 로컬 변수로 잠깐 쓸 경우 유용=>더 이상 안씀. UseInternalBuffer자체가 obj-pool기능으로 인해 relloc횟수가 점차 줄어드니까.
	//class CSendingMessage: public CMessage
	//{
	//public:
	//	CSendingMessage()
	//	{
	//		UseInternalBuffer();
	//		// 미리 공간을 확 확보한 후 다시 해제해버린다. 이렇게 해서 재할당 횟수를 0으로 만든다.
	//		// 예전에는 Proud::CNetConfig::MessageMaxLength를 넣었으나, 이것을 넣으면 CRT heap을 직접 접근해버리므로 캐삽질이 된다.
	//		// 따라서 고속 할당을 하는 CFastHeap::DefaultAccelBlockSizeLimit를 써야 한다.
	//		SetLength(CFastHeap::DefaultAccelBlockSizeLimit);
	//		SetLength(0);
	//	}
	//};

	// ------- 이건 더 이상 안쓴다. ProudNetConfig::MessageMaxLength를 런타임에서 수정할 수 있어야 하니까.
	// class CSendingMessage: public CMessage 
	// {
	// 	BYTE m_stackBuffer[CNetConfig::MessageMaxLength];
	// 
	// public:
	// 	CSendingMessage()
	// 	{
	// 		UseExternalBuffer(m_stackBuffer, sizeof(m_stackBuffer));
	// 	}
	// };

	// 로컬 변수로 잠깐 쓸 경우 유용
	// 주의: 버퍼 크기가 매우 작으므로 조심해서 쓸 것! 가변 크기 데이터를 넣거나 해서는 곤란!
	// ** PS4&UE4 포팅하던 상황에서, 이것이 고정 크기 배열을 내장했더니, 오류가 발생했었다. 그리고 이 클래스는
	// obj-pool 기능이 없던 시절에 들어갔던 기능이다. 따라서 지금은 이 클래스를 없애도 된다.
	class CSmallStackAllocMessage: public CMessage
	{
	public:
		inline CSmallStackAllocMessage()
		{
			UseInternalBuffer();
		}
	};

#ifdef _DEBUG
	/* 
	\~korean
	임시적으로 splitter test를 끄고자 할 때 이 객체를 로컬 변수로 둬서 쓰도록 한다.
	- 주로 serialize 함수 내부에서 쓴다.
	- CSendFragRefs에서 필요해서 넣은 클래스 

	\~english
	To temporarily turn off splitter test, this object can be as a local variable.
	- Mainly use in inside of serialize function
	- Class added since required by CSendFragRefs

	\~chinese
	想临时关闭splitter test的时候，要把此对象留为本地变数而使用。
	- 主要在serialize函数内部使用。
	- 在 CSendFragRefs%里需要而输入的类。

	\~japanese
	臨時的にsplitter testをオフしたい場合、このオブジェクトをローカル変数に置いて使うようにします。
	- 主に serialize関数の内部で使います。
	- CSendFragRefsで必要なので入れたクラス 
	\~
	*/
	class CMessageTestSplitterTemporaryDisabler
	{
		CMessage* m_msg;
		bool m_oldVal;
	public:
		CMessageTestSplitterTemporaryDisabler(CMessage& msg);
		~CMessageTestSplitterTemporaryDisabler();
	};

#endif


	/**  @} */
	

	
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif

#include "Message.inl"
