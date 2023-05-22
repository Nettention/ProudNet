//
//  ConnectViewController.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 16..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

// Server에 연결할 connectviewcontroller
@interface ConnectViewController : UIViewController
{
    IBOutlet UITextField *ipTextField;          // server address를 입력할 text field
    IBOutlet UITextField *userNameTextField;    // user name을 입력할 text field
    
}

@property (nonatomic, retain) IBOutlet UITextField *ipTextField;
@property (nonatomic, retain) IBOutlet UITextField *userNameTextField;

-(IBAction)ConnectButtonClicked:(id)sender;
-(IBAction)textFieldDoneEditing:(id)sender;

@end
