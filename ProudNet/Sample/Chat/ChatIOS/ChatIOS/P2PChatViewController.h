//
//  P2PChatViewController.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 29..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface P2PChatMessageInfo : NSObject
{
    NSString *chatmsg;  // 1줄의 채팅 메시지
    NSString *relayed;  // relay여부
}

@property (nonatomic, retain) NSString *chatmsg;
@property (nonatomic, retain) NSString *relayed;

@end

@interface P2PChatViewController : UIViewController
<UITableViewDataSource, UITableViewDelegate>
{
    IBOutlet UITableView *p2pchatTableView;
    IBOutlet UITableView *p2puserTableView;
    IBOutlet UITextField *chatTextField;        // chatting message를 입력할 textfield
    
    NSMutableArray *chatmsginfoArray;       // P2PChatMessageInfo array
    NSMutableDictionary *userDictionary;    // 유저 목록
    NSUInteger grouphostid;                 // 현재 p2p그룹의 hostid
}

@property (nonatomic, retain) IBOutlet UITableView *p2pchatTableView;
@property (nonatomic, retain) IBOutlet UITableView *p2puserTableView;
@property (nonatomic, retain) IBOutlet UITextField *chatTextField;
@property (nonatomic, retain) NSMutableArray *chatmsginfoArray;
@property (nonatomic, retain) NSMutableDictionary * userDictionary;
@property NSUInteger grouphostid;

-(IBAction)P2PGroudButtonClicked:(id)sender;
-(IBAction)SendButtonClicked:(id)sender;
-(IBAction)textFieldDoneEditing:(id)sender;

@end
