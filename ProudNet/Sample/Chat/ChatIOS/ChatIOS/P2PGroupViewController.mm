//
//  P2PGroupViewController.m
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 24..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "P2PGroupViewController.h"
#import "AppDelegate.h"
#import "MainViewController.h"
#import "P2PChatViewController.h"

@interface P2PGroupViewController ()

@end

@implementation P2PGroupViewController

@synthesize groupTableView;

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
    [groupTableView release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
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

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    AppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    return [delegate.mainViewController.groupDictionary count];
}

-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *tableidentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:tableidentifier];
    if(cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:tableidentifier];
    }
    
    NSUInteger row = [indexPath row];
    
    // row행에 있는 host를 찾는다.
    AppDelegate *delegate = [[UIApplication sharedApplication] delegate];
    NSArray *keyarr = [delegate.mainViewController.groupDictionary allKeys];
    
    id key = [keyarr objectAtIndex:row];
    cell.textLabel.text = [delegate.mainViewController.groupDictionary objectForKey:key];
    
    return cell;
}

-(void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // p2p chat view로 이동
    NSUInteger row = [indexPath row];
    AppDelegate *delegate = [[UIApplication sharedApplication] delegate];
    NSArray *keyarr = [delegate.mainViewController.groupDictionary allKeys];
    NSUInteger grouphostid = [[keyarr objectAtIndex:row] unsignedIntValue];
    
    [delegate.mainViewController switchViews:ViewType_P2PChat:grouphostid];
}

// p2p chat을 위한 view를 만든다.
-(void) loadP2PChatViewController
{
    
}

// serverchatView로 이동
-(IBAction)moveServerChatViewClicked:(id)sender
{
    AppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate.mainViewController switchViews:ViewType_ServerChat:0];
}

@end
