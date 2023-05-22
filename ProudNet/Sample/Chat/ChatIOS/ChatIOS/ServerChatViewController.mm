//
//  ServerChatViewController.m
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 18..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "ServerChatViewController.h"
#import "AppDelegate.h"
#import "MainViewController.h"
#import <new>
#import "ChatClient.h"

@interface ServerChatViewController ()

@end

@implementation ServerChatViewController

@synthesize chatTextField;
@synthesize chatTableView;
@synthesize userTableView;
@synthesize chatMessageArray;
@synthesize selecteduserArray;

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
    [chatTextField release];
    [chatTableView release];
    [userTableView release];
    [chatMessageArray removeAllObjects];
    [chatMessageArray release];
    [selecteduserArray release];
    
    [super dealloc];
}

// send 버튼 누른경우 c2s chat message를 보낸다.
-(IBAction)sendButtonClicked:(id)sender
{
    if(g_client)
    {
        // 암호화및 패킷 압축을 같이해서 보낸다.
        RmiContext context = RmiContext::ReliableSend;
        context.m_encryptMode = EM_Secure;
        context.m_compressMode = CM_Zip;
        
        String str = [chatTextField.text cStringUsingEncoding:NSUTF8StringEncoding];
        
        g_client->m_C2SProxy.Chat(HostID_Server, context, str);
        // text filed 초기화
        chatTextField.text = nil;
    }
}

// p2p그룹 생성 버튼을 누른경우
-(IBAction)createp2pGroupClicked:(id)sender
{
    // 서버에 p2p그룹 생성을 요청한다.
    HostIDArray groupMembers;
    
    for(int i = 0; i < [selecteduserArray count]; ++i)
    {
        NSUInteger hostID = [[selecteduserArray objectAtIndex:i] unsignedIntValue];
        groupMembers.Add((HostID)hostID);
    }
    
    // 자신도 포함시킨다. 
    groupMembers.Add(g_client->m_client->GetLocalHostID());
    
    g_client->m_C2SProxy.RequestP2PGroup(HostID_Server, RmiContext::ReliableSend, groupMembers);
    
    // p2p group view로 이동한다.
    AppDelegate *delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    [delegate.mainViewController switchViews:ViewType_P2PGroup:0];
}

// p2p 그룹 뷰로 이동
-(IBAction)moveP2PChatViewClicked:(id)sender
{
     AppDelegate *delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    [delegate.mainViewController switchViews:ViewType_P2PGroup:0];
}

// table view에 있는 열의 갯수를 설정하기 ( 테이블정보를 가지고있는 배열 혹은 Dictionary의 갯수를 리턴 )
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if(tableView == chatTableView)
    {
       return [self.chatMessageArray count];
    }
    
    AppDelegate *delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    return [delegate.mainViewController.usernameDictionary count];
}

// 테이블 뷰에 각각의 열을 추가하기 ( 주의! 위 함수에있는 열의갯수가 1이상이어야 호출됨 )
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *tableidentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:tableidentifier];
    if(cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:tableidentifier];
    }
        
    NSUInteger row = [indexPath row];
        
    if(tableView == chatTableView)
        cell.textLabel.text = [chatMessageArray objectAtIndex:row];
    else if(tableView == userTableView)
    {
        // 이미 체크된 cell일수도 있으니 먼저 원래대로 초기화한다.
        cell.accessoryType = UITableViewCellAccessoryNone;
        
        // row행에 있는 host를 찾는다.
        AppDelegate *delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
        NSArray *keyarr = [delegate.mainViewController.usernameDictionary allKeys];
        NSUInteger hostID = [[keyarr objectAtIndex:row] unsignedIntValue];
        
        // 현재의 row가 선택된 row인지 확인하여 update한다.
        for(int i = 0; i < [selecteduserArray count]; ++i)
        {
            if(hostID == [[selecteduserArray objectAtIndex:i] unsignedIntValue])
            {
                cell.accessoryType = UITableViewCellAccessoryCheckmark;
            }
        }
        
        // row행의 순서에있는 key를 cell에 등록한다.
        id key = [keyarr objectAtIndex:row];
        cell.textLabel.text = [delegate.mainViewController.usernameDictionary objectForKey:key];
    }
        
    return cell;
}

// 테이블뷰의 cell을 선택했을경우
-(void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // chat view는 선택해도 아무것도 하지 않는다.
    if(tableView == userTableView)
    {
        AppDelegate *delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
        
        // 선택한 row의 hostid를 배열에 저장한다.
        NSUInteger row = [indexPath row];
        NSArray *keyarr = [delegate.mainViewController.usernameDictionary allKeys];
        NSUInteger hostID = [[keyarr objectAtIndex:row] unsignedIntValue];
        NSNumber *hostNumber = [[NSNumber alloc] initWithUnsignedInt:hostID];
        
        // 선택된 row가 이미 배열안에 있는지 확인하여 있으면 목록에서 제거한다.
        for(int i = 0; i < [selecteduserArray count]; ++i)
        {
            if(hostNumber == [selecteduserArray objectAtIndex:i])
            {
                [selecteduserArray removeObjectAtIndex:i];
                [userTableView reloadData];
                return;
            }
        }
        
        [selecteduserArray addObject:hostNumber];
        [hostNumber release];
        
        // table을 다시 업데이트
        [userTableView reloadData];
    }
}


-(IBAction)textFieldDoneEditing:(id)sender
{
    [sender resignFirstResponder];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    chatMessageArray = [[NSMutableArray alloc] init];
    selecteduserArray = [[NSMutableArray alloc] init];
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
