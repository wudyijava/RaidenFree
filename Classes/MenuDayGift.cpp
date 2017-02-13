#include "MenuDayGift.h"
#include "GameData.h"
#include "PromptLayer.h"
#include "Preload.h"
#include "GlobalData.h"
#include "Sound.h"
#include "DayGift.h"
#include "MenuMainUI.h"
#include "Equipment.h"
#include "GiftVip.h"
#include "Common/Macro.h"
#include "Bmob/BmobUtils.h"

int MenuDayGift::m_day = 0;
int MenuDayGift::m_now = 0;

namespace MenuDayGiftConstant
{
	const int DayCnt = 30;
	const char* const STRING_GUIDE_PIECE_AIRCRAFT7_1= "guide_piece_aircraft7_1";

	const char* const STRING_DAY_GIFT_PROMPT = "day_gift_prompt";
	const char* const STRING_DAY_GIFT_PROMPT_VIP = "day_gift_prompt_vip";
};

Scene* MenuDayGift::scene()
{
	Scene *scene = Scene::create();
	MenuDayGift *layer = MenuDayGift::create();
	scene->addChild(layer);

	return scene;
}

MenuDayGift::MenuDayGift()
: m_pRootNode(nullptr)
, m_pBtnReceive(nullptr)
, m_pBtnVip(nullptr)
, m_pBtnVipContinue(nullptr)
, m_pBtnGiveup(nullptr)
, m_pScrollView(nullptr)
{
}

MenuDayGift::~MenuDayGift()
{
	for (auto p : m_giftArray)
	{
		delete p;
	}

	m_giftArray.clear();
}

time_t MenuDayGift::truncTime(time_t t)
{
	time_t xx =  t / (60 * 60 * 24) * (60 * 60 * 24);

	//struct tm* pTm = localtime(&t);
	//pTm->tm_hour = 0;
	//pTm->tm_min = 0;
	//pTm->tm_sec = 0;
	//time_t t2 = mktime(pTm);

	//std::string a = ctime(&xx);
	//std::string b = ctime(&t2);

	//DEBUG_LOG("a = %s, b = %s", a.c_str(), b.c_str());

	return xx;
}

bool MenuDayGift::isDisplayDailyGift()
{
	static const int secondInOneDay = 60 * 60 * 24;

	int tt = GameData::getInstance()->getValueToInt(GAMEDATA::TIME);
	if (tt < 0)
	{
		MenuDayGift::setTimeVal(time(NULL), 0);
		return true;
	}

	int timeNow = MenuDayGift::truncTime(tt);
	int lastLoginTime = MenuDayGift::truncTime(GameData::getInstance()->getValueToInt(GAMEDATA::DAY_REWARD_LAST_LOGIN));
	int continueDays = GameData::getInstance()->getValueToInt(GAMEDATA::DAY_REWARD_LOGIN_DAYS);

	int dayCnt = (timeNow - lastLoginTime) / secondInOneDay;
	DEBUG_LOG("displayDayGift : %d, timeNow = %d, lastLoginTime = %d, secondInOneDay = %d", 
		dayCnt, timeNow, lastLoginTime, secondInOneDay);
	if (dayCnt >= 2)
	{
		continueDays = 1;

		//GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LAST_LOGIN, timeNow);
		//GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LOGIN_DAYS, continueDays);
		//GameData::getInstance()->saveData();
		MenuDayGift::setTimeVal(timeNow, continueDays);

		return true;
	}
	else if (1 == dayCnt)
	{
		continueDays++;

		//GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LAST_LOGIN, timeNow);
		//GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LOGIN_DAYS, continueDays);
		//GameData::getInstance()->saveData();
		MenuDayGift::setTimeVal(timeNow, continueDays);

		return true;
	}
	else if (dayCnt < 1)
	{
		int day = GlobalData::getInstance()->dayGiftTestDay();
		if (day > 0)
		{
			//GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LAST_LOGIN, timeNow);
			//GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LOGIN_DAYS, day);
			//GameData::getInstance()->saveData();
			MenuDayGift::setTimeVal(timeNow, day);

			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void MenuDayGift::setTimeVal(int now, int continueDays)
{
	m_now = now;
	m_day = continueDays;
}

bool MenuDayGift::init()
{
	DEBUG_LOG("MenuDayGift::init");

	if (!Layer::init())
	{
		return false;
	}

	Sound::playSound(SOUND::ID_MENU_GB, true);

	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("Daily.csb");
	addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("Daily.csb");;
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, false);

	//m_day = GameData::getInstance()->getValueToInt(GAMEDATA::DAY_REWARD_LOGIN_DAYS);
	const std::vector<TDayGiftData*>& allDayGift = GlobalData::getInstance()->getDayGiftArray();

	//控件
	FIND_UI_CONTROL_RE(ui::Button*, "Received_Btn", m_pBtnReceive, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Vip_Btn", m_pBtnVip, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Vip_Continue_Btn", m_pBtnVipContinue, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Giveup_Btn", m_pBtnGiveup, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Daily_Day_Num1", m_pTextDay, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::ScrollView*, "ScrollView", m_pScrollView, m_pRootNode);
	
	if (m_day <= 5 * 2)
	{
		m_pScrollView->jumpToPercentVertical(0);
	}
	else if (m_day > 5*2 && m_day <= 5*4)
	{
		m_pScrollView->jumpToPercentVertical(50);
	}
	else if (m_day > 5 * 4)
	{
		m_pScrollView->jumpToPercentVertical(100);
	}

	char buf[50 + 1] = { 0 };
	snprintf(buf, 50, "%d", m_day);
	m_pTextDay->setString(buf);

	int day = 0;
	for (int i = 0; i < MenuDayGiftConstant::DayCnt; i++)
	{
		DailyGiftNode* node = new DailyGiftNode;
		snprintf(buf, 50, "Daily_Prop_Bg_%d", i + 1);
		FIND_UI_CONTROL_RE(Sprite*, buf, node->bg, m_pRootNode);
		FIND_UI_CONTROL_RE(Sprite*, "Daily_Prop", node->gift, node->bg);
		FIND_UI_CONTROL_RE(Sprite*, "Daily_Received_Img", node->received, node->bg);
		FIND_UI_CONTROL_RE(ui::TextAtlas*, "Daily_Day_Num2", node->day, node->bg);
		FIND_UI_CONTROL_RE(ui::TextAtlas*, "Daily_Prop_Num1", node->num, node->bg);
		m_giftArray.push_back(node);

		node->gift->setVisible(false);

		if (i + 1 < m_day)
		{
			node->received->setVisible(true);
		}
		else
		{
			node->received->setVisible(false);
		}

		int cnt = 0;
		int num = 0;
		const char* type = nullptr;
		for (; day < (int)allDayGift.size(); day++)
		{
			if (allDayGift[day]->day == i + 1)
			{
				cnt++;
				type = allDayGift[day]->gift.c_str();
				num = allDayGift[day]->count;
			}
			else if (allDayGift[day]->day > i + 1)
			{
				break;
			}
		}

		//设置礼物的图标
		Sprite* pGiftIcon = nullptr;
		if (1 == cnt) //礼物是单品
		{
			//图标
			if (!strcmp(type, GAMEDATA::MONEY))
			{
				pGiftIcon = Sprite::createWithSpriteFrameName("Daily_Prop_Diamond.png");
				pGiftIcon->setPosition(Vec2(node->bg->getContentSize() / 2) + Vec2(0, 4));
				pGiftIcon->setScale(0.4f);
			}
			else if (!strcmp(type, GAMEDATA::KILLER))
			{
				pGiftIcon = Sprite::createWithSpriteFrameName("Daily_Prop_Wingman.png");
				pGiftIcon->setPosition(Vec2(node->bg->getContentSize() / 2));
			}
			else if (!strcmp(type, GAMEDATA::SHIELD))
			{
				pGiftIcon = Sprite::createWithSpriteFrameName("Daily_Prop_Shield.png");
				pGiftIcon->setPosition(Vec2(node->bg->getContentSize() / 2));
			}
			else if (!strcmp(type, EquipPiece::Aircraft7)) //战机七的碎片
			{
				pGiftIcon = Sprite::createWithSpriteFrameName("Daily_Item_Plane07.png");
				pGiftIcon->setPosition(Vec2(42.71f, 52.6f));
				pGiftIcon->setScale(0.5f);
			}
			else if (!strcmp(type, EquipPiece::NoAircraft)) //护盾、副武器、僚机碎片
			{
				//TODO 暂不支持
			}

			//数量
			if (strcmp(type, EquipPiece::Aircraft7) && strcmp(type, EquipPiece::NoAircraft))
			{
				snprintf(buf, 50, "%d", num);
				node->num->setString(buf);
			}
		}
		else if (cnt > 1) //礼包
		{
			pGiftIcon = Sprite::createWithSpriteFrameName("Daily_Prop_Gift.png");
			pGiftIcon->setPosition(Vec2(node->bg->getContentSize() / 2) + Vec2(-2, -4));
			pGiftIcon->setScale(0.5f);
			node->num->setVisible(false);
		}
		node->bg->addChild(pGiftIcon);

		node->received->setLocalZOrder(pGiftIcon->getLocalZOrder() + 1);
	}

	Utils::addButtonTouchEffect(m_pBtnReceive, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		SetDataVal();

		auto layer = new PromptLayer;
		layer->setPrompt(m_prompt);
		layer->init();
		layer->autorelease();
		layer->setOnExitCallback([this](){
			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
		});
		this->addChild(layer);
	});

	Utils::addButtonTouchEffect(m_pBtnVip, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		auto pLayer = GiftVipLayer::create();
		pLayer->setOnExitCallback([this](){
			if (GameData::getInstance()->getVipTime() > 0)
			{
				m_pBtnVip->setVisible(false);
				m_pBtnVipContinue->setVisible(true);
			}
			else
			{
				m_pBtnVip->setVisible(true);
				m_pBtnVipContinue->setVisible(false);
			}
		});
		addChild(pLayer, 99);
	});

	Utils::addButtonTouchEffect(m_pBtnVipContinue, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		auto pLayer = GiftVipLayer::create();
		addChild(pLayer, 99);
	});

	Utils::addButtonTouchEffect(m_pBtnGiveup, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
	});

	//this->login();

	this->updateUI();

	DEBUG_LOG("MenuDayGift::init finish");

	return true;
}

void MenuDayGift::login(float dt)
{
	//为保证vip起始时间准确性，需联网获取时间
	RemoteLogin* pLogin = new RemoteLogin();
	pLogin->autorelease();
	pLogin->retain();
	pLogin->run([this, pLogin](int result){
		DEBUG_LOG("RemoteLogin, result = %d", result);
		if (result == 0)//成功
		{
			this->updateUI();
		}
		else //失败
		{
			this->scheduleOnce(SEL_SCHEDULE(&MenuDayGift::login), 2);
		}

		pLogin->release();
	});
}

void MenuDayGift::updateUI()
{
	int vipTime = GameData::getInstance()->getVipTime();
	bool bOnline = false;
	bool bVip = false;
	if (vipTime > 0) //联网，且是vip
	{
		bOnline = true;
		bVip = true;
	}
	else if (vipTime == 0) //联网，但非vip
	{
		bOnline = true;
		bVip = false;
	}
	else if (vipTime == -1) //未联网  是vip
	{
		bOnline = false;
		bVip = true;
	}
	else// if (vipTime < -1)//未联网，非vip
	{
		bOnline = false;
		bVip = false;
	}

	if (bVip)
	{
		m_pBtnVip->setVisible(false);
		m_pBtnVipContinue->setVisible(true);
	}
	else
	{
		m_pBtnVip->setVisible(true);
		m_pBtnVipContinue->setVisible(false);
	}

	if (bOnline)
	{
		m_pBtnReceive->setEnabled(true);
		m_pBtnVip->setEnabled(true);
		m_pBtnVipContinue->setEnabled(true);
		m_pBtnGiveup->setVisible(false);

	}
	else
	{
		m_pBtnReceive->setEnabled(false);
		m_pBtnVip->setEnabled(false);
		m_pBtnVipContinue->setEnabled(false);
		m_pBtnGiveup->setVisible(true);
	}
}

//把获得的礼物值传给data
void MenuDayGift::SetDataVal()
{
	bool bAircraft7PieceFlag = false;
	bool bFlag = false;
	const std::vector<TDayGiftData*>& array = GlobalData::getInstance()->getDayGiftArray();
	int maxDay = array.at(array.size() - 1)->day;
	m_day = MIN(m_day, maxDay);
	int vipTime = GameData::getInstance()->getVipTime();
	int iGiftRatio = 1;

	if (vipTime)
	{
		m_prompt = GlobalData::getInstance()->getParameter(MenuDayGiftConstant::STRING_DAY_GIFT_PROMPT_VIP);
		iGiftRatio = VipConstant::DailyGiftRatio; //vip双倍奖励
	}
	else
	{
		m_prompt = GlobalData::getInstance()->getParameter(MenuDayGiftConstant::STRING_DAY_GIFT_PROMPT);
		iGiftRatio = 1;
	}

	for (auto p : array)
	{
		if (p->day == m_day)
		{
			int cnt = 0;
			if (!strcmp(p->gift.c_str(), EquipPiece::Aircraft7)) //战机7碎片
			{
				float random = Utils::getRandom();
				cnt = int(p->count * random + 1) * iGiftRatio;
				if (cnt <= 0)
				{
					cnt = iGiftRatio;
				}
				else if (cnt > p->count * iGiftRatio)
				{
					cnt = p->count * iGiftRatio;
				}

				const std::vector<TEquipmentData*>& equipArray = GlobalData::getInstance()->getEquipmentArray(EquipTypeAircraft);
				auto pEquipment = equipArray.at(equipArray.size() - 1);
				pEquipment->pick += cnt;

				bAircraft7PieceFlag = true;
			}
			else if (!strcmp(p->gift.c_str(), EquipPiece::NoAircraft)) //其他装备碎片
			{
				//TODO 暂不支持

			}
			else //其他道具
			{
				cnt = p->count * iGiftRatio;
				GameData::getInstance()->incValue(p->gift.c_str(), cnt);
			}

			if (!bFlag)
			{
				char buf[21] = { 0 };
				snprintf(buf, 20, "%d", cnt);
				m_prompt.append(p->giftName).append("x").append(buf);
				bFlag = true;
			}
			else
			{
				char buf[21] = { 0 };
				snprintf(buf, 20, "%d", cnt);
				m_prompt.append(",").append(p->giftName).append("x").append(buf);
			}
		}
	}

	if (bAircraft7PieceFlag)
	{
		m_prompt.append(GlobalData::getInstance()->getParameter(MenuDayGiftConstant::STRING_GUIDE_PIECE_AIRCRAFT7_1));
	}

	GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LAST_LOGIN, m_now);
	GameData::getInstance()->setValue(GAMEDATA::DAY_REWARD_LOGIN_DAYS, m_day);

	DEBUG_LOG("MenuDayGift[day = %d, max = %d]: %s", m_day, maxDay, m_prompt.c_str());

	GameData::getInstance()->saveData();
}

