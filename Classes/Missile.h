#pragma once

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class Aircraft;
struct TBulletData;

////导弹专用属性
//const char* const BULLET_ATTR_MISSILE_VELOCITY = "MISSILE_VELOCITY";
//const char* const BULLET_ATTR_MISSILE_ACCEL = "MISSILE_ACCEL";
//const char* const BULLET_ATTR_MISSILE_TURN_RATE = "MISSILE_TURN_RATE";
//const char* const BULLET_ATTR_MISSILE_MAX_ANGLE = "MISSILE_MAX_ANGLE";

class Missile : public Bullet
{
public:
	static Missile* create(BulletGroup* pBulletGroup);

	//根据pBulletGroup中的子弹相关属性创建子弹
	virtual bool init(BulletGroup* pBulletGroup) override;

	virtual void reset() override;
	virtual void reuse();
	//发射
	void fly();
	//当对方飞机增加或者减少时，通知导弹
	virtual void nodifyTargetRemove(const Aircraft* pAircraft);

	const Aircraft* getTarget() const;
	void setTarget(Aircraft* pTarget);

	Missile();
	virtual ~Missile();

	void update(float dt);

protected:
	float m_fInitVelocity; //初始速率
	float m_fAccel;    //加速度
	float m_fTurnRate; //角速度
	float m_fMaxAngle; //寻找目标的最大角度

	float m_fLastAngle;
	float m_fVelocity; //速率
	//bool m_bHasTarget;
	Aircraft* m_pEnemy;
};

//跟踪导弹
class MissileGroup : public BulletGroup
{
public:
	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData) override;
	virtual void reset() override;

	//当对方飞机增加或者减少时，通知导弹
	virtual void nodifyTargetRemove(Aircraft* pAircraft) override;
	//查找离得最近的未被瞄准的敌机
	virtual Aircraft* searchEnemy(Missile* pMissile);

protected:
	//发射一次子弹
	virtual void AddBullet(float dt) override;
	//发射一颗子弹
	virtual bool AddBulletOne(const Vec2& srcOffset, float degree);

	//从子弹池中获取一颗子弹
	virtual Bullet* getOneBullet() override;


	virtual void update(float dt) override;
};
