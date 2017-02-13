#include "Aircraft.h"
#include "PlaneLayer.h"
#include "GlobalData.h"
#include "Enemy.h"
#include "Wingman.h"
#include "Bullet.h"
#include "Sound.h"
#include "GameData.h"
#include "Armature.h"
#include "Equipment.h"
#include "Reinforce.h"
#include "AircraftPool.h"
#include "Common/HpBar.h"

namespace AircraftConstant
{
	const int ACTION_TAG_AUTOSHOOT = 1;
}

Aircraft::Aircraft()
{
	this->reset();
}

Aircraft::~Aircraft()
{
}

void Aircraft::reset()
{
	m_data.reset();
	m_pDataCopy = nullptr;

	m_pEquipAircraft = nullptr;
	m_pEquipArmature = nullptr;
	m_pEquipMissile = nullptr;
	m_pEquipWingman = nullptr;

	m_iBulletGrade = 0;
	m_fAttackAdjust = 0;
	m_fMissileAdjust = 0;
	m_fBulletSpeedAdjust = 0;

	m_fVipRelifeAttrAdjust = 1.0f;

	m_fDefence = -1;
	m_fDefenceDura = 0;
	m_iCurBulletGrade = -1;
	m_bNoDie = false;
	m_otherSideArray = NULL;
	m_eAircraftType = EAircraftType::Type_Unknown;
	m_pDefenceBall = NULL;
	m_pHpBar = NULL;
	m_bulletGroupArray.clear();
	m_wingmanArray.clear();

	m_iAttrAttack = 0;
	m_iAttrArmor = 0;
	m_iAttrMissile = 0;
	m_iAttrWingman = 0;
	m_iAttrTotal = 0;

	m_bRecycled = false;

	m_bAutoShoot = false;
	m_orignBulletGrade = 0;
}

Vector<Aircraft*>* Aircraft::getOtherSidePlane() const
{
	return m_otherSideArray;
}

void Aircraft::setOtherSidePlane(Vector<Aircraft*>* const planes)
{
	m_otherSideArray = planes;
	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		m_bulletGroupArray.at(i)->setOtherSidePlane(planes);
	}

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->setOtherSidePlane(planes);
	}
}

bool Aircraft::initBody(Node* parent, const TAircraftData* pData)
{
	if (m_data.bodySizeW > 0.01 && m_data.bodySizeH > 0.01)
	{
		Vec2 center(m_data.bodyCenterX, m_data.bodyCenterY);
		setBodyCenter(center);

		Size size(m_data.bodySizeW, m_data.bodySizeH);
		setBodySize(size * m_data.scale);
	}
	else
	{
		setBodyCenter(Vec2(0,0));

		setBodySize(getContentSize() * 0.8f * m_data.scale);
	}

	return true;
}

bool Aircraft::initPosition(Node* parent, const TAircraftData* pData)
{
	Vec2 v(m_data.startPosX, m_data.startPosY);
	setPosition(v);

	return true;
}

bool Aircraft::initFire(Node* parent, const TAircraftData* pData)
{
	//有了骨骼就不用单独初始化fire了
	if (m_pArmature)
	{
		return true;
	}

	if (m_data.fireFameNameArray.size() > 0)
	{
		m_data.fireOffsetX = abs(m_data.fireOffsetX);

		if (m_data.fireOffsetX > 0.01)
		{
			addFire(+m_data.fireOffsetX, m_data.fireOffsetY, false);
			addFire(-m_data.fireOffsetX, m_data.fireOffsetY, true);
		}
		else
		{
			addFire(0, m_data.fireOffsetY, false);
		}
	}

	return true;
}

bool Aircraft::initHpBar(Node* parent, const TAircraftData* pData)
{
	if (m_data.hpStyle.length() > 0 && m_data.hpBgStyle.length() > 0)
	{
		std::vector<std::string> hpFiles;
		hpFiles.push_back(m_data.hpBgStyle);
		hpFiles.push_back(m_data.hpStyle);
		m_pHpBar = HpBar::createWithTexture(this, hpFiles, false);
		m_pHpBar->setMaxValue(m_data.maxHp);
		m_pHpBar->setCurValue(m_data.maxHp);
		if (m_pArmature && m_data.hpPosY == 0)
		{
			Vec2 v(0, - m_pArmature->getAnchorPointInPoints().y);
			m_pHpBar->setPosition(v);
		}
		else
		{
			Vec2 v(this->getContentSize().width / 2 + m_data.hpPosX, this->getContentSize().height / 2 + m_data.hpPosY);
			m_pHpBar->setPosition(v);
		}
		m_pHpBar->setScale(1.0f / m_data.scale);
	}

	return true;
}

bool Aircraft::initBullet(Node* parent, const TAircraftData* pData)
{
	m_iCurBulletGrade = m_iBulletGrade;

	if (m_data.bulletIdArray.size() <= 0)
	{
		//CCASSERT(NULL, "Aircraft::initBullet : m_data.bulletNameArray.size() <= 0");
		return true;
	}

	bool isShooting = this->isShooting();
	stopShoot();

	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		m_bulletGroupArray.at(i)->setPlane(NULL);
		//m_bulletGroupArray.at(i)->removeFromParent();
	}
	m_bulletGroupArray.clear();

	if ((int)m_data.bulletIdArray.size() <= m_iCurBulletGrade)
	{
		m_iCurBulletGrade = 0;
	}

	if (0 == m_iCurBulletGrade)
	{
		m_iAttrAttack = 0;
		m_iAttrMissile = 0;
	}

	for (unsigned i = 0; i < m_data.bulletIdArray.at(m_iCurBulletGrade)->size(); i++)
	{
		int bulletId = m_data.bulletIdArray.at(m_iCurBulletGrade)->at(i);

		const TBulletData* pData = GlobalData::getInstance()->getBulletData(bulletId);
		if (NULL == pData)
		{
			DEBUG_LOG("Can't find bullet data[%d]", bulletId);
			continue;
		}

		//战机、僚机武器攻击强化，难度增加攻击加成
		TBulletData data(*pData);
		if (data.type == BULLET_TYPE::MISSILE)
		{
			data.attack = (int)((data.attack * (1 + m_fMissileAdjust)) + 0.5);
		}
		else
		{
			data.attack = (int)((data.attack * (1 + m_fAttackAdjust)) + 0.5);

			//难度增加，子弹速度加快，间隔减小
			//data.speed = (int)((data.speed * (1 + m_fBulletSpeedAdjust)) + 0.5);
			data.interval = data.interval / (1 + m_fBulletSpeedAdjust);
			data.interval_2 = data.interval_2 / (1 + m_fBulletSpeedAdjust);
		}

		BulletGroup* bulletGroup = BulletGroupCreator::create(getParent(), this, &data);
		bulletGroup->regBulletUseUpListener(this);
		m_bulletGroupArray.pushBack(bulletGroup);

		PlaneLayer* pPlaneLayer = dynamic_cast<PlaneLayer*>(getParent());
		if (pPlaneLayer != NULL)
		{
			pPlaneLayer->addBullet(bulletGroup);
		}

		//计算飞机的攻击
		if (0 == m_iCurBulletGrade && data.type != BULLET_TYPE::MISSILE)
		{
			m_iAttrAttack += data.getMaxAttack();
		}

		if (0 == m_iCurBulletGrade && data.type == BULLET_TYPE::MISSILE)
		{
			m_iAttrMissile += data.getMaxAttack();
		}
	}

	if (isShooting)
	{
		startShoot();
	}

	return true;
}

bool Aircraft::upgradeBullet()
{
	m_orignBulletGrade = m_iCurBulletGrade;
	if (m_iCurBulletGrade < (int)m_data.bulletIdArray.size() - 1)
	{
		++m_iCurBulletGrade;
	}

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->upgradeBullet();
	}

	if (this->isMaxLevelBullet() && EAircraftType::Type_Player == m_eAircraftType)
	{
		GameData::getInstance()->setValue(GAMEDATA::USE_MAX_BULLLET, 1);
	}

	this->setBulletGrade(m_iCurBulletGrade);
	return initBullet(getParent(), &m_data);
}

bool Aircraft::upgradeMaxBullet()
{
	m_orignBulletGrade = m_iCurBulletGrade;
	m_iCurBulletGrade = (int)m_data.bulletIdArray.size() - 1;

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->upgradeMaxBullet();
	}

	if (this->isMaxLevelBullet() && EAircraftType::Type_Player == m_eAircraftType)
	{
		GameData::getInstance()->setValue(GAMEDATA::USE_MAX_BULLLET, 1);
	}

	this->setBulletGrade(m_iCurBulletGrade);
	return initBullet(getParent(), &m_data);
}

bool Aircraft::isMaxLevelBullet()
{
	if (m_iCurBulletGrade >= (int)m_data.bulletIdArray.size() - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Aircraft::isMinLevelBullet()
{
	if (m_iCurBulletGrade == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Aircraft::getBulletLevel()
{
	return m_iCurBulletGrade;
}

bool Aircraft::resetBulletLevel()
{
	m_iCurBulletGrade = 0;

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->resetBulletLevel();
	}

	this->setBulletGrade(m_iCurBulletGrade);
	return initBullet(getParent(), &m_data);
}

bool Aircraft::downgradeBullet()
{
	if (m_iCurBulletGrade <= 0)
	{
		m_iCurBulletGrade = 0;
		return true;
	}

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->downgradeBullet();
	}

	//--m_iCurBulletGrade;
	if (isMaxLevelBullet())
	{
		if (m_orignBulletGrade >= (int)m_data.bulletIdArray.size() - 1)
		{
			m_iCurBulletGrade--;
		}
		else
		{
			m_iCurBulletGrade = m_orignBulletGrade;
		}
	}
	else
	{
		--m_iCurBulletGrade;
	}

	m_orignBulletGrade = m_iCurBulletGrade;
	this->setBulletGrade(m_iCurBulletGrade);
	return initBullet(getParent(), &m_data);
}

void Aircraft::bulletUseUp()
{
	//TODO 这里需改为：当所有子弹都用尽时才降档
	//for (BulletGroup* bulletGroup : m_bulletGroupArray)
	//{
	//	if (!bulletGroup->isUseUp())
	//	{
	//		return;
	//	}
	//}

	//子弹都用完了再降级
	downgradeBullet();
}

bool Aircraft::initWingman(Node* parent, const TAircraftData* pData)
{
	for (auto w : m_wingmanArray)
	{
		w->destory();
		w->removeFromParent();
	}
	m_wingmanArray.clear();

	for (unsigned i = 0; i < m_data.wingmanArray.size(); i++)
	{
		const TAircraftData* pData = GlobalData::getInstance()->getWingmanData(m_data.wingmanArray.at(i));
		if (NULL == pData)
		{
			DEBUG_LOG("Can't find aircraftdata[%d]", m_data.wingmanArray.at(i));
			continue;
		}

		//以主飞机为中心，往左和往右偏移
		Vec2 v(getContentSize().width / 2, getContentSize().height / 2);
		{
			Wingman* plane1 = PlaneCreator<Wingman>::create(this, pData);
			plane1->setLeft(v * m_data.scale);
			m_wingmanArray.pushBack(plane1);
			//addChild(plane1);
		}

		{
			Wingman* plane2 = PlaneCreator<Wingman>::create(this, pData);
			plane2->setRight(v * m_data.scale);
			m_wingmanArray.pushBack(plane2);
			//addChild(plane2);
		}
	}

	return true;
}

bool Aircraft::init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());

		parent->addChild(this);
		m_pDataCopy = pData;
		m_data.clone(*pData);
		m_data.setLevelData(pLevelData);
		
		Node::setName(m_data.name);
		if (m_data.scale <= 0.001)
		{
			m_data.scale = 1;
		}

		//飞机图片动画
		GameObject::initSpriteWithFileList(m_data.styleArray, m_data.aniDura);
		GameObject::initArmature(m_data.armatureName, m_data.scale);

		//VIP复活后，战机的属性增强
		if (m_eAircraftType == Type_Player || m_eAircraftType == Type_Wingman)
		{
			m_fVipRelifeAttrAdjust = GameData::getInstance()->getValueToFloat(GAMEDATA::VIP_RELIFE_ATTR_ADJUST, 1.0f);
		}
		else
		{
			m_fVipRelifeAttrAdjust = 1.0f;
		}

		//攻击
		m_pEquipAircraft = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeAircraft);
		if (m_pEquipAircraft && m_pEquipAircraft->grade > 0)
		{
			//强化级别
			auto array = GlobalData::getInstance()->getReinforce(m_pEquipAircraft->reinforce_id);
			TReinforceGradeData* pReinforceGradeData = array.at(m_pEquipAircraft->grade - 1);
			m_fAttackAdjust = pReinforceGradeData->value;
			//setBulletSpeedAdjust(pReinforceGradeData->value);
		}
		m_fAttackAdjust = (1 + m_fAttackAdjust) * m_fVipRelifeAttrAdjust - 1;

		//装甲id
		m_pEquipArmature = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeArmature);
		if (m_pEquipArmature)
		{
			//强化级别
			if (m_pEquipArmature->grade > 0)
			{
				auto array = GlobalData::getInstance()->getReinforce(m_pEquipArmature->reinforce_id);
				auto p = array.at(m_pEquipArmature->grade - 1);
				m_data.hp += (int)((m_pEquipArmature->value * (1 + p->value)) + 0.5);
				m_data.maxHp = m_data.hp;
			}
			else
			{
				m_data.hp += m_pEquipArmature->value;
				m_data.maxHp = m_data.hp;
			}
		}
		m_data.hp *= m_fVipRelifeAttrAdjust;
		m_data.maxHp *= m_fVipRelifeAttrAdjust;

		//导弹id
		m_pEquipMissile = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeMissile);
		if (m_pEquipMissile)
		{
			int missileId = m_pEquipMissile->value;
			for (auto bulletIdArray : m_data.bulletIdArray)
			{
				bool found = false;
				for (auto bulletId : *bulletIdArray)
				{
					if (bulletId == missileId)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					bulletIdArray->push_back(missileId);
				}
			}

			//强化级别
			if (m_pEquipMissile->grade > 0)
			{
				auto array = GlobalData::getInstance()->getReinforce(m_pEquipMissile->reinforce_id);
				TReinforceGradeData* pReinforceGradeData = array.at(m_pEquipMissile->grade - 1);
				m_fMissileAdjust = pReinforceGradeData->value;
			}
		}
		m_fMissileAdjust = (1 + m_fMissileAdjust) * m_fVipRelifeAttrAdjust - 1;

		//僚机id
		m_pEquipWingman = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeWingman);
		if (m_pEquipWingman)
		{
			m_data.wingmanArray.clear();
			m_data.wingmanArray.push_back(m_pEquipWingman->value);
		}

		CC_BREAK_IF(!initBody(parent, &m_data));
		CC_BREAK_IF(!initPosition(parent, &m_data));
		CC_BREAK_IF(!initFire(parent, &m_data));
		CC_BREAK_IF(!initHpBar(parent, &m_data));
		CC_BREAK_IF(!initBullet(parent, &m_data));
		CC_BREAK_IF(!initWingman(parent, &m_data));

		setScale(m_data.scale);

		//计算飞机各项属性
		calculateAttr();

		bRet = true;
	} while (0);

	return bRet;
}

void Aircraft::calculateAttr()
{
	//攻击（自带子弹）
	//导弹
	//在initBullet中计算

	//防御
	m_iAttrArmor = m_data.maxHp;

	//僚机
	m_iAttrWingman = 0;
	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_iAttrWingman += m_wingmanArray.at(i)->getAttrAttack();
	}

	//综合
	m_iAttrTotal = m_iAttrAttack + m_iAttrArmor / 20 + m_iAttrMissile + m_iAttrWingman;
}

bool Aircraft::updateEquip()
{
	m_data.clone(*m_pDataCopy);

	//攻击
	m_pEquipAircraft = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeAircraft);
	if (m_pEquipAircraft && m_pEquipAircraft->grade > 0)
	{
		//强化级别
		auto array = GlobalData::getInstance()->getReinforce(m_pEquipAircraft->reinforce_id);
		TReinforceGradeData* pReinforceGradeData = array.at(m_pEquipAircraft->grade - 1);
		setAttackAdjust(pReinforceGradeData->value);
		//setBulletSpeedAdjust(pReinforceGradeData->value);
	}

	//装甲
	m_pEquipArmature = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeArmature);
	if (m_pEquipArmature)
	{
		//强化级别
		if (m_pEquipArmature->grade > 0)
		{
			auto array = GlobalData::getInstance()->getReinforce(m_pEquipArmature->reinforce_id);
			auto p = array.at(m_pEquipArmature->grade - 1);
			m_data.hp += (int)((m_pEquipArmature->value * (1 + p->value)) + 0.5);
			m_data.maxHp = m_data.hp;
		}
		else
		{
			m_data.hp += m_pEquipArmature->value;
			m_data.maxHp = m_data.hp;
		}
	}

	//导弹id
	m_pEquipMissile = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeMissile);
	if (m_pEquipMissile)
	{
		for (auto v : m_data.bulletIdArray)
		{
			v->push_back(m_pEquipMissile->value);
		}
	}

	//僚机id
	m_pEquipWingman = PlayerEquip::getInstance()->getEquip(getId(), EquipTypeWingman);
	if (m_pEquipWingman)
	{
		m_data.wingmanArray.push_back(m_pEquipWingman->value);
	}

	//resetBulletLevel();

	bool bRet = false;
	do
	{
		CC_BREAK_IF(!resetBulletLevel());
		CC_BREAK_IF(!initWingman(getParent(), &m_data));
		bRet = true;
	} while (0);

	calculateAttr();

	return bRet;
}

//生命值降为零、停止射击、删除精灵。子弹不能停
void Aircraft::destory()
{
	reduceHp(m_data.maxHp);

	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		m_bulletGroupArray.at(i)->setPlane(NULL);
	}
	stopShoot();

	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		BulletGroup* pBulletGroup = m_bulletGroupArray.at(i);
		const std::string& name = pBulletGroup->getName();
		if (BULLET_TYPE::LASER == name || BULLET_TYPE::SUPERLASER == name)
		{
			pBulletGroup->destory();
		}
	}
	m_bulletGroupArray.clear();

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->destory();
	}

	stopAllActions();
	unscheduleAllCallbacks();
}

void Aircraft::setNoDie(bool b)
{
	m_bNoDie = b;
}

void Aircraft::reduceHp(int hp)
{
	if (isNoDie())
	{
		return;
	}

	if (m_fDefence >= 0 && m_fDefence <= 1 && m_fDefenceDura > 0)
	{
		m_data.hp -= hp * m_fDefence;
	}
	else
	{
		m_data.hp -= hp;
	}

	if (m_data.hp < 0)
	{
		m_data.hp = 0;
	}

	if (m_pHpBar != NULL)
	{
		m_pHpBar->setCurValue(m_data.hp);
	}
}

void Aircraft::recoverHp(int hp)
{
	m_data.hp += hp;
	if (m_data.hp > m_data.maxHp)
	{
		m_data.hp = m_data.maxHp;
	}

	if (m_pHpBar != NULL)
	{
		m_pHpBar->setCurValue(m_data.hp);
	}
}

bool Aircraft::isAlive()
{
	return getHp() > 0 ? true : false;
}

void Aircraft::autoShoot()
{
	m_bAutoShoot = true;

	this->resetBulletLevel();
	this->startShoot();
}

void Aircraft::startShoot()
{
	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		m_bulletGroupArray.at(i)->startShoot();
	}

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->startShoot();
	}

	if (m_bAutoShoot)
	{
		auto delayDone = [this]()
		{
			autoChangeBulletGrade();
		};

		DelayTime* delay = DelayTime::create(2.0f);
		FiniteTimeAction* end = CallFunc::create(delayDone);
		Sequence* sequence = Sequence::create(delay, end, NULL);
		end->setTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);
		delay->setTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);
		sequence->setTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);
		this->runAction(sequence);
	}
}

void Aircraft::autoChangeBulletGrade()
{
	if (!this->isShooting())
	{
		return;
	}

	if (this->isMaxLevelBullet())
	{
		m_iCurBulletGrade = 0;
	}
	else
	{
		m_iCurBulletGrade = m_data.bulletIdArray.size() - 1;
	}

	this->setBulletGrade(m_iCurBulletGrade);
	initBullet(getParent(), &m_data);

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->autoChangeBulletGrade();
	}

	//auto delayDone = [this]()
	//{
	//	autoChangeBulletGrade();
	//};

	//DelayTime* delay = DelayTime::create(2.0f);
	//FiniteTimeAction* end = CallFunc::create(delayDone);
	//Sequence* sequence = Sequence::create(delay, end, NULL);
	//end->setTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);
	//delay->setTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);
	//sequence->setTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);
	//this->runAction(sequence);
}

void Aircraft::stopAutoShoot()
{
	this->stopAllActionsByTag(AircraftConstant::ACTION_TAG_AUTOSHOOT);

	stopShoot();
}

void Aircraft::stopShoot()
{

	for (int i = 0; i < m_bulletGroupArray.size(); i++)
	{
		m_bulletGroupArray.at(i)->stopShoot();
	}

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->stopShoot();
	}
}

bool Aircraft::isShooting()
{
	if (m_bulletGroupArray.size() > 0 && m_bulletGroupArray.at(0)->isShooting())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Aircraft::addFire(float offsetX, float offsetY, bool isFlipped)
{
	Sprite* fire = GameObject::createSpriteWithFileList(m_data.fireFameNameArray, m_data.fireAniDura);
	if (!fire)
	{
		return true;
	}

	if (m_data.fireFameNameArray.size() == 1)
	{
		ScaleTo* pScale1 = ScaleTo::create(m_data.fireAniDura, 1.0f, 0.9f);
		ScaleTo* pScale2 = ScaleTo::create(m_data.fireAniDura, 1.0f, 1.0f);
		Sequence* sequence = Sequence::create(pScale1, pScale2, NULL);
		Repeat* repeat = Repeat::create(sequence, CC_REPEAT_FOREVER);
		fire->runAction(repeat);
	}

	//添加
	if (m_data.fireOnTop)
	{
		addChild(fire);
	}
	else
	{
		addChild(fire, CONSTANT::ZORDER_PLAYERPLANE_FIRE);
	}

	//镜像翻转
	if (isFlipped)
	{
		fire->setFlippedX(true);
	}
	//位置
	fire->setPosition(Vec2(getContentSize().width / 2 + offsetX, offsetY) * m_data.scale);

	return true;
}

void Aircraft::blast()
{
	Sound::playSound(m_data.blastMusicId);

	//在父层中生成爆炸动画
	PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
	if (NULL != pLayer)
	{
		if (m_pArmature)
		{
			pLayer->addBlast(this->getLocalZOrder(), m_pArmature->getContentSize() * m_data.scale * 0.8f, getPosition(), m_data.blastStyleArray, m_data.blastAniDura);
		}
		else
		{
			pLayer->addBlast(this->getLocalZOrder(), this->getContentSize() * m_data.scale* 0.8f, getPosition(), m_data.blastStyleArray, m_data.blastAniDura);
		}

		//this->destory();
		//this->recycle();
		AircraftPool::getInstance()->recycle(this);
		pLayer->destoryPlane(this);
	}
}

void Aircraft::addShield()
{
	if (m_pDefenceBall && m_pDefenceBall->isVisible())
	{
		return;
	}

	const char* const filename = "protect.png";
	const float duration = 3.0f;
	const float deltaAngle = 360.0f;

	//开启防御效果
	if (m_pDefenceBall == NULL)
	{
		m_pDefenceBall = cocostudio::Armature::create("Prop_Shield");
		m_pDefenceBall->setPosition(getContentSize() / 2 * m_data.scale);
		addChild(m_pDefenceBall);
	}
	else
	{
		m_pDefenceBall->setVisible(true);
	}

	//m_pDefenceBall->stopAllActions();
	m_pDefenceBall->getAnimation()->stop();
	this->unschedule(CC_SCHEDULE_SELECTOR(Aircraft::defenceDone));

	//旋转
	m_pDefenceBall->getAnimation()->play(GlobalData::getInstance()->getArmatureData("Prop_Shield")->defaultAction);
	//FiniteTimeAction* action = CCRotateBy::create(duration, deltaAngle);
	//Repeat* repeat = Repeat::create(action, CC_REPEAT_FOREVER);
	//m_pDefenceBall->runAction(repeat);

}

void Aircraft::removeShield()
{
	//结束防御效果
	if (m_pDefenceBall != NULL)
	{
		m_pDefenceBall->stopAllActions();
		m_pDefenceBall->setVisible(false);
	}
}

void Aircraft::defenceDone(float dt)
{
	m_fDefence = -1;
	m_fDefenceDura = 0;

	//结束防御效果
	if (m_pDefenceBall != NULL)
	{
		m_pDefenceBall->stopAllActions();
		m_pDefenceBall->setVisible(false);
	}
}



bool Aircraft::isCollsion(const Rect& rect, Rect* pCollsionRect)
{
	if (pCollsionRect)
	{
		*pCollsionRect = this->getBodyBox();
	}

	return this->getBodyBox().intersectsRect(rect);
}

//回收
void Aircraft::recycle()
{
	if (m_bRecycled)
	{
		return;
	}

	setNoDie(false);
	//for (int i = 0; i < m_bulletGroupArray.size(); i++)
	//{
	//	m_bulletGroupArray.at(i)->setPlane(NULL);
	//}
	stopShoot();

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		//m_wingmanArray.at(i)->recycle();
		AircraftPool::getInstance()->recycle(m_wingmanArray.at(i));
	}

	pause();

	m_bRecycled = true;
}

//重用
void Aircraft::reuse()
{
	if (!m_bRecycled)
	{
		return;
	}

	if (m_pHpBar)
	{
		m_pHpBar->setMaxValue(m_data.maxHp);
		m_pHpBar->setCurValue(m_data.maxHp);
	}
	m_data.hp = m_data.maxHp;

	//for (int i = 0; i < m_bulletGroupArray.size(); i++)
	//{
	//	m_bulletGroupArray.at(i)->setPlane(this);
	//}
	m_iCurBulletGrade = 0;

	for (int i = 0; i < m_wingmanArray.size(); i++)
	{
		m_wingmanArray.at(i)->reuse();
	}

	if (m_pArmature)
	{
		m_pArmature->setColor(Color3B::WHITE);
		m_pArmature->getAnimation()->play(GlobalData::getInstance()->getArmatureData(m_data.armatureName)->defaultAction);
	}
	else
	{
		this->setColor(Color3B::WHITE);
	}

	resume();

	m_bRecycled = false;
}

//是否已回收（是否可用）
bool Aircraft::isRecycled()
{
	return m_bRecycled;
}
