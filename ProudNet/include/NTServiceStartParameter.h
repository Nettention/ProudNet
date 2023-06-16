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

#include <winsvc.h>
#include "BasicTypes.h"
#include "PNString.h"

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

	class INTServiceEvent;

	/**
	\~korean
	서비스를 등록할 때, CreateService 함수에 사용되는 설정값입니다.

	\~english
	It is a setting value that is used for CreateService function when registering a service.

	\~chinese
	注册服务时， 使用于CreateService函数的设定值。

	\~japanese
	サービスを登録する時、CreateService関数に使用される設定値です。
	*/
	class CServiceParameter
	{
	public:
		/**
		\~korean
		서비스에 대한 액세스를 지정합니다. 기본 값은 SERVICE_ALL_ACCESS입니다.

		\~english
		Appoint the access of the service. Default value is SERVICE_ALL_ACCESS.

		\~chinese
		指定对于服务的访问。基本值为SERVICE_ALL_ACCESS。

		\~japanese
		サービスに対するアクセスを指定します。デフォルト値はSERVICE_ALL_ACCESSです。
		*/
		DWORD m_desiredAccess;

		/**
		\~korean
		서비스의 타입을 지정합니다. 기본 값은 SERVICE_WIN32_OWN_PROCESS입니다.

		\~english
		Appoint the service type. Default value is SERVICE_WIN32_OWN_PROCESS.

		\~chinese
		指定服务类型。基本值为SERVICE_WIN32_OWN_PROCESS。

		\~japanese
		サービスのタイプを指定します。デフォルト値はSERVICE_WIN32_OWN_PROCESSです。
		*/
		DWORD m_serviceType;

		/**
		\~korean
		서비스의 시작 옵션을 설정합니다. 기본 값은 SERVICE_DEMAND_START입니다.

		\~english
		Set the start option of the service. Default value is SERVICE_DEMAND_START.

		\~chinese
		设定服务启动的选项。基本值为SERVICE_DEMAND_START。

		\~japanese
		サービスのスタートオプションを設定します。デフォル値はSERVICE_DEMAND_STARTです。
		*/
		DWORD m_startType;

		/**
		\~korean
		서비스 시작이 실패한 경우 오류의 수준을 지정합니다. 기본 값은 SERVICE_ERROR_NORMAL입니다.

		\~english
		Appoint the level of an error in case service start fails. Default value is SERVICE_ERROR_NORMAL.

		\~chinese
		指定服务启动失败时的错误的等级。基本值为SERVICE_ERROR_NORMAL。

		\~japanese
		サービススタートが失敗した場合、エラーのレベルを指定します。デフォルト値はSERVICE_ERROR_NORMALです。
		*/
		DWORD m_errorControl;

		/**
		\~korean
		서비스가 실행해야할 계정의 이름을 지정합니다.

		\~english
		Appoint the name of an account that the service executes.

		\~chinese
		指定实行服务时的账户名。

		\~japanese
		サービスが実行するアカウントの名を指定します。
		*/
		String m_serviceStartName;

		/**
		\~korean
		m_serviceStartName 매개 변수에 의해 지정된 계정 이름에 대한 암호을 지정합니다.

		\~english
		Appoint the password of the account name that has been appointed by m_serviceStartName parameter. 

		\~chinese
		对于因m_serviceStartName变数不同而指定的账户名，指定密码。

		\~japanese
		m_serviceStartName変数により指定されたアカウントの名に対する暗号を指定します。
		*/
		String m_password;

		/**
		\~korean
		생성자 메서드입니다.

		\~english
		It is a constructor method. 

		\~chinese
		生成者方法。

		\~japanese
		生成者マッソドです。
		*/
		 CServiceParameter()
		{
			m_desiredAccess	= SERVICE_ALL_ACCESS;
			m_serviceType	= SERVICE_WIN32_OWN_PROCESS;
			m_startType		= SERVICE_DEMAND_START;
			m_errorControl	= SERVICE_ERROR_NORMAL;
		}
	};


	/**
	\~korean
	CNTService.WinMain 에 의해 NTService를 생성할 때의 초기 설정값입니다.

	\~english
	It is an initial setting value when creating NTService by CNTService.WinMain.

	\~chinese
	因CNTService.WinMain而生成NTService时的初始设定值。

	\~japanese
	CNTService.WinMainによりNTServiceを生成する時の初期設定値です。
	*/
	class CNTServiceStartParameter
	{
	public:
		/**
		\~korean
		NT 서비스 이름입니다. 최대 문자열 길이는 256자이며, '/'나 '\'는 사용은 불가능합니다.

		\~english
		It is the name of NT service. Maximum length of character string is 256 and special characters (such as '/' and '\') are not available. 

		\~chinese
		NT服务的名称。字符窜最大长度为256个字符，且不可使用 '/' 或 '\'。

		\~japanese
		NTサービスの名です。文字列は最大256字であり、'/'や'\'は使用できません。
		*/
		String m_serviceName;

		/**
		\~korean
		CNTService가 필요로 하는 Event Sink 객체입니다.

		\~english
		It is an Even Sink object that CNTService needs. 
		
		\~chinese
		CNTService 所需的 Event Sink 对象。

		\~japanese
		CNTServiceが必要とするEvent Sinkオブジェクトです。
		*/
		INTServiceEvent* m_serviceEvent;

		/**
		\~korean
		서비스를 등록할 때, CreateService 함수에 사용되는 설정값입니다.

		\~english
		It is a setting value that is used for CreateService function when registering a service.  

		\~chinese
		注册服务时， CreateService 函数所使用的设定值。

		\~japanese
		CNTServiceが必要とするEvent Sinkオブジェクトです。
		*/
		CServiceParameter m_serviceParam;
	};


	/**  @} */

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
