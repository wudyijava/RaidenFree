#pragma once

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

//可自定义子弹的个数、夹角、偏移
class ScatterBullet : public BulletGroup
{
public:
	ScatterBullet();

	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData);
	virtual void reset();

protected:
	//发射一次子弹
	virtual void AddBullet(float dt) override;
	//计算发射中心线的角度
	virtual float calculateCenterLineDegree(Bullet* pBullet, Vec2& src);
	//发射一颗子弹
	virtual bool AddBulletOne(Vec2& srcOffset, float degree);

protected:
	float m_flyDistance;
	long m_shootCnt;
	int m_shootAddFlag;
};

//瞄准敌机后发射，然后按直线飞行
class AimScatterBullet : public ScatterBullet
{
public:
	AimScatterBullet();

	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData);
	virtual void reset() override;

protected:

	//计算发射中心线的角度
	virtual float calculateCenterLineDegree(Bullet* pBullet, Vec2& src) override;

protected:
	float m_fAngle;
};