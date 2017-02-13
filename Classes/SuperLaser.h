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

	//从子弹池中获取一颗子弹
	virtual Bullet* getOneBullet() override;

};