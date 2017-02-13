#pragma once

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

//���Զ����ӵ��ĸ������нǡ�ƫ��
class ScatterBullet : public BulletGroup
{
public:
	ScatterBullet();

	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData);
	virtual void reset();

protected:
	//����һ���ӵ�
	virtual void AddBullet(float dt) override;
	//���㷢�������ߵĽǶ�
	virtual float calculateCenterLineDegree(Bullet* pBullet, Vec2& src);
	//����һ���ӵ�
	virtual bool AddBulletOne(Vec2& srcOffset, float degree);

protected:
	float m_flyDistance;
	long m_shootCnt;
	int m_shootAddFlag;
};

//��׼�л����䣬Ȼ��ֱ�߷���
class AimScatterBullet : public ScatterBullet
{
public:
	AimScatterBullet();

	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData);
	virtual void reset() override;

protected:

	//���㷢�������ߵĽǶ�
	virtual float calculateCenterLineDegree(Bullet* pBullet, Vec2& src) override;

protected:
	float m_fAngle;
};