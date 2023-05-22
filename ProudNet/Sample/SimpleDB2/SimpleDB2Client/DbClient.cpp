#include "StdAfx.h"
#include "resource.h"
#include "DbClient.h"
#include "SimpleDB2ClientDlg.h"
#include "..\SimpleDB2Common\Vars.h"

CDbClient::CDbClient(CSimpleDB2ClientDlg *pOwner) :m_pOwner(pOwner)
{
	m_DbClient.Attach(CDbCacheClient2::New());
	bRequestComplete = false;

	//tag?
	// It made for custom setting ID by user.
	// Example) INT_PTR arg = new object; Request...(...,arg);
	// OnNotify...(CCallbackArgs args){ args.m_tag;// Value has returned which put here.}
	// This may use for ID or pointer.

	//사용자가 custom하게 아이디를 설정하기 위해 만들어졌습니다.
	//사용 예) INT_PTR arg = new object; Request...(...,arg);
	// OnNotify...(CCallbackArgs args){ args.m_tag;//여기로 넣었던 값이 들어옵니다.}
	//이것은 id혹은 pointer로 사용될수 있습니다.
	m_diffCustomArg = 10;
}

CDbClient::~CDbClient(void)
{
	m_DbClient.Free();
}

void CDbClient::SimpleTest()
{
	// TODO: Adds control report precessor code to here.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static int ClickNum = 1;

	if (m_pOwner->m_sampleCase == 1)
	{
		// Check whether received request or not.
		// request 가 왔는지 확인 합니다.
		if (!GetRequestComplete())
		{
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING119));
			return;
		}

		switch (ClickNum)
		{
		case 1:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"1st------------------AddGamerData---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->RequestExclusiveLoadNewData()");
			if (!AddGamerData())			//Request type sample  //Request 형 예제 
				m_pOwner->LogError(L"AddGamerData " + m_pOwner->LoadString(IDS_STRING113));
			break;

		case 2:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"2nd------------------AddHeroData---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->RequestAddData()");

			if (!AddHeroData()) 			//Request type sample  //Request 형 예제
				m_pOwner->LogError(L"AddHeroData " + m_pOwner->LoadString(IDS_STRING113));


			break;

		case 3:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"3rd------------------UpdateHeroData---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->RequestUpdateData()");
			if (!UpdateHeroData())	//Request type sample  //Request 형 예제
				m_pOwner->LogError(L"Failed to send. Hero UUID is wrong.");
			break;

		case 4:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"4th------------------BlockedAddItem---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->BlockedAddData()");
			if (!BlockedAddItem())		//Blocked type sample  //Blocked 형 예제
				m_pOwner->LogError(L"BlockedAddItem " + m_pOwner->LoadString(IDS_STRING113));
			break;
		case 5:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"5th------------------BlockedUpdateItem---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->BlockedUpdateData()");
			if (!BlockedUpdateItem())		//Blocked type sample  //Blocked 형 예제
				m_pOwner->LogError(L"BlockedUpdateItem " + m_pOwner->LoadString(IDS_STRING113));
			break;

		case 6:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"6th------------------BlockedRemoveItem---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->BlockedRemoveData()");
			if (!BlockedRemoveItem())	//Blocked type sample  //Blocked 형 예제
				m_pOwner->LogError(L"BlockedRemoveItem " + m_pOwner->LoadString(IDS_STRING113));
			break;

		case 7:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"7th------------------AddFriend---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->UnilateralAddData()");
			// General type sample - It is based on success.
			// 일반형 예제 - 반드시 성공을 전제로 합니다.
			if (!AddFriend())
				m_pOwner->LogError(L"AddFriend failed");
			break;

		case 8:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"8th------------------UpdateFriend---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->UnilateralUpdateData()");
			// General type sample - It is based on success.
			// 일반형 예제 - 반드시 성공을 전제로 합니다.
			if (!UpdateFriend())
				m_pOwner->LogError(L"UpdateFriend " + m_pOwner->LoadString(IDS_STRING113));
			break;

			// RemoveFriend 혹은 RemoveHeroData를 삭제하지 않아도 Root를 지우면 하위노드는 전부 삭제되도록 수정되었습니다.
		case 9:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"9th------------------RemoveGamerData---------------------");
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING112) + L": m_DbClient->RemoveGamerData()");
			if (!RemoveGamerData())		//Request type sample  //Request 형 예제
				m_pOwner->LogError(L"RemoveGamerData " + m_pOwner->LoadString(IDS_STRING113));
			break;

		default:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"======================================================");
			m_pOwner->LogEvent(L"======================================================");
			m_pOwner->LogEvent(L"Test End. If you click once more, you will back to beginning.");
			ClickNum = 0;
			break;
		}
	}
	else if (m_pOwner->m_sampleCase == 2)
	{
		// NonExclusive type does not use DB Cache.
		// NonExclusive is based on that do not use functions supporting Cache.

		// NonExclusive 계열은 DB Cache를 사용하지 않습니다.
		// NonExclusive 는 Cache를 지원하는 함수들과 같이 사용하지 않는 것을 기본 개념으로 합니다.

		// Check whether received request or not.
		// request 가 왔는지 확인 합니다.
		if (!GetRequestComplete())
		{
			m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING119));
			return;
		}

		switch (ClickNum)
		{
		case 1:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"1st----------------LoadRootAddExclusive-------------------");
			LoadRootAddExclusive();
			break;
		case 2:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"2nd------------------LoadDataSnapshot---------------------");
			LoadDataSnapshot();
			break;

		case 3:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"3rd------------------AddNonExclusive---------------------");
			AddNonExclusive();
			break;
		case 4:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"4th------------------AddChildNonExclusive---------------------");
			AddChildNonExclusive();
			break;
		case 5:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"5th----------------RequestModify-------------------");
			RequestModify(ValueOperType_Plus, 10);
			break;
		case 6:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"6th----------------RequestSetIfValue-------------------");
			RequestSetIfValue(ValueCompareType_NotEqual, 10, 20);
			break;
		case 7:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"7th----------------RemoveNonExclusive-------------------");
			m_pOwner->LogEvent(L"");

			// Item 혹은 Hero를 삭제하지 않아도 Root를 지우면 하위노드는 전부 삭제되도록 수정되었습니다.
			m_pOwner->LogEvent(L"Gamer data remove");
			RemoveNonExclusive(m_data->RootUUID);
			break;


		default:
			m_pOwner->LogEvent(L"");
			m_pOwner->LogEvent(L"======================================================");
			m_pOwner->LogEvent(L"======================================================");
			m_pOwner->LogEvent(L"Test End. If you click once more, you will back to beginning.");
			ClickNum = 0;
			break;
		}

	}

	++ClickNum;

}

bool CDbClient::AddGamerData()
{
	bRequestComplete = false;
	// Adds Gamer to DB. Exclusive loading.
	// DB에 Gamer를 추가한다.독점로딩.
	CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Gamer"));
	newNode->Fields[L"GamerID"] = CVariant(m_GamerName);
	newNode->Fields[L"Password"] = L"TestPass";

	m_DbClient->RequestExclusiveLoadNewData(L"Gamer", newNode, 0, true);
	return true;
}

bool CDbClient::AddHeroData()
{
	bRequestComplete = false;
	// Adds Hero to gamer.
	// 게이머에 Hero를 추가한다.
	CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Hero"));
	newNode->Fields[L"Name"] = CVariant(m_heroName);
	newNode->Fields[L"Removed"] = 0;
	newNode->Fields[L"HeroType"] = 1;

	if (!m_DbClient->RequestAddData(m_data->RootUUID, m_data->RootUUID, newNode, m_diffCustomArg))
	{
		m_pOwner->LogError(L"AddHeroData " + m_pOwner->LoadString(IDS_STRING114));
		return false;
	}

	return true;
}

bool CDbClient::UpdateHeroData()
{
	bRequestComplete = false;
	CPropNodePtr heroNode = m_data->GetNode(m_heroGuid);
	if (!heroNode || heroNode->UUID == Proud::Guid())
		return false;

	// Change information of hero.
	// 히어로의 정보를 바꾼다.
	heroNode->Fields[L"HeroType"] = 2;

	// Update information of hero.
	// 히어로의 정보를 업데이트한다.
	if (!m_DbClient->RequestUpdateData(heroNode, m_diffCustomArg))
	{
		m_pOwner->LogError(L"UpdateHeroData " + m_pOwner->LoadString(IDS_STRING114));
		return false;
	}
	// !!! If Request has joined, it can do other thing when it receives request response.
	// If you want to use function that automatically waiting and return then use Blocked function.
	// !!! Request가 붙으면 반드시 기다렸다가 상대가 돌아왔을 때에만 다른 무언가를 할 수 있습니다.
	// 자동으로 기다렸다가 리턴하는 함수를 사용하고자 할 때에는 Blocked 함수를 사용하면 됩니다.
	return true;
}

bool CDbClient::RemoveHeroData()
{
	bRequestComplete = false;
	// Delete information of hero.
	// 히어로의 정보를 삭제한다.
// 	if(!m_DbClient->RequestRemoveData(m_data->RootUUID, m_heroGuid, m_diffCustomArg))
// 	{
// 		m_pOwner->LogError(L"RemoveHeroData " + m_pOwner->LoadString(IDS_STRING114));
// 		return false;
// 	}
	// !!! If Request has joined, it can do other thing when it receives request response.
	// If you want to use function that automatically waiting and return then use Blocked function.
	// !!! Request가 붙으면 반드시 기다렸다가 상대가 돌아왔을 때에만 다른 무언가를 할 수 있습니다.
	// 자동으로 기다렸다가 리턴하는 함수를 사용하고자 할 때에는 Blocked 함수를 사용하면 됩니다.
	return true;
}

bool CDbClient::BlockedAddItem()
{
	// Adds Item as lower of hero.
	// 히어로의 하위로 Item을 추가한다.
	CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Item"));
	newNode->Fields[L"Name"] = L"15Sword";
	newNode->Fields[L"Type"] = 1;

	ErrorInfoPtr outerror = ErrorInfoPtr(new ErrorInfo);
	if (!m_DbClient->BlockedAddData(m_data->RootUUID, m_data->RootUUID, newNode, 10000, outerror))
	{
		m_pOwner->LogError(L"BlockedAddData " + m_pOwner->LoadString(IDS_STRING114));

		String txt;
		txt.Format(L"%s occured at %s", outerror->ToString().GetString(), __FUNCTIONW__);
		m_pOwner->LogError(txt.GetString());

		return false;
	}

	// Item Guid update
	m_itemGuid = newNode->UUID;
	// It only returns when it receive message that called Blocked so data update can available after return.
	// Blocked는 메시지가 도착해야지만 리턴되기 때문에 리턴 받은 후 data를 업데이트할 수 있습니다.
	m_data = m_DbClient->GetClonedLoadedDataBySessionGuid(m_SessionGuid);
	return true;
}

bool CDbClient::BlockedUpdateItem()
{
	CPropNodePtr newNode2 = m_data->GetNode(m_itemGuid);
	if (!newNode2 || newNode2->UUID == Proud::Guid())
		return false;
	newNode2->Fields[L"Name"] = L"18Sword";
	newNode2->Fields[L"Type"] = 18;

	//virtual bool BlockedUpdateData(CLoadedData2Ptr loadedData,bool transactional = true,DWORD timeOutTime = 30000,ErrorInfoPtr outError = ErrorInfoPtr()) = 0;
	//virtual bool BlockedUpdateData(CPropNodePtr updateData,DWORD timeOutTime = 30000,ErrorInfoPtr outError = ErrorInfoPtr()) = 0;
	ErrorInfoPtr outerror = ErrorInfoPtr(new ErrorInfo);
	if (!m_DbClient->BlockedUpdateData(newNode2, 30000, outerror))
	{
		m_pOwner->LogError(L"BlockedUpdateData " + m_pOwner->LoadString(IDS_STRING114));

		String txt;
		txt.Format(L"%s occured at %s", outerror->ToString().GetString(), __FUNCTIONW__);
		m_pOwner->LogError(txt.GetString());

		return false;
	}
	m_itemGuid = newNode2->UUID;
	m_data = m_DbClient->GetClonedLoadedDataBySessionGuid(m_SessionGuid);
	return true;

}

bool CDbClient::BlockedRemoveItem()
{
	ErrorInfoPtr errorout = ErrorInfoPtr(new ErrorInfo);
	if (!m_DbClient->BlockedRemoveData(m_data->RootUUID, m_itemGuid, 10000, errorout))
	{
		m_pOwner->LogError(L"BlockedRemoveItem " + m_pOwner->LoadString(IDS_STRING114));

		String txt;
		txt.Format(L"%s occured at %s", errorout->ToString().GetString(), __FUNCTIONW__);
		m_pOwner->LogError(txt.GetString());

		return false;
	}

	// It only returns when it receive message that called Blocked so data update can available after return.
	// Blocked는 메시지가 도착해야지만 리턴되기 때문에 리턴 받은 후 data를 업데이트할 수 있습니다.
	m_data = m_DbClient->GetClonedLoadedDataBySessionGuid(m_SessionGuid);
	return true;
}

bool CDbClient::AddFriend()
{
	CPropNodePtr newFriend = CPropNodePtr(new CPropNode(L"Friend"));
	newFriend->Fields[L"charLevel"] = 100;
	newFriend->Fields[L"Name"] = L"I'm your father";

	if (!m_DbClient->UnilateralAddData(m_data->RootUUID, m_data->RootUUID, newFriend, true))
	{
		m_pOwner->LogError(L"AddFriend " + m_pOwner->LoadString(IDS_STRING114));
		return false;
	}

	// Friend Guid Update
	// 친구 Guid Update
	m_friendGuid = newFriend->UUID;

	// Adds to m_data.
	// It is for synchronize with internal data of dbclient and data at here.
	//m_data에도 추가하여 줍니다.
	//dbclient의 내부 데이터와 여기의 데이터를 동기화 하기 위함입니다.
	m_data->InsertChild(m_data->GetRootNode(), newFriend);

	// Or,
	// You can synchronize like this.
	//혹은,
	//이렇게 동기화를 하여도 되겠습니다.
	//m_data = m_DbClient->GetClonedLoadedDataBySessionGuid(m_SessionGuid);
	return true;
}

bool CDbClient::UpdateFriend()
{
	CPropNodePtr node = m_data->GetNode(m_friendGuid);
	if (!node || node->UUID == Proud::Guid())
	{
		m_pOwner->LogError(L"UpdateFriend " + m_pOwner->LoadString(IDS_STRING114) + m_pOwner->LoadString(IDS_STRING115));
		return false;
	}

	node->Fields[L"charLevel"] = 300;

	if (!m_DbClient->UnilateralUpdateData(node, true))
	{
		m_pOwner->LogError(L"UpdateFriend " + m_pOwner->LoadString(IDS_STRING114));
		return false;
	}
	return true;
}

bool CDbClient::RemoveFriend()
{
	if (!m_DbClient->UnilateralRemoveData(m_data->RootUUID, m_friendGuid, true))
	{
		m_pOwner->LogError(L"RemoveFriend " + m_pOwner->LoadString(IDS_STRING114));
		return false;
	}

	CPropNodePtr friendNode = m_data->GetNode(m_friendGuid);
	m_data->RemoveNode(friendNode);

	return true;
}

bool CDbClient::RemoveGamerData()
{
	// 	if(!m_DbClient->RequestRemoveData(m_data->RootUUID, m_data->RootUUID, m_diffCustomArg))
	// 	{
	// 		m_pOwner->LogError(L"RemoveGamerData " + m_pOwner->LoadString(IDS_STRING114));
	// 		return false;
	// 	}

		// UnilateralRemoveData test
		/*if(!m_DbClient->UnilateralRemoveData(m_data->RootUUID, m_data->RootUUID, true))
		{
			m_pOwner->LogError(L"RemoveGamerData " + m_pOwner->LoadString(IDS_STRING114));
			return false;
		}*/

	return true;
}

void CDbClient::RequestCompleteTrue()
{
	CriticalSectionLock TRLock(m_CSLock, true);
	bRequestComplete = true;
}

bool CDbClient::GetRequestComplete()
{
	CriticalSectionLock TRLock(m_CSLock, true);
	return bRequestComplete;
}

/************************ NonExclusive **************************/

void CDbClient::LoadRootAddExclusive()
{
	// Request Add&Loading Root information
	// Root 정보를 Add&로딩 요청
	m_pOwner->LogEvent(m_pOwner->LoadString(IDS_STRING111));
	Proud::CPropNodePtr newprop = CPropNodePtr(new CPropNode(L"Gamer"));
	newprop->SetField(L"GamerID", CVariant(m_GamerName));
	newprop->SetField(L"Password", L"1234");

	bRequestComplete = false;

	m_DbClient->RequestExclusiveLoadNewData(L"Gamer", newprop, m_diffCustomArg, false);
}

void CDbClient::LoadDataSnapshot()
{
	bRequestComplete = false;
	m_DbClient->RequestNonExclusiveSnapshotData(L"Gamer", L"GamerID", CVariant(m_GamerName), m_diffCustomArg);
}

void  CDbClient::AddNonExclusive()
{
	// Add Hero 
	// Hero Table의 Hero 정보 삽입
	CPropNodePtr newData = CPropNodePtr(new CPropNode(L"Hero"));
	newData->SetField(L"Name", CVariant(m_heroName));
	newData->SetField(L"Removed", 0);
	newData->SetField(L"HeroType", 1);

	if (!m_data)
		m_pOwner->LogError(L"AddNonExclusive m_data is NULL.");

	// DB에 Hero 정보를 추가 합니다.
	bRequestComplete = false;
	m_DbClient->RequestNonExclusiveAddData(m_data->GetRootNode()->TypeName,
		m_data->RootUUID, m_data->RootUUID, newData, m_diffCustomArg, Proud::ByteArray());
}

void CDbClient::AddChildNonExclusive()
{
	// addItem - Insert Field information at Item table
	// addItem - Item 테이블의 Field정보 삽입
	CPropNodePtr newData = CPropNodePtr(new CPropNode(L"Item"));
	newData->SetField(L"Name", L"Sword");
	newData->SetField(L"Type", 2);

	if (!m_data)
		m_pOwner->LogError(L"AddChildNonExclusive m_data is NULL.");

	// Adds item to DB.
	// DB에 아이템을 추가합니다.
	bRequestComplete = false;
	m_DbClient->RequestNonExclusiveAddData(L"Gamer", m_data->RootUUID, m_heroGuid, newData, m_diffCustomArg, ByteArray());
}


void CDbClient::RequestModify(Proud::ValueOperType type, int value)
{
	if (!m_data)
		m_pOwner->LogError(L"RequestModify m_data is NULL.");


	CPropNodePtr modifyData = m_data->GetNode(m_itemGuid);
	if (!modifyData)
		m_pOwner->LogError(L"RequestModify m_itemGuid is wrong.");


	int var = modifyData->GetField(L"Type");

	switch (type)
	{
	case ValueOperType_Plus:
		var += value;
		break;
	case ValueOperType_Minus:
		var -= value;
		break;
	case ValueOperType_Multiply:
		var *= value;
		break;
	case ValueOperType_Division:
		var /= value;
		break;
	}
	bRequestComplete = false;


	// Change data value of DB. 
	// DB의 데이터의 값을 변화 시킵니다. 
	m_DbClient->RequestNonExclusiveModifyValue(L"Gamer", m_data->RootUUID, modifyData->UUID, L"Type", type, value, m_diffCustomArg, ByteArray());
}


void CDbClient::RemoveNonExclusive(Proud::Guid &removeUUID)
{
	// RemoveNonExclusive is repeatly calling at source.
	// So wait under message has arrived then process Remove command.
	// RemoveNonExclusive는 현재 소스에서 연속으로 호출되고 있습니다.
	// 때문에 메시지가 올 때가지 기다렸다가 다음 Remove명령을 수행합니다.
	while (!GetRequestComplete())
	{
		Sleep(100);
	}

	bRequestComplete = false;
	// Delete information of DB.
	// DB의 정보를 삭제 합니다.
	m_DbClient->RequestNonExclusiveRemoveData(L"Gamer", m_data->RootUUID, removeUUID, m_diffCustomArg, ByteArray());
}


void CDbClient::RequestSetIfValue(Proud::ValueCompareType tyep, int value, int value2)
{
	Proud::Guid waitTicket;

	m_data = m_DbClient->GetClonedLoadedDataByUUID(m_data->RootUUID);

	// Get PropNode of changing item.
	// 변화시킬 아이템의 PropNode를 얻어옵니다.
	CPropNodePtr modifyData = m_data->GetNode(m_itemGuid);
	int var = value;

	bRequestComplete = false;

	//  Changing value by Proud::ValueCompareType option.
	//  Proud::ValueCompareType 조건에 의하여 값을 변화시킵니다.
	m_DbClient->RequestNonExclusiveSetValueIf(L"Gamer", m_data->RootUUID, modifyData->UUID, L"Type", value, tyep, value2, m_diffCustomArg, ByteArray());
}