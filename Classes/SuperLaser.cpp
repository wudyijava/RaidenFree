#include "SuperLaser.h"

Laser* SuperLaser::create(BulletGroup* pBulletGroup)
{
	Laser *pRet = new(std::nothrow) SuperLaser();
	if (pRet && pRet->init(pBulletGroup))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void SuperLaser::rejustHeight(float fMinY)
{
	m_fHeight = CONSTANT::DESIGN_RES_HEIGHT;
}

//从子弹池中获取一颗子弹
Bullet* SuperLaserGroup::getOneBullet()
{
	if (!m_bIsShooting)
	{
		return NULL;
	}

	Bullet* pBullet = nullptr;

	if (getAllObject()->count() == 0)
	{
		pBullet = SuperLaser::create(this);
		m_pAllObject->addObject(pBullet);
	}
	else
	{
		pBullet = dynamic_cast<Bullet*>(m_pAllObject->getObjectAtIndex(0));
	}

	pBullet->setVisible(true);
	return pBullet;
}
