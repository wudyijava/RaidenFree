#include "Reward.h"
#include "Aircraft.h"
#include "Common/Macro.h"
#include "GlobalData.h"
#include "IListener.h"
#include "GameData.h"
#include "Sound.h"
#include "Armature.h"
#include "Preload.h"
#include <math.h>

namespace RewardConstant
{
	const char* const AniFile = "GameUI_Txt.csb";
}

//***************************************************
//工厂类
//***************************************************
Reward* RewardCreator::create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, const char* const rewardType)
{
	const TRewardData* pData = GlobalData::getInstance()->getRewardData(rewardType);
	if (!pData)
	{
		DEBUG_LOG("RewardCreator::create : pData is NULL. Reward type = %s", rewardType);
		CCASSERT(pData, "RewardCreator::create : pData is NULL");
	}

	return create(parent, pPlayerArray, pData);
}

Reward* RewardCreator::create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, int id)
{
	const TRewardData* pData = GlobalData::getInstance()->getRewardData(id);
	CCASSERT(pData, "RewardCreator::create : pData is NULL");

	return create(parent, pPlayerArray, pData);
}

Reward* RewardCreator::create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, const TRewardData* pRewardData)
{
	Reward* pRet = NULL;
	if (!strcmp(REWARD_TYPE_WEAPON, pRewardData->type.c_str()))
	{
		pRet = RewardCreatorTpl<RewardWeapon>::create(parent, pPlayerArray, pRewardData);
	}
	else if (!strcmp(REWARD_TYPE_REPAIR, pRewardData->type.c_str()))
	{
		pRet = RewardCreatorTpl<RewardRepair>::create(parent, pPlayerArray, pRewardData);
	}
	else if (!strcmp(REWARD_TYPE_KILLER, pRewardData->type.c_str()))
	{
		pRet = RewardCreatorTpl<RewardKiller>::create(parent, pPlayerArray, pRewardData);
	}
	else if (!strcmp(REWARD_TYPE_SHIELD, pRewardData->type.c_str()))
	{
		pRet = RewardCreatorTpl<RewardShield>::create(parent, pPlayerArray, pRewardData);
	}
	else if (!strcmp(REWARD_TYPE_RAMPAGE, pRewardData->type.c_str()))
	{
		pRet = RewardCreatorTpl<RewardRampage>::create(parent, pPlayerArray, pRewardData);
	}
	else if (!strcmp(REWARD_TYPE_MONEY, pRewardData->type.c_str()))
	{
		pRet = RewardCreatorTpl<RewardMoney>::create(parent, pPlayerArray, pRewardData);
	}

	//出现的奖励品的分值
	auto it = pRewardData->paramMap.find(REWARD_ATTR_SCORE);
	if (it != pRewardData->paramMap.end())
	{
		GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE, (int)it->second);
	}

	//爆出来的奖励品的个数
	GameData::getInstance()->incValue(GAMEDATA::LEVEL_REWARD_TOTAL, 1);

	return pRet;
}

//***************************************************
//奖励物基类
//***************************************************
Reward::Reward()
{
	reset();
}

Reward::~Reward()
{
}

void Reward::destory()
{

}

void Reward::reset()
{
	m_pPlayerArray = NULL;
	m_data.reset();
	m_pArmature = nullptr;
	m_speedX = 0;
	m_speedY = 0;
	m_time = 0;
}

bool Reward::init(const Node* pParent, const Vector<Aircraft*>* pPlayerArray, const TRewardData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!GameObject::init());

		CCASSERT(pData, "Reward::init pData is NULL");
		CCASSERT(pPlayerArray, "Reward::init pPlayer is NULL");

		m_pPlayerArray = pPlayerArray;
		m_data.clone(*pData);

		if (m_data.armatureName.length() > 0)
		{
			m_pArmature = cocostudio::Armature::create(m_data.armatureName);
			m_pArmature->setPosition(getContentSize() / 2);
			m_pArmature->getAnimation()->play(GlobalData::getInstance()->getArmatureData(m_data.armatureName)->defaultAction);

			addChild(m_pArmature);
		}
		else
		{
			GameObject::initSpriteWithFileList(m_data.styleArray, m_data.aniDura);
		}

		Vec2 center(m_data.bodyCenterX, m_data.bodyCenterY);
		setBodyCenter(center);

		Size size(m_data.bodySizeW, m_data.bodySizeH);
		setBodySize(size);

		bRet = true;
	} while (0);

	return bRet;
}

void Reward::fly(const Size& planeSize)
{
	m_prePos = this->getPosition();

	//开始时间
	m_time = 0;

	//随机角度
	static float ANGLE_ARRAY[] = { 
		CONSTANT::PI / 6 + CONSTANT::PI / 2 * 0,
		CONSTANT::PI / 4 + CONSTANT::PI / 2 * 0,
		CONSTANT::PI / 3 + CONSTANT::PI / 2 * 0,
		CONSTANT::PI / 6 + CONSTANT::PI / 2 * 1, 
		CONSTANT::PI / 4 + CONSTANT::PI / 2 * 1, 
		CONSTANT::PI / 3 + CONSTANT::PI / 2 * 1,
		CONSTANT::PI / 6 + CONSTANT::PI / 2 * 2,
		CONSTANT::PI / 4 + CONSTANT::PI / 2 * 2,
		CONSTANT::PI / 3 + CONSTANT::PI / 2 * 2,
		CONSTANT::PI / 6 + CONSTANT::PI / 2 * 3,
		CONSTANT::PI / 4 + CONSTANT::PI / 2 * 3,
		CONSTANT::PI / 3 + CONSTANT::PI / 2 * 3
	};
	unsigned len = sizeof(ANGLE_ARRAY) / sizeof(ANGLE_ARRAY[0]);
	static unsigned idx = 0;
	idx++;
	if (idx >= len)
	{
		idx = 0;
	}
	float randomAngle = ANGLE_ARRAY[idx];
	m_speedX = m_data.speed * cos(randomAngle);
	m_speedY = m_data.speed * sin(randomAngle);

	//新位置
	float delta = 0.1f;
	if (planeSize.width > 1 && planeSize.height > 1)
	{
		float dd = sqrt(planeSize.width*planeSize.width + planeSize.height + planeSize.height)*0.6*0.5;
		delta = dd / m_data.speed;
	}
	m_curPos.x = m_prePos.x + m_speedX * delta;
	m_curPos.y = m_prePos.y + m_speedY * delta;
	setPosition(m_curPos);

	this->scheduleUpdate();
}

void Reward::update(float delta)
{
	m_time += delta;

	//判断是否已超时
	bool end = false;
	if (m_time >= m_data.lifeTime)
	{
		end = true;
	}

	//判断是否在飞机的引力范围内
	Vec2 dest;
	float distance = 0;
	Aircraft* pPlayer = NULL;
	if (m_pPlayerArray != NULL && m_pPlayerArray->size() > 0 && m_pPlayerArray->at(0) && m_pPlayerArray->at(0)->isAlive())
	{
		pPlayer = m_pPlayerArray->at(0);
		dest = pPlayer->getPosition();
		distance = dest.distance(m_prePos);
	}

	if (pPlayer != NULL && distance <= m_data.radius) //在引力范围内
	{
		float ratio = m_data.maxSpeed * delta / distance;

		m_curPos.y = m_prePos.y + (dest.y - m_prePos.y) * ratio;
		m_curPos.x = m_prePos.x + (dest.x - m_prePos.x) * ratio;
		setPosition(m_curPos);

		m_prePos = m_curPos;

		//吃到奖励物
		if (pPlayer->getBoundingBox().intersectsRect(this->getBodyBox())
			&& getParent() != NULL)
		{
			Sound::playSound(m_data.musicId);

			GameData::getInstance()->incValue(GAMEDATA::LEVEL_REWARD, 1);
			GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE, (int)m_data.paramMap.at(REWARD_ATTR_SCORE));

			if (m_pPlayerArray
				&& m_pPlayerArray->size() > 0
				&& m_pPlayerArray->at(0)
				&& getParent())
			{
				doReward();
				doPrompt();
			}

			this->removeFromParent();
		}
	}
	else //不在引力范围内
	{
		//设置当前位置
		m_curPos.x = m_prePos.x + m_speedX * delta;
		m_curPos.y = m_prePos.y + m_speedY * delta;
		setPosition(m_curPos);

		//判断是否已需要反弹
		const Size& size = getContentSize();
		const Size& windowSize = Director::getInstance()->getWinSize();
		if (!end)
		{

			if (m_curPos.x < 0)
			{
				m_speedX = -1 * m_speedX;
				setPositionX(1.f);
			}
			else if (m_curPos.x > windowSize.width)
			{
				m_speedX = -1 * m_speedX;
				setPositionX(windowSize.width - 1);
			}

			if (m_curPos.y < 0)
			{
				m_speedY = -1 * m_speedY;
				setPositionY(1.f);
			}
			else if (m_curPos.y > windowSize.height)
			{
				m_speedY = -1 * m_speedY;
				setPositionY(windowSize.height - 1);
			}
		}
		else //已超时
		{
			if ((m_curPos.x + size.width / 2 < 0
				|| m_curPos.x - size.width / 2 > windowSize.width
				|| m_curPos.y + size.height / 2 < 0
				|| m_curPos.y - size.height / 2 > windowSize.height)
				&& getParent()
				)
			{
				getParent()->removeChild(this);
			}
		}

		m_prePos = m_curPos;
	}

}

void Reward::doPrompt()
{
	if (m_data.prompt.length() <= 0)
	{
		return;
	}

	Sprite* pPrompt = Sprite::createWithSpriteFrameName(m_data.prompt);
	if (NULL == pPrompt)
	{
		DEBUG_LOG("Reward::doPrompt : pPrompt is NULL");
		return;
	}

	if (m_pPlayerArray != NULL && m_pPlayerArray->size() > 0 && m_pPlayerArray->at(0) && getParent() != NULL)
	{
		Aircraft* pAircraft = m_pPlayerArray->at(0);
		getParent()->addChild(pPrompt, CONSTANT::ZORDER_REWARD);

		//Vec2 src(pAircraft->getContentSize().width / 2, pAircraft->getContentSize().height / 2);
		const Vec2& pos = pAircraft->getPosition();
		Vec2 dest(pos.x, pos.y + pAircraft->getContentSize().height / 2 + 10);
		pPrompt->setPosition(dest);


		FiniteTimeAction* actionMove = CCMoveTo::create(0.7f, dest);
		FiniteTimeAction* actionDone = CallFuncN::create(CC_CALLBACK_0(Reward::doPromptDone, this, pPrompt));

		//开始执行动作
		Sequence* sequenceL = Sequence::create(actionMove, actionDone, NULL);
		pPrompt->runAction(sequenceL);
	}

}

void Reward::doPrompt(const std::string& filename, int startIndex, int endIndex)
{
	Aircraft* pAircraft = m_pPlayerArray->at(0);
	auto pPrompt = Preload::getInstance()->getUI(filename);
	auto pAimAction = Preload::getInstance()->getUIAction(filename);

	getParent()->addChild(pPrompt, CONSTANT::ZORDER_REWARD);

	const Vec2& pos = pAircraft->getPosition();
	Vec2 dest(pos.x, pos.y + pAircraft->getContentSize().height / 2 + 10);
	pPrompt->setPosition(dest);
	pPrompt->runAction(pAimAction);

	pAimAction->gotoFrameAndPlay(startIndex, endIndex, false);
	pAimAction->setLastFrameCallFunc([this, pPrompt](){
		pPrompt->removeFromParent();
	});
}

void Reward::doPromptDone(Sprite* pSprite)
{
	if (pSprite != NULL && pSprite->getParent() != NULL)
	{
		pSprite->getParent()->removeChild(pSprite);
	}
}

//***************************************************
//具体的子类
//***************************************************

void RewardWeapon::doReward()
{
	m_pPlayerArray->at(0)->upgradeBullet();
}

void RewardWeapon::doPrompt()
{
	if (m_pPlayerArray->at(0)->isMaxLevelBullet())
	{
		Reward::doPrompt(RewardConstant::AniFile, 251, 300); //暴走
	}
	else
	{
		Reward::doPrompt(RewardConstant::AniFile, 0, 50);
	}
}

void RewardRampage::doReward()
{
	m_pPlayerArray->at(0)->upgradeMaxBullet();

	RewardWeapon::doPrompt();
}

void RewardRepair::doReward()
{
	if (m_pPlayerArray != NULL && m_pPlayerArray->size() > 0 && m_pPlayerArray->at(0))
	{
		float xx = GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_REPAIR_RATIO);
		float yy = m_pPlayerArray->at(0)->getMaxHp() * MAX(xx, m_data.paramMap.at(REWARD_ATTR_HP));
		m_pPlayerArray->at(0)->recoverHp(yy);
	}
}

void RewardRepair::doPrompt()
{
	Reward::doPrompt(RewardConstant::AniFile, 51, 100);
}

void RewardKiller::doReward()
{
	GameData::getInstance()->incValue(GAMEDATA::KILLER, 1);
}

void RewardKiller::doPrompt()
{
	Reward::doPrompt(RewardConstant::AniFile, 201, 250);
}

void RewardShield::doReward()
{
	GameData::getInstance()->incValue(GAMEDATA::SHIELD, 1);

	m_pPlayerArray->at(0)->addShield();
}

void RewardShield::doPrompt()
{
	Reward::doPrompt(RewardConstant::AniFile, 151, 200);
}

//***************************************************
//具体的子类 : 金币
//飞行机制与其他奖励物不同。
//***************************************************
RewardMoney::RewardMoney()
{
	m_iMoney = 0;
}

void RewardMoney::fly(const Size& planeSize)
{
	m_prePos = this->getPosition();
	m_time = 0;

	//随机角度
	float randomAngle = Utils::getRandom() * CONSTANT::PI * 2;
	m_speedX = m_data.speed * cos(randomAngle);
	m_speedY = m_data.speed * sin(randomAngle);

	//随机位置
	float dd = 50.f;
	if (planeSize.width > 1 && planeSize.height > 1)
	{
		dd = sqrt(planeSize.width*planeSize.width + planeSize.height + planeSize.height)*0.5;
	}

	//散射出去
	Vec2 dest1;
	dest1.x = m_prePos.x + m_speedX * m_data.lifeTime + Utils::getRandom() * dd;
	dest1.y = m_prePos.y + m_speedY * m_data.lifeTime + Utils::getRandom() * dd;
	MoveTo* pMoveTo1 = MoveTo::create(m_data.lifeTime, dest1);

	////新位置。getRandom() * 50表示与敌机之间的距离随机
	//float delta = 0.1f;
	//m_curPos.x = m_prePos.x + m_speedX * delta + getRandom() * 50;
	//m_curPos.y = m_prePos.y + m_speedY * delta + getRandom() * 50;
	//setPosition(m_curPos);

	//竖直向下飞行
	Vec2 dest2(dest1.x, -10);
	MoveTo* pMoveTo2 = MoveTo::create(dest1.y / m_data.maxSpeed, dest2);

	//消失
	auto flyEnd = [this]()
	{
		getParent()->removeChild(this);
	};
	FiniteTimeAction* flyEndFunc = CallFunc::create(flyEnd);
	Sequence* sequence = Sequence::create(pMoveTo1, pMoveTo2, flyEndFunc, NULL);

	this->runAction(sequence);

	this->scheduleUpdate();

}

void RewardMoney::update(float delta)
{
	m_time += delta;

	if (m_pPlayerArray == NULL || m_pPlayerArray->size() == 0 || m_pPlayerArray->at(0) == NULL)
	{
		return;
	}

	Aircraft* pPlayer = m_pPlayerArray->at(0);
	const Vec2& dest = pPlayer->getPosition();
	float distance = dest.distance(this->getPosition());

	//在引力范围内
	if (distance <= m_data.radius && m_time >= m_data.lifeTime)
	{
		float ratio = m_data.maxSpeed * delta / distance;

		m_curPos.y = m_prePos.y + (dest.y - m_prePos.y) * ratio;
		m_curPos.x = m_prePos.x + (dest.x - m_prePos.x) * ratio;
		setPosition(m_curPos);

		m_prePos = m_curPos;

		//吃到奖励物
		if (pPlayer->getBoundingBox().intersectsRect(this->getBodyBox())
			&& getParent() != NULL)
		{
			Sound::playSound(m_data.musicId);

			this->stopAllActions();

			GameData::getInstance()->incValue(GAMEDATA::LEVEL_REWARD, 1);
			GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE, (int)m_data.paramMap.at(REWARD_ATTR_SCORE));

			doReward();
			doPrompt();
			getParent()->removeChild(this);
		}
	}
	else
	{
		m_prePos = this->getPosition();
	}
}

//void RewardMoney::fly()
//{
//	m_prePos = this->getPosition();
//
//	//开始时间
//	m_time = 0;
//
//	//随机角度
//	float randomAngle = getRandom() * CONSTANT::PI * 2;
//	m_speedX = m_data.speed * cos(randomAngle);
//	m_speedY = m_data.speed * sin(randomAngle);
//
//	//新位置。getRandom() * 50表示与敌机之间的距离随机
//	float delta = 0.1f;
//	m_curPos.x = m_prePos.x + m_speedX * delta + getRandom() * 50;
//	m_curPos.y = m_prePos.y + m_speedY * delta + getRandom() * 50;
//	setPosition(m_curPos);
//
//	this->scheduleUpdate();
//}

//void RewardMoney::update(float delta)
//{
//	m_time += delta;
//
//	//判断是否已超时
//	bool timeout = false;
//	if (m_time >= m_data.lifeTime)
//	{
//		timeout = true;
//	}
//
//	if (timeout 
//		&& m_pPlayerArray != NULL 
//		&& m_pPlayerArray->size() > 0
//		&& m_pPlayerArray->at(0))  //直接飞向飞机
//	{
//		Aircraft* pPlayer = m_pPlayerArray->at(0);
//		Vec2 dest = pPlayer->getPosition();
//		float distance = dest.distance(m_prePos);
//		float ratio = m_data.maxSpeed * delta / distance;
//
//		m_curPos.y = m_prePos.y + (dest.y - m_prePos.y) * ratio;
//		m_curPos.x = m_prePos.x + (dest.x - m_prePos.x) * ratio;
//		setPosition(m_curPos);
//
//		m_prePos = m_curPos;
//
//		//吃到奖励物
//		if (pPlayer->getBoundingBox().intersectsRect(this->getBodyBox())
//			&& getParent() != NULL)
//		{
//			doPrompt();
//			doReward();
//			getParent()->removeChild(this);
//		}
//	}
//	else //自由飞行
//	{
//		//设置当前位置
//		m_curPos.x = m_prePos.x + m_speedX * delta;
//		m_curPos.y = m_prePos.y + m_speedY * delta;
//		setPosition(m_curPos);
//
//		m_prePos = m_curPos;
//
//		//判断是否已需要反弹
//		const Size& size = getContentSize();
//		const Size& windowSize = Director::getInstance()->getWinSize();
//		if (m_curPos.x <= 0 || m_curPos.x >= windowSize.width)
//		{
//			m_speedX = -1 * m_speedX;
//		}
//
//		if (m_curPos.y <= 0 || m_curPos.y >= windowSize.height)
//		{
//			m_speedY = -1 * m_speedY;
//		}
//	}
//}

void RewardMoney::setMoney(int money)
{
	m_iMoney = money;
}

void RewardMoney::doReward()
{
	GameData::getInstance()->incValue(GAMEDATA::LEVEL_MONEY, m_iMoney);
}
