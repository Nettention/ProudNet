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

//#if defined(_WIN32)
    #include <deque>
    #include "FastArrayPtr.h"
//#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
//#if defined(_WIN32)
	/** \addtogroup util_group
	*  @{
	*/
	template<typename T>
	class Deque: public std::deque<T>
	{
        typedef typename std::deque<T> DeqType;
        
	public:
		/**
		\~korean 
		\return Data의 수를 리턴합니다.

		\~english 
		\return the number of data.

		\~chinese
		\return 返回data的数。

		\~japanese
		\return Dataの数をリターンします。
		\~
		*/
		int GetCount() const
		{
			return (int)DeqType::size();
		}
#if defined (_MSC_VER)
		__declspec(property(get = GetCount)) int Count;
#endif

        inline bool IsEmpty() const { return DeqType::empty(); }

		/**
		\~korean 
		큐에 Data를 추가합니다.
		\param data 추가할 Data입니다.

		\~english TODO:translate needed.		
		\param data Data to be added 

		\~chinese
		队列里添加Data。
		\param data 要添加的Data。

		\~japanese
		キューにデータを追加します。
		\param data 追加するデータです。
		\~
		*/
		void PushBack(const T& data)
		{
			this->push_back(data);
		}

		/**
		\~korean 
		가장 앞에 있는 Data를 복사받습니다.
		\param [out] ret 복사받을 reference입니다.

		\~english TODO:translate needed.
		Get the very front data copied. 
		\param [out] ret This is a reference to be copied

		\~chinese
		复制最前面的data。
		\param [out] ret 得到复制的reference。

		\~japanese
		一番前にあるデータをコピーしてもらいます。
		\param [out] ret コピーしてもらうreferenceです。
		\~
		*/
		void PeekFront(T& ret)
		{
			ret = DeqType::front();
		}

		/**
		\~korean 
		\return 가장 앞의 데이터

		\~english 
		\return The first data.

		\~chinese
		\return 最前面的数据。

		\~japanese
		\return 一番前のデータ
		\~
		*/
		T PeekFront()
		{
			return DeqType::front();
		}

		/**
		\~korean 
		가장 앞의 데이터를 큐에서 제거하고 리턴받습니다.
		\return 가장 앞에 있었던 데이터

		\~english 
		Removes the very front data from the queue and get it returned. 
		\return The very front data.

		\~chinese
		从队列里删除最前面的数据后返回。
		\return 在最前面的数据。

		\~japanese
		一番前のデータをキューより除去してリターンしてもらいます。
		\return 一番前にあったデータ
		\~
		*/
		T PopFront()
		{
			T ret = DeqType::front();
            DeqType::pop_front();
			return ret;
		}

		/**
		\~korean
		가장 앞의 데이터를 큐에서 제거하고 reference로 받습니다.
		\param [out] ret Data를 받을 reference

		\~english 
		Deletes the very first data from the queue and get it as reference. 
		\param [out] ret             Reference to receive data

		\~chinese
		从队列里删除最前面的数据后，接收reference。
		\param [out] ret           接收Data的reference。

		\~japanese
		一番前のデータをキューより除去してreferenceとしてもらいます。
		\param [out] ret           Dataを受けるreference
		\~
		*/
		void PopFront(T& ret)
		{
			ret = DeqType::front();
            DeqType::pop_front();
		}

		/**
		\~korean
		모든 데이터 정보를 제거합니다.

		\~english 
		Deletes all data information.

		\~chinese
		删除所有数据信息。

		\~japanese
		全てのデータ情報を除去します。
		\~
		*/
		void Clear()
		{
            DeqType::clear();
		}

		// 이 메서드는 deque.iterator 자체가 느리기 때문에 비효율적이다.
		// 		void CloneToArray(CFastArray<T> &dest)
		// 		{
		// 			dest.SetCount(size());
		// 			int c=0;
		// 			for(iterator i=begin();i!=end();i++)
		// 			{
		// 				dest[c++]=*i;
		// 			}
		// 		}
	};

	/**  @} */
//#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
