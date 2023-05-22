//
//  ConnectViewController.m
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 16..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "ConnectViewController.h"

// ProudNet 을 쓰는 mm혹은 cpp 파일은 꼭 <new>를 포함해야합니다.
#import <new>
#import "ChatClient.h"

@interface ConnectViewController ()

@end

@implementation ConnectViewController

@synthesize ipTextField;
@synthesize userNameTextField;

- (void)dealloc
{
    [ipTextField release];
    [userNameTextField release];
    [super dealloc];
}


// connect 버튼을 클릭했을때의 action
- (IBAction)ConnectButtonClicked:(id)sender
{
    // NSString -> char*
    String str = [ipTextField.text cStringUsingEncoding:NSUTF8StringEncoding];
    
    if(g_client)
        g_client->Connect(str);
}

- (IBAction)textFieldDoneEditing:(id)sender
{
    [sender resignFirstResponder];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
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

@end
