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

#if defined(_WIN32)

#include <map>
#include <vector>
#include "FakeClr.h"
#include "Singleton.h"
#include "Marshaler.h"
#include "Message.h"
#include "PNString.h"
#include "Variant.h"

#include "strpool.h"
#include "ReaderWriterMonitor.h"

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
	/** \addtogroup db_group
	*  @{
	 */

	class CProperty;

	/**
	\~korean 
	PropCollection의 Message 직렬화 헬퍼 기능 
	
	\~english 
	PropCollection's Message serialization helper functions

	\~chinese
	PropCollection 的Message串联helper功能。

	\~japanese
	PropCollectionのMessage直列化ヘルパー機能
	\~
	*/
	PROUDSRV_API CMessage& operator<<(CMessage& packet, const CProperty &rhs);

	/**
	\~korean
	PropCollection의 Message 역직렬화 헬퍼 기능

	\~english
	PropCollection's Message deserialization helper functions

	\~chinese
	PropCollection 的Message并联helper功能。

	\~japanese
	PropCollectionのMessage並列化ヘルパー機能
	\~
	*/
	PROUDSRV_API CMessage& operator>>(CMessage& packet, CProperty &rhs);

	/**
	\~korean
	\brief 0개 이상의 Named Property를 가지는 Collection 객체이다.

	정의
	- Unique key container associated with one value. 즉,
	Set of (key as case insensitive string, value as variant).
	즉, key값으로 대소문자를 안가리는 string이 들어간다는 점을 제외하면 Visual Basic의 
	Collection과 같은 역할을 한다. 예를 들어, 
	\code
		{ ('name','baehj'), ('id',123), ('height',176) }
	\endcode

	기능
	- value type이 _variant_t type이므로, 이 변수를 다룰 때의 주의점
	이 그대로 적용된다.(예를 들어, long은 쓰일 수 있으나 int는 호환이 안됨)
	- PropCollection 객체 자체는 _variant_t 객체로 캐스팅될 수 있다. 또한 반대도
	가능하다. 이때, PropCollection의 내용은 _variant_t로 serialize된다.
	이는 OLE DB에서 PropCollection의 내용을 binary type field에 저장할 수 있다.

	사용 예
	\code
		PropCollection a;
		a.Fields[L"name"]=L"baehj";
		a.Fields[L"id"]=123L;
		a.Fields[L"height"]=176L;
	\endcode
	이제 a에는 { ('name','baehj'), ('id',123), ('height',176) }가 들어간다.
	\code
		_bstr_t name=a.GetField(L"name");  -- name 변수에는 "baehj"가 들어간다.
		_variant_t serialized=a;  //-- serialized에는 a가 serialize된 형태의 binary type data가 들어간다.
		PropCollection b;
		b=serialized;	 //-- b는 a와 같게 된다. 
	\endcode 

	\~english
	\brief A collection object that has more than 0 unit of Named Property 
	
	Definition
	- Unique key container associated with one value. 즉, Set of (key as case insensitive string, value as variant).
	  In other words, except the fact that a sting that does not distinguish capital/small letters as key value enters, this acts as Collection of Visual Basic.
	For an example, 
	\code
		{ ('name','baehj'), ('id',123), ('height',176) }
	\endcode

	Function
	- Since its value type is _variant_t type, the considerations must be taken when using this variable apply directly. (For an example, long can be used but int cannot be compatible.)
	- PropCollection object itself can be casted to _variant_t object. And the opposite is also possible. At that time, the contents of PropCollection will serialize to _variant_t.
	  This allows the contents of PropCollection can be stored at binary type field in OLE DB.

	Usage example
	\code
		PropCollection a;
		a.Fields[L"name"]=L"baehj";
		a.Fields[L"id"]=123L;
		a.Fields[L"height"]=176L;
	\endcode
	Now a has { ('name','baehj'), ('id',123), ('height',176) }.
	\code
		_bstr_t name=a.GetField(L"name");  -- "baehj" enters to name variable.
		_variant_t serialized=a;  //-- binary type data that a is serialized will enter serialized.
		PropCollection b;
		b=serialized;	 //-- b becomes equal to a. 
	\endcode 

	\~chinese
	\brief 拥有0个以上Named Property的Collection对象。

	定义
	- Unique key container associated with one value. 即,
	Set of (key as case insensitive string, value as variant).
	即，除了用key值不划分大小文字的string被输入的话，起着与Visual Basic的Collection一样的作用。例如，
	\code
		{ ('name','baehj'), ('id',123), ('height',176) }
	\endcode

	功能
	- 因为value type是_variant_t type，使用此变数的时候的注意事项会原封不动的被适用。（例如long可以被使用，但是int不能被兼容）
	- PropCollection对象本身可以被casting为_variant_t对象。而且相反也有可能。这时PropCollection的内容以_variant_t被serialize。这可以在OLE DB把PropCollection的内容储存在binary type field中。

	使用例
	\code
		PropCollection a;
		a.Fields[L"name"]=L"baehj";
		a.Fields[L"id"]=123L;
		a.Fields[L"height"]=176L;
	\endcode
	接下来a里进入{ ('name','baehj'), ('id',123), ('height',176) }。
	\code
		_bstr_t name=a.GetField(L"name");  -- name变数里进入"baehj"。
		_variant_t serialized=a;  //-- serialized里进入a被serialize形式的binary type data。
		PropCollection b;
		b=serialized;	 //-- b与a会相同。 
	\endcode

	\~japanese
	\brief 0個以上のNamed Propertyを持つCollectionオブジェクトです。 

	定義
	- Unique key container associated with one value. 即ち、
	Set of (key as case insensitive string, value as variant).
	
	即ち、キー値で大小文字に拘らずにstringが入るという点を除外したらVisual BasicのCollectionのような役割をします。例えば、 
	\code
	{ ('name','baehj'), ('id',123), ('height',176) }
	\endcode

	機能
	- value typeが_variant_t typeなので、この変数を扱う時の注意点がそのまま適用されます。(例えば、longは使われますが、intは互換されます)
	- PropCollection オブジェクト自体は、_variant_t オブジェクトでキャスティングできます。また、反対も可能です。この時、PropCollectionの内容は、_variant_tにserializeされまうｓ。
	これはOLE DBでPropCollectionの内容をbinary type fieldに保存することができます。

	使用例
	\code
	PropCollection a;
	a.Fields[L"name"]=L"baehj";
	a.Fields[L"id"]=123L;
	a.Fields[L"height"]=176L;
	\endcode

	もう、aには{ ('name','baehj'), ('id',123), ('height',176) }が入ります。
	\code
	_bstr_t name=a.GetField(L"name");  -- name 変数には"baehj"が入ります。
	_variant_t serialized=a;  //-- serializedにはaがserializeされた形態のbinary type dataが入ります。
	PropCollection b;
	b=serialized;	 //-- bはaのようになります。 
	\endcode 



	\~
	*/
	class  CProperty
	{
		friend PROUDSRV_API CMessage& operator<<(CMessage& packet, const CProperty &rhs);
		friend PROUDSRV_API CMessage& operator>>(CMessage& packet,CProperty &rhs);
		CStringPool::Holder m_strpool;

	public:
		/** 
		\~korean
		field 이름과 값에 대응하는 map 객체이다.
		- 주의!! iteration을 위해 이 데이터 멤버를 직접 접근하는 것은 허락하나, 읽기 전용으로만 쓰는 것을 허락한다.  

		\~english
		Map object that corresponds to field name and value
		- Caution!! It is allowed to directly access to this data member for iteration, but only allowed to write as read only.

		\~chinese
		对应field名和值的map对象。
		-注意！！虽允许因iteration接近此数据，但只允许为只读文件。

		\~japanese
		field名と値に対応するマップオブジェクトです。
		- ご注意!! Iterationのためにこのデータメンバーを直接アクセスすることは許可しますが、読み取り専用でのみ使うことを許可します。  
		\~
		*/
		typedef CFastMap<String, CVariant, StringTraits> MapType;
	protected:
		// 주의: 여기에 들어가는 value는 반드시 CStringPool::GetUnsafeRef().GetString(String(key).MakeUpper()) 으로 얻은 것만 들어가야!
		MapType m_map;
	private:
#ifdef _DEBUG
		RefCount<CReaderWriterAccessChecker> m_RWAccessChecker;
#endif // _DEBUG
	public:
		class const_iterator 
		{
			friend CProperty;

//			CProperty* m_owner;
			MapType::const_iterator m_intl;
		public:
			const_iterator() {}
			inline const_iterator(const const_iterator &src) 
			{
//				m_owner = src.m_owner;
				m_intl = src.m_intl;
			}

			inline const_iterator& operator=(const const_iterator &src) 
			{
//				m_owner = src.m_owner;
				m_intl = src.m_intl;

				return *this;
			}

			const_iterator& operator++()
			{
				// preincrement
				m_intl++;
				return (*this);
			}

			const_iterator operator++(int)
			{
				// postincrement
				const_iterator _Tmp = *this;
				++(*this);
				return (_Tmp);
			}

			inline bool operator==(const const_iterator& a) const 
			{
				return m_intl==a.m_intl /*&& m_owner==a.m_owner */;
			}

			inline bool operator!=(const const_iterator& a) const 
			{
				return !(m_intl==a.m_intl /*&& m_owner==a.m_owner */);
			}

			const String GetKey()
			{
				return m_intl->GetFirst();
			}
			CVariant GetValue()
			{
				return m_intl->GetSecond();
			}
			void SetValue(const CVariant &val)
			{
				m_intl->SetSecond(val);
			}
		
#if defined (WIN32)
			__declspec(property(get=GetKey)) const String Key;
			__declspec(property(get=GetValue,put=SetValue)) CVariant Value;
#endif

		};

		inline const_iterator begin() const
		{
			AssertThreadID(eAccessMode_Read);
			const_iterator ret;
			ret.m_intl = m_map.begin();
			
			ClearThreadID();
			return ret;
		}

		inline const_iterator end() const
		{
			AssertThreadID(eAccessMode_Read);
			const_iterator ret;
			ret.m_intl = m_map.end();

			ClearThreadID();
			return ret;
		}

		inline size_t GetCount() const
		{
			AssertThreadID(eAccessMode_Read);
			size_t count = m_map.GetCount();
			ClearThreadID();
			return count;
		}
#if defined (WIN32)
		__declspec(property(get = GetCount)) size_t Count;
#endif

		inline void Remove(const String& key)
		{
			AssertThreadID(eAccessMode_Write);
			m_map.Remove(String(key).MakeUpper());
			ClearThreadID();
		}
		inline void Clear()
		{
			AssertThreadID(eAccessMode_Write);
			m_map.Clear();
			ClearThreadID();
		}
		inline bool TryGetValue(const String& key, CVariant& ret) const
		{
			AssertThreadID(eAccessMode_Read);
			bool bRet = m_map.TryGetValue(String(key).MakeUpper(), ret);
			ClearThreadID();

			return bRet;
		}
		inline bool ContainsKey(const String& key) const
		{
			AssertThreadID(eAccessMode_Read);
			bool bRet = m_map.ContainsKey(String(key).MakeUpper());
			ClearThreadID();
			return bRet;
		}
		inline bool Add(const String& key, const CVariant& val)
		{
			AssertThreadID(eAccessMode_Write);
			bool bRet =m_map.Add(CStringPool::GetUnsafeRef().GetString(String(key).MakeUpper()),val);
			ClearThreadID();
			return bRet;
		}
		inline void CopyKeysTo(CFastArray<String> &dest)
		{
			AssertThreadID(eAccessMode_Read);
			dest.Clear();
			for(const_iterator i = begin(); i!=end();i++)
			{
				dest.Add(i.Key);
			}

			ClearThreadID();
		}

	protected:
		virtual void	AssertThreadID(eAccessMode eMode) const;
		virtual void	ClearThreadID() const;
	public:
		/** 
		\~korean
		이 collection 객체에 있는 특정 (key, value) pair를 구한다.
		- 이 메서드 대신 Fields[]를 직접 다루는 것이 더 편하므로 권장된다.
		\return pair의 value. 만약 key에 해당하는 pair가 없으면 empty type variant를 리턴한다.
		\param name key of pair whose value should be retrieved  

		\~english
		Gets a specific (key, value) pair in this collection object
		- Direct handling of Fields[] is recommended since it is easier than using this method.
		\return value of the pair. Returns empty type variant when there is no pair corresponds to key.
		\param name key of pair whose value should be retrieved
		\~chinese
		求在此collection对象的特定(key, value) pair。
		- 比起这个方法，直接使用Fields[]更方便，因此建议使用它。
		\return pair的value。如果没有有关key的pair的话返回empty type variant。
		\param name key of pair whose value should be retrieved

		\~japanese
		このcollectionオブジェクトにある特定(key, value)ペアを求めます。
		- このメソッドの代わりにFields[]を直接扱うのがもっと楽ですのでお勧めします。
		\return pairのvalue。もし、keyに該当するpairがなければempty type variantをリターンします。
		\param name key of pair whose value should be retrieved  
		\~
		*/
		 PROUDSRV_API CVariant GetField(const String &name);

		/** 
		\~korean
		이 collection 객체에 특정 (key, value) pair를 넣는다. 이미 같은 key를 가지는 pair가 있으면 그것을 새걸로 교체한다.
		- 이 메서드 대신 Fields[]를 직접 다루는 것이 더 편하므로 권장된다.
		\param name key of pair
		\param value value of pair  

		\~english
		Specific (key, value) pair to be entered to this collection object. If there is the pair that has same key than new comer replaces itself with old one.
		- Direct handling of Fields[] is recommended since it is easier than using this method.
		\param name key of pair
		\param value value of pair

		\~chinese
		往这个collection对象输入特定(key, value) pair。如果存在拥有相同key的pair的话，用新的代替。
		- 比起这个方法，直接使用Fields[]更方便，因此建议使用它。
		\param name key of pair
		\param value value of pair

		\~japanese
		このcollectionオブジェクトに特定(key, value)pairを入れます。既に同じkeyを持つpairがあれば、それを新しいものに入れ替えます。
		- このメソッドの代わりにFields[]を直接扱うことがもっと便利ですのでお勧めします。
		\param name key of pair
		\param value value of pair  

		\~
		*/
		 PROUDSRV_API 	 virtual void SetField(const String &name, const CVariant &value);

		/** 
		\~korean
		이 collection 객체에서 특정 (key, value) pair를 제거한다.
		\param name 제거할 pair의 key 값  

		\~english
		Removes specific (key, value) pair from this collection object
		\param name key value of pair to be removed

		\~chinese
		从collection对象删除特定(key, value) pair。
		\param name 要删除的pair的key值。

		\~japanese
		このcollectionオブジェクトで特定(key, value) pairを除去します。
		\param name 除去するpairのkey値  
		\~
		*/
		 PROUDSRV_API  virtual void RemoveField(const String &name);

		/** 
		\~korean
		serialize가 되어 있는 bytearray type의 variant의 내용을 deserialize해서 이 객체에 채운다.
		\param from.. 복사 원본  

		\~english
		The contents of variant of serialized bytearray type are to be deserialized and to be filled to this object.
		\param from.. oriinal to copy

		\~chinese
		被serialize的 bytearray type的variant内容进行deserialize后填充到此对象。
		\param from.. 复制原本

		\~japanese
		Serializeされているbytearray typeのvariantの内容をdeserializeしてこのオブジェクトに満たします。
		\param from.. コピー原本 
		\~
		*/
		 PROUDSRV_API  virtual void FromVariant(_variant_t from);

		 PROUDSRV_API virtual void FromByteArray(const ByteArray &from);

		 PROUDSRV_API  virtual void FromByteArray(const uint8_t* data,int length);

		 PROUDSRV_API  virtual void ToByteArray(ByteArray &output);


		/** 
		\~korean
		기본 생성자  

		\~english
		Base constructor

		\~chinese
		基本生成者。

		\~japanese
		基本生成子  
		\~
		*/
		 PROUDSRV_API  CProperty();
		 PROUDSRV_API  CProperty(const CProperty& rhs);

		 PROUDSRV_API virtual ~CProperty();

		/** 
		\~korean 
		CVariant type과 호환되게 하는 helper function 
		
		\~english 
		Helper function that make compatibility with CVariant type

		\~chinese
		与 CVariant type兼容的helper function

		\~japanese
		CVariant typeと互換されるようにするhelper function 
		\~
		*/
		CProperty(_variant_t from);

		/** 
		\~korean 
		_variant_t type과 호환되게 하는 helper function 
		
		\~english 
		Helper function that make compatibility with _variant_t type

		\~chinese
		与_variant_t type 兼容的helper function。

		\~japanese
		_variant_t typeと互換されるようにするhelper function 
		\~
		*/
		PROUDSRV_API 	operator _variant_t() const;
		PROUDSRV_API 	 operator ByteArrayPtr() const;

		/** 
		\~korean 
		복사 생성자 
		
		\~english 
		Copy creator

		\~chinese
		复制生成者。

		\~japanese
		コピー生成子
		\~
		*/
		inline CProperty & operator=( const _variant_t &from)
		{
			FromVariant(from);
			return *this;
		}
		inline CProperty & operator=( ByteArray &from)
		{
			FromByteArray(from);
			return *this;
		}

		inline CProperty & operator=( const CProperty &rhs)
		{
			rhs.AssertThreadID(eAccessMode_Read);
			AssertThreadID(eAccessMode_Write);
			m_map = rhs.m_map;
			ClearThreadID();
			rhs.ClearThreadID();

			return *this;
		}

		/** 
		\~korean
		필드의 값을 읽거나 기록할 때 사용한다. 자세한 것은 \ref dbc1_adding_fields  참조.  

		\~english
		Used to read or write field value. Please refer \ref db_adding_fields

		\~chinese
		读或者记录领域值的时候使用。详细请参考\ref dbc1_adding_fields%。

		\~japanese
		フィールドの値を読んだり記録する時に使います。詳しくは、\ref dbc1_adding_fieldsをご参照ください。
		\~
		*/
#if defined (WIN32)
		__declspec(property(get=GetField,put=SetField)) CVariant Fields[];
#endif

		/** 
		\~korean
		이 콜렉션 객체의 내용을 문자열로 만들어서 출력한다. 
		- 디버깅을 할 때 용이하다.  

		\~english
		Converts the contents of this collection object into text string then outputs 
		- Useful when debugging

		\~chinese
		把此collection对象的内容转换成字符串后输出。
		- 进行调试的时候会较为轻松。

		\~japanese
		このコレクションオブジェクトの内容を文字列で作って出力します。
		- デバッギングをする時に容易です。
		\~
		*/
		PROUDSRV_API String GetDumpedText();
	};

	class CDbCacheServerImpl;
	class CDbCacheClientImpl;
    class CDbCacheServer2Impl;
    class CDbCacheClient2Impl;
	class CLoadedData;
	class CLoadedData_S;
	class CLoadedData2;
	class CLoadedData2_S;
	class CPropForest;
	class CPropNode;
	typedef RefCount<CPropNode> CPropNodePtr;

	/** 
	\~korean 
	PropCollection의 Message 직렬화 헬퍼 기능 
	
	\~english 
	PropCollection's Message serialization helper functions 

	\~chinese
	PropCollection 的Message串联helper功能。

	\~japanese
	PropCollectionのMessage直列化ヘルパー機能 
	\~
	*/
	PROUDSRV_API CMessage& operator<<(CMessage& packet, CPropNode &rhs);

	/** 
	\~korean 
	PropCollection의 Message 역직렬화 헬퍼 기능 
	
	\~english 
	PropCollection's Message deserialization helper functions 

	\~chinese
	PropCollection 的Message并联helper功能。

	\~japanese
	PropCollectionのMessage並列化ヘルパー機能 
	\~
	*/
	PROUDSRV_API CMessage& operator>>(CMessage& packet, CPropNode &rhs);

	/** 
	\~korean
	Property 를 상속받은 클래스로서 CPropForest 의 한개의 노드를 가리킨다.
	CPropNodePtr 로 생성하여 CLoadedData 의 제공함수들에서 삽입이나 삭제를 하면 된다.
	CPropNodePtr node = new(CPropNode(L"테이블이름"));
	사용예는 CProperty 를 참고.

	\~english
	As the class that inherited Property, it points a node of CPropForest.
	Created using CPropNodePtr and to perform insertion or deletion at the functions provided by CLoadedData
	CPropNodePtr node = new(CPropNode(L"table name"));
	Please refer CProperty for how to use.

	\~chinese
	是继承Property的类，指向 CPropForest%的一个节点。
	用 CPropNodePtr%生成后在 CLoadedData%的提供函数插入或者删除即可。
	CPropNodePtr node = new(CPropNode(L"表格名字"));
	使用例请参考 CProperty%。

	\~japanese
	Propertyを相続してもらったクラスとしてCPropForestの一ノードを指します。
	CPropNodePtrで生成してCLoadedDataの提供関数などで挿入または削除すれば良いです。
	CPropNodePtr node = new(CPropNode(L"テーブル名"));
	使用例はCPropertyをご参照ください。
	\~
	*/
	class  CPropNode:public CProperty
	{
		// NOTE: children을 가진다. child=하위 노드, sibling=우측 노드
		friend CLoadedData;
		friend CLoadedData_S;
		friend CLoadedData2;
		friend CLoadedData2_S;
		friend CPropForest;
		friend PROUDSRV_API CMessage& operator<<(CMessage& packet,CPropNode &rhs);
		friend PROUDSRV_API CMessage& operator>>(CMessage& packet,CPropNode &rhs);
		friend CDbCacheClientImpl;
		friend CDbCacheServerImpl;

		friend CDbCacheClient2Impl;
		friend CDbCacheServer2Impl;

	private:
		//modify by rekfkno1 - 서로 ptr로 참조하고 있으면 영원히 파괴되지 않는 
		// 현상이 있다!!!! 꼭!!! 포인터로 사용할것...

		/** 
		\~korean
		객체를 child로 참조하고 있는 Node의 포인터

		\~english
		Pointer of node that refer to object as child

		\~chinese
		把对象参照为child的Node的指针。

		\~japanese
		オブジェクトをchildに参照しているNodeのポインター
		\~
		*/
		CPropNode*  m_parent;

		/** 
		\~korean
		객체를 parent로 참조하고 있는 Node의 포인터

		\~english
		Pointer of node that refer to object as parent

		\~chinese
		把对象参照为parent的Node指针。

		\~japanese
		オブジェクトをparentに参照しているNodeのポインター
		\~
		*/
		CPropNode*  m_child;

		/** 
		\~korean
		객체와 동급의 참조 포인터

		\~english
		Reference point that has same level with object

		\~chinese
		与对象同级别的参照指针。

		\~japanese
		オブジェクトと同級の参照ポインター
		\~
		*/
		CPropNode*  m_sibling;
 
		/*
		sibling 노드의 마지막 포인터를 가지고 있다.
		*/
		CPropNode*  m_endSibling;

		// 3가지 기본 필드. 노드 자체의 guid
		Guid		m_UUID;
		// 상위 노드의 guid
		Guid		m_ownerUUID;
		// 최상위 노드의 guid. loaded data의 단위이기도 하다.
		Guid		m_RootUUID;

		// 1개 이상의 field가 변경되었지만 아직 DB에 기록되지 않았으면 true
		bool		m_dirtyFlag;

		// 로컬 메모리가 변화하면 세팅되는 값.(soft=unilateral, hard=request/response)
		bool		m_issuedSoft;
	public:
		/** 
		\~korean
		이 노드의 타입이름.
		- DbCacheSystem 에서는 보통 테이블 이름으로 사용됩니다.

		\~english
		Type name of this node.
		- It use for normal table name at DbCacheSystem

		\~chinese
		此节点的类型名字。
		- 在DbCacheSystem一般使用于表格名字。

		\~japanese
		このノードのタイプ名
		- DbCacheSystemでは普通テーブル名で使用されます。
		\~
		*/
		String	m_INTERNAL_TypeName;
		inline const PNTCHAR* GetTypeName() const { return m_INTERNAL_TypeName.GetString(); }
		PROUDSRV_API 	 void SetTypeName(const PNTCHAR* str);

		inline String GetStringTypeName() const {return m_INTERNAL_TypeName;}
		PROUDSRV_API 	 void SetStringTypeName(String str);
	private:
		PROUDSRV_API virtual void FromVariant(_variant_t from) PN_OVERRIDE;

		PROUDSRV_API virtual void FromByteArray(const ByteArray &from) PN_OVERRIDE;

		PROUDSRV_API virtual void FromByteArray(const uint8_t* data,int length) PN_OVERRIDE;
		PROUDSRV_API void	FromByteArray(const uint8_t* data,int length,Guid& siblingUUID);

		inline void	SoftWorkIssued(){m_issuedSoft = true;}
		inline void	ClearSoftWorkIssued(){m_issuedSoft = false;}
	public:

		/** 
		\~korean 
		자식 노드를 제외한 사본을 생성합니다. 

		\~english
		Clones of this node without children. 

		\~chinese
		克隆这个节点没有孩子。(TODO: translation review needed)

		\~japanese
		子ノードを除外したコピーを生成します。
		\~ 
		*/
		PROUDSRV_API  CPropNodePtr CloneNoChildren();

	public:

		/** 
		\~korean
		이 node 객체에 특정 (key, value) pair를 넣는다. 이미 같은 key를 가지는 pair가 있으면 그것을 새걸로 교체한다.
		- 이 메서드 대신 Fields[]를 직접 다루는 것이 더 편하므로 권장된다.
		\param name key of pair
		\param value value of pair  

		\~english
		Put specific (key, value) pair to node object. If pair with same key is existed then replace it to brandnew one.
		- We recommend to use Fields[] instead of this method.
		\param name key of pair
		\param value value of pair  

		\~chinese
		往这个node对象输入特定(key, value) pair。如果存在拥有相同key的pair的话，用新的代替。
		- 比起这个方法，直接使用Fields[]更方便，因此建议使用它。
		\param name key of pair
		\param value value of pair

		\~japanese
		このnodeオブジェクトに特定(key, value) pairを入れます。既に同じkeyを持つpairがあればそれを新しいものに入れ替えます。
		- このメソッドの代わりにFields[]を直接扱った方がもっと楽ですのでお勧めします。
		\param value value of pair  
		\~
		*/
		PROUDSRV_API   virtual void SetField(const String &name,const CVariant &value) PN_OVERRIDE;

		/** 
		\~korean
		이 node 객체에서 특정 (key, value) pair를 제거한다.
		\param name 제거할 pair의 key 값  

		\~english
		Remove specific (key, value) pair from this node object.
		\param name Key value of pair that need to remove

		\~chinese
		从node对象删除特定(key, value) pair。
		\param name 要删除的pair的key值。

		\~japanese
		このnodeオブジェクトで特定(key, value) pairを除去します。
		\param name 除去するpairのkey値
		\~
		*/
		PROUDSRV_API   virtual void RemoveField(const String &name) PN_OVERRIDE;

		/** 
		\~korean
		데이터 정보를 ByteArray 에 담습니다.
		- 보통 사용자는 사용할 일이 없습니다.

		\~english
		Put data information to ByteArray.
		- Ordinary user does not need to use.

		\~chinese
		把数据信息装到 ByteArray。
		- 一般的用户不需要使用。

		\~japanese
		データ情報を ByteArrayに入れます。
		- 普通ユーザーは使用することがありません。
		\~
		*/
		PROUDSRV_API   virtual void ToByteArray(ByteArray &output) PN_OVERRIDE;

		inline CPropNode* GetChild(){return m_child;}
		inline CPropNode* GetSibling(){return m_sibling;}
		inline CPropNode* GetEndSibling(){ return m_endSibling; }
#if defined (WIN32)
		__declspec(property(get=GetChild)) CPropNode* Child;
		__declspec(property(get=GetSibling)) CPropNode* Sibling;
		__declspec(property(get = GetEndSibling)) CPropNode* EndSibling;
#endif

		inline Guid	GetUUID() { return m_UUID; }
		inline Guid	GetOwnerUUID() { return m_ownerUUID;  }
		inline Guid GetRootUUID() { return m_RootUUID; }

#if defined (WIN32)
		__declspec(property(get=GetUUID)) Guid UUID;
		__declspec(property(get=GetOwnerUUID)) Guid OwnerUUID;
		__declspec(property(get=GetRootUUID)) Guid RootUUID;
		__declspec(property(get=GetStringTypeName,put=SetStringTypeName)) String TypeName; 
#endif

		PROUDSRV_API  bool	IsSoftWorkIssued(){return m_issuedSoft;}
		
		/**
		\~korean
		기본 생성자 

		\~english
		Base constructor

		\~chinese
		基本生成者。

		\~japanese
		基本生成子
		\~
		*/
		  PROUDSRV_API CPropNode(const String& TypeName);
		  PROUDSRV_API CPropNode(const CPropNode& from);
		  PROUDSRV_API virtual ~CPropNode();

		inline CPropNode & operator=( const CPropNode& from )
		{
			AssertThreadID(eAccessMode_Write);
			m_map = from.m_map;
			m_parent = from.m_parent;
			m_child = from.m_child;
			m_sibling = from.m_sibling;
			m_endSibling = from.m_endSibling;
			m_INTERNAL_TypeName = CStringPool::GetUnsafeRef().GetString(String(from.m_INTERNAL_TypeName));
			m_UUID = from.m_UUID;
			m_ownerUUID = from.m_ownerUUID;
			m_RootUUID = from.m_RootUUID;
			m_dirtyFlag = from.m_dirtyFlag;
			ClearThreadID();
			return *this;
		}

		inline CPropNode& operator =(const CProperty& from)
		{
			CProperty::operator=(from);
			return *this;
		}

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif
	};

	

	/** 
	\~korean
	CPropForest 는 1개 이상의 tree를 가지는 숲 객체입니다.
	CPropForest 의 노드는 CPropNode 타입입니다. 
	- 사용자는 가급적 사용하지 마십시요.

	다음은 사용 예시입니다.
	\code

		CPropForest f; // 가계도입니다.

		// jack이 루트로 붙습니다.
		CPropNode* jack = new CPropNode;
		f.InsertChild(NULL, jack);

		// jane이 jack과 결혼했습니다.
		CPropNode* jane = new CPropNode;
		f.InsertSiblingAfter(jack, jane);

		// 딸 wendy가 태어났습니다.
		CPropNode* wendy = new CPropNode;
		f.InsertChild(jack, wendy);

		// wendy가 신랑 tony와 결혼했습니다. 
		CPropNode* tony = new CPropNode;
		f.InsertSiblingBefore(wendy, tony);

		// tony와 wendy 사이에 terry가 태어났습니다.
		CPropNode* terry = new CPropNode;
		f.InsertChild(tony, terry);

		// tony가 바람나서 도망갔습니다. terry도 같이 데려가 버렸습니다. 
		f.RemoveNode(tony);

	\endcode

	\~english
	CPropForest is a forest type object that has more than 1 tres.
	    Node of CPropForest is CpropNode type. 

	Following is an example for usage.
	\code

		CPropForest f; // Family tress

		// Jack is attached to root.
		CPropNode* jack = new CPropNode;
		f.InsertChild(NULL, jack);

		// Jane is married to Jack.
		CPropNode* jane = new CPropNode;
		f.InsertSiblingAfter(jack, jane);

		// Daughter Wendy is born.
		CPropNode* wendy = new CPropNode;
		f.InsertChild(jack, wendy);

		// Wendy is married to Tony. 
		CPropNode* tony = new CPropNode;
		f.InsertSiblingBefore(wendy, tony);

		// Tony and Wendy bore Terry.
		CPropNode* terry = new CPropNode;
		f.InsertChild(tony, terry);

		// Tony left haveing an affair. He took Terry as well. 
		f.RemoveNode(tony);
	\endcode

	\~chinese
	CpropForest 是拥有一个以上的tree的森林对象。
	CPropForest 的节点是 CPropNode%类型。
	- 用户尽可能不要使用。

	以下是使用例子。
	\code

		CPropForest f; // 家系图。

		// jack贴为根号。.
		CPropNode* jack = new CPropNode;
		f.InsertChild(NULL, jack);

		// jane与jack结婚了。
		CPropNode* jane = new CPropNode;
		f.InsertSiblingAfter(jack, jane);

		// 女儿wendy出生了。
		CPropNode* wendy = new CPropNode;
		f.InsertChild(jack, wendy);

		// wendy与新浪tony结婚了。 
		CPropNode* tony = new CPropNode;
		f.InsertSiblingBefore(wendy, tony);

		// tony与wendy之间terry出生了。
		CPropNode* terry = new CPropNode;
		f.InsertChild(tony, terry);

		// tony外遇逃跑了。将Terry也一起带走了。 
		f.RemoveNode(tony);

	\endcode

	\~japanese
	CPropForest は1個以上のtreeを持つ森オブジェクトです。
	CPropForestのノードはCPropNodeタイプです。
	- ユーザーはなるべく使わないでください。
	次は使用例示です。
	\code
	CPropForest f; // 家系図です。
	// jackがルートにつきます。
	CPropNode* jack = new CPropNode;
	f.InsertChild(NULL, jack);

	// janeがjackと結婚しました。.
	CPropNode* jane = new CPropNode;
	f.InsertSiblingAfter(jack, jane);

	// 娘のwendyが生まれました。
	CPropNode* wendy = new CPropNode;
	f.InsertChild(jack, wendy);

	// wendyが花婿のtonyと結婚しました。
	CPropNode* tony = new CPropNode;
	f.InsertSiblingBefore(wendy, tony);

	// tonyとwendyの間でterryが生まれました。
	CPropNode* terry = new CPropNode;
	f.InsertChild(tony, terry);

	// tonyが浮気をして逃げてしましました。Terryも一緒に連れて行きました。 
	f.RemoveNode(tony);
	\endcode


	\~
	*/


	/**  @}  */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif // _WIN32
