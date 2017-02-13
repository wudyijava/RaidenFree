#include "MenuReward.h"
#include "GameData.h"
#include "Sound.h"
#include "Preload.h"
#include "GlobalData.h"
#include "Achievement.h"
#include "Preload.h"
#include "Guide.h"
#include "Common/Macro.h"

MenuReward::MenuReward()
 : m_pRootNode(nullptr)
 , m_pBtnBack(nullptr)

 , m_pTextEnemy(nullptr)
 , m_pTextMoney(nullptr)
 , m_pTextScore(nullptr)

 , m_pScrollView(nullptr)
{

}

Scene* MenuReward::scene()
{
	Scene *scene = Scene::create();
	MenuReward *layer = MenuReward::create();
	scene->addChild(layer, 1);

	return scene;
}

bool MenuReward::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_pRootNode = Preload::getInstance()->getUI("Reward.csb");
	this->addChild(m_pRootNode);

	//加载动画： 
	auto pAimAction = Preload::getInstance()->getUIAction("Reward.csb");;
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, false);

	FIND_UI_CONTROL_RE(ui::Button*, "Back_Btn", m_pBtnBack, m_pRootNode);
	
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Reward_Num1", m_pTextEnemy, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Reward_Num2", m_pTextMoney, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Reward_Num3", m_pTextScore, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::ScrollView*, "ScrollView", m_pScrollView, m_pRootNode);

	Utils::addButtonTouchEffect(m_pBtnBack, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		Director::getInstance()->popScene();
	});

	char szEnemy[10 + 1] = { 0 };
	snprintf(szEnemy, 10, "%d", GameData::getInstance()->getValueToInt(GAMEDATA::ENEMY));
	m_pTextEnemy->setString(szEnemy);

	char szMoney[10 + 1] = { 0 };
	snprintf(szMoney, 10, "%d", GameData::getInstance()->getValueToInt(GAMEDATA::MONEY));
	m_pTextMoney->setString(szMoney);

	char szScore[10 + 1] = { 0 };
	snprintf(szScore, 10, "%d", GameData::getInstance()->getValueToInt(GAMEDATA::SCORE));
	m_pTextScore->setString(szScore);

	m_pScrollView->jumpToTop();

	this->updateList();

	if (!GameData::getInstance()->checkGuide(GuideType::MenuReward))
	{
		GameData::getInstance()->addGuide(GuideType::MenuReward);
	}

	return true;
}

void MenuReward::updateList()
{
	m_pScrollView->removeAllChildren();

	const std::map<int, TAchievementData*>& array = GlobalData::getInstance()->getAllAchievementData();
	for (unsigned i = 0; i < array.size(); i++)
	{
		TAchievementData* pData = array.at(i + 1);

		auto node = Preload::getInstance()->getUI("RewardItem.csb");
		m_pScrollView->addChild(node);

		auto a = m_pScrollView->getInnerContainerSize();

		Sprite* p = (Sprite*)node->getChildByName("Reward_Screen_Little_Bg");
		auto itemSize = p->getContentSize() - Size(0, 24); //去掉每个item上下的空隙
		node->setContentSize(itemSize);

		m_pScrollView->setInnerContainerSize(Size(
			m_pScrollView->getInnerContainerSize().width,
			itemSize.height * array.size() + 60
			));

		node->setPosition(m_pScrollView->getContentSize().width / 2,
			m_pScrollView->getInnerContainerSize().height - itemSize.height * (i + 0.5f));

		ui::Text* pTextTitle = nullptr;
		ui::Text* pTextReward = nullptr;
		ui::Text* pTextRemark = nullptr;
		ui::Button* pBtnReceive = nullptr;
		//Sprite* pSpriteReceived = nullptr;

		FIND_UI_CONTROL_RE(ui::Text*, "Text_Title", pTextTitle, node);
		FIND_UI_CONTROL_RE(ui::Text*, "Text_Reward", pTextReward, node);
		FIND_UI_CONTROL_RE(ui::Text*, "Text_Remark", pTextRemark, node);
		FIND_UI_CONTROL_RE(ui::Button*, "Receive_Btn", pBtnReceive, node);
		//FIND_UI_CONTROL_RE(Sprite*, "", , node);

		//title
		char szTitle[100] = { 0 };
		snprintf(szTitle, 99, "%d.%s", pData->id, pData->name.c_str());
		pTextTitle->setString(szTitle);

		//reward
		char szReward[100] = { 0 };
		snprintf(szReward, 99, "[%sX%d]", pData->rewardName.c_str(), pData->rewardValue);
		pTextReward->setString(szReward);

		//remark
		char szRemark[100] = { 0 };
		snprintf(szRemark, 99, "%s", pData->remark.c_str());
		pTextRemark->setString(szRemark);

		int value = GameData::getInstance()->getValueToInt(pData->key.c_str());
		if (value >= pData->value)
		{
			pData->isAchieve = 1;
		}

		if (0 == pData->isAchieve)
		{
			//未达成
			pBtnReceive->setVisible(false);
		}
		else if (1 == pData->isAchieve && 1 != pData->isReceive)
		{
			//已达成，未领取
			pBtnReceive->setVisible(true);
			pBtnReceive->setEnabled(true);

			Utils::addButtonTouchEffect(pBtnReceive, Utils::ButtonTouchEffect::NONE,
				[this, pData, pBtnReceive/*, pSpriteReceived*/](Ref* pSender, ui::Widget::TouchEventType type)
			{
				Sound::playSound(SOUND::ID_CLICK);

				this->getReward(pData, pBtnReceive);
			});
		}
		else // 1 == pData->isAchieve && 1 == pData->isReceive
		{
			//已达成，已领取
			pBtnReceive->setVisible(true);
			pBtnReceive->setEnabled(false);
		}
	}
}

void MenuReward::getReward(TAchievementData* pData, ui::Button* pBtnReceive)
{
	//界面元素
	pBtnReceive->setEnabled(false);

	//赠送钱、必杀、晶化等
	int result = GameData::getInstance()->incValue(pData->rewardKey.c_str(), pData->rewardValue);
	pData->isReceive = 1;

	//保存到文件
	GameData::getInstance()->saveData();

	//更新
	if (!strcmp(GAMEDATA::MONEY, pData->rewardKey.c_str()))
	{
		char szMoney[10 + 1] = { 0 };
		snprintf(szMoney, 10, "%d", result);
		m_pTextMoney->setString(szMoney);
	}

	//显示提示
	//char szGetReward[100] = { 0 };
	//const std::string& xx = GlobalData::getInstance()->getParameter(GLOBALDATA::STRING_RECEIVED_REWARD);
	//snprintf(szGetReward, 99, "%s%sX%d", xx.c_str(), pData->rewardName.c_str(), pData->rewardValue);
	//m_pLabelReceivedReward->setVisible(true);
	//m_pLabelReceivedReward->setString(szGetReward);

	//m_pLabelReceivedReward->setOpacity(0);
	//m_pLabelReceivedReward->setScale(0.85f);

	//FadeTo* pFade = FadeTo::create(.2f, 255.f);
	//ScaleTo* pSclae = ScaleTo::create(.2f, 1.0f);
	//Spawn* pSpawn = Spawn::create(pFade, pSclae, NULL);
	//DelayTime* pDelay = DelayTime::create(1.0f);
	//FiniteTimeAction* pEnd = CallFunc::create([this]()
	//{
	//	m_pLabelReceivedReward->setVisible(false);
	//});
	//Sequence* pSeq = Sequence::create(pSpawn, pDelay, pEnd, NULL);
	//m_pLabelReceivedReward->runAction(pSeq);
}
