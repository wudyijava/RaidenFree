#include "Bullet.h"
#include "ScatterBullet.h"
#include "CustomBullet.h"
#include "Missile.h"
#include "Laser.h"
#include "SuperLaser.h"
#include "Aircraft.h"
#include "SimpleAudioEngine.h"
#include "PlaneLayer.h"
#include "Common/Macro.h"
#include "Sound.h"
#include "Armature.h"
#include "GameData.h"
#include <math.h>

const int TAG_BULLET_BLAST = 0;
//***************************************************
//
//   Bullet
//   �����ӵ�
//
//***************************************************
Bullet::Bullet()
{
	reset();
}

Bullet::~Bullet()
{
}

void Bullet::reset()
{
	GameObject::reset();

	m_pArmature = nullptr;
	m_pBulletGroup = NULL;
	m_bBlast = false;
}

Bullet* Bullet::create(BulletGroup* pBulletGroup)
{
	Bullet *pRet = new(std::nothrow) Bullet();
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

//����pBulletGroup�е��ӵ�������Դ����ӵ�
bool Bullet::init(BulletGroup* pBulletGroup)
{
	m_pBulletGroup = pBulletGroup;
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!GameObject::init());

		//int planeZOrder = m_pBulletGroup->m_plane->getLocalZOrder();
		//if (m_pBulletGroup->m_data.topOfAircraft)
		//{
		//	m_pBulletGroup->getParent()->addChild(this, planeZOrder + 1);
		//}
		//else
		//{
		//	m_pBulletGroup->getParent()->addChild(this, planeZOrder - 1);
		//}

		if (m_pBulletGroup->m_data.topOfAircraft)
		{
			m_pBulletGroup->getParent()->addChild(this, CONSTANT::ZORDER_BULLET_ONTOP);
		}
		else
		{
			if (m_pBulletGroup->getPlane()->getAircraftType() == EAircraftType::Type_Enemy)
			{
				m_pBulletGroup->getParent()->addChild(this, CONSTANT::ZORDER_BULLET_ENEMY);
			}
			else
			{
				m_pBulletGroup->getParent()->addChild(this, CONSTANT::ZORDER_BULLET_PLAYER);
			}
		}


		setBodyCenter(m_pBulletGroup->getBodyCenter());
		setBodySize(m_pBulletGroup->getBodySize());

		GameObject::initSpriteWithFileList(m_pBulletGroup->m_data.styleArray, m_pBulletGroup->m_data.aniDura);

		if (m_pBulletGroup->m_data.armatureName.length() > 0)
		{
			m_pArmature = cocostudio::Armature::create(m_pBulletGroup->m_data.armatureName);
			m_pArmature->setPosition(getContentSize() / 2);
			m_pArmature->getAnimation()->play(GlobalData::getInstance()->getArmatureData(m_pBulletGroup->m_data.armatureName)->defaultAction);

			addChild(m_pArmature);
		}
		else
		{
			if (m_pBulletGroup->m_data.fireFameNameArray.size() > 0)
			{
				m_pBulletGroup->m_data.fireOffsetX = abs(m_pBulletGroup->m_data.fireOffsetX);

				if (m_pBulletGroup->m_data.fireOffsetX > 0.01)
				{
					addFire(+m_pBulletGroup->m_data.fireOffsetX, m_pBulletGroup->m_data.fireOffsetY, false);
					addFire(-m_pBulletGroup->m_data.fireOffsetX, m_pBulletGroup->m_data.fireOffsetY, true);
				}
				else
				{
					addFire(0, m_pBulletGroup->m_data.fireOffsetY, false);
				}
			}
		}



		bRet = true;
	} while (0);

	return bRet;
}

bool Bullet::addFire(float offsetX, float offsetY, bool isFlipped)
{
	Sprite* fire = GameObject::createSpriteWithFileList(
		m_pBulletGroup->m_data.fireFameNameArray, m_pBulletGroup->m_data.fireAniDura);

	//�ӵ��ķ����ٶȱȽϿ죬���Ի��治��Ҫ����
	//if (m_pBulletGroup->m_data.fireFameNameArray.size() == 1)
	//{
	//	ScaleTo* pScale1 = ScaleTo::create(m_pBulletGroup->m_data.fireAniDura, 1.0f, 0.9f);
	//	ScaleTo* pScale2 = ScaleTo::create(m_pBulletGroup->m_data.fireAniDura, 1.0f, 1.0f);
	//	Sequence* sequence = Sequence::create(pScale1, pScale2, NULL);
	//	Repeat* repeat = Repeat::create(sequence, CC_REPEAT_FOREVER);
	//	fire->runAction(repeat);
	//}

	//���
	if (m_pBulletGroup->m_data.fireOnTop)
	{
		addChild(fire);
	}
	else
	{
		addChild(fire, CONSTANT::ZORDER_PLAYERPLANE_FIRE);
	}

	//����ת
	if (isFlipped)
	{
		fire->setFlippedX(true);
	}
	//λ��
	fire->setPosition(Vec2(getContentSize().width / 2 + offsetX, offsetY));

	return true;
}
//***************************************************
//
//   BulletGroup
//   �ӵ��Ļ���
//
//***************************************************
BulletGroup::BulletGroup()
{
	this->reset();
}

BulletGroup::~BulletGroup()
{
}

void BulletGroup::reset()
{
	m_data.reset();

	m_iCount = 0;
	m_bIsShooting = false;
	m_plane = NULL;
	m_otherSideArray = NULL;
	m_iAvailableBullet = 0;

	m_iIntervalCnt = 0;
	m_iTimeCum = 0;
	m_iTimeCumThisGrade = 0;
	m_bFirstShoot = false;

	m_pBulletUseUpListener = NULL;
	m_bIsUseUp = false;
	m_timeLimitAdd = 0;
}

bool BulletGroup::init(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!GameObjectContainer::init());

		setPlane(pPlane);
		m_data.clone(*pData);

		Node::setName(m_data.type);

		if (m_data.topOfAircraft)
		{
			pParent->addChild(this, CONSTANT::ZORDER_BULLET_ONTOP);
		}
		else
		{
			if (pPlane->getAircraftType() == EAircraftType::Type_Enemy)
			{
				pParent->addChild(this, CONSTANT::ZORDER_BULLET_ENEMY);
			}
			else
			{
				pParent->addChild(this, CONSTANT::ZORDER_BULLET_PLAYER);
			}
		}

		//body
		Vec2 center(m_data.bodyCenterX, m_data.bodyCenterY);
		setBodyCenter(center);

		//���û�����ø����С����Ĭ����ͼƬ�ߴ��60%
		Size size(m_data.bodySizeW, m_data.bodySizeH);
		if (fabs(size.width) < 0.01 || fabs(size.height) < 0.01)
		{
			size.width = this->getContentSize().width * 0.6f;
			size.height = this->getContentSize().height * 0.6f;
		}
		setBodySize(size);

		//��ȡ�Է��ɻ��б�
		m_otherSideArray = pPlane->getOtherSidePlane();

		if (pPlane->getAircraftType() == EAircraftType::Type_Enemy)
		{
			m_timeLimitAdd = GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_RAMPAGE_DURA);
		}

		bRet = true;
	} while (0);

	return bRet;
}

void BulletGroup::destory()
{
	if (this->m_plane == NULL)
	{
		int i = 0;

		for (i = 0; i < getAllObject()->count(); i++)
		{
			Bullet* p = dynamic_cast<Bullet*>(getAllObject()->getObjectAtIndex(i));

			if (p != NULL && p->isVisible())
			{
				//�����ͷ�
				return;
			}
		}

		if (getAllObject()->count() == i)
		{
			for (i = 0; i < getAllObject()->count(); i++)
			{
				Bullet* p = dynamic_cast<Bullet*>(getAllObject()->getObjectAtIndex(i));

				if (p != NULL)
				{
					p->destory();
				}
			}

			getAllObject()->removeAllObjects();
			
			removeFromParent();
		}
	}
}

void BulletGroup::startShoot()
{
	m_bIsShooting = true;

	this->scheduleUpdate();
}

void BulletGroup::startShoot(int cnt)
{
	m_bIsShooting = false;
	m_iCount = 0;
	m_iIntervalCnt = 0;
	m_iTimeCum = 0;
	m_iTimeCumThisGrade = 0;
	m_bFirstShoot = 0;
	m_bSameGroup = false;
	m_bIsUseUp = false;

	m_data.max_count = cnt;
	m_bIsShooting = true;

	this->scheduleUpdate();
}

void BulletGroup::stopShoot()
{
	unscheduleUpdate();
	unscheduleAllCallbacks();

	m_bIsShooting = false;
}

bool BulletGroup::isShooting()
{
	return m_bIsShooting;
}

void BulletGroup::update(float dt)
{
	m_iTimeCum += dt;
	m_iTimeCumThisGrade += dt;

	if (m_data.timeLimit > 0 && m_iTimeCumThisGrade > m_data.timeLimit + m_timeLimitAdd)
	{
		m_bIsUseUp = true;
		m_iTimeCumThisGrade = 0;
		this->notifyBulletUseUp();
		this->stopShoot();
		return;
	}

	if (!m_bFirstShoot && m_iTimeCum < m_data.delay && m_data.delay >= 0.00001) //��û���е�һ�η��䣬�ȴ��ӳ�
	{
		return;
	}
	else if (!m_bFirstShoot && m_iTimeCum >= m_data.delay && m_data.delay >= 0.00001) //��һ�η��䣬��ʱ��
	{
		m_iTimeCum -= m_data.delay;
		m_bFirstShoot = true;

		m_iIntervalCnt++;
		if (Sound::isBulletSound())
		{
			Sound::playSound(m_data.musicId);
		}
		m_bSameGroup = true;
		AddBullet(dt);
		return;
	}

	//С�����
	if (m_iIntervalCnt < m_data.interval_2_cnt)
	{
		if (m_iTimeCum >= m_iIntervalCnt * m_data.interval_2)
		{
			m_iIntervalCnt++;
			if (Sound::isBulletSound())
			{
				Sound::playSound(m_data.musicId);
			}

			m_bSameGroup = true;
			AddBullet(dt);
			return;
		}

	}
	else //����
	{
		if (m_iTimeCum >= m_data.interval_2_cnt * m_data.interval_2 + m_data.interval)
		{
			m_iIntervalCnt = 1;
			m_iTimeCum = 0;
			if (Sound::isBulletSound())
			{
				Sound::playSound(m_data.musicId);
			}

			m_bSameGroup = false;
			AddBullet(dt);
			return;
		}
	}
}

Bullet* BulletGroup::getOneBullet()
{
	if (!m_bIsShooting)
	{
		return NULL;
	}

	if (m_data.max_count > 0 && m_iCount >= m_data.max_count)
	{
		m_bIsUseUp = true;
		this->notifyBulletUseUp();

		return NULL;
	}

	int i = 0;
	int size = getAllObject()->count();
	for (; i < size; i++)
	{
		Bullet* p = dynamic_cast<Bullet*>(m_pAllObject->getObjectAtIndex(i));

		if (p != NULL && p->isVisible() == false)
		{
			p->setVisible(true);
			p->startAnimate();
			m_iAvailableBullet++;
			m_iCount++;
			return p;
		}
	}

	if (m_pAllObject->count() == i)
	{
		Bullet* pBullet = Bullet::create(this);
		m_pAllObject->addObject(pBullet);
		m_iAvailableBullet++;
		m_iCount++;
		//CCLOG("Add bullet[%s], size = %d", Node::getName().c_str(), m_pAllObject->count());
		return pBullet;
	}

	return NULL;
}

//����Ŀ���ը��ɾ��
void BulletGroup::blast(Bullet* pBullet)
{
	//�ڸ��������ɱ�ը����
	PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
	if (NULL != pLayer)
	{
		Rect rect = pBullet->getBodyBox();
		Vec2 pos(rect.getMidX(), rect.getMidY());

		//���еı�ը����
		pLayer->addBlast(this->getLocalZOrder(), pos, m_data.blastStyleArray, m_data.blastAniDura);

		//����Ч����TODO ����Ҫʹ��particle�����ȡ��������
		ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("res/Blast/hitEnemy.plist");
		emitter1->setPosition(pos);    // ���÷������ӵ�λ��  
		emitter1->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
		emitter1->setDuration(0.3f);                                      // ��������ϵͳ�ĳ���ʱ����  

		pLayer->addChild(emitter1, getLocalZOrder() + 1);
	}
	
	RemoveBullet(pBullet);
}

//�ӵ��ɳ���Ļ��ɾ��
void BulletGroup::bulletMoveFinished(Node* pSender)
{
	RemoveBullet((Bullet*)pSender);
}

void BulletGroup::RemoveBullet(Bullet* pBullet)
{
	if (pBullet != NULL)
	{
		pBullet->stopAllActions();
		pBullet->setVisible(false);
		Node* parent = getParent();

		m_iAvailableBullet--;

		int size = getAllObject()->count();
		int i = 0;
		for (; i < size; i++)
		{
			Bullet* p = dynamic_cast<Bullet*>(m_pAllObject->getObjectAtIndex(i));
			//TODO �����и����⣬���������ӵ���visible�ģ�Ϊʲôm_iAvailableBullet�Ѿ���0����
			if (p != NULL && p->isVisible() == true && m_iAvailableBullet <= 0)
			{
				return;
			}
		}

	}

	//���������
	if (m_plane == NULL && m_iAvailableBullet <= 0 && getParent() != NULL && !this->isShooting())
	{
		int size = getAllObject()->count();
		int i = 0;
		for (; i < size; i++)
		{
			Bullet* p = dynamic_cast<Bullet*>(m_pAllObject->getObjectAtIndex(i));
			p->m_pBulletGroup = NULL;
			p->getParent()->removeChild(p);
		}

		PlaneLayer* layer = dynamic_cast<PlaneLayer*>(getParent());
		getParent()->removeChild(this);
		if (layer)
		{
			layer->removeBullet(this);
		}
	}

}

void BulletGroup::regBulletUseUpListener(IBulletUseUpListener* l)
{
	m_pBulletUseUpListener = l;
}

void BulletGroup::notifyBulletUseUp()
{
	if (m_pBulletUseUpListener != NULL)
	{
		m_pBulletUseUpListener->bulletUseUp();
	}
}

float BulletGroup::getPara(const char* const param)
{
	auto it = m_data.paramMap.find(param);
	if (it != m_data.paramMap.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

//***************************************************
//
//   �ӵ��鹤����
//
//***************************************************

BulletGroup* BulletGroupCreator::create(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	BulletGroup* pRet = NULL;
	if (!strcmp(BULLET_TYPE::COMMON, pData->type.c_str()))
	{
		pRet = new(std::nothrow) ScatterBullet();
	}
	else if (!strcmp(BULLET_TYPE::COMMON_AIM, pData->type.c_str()))
	{
		pRet = new(std::nothrow) AimScatterBullet();
	}
	else if (!strcmp(BULLET_TYPE::CUSTOM, pData->type.c_str()))
	{
		pRet = new(std::nothrow) CustomBullet();
	}
	else if (!strcmp(BULLET_TYPE::MISSILE, pData->type.c_str()))
	{
		pRet = new(std::nothrow) MissileGroup();
	}
	else if (!strcmp(BULLET_TYPE::LASER, pData->type.c_str()))
	{
		pRet = new(std::nothrow) LaserGroup();
	}
	else if (!strcmp(BULLET_TYPE::SUPERLASER, pData->type.c_str()))
	{
		pRet = new(std::nothrow) SuperLaserGroup();
	}

	if (pRet && pRet->init(pParent, pPlane, pData))
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
