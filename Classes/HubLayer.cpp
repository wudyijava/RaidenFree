#include "Common/Macro.h"
#include "Common/HpBar.h"
#include "HubLayer.h"
#include "PauseLayer.h"
#include "GiftLayer.h"
#include "GameLayer.h"
#include "PlaneLayer.h"
#include "GameData.h"
#include "MenuShop.h"
#include "CountDown.h"
#include "Goods.h"
#include "Common/Macro.h"
#include "Channel.h"
#include "Sound.h"
#include "Preload.h"
#include "ModeBase.h"
#include "Guide.h"
#include "GiftDuoLayer.h"
#include "Difficulty.h"

namespace HubLayerConstant
{
	std::string btn_killer = "GameUI_Wingman_Btn_Up.png";
	std::string btn_shield = "GameUI_Shield_Btn_Up.png";
	std::string btn_nuclear = "GameUI_Nuclear_Btn_Up.png";
	
	const float skillCdTime = 20.0f;
}

HubLayer::HubLayer()
: m_pPlaneLayer(nullptr)
, m_pHubRootNode(nullptr)
, m_pBtnPause(nullptr)
, m_pBtnShop(nullptr)
, m_pBtnKiller(nullptr)
, m_pBtnShield(nullptr)
, m_pPlayerHpBar(nullptr)
, m_pTextScore(nullptr)
, m_pTextMoney(nullptr)
, m_pTextLife(nullptr)
, m_pTextKiller(nullptr)
, m_pTextShield(nullptr)
, m_iMaxHp(100)
, m_pTextComboNum(nullptr)
, m_pSpriteCombo(nullptr)
, m_pTextKmNum(nullptr)
, m_pSpriteKm(nullptr)
, m_pTextMoneyPresent(nullptr)
, m_pSpriteMoneyPresent(nullptr)
, m_pBossHpBar(nullptr)
, m_pSpriteNewGift(nullptr)
, m_pBtnNewGift(nullptr)
, m_pHugGiftNode(nullptr)
, m_pBtnHugGift(nullptr)
{
}

HubLayer::~HubLayer()
{
	GameData::getInstance()->unRegListener(this, GAMEDATA::LEVEL_SCORE);
	GameData::getInstance()->unRegListener(this, GAMEDATA::LEVEL_MONEY);
	GameData::getInstance()->unRegListener(this, GAMEDATA::LEVEL_LIFE);
	GameData::getInstance()->unRegListener(this, GAMEDATA::KILLER);
	GameData::getInstance()->unRegListener(this, GAMEDATA::SHIELD);
}

HubLayer* HubLayer::create(PlaneLayer* layer)
{
	HubLayer *pRet = new(std::nothrow) HubLayer();
	if (pRet && pRet->init(layer))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}

}

bool HubLayer::init(PlaneLayer* layer)
{
	CCASSERT(layer, "HubLayer::init :  layer is NULL");

	if (!Layer::init())
	{
		return false;
	}

	m_pHubRootNode = Preload::getInstance()->getInstance()->getUI("GameUI.csb");
	addChild(m_pHubRootNode);

	auto pAction = CSLoader::createTimeline("GameUI.csb");
	m_pHubRootNode->runAction(pAction);
	pAction->gotoFrameAndPlay(0, true);

	//控件
	FIND_UI_CONTROL_RE(ui::Button*, "GameUI_Pause", m_pBtnPause, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "GameUI_Shop", m_pBtnShop, m_pHubRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "GameUI_Killer", m_pBtnKiller, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "GameUI_Shield", m_pBtnShield, m_pHubRootNode);

	FIND_UI_CONTROL_RE(ui::LoadingBar*, "Hp_Prog", m_pPlayerHpBar, m_pHubRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "GameUI_Boss_Blood_Bg", m_pBossHpBg, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "GameUI_Boss_Blood_Prog_3", m_pBossHp3, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "GameUI_Boss_Blood_Prog_2", m_pBossHp2, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "GameUI_Boss_Blood_Prog_1", m_pBossHp1, m_pHubRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Num1", m_pTextScore, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Num2", m_pTextMoney, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Num6", m_pTextLife, m_pHubRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Num5", m_pTextKiller, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Num3", m_pTextShield, m_pHubRootNode);
		
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Combo_Num", m_pTextComboNum, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Km_Num", m_pTextKmNum, m_pHubRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "GameUI_Combo_Txt", m_pSpriteCombo, m_pHubRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "GameUI_Km_Txt", m_pSpriteKm, m_pHubRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "GameUI_Num_Present", m_pTextMoneyPresent, m_pHubRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "GameUI_Diamond_Img_0", m_pSpriteMoneyPresent, m_pHubRootNode);
	

	FIND_UI_CONTROL_RE(Sprite*, "GameUI_New_Gift", m_pSpriteNewGift, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "GameUI_New_Gift_Light", m_pBtnNewGift, m_pHubRootNode);

	FIND_UI_CONTROL_RE(Node*, "Node_Hug_Gift", m_pHugGiftNode, m_pHubRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "GameUI_Gift2_Bg", m_pBtnHugGift, m_pHubRootNode);

	//控件属性
	m_pPlayerHpBar->setPercent(100);
	m_pPlayerHpBar->setDirection(ui::LoadingBar::Direction::LEFT);

	m_pSpriteCombo->setVisible(false);
	m_pSpriteKm->setVisible(false);
	m_pSpriteMoneyPresent->setVisible(false);
	m_pSpriteMoneyPresentPos = m_pSpriteMoneyPresent->getPosition();

	initGiftAni();
	displayNewGift();
	displayHugGift(false);

	//boss血槽
	initBossHp();
	hideBossHp();

	m_pTextScore->setString("0");
	m_pTextMoney->setString("0");
	m_pTextLife->setString("0");
	m_pTextKiller->setString("0");
	m_pTextShield->setString("0");

	//控件事件
	addButtonTouchEffect(m_pBtnPause, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		this->menuPauseCallback(pSender);
	});

	addButtonTouchEffect(m_pBtnShop, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		this->menuShopCallback(pSender);
	});

	addButtonTouchEffect(m_pBtnKiller, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		this->menuKillerCallback(pSender);
	});

	addButtonTouchEffect(m_pBtnShield, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		this->menuShieldCallback(pSender);
	});

	addButtonTouchEffect(m_pBtnNewGift, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		this->menuBuyNewGift(pSender);
	});

	addButtonTouchEffect(m_pBtnHugGift, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		this->menuBuyHugGift(pSender);
	});

	//注册监听器
	GameData::getInstance()->regListener(this, GAMEDATA::LEVEL_SCORE);
	GameData::getInstance()->regListener(this, GAMEDATA::LEVEL_MONEY);
	GameData::getInstance()->regListener(this, GAMEDATA::LEVEL_LIFE);
	GameData::getInstance()->regListener(this, GAMEDATA::KILLER);
	GameData::getInstance()->regListener(this, GAMEDATA::SHIELD);

	updateShield(GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD));
	updateKiller(GameData::getInstance()->getValueToInt(GAMEDATA::KILLER));
	updateLife(GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_LIFE));

	//setHubLayer必须要放在初始化的最后
	m_pPlaneLayer = layer;
	m_pPlaneLayer->setHubLayer(this);

	//不同的模式下，设置是否显示生命数
	Sprite* pLifeImg = nullptr;
	FIND_UI_CONTROL_RE(Sprite*, "GameUI_Life_Img", pLifeImg, m_pHubRootNode);
	
	int mode = GameData::getInstance()->getValueToInt(GAMEDATA::MODE);
	switch (mode)
	{
	case ModeBase::ModeEndless:
	case ModeBase::ModeRapid:
		pLifeImg->setVisible(false);
		break;
	default:
		pLifeImg->setVisible(true);
		break;
	}

	return true;
}

void HubLayer::displayGuideShield()
{
	Guide* pGuide = GuideMgr::getInstance()->show(GuideType::GameShield, this->getParent(), m_pBtnShield);
	if (pGuide)
	{
		m_pPlaneLayer->pause();
	}
}

void HubLayer::displayGuideKiller()
{
	Guide* pGuide = GuideMgr::getInstance()->show(GuideType::GameKiller, this->getParent(), m_pBtnKiller);
	if (pGuide)
	{
		m_pPlaneLayer->pause();
	}
}

void HubLayer::menuKillerCallback(Ref* pSender)
{
	DEBUG_LOG("HubLayer::menuKillerCallback");

	if (GuideMgr::getInstance()->destory(GuideType::GameKiller))
	{
		m_pPlaneLayer->resume();
	}

	int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::KILLER);
	if (cnt > 0)
	{
		float cdTimeAdjust = GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_KILLER_COLD);
		this->makeSkillCD(HubLayerConstant::btn_killer, m_pBtnKiller, HubLayerConstant::skillCdTime - cdTimeAdjust);
		//this->makeSkillCD(HubLayerConstant::btn_killer, m_pBtnWingman, HubLayerConstant::skillCdTime);

		cnt = GameData::getInstance()->incValue(GAMEDATA::KILLER, -1);
		//m_pPlaneLayer->spwanKiller();
		m_pPlaneLayer->spwanSuperKiller();
		this->updateKiller(cnt);

		if (cnt <= 0)
		{
			FadeTo* pFade = FadeTo::create(0.2f, 100);
			FadeTo* pFade2 = FadeTo::create(0.2f, 255);
			Sequence* seq = Sequence::create(pFade, pFade2, NULL);
			Repeat* pRepeat = Repeat::create(seq, 4);
			m_pBtnKiller->runAction(pRepeat);
		}

		GameData::getInstance()->setValue(GAMEDATA::USE_KILLER, 1);
	}
	else
	{
		PlaneLayer::pushCommand(ECommand::CommandKiller);

		m_pPlaneLayer->pause();

		auto pLayer = GiftLayer::create(GoodsType::KILLER_5);
		pLayer->setOnExitCallback([this]()
		{
			//返回时倒计时
			auto layer = CountDownLayer::create();
			layer->setPlaneLayer(m_pPlaneLayer);
			this->getParent()->addChild(layer);
		});

		this->getParent()->addChild(pLayer);
	}
}

void HubLayer::menuShieldCallback(Ref* pSender)
{
	DEBUG_LOG("HubLayer::menuShieldCallback");

	if (GuideMgr::getInstance()->destory(GuideType::GameShield))
	{
		m_pPlaneLayer->resume();
	}

	int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD);
	if (cnt > 0)
	{
		--cnt;
		GameData::getInstance()->setValue(GAMEDATA::SHIELD, cnt);
		m_pPlaneLayer->bulletShield();
		this->updateShield(cnt);

		if (cnt <= 0)
		{
			FadeTo* pFade = FadeTo::create(0.2f, 100);
			FadeTo* pFade2 = FadeTo::create(0.2f, 255);
			Sequence* seq = Sequence::create(pFade, pFade2, NULL);
			Repeat* pRepeat = Repeat::create(seq, 4);
			m_pBtnShield->runAction(pRepeat);
		}

		GameData::getInstance()->setValue(GAMEDATA::USE_SHIELD, 1);
	}
	else
	{
		PlaneLayer::pushCommand(ECommand::CommandShield);

		m_pPlaneLayer->pause();

		auto pLayer = GiftLayer::create(GoodsType::SHIELD_5);
		pLayer->setOnExitCallback([this]()
		{
			//返回时倒计时
			auto layer = CountDownLayer::create();
			layer->setPlaneLayer(m_pPlaneLayer);
			this->getParent()->addChild(layer);
		});

		this->getParent()->addChild(pLayer);
	}
}

void HubLayer::menuPauseCallback(Ref *pSender)
{
	//某些渠道需要在游戏暂停时做一些恶心的手脚
	Channel::pause();

	m_pPlaneLayer->pause();

	auto layer = PauseLayer::create();
	layer->setPlaneLayer(m_pPlaneLayer);
	this->getParent()->addChild(layer);
}

void HubLayer::menuShopCallback(Ref* pSender)
{
	Sound::pause();
	m_pPlaneLayer->pause();

	auto pLayer = MenuShopLayer::create();
	pLayer->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(m_pPlaneLayer);
		this->getParent()->addChild(layer);
	});

	this->getParent()->addChild(pLayer);
}

void HubLayer::menuBuyNewGift(Ref* pSender)
{
	DEBUG_LOG("HubLayer::menuBuyNewGift");

	m_pPlaneLayer->pause();

	auto pLayer = GiftLayer::create(GoodsType::NEW_GIFT);
	pLayer->setOnExitCallback([this]()
	{
		this->displayNewGift();

		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(m_pPlaneLayer);
		this->getParent()->addChild(layer);
	});

	this->getParent()->addChild(pLayer);
}

void HubLayer::displayNewGift()
{
	m_pSpriteNewGift->setVisible(false);
	return;

	if (Channel::getMainChannel() == CHANNEL::QIHU)
	{
		m_pSpriteNewGift->setVisible(false);
		return;
	}

	if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) < 1)
	{
		m_pSpriteNewGift->setVisible(false);
		return;
	}

	if (GameData::getInstance()->getValueToInt(GAMEDATA::NEW_GIFT))
	{
		m_pSpriteNewGift->setVisible(false);
		return;
	}

	m_pSpriteNewGift->setVisible(true);
}

void HubLayer::menuBuyHugGift(Ref* pSender)
{
	m_pPlaneLayer->pause();

	std::vector<const char*> goodsArray;
	goodsArray.push_back(GoodsType::HUG_GIFT);
	goodsArray.push_back(GoodsType::BIG_GIFT);
	auto pLayer = GiftDuoLayer::create(goodsArray);
	pLayer->setOnExitCallback([this]()
	{
		//返回时倒计时
		auto layer = CountDownLayer::create();
		layer->setPlaneLayer(m_pPlaneLayer);
		this->getParent()->addChild(layer);
	});

	addChild(pLayer);
}

void HubLayer::displayHugGift(bool bAnimation)
{
	m_pHugGiftNode->setVisible(false);
	return;

	if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) < 1)
	{
		m_pHugGiftNode->setVisible(false);
	}
	else
	{
		m_pHugGiftNode->setVisible(true);
	}

	if (m_pSpriteCombo->isVisible() || m_pSpriteKm->isVisible())
	{
		m_pHugGiftNode->setPositionY(m_pSpriteCombo->getPositionY()
			- m_pSpriteCombo->getContentSize().height / 2
			- m_pBtnHugGift->getContentSize().height / 2
			);
	}
	else
	{
		if (bAnimation)
		{
			MoveTo* pMove = MoveTo::create(0.15f, Vec2(m_pHugGiftNode->getPositionX(), m_pSpriteCombo->getPositionY()));
			m_pHugGiftNode->runAction(pMove);
		}
		else
		{
			m_pHugGiftNode->setPositionY(m_pSpriteCombo->getPositionY());
		}
	}
}

void HubLayer::initGiftAni()
{
	if (!m_pBtnNewGift)
	{
		return;
	}

	FadeTo* pFade1 = FadeTo::create(0.2f, 20);
	FadeTo* pFade2 = FadeTo::create(0.2f, 255);
	Sequence* pSeq = Sequence::create(pFade1, pFade2, nullptr);
	RepeatForever* pRepeat = RepeatForever::create(pSeq);
	m_pBtnNewGift->runAction(pRepeat);
}

void HubLayer::displayHp()
{

}

void HubLayer::hideHp()
{

}

void HubLayer::updateMaxHp(int hp)
{
	m_iMaxHp = hp;
	m_pPlayerHpBar->setPercent(100);
}

void HubLayer::updateHp(int hp)
{
	if (m_pPlayerHpBar)
	{
		m_pPlayerHpBar->setPercent(hp * 100.0f / m_iMaxHp);
	}
}

void HubLayer::updateValue(const std::string& type, int value)
{
	if (type == GAMEDATA::LEVEL_SCORE)
	{
		updateScore(value);
	}
	else if (type == GAMEDATA::LEVEL_MONEY)
	{
		updateMoney(value);
	}
	else if (type == GAMEDATA::LEVEL_LIFE)
	{
		updateLife(value);
	}
	else if (type == GAMEDATA::KILLER)
	{
		updateKiller(value);
	}
	else if (type == GAMEDATA::SHIELD)
	{
		updateShield(value);
	}

}

void HubLayer::updateLife(int life)
{
	if (m_pTextLife != NULL)
	{
		char sz[16] = { 0 };
		snprintf(sz, 15, "%d", life);
		std::string str(sz);
		m_pTextLife->setString(str);
	}
}

void HubLayer::updateScore(int score)
{
	if (m_pTextScore != NULL)
	{
		char sz[16] = {0};
		snprintf(sz, 15, "%d", score);
		std::string str(sz);
		m_pTextScore->setString(str);
	}
}

void HubLayer::updateMoney(int money)
{
	if (m_pTextMoney != NULL)
	{
		char sz[16] = { 0 };
		snprintf(sz, 15, "%d", money);
		std::string str(sz);
		m_pTextMoney->setString(str);
	}
}

void HubLayer::updateKiller(int cnt)
{
	if (m_pTextKiller != NULL)
	{
		char sz[16] = { 0 };
		snprintf(sz, 15, "%d", cnt);
		std::string str(sz);
		m_pTextKiller->setString(str);
	}
}

void HubLayer::updateShield(int cnt)
{
	if (m_pTextShield != NULL)
	{
		char sz[16] = { 0 };
		snprintf(sz, 15, "%d", cnt);
		std::string str(sz);
		m_pTextShield->setString(str);
	}
}

void HubLayer::initBossHp()
{
	m_pBossHpBar = HpBar::createWithLoadingBar(m_pBossHpBg);
	m_pBossHpBar->addHpBar(m_pBossHp3);
	m_pBossHpBar->addHpBar(m_pBossHp2);
	m_pBossHpBar->addHpBar(m_pBossHp1);
	m_pBossHpBar->setMaxValue(100);
	m_pBossHpBar->setCurValue(100);
	addChild(m_pBossHpBar);
}

void HubLayer::displayBossHp()
{
	if (m_pBossHpBar != NULL)
	{
		m_pBossHpBar->setVisible(true);
	}
}

void HubLayer::hideBossHp()
{
	if (m_pBossHpBar != NULL)
	{
		m_pBossHpBar->setVisible(false);
	}
}

void HubLayer::updateBossHp(int hp)
{
	if (m_pBossHpBar != NULL)
	{
		m_pBossHpBar->setCurValue(hp);
	}
}

void HubLayer::updateBossMaxHp(int hp)
{
	if (m_pBossHpBar != NULL)
	{
		m_pBossHpBar->setMaxValue(hp);
	}
}

void HubLayer::makeSkillCD(const std::string& filename, ui::Widget* pMenuItem, float fSecond)
{
	//设置按钮不可用
	pMenuItem->setEnabled(false);

	//创建技能cd样式
	auto pSprite = Sprite::createWithSpriteFrameName(filename);
	pSprite->setOpacity(120);
	pSprite->setColor(Color3B(0, 0, 0));

	auto cd = ProgressTimer::create(pSprite);

	//顺时针转一圈
	cd->setType(ProgressTimer::Type::RADIAL);
	cd->setReverseDirection(true);
	//从上到下
	//cd->setType(ProgressTimer::Type::BAR);
	//cd->setBarChangeRate(Point(0, 1));
	//cd->setMidpoint(Point(0.5, 0));

	cd->setPercentage(99.9f);
	cd->setPosition(pMenuItem->getPosition());
	this->addChild(cd, 1000);
	//技能冷却动画
	auto progressto = ProgressTo::create(fSecond, 0);
	//冷却完成之后的回调，销毁对象，同时使技能按钮可用
	auto callfunn = CallFunc::create([=]()
	{
		cd->removeFromParentAndCleanup(true);
		pSprite->removeFromParentAndCleanup(true);
		pMenuItem->setEnabled(true);
	});

	cd->runAction(Sequence::create(progressto, callfunn, NULL));
}

void HubLayer::displayKm(bool display)
{
	if (display)
	{
		m_pSpriteKm->setVisible(true);
	}
	else
	{
		m_pSpriteKm->setVisible(false);
	}

	this->displayHugGift();
}

void HubLayer::updateKm(int num)
{
	char sz[16] = { 0 };
	snprintf(sz, 15, "%d", num);
	//int len = snprintf(sz, 15, "%d.", num/10);
	//snprintf(sz + len, 15, "%d", num % 10);
	std::string str(sz);

	m_pTextKmNum->setString(str);
}

void HubLayer::displayCombo(bool display)
{
	if (display && !m_pSpriteCombo->isVisible())
	{
		m_pSpriteCombo->setVisible(true);

		this->displayHugGift();
	}
	else if (!display && m_pSpriteCombo->isVisible())
	{
		FadeTo* pFade = FadeTo::create(1.f, 0);
		FiniteTimeAction* pDone = CallFunc::create([this](){
			m_pSpriteCombo->setVisible(false);
			m_pSpriteCombo->setScale(1.0f);
			this->displayHugGift();
		});
		Sequence* pSeq = Sequence::create(pFade, pDone, nullptr);
		m_pSpriteCombo->runAction(pSeq);
	}

}

void HubLayer::updateCombo(int num)
{
	m_pSpriteCombo->setOpacity(255);

	char sz[16] = { 0 };
	snprintf(sz, 15, "%d", num);
	std::string str(sz);

	m_pTextComboNum->setString(str);

	m_pSpriteCombo->stopAllActions();

	ScaleTo* pScale1 = ScaleTo::create(0.07f, 1.3f);
	DelayTime* pDelay = DelayTime::create(0.04f);
	ScaleTo* pScale2 = ScaleTo::create(0.07f, 1.0f);
	Sequence* pSeq = Sequence::create(pScale1, pDelay, pScale2, nullptr);
	m_pSpriteCombo->runAction(pSeq);
}

void HubLayer::updateMoneyPresent(int num)
{
	char buf[11] = { 0 };
	snprintf(buf, 10, "%d", num);
	m_pTextMoneyPresent->setString(buf);
	m_pSpriteMoneyPresent->setOpacity(255);
	m_pSpriteMoneyPresent->setVisible(true);
	m_pSpriteMoneyPresent->setPosition(m_pSpriteMoneyPresentPos);

	MoveBy* pMove = MoveBy::create(1.0f, Vec2(0, 15));
	FadeTo* pFade = FadeTo::create(1.0f, 100);
	Spawn* pSpawn = Spawn::create(pMove, pFade, nullptr);
	FiniteTimeAction* pDone = CallFunc::create([this](){
		m_pSpriteMoneyPresent->setVisible(false);
	});
	Sequence* pSeq = Sequence::create(pSpawn, pDone, nullptr);
	m_pSpriteMoneyPresent->runAction(pSeq);
}
