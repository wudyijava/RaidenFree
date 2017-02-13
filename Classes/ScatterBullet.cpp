#include "ScatterBullet.h"
#include "Aircraft.h"

ScatterBullet::ScatterBullet()
	: m_flyDistance(0)
	, m_shootCnt(0)
	, m_shootAddFlag(0)
{

}

void ScatterBullet::reset()
{
	BulletGroup::reset();

	m_flyDistance = 0;
	m_shootCnt = 0;
	m_shootAddFlag = 0;
}

bool ScatterBullet::init(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BulletGroup::init(pParent, pPlane, pData));

		m_flyDistance = CONSTANT::DESIGN_RES_DIAGONAL;
		m_shootAddFlag = 1;

		if (m_data.count < 0)
		{
			m_data.count = 1;
		}

		bRet = true;
	} while (0);

	return bRet;
}

void ScatterBullet::AddBullet(float dt)
{
	m_shootCnt += m_shootAddFlag;

	float angleLeft = 0;
	float offsetXLeft = 0;
	if (m_data.count % 2 == 0)//ż��
	{
		//��������ߵ��ӵ��ļнǡ�ƫ��
		angleLeft = m_data.angle_interval / 2 + (m_data.count / 2 - 1) * m_data.angle_interval;
		offsetXLeft = -1 * (m_data.x_interval / 2 + (m_data.count / 2 - 1)*m_data.x_interval);
	}
	else //����
	{
		angleLeft = (m_data.count - 1) / 2 * m_data.angle_interval;
		offsetXLeft = -1 * (m_data.count - 1) / 2 * m_data.x_interval;
	}

	float offsetAngle = 0;
	if (m_data.angle_of_center > 0 && m_data.angle_of_center < 180)
	{
		offsetAngle = m_data.angle_of_center - 90;
	}
	else
	{
		offsetAngle = m_data.angle_of_center - 270;
	}

	for (int i = 0; i < m_data.count; i++)
	{
		float a = angleLeft - m_data.angle_interval * i;

		float l = offsetXLeft + m_data.x_interval * i;
		float x = l * cos(CC_DEGREES_TO_RADIANS(offsetAngle));
		float y = -l * sin(CC_DEGREES_TO_RADIANS(offsetAngle));
		Vec2 v(m_data.origin_offset_x + x, m_data.origin_offset_y + y);		
		if (!AddBulletOne(v, a))
		{
			break;
		}
	}
}

float ScatterBullet::calculateCenterLineDegree(Bullet* pBullet, Vec2& src)
{
	float angle = (m_shootCnt - 1) * m_data.rotate_angle;
	if (fabs(angle) > m_data.rotate_max_angle && m_data.rotate_max_angle >= 1)
	{
		if (m_data.rotate_flag == 0)
		{
			//m_shootAddFlag = -m_shootAddFlag;
			if (angle * m_data.rotate_angle > 0)
			{
				m_shootAddFlag = -1;
			}
			else
			{
				m_shootAddFlag = 1;
			}
		}
		else
		{
			m_shootCnt = 0;
			m_shootAddFlag = 1;
		}
	}

	return m_data.angle_of_center + angle;
}

//degree �Ƕ�ƫ�ơ�������ʾ��ʱ�룬������ʾ˳ʱ��
bool ScatterBullet::AddBulletOne(Vec2& srcOffset, float degree)
{
	//���һ���ӵ�����
	Bullet* pBullet = BulletGroup::getOneBullet();
	if (!pBullet || !getPlane())
	{
		return false;
	}

	//�ӵ���λ��
	Vec2 src;
	if (getPlane()->getAircraftType() == EAircraftType::Type_Wingman) //�Ż�
	{
		Aircraft* pMainPlane = dynamic_cast<Aircraft*>(getPlane()->getParent());

		src = pMainPlane->getPosition() + getPlane()->getPosition() - pMainPlane->getContentSize() / 2;
	}
	else //���Ż�
	{
		src = getPlane()->getPosition();
	}

	src.y += srcOffset.y;

	if (m_data.count == 1)
	{
		if (m_data.angle_of_center > 0 && m_data.angle_of_center < 180)
		{
			src.x += srcOffset.x;
		}
		else
		{
			src.x += -srcOffset.x;
		}
	}

	//��һ�����Ҫ����src.x += srcOffset.x֮ǰ
	float centerLineDegree = calculateCenterLineDegree(pBullet, src)/* + offsetDegree*/;

	if (m_data.count > 1)
	{
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
	}
	pBullet->setPosition(src);


	if (sinf(CC_DEGREES_TO_RADIANS(m_data.angle_of_center)) > 0)
	{
		pBullet->setRotation(90 - (centerLineDegree + degree));
	}
	else
	{
		pBullet->setRotation(90 - (centerLineDegree + degree) + 180);
	}

	if (degree > 0)
	{
		pBullet->setFlippedX(true);
	}

	//body
	Vec2& pos = m_bodyCenter;
	Vec2 pos2;
	pos2.x = sqrt(pow(pos.x, 2) + pow(pos.y, 2)) * cos(CC_DEGREES_TO_RADIANS(centerLineDegree + degree));
	pos2.y = sqrt(pow(pos.x, 2) + pow(pos.y, 2)) * sin(CC_DEGREES_TO_RADIANS(centerLineDegree + degree));
	pBullet->setBodyCenter(pos2);
	pBullet->setBodySize(getBodySize());

	//�ӵ��ɳ���Ļ����ľ��롢�����ٶȡ�����ʱ��
	float realMoveDuration = m_flyDistance / m_data.speed;

	//��realMoveDurationʱ���ڣ��ɵ�ָ��λ��
	float deltaX = m_flyDistance * cos(CC_DEGREES_TO_RADIANS(centerLineDegree + degree));
	float deltaY = m_flyDistance * sin(CC_DEGREES_TO_RADIANS(centerLineDegree + degree));

	Vec2 dest = Vec2(src.x + deltaX, src.y + deltaY);
	FiniteTimeAction* actionMove = CCMoveTo::create(realMoveDuration, dest);
	FiniteTimeAction* actionDone = CallFuncN::create(CC_CALLBACK_0(BulletGroup::bulletMoveFinished, this, pBullet));

	//��ʼִ�ж���
	Sequence* sequenceL = Sequence::create(actionMove, actionDone, NULL);
	pBullet->runAction(sequenceL);

	return true;
}


/////////////////////////////////////////////
//
//   ��׼��ɢ��
//
/////////////////////////////////////////////
AimScatterBullet::AimScatterBullet()
: m_fAngle(-99999)
{
}

void AimScatterBullet::reset()
{
	BulletGroup::reset();

	m_fAngle = -99999;
}

bool AimScatterBullet::init(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!ScatterBullet::init(pParent, pPlane, pData));

		m_flyDistance = CONSTANT::DESIGN_RES_DIAGONAL;

		bRet = true;
	} while (0);

	return bRet;
}

//���㷢�������ߵĽǶ�
float AimScatterBullet::calculateCenterLineDegree(Bullet* pBullet, Vec2& src)
{
	if (fabs(m_fAngle + 99999) > 0.01 && this->isSameGroup())
	{
		return m_fAngle;
	}

	bool bFoundOtherSide = false;

	//�Է��ɻ�������
	if (NULL == m_otherSideArray || m_otherSideArray->size() == 0)
	{
		return m_data.angle_of_center;
	}

	//Ѱ�Ҿ�������ķɻ�
	float minDistance = CONSTANT::DESIGN_RES_DIAGONAL;
	int idx = 0;
	for (int i = 0; i < m_otherSideArray->size(); i++)
	{
		Vec2 planePos = m_otherSideArray->at(0)->getPosition();
		if (planePos.x < 0 || planePos.y < 0
			|| planePos.x > CONSTANT::DESIGN_RES_WIDTH || planePos.y > CONSTANT::DESIGN_RES_HEIGHT)
		{
			continue;  //����ɻ�����Ļ���棬�򲻿���
		}

		float distance = planePos.distance(src);
		if (distance < minDistance)
		{
			minDistance = distance;
			idx = i;
		}
	}
	if (CONSTANT::DESIGN_RES_DIAGONAL == minDistance) //û���ҵ�
	{
		return m_data.angle_of_center;
	}

	//�ɻ���λ��
	Vec2 center = m_otherSideArray->at(idx)->getBodyCenter();
	Vec2 mid = m_otherSideArray->at(idx)->getPosition();
	const Vec2& srcBodyCenter = pBullet->getBodyCenter();
	mid.x += center.x + srcBodyCenter.x;
	mid.y += center.y + srcBodyCenter.y;

	Vec2 tmp(mid.x - src.x, mid.y - src.y);

	float degree = CC_RADIANS_TO_DEGREES(tmp.getAngle());
	//return m_data.angle + degree - 90;
	return m_fAngle = degree;
}
