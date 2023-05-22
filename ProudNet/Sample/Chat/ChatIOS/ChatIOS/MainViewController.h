//
//  MainViewController.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 22..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ConnectViewController;
@class ServerChatViewController;
@class P2PGroupViewController;

typedef enum 
{
    ViewType_Connect = 0,
    ViewType_ServerChat,
    ViewType_P2PGroup,
    ViewType_P2PChat,
    
}ViewType;

// 모든 controller를 관리하는 maincontroller
@interface MainViewController : UIViewController
{
    ConnectViewController *connectViewController;
    ServerChatViewController *serverChatViewController;
    P2PGroupViewController *p2pGroupViewController;
    ViewType currentview;
    UIViewController *prevViewController; // 이동하기 전의 viewController
    
    // view가 지워져도 계속 가지고 있어야하는 data의 경우 mainviewcontroller에서 직접 관리한다.
    NSMutableDictionary *groupDictionary;    // 방목록을 가지고있음. key : group host id, value : p2p room name
    NSMutableDictionary *p2pChatViewControllerDictionary;   // key : grouphostid, value : p2pChatViewController
    NSMutableDictionary *usernameDictionary;    // key : hostid, value : username
}

@property (nonatomic, retain) ConnectViewController *connectViewController;
@property (nonatomic, retain) ServerChatViewController *serverChatViewController;
@property (nonatomic, retain) P2PGroupViewController *p2pGroupViewController;
@property (nonatomic, assign) ViewType currentview;
@property (nonatomic, retain) UIViewController *prevViewController;
@property (nonatomic, retain) NSMutableDictionary *groupDictionary;
@property (nonatomic, retain) NSMutableDictionary *p2pChatViewControllerDictionary;
@property (nonatomic, retain) NSMutableDictionary *usernameDictionary;

- (void)switchViews:(ViewType)switchview:(NSUInteger)hostID;

@end
