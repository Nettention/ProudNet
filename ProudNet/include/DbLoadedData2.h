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

#if defined(_WIN32)

#include "BasicTypes.h"
#include "FakeClr.h"
#include "PropNode.h"
#include "FastMap.h"

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

    class CDbCacheClient2Impl;
    class CDbCacheServer2Impl;
	class CLoadedData2;

	typedef RefCount<CLoadedData2> CLoadedData2Ptr;
	typedef CFastMap<Guid, CPropNodePtr> PropNodeMap;
	typedef CFastList<CPropNodePtr> PropNodeList;

    class CLoadedData2
    {
        friend CDbCacheClient2Impl;
        friend CDbCacheServer2Impl;
        
	private:
		// forest를 직접 소유함. 
		CPropNode*	m_Root;
		// forest에서 guid를 키로 빨리 찾게 하기 위함
		PropNodeMap		m_propNodeMap;
		// DB에 delete 쿼리로 던져져야 하는 노드들
		PropNodeList	m_removePropNodeList;

		/*
		\~korean
		이것을 로딩한 DB cache client가 보내준 session 인식자

		\~english
		Session identifier that sent by loaded DB cache client

		\~chinese
		这个加载的DB cache client发送的session的识别者。
                
		\~japanese
		これをローディングしたDB cache clientが送ってくれたsession認識子
		\~
		*/
		Guid m_INTERNAL_sessionGuid;

        CReaderWriterAccessChecker m_RWAccessChecker;


		inline int	GetRemoveCount(){ return (int)m_removePropNodeList.GetCount(); }

		int _INTERNAL_NOACCESSMODE_GetDirtyCount();		
		CPropNodePtr _INTERNAL_NOACCESSMODE_GetNode(const Guid& guid);
		Proud::ErrorType _INTERNAL_NOACCESSMODE_CheckNode(CPropNodePtr node);

		Proud::ErrorType _INTERNAL_NOACCESSMODE_InsertChild(CPropNodePtr parent, CPropNodePtr newNode);
// 		Proud::ErrorType _INTERNAL_NOACCESSMODE_InsertSiblingAfter(CPropNodePtr sibling, CPropNodePtr newNode);
// 		Proud::ErrorType _INTERNAL_NOACCESSMODE_InsertSiblingBefore(CPropNodePtr sibling, CPropNodePtr newNode);
		CPropNode* _INTERNAL_NOACCESSMODE_GetEndSibling(CPropNode* node);

		Proud::ErrorType _INTERNAL_NOACCESSMODE_RemoveNode(CPropNodePtr node, bool addremovelist);
		Proud::CPropNodePtr _INTERNAL_NOACCESSMODE_GetRemoveNode(Proud::Guid &removeUUID);
		Proud::ErrorType _INTERNAL_NOACCESSMODE_AddRemovePropNodeList(CPropNodePtr node);
		
		void _INTERNAL_NOACCESSMODE_SerializeToMessage(CMessage &msg);
		void _INTERNAL_NOACCESSMODE_DeserializeFromMessageToChangeList(CMessage &msg);
		
		Proud::ErrorType _INTERNAL_NOACCESSMODE_MovePropNode(CPropNodePtr nodeToMove, CLoadedData2& destForest, CPropNodePtr& destParentNode);
		void _INTERNAL_NOACCESSMODE_RecursiveUpdateRootUUID(Proud::Guid& RootUUID, CPropNode* Node);
		void _INTERNAL_NOACCESSMODE_RecursiveRemove(CPropNode *node);
		void _INTERNAL_NOACCESSMODE_RecursiveAdd(CPropNodePtr node, PropNodeMap * destMap);
		
		void _INTERNAL_NOACCESSMODE_DeleteAll();

		void _INTERNAL_NOACCESSMODE_CopyTo(CLoadedData2& to);
		void _INTERNAL_NOACCESSMODE_CopyTo_Diff(CLoadedData2& to);
		void _INTERNAL_NOACCESSMODE_ClearChangeNode();

		CLoadedData2 & operator=(const CLoadedData2& from);

    protected:
		PROUDSRV_API virtual void	AssertThreadID(eAccessMode eMode) const;
		PROUDSRV_API         virtual void	ClearThreadID() const;
        
	protected:
		PROUDSRV_API 		virtual void Change_Serialize(CMessage& msg,bool isReading);
		PROUDSRV_API 		virtual CPropNodePtr GetRemoveNode(Proud::Guid removeUUID);

		PROUDSRV_API 		virtual void Serialize( CMessage& msg,bool isReading );
		PROUDSRV_API 		 void CopyTo_NoChildren(CLoadedData2* output);

    public:
		PROUDSRV_API         CLoadedData2();
		PROUDSRV_API 		CLoadedData2(const CLoadedData2& from);
		PROUDSRV_API          virtual ~CLoadedData2();
		/** 
		\~korean
		nodeToMove 노드를 destForest의 destParentNode 하위 노드로 이동 시킵니다. 
		nodeToMove의 OwnerUUID,RootUUID가 변경되고
		nodeToMove의 하위 노드의 RootUUID가 destForest의 RootUUID로 변경됩니다.

		\param destForest destParentNode가 속해있는 Forest
		\param nodeToMove 이동시킬 노드
		\param destParentNode 이동시킬 노드의 Owner Node
		\return 성공적으로 이동하면 ErrorType_Ok를 리턴합니다. 그 이외에는 다른 오류 값이 리턴됩니다.

		\~english 
		Move nodetoMove node to the lower node of destparentNode of destForest. 
		OwnerUUID,RootUUID of nodeToMove is changed and 
		RootUUID of the lower node of nodeToMove is changed to RootUUID of destForest
		\param destForest Forest where destParentNode belongs to
		\param nodeToMove 이동시킬 노드 Node to be moved 
		\param destParentNode Owner node of the node to be moved 
		\return When moved successfully, it returns ErrorType_Ok. Otherwise another error value is returned. 

		\~chinese
		把nodeToMove节点移动到destForest的destParentNode下一级节点。 
		nodeToMove的OwnerUUID,RootUUID将会变更
		nodeToMove的下一级节点的RootUUID变更为destForest的RootUUID。

		\paramdestForestdestParentNode包含的Forest
		\paramnodeToMove 要移动的节点
		\paramdestParentNode 要移动的节点的Owner Node
		\return 成功移动之后返回ErrorType_Ok。否则返回其他错误值。
		
		\~japanese
		nodeToMove ノードをdestForestのdestParentNode下位ノードに移動させます。
		nodeToMoveのOwnerUUID,RootUUIDが変更されて、
		nodeToMoveの下位ノードのRootUUIDがdestForestのRootUUIDに変更されます。

		\param destForest destParentNodeが属しているForest
		\param nodeToMove 移動させるノード
		\param destParentNode 移動させるノードのOwner Node
		\return 移動に成功したらErrorType_Okをリターンします。その他には他のエラー値がリターンされます。
		\~
		*/
		PROUDSRV_API 		Proud::ErrorType MovePropNode(CLoadedData2& destForest, CPropNodePtr nodeToMove, CPropNodePtr destParentNode);
	
		/** 
		\~korean
		parent 의 마지막 자식 노드로서 newNode를 넣습니다. parent=NULL인 경우 최상단의 노드로서 newNode가 추가됩니다.

		\param parent 만약 NULL인 경우 CPropTree 의 최상위 레벨의 맨 마지막 노드로 붙습니다.
		\param newNode 새로 추가할 노드. 어떠한 CPropForest 에도 이미 종속된 상태이어서는 안됩니다. 이를 어기면 오류 값을 리턴합니다.
		\return 성공적으로 추가하면 ErrorType_Ok 를 리턴합니다. 그 이외에는 다른 오류 값이 리턴됩니다.

		\~english
		Enters newNode as the last child node of parent. If parent=NULL then newNode will be added as the top node.
		If the newNode already has other child nodes then all of them will enter CPropForest all together.

		\param parent If it is equal to NULL then it will be attached as the last node of the top most node of CPropTree.
		\param newNode The node to be added newly. It must not be dependent to any CPropForest. If so, an error value will be returned.
		\return Returns ErrorType_Ok if successfuly added. Otherwise, a different error value will be returned.

		\~chinese
		因为是Parent的最后一个子子女节点node,要放入newNode。parent=NULL时，因为是最上端节点node以所以添加newNode。

		\param parent 如果是NULL的时候， CPropTree%的最上位级别的最后一个node来粘贴。
		\param newNode 要添加的新node。
		不管是什么样的 CPropForest%里已经从属状态的话是不行的。违背这一点返回错误数值。
		\return 成功的添加的话ErrorType_Ok可以返回。除此之外别的错误值返回。

		\~japanese
		Parentの最後の子ノードとしてnewNodeを入れます。parent=NULLの場合、最上段のノードとしてnewNodeが追加されます。
		
		\param parent もし、NULLである場合、CPropTreeの最上位レベルの最後ノードに付きます。
		\param newNode 新たに追加するノード。いかなるCPropForestにも既に従属された状態になってはいけません。これを守らなければエラー値をリターンします。
		\return 追加に成功したらErrorType_Okをリターンします。その他には他のエラー値がリターンされます。
		\~
		*/
		PROUDSRV_API          Proud::ErrorType InsertChild(CPropNodePtr parent, CPropNodePtr newNode);

		/** 
		\~korean
		이 객체이 이미 붙어있던 CPropNode 객체를 분리해냅니다.
		분리된 node의 child도 CPropForest 로부터 분리됩니다. 하지만 분리된 node의 child들은 node와의 자식관계를 계속 유지합니다.

		\param node 분리할 노드입니다. 이 CPropForest 에 종속되어 있어야 합니다.이를 어기면 오류 값을 리턴합니다.
		\param addremovelist removePropNodelist 에 추가할지에 대한 여부입니다. true 로 셋팅하신후 UpdateData를 호출하면 자동으로 DBMS에서 삭제됩니다.
		\return 성공적으로 제거하면 ErrorType_Ok 를 리턴합니다. 그 이외에는 다른 오류 값이 리턴됩니다.

		\~english TODO:translate needed.
		Separate the CPropNode object already attached to this object. 
		The child of the separated node is separated from CPropForest. However, the children of the separated node still maintain the parent and child relationship with the node. 

		\param node Node to be separated. It must be subordinated to this CPropForest. 
		\param addremovelist This is whether to add to removePropNodelist. After setting it to true and calling UpdateData, it is automatically deleted from DBMS. 
		\return When deleted successfully, it returns ErrorType_Ok. Otherwise another error value is returned. 

		\~chinese
		这个对象可以分离已经粘贴过的 CPropNode%的对象。
		分离的node的child从 CPropForest%分离出来。但是分离node的child们与node继续维持子女关系。

		\param node 需要分离的node。这个 CPropForest%已经处于从属状态。违背这一点返回错误数值。
		\param addremovelist removePropNodelist是否要添加的与否。 True设定后呼叫UpdateData的话自动在DBMS删除。 
		\return 成功的删除的话返回ErrorType_Ok。除此之外别的错误值返回。

		\~japanese
		このオブジェクトが既についていたCPropNodeオブジェクトを分離します。
		分離されたnodeのchildもCPropForestから分離されます。しかし、分離されたnodeのchildはnodeとの親子関係を続けて維持します。
		\param node 分離するノードです。このCPropForestに従属されていなければなりません。これを守らなければエラー値をリターンします。
		\param addremovelist removePropNodelist に追加するかどうかです。Trueに設定した後、UpdateDataを呼び出すと自動にDBMSから削除されます。
		\return 除去に成功したらErrorType_Okをリターンします。その他には他のエラー値がリターンされます。
		\~
		*/
		PROUDSRV_API          virtual Proud::ErrorType RemoveNode(CPropNodePtr node, bool addremovelist=true);

		/** 
		\~korean
		RemoveNode(node)와 같은 기능을 하지만, UUID로 동작 합니다.

		\param removeUUID 제거할 Node UUID
		\param addremovelist removePropNodelist에 추가할지에 대한 여부입니다. true 로 셋팅하신후 UpdateData를 호출하면 자동으로 DBMS에서 삭제됩니다.
		\return 성공적으로 제거하면 ErrorType_Ok 를 리턴합니다.그 이외에는 다른 오류 값이 리턴됩니다.

		\~english 
		It has the same function as RemoveNode(node) but operates as UUID. 
		\param Node UUID to be deleted
		\param This is whether to add to removePropNodelist. After setting it to true and calling UpdateData, it is automatically deleted from DBMS. 
		\return When deleted successfully, it returns ErrorType_Ok. Otherwise another error value is returned.

		\param removeUUID Node UUID that will be removed.

		\return Once it successfully removed then return ErrorType_Ok instead of this case, other error value will be returned.

		\~chinese
		虽然与RemoveNode(node) 一样的技能，但是以UUID来启动。

		\param removeUUID 要删除的Node UUID
	        \param addremovelist 是否要添加到removePropNodelist的与否。True设定后呼叫UpdateData的话自动在DBMS删除。    
	        \return 成功的取消的话ErrorType_Ok可以返回。除此之外别的错误值返回。

		\~japanese
		RemoveNode(node)のような機能をしますが、UUIDで動作します。
		\param removeUUID 除去する Node UUID
		\param addremovelist removePropNodelistに追加するかどうかです。trueに設定した後、UpdateDataを呼び出すと自動にDBMSから削除されます。
		\return 除去に成功したらErrorType_Okをリターンします。その他には他のエラー値がリターンされます。
		\~
		*/
		PROUDSRV_API 		 virtual Proud::ErrorType RemoveNode(Proud::Guid removeUUID, bool addremovelist=true);

		/** 
		\~korean
		guid에 해당하는 node를 리턴 합니다.검색 용도로 사용됩니다.
		\param guid 찾아낼 node의 uuid
		\return 성공적이라면, node를 리턴 합니다. NULL 이라면 없는 node 입니다.

		\~english
		Return node that as guid. It will use for searching.
		\param UUID of node that find out guid
		\return If it succeed, return node. If it is NULL, it is not exsit node.

		\~chinese
		Guid相应的node返回。适用搜索功能。
		\param guid 需要找的node的uuid。
		\return 成功的话node返回，NULL的话没有node。

		\~japanese
		Guidに該当するnodeをリターンします。検索用とで使われます。
		\param guid 見つけ出すnodeのuuid
		\return 成功なら、nodeをリターンします。NULL なら、無いnodeです。
		\~
		*/
		PROUDSRV_API 		 virtual CPropNodePtr GetNode(const Guid& guid);
		
		/** 
		\~korean
		이 객체가 가지고 있는 최상위 데이터 노드를 리턴합니다.
		\return 성공적이라면, 최상위 노드를 리턴합니다. 실패라면 비어 있는 CPropNodePtr 을 리턴합니다.

		\~english
		It returns top data node that this object owned.
		\return If it is succeed, return top node. If it is failed, return an empty CpropNodePtr.

		\~chinese
		这个对象拥有的返回最上位数据node。
		\return 成功的话，返回到最上位node。失败的话返回到空的 CPropNodePtr%。

		\~japanese
		このオブジェクトが持っている最上位データノードをリターンします。
		\return 成功なら、最上位ノードをリターンします。失敗なら空いているCPropNodePtrをリターンします。
		\~
		*/
		PROUDSRV_API          virtual CPropNodePtr GetRootNode();

		/** 
		\~korean
		이 객체가 가지고 있는 최상위 데이터 노드의 UUID를 리턴합니다.

		\~english
		It returns top data node that this object owned.

		\~chinese
		返回到这个对象拥有的最上位数据节点的UUID。

		\~japanese
		このオブジェクトが持っている最上位データノードのUUIDをリターンします。
		\~
		*/
		PROUDSRV_API          Guid GetRootUUID();

		/** 
		\~korean
		Session Guid를 얻는다.

		\~english TODO:translate needed.

		\~chinese
		取得Session Guid。

		\~japanese
		Session Guidを得ます。
		\~
		*/
		PROUDSRV_API 		 Guid GetSessionGuid() const;

		/** 
		\~korean
		\param val SessionGuid를 설정한다.

		\~english
		Set SessionGuid.

		\~chinese
		\param val 设置SessionGuid。

		\~japanese
		\param val SessionGuidを設定します。
		\~
		*/
		PROUDSRV_API 		 void SetSessionGuid(Guid val);

#if defined (_MSC_VER)
		__declspec(property(get=GetSessionGuid,put=SetSessionGuid)) Guid sessionGuid;
		__declspec(property(get=GetRootUUID)) Guid RootUUID;
#endif

		/** 
		\~korean
		데이터를 복사하여 다른 객체를 만듭니다.

		\~english
		Create other object to copy data.

		\~chinese
		数据复制后制作别的对象。

		\~japanese
		データをコピーして他のオブジェクトを作ります。
		\~
		*/
		PROUDSRV_API 		 CLoadedData2Ptr Clone();

		/** 
		\~korean
		내부 데이터를 ByteArray 로 담습니다.
		\param output 데이터가 담길 ByteArray 객체 입니다.

		\~english
		Put internal data to ByteArray.
		\param ByteArray object for output data.

		\~chinese
  		装进内部数据 ByteArray。
		\param output 要装入的数据 ByteArray%对象。

		\~japanese
		内部データを ByteArrayに入れます。
		\param output データが入るByteArrayオブジェクトです。

		\~
		*/
		PROUDSRV_API 		 void ToByteArray(ByteArray& output);
		/** 
		\~korean
		input로 부터 데이터를 복사합니다.

		\param input 데이터가 담긴 ByteArray 객체 입니다.

		\~english
		Copy data from input.

		\param ByteArray object that has input data.

		\~chinese
		从input复制数据。
		
		\param input 已被装入的数据 ByteArray%对象。

		\~japanese
		Inputからデータをコピーします。
		\param input データが入るByteArrayオブジェクトです。
		\~
		*/
		PROUDSRV_API 		 void FromByteArray(const ByteArray& input);
		
		// 트리의 consistent 를 확인 하기 위해서.
/*		 bool IsConsistent();*/

		/** 
		\~korean
		DBCacheServer2에서 내부적으로 사용하는 함수들 입니다.
		- 사용자는 사용하지 말아 주세요.

		\~english
		Function that use internally at DBCacheServer2.
		- Do not use.

		\~chinese
		DBCacheServer2里是内部使用的函数。
		- 用户请不要使用。

		\~japanese
		DBCacheServer2で内部的に使う関数です。
		- ユーザーは使わないでください。
		\~
		*/
		PROUDSRV_API 		 virtual void _INTERNAL_ClearChangeNode();

		/** 
		\~korean
		DBCacheServer2에서 내부적으로 사용하는 함수들 입니다.
		- 사용자는 사용하지 말아 주세요.

		\~english
		Function that use internally at DBCacheServer2.
		- Do not use.

		\~chinese
		DBCacheServer2里是内部使用的函数。
		- 用户请不要使用。

		\~japanese
		DBCacheServer2で内部的に使う関数です。
		- ユーザーは使わないでください。
		\~
		*/
		PROUDSRV_API 		 virtual void _INTERNAL_ChangeToByteArray(ByteArray& outArray);

		/** 
		\~korean
		DBCacheServer2에서 내부적으로 사용하는 함수들 입니다.
		- 사용자는 사용하지 말아 주세요.

		\~english
		Function that use internally at DBCacheServer2.
		- Do not use.

		\~chinese
		DBCacheServer2里是内部使用的函数。
		- 用户请不要使用。

		\~japanese
		DBCacheServer2で内部的に使う関数です。
		- ユーザーは使わないでください。
		\~
		*/
		PROUDSRV_API 		 virtual void _INTERNAL_FromByteArrayToChangeList(const ByteArray &inArray);

		/** 
		\~korean
		DBCacheServer2에서 내부적으로 사용하는 함수들 입니다.
		- 사용자는 사용하지 말아 주세요.

		\~english
		Function that use internally at DBCacheServer2.
		- Do not use.

		\~chinese
		DBCacheServer2里是内部使用的函数。
		- 用户请不要使用。

		\~japanese
		DBCacheServer2で内部的に使う関数です。
		- ユーザーは使わないでください。
		\~
		*/
		PROUDSRV_API 		 virtual const PropNodeList* _INTERNAL_GetRemoveNodeList() const;

		/** 
		\~korean
		DBCacheServer2에서 내부적으로 사용하는 함수들 입니다.
		- 사용자는 사용하지 말아 주세요.

		\~english
		Function that use internally at DBCacheServer2.
		- Do not use.

		\~chinese
		DBCacheServer2里是内部使用的函数。
		- 用户请不要使用。

		\~japanese
		DBCacheServer2で内部的に使う関数です。
		- ユーザーは使わないでください。
		\~
		*/
		PROUDSRV_API 		 virtual Proud::ErrorType _INTERNAL_AddRemovePropNodeList(Proud::Guid removeUUID);


#ifdef _WIN32
#pragma push_macro("new")
#undef new
        // 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif
    };

	/**  @} */

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif // _WIN32
