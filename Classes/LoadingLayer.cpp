#include "Common/Macro.h"
#include "LoadingLayer.h"
#include "GameScene.h"
#include "MenuDayGift.h"
#include "GlobalData.h"
#include "Channel.h"
#include "MenuMainUI.h"
#include "StartLayer.h"


namespace LoadingLayerConstant
{
	const float NeedleMaxDegree = 68;
	const float NeedleMinDegree = -68;

	const char* const StringErrorApk = "error_apk";
	const char* const StringTipsDaily = "tips_daily";
}

Scene* LoadingLayer::scene()
{
	Scene *scene = Scene::create();
	LoadingLayer *layer = LoadingLayer::create();
	scene->addChild(layer, 1);

	return scene;
}

LoadingLayer::LoadingLayer(void)
: m_pRootNode(nullptr)
, m_pNeedle(nullptr)
, m_pLoadingBar(nullptr)
, m_pTxtErrorInfo(nullptr)
, m_iBeginTime(0)
, m_iEndTime(0)
, m_iStart(0)
{
}

LoadingLayer::~LoadingLayer(void)
{
}

void LoadingLayer::onStart()
{
}

void LoadingLayer::onProgress(int percent)
{
	float degree = LoadingLayerConstant::NeedleMinDegree +
		(LoadingLayerConstant::NeedleMaxDegree - LoadingLayerConstant::NeedleMinDegree) * percent / 100;
	m_pNeedle->setRotation(degree);
}

void LoadingLayer::onError(const char* info)
{
	m_pTxtErrorInfo->setVisible(true);
	m_pTxtErrorInfo->setString(info);
}

void LoadingLayer::onWarning(const char* info)
{
	m_pTxtErrorInfo->setVisible(true);
	m_pTxtErrorInfo->setString(info);
}

void LoadingLayer::onEnd(PreloadError errorCode)
{
	m_pNeedle->setRotation(LoadingLayerConstant::NeedleMaxDegree);

	struct timeval tv;
	gettimeofday(&tv, NULL);
	m_iEndTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	switch (errorCode)
	{
	case PreloadError::Success:
		Director::getInstance()->replaceScene(StartLayer::scene());
		break;
	case PreloadError::RemoteDataError:
		//网络连接失败，您可能无法领取礼品，是否强行进入游戏？确定、重试。 正在联机读取游戏数据......
		//小提示：联网状态才能领取每日登陆礼包
		Director::getInstance()->replaceScene(StartLayer::scene());
		break;
	case PreloadError::CommonError:
		//游戏包体被破坏，无法进入游戏，请重新安装
		m_pTxtErrorInfo->setVisible(true);
		m_pTxtErrorInfo->setString(GlobalData::getInstance()->getParameter(LoadingLayerConstant::StringErrorApk));
		break;
	default:
		break;
	}

	return;
}

bool LoadingLayer::init()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	m_iBeginTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	initUI();

	scheduleUpdate();

	return true;
}

void LoadingLayer::update(float dt)
{
	if (m_iStart < 10)
	{
		++m_iStart;
		return;
	}

	Preload::getInstance()->start(this);

	this->unscheduleUpdate();
}

void LoadingLayer::initUI()
{
	Size winSize = CCDirector::getInstance()->getWinSize();

	m_pRootNode = Preload::getInstance()->getUI("Loading.csb");
	this->addChild(m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Loading_Needle", m_pNeedle, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "Loading_Prog", m_pLoadingBar, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Text*, "Error_Info_Text", m_pTxtErrorInfo, m_pRootNode);

	m_pNeedle->setRotation(LoadingLayerConstant::NeedleMinDegree);
	m_pTxtErrorInfo->setVisible(false);
}
