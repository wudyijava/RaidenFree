#include "Missile.h"
#include "Aircraft.h"
#include "SimpleAudioEngine.h"
#include "Equipment.h"
#include "AimPool.h"

//***************************************************
//
//   导弹基类
//
//***************************************************

Missile::Missile()
{
	reset();
}

Missile::~Missile()
{
}

void Missile::reset()
{
	m_fInitVelocity = 0;
	m_fAccel = 0;
	m_fTurnRate = 0;
	m_fMaxAngle = 0;

	m_fLastAngle = 0;
	m_fVelocity = 0;
	//m_bHasTarget = false;
	m_pEnemy = NULL;
}

void Missile::reuse()
{
	m_fVelocity = m_fInitVelocity;
	m_fLastAngle = 0;
	//m_bHasTarget = false;
	m_pEnemy = NULL;
}

Missile* Missile::create(BulletGroup* pBulletGroup)
{
	Missile *pRet = new(std::nothrow) Missile();
	if (pRet && pRet->init(pBulletGroup))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

//根据pBulletGroup中的子弹相关属性创建子弹
bool Missile::init(BulletGroup* pBulletGroup)
{
	m_pBulletGroup = pBulletGroup;
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Bullet::init(m_pBulletGroup));

		//GameObject::initSpriteWithFileList(m_pBulletGroup->m_data.styleArray, m_pBulletGroup->m_data.aniDura);

		m_fInitVelocity = m_pBulletGroup->m_data.paramMap.at(BULLET_ATTR::MISSILE_VELOCITY);
		m_fAccel = m_pBulletGroup->m_data.paramMap.at(BULLET_ATTR::MISSILE_ACCEL);
		m_fTurnRate = m_pBulletGroup->m_data.paramMap.at(BULLET_ATTR::MISSILE_TURN_RATE);
		m_fMaxAngle = m_pBulletGroup->m_data.paramMap.at(BULLET_ATTR::MISSILE_MAX_ANGLE);

		m_fVelocity = m_fInitVelocity;

		//setBodyCenter(pBulletGroup->getBodyCenter());
		//setBodySize(pBulletGroup->getBodySize());

		bRet = true;
	} while (0);

	return bRet;
}

void Missile::fly()
{
	this->scheduleUpdate();
}

void Missile::nodifyTargetRemove(const Aircraft* pAircraft)
{
	if (pAircraft == m_pEnemy)
	{
		m_pEnemy = NULL;
	}
}

const Aircraft* Missile::getTarget() const
{
	return m_pEnemy;
}

void Missile::setTarget(Aircraft* pTarget)
{
	m_pEnemy = pTarget;
}

void Missile::update(float dt)
{
	if (!this->isVisible() || m_pBulletGroup == NULL)
	{
		return;
	}

	//飞出屏幕
	const Rect& rect = this->getBoundingBox();
	const Size& windowSize = Director::getInstance()->getWinSize();
	if (rect.getMinX() > windowSize.width || rect.getMaxX() < 0
		|| rect.getMinY() > windowSize.height || rect.getMaxY() < 0)
	{
		m_pBulletGroup->bulletMoveFinished(this);
		return;
	}

	//寻找距离最近，且夹角小于70度的敌机
	if ((!m_pEnemy || !m_pEnemy->isAlive()) && m_pBulletGroup && m_pBulletGroup->m_otherSideArray)
	{
		//float minDistance = CONSTANT::DESIGN_RES_DIAGONAL;
		//int idx = 0;
		//for (int i = 0; i < m_pBulletGroup->m_otherSideArray->size(); i++)
		//{
		//	Aircraft* pTarget = m_pBulletGroup->m_otherSideArray->at(i);
		//	Vec2 planePos = pTarget->getPosition();

		//	if (!pTarget->isAlive())
		//	{
		//		continue;
		//	}

		//	//如果飞机在屏幕外面，则不考虑
		//	if (planePos.x < 0 || planePos.y < 0
		//		|| planePos.x > CONSTANT::DESIGN_RES_WIDTH || planePos.y > CONSTANT::DESIGN_RES_HEIGHT)
		//	{
		//		continue;
		//	}

		//	if (m_pBulletGroup->m_otherSideArray->size() > 1)
		//	{
		//		bool sameTarget = false;
		//		for (int j = 0; j < m_pBulletGroup->getAllObject()->count(); j++)
		//		{
		//			Missile* m = (Missile*)m_pBulletGroup->getAllObject()->getObjectAtIndex(j);
		//			if (this != m && m->getTarget() == pTarget)
		//			{
		//				sameTarget = true;
		//			}
		//		}

		//		if (sameTarget)
		//		{
		//			continue;
		//		}
		//	}

		//	//距离与夹角
		//	float angle = -CC_RADIANS_TO_DEGREES((getPosition() - planePos).getAngle());
		//	float distance = planePos.distance(this->getPosition());
		//	if (distance < minDistance && fabsf(angle - 90) <= m_fMaxAngle)
		//	{
		//		minDistance = distance;
		//		idx = i;
		//	}
		//}

		//if (CONSTANT::DESIGN_RES_DIAGONAL == minDistance) //没有找到
		//{
		//	m_pEnemy = NULL;
		//}
		//else //找到了
		//{
		//	m_pEnemy = m_pBulletGroup->m_otherSideArray->at(idx);
		//	m_bHasTarget = true;

		//	float f = -CC_RADIANS_TO_DEGREES((getPosition() - m_pEnemy->getPosition()).getAngle());
		//	//DEBUG_LOG("Missile[%p,%.0f,%.0f] found emeny[%p,%.0f,%.0f], angle[%f]",
		//	//	this, getPosition().x, getPosition().y,
		//	//	m_pEnemy, m_pEnemy->getPosition().x, m_pEnemy->getPosition().y, f);
		//}

		((MissileGroup*)m_pBulletGroup)->searchEnemy(this);
	}

	float f = 0;
	if (m_pEnemy && m_pEnemy->isAlive())
	{
		//转向目标
		float curRot = getRotation();
		float angle = -CC_RADIANS_TO_DEGREES((getPosition() - m_pEnemy->getPosition()).getAngle());
		float tmpAngle = angle;
		if (angle - 90 - curRot < -90 && angle - 90 - curRot + 360 < 90)
		{
			angle += 360;
		}
		else if (angle - 90 - curRot > 90 && angle - 90 - curRot - 360 > -90)
		{
			angle -= 360;
		}
		else
		{
			if (fabsf(m_fLastAngle - angle) > 180)
			{
				if (m_fLastAngle > 0)
				{
					angle += 360;
				}
				else if (m_fLastAngle < 0)
				{
					angle -= 360;
				}
			}
		}

		m_fLastAngle = angle;

		//最大偏转角度
		float angleDif = std::min(std::max((angle - 90) - curRot, -m_fTurnRate*dt), m_fTurnRate*dt);
		f = curRot + angleDif;
		//DEBUG_LOG("Missile[%p,%.0f,%.0f] aimed emeny[%p,%.0f,%.0f], "
		//	"angle[%.2f, %.2f],max[%.2f], curRot[%.2f],angleDif[%.2f],f[%.2f]",
		//	this, getPosition().x, getPosition().y,
		//	m_pEnemy, m_pEnemy->getPosition().x, m_pEnemy->getPosition().y,
		//	tmpAngle, angle, m_fTurnRate*dt, curRot, angleDif, f);

	}
	
	if (!m_pEnemy || !m_pEnemy->isAlive())
	{
		f = getRotation();
		//DEBUG_LOG("Missile[%p,%.0f,%.0f] aimed emeny[NULL], angle[%f]", 
		//	this, getPosition().x, getPosition().y, f);
	}

	setRotation(f);
	setPosition(getPosition() + 
		Vec2(sinf(CC_DEGREES_TO_RADIANS(f))*m_fVelocity, cosf(CC_DEGREES_TO_RADIANS(f))*m_fVelocity)
		* Director::getInstance()->getScheduler()->getTimeScale());

	Vec2 pos2;
	float dd = sqrt(pow(m_bodyCenter.x, 2) + pow(m_bodyCenter.y, 2));
	pos2.x = dd * cos(CC_DEGREES_TO_RADIANS(90 - f));
	pos2.y = dd * sin(CC_DEGREES_TO_RADIANS(90 - f));
	this->setBodyCenter(pos2);

	//当子弹旋转超过45°时，宽高值交换
	if (fabsf(f) > 45 && fabsf(f) < 135)
	{
		Size size = getOrignBodySize();
		float tmp = size.width;
		size.width = size.height;
		size.height = tmp;
		this->setBodySize(size);
	}

	m_fVelocity += m_fAccel*dt;
}
//***************************************************
//
//   导弹组
//
//***************************************************

void MissileGroup::reset()
{
	BulletGroup::reset();
}

bool MissileGroup::init(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BulletGroup::init(pParent, pPlane, pData));

		bRet = true;
	} while (0);

	return bRet;
}

void MissileGroup::nodifyTargetRemove(Aircraft* pAircraft)
{
	int i = 0;
	int size = getAllObject()->count();
	for (; i < size; i++)
	{
		Missile* p = dynamic_cast<Missile*>(m_pAllObject->getObjectAtIndex(i));

		if (p != NULL)
		{
			p->nodifyTargetRemove(pAircraft);
		}
	}

	AimPool::getInstance()->removeEnemy(pAircraft);
}

Bullet* MissileGroup::getOneBullet()
{
	if (m_data.max_count > 0 && m_iCount >= m_data.max_count)
	{
		return NULL;
	}

	int i = 0;
	int size = getAllObject()->count();
	for (; i < size; i++)
	{
		Missile* p = dynamic_cast<Missile*>(m_pAllObject->getObjectAtIndex(i));

		if (p != NULL && p->isVisible() == false)
		{
			p->reuse();
			p->setVisible(true);
			p->setBodySize(p->getOrignBodySize());
			m_iAvailableBullet++;
			m_iCount++;
			return p;
		}
	}

	if (m_pAllObject->count() == i)
	{
		Bullet* pBullet = Missile::create(this);
		m_pAllObject->addObject(pBullet);
		m_iAvailableBullet++;
		m_iCount++;
		//CCLOG("Add bullet[%s], size = %d", Node::getName().c_str(), m_pAllObject->count());
		return pBullet;
	}

	return NULL;
}

void MissileGroup::AddBullet(float dt)
{
	int cnt = m_data.count;
	float angle = m_data.angle_interval;
	float offset = m_data.x_interval;

	if (cnt <= 0)
	{
		cnt = 1;
	}

	float angleLeft = 0;
	float offsetLeft = 0;
	if (cnt % 2 == 0)//偶数
	{
		//计算最左边的子弹的偏移
		angleLeft = - angle / 2 - (cnt / 2 - 1) * angle;
		offsetLeft = -1 * (offset / 2 + (cnt / 2 - 1)*offset);
	}
	else //奇数
	{
		angleLeft = -(cnt - 1) / 2 * angle;
		offsetLeft = -1 * (cnt - 1) / 2 * offset;
	}

	for (int i = 0; i < cnt; i++)
	{
		float a = angleLeft + angle * i;
		float o = offsetLeft + offset * i;
		Vec2 v(m_data.origin_offset_x + o, m_data.origin_offset_y);
		if (!AddBulletOne(v, a))
		{
			break;
		}
	}
}

bool MissileGroup::AddBulletOne(const Vec2& srcOffset, float degree)
{
	//添加一个子弹精灵
	Missile* pBullet = dynamic_cast<Missile*>(this->getOneBullet());
	if (NULL == pBullet)
	{
		return false;
	}

	//子弹的位置
	Vec2 src;
	if (getPlane()->getAircraftType() == EAircraftType::Type_Wingman) //僚机
	{
		Aircraft* pMainPlane = dynamic_cast<Aircraft*>(getPlane()->getParent());
		//src = pMainPlane->convertToWorldSpace(getPlane()->getPosition());
		src = pMainPlane->getPosition() + getPlane()->getPosition() - pMainPlane->getContentSize() / 2;
	}
	else //非僚机
	{
		src = getPlane()->getPosition();
	}

	if (m_data.angle_of_center > 0 && m_data.angle_of_center < 180)
	{
		src.x += srcOffset.x;
		src.y += pBullet->getContentSize().height / 2;
	}
	else
	{
		src.x += -srcOffset.x;
		src.y -= pBullet->getContentSize().height / 2;
	}
	src.y += srcOffset.y;
	pBullet->setPosition(src);
	pBullet->setRotation(90 - m_data.angle_of_center + degree);

	if (srcOffset.x > 0)
	{
		pBullet->setFlippedX(true);
	}

	//设置目标敌机
	Aircraft* pTarget = searchEnemy(pBullet);
	if (pTarget)
	{
		//添加瞄准动画
		AimPool::getInstance()->addEnemy(pTarget);
	}
	pBullet->setTarget(pTarget);

	//飞
	pBullet->fly();

	return true;
}

void MissileGroup::update(float dt)
{
	//遍历所有导弹的目标，判断是否与瞄准池中的目标一致
	BulletGroup::update(dt);
}

Aircraft* MissileGroup::searchEnemy(Missile* pMissile)
{
	if (!m_otherSideArray)
	{
		return nullptr;
	}

	float fMaxAngle = m_data.paramMap.at(BULLET_ATTR::MISSILE_MAX_ANGLE);
	Aircraft* pEnemy = nullptr;

	//查找离得最近的未被瞄准的敌机
	float minDistance = CONSTANT::DESIGN_RES_DIAGONAL;
	int idx = 0;
	std::vector<Aircraft*> sameEnemyArray;
	for (int i = 0; i < m_otherSideArray->size(); i++)
	{
		Aircraft* pTarget = m_otherSideArray->at(i);
		Vec2 planePos = pTarget->getPosition();

		if (!pTarget->isAlive())
		{
			continue;
		}

		//如果飞机在屏幕外面，则不考虑
		if (planePos.x < 0 || planePos.y < 0
			|| planePos.x > CONSTANT::DESIGN_RES_WIDTH || planePos.y > CONSTANT::DESIGN_RES_HEIGHT)
		{
			continue;
		}

		if (m_otherSideArray->size() > 1)
		{
			bool sameTarget = false;
			for (int j = 0; j < getAllObject()->count(); j++)
			{
				Missile* m = (Missile*)getAllObject()->getObjectAtIndex(j);
				if (m->isVisible() && pMissile != m && m->getTarget() == pTarget)
				{
					sameTarget = true;
					sameEnemyArray.push_back(pTarget);
					break;
				}
			}

			if (sameTarget)
			{
				continue;
			}
		}

		//距离与夹角
		float angle = -CC_RADIANS_TO_DEGREES((getPosition() - planePos).getAngle());
		float distance = planePos.distance(this->getPosition());
		if (distance < minDistance && fabsf(angle - 90) <= fMaxAngle)
		{
			minDistance = distance;
			idx = i;
		}
	}

	if (CONSTANT::DESIGN_RES_DIAGONAL == minDistance) //没有找到
	{
		pEnemy = nullptr;
		if (sameEnemyArray.size() > 0)
		{
			unsigned idx = (unsigned)(sameEnemyArray.size() * Utils::getRandom());
			if (idx < 0)
			{
				idx = 0;
			}
			else if (idx > sameEnemyArray.size() - 1)
			{
				idx = sameEnemyArray.size() - 1;
			}
			pEnemy = sameEnemyArray.at(idx);
		}
	}
	else //找到了
	{
		pEnemy = m_otherSideArray->at(idx);
	}

	return pEnemy;
}