//
//  P2PGroupViewController.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 24..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface P2PGroupViewController : UIViewController
<UITableViewDataSource, UITableViewDelegate>
{
    IBOutlet UITableView *groupTableView;    
}

@property (nonatomic, retain) IBOutlet UITableView *groupTableView;

-(IBAction)moveServerChatViewClicked:(id)sender;

@end
