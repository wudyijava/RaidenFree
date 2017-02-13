#pragma once

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class Aircraft;
struct TBulletData;

class Laser : public Bullet
{
public:
	static Laser* create(BulletGroup* pBulletGroup);

	//根据pBulletGroup中的子弹相关属性创建子弹
	virtual bool init(BulletGroup* pBulletGroup) override;

	virtual void reset() override;

	Laser();
	virtual ~Laser();

	void update(float dt);

	virtual void rejustHeight(float fMinY);

protected:
	Sprite* m_pLaser;
	Sprite* m_pBlast;

	float m_fTmpOffset;

	float m_fWidth;
	float m_fHeight;
	Rect m_rect;
};

//激光
class LaserGroup : public BulletGroup
{
public:
	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData) override;
	virtual void reset() override;

	//开始射击
	virtual void startShoot()override;
	//停止射击
	virtual void stopShoot()override;

	void blast(Bullet* pBullet);

	virtual void update(float dt) override;

protected:

	//发射一次子弹（可能会包含多颗子弹）
	virtual void AddBullet(float dt) override;

	//从子弹池中获取一颗子弹
	virtual Bullet* getOneBullet() override;
};
