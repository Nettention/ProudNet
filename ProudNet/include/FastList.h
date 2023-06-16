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

#include <assert.h>
#include "Ptr.h"
#include "BasicTypes.h"
#include "Singleton.h"
#include "Enums.h"

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
	extern const char* CannotUseFastHeapForFilledCollectionErrorText;

	/** \addtogroup util_group
	*  @{
	*/

	/**

	\~korean
	매우 빠르게 처리 되는 Linked List.
	- 내부적으로 CFastHeap 을 사용하십시오. 그러면 element를 추가/삭제 하는 비용이 줄어들것입니다.
	단, <a target="_blank" href="http://guide.nettention.com/cpp_ko#fast_heap" >Fast heap</a> 에 명시된 주의사항을 준수하십시오.
	- CFastArray 와는 다르게 class 들을 생성자, 소멸자, 복사 대입 연산자와 함께 사용할 수 있습니다.

	\param E collection의 항목 타입.
	\param ETraits Collection 항목을 다루는 특성.
	\param AllocT AllocType 값 중 하나.

	\~english
	Linked List that can be processed with extremely fast.
	- Uses CFastHeap internally. Therefore, has little cost at adding/deleting element.
	  However, it must correctly follow all the rules described in <a target="_blank" href="http://guide.nettention.com/cpp_en#fast_heap" >Fast heap</a>.
	- In contrast with CFastArray, classes can be used with constructor, destructor and copy assignment operator.

	\param E clause type of collection
	\param ETraits characteristic that handles collection clause
	\param AllocT one of the values of AllocType

	\~chinese
	是被快速处理的Linked List。
	-请在内部使用CFastHeap。这样可以减少对element的添加/删除费用。
	但请遵守<a target="_blank" href="http://guide.nettention.com/cpp_zh#fast_heap" >Fast heap</a>的明示内容。
	-与CFastArray不同，可以同时使用class与Constructor、Destructor、复制赋值运算符。

	\param E Collection的项目类型。
	\param ETraits 操作Collection项目的特性。
	\param AllocT AllocType值之一。

	\~japanese
	すごく速い速度で処理される Linked List.
	- 内部的に CFastHeapを使用してください。そうすると elementを追加/削除する費用を減らせます。
	ただし、 <a target="_blank" href="http://guide.nettention.com/cpp_jp#fast_heap" >Fast heap</a>に明示されている注意事項を遵守してください。
	- CFastArray とは違って classたちを構築子(コンストラクタ)、消去子（デストラクタ）, Copy 代入演算子(Copy assignment operator)と一緒に使用できます。

	\param E collectionの項目タイプ
	\param ETraits Collection 項目を扱う特性
	\param AllocT AllocType 値の中の1つ

	\~
	*/
	template < typename E, typename ETraits = CPNElementTraits< E > >
	class CFastList
	{
	public:
		typedef typename ETraits::INARGTYPE INARGTYPE;

	private:
		class CNode : public __Position
		{
		public:
			inline CNode()
			{
			}
			inline CNode( INARGTYPE element)
				:m_element( element )
			{
			}
			inline ~CNode() throw()
			{
			}

		public:
			CNode* m_pNext;
			CNode* m_pPrev;
			E m_element;

		private:
			CNode( const CNode& ) throw();
		};

	public:

		CFastList() throw()
		{
			m_nElements = 0;
			m_pHead = NULL;
			m_pTail = NULL;
		}

#if defined(_MSC_VER)
		__declspec(property(get = GetCount)) intptr_t Count;
#endif

		inline intptr_t GetCount() const throw()
		{
			return( m_nElements );
		}

		/**
		\~korean
		\return 아무런 데이터가 없으면 true, 존재하면 false를 리턴

		\~english 
		If there is no data then true, otherwise return false. 

		\~chinese
		\return 没有任何数据的话true，存在的话返回false。

		\~japanese
		\return 何らのデータがなければtrue、存在するとfalseをリターンします。
		\~
		*/
		inline bool IsEmpty() const throw()
		{
			return( m_nElements == 0 );
		}

		/**
		\~korean
		\return list의 첫번째 data를 리턴합니다.

		\~english 
		Return the first data in the \return list. 

		\~chinese
		\return 返回list的第一个data。

		\~japanese
		\return listの最初データをリターンします。
		\~
		*/
		inline E& GetHead()
		{
			assert( m_pHead != NULL );
			return( m_pHead->m_element );
		}

		/**
		\~korean
		\return list의 첫번째 data를 const 변수로 리턴합니다.

		\~english 
		\return the first data in list as a const variable. 

		\~chinese
		\return 返回list的第一个data为const变数。

		\~japanese
		\return listの最初データをconst変数にリターンします。
		\~
		*/
		inline const E& GetHead() const
		{
			assert( m_pHead != NULL );
			return( m_pHead->m_element );
		}

		/**
		\~korean
		\return list의 마지막 Data를 리턴합니다.

		\~english 
		\return the last data in list. 

		\~chinese
		\return 返回list的最后data。

		\~japanese
		\return listの最後データをリターンします。
		\~
		*/
		inline E& GetTail()
		{
			assert( m_pTail != NULL );
			return( m_pTail->m_element );
		}

		/**
		\~korean
		\return list의 마지막 Data를 const 변수로 리턴합니다.

		\~english 
		\return the last data in list as a const variable. 

		\~chinese
		\return 返回list的最后data为const变数。

		\~japanese
		\return listの最後データをconst変数にリターンします。
		\~
		*/
		inline const E& GetTail() const
		{
			assert( m_pTail != NULL );
			return( m_pTail->m_element );
		}

		/**
		\~korean
		list 의 첫번째 Data를 list내에서 제거하고 리턴해 줍니다.
		\return list에서 제거된 data

		\~english 
		Deletes the first data within the list and returns it. 
		\return Data deleted from \return list

		\~chinese
		从list内删除并返回list的第一个data。
		\return 在list被删除的data。

		\~japanese
		Listの最初データをリスト内より除去してリターンします。
		\return listより除去されたデータ 
		\~
		*/
		inline E RemoveHead()
		{
			assert( m_pHead != NULL );

			CNode* pNode = m_pHead;
			E element( pNode->m_element );

			m_pHead = pNode->m_pNext;
			if ( m_pHead != NULL )
			{
				m_pHead->m_pPrev = NULL;
			}
			else
			{
				m_pTail = NULL;
			}
			FreeNode( pNode );

			return( element );
		}

		/**
		\~korean
		list 의 마지막 Data를 list내에서 제거하고 리턴해 줍니다.
		\return list에서 제거된 data

		\~english 
		Deletes the first data within the list and returns it. 
		\return Data deleted from \return list

		\~chinese
		从list内删除并返回list的最后data。
		\return 在list被删除的data。

		\~japanese
		Listの最後のデータをリスト内で除去してリターンします。
		\return listより除去されたデータ
		\~
		*/
		inline E RemoveTail()
		{
			assert( m_pTail != NULL );

			CNode* pNode = m_pTail;

			E element( pNode->m_element );

			m_pTail = pNode->m_pPrev;
			if ( m_pTail != NULL )
			{
				m_pTail->m_pNext = NULL;
			}
			else
			{
				m_pHead = NULL;
			}
			FreeNode( pNode );

			return( element );
		}

		/**
		\~korean
		list의 첫번째 data를 제거합니다.

		\~english 
		First data of the list is deleted. 

		\~chinese
		删除list的第一个data。

		\~japanese
		Listの最初データを除去します。
		\~
		*/
		inline void RemoveHeadNoReturn() throw()
		{
			assert( m_pHead != NULL );

			CNode* pNode = m_pHead;

			m_pHead = pNode->m_pNext;
			if ( m_pHead != NULL )
			{
				m_pHead->m_pPrev = NULL;
			}
			else
			{
				m_pTail = NULL;
			}
			FreeNode( pNode );
		}

		/**
		\~korean
		list의 마지막 data를 제거합니다.

		\~english 
		Last data of the list is deleted. 

		\~chinese
		删除list的最后data。

		\~japanese
		Listの最後データを除去します。
		\~
		*/
		inline void RemoveTailNoReturn() throw()
		{
			assert( m_pTail != NULL );

			CNode* pNode = m_pTail;

			m_pTail = pNode->m_pPrev;
			if ( m_pTail != NULL )
			{
				m_pTail->m_pNext = NULL;
			}
			else
			{
				m_pHead = NULL;
			}
			FreeNode( pNode );
		}

		/**
		\~korean
		Data가 비어있는 해더를 추가한다.
		\return 새로 추가된 노드의 Position

		\~english 
		An empty header without data is added. 
		\return Position of the newly added node 

		\~chinese
		添加data为空的header。
		\return 新添加的节点Position。

		\~japanese
		データが空いているヘッダーを追加します。
		\return 新たに追加されたノードのPosition
		\~
		*/
		inline Proud::Position AddHead()
		{
			CNode* pNode = NewNode( NULL, m_pHead );
			if ( m_pHead != NULL )
			{
				m_pHead->m_pPrev = pNode;
			}
			else
			{
				m_pTail = pNode;
			}
			m_pHead = pNode;

			return( Proud::Position( pNode ) );
		}

		/**
		\~korean
		헤더를 추가합니다.
		\param element 새로운 헤더에 넣을 Data
		\return 새로 추가된 노드의 Position

		\~english 
		Add a header 
		\param element Data to be inserted to a new header
		\return Position of a newly added node

		\~chinese
		添加header。
		\param element 往新的header输入的data。
		\return 新添加的节点的Position。

		\~japanese
		ヘッダーを追加します。
		\param element 新しいヘッダーに入れるデータ
		\return 新たに追加されたノードのPosition
		\~
		*/
		inline Proud::Position AddHead( INARGTYPE element )
		{
			CNode* pNode;

			pNode = NewNode( element, NULL, m_pHead );

			if ( m_pHead != NULL )
			{
				m_pHead->m_pPrev = pNode;
			}
			else
			{
				m_pTail = pNode;
			}
			m_pHead = pNode;

			return( Proud::Position( pNode ) );
		}

		/**
		\~korean
		헤더에 새로운 list를 추가합니다.
		\param plNew 헤더에 추가할 list

		\~english 
		A new list is added to the header
		\param plNew List to be added to the header 

		\~chinese
		往header添加新的list。
		\param plNew 要往header添加的list。

		\~japanese
		ヘッダーに新しいListを追加します。
		\param plNew ヘッダーに追加するList
		\~
		*/
		inline void AddHeadList( const CFastList* plNew )
		{
			assert( plNew != NULL );

			Proud::Position pos = plNew->GetTailPosition();
			while ( pos != NULL )
			{
				INARGTYPE element = plNew->GetPrev( pos );
				AddHead( element );
			}
		}

		/**
		\~korean
		list의 마지막에 빈 노드를 추가한다.
		\return 새로 추가된 노드의 Position

		\~english 
		An empty node is added to the end of the list. 
		\return Position of the newly added node. 

		\~chinese
		往list的最后添加空节点。
		\return 新添加的节点Position。

		\~japanese
		Listの最後に空ノードを追加します。
		\return 新たに追加されたノードのPosition
		\~
		*/
		inline Proud::Position AddTail()
		{
			CNode* pNode = NewNode( m_pTail, NULL );
			if ( m_pTail != NULL )
			{
				m_pTail->m_pNext = pNode;
			}
			else
			{
				m_pHead = pNode;
			}
			m_pTail = pNode;

			return( Proud::Position( pNode ) );
		}

		/**
		\~korean
		list의 마지막에 node를 추가
		\param element 마지막 노드에 삽입을 Data
		\return 새로 추가된 노드의 Position

		\~english 
		Add a node at the end of the list
		\param element Data to be inserted to the last node 
		\return Position of the newly added node

		\~chinese
		往list的最后添加node。
		\param element 往最后节点插入的data。
		\return 新添加的节点的Position。

		\~japanese
		listの最後にnodeを追加
		\param element 最後のノードに挿入するデータ
		\return 新たに追加されたノードのPosition
		\~
		*/
		inline Proud::Position AddTail( INARGTYPE element )
		{
			CNode* pNode;

			pNode = NewNode( element, m_pTail, NULL );

			if ( m_pTail != NULL )
			{
				m_pTail->m_pNext = pNode;
			}
			else
			{
				m_pHead = pNode;
			}
			m_pTail = pNode;

			return( Proud::Position( pNode ) );
		}

		/**
		\~korean
		List의 마지막노드에 새로운 List를 추가
		\param plNew 추가할 List

		\~english 
		A new list is added to the last node of the list
		\param plNew List to be added 

		\~chinese
		往list的最后节点添加新的list。
		\param plNew 要添加的list。

		\~japanese
		Listの最後ノードに新しいリストを追加
		\param plNew 追加するList
		\~
		*/
		inline void AddTailList( const CFastList* plNew )
		{
			assert( plNew != NULL );

			Proud::Position pos = plNew->GetHeadPosition();
			while ( pos != NULL )
			{
				INARGTYPE element = plNew->GetNext( pos );
				AddTail( element );
			}
		}

		/**
		\~korean
		모든 데이터를 제거. Clear()와 같음.

		\~english 
		Delete all data. Same as Clear()

		\~chinese
		删除所有数据。与Clear()一样。

		\~japanese
		全てのデータを除去。 Clear()と同じです。
		\~
		*/
		inline void RemoveAll() throw()
		{
			while ( m_nElements > 0 )
			{
				CNode* pKill = m_pHead;
				assert( pKill != NULL );

				m_pHead = m_pHead->m_pNext;
				FreeNode( pKill );
			}

			assert( m_nElements == 0 );
			m_pHead = NULL;
			m_pTail = NULL;
		}

		/**
		\~korean
		모든 데이터를 제거.RemoveAll()같음.

		\~english 
		Delete all data. Same as RemoveAll()

		\~chinese
		删除所有数据。与RemoveAll()一样。

		\~japanese
		全てのデータを除去。RemoveAll()と同じです。
		\~
		*/
		inline void Clear() { RemoveAll(); }

		/**
		\~korean
		\return 헤더의 Position을 리턴

		\~english 
		\return the position of the header.

		\~chinese
		\return 返回header的Position。

		\~japanese
		\return ヘッダーのPositionをリターン
		\~
		*/
		inline Proud::Position GetHeadPosition() const throw()
		{
			return( Proud::Position( m_pHead ) );
		}

		/**
		\~korean
		\return list의 마지막 노드의 Position을 리턴

		\~english 
		\return the position of the last node of list.

		\~chinese
		\return 返回llist的最后节点的Position。
		\~japanese
		\return listの最後ノードのPositionをリターン
		\~
		*/
		inline Proud::Position GetTailPosition() const throw()
		{
			return( Proud::Position( m_pTail ) );
		}

		/**
		\~korean
		인자로 넣은 Position의 다음 node Position을 얻는다.
		\param pos 현재 Position
		\return 다음 노드의 Data를 리턴

		\~english 
		Obtain the next node position of the position inserted as a factor 
		\param pos Current position.
		\return the data of the next node.

		\~chinese
		获得输入为因子的Position之后的node Position。
		\param pos 现在的Position。
		\return 返回之后节点的data。

		\~japanese
		\~
		*/
		inline E& GetNext( Proud::Position& pos )
		{
			CNode* pNode;

			assert( pos != NULL );
			pNode = (CNode*)pos;
			pos = Proud::Position( pNode->m_pNext );

			return( pNode->m_element );
		}

		/**
		\~korean
		인자로 넣은 Position의 다음 node Position을 얻는다.
		\param pos 현재 Position
		\return 다음 노드의 Data를 const변수로 리턴

		\~english 
		Obtain the next node position of the position inserted as a factor
		\param pos Current position
		\return Return the data of the next node

		\~chinese
		获得输入为因子的Position之后的node Position。
		\param pos 现在的Position。
		\return 返回之后节点的data为const变数。

		\~japanese
		因子で入れたPositionの次のnode Positionを得ます。
		\param pos 現在 Position
		\return 次のノードのデータをconst変数にリターン
		\~
		*/
		inline const E& GetNext( Proud::Position& pos ) const
		{
			CNode* pNode;

			assert( pos != NULL );
			pNode = (CNode*)pos;
			pos = Proud::Position( pNode->m_pNext );

			return( pNode->m_element );
		}

		/**
		\~korean
		인자로 넣은 Position의 전 node Position을 얻는다.
		\param pos 현재 Position
		\return 전 노드의 Data

		\~english 
		Obtain the previous node position of the position inserted as a factor
		\param pos Current position
		\return Data of the previous node 

		\~chinese
		因子で入れたPositionの後のnode Positionを得ます。
		\param pos 现在的Position。
		\return 之前节点的data。

		\~japanese
		因子で入れたPositionの後のnode Positionを得ます。
		\param pos 現在 Position
		\return 前ノードのData
		\~
		*/
		inline E& GetPrev( Proud::Position& pos )
		{
			CNode* pNode;

			assert( pos != NULL );
			pNode = (CNode*)pos;
			pos = Proud::Position( pNode->m_pPrev );

			return( pNode->m_element );
		}

		/**
		\~korean
		인자로 넣은 Position의 전 node Position을 얻는다.
		\param pos 현재 Position
		\return 전 노드의 Data를 const 변수로 리턴한다.

		\~english 
		Obtain the previous node position of the position inserted as a factor
		\param pos Current position 
		\return Return the data of the previous node as a const variable 

		\~chinese
		获得输入为因子的Position之前的node Position。
		\param pos 现在的Position。
		\return 返回之前节点的data为const变数。

		\~japanese
		因子で入れたPositionの後のnode Positionを得ます。
		\param pos 現在 Position
		\return 前ノードのデータをconst 変数にリターンします。
		\~
		*/
		inline const E& GetPrev( Proud::Position& pos ) const throw()
		{
			CNode* pNode;

			assert( pos != NULL );
			pNode = (CNode*)pos;
			pos = Proud::Position( pNode->m_pPrev );

			return( pNode->m_element );
		}

		/**
		\~korean
		현재 노드의 Data 를 얻는다.
		\param pos 현재 노드의 Position
		\return 현재 노드의 Data

		\~english 
		Obtain the data of the current node 
		\param pos Position of the current node
		\return Data of the current node 

		\~chinese
		获得现在节点的data。
		\param pos 现在节点的Position。
		\return 现在节点的data。

		\~japanese
		現在ノードのデータを得ます。
		\param pos 現在ノードのPosition
		\return 現在ノードのData
		\~
		*/
		inline E& GetAt( Proud::Position pos )
		{
			assert( pos != NULL );
			CNode* pNode = (CNode*)pos;
			return( pNode->m_element );
		}

		/**
		\~korean
		현재 노드의 Data 를 const 변수로 얻는다.
		\param pos 현재 노드의 Position
		\return 현재 노드의 Data의 const 변수 리턴

		\~english 
		Obtain the data of the current node as a const variable
		\param pos Position of the current node 
		\return Return the const variable of the current node data

		\~chinese
		获得现在节点的data为const变数。
		\param pos 现在节点的Position。
		\return 返回现在节点的data为const变数。

		\~japanese
		現在ノードのデータをconst変数から得る。
		\param pos 現在ノードの Position
		\return 現在ノードのデータのconst変数をリターン
		\~
		*/
		inline const E& GetAt( Proud::Position pos ) const
		{
			assert( pos != NULL );
			CNode* pNode = (CNode*)pos;
			return( pNode->m_element );
		}

		/**
		\~korean
		현재 Position이 가리키는 노드의 Data를 세팅한다.
		\param pos 현재 노드의 Position
		\param element 세팅할 data

		\~english 
		Configure the data of the node pointed by the current position
		\param pos Position of the current node 
		\param element Data to be configured 

		\~chinese
		设置现在Position所指节点的data。
		\param pos 现在节点的Position。
		\return 要设置的data。


		\~japanese
		現在Positionが指すノードのデータを設定します。
		\param pos 現在ノードの Position
		\param element 設定するデータ
		\~
		*/
		inline void SetAt( Proud::Position pos, INARGTYPE element )
		{
			assert( pos != NULL );
			CNode* pNode = (CNode*)pos;
			pNode->m_element = element;
		}

		/**
		\~korean
		현재 Position이 가르키는 node 를 제거하고 다음 Position을 리턴해 준다.
		\param pos 제거하고자 하는 node의 Position
		\return 제거된 Position의 다음 Position

		\~english 
		Delete the node pointed by the current position and then return the next position
		\param pos Position of the node to be deleted
		\return Next position of the deleted position 

		\~chinese
		删除现在Position所指的node，返回之后Position。
		\param pos 想删除的node的Position。
		\return 被删除的Position的之后Position。


		\~japanese
		現在Positionが指すノードを除去して次のPositionをリターンします。
		\param pos 除去したいノードの Position
		\return 除去されたPositionの次のPosition
		\~
		*/
		inline Proud::Position RemoveAt( Proud::Position pos ) throw()
		{
			Proud::Position newPos = pos;

			GetNext(newPos);  // 다음 위치를 미리 얻어둔다.
			assert( pos != NULL );

			CNode* pOldNode = (CNode*)pos;

			// remove pOldNode from list
			if ( pOldNode == m_pHead )
			{
				m_pHead = pOldNode->m_pNext;
			}
			else
			{
#if defined(_WIN32)
				assert( AtlIsValidAddress( pOldNode->m_pPrev, sizeof(CNode) ));
#endif
				pOldNode->m_pPrev->m_pNext = pOldNode->m_pNext;
			}
			if ( pOldNode == m_pTail )
			{
				m_pTail = pOldNode->m_pPrev;
			}
			else
			{
#if defined(_WIN32)
				assert( AtlIsValidAddress( pOldNode->m_pNext, sizeof(CNode) ));
#endif
				pOldNode->m_pNext->m_pPrev = pOldNode->m_pPrev;
			}
			FreeNode( pOldNode );

			return newPos;
		}

		/**
		\~korean
		현재 Position이 가리키는 node의 앞에 새로운 node를 추가한다.
		\param pos 현재 node를 가리키는 Position
		\param element 추가할 새 노드의 Data

		\~english 
		Add a new node in front of the node pointed by the current position 
		\param pos Position pointing the current node 
		\param element Data of the new node to be added 

		\~chinese
		往现在Position所指的node前面添加新的node。
		\param pos 指向现在node的Position。
		\param element 要添加的新节点的data。

		\~japanese
		現在Positionが指すノードの前に新しいノードを追加します。
		\param pos 現在ノードを指すPosition
		\param element 追加する新しいノードのデータ
		
		\~
		*/
		inline Proud::Position InsertBefore( Proud::Position pos, INARGTYPE element )
		{
			assert(this != nullptr);

			if ( pos == NULL )
				return AddHead( element ); // insert before nothing -> head of the list

			// Insert it before position
			CNode* pOldNode = (CNode*)pos;
			CNode* pNewNode = NewNode( element, pOldNode->m_pPrev, pOldNode );

			if ( pOldNode->m_pPrev != NULL )
			{
#if defined(_WIN32)
				assert(AtlIsValidAddress(pOldNode->m_pPrev, sizeof(CNode)));
#endif
				pOldNode->m_pPrev->m_pNext = pNewNode;
			}
			else
			{
				assert( pOldNode == m_pHead );
				m_pHead = pNewNode;
			}
			pOldNode->m_pPrev = pNewNode;

			return( Proud::Position( pNewNode ) );
		}

		/**
		\~korean
		현재 Position이 가리키는 node의 뒤에 새로운 node를 추가한다.
		\param pos 현재 node를 가리키는 Position
		\param element 추가할 새 노드의 Data

		\~english 
		Add a new node behind the node pointed by the current position 
		\param pos Position pointing the current node 
		\param element Data of the new node to be added 

		\~chinese
		往现在Position所指的node后面添加新的node。
		\param pos 指向现在node的Position。
		\param element 要添加的新节点的data。

		\~japanese
		現在Positionが指すノードの後に新しいノードを追加します。
		\param pos 現在ノードを指すPosition
		\param element 追加する新しいノードのデータ
		\~
		*/
		inline Proud::Position InsertAfter( Proud::Position pos, INARGTYPE element )
		{
			assert(this != nullptr);

			if ( pos == NULL )
				return AddTail( element ); // insert after nothing -> tail of the list

			// Insert it after position
			CNode* pOldNode = (CNode*)pos;
			CNode* pNewNode = NewNode( element, pOldNode, pOldNode->m_pNext );

			if ( pOldNode->m_pNext != NULL )
			{
				assert(AtlIsValidAddress(pOldNode->m_pNext, sizeof(CNode)));
				pOldNode->m_pNext->m_pPrev = pNewNode;
			}
			else
			{
				assert( pOldNode == m_pTail );
				m_pTail = pNewNode;
			}
			pOldNode->m_pNext = pNewNode;

			return( Proud::Position( pNewNode ) );
		}

		/**
		\~korean
		Data로 node를 찾는다.
		\param element 찾을 node의 data
		\param posStartAfter 이 Position이후부터 비교하여 찾는다.

		\~english 
		Find a node with data
		\param element Data of the node to be searched for
		\param posStartAfter Search by comparing after this position

		\~chinese
		以data寻找node。
		\param element 要找的node的data。
		\param posStartAfter 从这个Position之后开始比较寻找。

		\~japanese
		データでノードを探します。
		\param element 探すノードのデータ
		\param posStartAfter このPosition以降から比較して探します。
		\~
		*/
		inline Proud::Position Find( INARGTYPE element, Proud::Position posStartAfter = NULL ) const throw()
		{
			assert(this != nullptr);

			CNode* pNode = (CNode*)posStartAfter;
			if ( pNode == NULL )
			{
				pNode = m_pHead;  // start at head
			}
			else
			{
#ifdef _WIN32
				assert(AtlIsValidAddress(pNode, sizeof(CNode)));
#endif
				pNode = pNode->m_pNext;  // start after the one specified
			}

			for ( ; pNode != NULL; pNode = pNode->m_pNext )
			{
				if ( ETraits::CompareElements( pNode->m_element, element ) )
					return( Proud::Position( pNode ) );
			}

			return( NULL );
		}

		/**
		\~korean
		헤더로 부터 iElement갯수 만큼의 다음 node의 Position을 리턴
		\param iElement 다음으로 넘어갈 노드의 갯수
		\return node의 Position

		\~english 
		Return the position of the next node for the number if iElement from the header 
		\param iElement Number of the node to be passed after iElement
		\return Position of node

		\~chinese
		返回从header开始相当于iElement个数之后node的Position。
		\param iElement 要跳过去的节点的个数。
		\return node的Position。

		\~japanese
		ヘッダーからiElement数だけの次のノードのPositionをリターン
		\param iElement 次に越えるノードの数
		\return nodeのPosition
		\~
		*/
		inline Proud::Position FindIndex( intptr_t iElement ) const throw()
		{
			assert(this != nullptr);

			if ( iElement >= m_nElements )
				return NULL;  // went too far

			if (m_pHead == NULL)
				return NULL;

			CNode* pNode = m_pHead;
			for ( intptr_t iSearch = 0; iSearch < iElement; iSearch++ )
			{
				pNode = pNode->m_pNext;
			}

			return( Proud::Position( pNode ) );
		}

		/**
		\~korean
		현재 Position이 가리키는 node를 list의 처음으로 보낸다.
		\param pos 현재 node의 Position

		\~english 
		Send the node pointed by the current position to the beginning of the list 
		\param pos Position of the current node 

		\~chinese
		把现在的Position所指的node发送到list的最前面。
		\param pos 现在node的Position。

		\~japanese
		現在Positionが指すノードをリストの最初へ送ります。
		\param pos 現在ノードのPosition
		\~
		*/
		inline void MoveToHead( Proud::Position pos )
		{
			assert( pos != NULL );

			CNode* pNode = static_cast< CNode* >( pos );

			if ( pNode == m_pHead )
			{
				// Already at the head
				return;
			}

			if ( pNode->m_pNext == NULL )
			{
				assert( pNode == m_pTail );
				m_pTail = pNode->m_pPrev;
			}
			else
			{
				pNode->m_pNext->m_pPrev = pNode->m_pPrev;
			}

			assert( pNode->m_pPrev != NULL );  // This node can't be the head, since we already checked that case
			pNode->m_pPrev->m_pNext = pNode->m_pNext;

			m_pHead->m_pPrev = pNode;
			pNode->m_pNext = m_pHead;
			pNode->m_pPrev = NULL;
			m_pHead = pNode;
		}

		/**
		\~korean
		현재 Position이 가리키는 node를 list의 마지막으로 보낸다.
		\param pos 현재 node의 Position

		\~english 
		Send the node pointed by the current position to the end of the list 
		\param pos Position of the current node 

		\~chinese
		把现在Position所指的node发送的list的最后。
		\param pos 现在node的Position。

		\~japanese
		現在Positionが指すノードをリストの最後へ送ります。
		\param pos 現在ノードのPosition
		\~
		*/
		inline void MoveToTail( Proud::Position pos )
		{
			assert( pos != NULL );
			CNode* pNode = static_cast< CNode* >( pos );

			if ( pNode == m_pTail )
			{
				// Already at the tail
				return;
			}

			if ( pNode->m_pPrev == NULL )
			{
				assert( pNode == m_pHead );
				m_pHead = pNode->m_pNext;
			}
			else
			{
				pNode->m_pPrev->m_pNext = pNode->m_pNext;
			}

			pNode->m_pNext->m_pPrev = pNode->m_pPrev;

			m_pTail->m_pNext = pNode;
			pNode->m_pPrev = m_pTail;
			pNode->m_pNext = NULL;
			m_pTail = pNode;
		}

		/**
		\~korean
		두 노드의 위치를 바꾼다. Elenemt를 직접 Swap해주게되면 큰 낭비가 있을 수 있음으로 두 노드에 해당 포인터 들을 바꾸어준다.
		\param pos1 바꿀 첫번째 node의 Position
		\param pos2 바꿀 두번째 node의 Position

		\~english 
		Switch the position of the two nodes. As a big waste may occur if you directly swap the elements so you change the relevant pointers of the two nodes. 
		\param pos1 Position of the first node to be changed 
		\param pos2 Position of the second node to be changed

		\~chinese
		交换两个节点的位置。如果直接Swap Elenemt的话，会成为很大浪费，交换两个相关节点的指针。
		\param pos1 交换的第一个node的Position。
		\param pos2 交换的第二个node的Position。

		\~japanese
		両ノードの位置を変えます。Elementを直接Swapすると無駄遣いになれますので、両ノードに該当ポインターを変えてくれます。
		\param pos1 変える最初ノードのPosition
		\param pos2 変える2番目ノードのPosition
		\~
		*/
		void SwapElements( Proud::Position pos1, Proud::Position pos2 ) throw()
		{
			assert( pos1 != NULL );
			assert( pos2 != NULL );

			if ( pos1 == pos2 )
			{
				// Nothing to do
				return;
			}

			CNode* pNode1 = static_cast< CNode* >( pos1 );
			CNode* pNode2 = static_cast< CNode* >( pos2 );
			if ( pNode2->m_pNext == pNode1 )
			{
				// Swap pNode2 and pNode1 so that the next case works
				CNode* pNodeTemp = pNode1;
				pNode1 = pNode2;
				pNode2 = pNodeTemp;
			}
			if ( pNode1->m_pNext == pNode2 )
			{
				// Node1 and Node2 are adjacent
				pNode2->m_pPrev = pNode1->m_pPrev;
				if ( pNode1->m_pPrev != NULL )
				{
					pNode1->m_pPrev->m_pNext = pNode2;
				}
				else
				{
					assert( m_pHead == pNode1 );
					m_pHead = pNode2;
				}
				pNode1->m_pNext = pNode2->m_pNext;
				if ( pNode2->m_pNext != NULL )
				{
					pNode2->m_pNext->m_pPrev = pNode1;
				}
				else
				{
					assert( m_pTail == pNode2 );
					m_pTail = pNode1;
				}
				pNode2->m_pNext = pNode1;
				pNode1->m_pPrev = pNode2;
			}
			else
			{
				// The two nodes are not adjacent
				CNode* pNodeTemp;

				pNodeTemp = pNode1->m_pPrev;
				pNode1->m_pPrev = pNode2->m_pPrev;
				pNode2->m_pPrev = pNodeTemp;

				pNodeTemp = pNode1->m_pNext;
				pNode1->m_pNext = pNode2->m_pNext;
				pNode2->m_pNext = pNodeTemp;

				if ( pNode1->m_pNext != NULL )
				{
					pNode1->m_pNext->m_pPrev = pNode1;
				}
				else
				{
					assert( m_pTail == pNode2 );
					m_pTail = pNode1;
				}
				if ( pNode1->m_pPrev != NULL )
				{
					pNode1->m_pPrev->m_pNext = pNode1;
				}
				else
				{
					assert( m_pHead == pNode2 );
					m_pHead = pNode1;
				}
				if ( pNode2->m_pNext != NULL )
				{
					pNode2->m_pNext->m_pPrev = pNode2;
				}
				else
				{
					assert( m_pTail == pNode1 );
					m_pTail = pNode2;
				}
				if ( pNode2->m_pPrev != NULL )
				{
					pNode2->m_pPrev->m_pNext = pNode2;
				}
				else
				{
					assert( m_pHead == pNode1 );
					m_pHead = pNode2;
				}
			}
		}

#ifdef _DEBUG
		void AssertValid() const
		{
			if ( IsEmpty() )
			{
				// empty list
				assert(m_pHead == NULL);
				assert(m_pTail == NULL);
			}
			else
			{
				// 메모리를 긁는지 시험한다.
				CNode* node = m_pHead;
				while(node!=NULL)
				{
					node = node->m_pNext;
				}
				// non-empty list
				// 		assert(AtlIsValidAddress(m_pHead, sizeof(CNode)));
				// 		assert(AtlIsValidAddress(m_pTail, sizeof(CNode)));
			}
		}

#endif  // _DEBUG

		// Implementation
	private:
		CNode* m_pHead;
		CNode* m_pTail;
		intptr_t m_nElements;

	private:
		inline CNode* NewNode( CNode* pPrev, CNode* pNext )
		{
			CNode* ret = (CNode*)CProcHeap::Alloc(sizeof(CNode));
			if(ret == NULL)
				ThrowBadAllocException();

			CallConstructor<CNode>(ret);

			ret->m_pPrev = pPrev;
			ret->m_pNext = pNext;

			m_nElements++;

			return ret;
		}
		inline CNode* NewNode( INARGTYPE element, CNode* pPrev, CNode* pNext )
		{
			CNode* ret = (CNode*)CProcHeap::Alloc(sizeof(CNode));
			if(ret == NULL)
				ThrowBadAllocException();

			CallConstructor<CNode>(ret, element);

			ret->m_pPrev = pPrev;
			ret->m_pNext = pNext;

			m_nElements++;

			return ret;
		}

		inline void FreeNode( CNode* pNode ) throw()
		{
			CallDestructor<CNode>(pNode);

				CProcHeap::Free(pNode);
			m_nElements--;
		}

	public:
		inline ~CFastList() throw()
		{
			RemoveAll();
			assert( m_nElements == 0 );
		}

	private:
		// Private to prevent use
		CFastList( const CFastList& ) throw();
		CFastList& operator=( const CFastList& ) throw();

	public:

		class value_type
		{
		public:
			Proud::Position m_pos;
			CFastList* m_owner;
		};

		class iterator;
		friend class iterator;

#ifndef PROUDNET_NO_CONST_ITERATOR
		class const_iterator;
		friend class const_iterator;

		/**
		\~korean
		STL의 const_iterator와 같은 역할을 한다.

		\~english
		Performs the same role as const_iterator of STL

		\~chinese
		起着与STL的const_iterator一样的作用。

		\~japanese
		STLのconst_iteratorのような役割をします。
		\~
		*/
		class const_iterator:public value_type
		{
		public:
			inline const_iterator() {}
			inline const_iterator(const iterator& src):value_type(src) {}

			inline const_iterator& operator++()
			{
				// preincrement
                value_type::m_owner->GetNext(value_type::m_pos);
				return (*this);
			}

			inline const_iterator operator++(int)
			{
				// postincrement
				const_iterator _Tmp = *this;
				++*this;
				return (_Tmp);
			}

			inline bool operator==(const const_iterator& a) const
			{
				return value_type::m_pos==a.m_pos && value_type::m_owner==a.m_owner;
			}

			inline bool operator!=(const const_iterator& a) const
			{
				return !(value_type::m_pos==a.m_pos && value_type::m_owner==a.m_owner);
			}

			inline const E& operator*() const
			{
				return value_type::m_owner->GetAt(value_type::m_pos);
			}
		};
#endif
		/**
		\~korean
		STL의 iterator와 같은 역할을 한다.

		\~english
		Performs the same role as iterator of STL

		\~chinese
		起着与STL的iterator一样的作用。

		\~japanese
		STLのiteratorのような役割をします。
		\~
		*/
		class iterator:public value_type
		{
		public:
			inline iterator() {}
			inline iterator(const const_iterator& src):value_type(src) {}

			inline bool operator==(const iterator& a) const
			{
				return value_type::m_pos==a.m_pos && value_type::m_owner==a.m_owner;
			}

			inline bool operator!=(const iterator& a) const
			{
				return !(value_type::m_pos==a.m_pos && value_type::m_owner==a.m_owner);
			}

			inline iterator& operator++()
			{
				// preincrement
				value_type::m_owner->GetNext(value_type::m_pos);
				return (*this);
			}

			inline iterator operator++(int)
			{
				// postincrement
				iterator _Tmp = *this;
				++*this;
				return (_Tmp);
			}

			inline E& operator*() const
			{
				return value_type::m_owner->GetAt(value_type::m_pos);
			}
		};

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs the same role as the same named method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline iterator begin()
		{
			iterator ret;
			ret.m_pos = GetHeadPosition();
			ret.m_owner=this;

			return ret;
		}

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs the same role as the same named method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline iterator end()
		{
			iterator ret;
			ret.m_pos=NULL;
			ret.m_owner=this;

			return ret;
		}

#ifndef PROUDNET_NO_CONST_ITERATOR
		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs the same role as the same named method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline const_iterator begin() const
		{
			const_iterator ret;
			ret.m_pos = GetHeadPosition();
			ret.m_owner = (CFastList*)this;

			return ret;
		}

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs the same role as the same named method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline const_iterator end() const
		{
			const_iterator ret;
			ret.m_pos=NULL;
			ret.m_owner = (CFastList*)this;

			return ret;
		}
#endif

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Performs the same role as the same named method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		*/
		inline iterator erase(iterator iter)
		{
			if(iter.m_owner!=this)
			{
				ThrowInvalidArgumentException();
			}
			iterator ret = iter;
			ret++;
			RemoveAt(iter.m_pos);

			return ret;
		}

		/**
		\~korean
		현재 CFastList 와 다른 CFastList 의 내용이 같은지 비교한다. Data를 직접 비교하여 확인한다..
		\param rhs 비교할 CFastList
		\return 같으면 true 다른면 false 리턴

		\~english 
		Compare whether the content of the current CFastList and other CFastList are the same. Directly compare the data to confirm. 
		\param rhs CFastList to be compared 
		\return True if the same, otherwise return false. 

		\~chinese
		比较现在的 CLinkedList%和其他 CLinkedList%的内容是否相同。直接比较data并确认。
		\param rhs 要比较的 CLinkedList%。
		\return 一样的话true，不同的话返回false。

		\~japanese
		現在CFastListと他のCFastListの内容が同じであるかどうかを比較します。データを直接比較して確認します。
		\param rhs 比較する CFastList
		\return 同じであるとtrue、違うとfalseをリターン
		\~
		*/
		inline bool Equals(const CFastList& rhs) const
		{
			if(rhs.GetCount()!=GetCount())
				return false;

			Proud::Position i1=GetHeadPosition();
			Proud::Position i2=rhs.GetHeadPosition();
			while( i1!=NULL && i2!=NULL )
			{
				if(GetAt(i1)!=rhs.GetAt(i2))
					return false;
				GetNext(i1);
				rhs.GetNext(i2);
			}

			return true;
		}
	};

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
