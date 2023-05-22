//
//  P2PChatViewController.m
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 29..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "P2PChatViewController.h"
#import "AppDelegate.h"
#import "MainViewController.h"
#import <new>
#import "ChatClient.h"

// p2pchatmessageInfo
@implementation P2PChatMessageInfo

@synthesize chatmsg;
@synthesize relayed;

-(void)dealloc
{
    [chatmsg release];
    [relayed release];
    [super dealloc];
}

@end

// p2pchatviewcontroller
@interface P2PChatViewController ()

@end

@implementation P2PChatViewController

@synthesize p2pchatTableView;
@synthesize p2puserTableView;
@synthesize chatTextField;
@synthesize chatmsginfoArray;
@synthesize userDictionary;
@synthesize grouphostid;

// p2p 그룹창으로 돌아가기
-(IBAction)P2PGroudButtonClicked:(id)sender
{
    AppDelegate *delegate = [[UIApplication sharedApplication] delegate];
    [delegate.mainViewController switchViews:ViewType_P2PGroup:0];
}

// p2p 채팅 메시지 보내기
-(IBAction)SendButtonClicked:(id)sender
{
    if(g_client)
    {
        RmiContext context = RmiContext::ReliableSend;
        context.m_compressMode = CM_Zip;
        String str = [chatTextField.text cStringUsingEncoding:NSUTF8StringEncoding];

        g_client->m_C2CProxy.P2P_Chat((HostID)grouphostid, context, (HostID)grouphostid, str);

        // text filed 초기화
        chatTextField.text = nil;
    }
}

-(IBAction)textFieldDoneEditing:(id)sender
{
    [sender resignFirstResponder];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if(tableView == p2pchatTableView)
        return [self.chatmsginfoArray count];
    
    return [self.userDictionary count];
}

-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *tableidentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:tableidentifier];
    if(cell == nil)
    {
        if(tableView == p2puserTableView)
            cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:tableidentifier];
        else
            cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:tableidentifier];
    }
    
    NSUInteger row = [indexPath row];
    
    if(tableView == p2puserTableView)
    {
        // row행에 있는 host를 찾는다.
        NSArray *keyarr = [userDictionary allKeys];
        id key = [keyarr objectAtIndex:row];
        
        cell.textLabel.text = [userDictionary objectForKey:key];
    }
    else
    {
        P2PChatMessageInfo *info = [chatmsginfoArray objectAtIndex:row];
        
        if(info != nil)
        {
            cell.textLabel.text = info.chatmsg;
            cell.detailTextLabel.text = info.relayed;
        }
    }
    
    return cell;
}

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
    [p2pchatTableView release];
    [p2puserTableView release];
    [chatTextField release];
    
    [chatmsginfoArray release];
    [userDictionary release];
    [super dealloc];
}

// user table에 user정보를 채워넣는다.
- (void)setUserTableInfo
{
    HostIDArray members;
    g_client->m_client->GetGroupMembers((HostID)grouphostid, members);
    
    AppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    
    for(int i = 0; i < members.GetCount(); ++i)
    {
        HostID member = members[i];
        NSNumber *hostnumber = [[NSNumber alloc] initWithInt:(int)member];
        NSString* username = [delegate.mainViewController.usernameDictionary objectForKey:hostnumber];
        
        [userDictionary setObject:username forKey:hostnumber];
    }
    
    [p2puserTableView reloadData];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    chatmsginfoArray = [[NSMutableArray alloc] init];
    userDictionary = [[NSMutableDictionary alloc] init];
    
    [self setUserTableInfo];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
