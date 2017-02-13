#include "AppDelegate.h"
#include "LoadingLayer.h"
#include "GameLayer.h"
#include "HubLayer.h"
#include "PlaneLayer.h"
#include "Channel.h"
#include "Carrier.h"
#include "Common/Macro.h"
#include "Bmob/BmobSdk.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    Utils::initLog();

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::createWithRect("Raiden",
			Rect(0, 0, CONSTANT::DESIGN_RES_WIDTH * CONSTANT::RES_RATIO, CONSTANT::DESIGN_RES_HEIGHT * CONSTANT::RES_RATIO));
        director->setOpenGLView(glview);
    }

	auto frameSize = glview->getFrameSize();
	if (frameSize.width >= frameSize.height)
	{
		director->getOpenGLView()->setDesignResolutionSize(CONSTANT::DESIGN_RES_WIDTH, CONSTANT::DESIGN_RES_HEIGHT, ResolutionPolicy::SHOW_ALL);
	}
	else
	{
		director->getOpenGLView()->setDesignResolutionSize(CONSTANT::DESIGN_RES_WIDTH, CONSTANT::DESIGN_RES_HEIGHT, ResolutionPolicy::EXACT_FIT);
	}

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    auto scene = LoadingLayer::scene();

    // run
    director->runWithScene(scene);

	BmobSDKInit::initialize("app_id", "app_key");

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	//为保证接入百度sdk时不重复弹出广告，这里暂时禁用自动暂停功能
	if (Channel::getMainChannel() == CHANNEL::BAIDU)
	{
		return;
	}

    if (!GameLayer::getPlaneLayer() || !GameLayer::getHubLayer())
    {
        return;
    }

    PlaneLayer* pPlaneLayer = dynamic_cast<PlaneLayer*>(GameLayer::getPlaneLayer());
    HubLayer* pHubLayer = dynamic_cast<HubLayer*>(GameLayer::getHubLayer());
    if (pHubLayer && pPlaneLayer && !pPlaneLayer->isPause())
    {
        pHubLayer->menuPauseCallback(nullptr);
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

}
