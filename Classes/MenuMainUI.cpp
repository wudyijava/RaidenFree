#include "Achievement.h"
#include "BillingInterface.h"
#include "Channel.h"
#include "GeneralDialog.h"
#include "GameData.h"
#include "GameScene.h"
#include "GiftLayer.h"
#include "GiftDuoLayer.h"
#include "GlobalData.h"
#include "MenuBag.h"
#include "MenuMainUI.h"
#include "MenuMission.h"
#include "MenuShop.h"
#include "MenuStrengthen.h"
#include "ModeBase.h"
#include "MenuReward.h"
#include "Player.h"
#include "Preload.h"
#include "Setting.h"
#include "Sound.h"
#include "PromptLayer.h"
#include "FakeLoadingLayer.h"
#include "Guide.h"
#include "GiftVip.h"
#include "Difficulty.h"

namespace MenuMainUIConstant
{
	//飞机图片半径
	const int PLANE_R = 50;

	const int PLANE_POS_Y = 110;

	//两架飞机间的距离
	const int PLANE_DISTANCE = 300;

	const char* const STRING_UNLOCK_PLANE_LEVEL_4 = "unlock_plane_level_4";
	const char* const STRING_UNLOCK_PLANE_LEVEL_4_1 = "unlock_plane_level_4_1";

	const char* const STRING_ENDLESS_OPEN_LEVEL = "endless_open_level";
	const char* const STRING_RAPIDLY_OPEN_LEVEL = "rapidly_open_level";

	const int AircraftOtherOpen = 1;
	const int Aircraft2Open = 8;
	const int Aircraft3Open = 16;
}

Scene* MenuMainUI::scene()
{
	Scene *scene = Scene::create();
	MenuMainUI *layer = MenuMainUI::create();
	scene->addChild(layer);

	return scene;
}

MenuMainUI::MenuMainUI()
: m_pRootNode(nullptr)
, m_pRootNodeVip(nullptr)
//三种游戏模式
, m_pBtnLevel(nullptr)
, m_pBtnEndless(nullptr)
, m_pBtnRapidly(nullptr)
, m_pBtnUnlock(nullptr)
, m_pUnlockPrompt(nullptr)
//菜单按钮
, m_pBtnShop(nullptr)
, m_pBtnSetup(nullptr)
, m_pBtnBag(nullptr)
, m_pBtnStrengthen(nullptr)
//左右选飞机
, m_pBtnArrowL(nullptr)
, m_pBtnArrowR(nullptr)
//分数、宝石
, m_pTextScore(nullptr)
, m_pTextMoney(nullptr)
//战机评级
, m_pTextAttack(nullptr)
, m_pTextMissile(nullptr)
, m_pTextArmor(nullptr)
, m_pTextWingman(nullptr)
//战机介绍
, m_pTextAircraftTitle(nullptr)
, m_pTextAircraftDesc(nullptr)
//战机评级条
, m_pBarAttack(nullptr)
, m_pBarMissile(nullptr)
, m_pBarArmor(nullptr)
, m_pBarWingman(nullptr)
//礼包、奖励
, m_pBtnGift(nullptr)
, m_pBtnReward(nullptr)
//vip
, m_pBtnVip(nullptr)
, m_pTxtVipDays(nullptr)
//奖励提醒
, m_pNewAchievement(nullptr)
//键盘监听
, m_pKeyboardListener(nullptr)
{
	m_menuFlg = false;
	m_moveFlg = false;
	m_visibleSize = Director::getInstance()->getWinSize();

	m_playerNumber = 0;

	m_money = 0;
	m_score = 0;

}

MenuMainUI::~MenuMainUI()
{
	GameData::getInstance()->unRegListener(this, GAMEDATA::MONEY);
}

bool MenuMainUI::init()
{
	DEBUG_LOG("MenuMainUI::init");

	if (!Layer::init())
	{
		return false;
	}

	//音乐
	Sound::turnOffBulletSound();
	Sound::playSound(SOUND::ID_MENU_GB, true);

	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("MainUI.csb");
	addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("MainUI.csb");
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, true);

	//控件
	FIND_UI_CONTROL_RE(ui::Button*, "Level_Btn", m_pBtnLevel, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Endless_Btn", m_pBtnEndless, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Rapidly_Btn", m_pBtnRapidly, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Unlock_Btn", m_pBtnUnlock, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "UnlockPrompt_Txt", m_pUnlockPrompt, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Shop_Btn", m_pBtnShop, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Setup_Btn", m_pBtnSetup, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Bag_Btn", m_pBtnBag, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Strengthen_Btn", m_pBtnStrengthen, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "MainUI_Arrow_L_Btn", m_pBtnArrowL, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "MainUI_Arrow_R_Btn", m_pBtnArrowR, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num1", m_pTextScore, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num2", m_pTextMoney, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num3", m_pTextAttack, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num4", m_pTextArmor, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num5", m_pTextWingman, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num6", m_pTextMissile, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::LoadingBar*, "MainUI_Prog1", m_pBarAttack, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "MainUI_Prog2", m_pBarArmor, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "MainUI_Prog3", m_pBarWingman, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::LoadingBar*, "MainUI_Prog4", m_pBarMissile, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "MainUI_Aircraft_Title_Txt", m_pTextAircraftTitle, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "MainUI_Aircraft_Desc_Txt", m_pTextAircraftDesc, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "MainUI_Gift_Btn", m_pBtnGift, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "MainUI_Reward_Btn", m_pBtnReward, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "MainUI_Reward_Message_Bg", m_pNewAchievement, m_pRootNode);

	//vip
	FIND_UI_CONTROL_RE(Node*, "Vip_Node", m_pRootNodeVip, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Vip_Btn", m_pBtnVip, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Vip_Days_Txt", m_pTxtVipDays, m_pRootNode);

	updateVipState();

	//vip未联网
	if (GameData::getInstance()->getVipTime() == -1)
	{
		auto pLayer = GiftVipLayer::create();
		pLayer->setOnExitCallback([this](){
			this->updateVipState();
		});
		addChild(pLayer, 99);
	}

	//控件属性
	//m_pBtnLevel->setVisible(false);
	((Label*)m_pTextAircraftDesc->getVirtualRenderer())->setMaxLineWidth(420.f);
	updateScoreAndMoney(m_money, m_score);

	Guide* pGuide = nullptr;
	if (!pGuide)
	{
		if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= MenuMainUIConstant::Aircraft2Open
			&& !GameData::getInstance()->checkGuide(GuideType::Aircraft2Open))
		{
			int iPlayerId = 2;
			GameData::getInstance()->setValue(GAMEDATA::PLAYER, iPlayerId);
			SelectPlane(iPlayerId);
			pGuide = GuideMgr::getInstance()->show(GuideType::Aircraft2Open, this, nullptr);
		}
	}
	if (!pGuide)
	{
		if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= MenuMainUIConstant::Aircraft3Open
			&& !GameData::getInstance()->checkGuide(GuideType::Aircraft3Open))
		{
			int iPlayerId = 3;
			GameData::getInstance()->setValue(GAMEDATA::PLAYER, iPlayerId);
			SelectPlane(iPlayerId);
			pGuide = GuideMgr::getInstance()->show(GuideType::Aircraft3Open, this, nullptr);
		}
	}
	if (!pGuide)
	{
		SelectPlane(GameData::getInstance()->getValueToInt(GAMEDATA::PLAYER));
	}

	//控件事件
	addButtonTouchEffect(m_pBtnVip, Utils::ButtonTouchEffect::DARK, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		auto pLayer = GiftVipLayer::create();
		pLayer->setOnExitCallback([this](){
			this->updateVipState();
		});
		addChild(pLayer);

		GameData::getInstance()->setValue(GAMEDATA::VIP_EXPIRE_REMIND, 1);
		GameData::getInstance()->setValue(GAMEDATA::VIP_ORDER_REMIND, 1);

		this->updateVipState();
	});

	addButtonTouchEffect(m_pBtnGift, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		//auto pLayer = GiftLayer::create(GoodsType::HUG_GIFT);
		std::vector<const char*> goodsArray;
		goodsArray.push_back(GoodsType::HUG_GIFT);
		goodsArray.push_back(GoodsType::BIG_GIFT);
		auto pLayer = GiftDuoLayer::create(goodsArray);
		addChild(pLayer);
	});

	addButtonTouchEffect(m_pBtnReward, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::MenuReward);

		Director::getInstance()->pushScene(MenuReward::scene());
	});

	addButtonTouchEffect(m_pBtnLevel, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::WelcomeClickButtonLevel);

		GameData::getInstance()->setValue(GAMEDATA::MODE, ModeBase::ModeLevel);
		Scene* pScene = MenuMission::scene();
		TransitionProgressInOut* ts = TransitionProgressInOut::create(0.3f, pScene);
		Director::getInstance()->replaceScene(ts);
	});

	addButtonTouchEffect(m_pBtnEndless, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::ModeEndless);

		if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= ModeBase::ModeEndlessOpen)
		{
			GuideMgr::getInstance()->destory(GuideType::ModeEndless);

			GameData::getInstance()->setValue(GAMEDATA::MODE, ModeBase::ModeEndless);
			GameData::getInstance()->setValue(GAMEDATA::LEVEL, ModeBase::LevelIdEndless);

			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, FakeLoadingLayer::scene()));
		}
		else
		{
			//提示第八关后开启
			auto pPromptLayer = new PromptLayer;
			pPromptLayer->init();
			pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(MenuMainUIConstant::STRING_ENDLESS_OPEN_LEVEL));
			pPromptLayer->autorelease();
			this->addChild(pPromptLayer);
			pPromptLayer->enableTouch(true);
		}
	});

	addButtonTouchEffect(m_pBtnRapidly, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::ModeRapid);

		if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= ModeBase::ModeRapidOpen)
		{
			GuideMgr::getInstance()->destory(GuideType::ModeRapid);
			
			GameData::getInstance()->setValue(GAMEDATA::MODE, ModeBase::ModeRapid);
			GameData::getInstance()->setValue(GAMEDATA::LEVEL, ModeBase::LevelIdRapid);

			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, FakeLoadingLayer::scene()));
		}
		else
		{
			//提示第13关后开启
			auto pPromptLayer = new PromptLayer;
			pPromptLayer->init();
			pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(MenuMainUIConstant::STRING_RAPIDLY_OPEN_LEVEL));
			pPromptLayer->autorelease();
			this->addChild(pPromptLayer);
			pPromptLayer->enableTouch(true);
		}
	});

	addButtonTouchEffect(m_pBtnUnlock, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->unlockPlane();
	});

	addButtonTouchEffect(m_pBtnShop, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->removeKeyboardListener();

		auto pLayer = MenuShopLayer::create();
		pLayer->setOnExitCallback([this](){
			this->addKeyboardListener();

		});

		this->addChild(pLayer);
	});

	addButtonTouchEffect(m_pBtnSetup, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->removeKeyboardListener();

		auto pLayer = SettingLayer::create();
		pLayer->setOnExitCallback([this](){
			this->addKeyboardListener();

		});

		this->addChild(pLayer);
	});

	addButtonTouchEffect(m_pBtnBag, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::MenuBag);

		this->removeKeyboardListener();

		auto pLayer = MenuBag::create();
		pLayer->setOnExitCallback([this](){
			this->addKeyboardListener();

			int oldPlayer = m_pageNumber;
			int newPlayer = m_pageNumber;
			auto array = GlobalData::getInstance()->getEquipmentArray(EquipTypeAircraft);
			for (unsigned i = 0; i < array.size(); i++)
			{
				if (array.at(i)->equip == 1)
				{
					newPlayer = i;
					break;
				}
			}

			//装备的飞机改变
			if (newPlayer != oldPlayer)
			{
				m_playerArray.at(m_pageNumber)->stopAutoShoot();
				for (int i = 0; (unsigned)i < m_playerArray.size(); i++)
				{
					if (i < newPlayer) //左
					{
						m_playerArray[i]->setPosition(
							Vec2(-m_playerArray[i]->getContentSize().width - MenuMainUIConstant::PLANE_DISTANCE, MenuMainUIConstant::PLANE_POS_Y));
					}
					else if (i == newPlayer) //当前
					{
						m_playerArray[i]->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, MenuMainUIConstant::PLANE_POS_Y));
						m_playerArray[i]->updateEquip();
						m_playerArray[i]->autoShoot();
						m_pageNumber = i;
					}
					else //右
					{
						m_playerArray[i]->setPosition(
							Vec2(Director::getInstance()->getWinSize().width 
								+ m_playerArray[i]->getContentSize().width 
								+ MenuMainUIConstant::PLANE_DISTANCE, MenuMainUIConstant::PLANE_POS_Y));
					}
				}
			}
			else //装备的飞机未变
			{
				m_playerArray.at(m_pageNumber)->stopAutoShoot();
				m_playerArray.at(m_pageNumber)->updateEquip();
				m_playerArray.at(m_pageNumber)->autoShoot();
			}

			this->updateAircraftAttr();
			this->updateButtonState();
		});

		this->addChild(pLayer);
	});

	addButtonTouchEffect(m_pBtnStrengthen, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::MenuStrengthen);

		this->removeKeyboardListener();

		auto pLayer = MenuStrengthen::create();
		pLayer->setOnExitCallback([this](){
			this->addKeyboardListener();

		});

		this->addChild(pLayer);
	});

	addButtonTouchEffect(m_pBtnArrowL, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->scrollImage(m_pBtnArrowL);
	});

	addButtonTouchEffect(m_pBtnArrowR, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->scrollImage(m_pBtnArrowR);
	});

	this->addKeyboardListener();

	GameData::getInstance()->regListener(this, GAMEDATA::MONEY);

	this->updateAircraftAttr();

	Channel::start();

	DEBUG_LOG("MenuMainUI::init finish");

	return true;
}

void MenuMainUI::onEnter()
{
	Layer::onEnter();

	//新手引导
	Guide* pGuide = GuideMgr::getInstance()->show(GuideType::WelcomeGirl, this, nullptr, [this](){
		GuideMgr::getInstance()->show(GuideType::WelcomeClickButtonLevel, this, m_pBtnLevel);
	});

	if (!pGuide)
	{
		pGuide = GuideMgr::getInstance()->show(GuideType::MenuBag, this, m_pBtnBag);
	}

	if (!pGuide)
	{
		pGuide = GuideMgr::getInstance()->show(GuideType::MenuStrengthen, this, m_pBtnStrengthen);
	}

	if (!pGuide)
	{
		pGuide = GuideMgr::getInstance()->show(GuideType::ModeEndless, this, m_pBtnEndless);
	}

	if (!pGuide)
	{
		pGuide = GuideMgr::getInstance()->show(GuideType::ModeRapid, this, m_pBtnRapidly);
	}

	DEBUG_LOG("MenuMainUI::onEnter : judgeAchievement");
	int cnt = AchievementMgr::getInstance()->judgeAchievement();
	if (cnt > 0 )
	{
		m_pNewAchievement->setVisible(true);

		if (!pGuide)
		{
			GuideMgr::getInstance()->show(GuideType::MenuReward, this, m_pBtnReward);
		}
	}
	else
	{
		m_pNewAchievement->setVisible(false);
	}

	this->updateValue(GAMEDATA::MONEY, GameData::getInstance()->getValueToInt(GAMEDATA::MONEY));
	this->updateButtonState(); 

	DEBUG_LOG("MenuMainUI::onEnter : finish");
}

void MenuMainUI::updateVipState()
{
	int enableVip = Utils::str2int(GlobalData::getInstance()->getParameter(GLOBALDATA::ENABLE_VIP).c_str());
	if (enableVip == 0)
	{
		m_pRootNodeVip->setVisible(false);
		return;
	}

	m_pRootNodeVip->setVisible(true);

	int time = GameData::getInstance()->getVipTime();
	int day, hour, minute, second;
	Utils::timeToString(time, day, hour, minute, second);
	if (hour > 0 || minute > 0 || second > 0)
	{
		++day;
	}

	if (day > 0)
	{
		char buf[11] = { 0 };
		snprintf(buf, 10, "%d", day);
		m_pTxtVipDays->setString(buf);
		m_pTxtVipDays->setVisible(true);
	}
	else
	{
		m_pTxtVipDays->setVisible(false);
	}

	if ((day <= 3 && day > 0 && GameData::getInstance()->getValueToInt(GAMEDATA::VIP_EXPIRE_REMIND) == 0) //快要到期了
		|| (day == 0 && GameData::getInstance()->getValueToInt(GAMEDATA::VIP_ORDER_REMIND) == 0) //未购买提醒
		) 
	{
		auto pAimActionVip = Preload::getInstance()->getUIAction("Vip.csb");
		m_pRootNodeVip->runAction(pAimActionVip);
		pAimActionVip->gotoFrameAndPlay(0, true);

		m_pBtnVip->loadTextureNormal("MainUI_VIP_Img.png", ui::Widget::TextureResType::PLIST);
		m_pBtnVip->loadTexturePressed("MainUI_VIP_Img.png", ui::Widget::TextureResType::PLIST);
	}
	else
	{
		m_pRootNodeVip->stopAllActions();

		if (day > 0)
		{
			m_pBtnVip->loadTextureNormal("MainUI_VIP_Img.png", ui::Widget::TextureResType::PLIST);
			m_pBtnVip->loadTexturePressed("MainUI_VIP_Img.png", ui::Widget::TextureResType::PLIST);
		}
		else
		{
			m_pBtnVip->loadTextureNormal("MainUI_VIP_Img_Disable.png", ui::Widget::TextureResType::PLIST);
			m_pBtnVip->loadTexturePressed("MainUI_VIP_Img_Disable.png", ui::Widget::TextureResType::PLIST);
		}

	}


}

void MenuMainUI::unlockPlane()
{
	if (m_pageNumber == 3)
	{
		auto layer = GiftLayer::create(GoodsType::PLANE_4TH);
		layer->setOnExitCallback([this](){
			this->updateButtonState();
		});
		this->addChild(layer);
	}
	else if (m_pageNumber == 4)
	{
		auto layer = GiftLayer::create(GoodsType::PLANE_5TH);
		layer->setOnExitCallback([this](){
			this->updateButtonState();
		});
		this->addChild(layer);
	}
	else if (m_pageNumber == 5)
	{
		auto layer = GiftLayer::create(GoodsType::PLANE_6TH);
		layer->setOnExitCallback([this](){
			this->updateButtonState();
		});
		this->addChild(layer);
	}
}

void MenuMainUI::updateValue(const std::string& type, int value)
{
	if (type == GAMEDATA::MONEY)
	{
		char sz[11] = { 0 };
		snprintf(sz, 10, "%d", value);
		m_pTextMoney->setString(sz);
	}
}

void MenuMainUI::updateScoreAndMoney(int &money,int &score)
{
	money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);
	score = GameData::getInstance()->getValueToInt(GAMEDATA::SCORE);

	char strScore[20];
	char strMoney[20];
	sprintf(strScore, "%d", score);
	sprintf(strMoney, "%d", money);

	m_pTextScore->setString(strScore);
	m_pTextMoney->setString(strMoney);
}

void MenuMainUI::updateAircraftAttr()
{
	auto pAircraft = m_playerArray.at(m_pageNumber);
	char buf[20 +1] = { 0 };

	snprintf(buf, 20, "%d", pAircraft->getAttrAttack());
	m_pTextAttack->setString(buf);
	m_pBarAttack->setPercent(pAircraft->getAttrAttack()*100.f / 11000.f);

	snprintf(buf, 20, "%d", pAircraft->getAttrArmor());
	m_pTextArmor->setString(buf);
	m_pBarArmor->setPercent(pAircraft->getAttrArmor()*100.f / 5000.f);

	snprintf(buf, 20, "%d", pAircraft->getAttrMissile());
	m_pTextMissile->setString(buf);
	m_pBarMissile->setPercent(pAircraft->getAttrMissile()*100.f / 7000.f);

	snprintf(buf, 20, "%d", pAircraft->getAttrWingman());
	m_pTextWingman->setString(buf);
	m_pBarWingman->setPercent(pAircraft->getAttrWingman()*100.f / 4000.f);

	m_pTextAircraftTitle->setString(pAircraft->getName());
	m_pTextAircraftDesc->setString(pAircraft->getAttr(AircraftAttr::Desc));
}

void MenuMainUI::SelectPlane(int playerId)
{
	if (m_playerArray.size() == 0)
	{
		const std::vector<TAircraftData*>& allPlayer = GlobalData::getInstance()->getAllPlayerData();
		m_playerNumber = allPlayer.size();
		if (Utils::str2int(GlobalData::getInstance()->getParameter(GLOBALDATA::ENABLE_VIP).c_str()) == 0)
		{
			--m_playerNumber;
		}

		//飞机每次移动距离
		int moveLen = m_visibleSize.width / 2 + MenuMainUIConstant::PLANE_R;

		//创建容器、设置大小
		Size mysize = Size(moveLen*m_playerNumber, MenuMainUIConstant::PLANE_POS_Y);
		LayerColor* scrollLayer = LayerColor::create(Color4B(0, 0, 255, 0));
		scrollLayer->setContentSize(mysize);

		//容器中添加三个图片
		m_pageNumber = -1;
		for (int i = 0; i < m_playerNumber; i++)
		{
			PlayerPlane* player = PlaneCreator<PlayerPlane>::create(scrollLayer, allPlayer.at(i));
			m_playerArray.push_back(player);

			if (player->getId() == playerId)
			{
				m_pageNumber = i;
				player->autoShoot();
				player->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, MenuMainUIConstant::PLANE_POS_Y));
			}
			else
			{
				player->setPosition(Vec2(Director::getInstance()->getWinSize().width 
					+ player->getContentSize().width 
					+ MenuMainUIConstant::PLANE_DISTANCE, 
					MenuMainUIConstant::PLANE_POS_Y));
			}
			//moveLen是飞机的间距：PLANE_R是让飞机显示的位置在屏幕中心 
			//player->setPosition(Vec2(moveLen * i - player->getContentSize().width/2, MenuMainUIConstant::PLANE_POS_Y));
		}

		if (m_pageNumber < 0)
		{
			m_pageNumber = 0;
			m_playerArray.at(0)->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, MenuMainUIConstant::PLANE_POS_Y));
			m_playerArray.at(0)->autoShoot();
		}

		ScrollView* scrollView = ScrollView::create(Size(m_visibleSize.width, 360/*458*/), scrollLayer);//滚动窗口大小
		scrollView->setPosition(0, m_visibleSize.height * 1 / 2 - 100);//滚动窗口距离左边界的距离
		scrollView->setTouchEnabled(false);
		this->addChild(scrollView);
	}
	else
	{
		for (unsigned i = 0; i < m_playerArray.size(); i++)
		{
			if (m_playerArray[i]->getId() == playerId)
			{
				m_pageNumber = i;
				m_playerArray[i]->autoShoot();
				m_playerArray[i]->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, MenuMainUIConstant::PLANE_POS_Y));
			}
			else
			{
				m_playerArray[i]->stopAutoShoot();
				m_playerArray[i]->setPosition(Vec2(Director::getInstance()->getWinSize().width 
					+ m_playerArray[i]->getContentSize().width 
					+ MenuMainUIConstant::PLANE_DISTANCE, 
					MenuMainUIConstant::PLANE_POS_Y));
			}
		}
	}


	//point
	const int distance = 20;
	float indicatorLeft = 0;
	if (m_playerNumber % 2 == 0)
	{
		indicatorLeft = -distance / 2 - (m_playerNumber - 1) / 2 * distance;
	}
	else
	{
		indicatorLeft = -(m_playerNumber - 1) / 2 * distance;
	}

	if (m_indicatorArray.size() == 0)
	{
		for (int i = 0; i < m_playerNumber; i++)
		{
			Sprite* p = NULL;
			if (i == m_pageNumber)
			{
				p = Sprite::createWithSpriteFrameName("activity2.png");
			}
			else
			{
				p = Sprite::createWithSpriteFrameName("activity1.png");
			}
			p->setPosition(Vec2(m_visibleSize.width / 2 + indicatorLeft + distance * i, m_visibleSize.height / 2 - 110));
			this->addChild(p);
			m_indicatorArray.push_back(p);
		}
	}
	else
	{
		for (unsigned i = 0; i < m_indicatorArray.size(); i++)
		{
			if (i == m_pageNumber)
			{
				m_indicatorArray[i]->setSpriteFrame("activity2.png");
			}
			else
			{
				m_indicatorArray[i]->setSpriteFrame("activity1.png");
			}
		}
	}
}

void MenuMainUI::scrollImage(Ref* sender)
{
	Sound::playSound(SOUND::ID_CLICK);

	int orignPageNumber = m_pageNumber;
	if (sender == m_pBtnArrowL/* && pLeft->isEnabled()*/)
	{
		--m_pageNumber;
		m_pageNumber = (m_pageNumber < 0 ? m_playerNumber - 1 : m_pageNumber);
	}
	else/* if (pRight->isEnabled())*/
	{
		++m_pageNumber;
		m_pageNumber = (m_pageNumber > m_playerNumber - 1 ? 0 : m_pageNumber);
	}

	float y = m_playerArray.at(orignPageNumber)->getPosition().y;
	Vec2 left(-MenuMainUIConstant::PLANE_DISTANCE, y);
	Vec2 right(Director::getInstance()->getWinSize().width + MenuMainUIConstant::PLANE_DISTANCE, y);
	Vec2 center(Director::getInstance()->getWinSize().width / 2, y);

	PlayerPlane* pOrignPlane = m_playerArray.at(orignPageNumber);
	PlayerPlane* pDestPlane = m_playerArray.at(m_pageNumber);

	pOrignPlane->stopAutoShoot();

	if (sender == m_pBtnArrowL)
	{
		MoveTo* moveOut = MoveTo::create(0.3f, right);
		pOrignPlane->runAction(moveOut);

		pDestPlane->setPosition(left);
		MoveTo* moveIn = MoveTo::create(0.3f, center);
		FiniteTimeAction* moveDone = CallFuncN::create(CC_CALLBACK_0(MenuMainUI::moveCenterDone, this, pDestPlane));

		Sequence* sequence = Sequence::create(moveIn, moveDone, NULL);
		pDestPlane->runAction(sequence);
	}
	else
	{
		MoveTo* moveOut = MoveTo::create(0.3f, left);
		pOrignPlane->runAction(moveOut);

		pDestPlane->setPosition(right);
		MoveTo* moveIn = MoveTo::create(0.3f, center);
		FiniteTimeAction* moveDone = CallFuncN::create(CC_CALLBACK_0(MenuMainUI::moveCenterDone, this, pDestPlane));

		Sequence* sequence = Sequence::create(moveIn, moveDone, NULL);
		pDestPlane->runAction(sequence);
	}

	GameData::getInstance()->setValue(GAMEDATA::PLAYER, pDestPlane->getId());

	Vec2 p1 = m_indicatorArray.at(m_pageNumber)->getPosition();
	Vec2 p2 = m_indicatorArray.at(orignPageNumber)->getPosition();

	m_indicatorArray.at(m_pageNumber)->setPosition(p2);
	m_indicatorArray.at(orignPageNumber)->setPosition(p1);

	Sprite* tmp = m_indicatorArray.at(m_pageNumber);
	m_indicatorArray[m_pageNumber] = m_indicatorArray[orignPageNumber];
	m_indicatorArray[orignPageNumber] = tmp;

	this->updateAircraftAttr();
}

void MenuMainUI::moveCenterDone(PlayerPlane* p)
{
	p->updateEquip();
	p->autoShoot();

	for (unsigned i = 0; i < m_playerArray.size(); i++)
	{
		if (m_playerArray.at(i) != p)
		{
			m_playerArray.at(i)->stopAutoShoot();
		}
	}

	updateButtonState();
}

void MenuMainUI::updateButtonState()
{
	int finishLevel = GameData::getInstance()->getLevelFinish(Difficulty::Easy);

	auto array = GlobalData::getInstance()->getEquipmentArray(EquipTypeAircraft);
	auto pData = array.at(m_pageNumber);
	int equipId = m_pageNumber + 1;
	//第一架飞机默认开启
	if (0 == m_pageNumber)
	{
		m_pBtnLevel->setVisible(true);
		m_pBtnLevel->setEnabled(true);
		m_pBtnUnlock->setVisible(false);

		displayUnlockPrompt("");
	}
	//第2架飞机在完成第8关后开启
	else if (1 == m_pageNumber)
	{
		if (finishLevel >= MenuMainUIConstant::Aircraft2Open)
		{
			m_pBtnLevel->setEnabled(true);
			m_pBtnLevel->setVisible(true);
			m_pBtnUnlock->setVisible(false);

			displayUnlockPrompt("");

			pData->own = 1;
			pData->equip = 1;
			PlayerEquip::getInstance()->addEquip(equipId, GlobalData::getInstance()->getEquipment(equipId));
		}
		else
		{
			m_pBtnLevel->setEnabled(false);
			m_pBtnLevel->setVisible(true);
			m_pBtnUnlock->setVisible(false);

			displayUnlockPrompt(GlobalData::getInstance()->getParameter(GLOBALDATA::STRING_UNLOCK_PLANE_LEVEL_2));

			pData->own = 0;
			pData->equip = 0;
			PlayerEquip::getInstance()->removeEquip(equipId, equipId);
		}
	}
	//第3架飞机在完成第16关后开启
	else if (2 == m_pageNumber)
	{
		if (finishLevel >= MenuMainUIConstant::Aircraft3Open)
		{
			m_pBtnLevel->setEnabled(true);
			m_pBtnLevel->setVisible(true);
			m_pBtnUnlock->setVisible(false);

			displayUnlockPrompt("");

			pData->own = 1;
			pData->equip = 1;
			PlayerEquip::getInstance()->addEquip(equipId, GlobalData::getInstance()->getEquipment(equipId));
		}
		else
		{
			m_pBtnLevel->setEnabled(false);
			m_pBtnLevel->setVisible(true);
			m_pBtnUnlock->setVisible(false);

			displayUnlockPrompt(GlobalData::getInstance()->getParameter(GLOBALDATA::STRING_UNLOCK_PLANE_LEVEL_3));

			pData->own = 0;
			pData->equip = 0;
			PlayerEquip::getInstance()->removeEquip(equipId, equipId);
		}
	}
	//其他飞机在完成第一关后可解锁，但需付费
	else if (3 <= m_pageNumber && m_pageNumber < 6)
	{
		if (finishLevel >= MenuMainUIConstant::AircraftOtherOpen)
		{
			if (pData->own)
			{
				m_pBtnLevel->setEnabled(true);
				m_pBtnLevel->setVisible(true);
				m_pBtnUnlock->setVisible(false);
	
				pData->own = 1;
				pData->equip = 1;
				PlayerEquip::getInstance()->addEquip(equipId, GlobalData::getInstance()->getEquipment(equipId));

				switch (m_pageNumber)
				{
				case 3:
					GameData::getInstance()->setValue(GAMEDATA::USE_AIRCRAFT_4, 1);
					break;
				case 4:
					GameData::getInstance()->setValue(GAMEDATA::USE_AIRCRAFT_5, 1);
					break;
				case 5:
					GameData::getInstance()->setValue(GAMEDATA::USE_AIRCRAFT_6, 1);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pBtnLevel->setEnabled(false);
				m_pBtnLevel->setVisible(false);
				m_pBtnUnlock->setVisible(true);
	
				pData->own = 0;
				pData->equip = 0;
				PlayerEquip::getInstance()->removeEquip(equipId, equipId);
			}

			displayUnlockPrompt("");
		}
		else
		{
			m_pBtnLevel->setEnabled(false);
			m_pBtnLevel->setVisible(true);
			m_pBtnUnlock->setVisible(false);

			displayUnlockPrompt(GlobalData::getInstance()->getParameter(GLOBALDATA::STRING_UNLOCK_PLANE_LEVEL_1));

			pData->own = 0;
			pData->equip = 0;
			PlayerEquip::getInstance()->removeEquip(equipId, equipId);
		}
	}
	else if (m_pageNumber == 6)
	{
		if (pData->own)//已拥有
		{
			m_pBtnLevel->setEnabled(true);
			m_pBtnLevel->setVisible(true);
			m_pBtnUnlock->setVisible(false);

			pData->own = 1;
			pData->equip = 1;
			PlayerEquip::getInstance()->addEquip(equipId, GlobalData::getInstance()->getEquipment(equipId));

			displayUnlockPrompt("");
		}
		else //未拥有
		{
			if (pData->pick >= pData->piece) //已集齐碎片
			{
				m_pBtnLevel->setEnabled(true);
				m_pBtnLevel->setVisible(true);
				m_pBtnUnlock->setVisible(false);

				//pData->own = 1;
				//pData->equip = 1;
				//PlayerEquip::getInstance()->addEquip(equipId, GlobalData::getInstance()->getEquipment(equipId));

				if (pData->own == 1)
				{
					displayUnlockPrompt("");
				}
				else
				{
					displayUnlockPrompt(GlobalData::getInstance()->getParameter(MenuMainUIConstant::STRING_UNLOCK_PLANE_LEVEL_4_1));
				}
			}
			else //未集齐碎片
			{
				m_pBtnLevel->setEnabled(false);
				m_pBtnLevel->setVisible(true);
				m_pBtnUnlock->setVisible(false);

				pData->own = 0;
				pData->equip = 0;
				PlayerEquip::getInstance()->removeEquip(equipId, equipId);

				displayUnlockPrompt(GlobalData::getInstance()->getParameter(MenuMainUIConstant::STRING_UNLOCK_PLANE_LEVEL_4));
			}
		}

	}


	//设置无尽模式、急速模式两个按钮
	if (m_pBtnLevel->isVisible() && m_pBtnLevel->isEnabled())
	{
		m_pBtnEndless->setEnabled(true);
		m_pBtnRapidly->setEnabled(true);
	}
	else
	{
		m_pBtnEndless->setEnabled(false);
		m_pBtnRapidly->setEnabled(false);
	}

	for (auto p : array)
	{
		if (p == pData && pData->own == 1)
		{
			p->equip = 1;
		}
		else
		{
			p->equip = 0;
		}
	}
}

void MenuMainUI::displayUnlockPrompt(const std::string& prompt)
{
	if (!m_pUnlockPrompt)
	{
		return;
	}

	if (prompt.length() <= 0)
	{
		m_pUnlockPrompt->setVisible(false);
		return;
	}

	m_pUnlockPrompt->setString(prompt);

	m_pUnlockPrompt->setVisible(true);
	m_pUnlockPrompt->setOpacity(0);
	m_pUnlockPrompt->setScale(0.85f);
	FadeTo* pFade = FadeTo::create(.2f, 255.f);
	ScaleTo* pSclae = ScaleTo::create(.2f, 1.0f);
	Spawn* pSpawn = Spawn::create(pFade, pSclae, NULL);
	m_pUnlockPrompt->runAction(pSpawn);
}

void MenuMainUI::addKeyboardListener()
{
	//按返回键退出
	m_pKeyboardListener = EventListenerKeyboard::create();
	m_pKeyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == key
			|| EventKeyboard::KeyCode::KEY_ESCAPE == key
			|| EventKeyboard::KeyCode::KEY_BACKSPACE == key)
		{
			if (!BillingImpl::getInstance()->exit()
				&& !Channel::exit())
			{
				GameData::getInstance()->saveData();
				this->addChild(GeneralDialogCreator::exitGame());
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pKeyboardListener, this);
}

void MenuMainUI::removeKeyboardListener()
{
	_eventDispatcher->removeEventListener(m_pKeyboardListener);
}