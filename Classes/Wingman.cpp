#include "Wingman.h"
#include "GlobalData.h"
#include "Equipment.h"
#include "PlaneLayer.h"
#include "Reinforce.h"
#include "Bullet.h"
#include "Player.h"

//***************************************************
//
//   Wingman
//   僚机
//
//***************************************************

bool Wingman::init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData)
{
	bool bRet = false;
	do
	{
		//auto pEquipWingman = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeWingman);
		//if (pEquipWingman && pEquipWingman->grade > 0)
		//{
		//	//强化级别
		//	auto array = GlobalData::getInstance()->getReinforce(pEquipWingman->reinforce_id);
		//	TReinforceGradeData* pReinforceGradeData = array.at(pEquipWingman->grade - 1);
		//	setAttackAdjust(pReinforceGradeData->value);
		//	setBulletSpeedAdjust(pReinforceGradeData->value);
		//}

		//CC_BREAK_IF(!Aircraft::init(parent, pData, pLevelData));
		CC_BREAK_IF(!Node::init());

		parent->addChild(this);
		m_pDataCopy = pData;
		m_data.clone(*pData);
		m_data.setLevelData(pLevelData);

		if (m_data.scale <= 0.001)
		{
			m_data.scale = 1;
		}

		//飞机图片动画
		GameObject::initSpriteWithFileList(m_data.styleArray, m_data.aniDura);
		GameObject::initArmature(m_data.armatureName, m_data.scale);

		//攻击
		PlayerPlane* pParent = dynamic_cast<PlayerPlane*>(getParent());
		if (pParent)
		{
			m_pEquipAircraft = PlayerEquip::getInstance()->getEquip(pParent->getId(), EquipTypeWingman);
			if (m_pEquipAircraft && m_pEquipAircraft->grade > 0)
			{
				//强化级别
				auto array = GlobalData::getInstance()->getReinforce(m_pEquipAircraft->reinforce_id);
				TReinforceGradeData* pReinforceGradeData = array.at(m_pEquipAircraft->grade - 1);
				setAttackAdjust(pReinforceGradeData->value);
				//setBulletSpeedAdjust(pReinforceGradeData->value);
			}
		}

		CC_BREAK_IF(!initPosition(parent, &m_data));
		CC_BREAK_IF(!initFire(parent, &m_data));
		CC_BREAK_IF(!initBullet(parent, &m_data));

		setScale(m_data.scale);

		//计算飞机各项属性
		calculateAttr();

		m_eAircraftType = EAircraftType::Type_Wingman;
		this->setLocalZOrder(CONSTANT::ZORDER_WINGMAN);

		m_armatureMap.insert(std::map<int, std::string>::value_type(WingmanFlyStatus::Normal, "Wait_1"));

		m_armatureMap.insert(std::map<int, std::string>::value_type(WingmanFlyStatus::TurnRampage, "Transform_1"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(WingmanFlyStatus::Rampage, "Wait_2"));
		m_armatureMap.insert(std::map<int, std::string>::value_type(WingmanFlyStatus::RampageRecover, "Transform_2"));

		m_flyStatus = WingmanFlyStatus::Normal;

		bRet = true;
	} while (0);

	return bRet;
}

void Wingman::setLeft(const Vec2& offset)
{
	Vec2 v(-abs(m_data.startPosX) + offset.x, m_data.startPosY + offset.y);
	setPosition(v);
}

void Wingman::setRight(const Vec2& offset)
{
	Vec2 v(abs(m_data.startPosX) + offset.x, m_data.startPosY + offset.y);
	setPosition(v);
	if (m_pArmature)
	{
		setScaleX(-1);
	}
	else
	{
		setFlippedX(true);
	}
}

bool Wingman::initBullet(Node* parent, const TAircraftData* pData)
{
	m_iCurBulletGrade = m_iBulletGrade;

	if (m_data.bulletIdArray.size() <= 0)
	{
		CCASSERT(NULL, "Aircraft::initBullet : m_data.bulletNameArray.size() <= 0");
		return false;
	}

	bool isShooting = this->isShooting();
	stopShoot();

	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		m_bulletGroupArray.at(i)->setPlane(NULL);
	}
	m_bulletGroupArray.clear();

	if (0 == m_iCurBulletGrade)
	{
		m_iAttrAttack = 0;
	}

	for (unsigned i = 0; i < m_data.bulletIdArray.at(m_iBulletGrade)->size(); i++)
	{
		int bulletId = m_data.bulletIdArray.at(m_iBulletGrade)->at(i);

		const TBulletData* pData = GlobalData::getInstance()->getBulletData(bulletId);
		if (NULL == pData)
		{
			DEBUG_LOG("Can't find bullet data[%d]", bulletId);
			continue;
		}

		TBulletData data(*pData);
		data.attack = (int)((data.attack * (1 + m_fAttackAdjust)) + 0.5);

		BulletGroup* bulletGroup = BulletGroupCreator::create(getParent()->getParent(), this, &data);
		m_bulletGroupArray.pushBack(bulletGroup);

		PlaneLayer* pPlaneLayer = dynamic_cast<PlaneLayer*>(getParent()->getParent());
		if (pPlaneLayer != NULL)
		{
			pPlaneLayer->addBullet(bulletGroup);
		}

		//计算飞机的攻击
		if (0 == m_iCurBulletGrade)
		{
			m_iAttrAttack += data.getMaxAttack();
		}

	}

	if (isShooting)
	{
		startShoot();
	}

	return true;
}


void Wingman::playArmature(int status, int endStatus)
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

void Wingman::toRampage()
{
	if (WingmanFlyStatus::Rampage != m_flyStatus)
	{
		playArmature(WingmanFlyStatus::TurnRampage, WingmanFlyStatus::Rampage);
	}
}

void Wingman::toNormal()
{
	if (WingmanFlyStatus::Rampage == m_flyStatus || WingmanFlyStatus::TurnRampage == m_flyStatus)
	{
		playArmature(WingmanFlyStatus::RampageRecover, WingmanFlyStatus::Normal);
	}
}

bool Wingman::upgradeBullet()
{
	bool result = Aircraft::upgradeBullet();

	if (Aircraft::isMaxLevelBullet())
	{
		toRampage();
	}

	return result;
}

bool Wingman::downgradeBullet()
{
	bool result = Aircraft::downgradeBullet();

	if (!Aircraft::isMaxLevelBullet())
	{
		toNormal();
	}

	return result;
}

bool Wingman::resetBulletLevel()
{
	toNormal();

	return Aircraft::resetBulletLevel();
}

void Wingman::autoChangeBulletGrade()
{
	if (!this->isShooting())
	{
		return;
	}

	if (this->isMaxLevelBullet())
	{
		m_iCurBulletGrade = 0;
		toNormal();
	}
	else
	{
		m_iCurBulletGrade = m_data.bulletIdArray.size() - 1;
		toRampage();
	}

	Aircraft::setBulletGrade(m_iCurBulletGrade);
	this->initBullet(getParent(), &m_data);
}