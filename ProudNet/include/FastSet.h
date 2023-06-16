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

#include "FastMap.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	CFastMap 과 달리 key만을 가지는 클래스이다. 
	- 나머지는 CFastMap 과 성향이 같다. CFastMap 을 참고할 것. 

	\param K 콜렉션의 값 타입
	\param KTraits 콜렉션의 값 타입을 다루는 특성을 정의한 클래스
	\param AllocT AllocType 값 중 하나

	\~english
	Not like CFastMap, this is a class that only possesses key. 
	- Has same nature as CFastMap for the rest. Please refer CFastMap. 

	\param K value type of collection
	\param KTraits class that defines the characteristics that handle value type of collection
	\param AllocT one of the values of AllocType

	\~chinese
	与 CFastMap%不同，只拥有key的类。
	- 剩下的则与 CFastMap%趋向相同。请参考 CFastMap%。

	\param K collection的值类型。
	\param KTraits 定义处理collection值类型的特性的类。
	\param AllocT AllocType值中之一。

	\~japanese
	CfastMapとは違って、キーのみを持つクラスです。 
	- 残りはCFastMapと性向が同じです。CFastMap をご参照ください。 

	\param K コレクションの値タイプ
	\param KTraits コレクションの値タイプを扱う特性を定義したクラス
	\param AllocT AllocType 値の一つ
	\~
	*/
	template < typename K, typename KTraits = CPNElementTraits< K > >
	class CFastSet
	{
		typedef CFastMap<K,char,KTraits, CPNElementTraits< char > > MapType;
		MapType m_intl;
	public:
		class iterator;
		friend class iterator;
		
		/** 
		\~korean
		STL의 iterator와 같은 역할을 한다.

		\~english
		Performs a role as iterator of STL

		\~chinese
		起着与STL的interator一样的作用。

		\~japanese
		STLのiteratorのような役割をします。
		\~
		*/
		class iterator 
		{
			friend class CFastSet;
		public:
			typename MapType::iterator m_iterIntl;

			iterator& operator++()
			{
				// preincrement
				m_iterIntl++;
				return (*this);
			}

			iterator operator++(int)
			{
				// postincrement
				iterator _Tmp = *this;
				m_iterIntl++;
				return (_Tmp);
			}

			inline bool operator==(const iterator& a) const 
			{
				return m_iterIntl == a.m_iterIntl;
			}

			inline bool operator!=(const iterator& a) const 
			{
				return !(m_iterIntl == a.m_iterIntl);
			}

			const K& operator*() const
			{
				return m_iterIntl->GetFirst();
			}

		};

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline iterator begin()
		{
			iterator ret;
			ret.m_iterIntl = m_intl.begin();

			return ret;
		}

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline iterator end()
		{
			iterator ret;
			ret.m_iterIntl = m_intl.end();

			return ret;
		}

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline void insert(const K& val)
		{
			m_intl.Add(val,0);
		}

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline intptr_t GetCount() const
		{
			return m_intl.GetCount();
		}

#if defined(_MSC_VER)       
		__declspec(property(get=GetCount)) intptr_t Count;
#endif

		inline bool empty() const
		{
			return m_intl.IsEmpty();
		}

		inline bool IsEmpty() const
		{
			return m_intl.IsEmpty();
		}

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline void RemoveAll()
		{
			m_intl.Clear();
		}
		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline void Clear()
		{
			m_intl.Clear();
		}

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline iterator erase(iterator iter)
		{
			iterator ret;
			ret.m_iterIntl = m_intl.erase(iter.m_iterIntl);
			return ret;
		}

		/** 
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs a role as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline void erase( const K &key,bool rehashOnNeed = false)
		{
			m_intl.Remove(key,rehashOnNeed);
		}

		/** 
		\~korean
		항목 하나를 찾아서 제거한다.

		\~english
		Finds a clause and removes it

		\~chinese
		找到一个项目并删除。

		\~japanese
		一項目を探して除去します。
		\~
		*/
		inline bool Remove( const K& key,bool rehashOnNeed=false ) 
		{
			return m_intl.Remove(key,rehashOnNeed);
		}

		/** 
		\~korean
		STL의 동명 메서드와 같음

		\~english
		Same as same name method of STL

		\~chinese
		与STL的同名方法一样。

		\~japanese
		STLの同名メソッドと同じです。
		\~
		*/
		inline K front()
		{
			return m_intl.front().GetFirst();
		}

		/** 
		\~korean
		key 가 이 컨테이너에 있으면 true를 리턴한다.

		\~english
		Returns true when key is in this container

		\~chinese
		Key 在container里的话返回true。

		\~japanese
		キーがこのコンテナーにあればtrueをリターンします。
		\~
		*/
		inline bool ContainsKey(const K& key) const
		{
			return m_intl.ContainsKey(key);
		}

		/** 
		\~korean
		새 항목을 추가한다.
		\param key 추가할 항목의 key
		\return 성공적으로 들어갔으면 true, 아니면 false를 리턴한다.

		\~english
		Adds a new clause
		\param key key of the clause to be added
		\return True if successfully entered, otherwise returns false.

		\~chinese
		添加新项目。
		\param key 添加项目的key。
		\return 成功进入的话true，不是的话返回false。

		\~japanese
		新しい項目を追加します。
		\param key 追加する項目のキー
		\return 成功的に入ったらtrue、それではなければfalseをリターンします。
		\~
		*/
		inline bool Add(const K& key)
		{
			if(ContainsKey(key))
				return false;

			m_intl[key] = 0;
			return true;
		}

		void UseFastHeap(CFastHeap* heap)
		{
			m_intl.UseFastHeap(heap);		
		}
	};

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
