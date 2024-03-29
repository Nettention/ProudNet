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

#ifdef USE_HLA
#include "HlaEntity_S.h"
#include "HlaDelagate_Common.h"

namespace Proud 
{
	class CHlaEntityManagerBase_S;
	class CriticalSection;
	class CHlaSpace_S;
	class IHlaDelegate_S;

	/**
	\~korean
	HLA 세션 서버 메서드 인터페이스입니다.

	\~english TODO:translate needed.
	This is a HLA session server method interface. 

	\~chinese
	是HLA session服务器方法界面。

	\~japanese
	HLAセッションサーバーメソッドインターフェースです。
	\~
	*/
	class IHlaHost_S
	{
	public:
		virtual ~IHlaHost_S() {};

		/** 
		\~korean
		HLA entity class를 등록합니다. 
		HLA entity class는 HLA compiler output이어야 합니다. 

		\~english TODO:translate needed.
		An HLA entity class is registered. 
		An HLA entity class must be HLA complier output. 
	
		\~chinese
		登录HLA entity class。
		HLA entity class得是HLA compiler output。

		\~japanese
		HLA entity classを登録します。
		HLA entity classはHLA compiler outputではなければなりません。

		*/
		virtual void HlaAttachEntityTypes(CHlaEntityManagerBase_S* entityManager) = 0;

		/** 
		\~korean
		이 모듈에 의해 콜백되는 메서드들을 구현한 객체를 받아들입니다. 

		\~english TODO:translate needed.
		This receives the objects that implements the methods called back by this module. 
	
		\~chinese
		在接收体现被此模块的回拨方法的对象。

		\~japanese
		このモジュールによってコールバックされるメソッドを実現したオブジェクトを受け入れます。

		*/
		virtual void HlaSetDelegate(IHlaDelegate_S* dg) = 0;

		/** 
		\~korean
		사용자는 이 함수를 일정 시간마다 콜 해야 합니다. 

		\~english TODO:translate needed.
		Users must call this function at a certain interval. 
	
		\~chinese
		用户要在每隔一段时间拨此函数。

		\~japanese
		ユーザーはこの関数を所定時間ごとに呼び出さなければなりません。

		*/
		virtual void HlaFrameMove() = 0;
		
		virtual CHlaSpace_S* HlaCreateSpace() = 0;
		virtual void HlaDestroySpace(CHlaSpace_S* space) = 0;

		virtual CHlaEntity_S* HlaCreateEntity(HlaClassID classID) = 0;
		virtual void HlaDestroyEntity(CHlaEntity_S* Entity) = 0;

		virtual void HlaViewSpace(HostID viewerID, CHlaSpace_S* space, double levelOfDetail = 1) = 0;
		virtual void HlaUnviewSpace(HostID viewerID, CHlaSpace_S* space) = 0;
	};

	/** 
	\~korean
	HLA 세션 서버에 의해 콜백되는 인터페이스입니다. 
	
	참고
	- C++ 이외 버전에서는 본 메서드는 delegate callback 형태일 수 있습니다. 

	\~english
	This is an interface called back by the HLA session server.

	Note
	- In versions other than C++, this method may be in the form of delegate callback. 
	
	\~chinese
	被HLA会话服务器回调的接口。 
	参照
	- 在C++ 之外的版本中本方法可以是delegate callback形式。 

	\~japanese
	HLAセッションサーバーによってコールバックされるインターフェースです。
	
	参考
	- C++ 以外のバージョンでは、本メソッドはdelegate callback形態である場合があります。

	*/
	class IHlaDelegate_S:public IHlaDelegate_Common
	{
	public:
		virtual ~IHlaDelegate_S() {}

	};
}
#endif
#ifdef _MSC_VER
#pragma pack(pop)
#endif