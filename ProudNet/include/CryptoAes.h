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

#include "Enums.h"
#include "ByteArray.h"
#include "EncryptEnum.h"
#include "ByteArrayPtr.h"

#ifdef _MSC_VER
#pragma warning(disable:4267)
#endif

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

	class CCryptoAesKey;
	class CMessage;

	/** \addtogroup util_group
	*  @{
	*/

	/**
	\~korean
	Rijndael(레인달)은 블럭 암호화 알고리즘입니다. 레인달은 Joan Daeden과 Vincent Rijmen에 의해 만들어졌고 AES의 후보였습니다.
	이 암호화 알고리즘은 다양한 크기의 블럭 길이와 키 길이를 가집니다. 키 길이는 128,192.256비트이며 블럭의 크기는 128,192,256비트입니다.
	키 길이와 블록 길이는 다양하게 선택되어도 됩니다. 블록 길이, 키 길이 모두 32비트의 배수로 쉽게 확장 가능합니다.

	레인달은 매우 다양한 종류의 하드웨어 프로세서에서 매우 효과적으로 구현될 수 있습니다.

	이 구현은 Crptix tookit에 쓰인 Javaj 구현을 기초로 제작되었으며 이는 아래에서 찾을 수 있습니다.
	<a href="http://www.efgh.com/software/rijndael.zip" target="_blank">http://www.efgh.com/software/rijndael.zip</a>

	Java 코드 작성자: Raif S. Naffah, Paulo S. L. M. Barreto

	이 구현은 원저작자에 의해 공개된 KAT 테스트에서 시험되었으며 결과는 이상무였습니다.

	\~english
	Rijndael (pronounced Reindaal) is a block cipher, designed by Joan Daemen and Vincent Rijmen as a candidate algorithm for the AES.
	The cipher has a variable block length and key length. The authors currently specify how to use keys with a length
	of 128, 192, or 256 bits to encrypt blocks with length of 128, 192 or 256 bits (all nine combinations of
	key length and block length are possible). Both block length and key length can be extended very easily to
	multiples of 32 bits.

	Rijndael can be implemented very efficiently on a wide range of processors and in hardware.

	This implementation is based on the Java Implementation used with the Cryptix toolkit found at:
	<a href="http://www.efgh.com/software/rijndael.zip" target="_blank">http://www.efgh.com/software/rijndael.zip</a>

	Java code authors: Raif S. Naffah, Paulo S. L. M. Barreto

	This Implementation was tested against KAT test published by the authors of the method and the
	results were identical.

	\~chinese
	Rijndael是block加密算法。Rijndael是由Joan Daeden和Vincent Rijmen创建，是AES的替补。
	此加密算法拥有着多样化大小的block长度和key长度。key长度是128,192.256bit，block的大小是128,192,256bit。
	key长度和block长度可以多样选择。block长度，key长度都可以容易扩张至32bit的倍数。
	Rijndael在多样化硬件处理器里会非常有效的体现出来.
	Rijndael以在Crptix tookit的Java为基础制造的，可以在以下地址找到。
	<a href="http://www.efgh.com/software/rijndael.zip" target="_blank">http://www.efgh.com/software/rijndael.zip</a>
	Java代码作者：Raif S. Naffah, Paulo S. L. M. Barreto
	Rijndael原著作公开的KAT测试上试验过，结果无异常。

	\~japanese
	Rijndael(レインダル)はブロック暗号化アルゴリズムです。レインダルはJoan DaedenとVincent Rijmenによって作られてAESの候補ｆでした。
	この暗号化アルゴリズムは多様なサイズのブロック長さとキー長さを持ちます。キーの長さは128,192.256ビットで、ブロックのサイズは128,192,256ビットです。
	キー長さとブロック長さは多様な選択されても良いです。ブロック長さ、キー長さとも32ビットの倍数で簡単に拡張できます。
	レインダルはとても多様な種類のハードウェアプロセッサーでとても効果的に実現されることができます。
	この実現は、Crptix tookitで使われたJavaj実現を土台に作成されて、これは下で見つけることができます。
	<ahref="http://www.efgh.com/software/rijndael.zip" target="_blank">http://www.efgh.com/software/rijndael.zip</a>
	Java コード作成者：Raif S. Naffah, Paulo S. L. M. Barreto
	この実現は元著作者によって公開されたKATテストが行われて結果は異常ありませんでした。
	\~
	*/
	class CCryptoAes
	{
	public:
		/**
		\~korean
		- ECB, CBC, CFB 모드
		- ECB 모드 : 같은 블럭을 두번 암호화 하면 본래의 암호화 되지 않은 본문이 나옵니다.
		- CBC 모드 : 이전 블럭과 xor연산을 후 암호화 합니다.
		- CFB 모드 : 해당 블럭을 암호화 후 이전 블럭과 xor연산을 합니다.

		\~english
		- ECB, CBC, CFB Mode
		- ECB Mode: when the same block is encrypted twice, the original unencrypted test appears. 
		- CBC mode: the previous block and xor calculation is encrypted afterwards. 
		- CFB mode: after the block is encrypted, it does xor calculation with the previous block. 
		

		\~chinese
		- ECB，CBC，CFB模式
		- ECB模式：加密同样的block的话会出来原来没有加密的原文。
		- CBC模式：与之前的block和xor运算之后加密。
		- CFB模式：加密相关block以后与之前的block xor运算。

		\~japanese
		- ECB, CBC, CFB モード
		- ECBモード：同じブロックを2回暗号化すると本来の暗号化されなかった本文が出ます。
		- CBCモード：以前のブロックとxor演算を行ってから暗号化します。
		- CFBモード：該当ブロックを暗号化した後、以前のブロックとxor演算を行います。
		\~
		*/
		enum EncryptMode
		{
			ECB = 0,
			CBC = 1,
			CFB = 2
		};

	public:
		enum { DEFAULT_BLOCK_SIZE = 16 };
		enum { MAX_BLOCK_SIZE = 32, MAX_ROUNDS = 14, MAX_KC = 8, MAX_BC = 8 };

	public:
		 CCryptoAes();
		 virtual ~CCryptoAes();

	public:
		/**
		\~korean
		키와 블록의 길이 128비트일 때 - 9라운드
		- 키 또는 블록의 길이가 192비트일 때 - 11라운드
		- 키 또는 블록의 길이가 256비트일 때 - 13라운드
		- 블럭사이즈 : 한번에 암호화 되는 사이즈

		\param outKey 생성된 암호화 키가 저장됩니다..
		\param inputKey 128/192/256-bit 를 사용할 수 있으며 초기 Aes키를 만들기 위한 랜덤 블럭 데이터입니다.
		\param keyLength 16, 24 또는 32 bytes 입니다.
		\param blockSize Aes 알고리즘 내의 블럭 사이즈 입니다. (16, 24 or 32 bytes).

		\~english TODO:translate needed.
		When the length of the key and the block is 128bit – 9 rounds 
		- When the length of the key and the block is 192bit – 9 rounds
		- When the length of the key and the block is 256bit – 9 rounds
		- Block size: size that can be encrypted at once. 


		\param outKey Generated encryption key is saved
		\param inputKey The 128/192/256-bit can be used and this is the random block data to make the initial Aes key. 
		\param keyLength It’s either 16, 24 or 32 bytes
		\param This is the block size within the Aes algorithm (16, 24 or 32 bytes). 

		\~chinese
		Key和block的长度是128bit的时候 – 9回合
		- key或者block的长度是192bit的时候 – 11回合
		- key或者block的长度是256bit的时候 – 13回合
		- block大小：一次能加密的大小

		\param outKey 储存生成的加密化key
		\param inputKey 可以使用128/192/256-bit，为了创建初始Aes key的随机block数据。
		\param keyLength 16，24或者32bytes。
		\param blockSize Aes算法内的bolck大小（16，24或者32bytes）

		\~japanese
		キーとブロックの長さ128ビットである時 – 9ラウンド
		- キーまたはブロックの長さが192ビットである時 – 11ラウンド
		- キーまたはブロックの長さが256ビットである時 – 13ラウンド
		- ブロックサイズ：一度に暗号化されるサイズ

		\param outKey 生成された暗号化キーが保存されます。
		\param inputKey 128/192/256-bit を使うことができ、初期Aesキーを作るためのランダムブロックデータです。
		\param keyLength 16, 24 または 32 bytes です。
		\param blockSize Aes アルゴリズム内のブロックサイズです。(16, 24 or 32 bytes).
		\~
		*/
		 PROUD_API static bool ExpandFrom(CCryptoAesKey &outKey, const uint8_t* const inputKey, int keyLength = DEFAULT_BLOCK_SIZE, int blockSize = DEFAULT_BLOCK_SIZE);

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
		\param key 对称key
		\param inputLength 要加密的数据长度
		\return 加密的数据长度

		\~japanese
		\param key 対称キーです。
		\param inputLength 暗号化するデータの長さ
		\return 暗号化されたデータの長さ
		\~
		*/
		 static int GetEncryptSize(const CCryptoAesKey &key, int inputLength);

		/**
		\~korean
		데이터를 암호화 합니다.
		\param key 대칭키입니다.
		\param input 평문 메시지입니다.
		\param inputLength 평문의 사이즈입니다.
		\param output 암호화된 결과값입니다.
		\param outputLength 암호화된 결과의 사이즈입니다.
		\param initializationVector CBC, CFB 모드에서 암호화를 위한 초기화 할때 사용하는 벡터입니다.
		\param initializationVectorLength initializationVector의 바이트 길이 입니다.
		\param encryptMode 암호화 모드 입니다. ECB, CBC, CFB 모드가 있습니다.

		\~english
		Encrypt data
		\param key Symmetric key
		\param input Plain text message
		\param inputLength Size of plain text
		\param output Encrypted result value
		\param outputLength Size of Encrypted result
		\param initializationVector Initialization Vector (IV) for CBC or CFB mode encryption.
		\param initializationVectorLength Length of initializationVector in bytes.
		\param encryptMode Encryption mode. (ECB, CBC, CFB mode)

		\~chinese
		加密数据。
		\param key 对称key。
		\param input 明文信息。
		\param inputLength 明文的大小
		\param output 加密的结果值。
		\param outputLength 加密的结果大小。
		\param initializationVector 在CBC，CFB模式中为加密进行初始化时所使用的Vector。
		\param initializationVectorLength 是 initializationVector Byte长度。
		\param encryptMode 加密模式。有ECB，CBC，CFB模式。

		\~japanese
		データを暗号化します。
		\param key 対称キーです。
		\param input 平文メッセージです。
		\param inputLength 平文のサイズです。
		\param output 暗号化された結果値です。
		\param outputLength 暗号化された結果のサイズです。
		\param initializationVector CBC, CFB モードで暗号化のための初期化時に使うベクトルです。
		\param initializationVectorLength initializationVectorのバイト長さです。
		\param encryptMode 暗号化モードです。ECB, CBC, CFBモードがあります。
		\~
		*/
		 static bool Encrypt(
			const CCryptoAesKey &key, 
			const uint8_t* const input, 
			int inputLength, 
			uint8_t *output, 
			int &outputLength, 
			uint8_t * initializationVector = NULL, 
			int initializationVectorLength = 0, 
			EncryptMode encryptMode = ECB
			);

		/**
		\~korean
		데이터를 복호화 합니다.
		\param key 대칭키 입니다.
		\param input 암호화된 메시지입니다.
		\param inputLength 암호화된 메시지의 사이즈입니다.
		\param output 복호화된 결과값 입니다.
		\param outputLength 복호화된 결과값의 사이즈입니다.
		\param initializationVector CBC, CFB 모드에서 암호화를 위한 초기화 할때 사용하는 벡터입니다.
		\param initializationVectorLength initializationVector의 바이트 길이 입니다.
		\param encryptMode 암호화 모드 입니다. ECB, CBC, CFB 모드가 있습니다.

		\~english
		Decrypt data
		\param key Symmetric key
		\param input Encrypted message
		\param inputLength Size of Encrypted message
		\param output Decrypted result value
		\param outputLength Size of Decrypted text
		\param initializationVector Initialization Vector (IV) for CBC or CFB mode encryption.
		\param initializationVectorLength Length of initializationVector in bytes.
		\param encryptMode Encryption mode. (ECB, CBC, CFB mode)

		\~chinese
		破译数据。
		\param key 对称key。
		\param input 加密的信息。
		\param inputLength 加密信息的大小
		\param output 译码的结果值。
		\param outputLength 译码的结果大小。
		\param initializationVector 在CBC，CFB模式中为加密进行初始化时所使用的Vector。
		\param initializationVectorLength 是 initializationVector Byte长度。
		\param encryptMode 加密模式。有ECB，CBC，CFB模式。

		\~japanese
		データを復号化します。
		\param key 対称キーです。
		\param input 暗号化されたメッセージです。
		\param inputLength 暗号化されたメッセージのサイズです。
		\param output 復号化された結果値です。
		\param outputLength 復号化された結果値のサイズです。
		\param initializationVector CBC, CFB モードで暗号化のための初期化時に使うベクトルです。
		\param initializationVectorLength initializationVectorのバイト長さです。
		\param encryptMode 暗号化モードです。 ECB, CBC, CFB モードがあります。
		\~
		*/
		 static bool Decrypt(
			const CCryptoAesKey &key, 
			const uint8_t* const input, 
			int inputLength, 
			uint8_t *output, 
			int &outputLength, 
			uint8_t * initializationVector = NULL, 
			int initializationVectorLength = 0,
			EncryptMode encryptMode = ECB
			);

		/**
		\~korean
		ByteArray를 암호화 합니다.
		\param key 대칭키입니다.
		\param input 평문 메시지입니다.
		\param output 암호화된 결과값입니다.
		\param initializationVector CBC, CFB 모드에서 암호화를 위한 초기화 할때 사용하는 벡터입니다.
		\param initializationVectorLength initializationVector의 바이트 길이 입니다.
		\param encryptMode 암호화 모드 입니다. ECB, CBC, CFB 모드가 있습니다.

		\~english
		Encrypt ByteArray
		\param key Symmetric key
		\param input Plain text message
		\param output Encrypted result value
		\param initializationVector Initialization Vector (IV) for CBC or CFB mode encryption.
		\param initializationVectorLength Length of initializationVector in bytes.
		\param encryptMode Encryption mode. (ECB, CBC, CFB mode)

		\~chinese
		加密 ByteArray。
		\param key 对称key。
		\param input 明文信息。
		\param output 加密的结果值。
		\param initializationVector 在CBC，CFB模式中为加密进行初始化时所使用的Vector。
		\param initializationVectorLength 是 initializationVector Byte长度。
		\param encryptMode 加密模式。有ECB，CBC，CFB模式。

		\~japanese
		ByteArrayを暗号化します。
		\param key 対称キーです。
		\param input 平文メッセージです。
		\param output 暗号化された結果値です。
		\param initializationVector CBC, CFBモードで暗号化のための初期化時に使うベクトルです。
		\param initializationVectorLength initializationVectorのバイト長さです。
		\param encryptMode 暗号化モードです。ECB, CBC, CFB モードがあります。
		\~
		*/
		 static bool EncryptByteArray(
			const CCryptoAesKey &key, 
			const ByteArray &input, 
			ByteArray &output, 
			uint8_t * initializationVector = NULL, 
			int initializationVectorLength = 0, 
			EncryptMode encryptMode = ECB
			);

		/**
		\~korean
		ByteArray를 복호화 합니다.
		\param key 대칭키 입니다.
		\param input 암호화된 메시지입니다.
		\param output 복호화된 결과값 입니다.
		\param initializationVector CBC, CFB 모드에서 암호화를 위한 초기화 할때 사용하는 벡터입니다.
		\param initializationVectorLength initializationVector의 바이트 길이 입니다.
		\param encryptMode 암호화 모드 입니다. ECB, CBC, CFB 모드가 있습니다.

		\~english
		Decrypt ByteArray
		\param key Symmetric key
		\param input Encrypted message
		\param output Decrypted result value
		\param initializationVector Initialization Vector (IV) for CBC or CFB mode encryption.
		\param initializationVectorLength Length of initializationVector in bytes.
		\param encryptMode Encryption mode. (ECB, CBC, CFB mode)

		\~chinese
		破译 ByteArray。
		\param key 对称key。
		\param input 加密的信息。
		\param output 破译的结果值。
		\param initializationVector 在CBC，CFB模式中为加密进行初始化时所使用的Vector。
		\param initializationVectorLength 是 initializationVector Byte长度。
		\param encryptMode 加密模式。有ECB，CBC，CFB模式。

		\~japanese
		ByteArrayを復号化します。
		\param key 対称キーです。
		\param input 暗号化されたメッセージです。
		\param output 復号化された結果値です。
		\param initializationVector CBC, CFB モードで暗号化のための初期化時に使うベクトルです。
		\param initializationVectorLength initializationVectorのバイト長さです。
		\param encryptMode 暗号化モードです。ECB, CBC, CFBモードがあります。
		\~
		*/
		 PROUD_API static bool DecryptByteArray(
			const CCryptoAesKey &key,
			const ByteArray &input, ByteArray &output, 
			uint8_t * initializationVector = NULL, 
			int initializationVectorLength = 0, 
			EncryptMode encryptMode = ECB
			);

		/**
		\~korean
		Message를 암호화 합니다.
		\param key 대칭키입니다.
		\param input 평문 메시지입니다.
		\param output 암호화된 결과값입니다.
		\param offset 평문 메시지에서 암호화를 시작할 위치
		\param initializationVector CBC, CFB 모드에서 암호화를 위한 초기화 할때 사용하는 벡터입니다.
		\param initializationVectorLength initializationVector의 바이트 길이 입니다.
		\param encryptMode 암호화 모드 입니다. ECB, CBC, CFB 모드가 있습니다.

		\~english
		Encrypt Message
		\param key Symmetric key
		\param input Plain text message
		\param output Encrypted result value
		\param offset Encrypt start position at plain text message
		\param initializationVector Initialization Vector (IV) for CBC or CFB mode encryption.
		\param initializationVectorLength Length of initializationVector in bytes.
		\param encryptMode Encryption mode. (ECB, CBC, CFB mode)

		\~chinese
		加密Message。
		\param key 对称key。
		\param input 明文信息。
		\param output 加密的结果值。
		\param offset 在明文信息开始加密的位置。
		\param initializationVector 在CBC，CFB模式中为加密进行初始化时所使用的Vector。
		\param initializationVectorLength 是 initializationVector Byte长度。
		\param encryptMode 加密模式。有ECB，CBC，CFB模式。

		\~japanese
		Messageを暗号化します。
		\param key 対称キーです。
		\param input 平文メッセージです。
		\param output 暗号化された結果値です。
		\param offset 平文メッセージで暗号化を開始する位置
		\param initializationVector CBC, CFB モードで暗号化のための初期化時に使うベクトルです。
		\param initializationVectorLength initializationVectorのバイト長さです。
		\param encryptMode 暗号化モードです。ECB, CBC, CFBモードがあります。
		\~
		*/
		 PROUD_API static bool EncryptMessage(
			const CCryptoAesKey &key, 
			const CMessage &input, 
			CMessage &output, 
			int offset, 
			uint8_t * initializationVector = NULL, 
			int initializationVectorLength = 0, 
			EncryptMode encryptMode = ECB
			);

		/**
		\~korean
		Message를 복호화 합니다.
		\param key 대칭키 입니다.
		\param input 암호화된 메시지입니다.
		\param output 복호화된 결과값 입니다.
		\param offset 암호화된 메시지에서 복호화를 시작할 위치
		\param initializationVector CBC, CFB 모드에서 암호화를 위한 초기화 할때 사용하는 벡터입니다.
		\param initializationVectorLength initializationVector의 바이트 길이 입니다.
		\param encryptMode 암호화 모드 입니다. ECB, CBC, CFB 모드가 있습니다.

		\~english
		Decrypt Message
		\param key Symmetric key
		\param input Encrypted message
		\param output Decrypted result value
		\param offset Decrypt start position at encrypted message
		\param initializationVector Initialization Vector (IV) for CBC or CFB mode encryption.
		\param initializationVectorLength Length of initializationVector in bytes.
		\param encryptMode Encryption mode. (ECB, CBC, CFB mode)

		\~chinese
		破译Message。
		\param key 对称key。
		\param input 加密的信息。
		\param output 破译的结果值。
		\param offset 在加密信息开始破译的位置。
		\param initializationVector 在CBC，CFB模式中为加密进行初始化时所使用的Vector。
		\param initializationVectorLength 是 initializationVector Byte长度。
		\param encryptMode 加密模式。有ECB，CBC，CFB模式。

		\~japanese
		Messageを復号化します。
		\param key 対称キーです。
		\param input 暗号化されたメッセージです。
		\param output 復号化された結果値です。
		\param offset 暗号化されたメッセージで復号化を開始する位置
		\param initializationVector CBC, CFB モードで暗号化のための初期化時に使うべくトルです。
		\param initializationVectorLength initializationVectorのバイト長さです。
		\param encryptMode 暗号化モードです。ECB, CBC, CFBモードがあります。
		\~
		*/
		 PROUD_API static bool DecryptMessage(
			const CCryptoAesKey &key, 
			const CMessage &input, 
			CMessage &output, 
			int offset, 
			uint8_t * initializationVector = NULL, 
			int initializationVectorLength = 0, 
			EncryptMode encryptMode = ECB
			);

	private:
		static bool DefaultEncryptBlock(const CCryptoAesKey &key, const uint8_t* in, uint8_t *result);
		static bool DefaultDecryptBlock(const CCryptoAesKey &key, const uint8_t* in, uint8_t *result);
		static bool EncryptBlock(const CCryptoAesKey &key, const uint8_t* in, uint8_t *result);
		static bool DecryptBlock(const CCryptoAesKey &key, const uint8_t* in, uint8_t *result);
		static int Mul(int a, int b);
		static int Mul4(int a, char b[]);
		static void Xor(uint8_t *buff, const uint8_t* chain, const int blockSize);

	private:
		/** Null chain */
		static char const* m_chain0;

		static const int m_alog[256];
		static const int m_log[256];

		static const int8_t m_s[256];

		static const int8_t m_si[256];

		static const int m_t1[256];
		static const int m_t2[256];
		static const int m_t3[256];
		static const int m_t4[256];

		static const int m_t5[256];
		static const int m_t6[256];
		static const int m_t7[256];
		static const int m_t8[256];

		static const int m_u1[256];
		static const int m_u2[256];
		static const int m_u3[256];
		static const int m_u4[256];
		static const int8_t m_rcon[30];
		static const int m_shifts[3][4][2];
	};

	class CCryptoAesKey
	{
		friend class CCryptoAes;

	public:
		inline CCryptoAesKey()
		{
			m_keyLength = 0;
		}

		/**
		\~korean
		aes 키 길이를 얻어옵니다. (byte)
		\return aes 키 길이

		\~english
		Gets length of AES key. (byte)
		\return AES key length

		\~chinese
		获得aes key长度。（byte）
		\return aes key长度

		\~japanese
		aes キーの長さを得ます。(byte)
		\return aes キーの長さ
		\~
		*/
		 int GetKeyLength() const;

		/**
		\~korean
		블럭 사이즈를 얻어옵니다. (byte)
		\return m_blockSize

		\~english
		Gets block size. (byte)
		\return m_blockSize

		\~chinese
		获得block大小。（byte）
		\return m_blockSize

		\~japanese
		ブロックサイズを得ます。(byte)
		\return m_blockSize

		\~
		*/
		 int	GetBlockSize() const;

		/**
		\~korean
		라운드 수를 얻어옵니다.
		\return m_rounds

		\~english
		Gets number of round.
		\return m_rounds

		\~chinese
		获得回合数。
		\return m_rounds

		\~japanese
		ラウンド数を得ます。
		\return m_rounds

		\~
		*/
		 int GetRounds() const;

		/**
		\~korean
		키가 이미 생성되었는지를 확인합니다.

		\~english
		Check key is already existed or not.

		\~chinese
		确认key是否已经生成了。

		\~japanese
		キーが既に生成されているかを確認します。
		\~
		*/
		 PROUD_API bool KeyExists() const;

		 void Clear();

	private:
		// 암호화에 사용되는 round key
		int m_ke[CCryptoAes::MAX_ROUNDS + 1][CCryptoAes::MAX_BC];
		// 복호화에 사용되는 round key
		int m_kd[CCryptoAes::MAX_ROUNDS + 1][CCryptoAes::MAX_BC];
		// 키 길이 (byte)
		int m_keyLength;
		// 블럭 사이즈
		int	m_blockSize;
		// 라운드 수
		int m_rounds;

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif

	};

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
