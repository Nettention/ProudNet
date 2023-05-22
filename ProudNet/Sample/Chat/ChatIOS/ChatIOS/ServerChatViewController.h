//
//  ServerChatViewController.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 18..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

// c2s Chatting을 담당할 View
@interface ServerChatViewController : UIViewController
<UITableViewDataSource, UITableViewDelegate>
{
    IBOutlet UITextField *chatTextField;        // chatting message를 입력할 textfield
    IBOutlet UITableView *chatTableView;        // chatting message를 보여줄 table view
    IBOutlet UITableView *userTableView;        // 접속자 목록을 보여줄 table view
    NSMutableArray *chatMessageArray;           // chat msg를 저장할 array
    
    NSMutableArray *selecteduserArray;          // 접속자 목록에서 체크한 사용자 목록
}

@property (nonatomic, retain) IBOutlet UITextField *chatTextField;
@property (nonatomic, retain) IBOutlet UITableView *chatTableView;
@property (nonatomic, retain) IBOutlet UITableView *userTableView;
@property (nonatomic, retain) NSMutableArray *chatMessageArray;
@property (nonatomic, retain) NSMutableArray *selecteduserArray;

-(IBAction)sendButtonClicked:(id)sender;
-(IBAction)createp2pGroupClicked:(id)sender;
-(IBAction)moveP2PChatViewClicked:(id)sender;
-(IBAction)textFieldDoneEditing:(id)sender;

@end
