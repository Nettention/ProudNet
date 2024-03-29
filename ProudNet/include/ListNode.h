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

#include "./Exception.h"

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
	
	PROUD_API extern const char* ConsistencyProblemText;
	
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	double linked list 를 구성하는 node이다.
	- CListOwner 에 붙는 경우에만 연결 관계를 구성할 수 있다. 

	\~english
	node that consist double linked list
	- A linkage can be set only if attached to CListOwner.

	\~chinese
	构成double linked list的node。
	- 只有在贴于 CListOwner%的情况下才能构成连接关系。

	\~japanese
	double linked listを構成するnodeです。
	- ClistOwnerに付く場合のみ連結関係を構成することができます。 
	\~
	*/
	template<typename T>
	class CListNode
	{
	public:	
		class CListOwner
		{
			T* m_first;
			T* m_last;
			int m_count;

			void AssertInThis(bool expResult)
			{
				if (m_enableAssertInThis && !expResult)
					ThrowException(ConsistencyProblemText);
			}

			// 내부 데이터 일관성 검사
			// 이 함수는 구동이 느리다. 따라서 꼭 필요한 경우에만 기능을 켜자.
			void AssertConsist()
			{
				if (m_enableAssertInThis)
				{
					T* node = m_first;

					if (m_first == NULL)
					{
						// 텅빈 경우의 검사
						AssertInThis(m_count == 0);
						AssertInThis(m_last == NULL);
					}

					while (node != NULL)
					{
						// 끝까지 돌면서 prev-node-next 관계 일관성 검사
						AssertInThis(node->m_listOwner == this);

						if (node->m_prev == NULL)
						{
							AssertInThis(m_first == node);
						}
						else
						{
							AssertInThis(node->m_prev->m_next == node);
						}
						if (node->m_next == NULL)
						{
							AssertInThis(m_last == node);
						}
						else
						{
							AssertInThis(node->m_next->m_prev == node);
						}
						node = node->m_next;
					}
				}
			}

		public:
			// 이 값을 true로 세팅하면 일관성 검사를 하는 루틴이 작동한다.
			// 성능이 크게 떨어지므로 주의 요망.
			// debug or release 상관없이 켜거나 끌 수 있어야 해서 추가됨.
			bool m_enableAssertInThis;

			inline CListOwner()
			{
				m_first = NULL;
				m_last = NULL;
				m_count = 0;
				m_enableAssertInThis = false;
			}

			inline ~CListOwner()
			{
				UnlinkAll();
			}

			/**
			\~korean
			연결된 링크를 제거합니다.

			\~english 
			Removed the connected link 

			\~chinese
			删除连接的链接。

			\~japanese
			連結されたリンクを除去します。
			\~
			*/
			inline void UnlinkAll()
			{
				while(!IsEmpty())
				{
					Erase(m_first);
				}
			}

			/**
			\~korean
			노드가 비었는지 확인한다. \return 비었으면 true, 비어있지 않으면 false 리턴

			\~english 
			Check to see if the node is empty. \return Return true if empty, false if not. 
			
			\~chinese
			确认节点是否空。
			\return 空的话true，不是空的话返回false。

			\~japanese
			ノードが空いているか確認します。\returnが空いていればtrue、空いていなければfalseをリターン
			\~
			*/
			inline bool IsEmpty() const
			{
				return m_first == NULL;
			}

			// node를 this container에서 제거한다.
			void Erase(T* node)
			{
				// 각종 검사
				AssertInThis(node->m_listOwner == this);

				T* nextNode = node->m_next;
				T* prevNode = node->m_prev;

				if (nextNode == NULL)
				{
					AssertInThis(m_last == node);
				}

				if (prevNode == NULL)
				{
					AssertInThis(m_first == node);
				}

				// 데이터 업데이트.
				// 이렇게 검사 구간과 업데이트 구간을 나누어야 부분만 변경하다 마는 사태 예방.
				if (nextNode != NULL)
					nextNode->m_prev = prevNode;

				if (prevNode != NULL)
					prevNode->m_next = nextNode;

				if (nextNode == NULL)
				{
					m_last = prevNode;
				}

				if (prevNode == NULL)
				{
					m_first = nextNode;
				}

				node->m_prev = NULL;
				node->m_next = NULL;
				node->m_listOwner = NULL;

				m_count--;

				AssertConsist();
			}

			// prevNode의 뒤에 node를 추가한다.
			// prevNode=null이면 맨 앞에 추가한다.
			void Insert(T* prevNode, T* node)
			{
				// 주의: 검사를 모두 마치기 전에는 데이터 변형을 가하지 말 것
				AssertInThis(node->m_listOwner == NULL);

				if (prevNode == NULL)
				{
					// 맨 앞에 추가
					if (m_first != NULL)
					{
						m_first->m_prev = node;
						node->m_next = m_first;
						m_first = node;
					}
					else
					{
						m_first = m_last = node;
					}
				}
				else
				{
					// prevNode 뒤에 추가
					AssertInThis(prevNode->m_listOwner == this);

					T* node_next = prevNode->m_next;
					prevNode->m_next = node;
					node->m_prev = prevNode;
					node->m_next = node_next;
					if (node_next == NULL)
					{
						// prevNode는 last였다. 이제 node가 last가 되어야 한다.
						m_last = node;
					}
					else
					{
						// next의 상태도 갱신해야
						node_next->m_prev = node;
					}
				}
				node->m_listOwner = this;
				m_count++;
				AssertConsist();
			}

			// 맨 뒤에 추가한다.
			inline void PushBack(T* node)
			{
				Insert(m_last, node);
			}

			/* 기존 노드의 위치에 새로운 노드로 교체한다.
			node: 기존 노드 newNode 변경할 노드 */
			inline void Replace(T* node, T* newNode)
			{
				T* prev = node->m_prev;

				// 아래 erase-insert가 partially done이 되면 안되므로 여기서 사전 검사를 다 해버려야 한다.
				AssertInThis(node->m_listOwner == this);
				AssertInThis(newNode->m_listOwner == NULL);

				Erase(node);
				Insert(prev, newNode);
			}

			/**
			\~korean
			first 노드를 리턴받습니다.

			\~english 
			Get the first node returned

			\~chinese
			返回接收first节点。

			\~japanese
			first ノードをリターンしてもらいます。
			\~
			*/
			inline T* GetFirst() const
			{
				return (T*)m_first;
			}

			/**
			\~korean
			last노드를 리턴받습니다.

			\~english 
			Get the last node returned

			\~chinese
			返回接收last节点。

			\~japanese
			lastノードをリターンしてもらいます。
			\~
			*/
			inline T* GetLast() const
			{
				return (T*)m_last;
			}

			/**
			\~korean
			연결되어 있는 노드의 카운트를 리턴받습니다.

			\~english 
			Get the count of the connected node

			\~chinese
			返回接收已连接的节点的数。

			\~japanese
			連結されているノードのカウントをリターンしてもらいます。
			\~
			*/
			inline int GetCount() const
			{
				return m_count;
			}

#if defined(_MSC_VER)
			__declspec(property(get=GetCount)) int Count;
#endif
		};

	private:
		friend class CListOwner;
		T* m_prev, *m_next;
		CListOwner* m_listOwner;

	public:
		inline CListNode(void)
		{
			m_prev = NULL;
			m_next = NULL;
			m_listOwner = NULL;
		}

		inline ~CListNode(void)
		{
			if (m_listOwner != NULL)
				m_listOwner->Erase((T*)this);
		}

		/**
		\~korean
		이 node를 가지고 있는 owner를 리턴합니다.

		\~english 
		Return the owner who has this node

		\~chinese
		返回拥有此node的woner。

		\~japanese
		このnodeを持っているownerをリターンします。
		\~
		*/
		inline CListOwner* GetListOwner() const 
		{
			return m_listOwner;
		}

		/**
		\~korean
		다음노드를 리턴받습니다.

		\~english 
		Get the next node returned

		\~chinese
		返回接收之后的节点。

		\~japanese
		次のノードをリターンしてもらいます。
		\~
		*/
		inline T* GetNext() const
		{
			return (T*)m_next;
		}

		/**
		\~korean
		이전 노드를 리턴받습니다.

		\~english 
		Get the previous node returned

		\~chinese
		返回接收之前的节点。

		\~japanese
		以前のノードをリターンしてもらいます。
		\~
		*/
		inline T* GetPrev() const
		{
			return (T*)m_prev;
		}

		/* \~korean owner 와의 링크를 제거합니다. \~english Remove the link with the owner \~chinese 删除与owner的链接。
		\~japanese ownerとのリンクを除去します。
		이거 퇴역 这个退役
		inline void UnlinkSelf()
		{
			if(m_pListOwner!=NULL)
				m_pListOwner->Erase((T*)this);
		}
\~
*/
	};

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
