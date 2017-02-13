#include "PauseLayer.h"
#include "GameData.h"
#include "GlobalData.h"
#include "GiftLayer.h"
#include "MenuShop.h"
#include "Sound.h"
#include "GeneralDialog.h"
#include "CountDown.h"
#include "GameLayer.h"
#include "Preload.h"
#include "Common/Macro.h"

USING_NS_TIMELINE;

bool PauseLayer::doInit()
{
	if (!Layer::init())
	{
		return false;
	}

	Node *pRootNode = Preload::getInstance()->getUI("Pause.csb");
	this->addChild(pRootNode);

	//加载动画： 
	ActionTimeline *action = Preload::getInstance()->getUIAction("Pause.csb");;
	pRootNode->runAction(action);
	action->gotoFrameAndPlay(0, false);

	FIND_UI_CONTROL_RE(ui::Button*, "Pause_Continue_Btn", m_pBtnResume, pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Pause_Giveup_Btn", m_pBtnMainMenu, pRootNode);
	//FIND_UI_CONTROL_RE(ui::Button*, "ButtonSoundOn", m_pBtnSoundOn, pRootNode);
	//FIND_UI_CONTROL_RE(ui::Button*, "ButtonSoundOff", m_pBtnSoundOff, pRootNode);
	//FIND_UI_CONTROL_RE(ui::Button*, "ButtonGift", m_pBtnGift, pRootNode);
	//FIND_UI_CONTROL_RE(ui::Button*, "ButtonGiftShield", m_pBtnGiftShield, pRootNode);
	//FIND_UI_CONTROL_RE(ui::Button*, "ButtonGiftKiller", m_pBtnGiftKiller, pRootNode);

	m_pBtnResume->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuContinueCallback, this));
	m_pBtnMainMenu->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuBackToMainMenuCallback, this));
	//m_pBtnGift->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuGiftCompositionCallback, this));
	//m_pBtnSoundOn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuSoundCallback, this));
	//m_pBtnSoundOff->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuSoundCallback, this));
	//m_pBtnGiftShield->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuGiftShieldCallback, this));
	//m_pBtnGiftKiller->addTouchEventListener(CC_CALLBACK_2(PauseLayer::menuKillerCallback, this));

	//if (GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	//{
	//	m_pBtnSoundOn->setVisible(false);
	//	m_pBtnSoundOff->setVisible(true);
	//}
	//else
	//{
	//	m_pBtnSoundOn->setVisible(true);
	//	m_pBtnSoundOff->setVisible(false);
	//}

	Sound::pause();

	return true;
}

void PauseLayer::menuContinueCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	this->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(getPlaneLayer());
		getParent()->addChild(layer);
	});

	Sound::playSound(SOUND::ID_CLICK);

	this->removeFromParent();
}

void PauseLayer::menuBackToMainMenuCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	Sound::playSound(SOUND::ID_CLICK);

	//询问用户是否放弃关卡回到主菜单
	auto pLayer = GeneralDialogCreator::backToMainMenu();
	pLayer->setPlaneLayer(PopupLayer::getPlaneLayer());
	this->getParent()->addChild(pLayer);
	this->removeFromParent();
}

void PauseLayer::menuSoundCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	//关闭/播放声音
	if (GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
		GameData::getInstance()->setValue(GAMEDATA::SOUND, 0);

		m_pBtnSoundOn->setVisible(true);
		m_pBtnSoundOff->setVisible(false);

		//Sound::pause();
	}
	else
	{
		GameData::getInstance()->setValue(GAMEDATA::SOUND, 1);

		m_pBtnSoundOn->setVisible(false);
		m_pBtnSoundOff->setVisible(true);

		//Sound::resume();
	}

	Sound::playSound(SOUND::ID_CLICK);
}

//进入商店
void PauseLayer::menuShopCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	Sound::playSound(SOUND::ID_CLICK);

	auto pLayer = MenuShopLayer::create();
	pLayer->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(getPlaneLayer());
		getParent()->addChild(layer);
	});

	this->getParent()->addChild(pLayer);
	this->removeFromParent();
}

//超值礼包
void PauseLayer::menuGiftCompositionCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	Sound::playSound(SOUND::ID_CLICK);

	//获取超值礼包
	GiftLayer* pLayer = GiftLayer::create(GoodsType::MID_GIFT);
	pLayer->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(getPlaneLayer());
		getParent()->addChild(layer);
	});

	this->getParent()->addChild(pLayer);
	this->removeFromParent();
}

//护盾
void PauseLayer::menuGiftShieldCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	Sound::playSound(SOUND::ID_CLICK);

	//获取子弹晶化
	GiftLayer* pLayer = GiftLayer::create(GoodsType::SHIELD_5);
	pLayer->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(getPlaneLayer());
		getParent()->addChild(layer);
	});

	this->getParent()->addChild(pLayer);
	this->removeFromParent();
}

//必杀
void PauseLayer::menuKillerCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
	{
		return;
	}

	Sound::playSound(SOUND::ID_CLICK);

	//获取必杀
	GiftLayer* pLayer = GiftLayer::create(GoodsType::KILLER_5);
	pLayer->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(getPlaneLayer());
		getParent()->addChild(layer);
	});

	this->getParent()->addChild(pLayer);
	this->removeFromParent();
}
