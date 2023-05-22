//
//  MainViewController.m
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 22..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "MainViewController.h"
#import "ConnectViewController.h"
#import "ServerChatViewController.h"
#import "P2PGroupViewController.h"
#import "P2PChatViewController.h"

@implementation MainViewController

@synthesize connectViewController;
@synthesize serverChatViewController;
@synthesize p2pGroupViewController;
@synthesize currentview;
@synthesize prevViewController;
@synthesize groupDictionary;
@synthesize p2pChatViewControllerDictionary;
@synthesize usernameDictionary;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)dealloc
{
    prevViewController = nil;
    [connectViewController release];
    [ServerChatViewController release];
    [p2pGroupViewController release];
    
    for(id key in groupDictionary)
    {
        NSString* item = [[groupDictionary objectForKey:key] retain];
        [item release];
        item = nil;
    }
    
    [groupDictionary removeAllObjects];
    [groupDictionary release];
    
    for(id key in p2pChatViewControllerDictionary)
    {
        P2PChatViewController* item = [[p2pChatViewControllerDictionary objectForKey:key] retain];
        [item release];
        item = nil;
    }
    
    [p2pChatViewControllerDictionary removeAllObjects];
    [p2pChatViewControllerDictionary release];
    
    for(id key in usernameDictionary)
    {
        NSString* item = [[usernameDictionary objectForKey:key] retain];
        [item release];
        item = nil;
    }
    
    [usernameDictionary removeAllObjects];
    [usernameDictionary release];
    
    [super dealloc];
}

- (void) didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    // 현재 뷰가 아닌 뷰들은 전부 릴리즈 한다. 어차피 뷰를 바꿀때 없으면 다시 alloc하므로..
    switch(currentview)
    {
    case ViewType_Connect:
        {
            [ServerChatViewController release];
            [p2pGroupViewController release];
            for(id key in p2pChatViewControllerDictionary)
            {
                P2PChatViewController* item = [[p2pChatViewControllerDictionary objectForKey:key] retain];
                [item release];
                item = nil;
            }
            
            [p2pChatViewControllerDictionary removeAllObjects];
            [p2pChatViewControllerDictionary release];
        }
            break;
    case ViewType_ServerChat:
        {
            [connectViewController release];
            [p2pGroupViewController release];
            for(id key in p2pChatViewControllerDictionary)
            {
                P2PChatViewController* item = [[p2pChatViewControllerDictionary objectForKey:key] retain];
                [item release];
                item = nil;
            }
            
            [p2pChatViewControllerDictionary removeAllObjects];
            [p2pChatViewControllerDictionary release];
        }
            break;
    case ViewType_P2PGroup:
        {
            [connectViewController release];
            [ServerChatViewController release];
            for(id key in p2pChatViewControllerDictionary)
            {
                P2PChatViewController* item = [[p2pChatViewControllerDictionary objectForKey:key] retain];
                [item release];
                item = nil;
            }
            
            [p2pChatViewControllerDictionary removeAllObjects];
            [p2pChatViewControllerDictionary release];
        }
            break;
    case ViewType_P2PChat:
        {
            [connectViewController release];
            [ServerChatViewController release];
            [p2pGroupViewController release];
        }
            break;
    }
}

- (void)switchViews:(ViewType)switchview:(NSUInteger)hostID
{
    // 전의 viewController를 부모로부터 삭제
    [prevViewController.view removeFromSuperview];
    
    // connect view는 따로 처리하지 않는다. 갈일이 없기때문.
    switch (switchview) {
        case ViewType_ServerChat:
        {
            if(self.serverChatViewController == nil)
            {
                ServerChatViewController *serverController = [[ServerChatViewController alloc] initWithNibName:@"ServerChatViewController" bundle:nil];
                self.serverChatViewController = serverController;
                [serverController release];
            }
            
            [self.view insertSubview:self.serverChatViewController.view atIndex:0];
            currentview = ViewType_ServerChat;
            prevViewController = self.serverChatViewController;
        }
            break;
        case ViewType_P2PGroup:
        {
            if(self.p2pGroupViewController == nil)
            {
                P2PGroupViewController *p2pController = [[P2PGroupViewController alloc] initWithNibName:@"P2PGroupViewController" bundle:nil];
                self.p2pGroupViewController = p2pController;
                [p2pController release];
            }
            
            [self.view insertSubview:self.p2pGroupViewController.view atIndex:0];
            currentview = ViewType_P2PGroup;
            prevViewController = self.p2pGroupViewController;
        }
            break;
        case ViewType_P2PChat:
        {
            // 해당 grouphostid의 view가 이미 있는지 확인한다.
            P2PChatViewController* p2pchatController; 
            
            NSNumber *host = [[NSNumber alloc] initWithInt:(int)hostID];
            
            // 찾아보고 없다면 새로생성한다.
            p2pchatController = [p2pChatViewControllerDictionary objectForKey:host]; 
            
            if(p2pchatController == nil)
            {
                p2pchatController = [[P2PChatViewController alloc] init];
                
                // group host id 등록
                p2pchatController.grouphostid = [host unsignedIntValue];
                
                // 컨테이너에 추가
                [p2pChatViewControllerDictionary setObject:p2pchatController forKey:host];
            }
            
            [self.view insertSubview:p2pchatController.view atIndex:0];
            
            [host release];
            currentview = ViewType_P2PChat;
            prevViewController = p2pchatController;
        }
        default:
            break;
    }
}

- (void)viewDidLoad
{
    self.connectViewController = [[ConnectViewController alloc] initWithNibName:@"ConnectViewController" bundle:nil];
    [self.view insertSubview:self.connectViewController.view atIndex:0];
    currentview = ViewType_Connect;
    prevViewController = self.connectViewController;
    
    groupDictionary = [[NSMutableDictionary alloc] init];
    p2pChatViewControllerDictionary = [[NSMutableDictionary alloc] init];
    usernameDictionary = [[NSMutableDictionary alloc] init] ;

    [super viewDidLoad];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
