#pragma once

#include "ProcHeap.h"

namespace Proud
{
	/* Type이 가리키는 객체 타입 자체가 다음 특징을 가지게 한다.
	- 객체 자체가 스마트 포인터 타입이 되게 한다. 즉 복사 비용을 가볍게 한다.
	- 객체가 가지는 본질 객체(substance)가 class level의 전역 object pool에서 재사용되게 한다. 즉 객체 생성/할당 비용을 절약할 수 있게 해준다.

	주의사항: 변수 초기화시 이전 본질객체가 재사용된다는 전제 때문에, 멤버 변수들이 남아있을 수 있다. 즉 필요시 멤버 변수들을 수동 청소해 주어야 한다.
	Type은 Clear(), SuspendShrink()를 가져야 한다. 아무것도 안하더라도.
	프넷 사용자는 직접 사용 불가능한 클래스. src/ 안에서만 쓴다. */
	template<typename Type, bool ClearOnDrop>
	class BiasManagedPointer
	{
	public:
		// 참조 카운트와 객체 본체를 한 메모리 블럭에 모두 가진다.
		class Tombstone
		{
		public:
			// 실제 데이터 본체
			Type m_substance;

			// 참조 카운트
			volatile intptr_t m_refCount;

			inline Tombstone()
			{
				m_refCount = 0;
			}

			// 배열 등이나 갖고 있는 버퍼가 남아돌면 줄이는 기능을 해제한다.
			void SuspendShrink()
			{
				m_substance.SuspendShrink();
			}
			void OnRecycle() {}
			void OnDrop()
			{
				m_substance.OnDrop();
			}


			DECLARE_NEW_AND_DELETE
		};

	private:
		// refcount와 Type 객체를 가지는 본체.
		Tombstone* m_tombstone;

		//////////////////////////////////////////////////////////////////////////
		// src/ 안에서 구현됨. 헤더 파일에 남지 못함. 

		static Tombstone* AllocTombstone();
		static void FreeTombstone(Tombstone* tombstone);

		//////////////////////////////////////////////////////////////////////////

	protected:
		// 파생 클래스 안에서 본체를 액세스할 때 쓰인다.
		inline Tombstone* GetTombstone() const
		{
			return m_tombstone;
		}

	public:
		inline BiasManagedPointer()
			: m_tombstone(NULL)  // 처음에는 Tombstone 자체도 없다. 만드려면 사용자가 InitTombstoneIfEmpty()을 콜 해야 한다.
		{
		}

		// 복사 생성자
		inline BiasManagedPointer(const BiasManagedPointer& src)
		{
			// src의 tombstone을 공유한다.
			if (src.m_tombstone)
			{
				AtomicIncrementPtr(&src.m_tombstone->m_refCount);
				m_tombstone = src.m_tombstone;
			}
			else
				m_tombstone = NULL;
		}

#ifdef SUPPORTS_CPP11
		// 이동 생성자
		inline BiasManagedPointer(BiasManagedPointer&& src)
		{
			if (src.m_tombstone)
			{
				// smart ptr이므로 이동 생성자가 더 싸다.
				m_tombstone = src.m_tombstone;
				// 이동 생성자이므로, src를 비워 버린다.
				src.m_tombstone = NULL;
			}
			else
				m_tombstone = NULL;
		}
#endif

		// 복사 연산자
		inline BiasManagedPointer& operator=(const BiasManagedPointer& src)
		{
			if (src.m_tombstone != m_tombstone)
			{
				UninitTombstone(); // 기존 것 삭제

				if (src.m_tombstone != NULL)
				{
					AtomicIncrementPtr(&src.m_tombstone->m_refCount);
					m_tombstone = src.m_tombstone;
				}
			}

			return *this;
		}

#ifdef SUPPORTS_CPP11
		// 이동 연산자
		inline BiasManagedPointer& operator=(BiasManagedPointer&& src)
		{
			if (&src != this)
			{
				UninitTombstone(); // 기존 것 삭제

				// 소유권 이양
				m_tombstone = src.m_tombstone;
				// src를 비워 버린다.
				src.m_tombstone = NULL;
			}

			return *this;
		}
#endif

		inline ~BiasManagedPointer()
		{
			UninitTombstone(); 
		}

	protected:
		// Tombstone을 버리고 초기 상태로 간다.
		inline void UninitTombstone()
		{
			if (m_tombstone)
			{
				// 리퍼런스 카운트 줄이고 0이 되면
				if (AtomicDecrementPtr(&m_tombstone->m_refCount) == 0)
				{
					// 요청하에 내용물 청소
					if(ClearOnDrop)
						m_tombstone->m_substance.Clear();

					// obj-pool에 반환
					FreeTombstone(m_tombstone);
				}
				m_tombstone = NULL;
			}
		}

		// Tombstone을 할당한다. 이미 갖고 있으면 skip.
		// Tombstone 할당이라 함은, Type subtance 객체를 실 생성함을 의미한다.
		// 물론, object-pool에서 가져온다.
		inline void InitTombstoneIfEmpty()
		{
			if(!m_tombstone)
			{
				m_tombstone = AllocTombstone();
				m_tombstone->SuspendShrink();
				m_tombstone->m_refCount = 1;
			}
		}
	};
}

#include "BiasManagedPointer.inl"
