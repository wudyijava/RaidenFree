#pragma once

#include "cocos2d.h"
#include "Laser.h"

USING_NS_CC;

class SuperLaser : public Laser
{
public:
	static Laser* create(BulletGroup* pBulletGroup);

	virtual void rejustHeight(float fMinY) override;

};

class SuperLaserGroup : public LaserGroup
{
protected:

	//���ӵ����л�ȡһ���ӵ�
	virtual Bullet* getOneBullet() override;

};