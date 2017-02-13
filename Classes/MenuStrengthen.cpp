#include "MenuStrengthen.h"
#include "MenuMainUI.h"
#include "GlobalData.h"
#include "Reinforce.h"
#include "GameData.h"
#include "MenuShop.h"
#include "GiftLayer.h"
#include "GiftDuoLayer.h"
#include "Achievement.h"
#include "Sound.h"
#include "Goods.h"
#include "Preload.h"
#include "Analytics.h"
#include "Guide.h"
#include "Common/Macro.h"

namespace MenuStrengthenConstant
{
	const int GradeCnt = 10;
}

Scene* MenuStrengthen::scene()
{
	Scene *scene = Scene::create();
	MenuStrengthen *layer = MenuStrengthen::create();
	scene->addChild(layer);

	return scene;
}

MenuStrengthen::MenuStrengthen()
: m_pRootNode(nullptr)
, m_pBtnClose(nullptr)
, m_pBtnStrengthen(nullptr)
//分数、宝石
, m_pTextScore(nullptr)
, m_pTextMoney(nullptr)
, m_curItem(0)
{
}

MenuStrengthen::~MenuStrengthen()
{
	GameData::getInstance()->unRegListener(this, GAMEDATA::MONEY);
}

bool MenuStrengthen::doInit()
{
	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("Strengthen.csb");
	addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("Strengthen.csb");;
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, 13, false);
	pAimAction->setLastFrameCallFunc([this, pAimAction](){
		Guide* pGuide = GuideMgr::getInstance()->show(GuideType::MenuStrengthen1, this->getParent(), m_itemArray.at(0).node);
	});

	////控件
	FIND_UI_CONTROL_RE(ui::Button*, "Back_Btn", m_pBtnClose, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Strengthen_Btn", m_pBtnStrengthen, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Text*, "Strengthen_Info", m_pStrengthenInfo, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Text*, "Strengthen_Price", m_pStrengthenPrice, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num1", m_pTextScore, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num2", m_pTextMoney, m_pRootNode);

	m_pStrengthenPrice->setVisible(false);

	m_pStrengthenInfo->setTextAreaSize(Size(450, 200));
	m_pStrengthenInfo->ignoreContentAdaptWithSize(false);
	m_pStrengthenInfo->setAnchorPoint(Vec2(0, 1));

	Utils::addButtonTouchEffect(m_pBtnClose, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);
		
		//Director::getInstance()->popScene();
		this->removeFromParent();
	});

	Utils::addButtonTouchEffect(m_pBtnStrengthen, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::MenuStrengthen2);

		this->doStrengthen(m_curItem);
		this->updateStrengthenGrade(m_curItem);
		this->updateScoreAndMoney();
	});

	char buf1[51] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		snprintf(buf1, 50, "Strengthen_Bg%d", i + 1);

		StrengthenItem item;
		FIND_UI_CONTROL_RE(ui::CheckBox*, buf1, item.node, m_pRootNode);
		FIND_UI_CONTROL_RE(ui::LoadingBar*, "Strengthen_Prog", item.grade, item.node);
		m_itemArray.push_back(item);

		//控件事件
		Utils::addButtonTouchEffect(item.node, [this, i](Ref* pSender, ui::CheckBox::EventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			if (0 == i)
			{
				GuideMgr::getInstance()->destory(GuideType::MenuStrengthen1);
				Guide* pGuide = GuideMgr::getInstance()->show(GuideType::MenuStrengthen2, this->getParent(), m_pBtnStrengthen);
			}

			for (unsigned i = 0; i < m_itemArray.size(); i++)
			{
				if (m_itemArray[i].node == pSender)
				{
					m_itemArray[i].node->setSelected(true);
					m_curItem = i;

					updateStrengthenInfo(m_curItem);
				}
				else
				{
					m_itemArray[i].node->setSelected(false);
				}
			}

		});
	}

	for (unsigned i = 0; i < m_itemArray.size(); i++)
	{
		this->updateStrengthenGrade(i);
	}

	//默认选中中间那个
	m_itemArray[4].node->setSelected(true);
	m_curItem = 4;
	updateStrengthenInfo(m_curItem);

	this->updateScoreAndMoney();

	//按返回键退出
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == key
			|| EventKeyboard::KeyCode::KEY_ESCAPE == key
			|| EventKeyboard::KeyCode::KEY_BACKSPACE == key)
		{
			GameData::getInstance()->saveData();
			this->removeFromParent();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	GameData::getInstance()->regListener(this, GAMEDATA::MONEY);

	if (!GameData::getInstance()->checkGuide(GuideType::MenuStrengthen))
	{
		GameData::getInstance()->addGuide(GuideType::MenuStrengthen);
		GameData::getInstance()->addGuide(GuideType::MenuStrengthen1);
		GameData::getInstance()->addGuide(GuideType::MenuStrengthen2);
	}

	return true;
}

void MenuStrengthen::removeFromParent()
{
	auto pAimAction = Preload::getInstance()->getUIAction("Strengthen.csb");;
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(14, 23, false);
	pAimAction->setLastFrameCallFunc([this](){
		Node::removeFromParent();
	});
}

void MenuStrengthen::onEnter()
{
	Layer::onEnter();

	//TODO 更新宝石数
}

//更新强化等级
void MenuStrengthen::updateStrengthenGrade(int item)
{
	//更新各个强化等级
	m_gradeArray.clear();
	m_gradeArray.push_back(GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_GLOBAL_DIAMOND));
	m_gradeArray.push_back(GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_GLOBAL_RAMPAGE));
	m_gradeArray.push_back(GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_GLOBAL_KILLER));
	m_gradeArray.push_back(GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_GLOBAL_SHIELD));
	m_gradeArray.push_back(GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_GLOBAL_REPAIR));

	for (unsigned i = 0; i < m_gradeArray.size(); i++)
	{
		if (m_gradeArray[i] < 0 || m_gradeArray[i] > MenuStrengthenConstant::GradeCnt)
		{
			m_gradeArray[i] = 0;
		}
	}

	//根据强化等级展示黄/红块
	unsigned grade = (unsigned)m_gradeArray[item];
	float percent = grade * 1.f / MenuStrengthenConstant::GradeCnt * 100;
	m_itemArray[item].grade->setPercent(percent);
}

//更新强化信息
void MenuStrengthen::updateStrengthenInfo(int item)
{
	int grade = m_gradeArray.at(item);
	if (grade >= MenuStrengthenConstant::GradeCnt)
	{
		//已满级
		m_pStrengthenPrice->setVisible(false);
		m_pStrengthenInfo->setString(GlobalData::getInstance()->getParameter(GLOBALDATA::STRING_UPGRADE_WEAPON_MAX_LEVEL));
		m_pBtnStrengthen->setEnabled(false);

		DEBUG_LOG("updateStrengthenInfo[%d] : max grade", item);
	}
	else
	{
		m_pBtnStrengthen->setEnabled(true);

		const TReinforceGradeData* pGradeData = GlobalData::getInstance()->getReinforce(item + 1).at(grade);
		//TODO 宝石图标

		//所需宝石数量
		int money = pGradeData->money;
		char buf[51] = { 0 };
		snprintf(buf, 50, "%d", money);

		//强化内容
		m_pStrengthenPrice->setVisible(true);
		m_pStrengthenPrice->setString(buf);
		m_pStrengthenInfo->setString(pGradeData->remark);
		DEBUG_LOG("updateStrengthenInfo[%d] : money = %d, remark = %s", item, money, pGradeData->remark.c_str());
	}

}

void MenuStrengthen::doStrengthen(int item)
{
	int grade = m_gradeArray[item];
	if (grade >= MenuStrengthenConstant::GradeCnt)
	{
		return;
	}

	const TReinforceGradeData* pGradeData = GlobalData::getInstance()->getReinforce(item + 1).at(grade);
	int moneyCost = pGradeData->money;
	int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);
	if (money >= moneyCost)
	{
		Analytics::getInstance()->use(pGradeData->key.c_str(), 1, moneyCost);
		//减钱
		GameData::getInstance()->incValue(GAMEDATA::MONEY, -moneyCost);

		//等级+1
		++m_gradeArray[item];

		//改数值
		GameData::getInstance()->setValue(pGradeData->key.c_str(), pGradeData->value);

		++grade;
		switch (m_curItem)
		{
		case 0:
			GameData::getInstance()->setValue(GAMEDATA::REINFORCE_GLOBAL_DIAMOND, grade);
			break;
		case 1:
			GameData::getInstance()->setValue(GAMEDATA::REINFORCE_GLOBAL_RAMPAGE, grade);
			break;
		case 2:
			GameData::getInstance()->setValue(GAMEDATA::REINFORCE_GLOBAL_KILLER, grade);
			break;
		case 3:
			GameData::getInstance()->setValue(GAMEDATA::REINFORCE_GLOBAL_SHIELD, grade);
			GameData::getInstance()->incValue(GAMEDATA::SHIELD, 1);
			break;
		case 4:
			GameData::getInstance()->setValue(GAMEDATA::REINFORCE_GLOBAL_REPAIR, grade);
			break;
		default:
			break;
		}

		if (GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_ALL) == 0)
		{
			const std::vector<TReinforceData*>& allData = GlobalData::getInstance()->getAllReinforce();
			int maxLevelCnt = 0;
			for (unsigned i = 0; i < allData.size(); i++)
			{
				const char* const key = allData.at(i)->key.c_str();
				if (MenuStrengthenConstant::GradeCnt == GameData::getInstance()->getValueToInt(key))
				{
					maxLevelCnt++;
				}

				if (m_curItem == i)
				{
					//TODO achievement表中的key字段需根据强化表来修改
					AchievementMgr::getInstance()->judgeAchievement(this, key);
				}
			}

			//所有项都满级
			if (5 == maxLevelCnt)
			{
				GameData::getInstance()->setValue(GAMEDATA::REINFORCE_ALL, 1);
				AchievementMgr::getInstance()->judgeAchievement(this, GAMEDATA::REINFORCE_ALL);
			}
		}

		//成就：第一次使用强化
		if (GameData::getInstance()->getValueToInt(GAMEDATA::REINFORCE_FIRST) == 0)
		{
			GameData::getInstance()->setValue(GAMEDATA::REINFORCE_FIRST, 1);
			AchievementMgr::getInstance()->judgeAchievement(this, GAMEDATA::REINFORCE_FIRST);
		}

		//存盘
		GameData::getInstance()->saveData();

		this->updateStrengthenGrade(m_curItem);
		this->updateStrengthenInfo(m_curItem);

	}
	else
	{
		std::vector<const char*> goodsArray;
		goodsArray.push_back(GoodsType::HUG_GIFT);
		goodsArray.push_back(GoodsType::BIG_GIFT);
		auto pLayer = GiftDuoLayer::create(goodsArray);
		this->addChild(pLayer);
	}

}

void MenuStrengthen::updateScoreAndMoney()
{
	int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);
	int score = GameData::getInstance()->getValueToInt(GAMEDATA::SCORE);

	char strScore[20] = { 0 };
	char strMoney[20] = { 0 };
	sprintf(strScore, "%d", score);
	sprintf(strMoney, "%d", money);

	m_pTextScore->setString(strScore);
	m_pTextMoney->setString(strMoney);
}

void MenuStrengthen::updateValue(const std::string& type, int value)
{
	if (type == GAMEDATA::MONEY)
	{
		updateScoreAndMoney();
	}
}