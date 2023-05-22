//
//  ChatClient.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 16..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Var.h"
#include "ChatC2C_proxy.h"
#include "ChatC2C_stub.h"
#include "ChatC2S_proxy.h"
#include "ChatS2C_stub.h"

@class MainViewController;

// proudnet client을 가지고있는 client객체
class CChatClient: public INetClientEvent, public ChatS2C::Stub, public ChatC2C::Stub 
{
public:
    CChatClient(MainViewController *maincontroller);
    ~CChatClient();
public:
    
    CNetClient *m_client;
    MainViewController *m_mainController;
    
    bool Connect(String serverAdress);
    
	virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer) PN_OVERRIDE;
    
	virtual void OnLeaveServer(ErrorInfo *errorInfo) PN_OVERRIDE;
    
	virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField)PN_OVERRIDE;
	
	virtual void OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount) PN_OVERRIDE;
    
	virtual void OnChangeServerUdpState(ErrorType reason) PN_OVERRIDE;
    
	virtual void OnChangeP2PRelayState(HostID remoteHostID, ErrorType reason) PN_OVERRIDE {};
	virtual void OnSynchronizeServerTime() PN_OVERRIDE {};
    
	virtual void OnError(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnWarning(ErrorInfo *errorInfo) PN_OVERRIDE;
    
	virtual void OnInformation(ErrorInfo *errorInfo) PN_OVERRIDE;
    
	virtual void OnException(const Exception &e) PN_OVERRIDE;
    
	virtual void OnNoRmiProcessed(Proud::RmiID rmiID) PN_OVERRIDE {}

public:
    DECRMI_ChatS2C_ShowChat;
    DECRMI_ChatS2C_UserList_Add;
    DECRMI_ChatS2C_UserList_Remove;
    
    DECRMI_ChatC2C_P2P_Chat;
    
public:
    ChatC2S::Proxy m_C2SProxy;
    ChatC2C::Proxy m_C2CProxy;
};

extern CChatClient *g_client;



