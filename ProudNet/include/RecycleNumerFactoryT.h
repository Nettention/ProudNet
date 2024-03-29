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

#include "FastMap.h"
#include "FastList.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** 
	\~korean
	유니크한 정수를 생성하되, 재사용가능한 값을 생성하는 역할을 한다.
	TCP port number처럼, 값을 재사용하되 일정 시간 쿨타임을 두는 형식으로 작동한다.
	VLQ 최적화를 위해 최소한의 값만 생성해서 다룬다.

	\~english TODO:translate needed.
	Generate a unique integer, however it performs the role of generating a recyclable value. 
	Recycle the value as the TCP port number but it operates in the format to have cool time for a certain period of time. 
	To optimize VLQ, generate and handle the minimum value. 
	

	\~chinese
	生成unique的正数，起着生成可再次使用值的作用。
	就像TCP port number，运转为再次使用值，却设置一定的冷却时间的形式。
	为了优化VLQ，只生成最小限制的值并进行处理。


	\~japanese
	ユニークな整数を生成しますが、再使用可能な値を生成する役割をします。
	TCP port numberのように、値を再使用しますが、所定時間のクールタイムを置く形式で作動します。
	VLQの最適化のために最小限の値のみを生成して取り扱います。

	\~
	*/
	template<typename TYPE>
	class CRecycleNumberFactoryT
	{
	// TODO: Proud.CRecycleHostIDFactory 에서 복사해 왔음. 단, 값 범위 지정과 특정 값을 skip하는 기능은 제거했다. 나중에 소스 공통화하자. 
	private:
		class CRecycleBucket
		{
		public:
			TYPE m_number;
			TYPE m_recycleCount;
			int64_t m_droppedTime;

			inline CRecycleBucket() 
			{
				m_number = 0;
				m_recycleCount = 0;
				m_droppedTime = 0;
			}
		};

		// 휴지통에 들어간 값은 일정 시간 동안 재사용 금지. 이것은 그 금지 최소 기간이다.
		double m_issueValidTime;

		// 가장 마지막에 발급한 값
		TYPE m_lastIssuedValue;

		CFastMap<TYPE,CRecycleBucket> m_dropMap; //실제 값들을 관리하기위해 들어감.
		CFastList<TYPE> m_dropList; // 순서를 보장하기 위해 들어감.

		TYPE NewNumber()
		{
			m_lastIssuedValue = m_lastIssuedValue + 1;

			return m_lastIssuedValue;
		}

		TYPE GetRecycleCount( TYPE hostID )
		{
			CFastMap<TYPE,CRecycleBucket>::iterator itr = m_dropMap.find(hostID);

			if(itr != m_dropMap.end())
			{
				return itr->GetSecond().m_recycleCount;
			}

			return 0;
		}

	public:
		CRecycleNumberFactoryT( double issueValidTime )
		{
			m_issueValidTime = issueValidTime;
			m_lastIssuedValue = 1;
		}

		~CRecycleNumberFactoryT()
		{
			m_lastIssuedValue = 1;
			m_dropMap.Clear();
			m_dropList.Clear();
		}

		// 버려진지 얼마 안된 값은 재사용 가능한 최소 대기 시간이 있어서 curTime이 필요
		TYPE Create( int64_t curTime )
		{
			//drop가 된것이 없다면 새로 발급
			if(m_dropList.GetCount() == 0)
			{
				return NewNumber();
			}

			CFastMap<TYPE,CRecycleBucket>::iterator itr = m_dropMap.find(m_dropList.GetHead());

			//droplist에 있는데 map에 없다니...
			if(itr == m_dropMap.end())
			{
				//error! - 이런 상황이 오면 안된다.
				m_dropList.RemoveHead();
				return NewNumber();
			}

			//일정시간이 지난것만 재사용.
			if(curTime - itr->GetSecond().m_droppedTime > m_issueValidTime)
			{
				//재사용에서 나가는 것은 droptime를 0으로 만들어 준다.
				//사용자 실수로 drop를 2 3번 호출하는것을 미연에 방지 하기위해 플래그를 세운다.
				itr->GetSecond().m_droppedTime = 0;
				++(itr->GetSecond().m_recycleCount);

				return m_dropList.RemoveHead();
			}

			//시간이 안되었다면 새로 발급.
			return NewNumber();
		}

		void Drop( int64_t curTime,TYPE dropID )
		{
			CFastMap<TYPE,CRecycleBucket>::iterator itr = m_dropMap.find(dropID);

			//현재 drop가 된것이 있다면, droptime만 update하고 addtail한다.
			if(itr != m_dropMap.end())
			{
				//Create에서는 droptime를 0으로 만들어 준다.
				//사용자 실수로 drop를 2 3번 호출하는것을 미연에 방지 하기위해 플래그를 세운다.
				if(itr->GetSecond().m_droppedTime != 0)
					return;

				itr->GetSecond().m_droppedTime = curTime;

				m_dropList.AddTail(dropID);
				return;
			}

			CRecycleBucket add;
			add.m_droppedTime = curTime;
			add.m_number = dropID;
			add.m_recycleCount = 0;

			m_dropMap.Add(dropID,add);
			m_dropList.AddTail(dropID);
		}


	};

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
