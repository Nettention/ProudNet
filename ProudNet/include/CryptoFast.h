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


此程序的版??Nettention公司所有。
?此程序的修改、使用、?布相?的事?要遵守此程序的所有?者的??。
不遵守???要原?性的禁止擅自使用。
擅自使用的?任明示在?此程序所有?者的合同?里。

** 注意：不要移除?于制作物的上述明示。


このプログラムの著作権はNettentionにあります。
このプログラムの修正、使用、配布に関する事項は本プログラムの所有権者との契約に従い、
契約を遵守しない場合、原則的に無断使用を禁じます。
無断使用による責任は本プログラムの所有権者との契約書に明示されています。

** 注意：著作物に関する上記の明示を除去しないでください。

*/

#pragma once

#include "ErrorInfo.h"
#include "ByteArray.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class CByteArray;
	class CMessage;

	class CCryptoFastKey
	{
		friend class CCryptoFast;

	private:
		// 대칭키
		ByteArray m_key;

	public:
		 PROUD_API CCryptoFastKey();
		 PROUD_API ~CCryptoFastKey();

		/**
		\~korean
		Fast 키 길이를 얻어옵니다. (byte)
		\return Fast 키 길이

		\~english
		Gets length of Fast key. (byte)
		\return Fast key length

		\~chinese
		?得Fast key?度。（byte）
		\return Fast key?度

		\~japanese
		Fast キーの長さを得てきます。(byte)
		\return Fast キーの長さ
		\~
		*/
		 int GetKeyLength() const;


		/**
		\~korean
		키가 이미 생성되었는지를 확인합니다.

		\~english
		Check key is already existed or not.

		\~chinese
		??key是否已?生成了。

		\~japanese
		キーが既に生成されているかを確認します。
		\~
		*/
		 PROUD_API bool KeyExists() const;

		 void Clear();
	};

	// 매우 단순한 수준의 암호화 알고리즘
	// 게임 네트워킹 메시지를 모두 암호화되 큰 부하를 주지 않을만큼 가볍게 작동하지만 쉽게 해킹당한다는 단점이 있다.
	// 초보적인 해커 정도는 걸러내는 용도 가령 게임 플레이의 모든 메시지를 암호화하는데 쓰이면 된다.
	class CCryptoFast
	{
	public:
		/**
		\~korean
		암호화에 사용되는 키 객체를 생성하는 함수입니다.
		\param outKey 생성된 암호화 키 객체가 저장됩니다.
		\param inputKey
		\param keyLength 16 or 24 or 32 bytes 입니다.

		\~english
		\param outKey
		\param inputKey
		\param keyLength 16 or 24 or 32 bytes

		\~chinese
		\param outKey
		\param inputKey
		\param keyLength 16 or 24 or 32 bytes。
		

		\~japanese
		暗号化に使われるキーオブジェクトを生成する関数です。
		\param outKey 生成された暗号化キーオブジェクトが保存されます。
		\param inputKey
		\param keyLength 16 or 24 or 32 bytes です。
		\~
		*/
		 PROUD_API static bool ExpandFrom(CCryptoFastKey& outKey, const uint8_t* inputKey, int keyLength);

		/**
		\~korean
		\param key 대칭키입니다.
		\param inputLength 암호화할 데이터의 길이
		\return 암호화된 데이터의 길이

		\~english
		\param key Symmetric key
		\param inputLength Size of plain text
		\return Size of Encrypted result

		\~chinese
		\param key ??key
		\param inputLength 要加密的?据?度
		\return 加密的?据?度

		\~japanese
		\param key 対称キーです。
		\param inputLength 暗号化するデータの長さ
		\return 暗号化されたデータの長さ
		\~
		*/
		 static int GetEncryptSize(int inputLength);

		/**
		\~korean
		데이터를 암호화 합니다.
		\param key 대칭키입니다.
		\param input 평문 메시지입니다.
		\param inputLength 평문의 사이즈입니다.
		\param output 암호화된 결과값입니다.
		\param outputLength 암호화된 결과의 사이즈입니다.

		\~english
		Encrypt data
		\param key Symmetric key
		\param input Plain text message
		\param inputLength Size of plain text
		\param output Encrypted result value
		\param outputLength Size of Encrypted result

		\~chinese
		加密?据。
		\param key ??key。
		\param input 明文信息。
		\param inputLength 明文的大小
		\param output 加密的?果?。
		\param outputLength 加密的?果大小。

		\~japanese
		データを暗号化します。
		\param key 対称キーです。
		\param input 平文メッセージです。
		\param inputLength 平文のサイズです。
		\param output 暗号化された結果値です。
		\param outputLength 暗号化された結果のサイズです。
		\~
		*/
		 static bool Encrypt(
			const CCryptoFastKey& key,
			const uint8_t* input,
			int inputLength,
			uint8_t *output,
			int& outputLength,
			ErrorInfoPtr& errorInfo
			);

		/**
		\~korean
		데이터를 복호화 합니다.
		\param key 대칭키 입니다.
		\param input 암호화된 메시지입니다.
		\param inputLength 암호화된 메시지의 사이즈입니다.
		\param output 복호화된 결과값 입니다.
		\param outputLength 복호화된 결과값의 사이즈입니다.

		\~english
		Decrypt data
		\param key Symmetric key
		\param input Encrypted message
		\param inputLength Size of Encrypted message
		\param output Decrypted result value
		\param outputLength Size of Decrypted text

		\~chinese
		破??据。
		\param key ??key。
		\param input 加密的信息。
		\param inputLength 加密信息的大小
		\param output ??的?果?。
		\param outputLength ??的?果大小。

		\~japanese
		データを復号化します。
		\param key 対称キーです。
		\param input 暗号化されたメッセージです。
		\param inputLength 暗号化されたメッセージのサイズです。
		\param output 復号化された結果値です。
		\param outputLength 復号化された結果値のサイズです。
		\~
		*/
		 static bool Decrypt(
			const CCryptoFastKey& key,
			const uint8_t* input,
			int inputLength,
			uint8_t *output,
			int& outputLength,
			ErrorInfoPtr& errorInfo
			);

		/**
		\~korean
		ByteArray를 암호화 합니다.
		\param key 대칭키입니다.
		\param input 평문 메시지입니다.
		\param output 암호화된 결과값입니다.

		\~english
		Encrypt ByteArray
		\param key Symmetric key
		\param input Plain text message
		\param output Encrypted result value

		\~chinese
		加密 ByteArray。
		\param key ??key。
		\param input 明文信息。
		\param output 加密的?果?。

		\~japanese
		ByteArrayを暗号化します。
		\param key 対称キーです。
		\param input 平文メッセージです。
		\param output 暗号化された結果値です。
		
		\~
		*/
		 static bool EncryptByteArray(
			const CCryptoFastKey& key,
			const ByteArray& input,
			ByteArray& output,
			ErrorInfoPtr& errorInfo
			);

		/**
		\~korean
		ByteArray를 복호화 합니다.
		\param key 대칭키 입니다.
		\param input 암호화된 메시지입니다.
		\param output 복호화된 결과값 입니다.

		\~english
		Decrypt ByteArray
		\param key Symmetric key
		\param input Encrypted message
		\param output Decrypted result value

		\~chinese
		破? ByteArray。
		\param key ??key。
		\param input 加密的信息。
		\param output 破?的?果?。

		\~japanese
		ByteArrayを復号化します。
		\param key 対称キーです。
		\param input 暗号化されたメッセージです。
		\param output 復号化された結果値です。
		\~
		*/
		 static bool DecryptByteArray(
			const CCryptoFastKey& key,
			const ByteArray& input, 
			ByteArray& output,
			ErrorInfoPtr& errorInfo
			);

		/**
		\~korean
		Message를 암호화 합니다.
		\param key 대칭키입니다.
		\param input 평문 메시지입니다.
		\param output 암호화된 결과값입니다.
		\param offset 평문 메시지에서 암호화를 시작할 위치

		\~english
		Encrypt Message
		\param key Symmetric key
		\param input Plain text message
		\param output Encrypted result value
		\param offset Encrypt start position at plain text message

		\~chinese
		加密Message。
		\param key ??key。
		\param input 明文信息。
		\param output 加密的?果?。
		\param offset 在明文信息?始加密的位置。

		\~japanese
		Messageを暗号化します。
		\param key 対称キーです。
		\param input 平文メッセージです。
		\param output 暗号化された結果値です。
		\param offset 平文メッセージで暗号化を開始する位置
		\~
		*/
		 PROUD_API static bool EncryptMessage(
			const CCryptoFastKey& key,
			const CMessage& input,
			CMessage& output,
			int offset,
			ErrorInfoPtr& errorInfo
			);

		/**
		\~korean
		Message를 복호화 합니다.
		\param key 대칭키 입니다.
		\param input 암호화된 메시지입니다.
		\param output 복호화된 결과값 입니다.
		\param offset 암호화된 메시지에서 복호화를 시작할 위치

		\~english
		Decrypt Message
		\param key Symmetric key
		\param input Encrypted message
		\param output Decrypted result value
		\param offset Decrypt start position at encrypted message

		\~chinese
		破?Message。
		\param key ??key。
		\param input 加密的信息。
		\param output 破?的?果?。
		\param offset 在加密信息?始破?的位置。

		\~japanese
		Messageを復号化します。
		\param key 対称キーです。
		\param input 暗号化されたメッセージです。
		\param output 復号化された結果値です。
		\param offset 暗号化されたメッセージで復号化を開始する位置
		\~
		*/
		 PROUD_API static bool DecryptMessage(
			const CCryptoFastKey& key,
			const CMessage& input,
			CMessage& output,
			int offset,
			ErrorInfoPtr& errorInfo
			);
	};
}
#ifdef _MSC_VER
#pragma pack(pop)
#endif
