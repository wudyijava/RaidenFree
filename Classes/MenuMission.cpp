#include "MenuMission.h"
#include "MenuMainUI.h"
#include "MenuBag.h"
#include "FakeLoadingLayer.h"
#include "GameData.h"
#include "Sound.h"
#include "Achievement.h"
#include "GiftLayer.h"
#include "GiftDuoLayer.h"
#include "GlobalData.h"
#include "Level.h"
#include "GeneralDialog.h"
#include "Preload.h"
#include "Difficulty.h"
#include "Common/SpriteWithHue.h"
#include "Common/Macro.h"
#include "Guide.h"

namespace MenuMissionConstant
{

};

Scene* MenuMission::scene()
{
	Scene *scene = Scene::create();
	MenuMission *layer = MenuMission::create();
	scene->addChild(layer, 1);

	return scene;
}

MenuMission::MenuMission()
: m_pRootNode(nullptr)
, m_pScrollView(nullptr)

, m_pCbDifficulty_1(nullptr)
, m_pCbDifficulty_2(nullptr)
, m_pCbDifficulty_3(nullptr)

, m_pBtnReturn(nullptr)
, m_pBtnWarehouse(nullptr)

, m_pSpritePlayer(nullptr)

, m_pKeyboardListener(nullptr)

, m_levelMax(1)
, m_levelFinish(0)

, m_bInit(false)
{
}

MenuMission::~MenuMission()
{
	if (m_pSpritePlayer)
	{
		m_pSpritePlayer->release();
	}
}

bool MenuMission::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//音乐
	Sound::playSound(SOUND::ID_MENU_GB, true);

	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("Mission.csb");
	addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("Mission.csb");
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, false);

	//控件
	FIND_UI_CONTROL_RE(ui::ScrollView*, "ScrollView_1", m_pScrollView, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::CheckBox*, "Lv_Eazy_Btn", m_pCbDifficulty_1, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::CheckBox*, "Lv_Normal_Btn", m_pCbDifficulty_2, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::CheckBox*, "Lv_Hard_Btn", m_pCbDifficulty_3, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Back_Btn", m_pBtnReturn, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Com_Bag_Btn", m_pBtnWarehouse, m_pRootNode);

	char tmp1[21] = { 0 };
	char tmp2[21] = { 0 };
	Node* pGuideNode = nullptr;
	for (int i = 0; i < CONSTANT::TOTAL_MISSION; i++)
	{
		snprintf(tmp1, 20, "Stage_Normal%d", i + 1);

		MissionNode node;
		FIND_UI_CONTROL_RE(ui::ImageView*, tmp1, node.bg, m_pRootNode);
		FIND_UI_CONTROL_RE(Sprite*, "Stage_Star1", node.star1, node.bg);
		FIND_UI_CONTROL_RE(Sprite*, "Stage_Star2", node.star2, node.bg);
		FIND_UI_CONTROL_RE(Sprite*, "Stage_Star3", node.star3, node.bg);
		node.num = (ui::TextAtlas*)(node.bg->getChildByName("Stage_Num"));

		if (0 == i)
		{
			node.line = nullptr;

			//新手引导
			pGuideNode = node.bg;
		}
		else
		{
			snprintf(tmp2, 20, "Stage_Line%d", i);
			FIND_UI_CONTROL_RE(Sprite*, tmp2, node.line, m_pRootNode);
		}

		m_missionArray.push_back(node);
	}

	//控件事件
	addButtonTouchEffect(m_pBtnReturn, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
	});

	addButtonTouchEffect(m_pBtnWarehouse, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::MenuBag);

		this->removeKeyboardListener();

		auto pLayer = MenuBag::create();
		pLayer->setOnExitCallback([this](){
			this->addKeyboardListener();
		});

		addChild(pLayer);
	});

	Utils::addButtonTouchEffect(m_pCbDifficulty_1, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_LEVEL, 1);
		this->updateDifficulty(1);
		this->updateLevelStar();
		this->updateBg();
	});

	Utils::addButtonTouchEffect(m_pCbDifficulty_2, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::Difficulty2);

		if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) < Difficulty::Difficulty2Open)
		{
			this->addChild(GeneralDialogCreator::difficulty2Forbidden());
			m_pCbDifficulty_2->setSelected(false);
		}
		else
		{
			GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_LEVEL, 2);
			this->updateDifficulty(2);
			this->updateLevelStar();
			this->updateBg();
		}
	});

	Utils::addButtonTouchEffect(m_pCbDifficulty_3, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::Difficulty3);

		if (GameData::getInstance()->getLevelFinish(Difficulty::Easy) < Difficulty::Difficulty3Open)
		{
			this->addChild(GeneralDialogCreator::difficulty3Forbidden());
			m_pCbDifficulty_3->setSelected(false);
		}
		else
		{
			GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_LEVEL, 3);
			this->updateDifficulty(3);
			this->updateLevelStar();
			this->updateBg();
		}
	});

	//控件属性
	int difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	if (difficulty < 1 || difficulty > 3)
	{
		difficulty = 1;
		GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_LEVEL, difficulty);
	}

	updateDifficulty(difficulty);
	updateLevelStar();
	updateBg();

	this->addKeyboardListener();
	m_bInit = true;

	if (pGuideNode)
	{
		GuideMgr::getInstance()->show(GuideType::WelcomeClickMissionOne, this, pGuideNode);
	}

	return true;
}

void MenuMission::onEnter()
{
	Layer::onEnter();

	auto pGuide = GuideMgr::getInstance()->show(GuideType::MenuBag, this, m_pBtnWarehouse);

	if (!pGuide)
	{
		pGuide = GuideMgr::getInstance()->show(GuideType::Difficulty2, this, m_pCbDifficulty_2);
	}

	if (!pGuide)
	{
		pGuide = GuideMgr::getInstance()->show(GuideType::Difficulty3, this, m_pCbDifficulty_3);
	}

	int cnt = AchievementMgr::getInstance()->judgeAchievement();
}

void MenuMission::addKeyboardListener()
{
	//按返回键退出
	m_pKeyboardListener = EventListenerKeyboard::create();
	m_pKeyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == key
			|| EventKeyboard::KeyCode::KEY_ESCAPE == key
			|| EventKeyboard::KeyCode::KEY_BACKSPACE == key)
		{
			this->ToPlaneMenu();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pKeyboardListener, this);
}

void MenuMission::removeKeyboardListener()
{
	_eventDispatcher->removeEventListener(m_pKeyboardListener);
}

void MenuMission::updateDifficulty(int difficulty)
{
	if (1 == difficulty)
	{
		m_pCbDifficulty_1->setSelected(true);
		m_pCbDifficulty_2->setSelected(false);
		m_pCbDifficulty_3->setSelected(false);
	}
	else if (2 == difficulty)
	{
		m_pCbDifficulty_1->setSelected(false);
		m_pCbDifficulty_2->setSelected(true);
		m_pCbDifficulty_3->setSelected(false);
	}
	else //(3 == difficulty)
	{
		m_pCbDifficulty_1->setSelected(false);
		m_pCbDifficulty_2->setSelected(false);
		m_pCbDifficulty_3->setSelected(true);
	}
}

void MenuMission::updateLevelStar()
{
	//根据不同的难度获取不同的maxLevel和finishLevel
	m_levelMax = GameData::getInstance()->getLevelMax();
	m_levelFinish = GameData::getInstance()->getLevelFinish();

	//当前光卡：飞机图标
	if (!m_pSpritePlayer)
	{
		m_pSpritePlayer = Sprite::createWithSpriteFrameName("Stage_Player.png");
		m_pSpritePlayer->retain();
	}
	if (m_pSpritePlayer->getParent())
	{
		m_pSpritePlayer->removeFromParent();
	}
	m_missionArray[m_levelMax - 1].bg->addChild(m_pSpritePlayer);
	m_pSpritePlayer->setPosition(Vec2(m_missionArray[m_levelMax - 1].bg->getContentSize() / 2) + Vec2(0, 20));

	if (m_levelMax <= 8)
	{
		if (m_bInit)
		{
			m_pScrollView->scrollToBottom(0.3f, true);
		}
		else
		{
			m_pScrollView->jumpToBottom();
		}
	}
	else if (m_levelMax >= CONSTANT::TOTAL_MISSION - 8)
	{
		if (m_bInit)
		{
			m_pScrollView->scrollToTop(0.3f, true);
		}
		else
		{
			m_pScrollView->jumpToTop();
		}
	}
	else
	{
		float h1 = m_missionArray[m_levelMax - 1].bg->getPosition().y;
		float h2 = m_pScrollView->getInnerContainerSize().height;
		if (m_bInit)
		{
			m_pScrollView->scrollToPercentVertical(100 - h1 * 100.f / h2, 0.3f, true);
		}
		else
		{
			m_pScrollView->jumpToPercentVertical(100 - h1 * 100.f / h2);
		}
	}

	for (unsigned i = 0; i < m_missionArray.size(); i++)
	{
		//点击响应
		addButtonTouchEffect(m_missionArray[i].bg, Utils::ButtonTouchEffect::NONE,
			[this, i](Ref* pSender, ui::Widget::TouchEventType type)
		{
			if (i < m_levelMax)
			{
				Sound::playSound(SOUND::ID_CLICK);
				if (i + 1 >= 25)
				{
					this->missionForbidden();
				}
				else
				{
					GameData::getInstance()->setValue(GAMEDATA::LEVEL, (int)i + 1);
					this->ToGame();
				}
			}
		});

		const TLevelData* pLevelData = GlobalData::getInstance()->getLevelData(i + 1);
		if (i < m_levelMax)
		{
			switch (pLevelData->bossType)
			{
			case BossTypeNone:
				m_missionArray[i].bg->loadTexture("Stage_On_Img.png", ui::TextureResType::PLIST);
				break;
			case BossTypeSmall:
				m_missionArray[i].bg->loadTexture("Stage_Npc_On_Img.png", ui::TextureResType::PLIST);
				break;
			case BossTypeBig:
				m_missionArray[i].bg->loadTexture("Stage_Boss_On_Img.png", ui::TextureResType::PLIST);
				break;
			default:
				break;
			}

			if (0 != i)
			{
				m_missionArray[i].line->setSpriteFrame("Stage_Line_On.png");
			}
		}
		else
		{
			switch (pLevelData->bossType)
			{
			case BossTypeNone:
				m_missionArray[i].bg->loadTexture("Stage_Off_Img.png", ui::TextureResType::PLIST);
				break;
			case BossTypeSmall:
				m_missionArray[i].bg->loadTexture("Stage_Npc_Off_Img.png", ui::TextureResType::PLIST);
				break;
			case BossTypeBig:
				m_missionArray[i].bg->loadTexture("Stage_Boss_Off_Img.png", ui::TextureResType::PLIST);
				break;
			default:
				break;
			}

			if (0 != i)
			{
				m_missionArray[i].line->setSpriteFrame("Stage_Line_Off.png");
			}
		}

		//已经通过的关卡
		if (i < m_levelFinish)
		{
			//显示过关评级  grade取值为1，2，3，表示1级、2级、3级
			//char szLevelGradeKey[50 + 1] = { 0 };
			//snprintf(szLevelGradeKey, 50, "%s%d", GAMEDATA::LEVEL_GRADE, i + 1);
			int difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
			int grade = GameData::getInstance()->getLevelStar(difficulty, i + 1);

			if (m_missionArray[i].num)
			{
				m_missionArray[i].num->setVisible(true);
			}
			m_missionArray[i].star1->setVisible(true);
			m_missionArray[i].star2->setVisible(true);
			m_missionArray[i].star3->setVisible(true);

			if (grade < 1)
			{
				m_missionArray[i].star1->setSpriteFrame("Stage_Star_Dark.png");
				m_missionArray[i].star2->setSpriteFrame("Stage_Star_Dark.png");
				m_missionArray[i].star3->setSpriteFrame("Stage_Star_Dark.png");
			}
			else if (1 == grade)
			{
				m_missionArray[i].star1->setSpriteFrame("Stage_Star_Light.png");
				m_missionArray[i].star2->setSpriteFrame("Stage_Star_Dark.png");
				m_missionArray[i].star3->setSpriteFrame("Stage_Star_Dark.png");
			}
			else if (2 == grade)
			{
				m_missionArray[i].star1->setSpriteFrame("Stage_Star_Light.png");
				m_missionArray[i].star2->setSpriteFrame("Stage_Star_Light.png");
				m_missionArray[i].star3->setSpriteFrame("Stage_Star_Dark.png");
			}
			else //grade >= 3
			{
				m_missionArray[i].star1->setSpriteFrame("Stage_Star_Light.png");
				m_missionArray[i].star2->setSpriteFrame("Stage_Star_Light.png");
				m_missionArray[i].star3->setSpriteFrame("Stage_Star_Light.png");
			}
		}
		//可以玩，但没通过的关卡
		else if (i >= m_levelFinish && i < m_levelMax)
		{
			if (m_missionArray[i].num)
			{
				m_missionArray[i].num->setVisible(false);
			}
			m_missionArray[i].star1->setVisible(false);
			m_missionArray[i].star2->setVisible(false);
			m_missionArray[i].star3->setVisible(false);

			//TODO 放些动画提示用户点击进入

		}
		//还没开放的关卡
		else
		{
			if (m_missionArray[i].num)
			{
				m_missionArray[i].num->setVisible(false);
			}
			m_missionArray[i].star1->setVisible(false);
			m_missionArray[i].star2->setVisible(false);
			m_missionArray[i].star3->setVisible(false);
		}
	}
}

void MenuMission::updateBg()
{
	if (m_BgArray.size() == 0)
	{
		for (int i = 1; i <= 3; i++)
		{
			Sprite* pBg = nullptr;
			char buf[50] = {0};
			snprintf(buf, 50 - 1, "Mission_Map%d", i);
			FIND_UI_CONTROL_RE(Sprite*, buf, pBg, m_pRootNode);

			auto pBgNew = SpriteWithHue::createWithSpriteFrame(pBg->getSpriteFrame());
			pBgNew->setPosition(pBg->getPosition());
			pBgNew->setLocalZOrder(pBg->getLocalZOrder() - 1);
			pBg->getParent()->addChild(pBgNew);

			pBg->removeFromParent();
			m_BgArray.push_back(pBgNew);
		}
	}

	int difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	int hue = 0;
	switch (difficulty)
	{
	case 1: hue = 0; break;
	case 2: hue = 1; break;
	case 3: hue = 2; break;
	default: hue = 0; break;
	}

	for (auto p : m_BgArray)
	{
		p->setHue(hue);
	}
}

void MenuMission::ToGame()
{
	Sound::playSound(SOUND::ID_CLICK);

	//第2关推荐新手礼包
	if (GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 2)
	{
		if (!GameData::getInstance()->getValueToInt(GAMEDATA::NEW_GIFT))
		{
			GiftLayer* pLayer = GiftLayer::create(GoodsType::NEW_GIFT);
			pLayer->setOnExitCallback([]()
			{
				//返回时进入游戏
				Director::getInstance()->replaceScene(FakeLoadingLayer::scene());
			});
			addChild(pLayer, 999);

			GameData::getInstance()->setValue(GAMEDATA::NEW_GIFT_RECOMMEND, 1);

			return;
		}
	}
	//第3关推荐土豪金礼包
	//第5关推荐超土豪礼包
	//第8关推荐超土豪礼包
	else if (GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 5
		|| GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 8
		|| GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 13
		|| GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 16
		|| GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 24
		)
	{
		//超土豪礼包
		if (!GameData::getInstance()->getValueToInt(GAMEDATA::HUG_GIFT))
		{
			//std::vector<const char*> goodsArray;
			//goodsArray.push_back(GoodsType::HUG_GIFT);
			//goodsArray.push_back(GoodsType::BIG_GIFT);
			//auto pLayer = GiftDuoLayer::create(goodsArray);

			auto pLayer = GiftLayer::create(GoodsType::HUG_GIFT);
			pLayer->setOnExitCallback([]()
			{
				//返回时进入游戏
				Director::getInstance()->replaceScene(FakeLoadingLayer::scene());
			});
			addChild(pLayer, 999);

			GameData::getInstance()->setValue(GAMEDATA::HUG_GIFT_RECOMMEND, 1);

			return;
		}
		else if (!GameData::getInstance()->getValueToInt(GAMEDATA::BIG_GIFT))
		{
			//std::vector<const char*> goodsArray;
			//goodsArray.push_back(GoodsType::HUG_GIFT);
			//goodsArray.push_back(GoodsType::BIG_GIFT);
			//auto pLayer = GiftDuoLayer::create(goodsArray);

			auto pLayer = GiftLayer::create(GoodsType::BIG_GIFT);
			pLayer->setOnExitCallback([]()
			{
				//返回时进入游戏
				Director::getInstance()->replaceScene(FakeLoadingLayer::scene());
			});
			addChild(pLayer, 999);

			GameData::getInstance()->setValue(GAMEDATA::BIG_GIFT_RECOMMEND, 1);

			return;
		}
		else if (!GameData::getInstance()->getValueToInt(GAMEDATA::MID_GIFT))
		{
			GiftLayer* pLayer = GiftLayer::create(GoodsType::MID_GIFT);
			pLayer->setOnExitCallback([]()
			{
				//返回时进入游戏
				Director::getInstance()->replaceScene(FakeLoadingLayer::scene());
			});
			addChild(pLayer, 999);

			GameData::getInstance()->setValue(GAMEDATA::MID_GIFT_RECOMMEND, 1);

			return;
		}
	}

	//直接进入游戏
	Director::getInstance()->replaceScene(FakeLoadingLayer::scene());
}

void MenuMission::ToShop()
{
	Sound::playSound(SOUND::ID_CLICK);

	//Director::getInstance()->pushScene(TransitionCrossFade::create(.5f, MenuShopLayer::scene()));
}

void MenuMission::ToPlaneMenu()
{
	Sound::playSound(SOUND::ID_CLICK);

	Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
}

void MenuMission::missionForbidden()
{
	this->addChild(GeneralDialogCreator::missionForbidden());
}