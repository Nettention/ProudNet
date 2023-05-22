#include "stdafx.h"
#include "AppDelegate.h"
#include "ConnectLayer.h"
#include "World.h"

USING_NS_CC;


const CCSize ScreenSize = CCSize(480, 320);

AppDelegate::AppDelegate() {
	g_World = new CWorld;
}

AppDelegate::~AppDelegate() 
{
	delete g_World;
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

	// Set the design resolution size
	pEGLView->setDesignResolutionSize(ScreenSize.width, ScreenSize.height, kResolutionNoBorder);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = CCScene::create();

	// 'layer' is an autorelease object
	ConnectLayer* layer = ConnectLayer::create();

	// add layer as a child to scene
	pScene->addChild(layer);

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
