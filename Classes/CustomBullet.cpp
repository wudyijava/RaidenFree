#include "CustomBullet.h"
#include "Aircraft.h"
#include "SimpleAudioEngine.h"


CustomBullet::CustomBullet()
{
	m_i = 0;
	m_time = 0;
}

bool CustomBullet::init(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BulletGroup::init(pParent, pPlane, pData));

		CCASSERT(m_data.customArray.size() > 0, "CustomBullet::init : m_data.customArray.size() = 0");

		bRet = true;
	} while (0);

	return bRet;
}

void CustomBullet::update(float dt)
{
	m_time += dt;

	for (; m_i < m_data.customArray.size(); m_i++)
	{
		if (m_data.customArray.at(m_i)->time <= m_time)
		{
			if (!AddCustomBullet(m_data.customArray.at(m_i)))
			{
				return;
			}
		}
		else
		{
			break;
		}
	}

	if (m_i >= m_data.customArray.size())
	{
		m_i = 0;
		m_time = 0;
	}
}

bool CustomBullet::AddCustomBullet(const TBulletCustomData* customData)
{
	//添加一个子弹精灵
	Bullet* pBullet = BulletGroup::getOneBullet();
	if (NULL == pBullet)
	{
		return false;
	}

	//子弹的位置
	Vec2 src;
	if (getPlane()->getAircraftType() == EAircraftType::Type_Wingman) //僚机
	{
		Aircraft* pMainPlane = dynamic_cast<Aircraft*>(getPlane()->getParent());
		src = pMainPlane->convertToWorldSpace(getPlane()->getPosition());
	}
	else //非僚机
	{
		src = getPlane()->getPosition();
	}

	src.x += customData->x;
	src.y += customData->y;

	pBullet->setPosition(src);

	//pBullet->setRotation(90 - customData->angle);
	if (sinf(CC_DEGREES_TO_RADIANS(m_data.angle_of_center)) > 0)
	{
		pBullet->setRotation(90 - customData->angle);
	}
	else
	{
		pBullet->setRotation(90 - customData->angle + 180);
	}


	if (customData->angle > 0 && customData->angle < 180)
	{
		src.y += pBullet->getContentSize().height / 2;
	}
	else
	{
		src.y -= pBullet->getContentSize().height / 2;
	}

	//body
	Vec2& pos = m_bodyCenter;
	Vec2 pos2;
	pos2.x = sqrt(pow(pos.x, 2) + pow(pos.y, 2)) * cos(CC_DEGREES_TO_RADIANS(customData->angle));
	pos2.y = sqrt(pow(pos.x, 2) + pow(pos.y, 2)) * sin(CC_DEGREES_TO_RADIANS(customData->angle));
	pBullet->setBodyCenter(pos2);
	pBullet->setBodySize(getBodySize());

	Vec2 dest = src + Vec2(CONSTANT::DESIGN_RES_DIAGONAL * cos(CC_DEGREES_TO_RADIANS(customData->angle)),
		CONSTANT::DESIGN_RES_DIAGONAL * sin(CC_DEGREES_TO_RADIANS(customData->angle)));

	float realMoveDuration = CONSTANT::DESIGN_RES_DIAGONAL * 1.f / customData->speed;

	FiniteTimeAction* actionMove = CCMoveTo::create(realMoveDuration, dest);
	FiniteTimeAction* actionDone = CallFuncN::create(CC_CALLBACK_0(BulletGroup::bulletMoveFinished, this, pBullet));

	//开始执行动作
	Sequence* sequenceL = Sequence::create(actionMove, actionDone, NULL);
	pBullet->runAction(sequenceL);

	return true;
}