#pragma once

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class Aircraft;
struct TBulletData;

////����ר������
//const char* const BULLET_ATTR_MISSILE_VELOCITY = "MISSILE_VELOCITY";
//const char* const BULLET_ATTR_MISSILE_ACCEL = "MISSILE_ACCEL";
//const char* const BULLET_ATTR_MISSILE_TURN_RATE = "MISSILE_TURN_RATE";
//const char* const BULLET_ATTR_MISSILE_MAX_ANGLE = "MISSILE_MAX_ANGLE";

class Missile : public Bullet
{
public:
	static Missile* create(BulletGroup* pBulletGroup);

	//����pBulletGroup�е��ӵ�������Դ����ӵ�
	virtual bool init(BulletGroup* pBulletGroup) override;

	virtual void reset() override;
	virtual void reuse();
	//����
	void fly();
	//���Է��ɻ����ӻ��߼���ʱ��֪ͨ����
	virtual void nodifyTargetRemove(const Aircraft* pAircraft);

	const Aircraft* getTarget() const;
	void setTarget(Aircraft* pTarget);

	Missile();
	virtual ~Missile();

	void update(float dt);

protected:
	float m_fInitVelocity; //��ʼ����
	float m_fAccel;    //���ٶ�
	float m_fTurnRate; //���ٶ�
	float m_fMaxAngle; //Ѱ��Ŀ������Ƕ�

	float m_fLastAngle;
	float m_fVelocity; //����
	//bool m_bHasTarget;
	Aircraft* m_pEnemy;
};

//���ٵ���
class MissileGroup : public BulletGroup
{
public:
	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData) override;
	virtual void reset() override;

	//���Է��ɻ����ӻ��߼���ʱ��֪ͨ����
	virtual void nodifyTargetRemove(Aircraft* pAircraft) override;
	//������������δ����׼�ĵл�
	virtual Aircraft* searchEnemy(Missile* pMissile);

protected:
	//����һ���ӵ�
	virtual void AddBullet(float dt) override;
	//����һ���ӵ�
	virtual bool AddBulletOne(const Vec2& srcOffset, float degree);

	//���ӵ����л�ȡһ���ӵ�
	virtual Bullet* getOneBullet() override;


	virtual void update(float dt) override;
};
