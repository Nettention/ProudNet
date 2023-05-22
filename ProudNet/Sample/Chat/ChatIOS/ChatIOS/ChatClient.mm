//
//  ChatClient.mm
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 16..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//


// ProudNet 을 쓰는 mm혹은 cpp 파일은 꼭 <new>를 포함해야합니다.

#include <new>
#include "ChatClient.h"
#include "ChatC2C_common.cpp"
#include "ChatC2C_proxy.cpp"
#include "ChatC2C_stub.cpp"
#include "ChatC2S_common.cpp"
#include "ChatC2S_proxy.cpp"
#include "ChatS2C_common.cpp"
#include "ChatS2C_stub.cpp"
#include "AppDelegate.h"
#include "MainViewController.h"
#include "ConnectViewController.h"
#include "ServerChatViewController.h"
#include "P2PGroupViewController.h"
#include "P2PChatViewController.h"

CChatClient::CChatClient(MainViewController *maincontroller)
{
    m_client = CNetClient::Create();
    m_client->SetEventSink(this);
    m_client->AttachProxy(&m_C2SProxy);
    m_client->AttachProxy(&m_C2CProxy);
    m_client->AttachStub((ChatC2C::Stub*)this);
    m_client->AttachStub((ChatS2C::Stub*)this);
    m_mainController = maincontroller;
}

CChatClient::~CChatClient()
{
    m_mainController = nil;
}

bool CChatClient::Connect(String serverAdress)
{
    m_client->Disconnect();

    CNetConnectionParam param;
    param.m_serverIP = serverAdress;
    param.m_serverPort = (uint16_t)g_ServerPort;
    param.m_protocolVersion = g_ProtocolVersion;
    
    return m_client->Connect(param);
}

void CChatClient::OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer)
{
    if(info->m_errorType != ErrorType_Ok)
    {
        NSString *errstr = [[NSString alloc] initWithFormat:@"ErrorType:%d,DetailType:%d",(int)info->m_errorType,(int)info->m_detailType];

        // 경고창띄우기
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnJoinServerComplete" message:errstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

        [alert show];
        [errstr release];
        [alert release];

    }
    else 
    {
        // 로그온 요청메시지 보내기
        ConnectViewController *connectController = m_mainController.connectViewController;

        if(connectController != nil)
        {
            String str = (PNTCHAR*)[connectController.userNameTextField.text cStringUsingEncoding:NSUTF8StringEncoding];
        
            m_C2SProxy.RequestLogon(HostID_Server, RmiContext::ReliableSend, str);

            // 접속이 완료되었으므로 ServerChatView로 전환
            [m_mainController switchViews:ViewType_ServerChat:0];
        }
    }
}

void CChatClient::OnLeaveServer(ErrorInfo *errorInfo)
{
    NSString *errstr = [[NSString alloc] initWithFormat:@"ErrorType:%d,DetailType:%d",(int)errorInfo->m_errorType,(int)errorInfo->m_detailType];

    // 경고창띄우기
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnJoinServerComplete" message:errstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

    [alert show];
    [errstr release];
    [alert release];
}

void CChatClient::OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField)
{
    NSNumber *host = [[NSNumber alloc] initWithInt:(int)groupHostID];

    // 이미 dictionary에 들어가있다면 그냥 넘어감
    if([m_mainController.groupDictionary objectForKey:host] != nil)
        return;

    NSString *groupName = [[NSString alloc] initWithFormat:@"P2PGroup hostID:%@",host]; 

    [m_mainController.groupDictionary setObject:groupName forKey:host];

    [host release];

    [groupName release];

    if(m_mainController.p2pGroupViewController != nil)
        [m_mainController.p2pGroupViewController.groupTableView reloadData];
}

void CChatClient::OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount)
{
    // p2p채팅창을 찾아서 멤버를 제거한다. 그리고 모든 유저가 나가게되면 해당 p2p채팅뷰를 제거한다.
    NSNumber *grouphost = [[NSNumber alloc] initWithInt:(int)groupHostID];
    P2PChatViewController *controller = [m_mainController.p2pChatViewControllerDictionary objectForKey:grouphost];

    if(controller != nil)
    {
        NSNumber *memberhost = [[NSNumber alloc] initWithInt:(int)memberHostID];
        [controller.userDictionary removeObjectForKey:memberhost];
        [controller.p2puserTableView reloadData];

        if([controller.userDictionary count] == 0)
        {
           // 현재 view가 p2p chat view라면 group view로 이동한다. 
           if(m_mainController.currentview == ViewType_P2PChat)
               [m_mainController switchViews:ViewType_P2PGroup :0];

           // group view 에서 삭제.
           [m_mainController.groupDictionary removeObjectForKey:grouphost];

           if(m_mainController.p2pGroupViewController != nil)
               [m_mainController.p2pGroupViewController.groupTableView reloadData];

           // 해당 p2pchat view controller를 삭제한다.

           [m_mainController.p2pChatViewControllerDictionary removeObjectForKey:grouphost];

           [controller release];

           controller = nil;

        }
        [memberhost release];
    }
    [grouphost release];
}

void CChatClient::OnError(ErrorInfo *errorInfo)
{
    NSString *errstr = [[NSString alloc] initWithFormat:@"ErrorType:%d,DetailType:%d",(int)errorInfo->m_errorType,(int)errorInfo->m_detailType];

    // 경고창띄우기
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnError" message:errstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

    [alert show];

    [errstr release];

    [alert release];
}

void CChatClient::OnWarning(ErrorInfo *errorInfo)
{
    NSString *errstr = [[NSString alloc] initWithFormat:@"ErrorType:%d,DetailType:%d",(int)errorInfo->m_errorType,(int)errorInfo->m_detailType];

    // 경고창띄우기
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnWarning" message:errstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

    [alert show];

    [errstr release];

    [alert release];
}

void CChatClient::OnInformation(ErrorInfo *errorInfo)
{
    NSString *errstr = [[NSString alloc] initWithFormat:@"ErrorType:%d,DetailType:%d",(int)errorInfo->m_errorType,(int)errorInfo->m_detailType];

    // 경고창띄우기
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnInformation" message:errstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

    [alert show];

    [errstr release];

    [alert release];
}

void CChatClient::OnException(const Exception &e)
{
    NSString *exstr = [NSString stringWithCString:e.what() encoding:NSUTF8StringEncoding];

    // 경고창띄우기
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnException" message:exstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

    [alert show];

    [exstr release];

    [alert release];
}

void CChatClient::OnChangeServerUdpState(ErrorType reason)
{
    if(reason != ErrorType_Ok)
    {
        NSString *errstr = [[NSString alloc] initWithFormat:@"ErrorType:%d",(int)reason];

        // 경고창띄우기
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OnChangeServerUdpState" message:errstr delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];

        [alert show];

        [errstr release];

        [alert release];
    }
}

DEFRMI_ChatS2C_ShowChat(CChatClient)
{
    // server controller를 얻기
    ServerChatViewController *ServerChatController = m_mainController.serverChatViewController;

    if(ServerChatController != nil)
    {
        StringA str1 = StringT2A(userName);
        NSString *userstr = [NSString stringWithCString:str1.GetString() encoding:NSUTF8StringEncoding];
        StringA str2 = StringT2A(text);
        NSString *chatstr = [NSString stringWithCString:str2.GetString() encoding:NSUTF8StringEncoding];
        NSString *chatall = [NSString stringWithFormat:@"%@:%@", userstr, chatstr];

        // chat message array에 추가
        [ServerChatController.chatMessageArray addObject: chatall];

        // chat table view 업데이트
        [ServerChatController.chatTableView reloadData];
    }

    return true;
}

DEFRMI_ChatS2C_UserList_Add(CChatClient)
{
    ServerChatViewController *ServerChatController = m_mainController.serverChatViewController;

    if(ServerChatController != nil)
    {
        StringA str = StringT2A(userName);
        NSString *userstr = [NSString stringWithCString:str.GetString() encoding:NSUTF8StringEncoding];
        NSNumber *host = [[NSNumber alloc] initWithInt:(int)hostID];

        // hostiD가 자기자신이면 사용자 목록앞에 *를 붙인다.
        if(hostID == m_client->GetLocalHostID())
        {
            NSMutableString* myname = [NSMutableString stringWithFormat:@"*%@",userstr];  
            [m_mainController.usernameDictionary setObject:myname forKey:host];
        }
        else 
        {
            [m_mainController.usernameDictionary setObject:userstr forKey:host];
        }

        [host release];
        // user table view update

        [ServerChatController.userTableView reloadData];
    }

    return true;
}

DEFRMI_ChatS2C_UserList_Remove(CChatClient)
{
    ServerChatViewController *ServerChatController = m_mainController.serverChatViewController;

    if(ServerChatController != nil)
    {
        NSNumber *host = [[NSNumber alloc] initWithInt:(int)hostID];

        // 해당키를 가진놈을 삭제
        [m_mainController.usernameDictionary removeObjectForKey:host];

        [host release];

        // user table view update
        [ServerChatController.userTableView reloadData];
    }
    return true;
}

DEFRMI_ChatC2C_P2P_Chat(CChatClient)
{
    NSNumber *grouphost = [[NSNumber alloc] initWithInt:(int)p2pGroupID];
    P2PChatViewController *controller = [m_mainController.p2pChatViewControllerDictionary objectForKey:grouphost];

    if(controller != nil)
    {
        NSNumber *userhostid = [[NSNumber alloc] initWithInt:(int)remote];
        NSString *username = [m_mainController.usernameDictionary objectForKey:userhostid];
        StringA str = StringT2A(text);
        NSString *chatstr = [NSString stringWithCString:str.GetString() encoding:NSUTF8StringEncoding];
        NSString *chatall = [NSString stringWithFormat:@"%@:%@", username, chatstr];

        P2PChatMessageInfo *chatinfo = [[P2PChatMessageInfo alloc] init];
        chatinfo.chatmsg = chatall;

        if(remote == m_client->GetLocalHostID())
            chatinfo.relayed = @"self";

        else if(rmiContext.m_relayed == true)
            chatinfo.relayed = @"relayed";
        else
            chatinfo.relayed = @"directp2p";

        

        [controller.chatmsginfoArray addObject:chatinfo];
        [controller.p2pchatTableView reloadData];
        [userhostid release];
        [chatinfo release];
    }

    return true;
}