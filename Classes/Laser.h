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

	//����pBulletGroup�е��ӵ�������Դ����ӵ�
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

//����
class LaserGroup : public BulletGroup
{
public:
	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData) override;
	virtual void reset() override;

	//��ʼ���
	virtual void startShoot()override;
	//ֹͣ���
	virtual void stopShoot()override;

	void blast(Bullet* pBullet);

	virtual void update(float dt) override;

protected:

	//����һ���ӵ������ܻ��������ӵ���
	virtual void AddBullet(float dt) override;

	//���ӵ����л�ȡһ���ӵ�
	virtual Bullet* getOneBullet() override;
};
