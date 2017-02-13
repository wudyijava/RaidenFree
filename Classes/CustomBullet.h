#pragma once

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class Aircraft;
struct TBulletData;

//自定义轨迹子弹
class CustomBullet : public BulletGroup
{
public:
	CustomBullet();

	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData) override;

	virtual void update(float dt) override;

protected:

	bool AddCustomBullet(const TBulletCustomData* customData);

private:
	unsigned int m_i;
	float m_time;
};
