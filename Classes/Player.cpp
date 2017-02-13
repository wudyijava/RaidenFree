#include "Player.h"
#include "GlobalData.h"
#include "IListener.h"
#include "PlaneLayer.h"
#include "GameData.h"
#include "Common/Shake.h"

//***************************************************
//
//   PlayerPlane
//   玩家飞机类
//
//***************************************************
void PlayerPlane::reset()
{
	Aircraft::reset();

	m_iFrameCnt = 0;
	m_flyStatus = PlayerFlyStatus::Normal;
}

bool PlayerPlane::init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData)
{
	bool bRet = false;
	do
	{
		this->setLocalZOrder(CONSTANT::ZORDER_PLAYER);
		m_eAircraftType = EAircraftType::Type_Player;

		//武器初始等级
		setBulletGrade(0);

		//武器威力强化
		//float bulletAttackAdjust = GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_BULLET_ATTACK);
		//setAttackAdjust(bulletAttackAdjust);

		//初始化战机
		CC_BREAK_IF(!Aircraft::init(parent, pData, pLevelData));

		//生命强化
		//float hpChange = GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_HP_CHANGE);
		//m_data.maxHp = (int)((m_data.maxHp * (1 + hpChange)) + 0.5);
		//m_data.hp = m_data.maxHp;

		//飞机动画
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::Normal, "Wait_1"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::TurnLeft, "Move_L_1"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::Left, "Left"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::LeftRecover, "Move_L_2"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::TurnRight, "Move_R_1"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::Right, "Right"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::RightRecover, "Move_R_2"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::TurnRampage, "Transform_1"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::Rampage, "Wait_2"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(PlayerFlyStatus::RampageRecover, "Transform_2"));
		m_flyStatus = PlayerFlyStatus::Normal;

		//调整飞行状态
		scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

bool PlayerPlane::initHpBar(Node* parent, const TAircraftData* pData)
{
	Aircraft::initHpBar(parent, pData);

	PlaneLayer* layer = dynamic_cast<PlaneLayer*>(parent);
	if (layer != NULL)
	{
		layer->notifyHp(m_data.maxHp);
		layer->notifyMaxHp(m_data.maxHp);
	}

	return true;
}

void PlayerPlane::reduceHp(int hp)
{
	Aircraft::reduceHp(hp);

	PlaneLayer* layer = dynamic_cast<PlaneLayer*>(getParent());
	if (layer != NULL)
	{
		layer->notifyHp(m_data.hp);
	}

	Utils::vibrate(300);
	Shake* shake = Shake::create(0.1f, 5);
	getParent()->runAction(shake);
}

void PlayerPlane::recoverHp(int hp)
{
	Aircraft::recoverHp(hp);

	PlaneLayer* layer = dynamic_cast<PlaneLayer*>(getParent());
	if (layer != NULL)
	{
		layer->notifyHp(m_data.hp);
	}
}


void PlayerPlane::jumpBy(Vec2 offset, bool isWindow)
{
	Vec2 location = getPosition() + offset;

	if (this->isAlive())
	{
		Size winSize = CCDirector::getInstance()->getWinSize();
		Size planeSize = getContentSize();

		//确保飞机在屏幕内部
		if (isWindow)
		{
			if (location.x<planeSize.width / 2)
			{
				location.x = planeSize.width / 2;
			}
			if (location.x>winSize.width - planeSize.width / 2)
			{
				location.x = winSize.width - planeSize.width / 2;
			}
			if (location.y<planeSize.height / 2)
			{
				location.y = planeSize.width / 2 + 10;
			}
			if (location.y>winSize.height - planeSize.height / 2)
			{
				location.y = winSize.height - planeSize.height / 2;
			}

		}
		setPosition(location);

		if (offset.x <= -1)
		{
			m_iFrameCnt = 0;
			toLeft();
		}
		else if (int(offset.x) == 0)
		{
			toMiddle();
		}
		else if (offset.x >= 1)
		{
			m_iFrameCnt = 0;
			toRight();
		}
	}
}

void PlayerPlane::playArmature(int status, int endStatus)
{
	if (!m_pArmature)
	{
		return;
	}

	m_flyStatus = status;

	m_pArmature->getAnimation()->play(m_armatureMap.at(status));

	m_pArmature->getAnimation()->setMovementEventCallFunc(
		[this, status, endStatus](cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID)
		{
			if (MovementEventType::START == movementType)
			{
				return;
			}

			m_pArmature->getAnimation()->play(m_armatureMap.at(endStatus));

			m_flyStatus = endStatus;
			m_pArmature->getAnimation()->setMovementEventCallFunc(nullptr);

		}
	);

}

void PlayerPlane::toLeft()
{
	if (PlayerFlyStatus::Normal == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::TurnLeft, PlayerFlyStatus::Left);
	}
	else if (PlayerFlyStatus::Right == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::RightRecover, PlayerFlyStatus::Normal);
	}
}

void PlayerPlane::toMiddle()
{
	if (PlayerFlyStatus::Right == m_flyStatus || PlayerFlyStatus::TurnRight == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::RightRecover, PlayerFlyStatus::Normal);
	}
	else if (PlayerFlyStatus::Left == m_flyStatus || PlayerFlyStatus::TurnLeft == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::LeftRecover, PlayerFlyStatus::Normal);
	}
}

void PlayerPlane::toRight()
{
	if (PlayerFlyStatus::Normal == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::TurnRight, PlayerFlyStatus::Right);
	}
	else if (PlayerFlyStatus::Left == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::LeftRecover, PlayerFlyStatus::Normal);
	}
}

void PlayerPlane::toRampage()
{
	if (PlayerFlyStatus::Rampage != m_flyStatus)
	{
		playArmature(PlayerFlyStatus::TurnRampage, PlayerFlyStatus::Rampage);
	}
}

void PlayerPlane::toNormal()
{
	if (PlayerFlyStatus::Rampage == m_flyStatus || PlayerFlyStatus::TurnRampage == m_flyStatus)
	{
		playArmature(PlayerFlyStatus::RampageRecover, PlayerFlyStatus::Normal);
	}
}

bool PlayerPlane::upgradeBullet()
{
	bool result = Aircraft::upgradeBullet();

	if (Aircraft::isMaxLevelBullet())
	{
		toRampage();
	}

	return result;
}

bool PlayerPlane::downgradeBullet()
{
	bool result = Aircraft::downgradeBullet();

	if (!Aircraft::isMaxLevelBullet())
	{
		toNormal();
	}

	return result;
}

bool PlayerPlane::resetBulletLevel()
{
	toNormal();

	return Aircraft::resetBulletLevel();
}

void PlayerPlane::autoChangeBulletGrade()
{
	Aircraft::autoChangeBulletGrade();
	if (this->isMaxLevelBullet())
	{
		toRampage();
	}
	else
	{
		toNormal();
	}

	//if (!this->isShooting())
	//{
	//	return;
	//}

	//if (this->isMaxLevelBullet())
	//{
	//	m_iCurBulletGrade = 0;
	//	toNormal();
	//}
	//else
	//{
	//	m_iCurBulletGrade = m_data.bulletIdArray.size() - 1;
	//	toRampage();
	//}

	//setBulletGrade(m_iCurBulletGrade);
	//initBullet(getParent(), &m_data);

	//for (int i = 0; i < m_wingmanArray.size(); i++)
	//{
	//	m_wingmanArray.at(i)->autoChangeBulletGrade();
	//}

	//auto delayDone = [this]()
	//{
	//	autoChangeBulletGrade();
	//};

	//DelayTime* delay = DelayTime::create(2.0f);
	//FiniteTimeAction* end = CallFunc::create(delayDone);
	//Sequence* sequence = Sequence::create(delay, end, NULL);
	//runAction(sequence);
}

void PlayerPlane::update(float dt)
{
	//一定时间内玩家手指没有移动，则返回平飞状态
	if (++m_iFrameCnt > 15 && (PlayerFlyStatus::Right == m_flyStatus || PlayerFlyStatus::Left == m_flyStatus))
	{
		m_iFrameCnt = 0;

		toMiddle();
	}
}
