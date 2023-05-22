//
//  AppDelegate.h
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 16..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    NSTimer *frameMoveTimer;    // 일정시간마다 client의 framemove를 호출해줄 timer
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) MainViewController *mainViewController;
@property (nonatomic, retain) NSTimer *frameMoveTimer;

@end
