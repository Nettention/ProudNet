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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	// 주의: 이것도 바꾸면 ProudClr namespace의 동명 심볼도 수정해야 한다.

	/** 
	\~korean
	암호화 및 복호화 설정값입니다.

	\~english
	Setup value of encryption and decryption

	\~chinese
	加密以及解密设定值。

	\~japanese
	暗号化及び復号化の設定値です。
	\~
	*/
	enum EncryptMode
	{
		/** 
		\~korean
		암호화를 안합니다.

		\~english
		Do not encrypt

		\~chinese
		不加密。

		\~japanese
		暗号化をしません。
		\~
		*/
		EM_None,		
		/** 
		\~korean
		RSA와 AES를 혼용한 암호화를 수행합니다. 로그온 등 보안성이 높은 메시지에 쓰십시오.

		\~english
		Do encryption that mixture with RSA and AES. Do not use with secure message such as logon

		\~chinese
		实行混用RSA和AES的加密。用在像登录等安全性高的信息上。

		\~japanese
		RSAとAESを混用した暗号化を遂行します。ログオンなどセキュリティ性の高いメッセージに使ってください。
		\~
		*/
		EM_Secure,		
		/** 
		\~korean
		RSA와 Fast를 혼용한 암호화를 수행합니다. 캐릭터 이동 등 약간의 암호화는 필요하지만 성능 또한 고려해야 하는 상황에서 쓰십시오.

		\~english
		Do encryption that mixture with RSA and Fast. Encryption is required like moving character but please consider about performance.

		\~chinese
		实行混用RSA和Fast的加密。如角色移动等虽加密程度不高，但需考虑性能的状况下请使用此方法。 

		\~japanese
		RSAとFastを混用した暗号化を遂行します。キャラクター移動など少しの暗号化は必要ですが、性能も考慮すべき状況で使ってください。
		\~
		*/
		EM_Fast,			
		EM_LAST,
	};

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
