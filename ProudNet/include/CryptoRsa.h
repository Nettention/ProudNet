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

#include "Enums.h"
#include "FastArray.h"
#include "ErrorInfo.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

typedef struct Rsa_key rsa_key;

namespace Proud
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	RSA Key 데이터 클래스 입니다.

	\~english
	RSA Key data class.

	\~chinese
	RSA Key数据类。

	\~japanese
	RSA Key データクラスです。
	\~
	*/
	class CCryptoRsaKey
	{
	public:

		/** RSA Key */
		Rsa_key *m_key;

		 	PROUD_API CCryptoRsaKey();
		 PROUD_API ~CCryptoRsaKey();

		/** 
		\~korean
		ByteArray 로 이루어진 공개키를 CRsaKey 로 Import 합니다.
		\param blob CRsaKey 에서 뽑아진 ByteArray 공개키 

		\~english
		Import public key with CRsaKey that made with ByteArray.
		\param blob ByteArray public key delegated from CRsaKey

		\~chinese
		由 ByteArray%组成的公开key import为 CRsaKey%。
		\param blob 从 CRsaKey%选的 ByteArray%公开key。

		\~japanese
		ByteArrayからなる公開キーをCRsaKeyでImportします。
		\param blob CRsaKeyより選られたByteArray公開キー 
		\~
		*/
		 PROUD_API bool FromBlob(const ByteArray& blob);

		/** 
		\~korean
		CRsaKey 에서 공개키를 ByteArray 로 뽑아냅니다.
		\param outBlob 키에서 뽑아낸 공개키의 ByteArray 입니다. 

		\~english
		Delegate public key with ByteArray from CRsaKey.
		\param outBlob ByteArray of public key that delegated from key.

		\~chinese
		从 CRsaKey%选公开key为 ByteArray。
		\param outBlob 从key选的公开key的 ByteArray%。

		\~japanese
		CrsaKeyより公開キーをByteArrayで引き取ります。
		\param outBlob キーより引き取った公開キーのByteArrayです。 
		\~
		*/
		 bool ToBlob(ByteArray &outBlob);

		/** Same to ToBlob(), but it outputs private key.*/
		 PROUD_API bool ToBlob_privateKey(ByteArray &outBlob);
	private:
		bool ToBlob_internal(ByteArray &outBlob, int keyType);
	public:

		operator Rsa_key*() 
 		{
 			return m_key;
 		}

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif

	};

	typedef RefCount<CCryptoRsaKey> CRsaKeyPtr;

	/** 
	\~korean
	RSA 공개키, 비공개키 암호화 클래스 

	\~english
	Encryption class of RSA publick key, private key

	\~chinese
	RSA公开key，非公开key的加密类。

	\~japanese
	RSA公開キー、非公開キー暗号化クラス
	\~
	*/
	class CCryptoRsa
	{
	public:
		/** 
		\~korean
		Random Block 을 생성합니다.
		\param output 생성될 랜덤블럭의 ByteArray 입니다.
		\param length 생성할 랜덤블럭의 길이입니다. 
		\return 랜덤블럭 생성에 성공하면 true를 리턴하고, 실패하면 false 리턴합니다.

		\~english TODO:translate needed.
		Generate a random block.
		\param output This is the ByteArray of the random block to be generated. 
		\param length This is the length of the random block to be generated. 
		\return true if successful in generating a random block or otherwise return false. 

		\~chinese
		生成Random Block。
		\param output 要生成的随机block的 ByteArray%。
		\param length 要生成的随机block的长度。 
		\return 成功生成随机block的话返回true，失败的话返回false。

		\~japanese
		Random Blockを生成します。
		\param output 生成されるランダムブロックのByteArrayです。
		\param length 生成するランダムブロックの長さです。 
		\return ランダムブロックの生成に成功するとtrueをリターンして、失敗するとfalseをリターンします。
		\~
		*/
		 PROUD_API static bool CreateRandomBlock(ByteArray &output, int length);

		/** 
		\~korean
		RSA 공개키와 비공개키를 생성합니다.
		\param outXchgKey 생성될 RSA 키 입니다.
		\param outPubKeyBlob 생성될 RSA 키에서 export 된 공개키의 ByteArray 값입니다. 
		\return 키 생성에 성공하면 true를 리턴하고, 실패하면 false 리턴합니다.

		\~english TODO:translate needed.
		Generate a public key and a private key. 
		\param outXchgKey This is an RSA key to be generated
		\param outPubKeyBlob This is the value of ByteArray of a public key exported from the RSA key to be generated. 
		\return true if successful in generating a key or otherwise return false.

		\~chinese
		生成RSA公开key和非公开key。
		\param outXchgKey 要生成的RSA key。
		\param outPubKeyBlob 从要生成的RSA key中export的公开key的 ByteArray%值。 
		\return 成功生成key的话返回true，失败的话返回false。

		\~japanese
		RSA公開キーと非公開キーを生成します。
		\param outXchgKey 生成されるRSAキーです。
		\param outPubKeyBlob 生成されるRSAキーよりexportされた公開キーのByteArray値です。
		\return キーの生成に成功するとtrueをリターンして、失敗するとfalseをリターンします。
		\~
		*/
		 PROUD_API static bool CreatePublicAndPrivateKey(CCryptoRsaKey &outXchgKey, ByteArray &outPubKeyBlob);

		/** 
		\~korean
		공개키로 블럭을 암호화합니다.
		- randomBlock 의 사이즈가 key 의 Modulus N 의 값보다 초과하면 Encrypt를 할 수 없습니다. 
		\param outEncryptedSessionKey 암호화 될 랜덤블럭입니다.
		\param randomBlock 암호화 할 랜덤블럭입니다.
		\param publicKeyBlob RSA 공개키입니다.
		\return 공개키로 암호화된 대칭키를 얻는데 성공하면 true를 리턴하고, 실패하면 false 리턴합니다.

		\~english TODO:translate needed.
		Encode the block with the public key
		- If the size of randomBlock exceeds the Modulus N value of the key, you cannot do encryption. 
		\param outEncryptedSessionKey This is the random block to be encrypted.
		\param randomBlock This is the random block you will encrypt. 
		\param publicKeyBlob  This is the RSA public key. 
		\return true if successful in obtaining an encrypted symmetric key with the public key or otherwise return false. 

		\~chinese
		用公开key加密block。
		- randomBlock的大小超过key的Modulus N的值的话不能Encrypt。
		\param outEncryptedSessionKey 要被加密的随机block。
		\param randomBlock 要加密的随机block。
		\param publicKeyBlob RSA公开key。
		\return 用公开key获得加密的对称key成功的话返回true，失败的话返回false。
		
		\~japanese
		公開キーでブロックを暗号化します。
		- randomBlock のサイズがキーのModulus Nの値を超過するとEncryptができません。
		\param outEncryptedSessionKey 暗号化されるランダムブロックです。
		\param randomBlock 暗号化するランダムブロックです。
		\param publicKeyBlob RSA 公開キーです。
		\return 公開キーで暗号化された対称キーを得ることに成功するとtrueをリターンして、失敗するとfalseをリターンします。
		\~
		*/
		 PROUD_API static bool EncryptSessionKeyByPublicKey(ByteArray &outEncryptedSessionKey, const ByteArray &randomBlock, const ByteArray &publicKeyBlob);

		/** 
		\~korean
		비공개키로 암호화된 블럭을 복호화합니다.
		\param outRandomBlock 복호화 할 ByteArray
		\param encryptedSessionKey 암호화된 ByteArray
		\param privateKey 비공개키입니다. 
		\return 개인키로 암호화된 대칭키를 복호화 하여 얻는데 실패하면 ErrorInforPtr을 리턴합니다.

		\~english TODO:translate needed.
		Decode the encrypted block with the private key.
		\param outRandomBlock ByteArray to decrypt.
		\param encryptedSessionKey Encrypted ByteArray.
		\param privateKey This is a private key.
		\return When you fail to obtain an encrypted symmetric key with a private key, returns ErrorInforPtr. 

		\~chinese
		用非公开key破译被加密的block。
		\param outRandomBlock 要破译的 ByteArray%。
		\param encryptedSessionKey 加密的 ByteArray%。
		\param privateKey 非公开key。
		\return 用个人key破译已被加密的对称key失败的话返回ErrorInforPtr。

		\~japanese
		非公開キーで暗号化されたブロックを復号化します。
		\param outRandomBlock 復号化する ByteArray
		\param encryptedSessionKey 暗号化された ByteArray
		\param privateKey 非公開キーです。 
		\return 個人キーで暗号化された対称キーを復号化して得ることに失敗すると ErrorInforPtrをリターンします。
		\~
		*/
		 PROUD_API static ErrorInfoPtr DecryptSessionKeyByPrivateKey(ByteArray &outRandomBlock, const ByteArray &encryptedSessionKey, const CCryptoRsaKey &privateKey);
	};

	typedef uint16_t CryptCount;

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
