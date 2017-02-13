#include "Achievement.h"
#include "GlobalData.h"
#include "GameData.h"
#include "Difficulty.h"
#include "Common/Macro.h"

AchievementMgr* AchievementMgr::m_pInstance = NULL;

AchievementMgr::AchievementMgr()
	: m_bDisplayInfo(false)
{
}

AchievementMgr::~AchievementMgr()
{
}

bool AchievementMgr::init()
{
	const std::map<int, TAchievementData*>& map = GlobalData::getInstance()->getAllAchievementData();
	if (map.size() <= 0)
	{
		return false;
	}

	auto it = map.begin();
	for (; it != map.end(); it++)
	{
		auto it2 = m_achievementMap.find(it->second->key);
		if (it2 != m_achievementMap.end())
		{
			it2->second->push_back(it->second);
		}
		else
		{
			std::vector<TAchievementData*>* pArray = new std::vector<TAchievementData*>();
			pArray->push_back(it->second);
			m_achievementMap.insert(std::map<std::string, std::vector<TAchievementData*>*>::value_type(it->second->key, pArray));
		}
	}

	return true;
}

int AchievementMgr::judgeAchievement()
{
	int cnt = 0;
	const std::map<int, TAchievementData*>& allAchievementMap = GlobalData::getInstance()->getAllAchievementData();
	std::map<int, TAchievementData*>::const_iterator it = allAchievementMap.begin();
	for (; it != allAchievementMap.end(); it++)
	{
		TAchievementData* pData = it->second;
		if (pData->isAchieve != 1) //未达成
		{
			int value = 0;
			if (pData->key == GAMEDATA::LEVEL_FINISH)
			{
				value = GameData::getInstance()->getLevelFinish(Difficulty::Easy);
			}
			else
			{
				value = GameData::getInstance()->getValueToInt(pData->key.c_str());
			}

			if (value >= pData->value)
			{
				pData->isAchieve = 1;
				cnt++;
			}

		}
		else if (pData->isAchieve == 1 && pData->isReceive != 1) //已达成，未领取
		{
			cnt++;
		}
	}

	DEBUG_LOG("Achievement : %d", cnt);

	return cnt;
}

bool AchievementMgr::judgeAchievement(Layer* pLayer, const char* const key)
{
	//然后判断value是否大于等于TAchievementData.value
	int value = GameData::getInstance()->getValueToInt(key);

	auto it = m_achievementMap.find(key);
	if (it == m_achievementMap.end())
	{
		return false;
	}

	std::vector<TAchievementData*>* pArray = it->second;
	for (unsigned i = 0; i < pArray->size(); i++)
	{
		TAchievementData* pData = pArray->at(i);
		if (value >= pData->value && 0 == pData->isAchieve)
		{
			pArray->at(i)->isAchieve = 1;
			m_toDisplayInfo.push_back(pData);

			displayInfo(pLayer);
		}
	}

	return true;
}

bool AchievementMgr::displayInfo(Layer* pLayer)
{
	if (m_toDisplayInfo.size() > 0 && pLayer != NULL && !m_bDisplayInfo)
	{
		m_bDisplayInfo = true;

		TAchievementData* pData = m_toDisplayInfo.front();

		//开始动画
		Sprite* pSprite = Sprite::createWithSpriteFrameName("achievement_bg.png");
		pLayer->addChild(pSprite);
		const Size& size = Director::getInstance()->getWinSize();
		pSprite->setPosition(size.width / 2, size.height + pSprite->getContentSize().height/2);
		pSprite->setGlobalZOrder(CONSTANT::ZORDER_ACHIEVEMENT);

		//文字
		Label* pLable = Label::createWithBMFont("Achievement/achievement.fnt", pData->name);
		pSprite->addChild(pLable);
		pLable->setGlobalZOrder(pSprite->getGlobalZOrder() + 1);
		pLable->setAnchorPoint(Vec2(0, 0));
		pLable->setPosition(80.f, 22.f);

		MoveBy* pDown = MoveBy::create(0.3f * Director::getInstance()->getScheduler()->getTimeScale(), 
			Vec2(0, -pSprite->getContentSize().height));
		DelayTime* pDelay = DelayTime::create(1.0f);
		MoveBy* pUp = pDown->reverse();
		auto end = [pLayer, pSprite, this]()
		{
			pSprite->removeFromParent();

			m_bDisplayInfo = false;
			this->displayInfo(pLayer);
		};
		FiniteTimeAction* pEnd = CallFunc::create(end);
		Sequence* pSeq = Sequence::create(pDown, pDelay, pUp, pEnd, NULL);
		pSprite->runAction(pSeq);
		//
		m_toDisplayInfo.pop_front();
	}

	return true;
}
