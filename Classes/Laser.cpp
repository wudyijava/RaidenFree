#include "Laser.h"
#include "Aircraft.h"
#include "SimpleAudioEngine.h"

//***************************************************
//
//   ������������
//
//***************************************************

Laser::Laser()
{
	reset();
}

Laser::~Laser()
{
}

void Laser::reset()
{
	m_pLaser = nullptr;
	m_pBlast = nullptr;

	m_fTmpOffset = 0;

	m_fWidth = 0;
	m_fHeight = 0;
}

Laser* Laser::create(BulletGroup* pBulletGroup)
{
	Laser *pRet = new(std::nothrow) Laser();
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
bool Laser::init(BulletGroup* pBulletGroup)
{
	if (!GameObject::init())
	{
		return false;
	}

	m_pBulletGroup = pBulletGroup;
	//int planeZOrder = m_pBulletGroup->m_plane->getLocalZOrder();
	if (m_pBulletGroup->getPlane()->getAircraftType() == EAircraftType::Type_Killer)
	{
		m_pBulletGroup->getParent()->addChild(this, CONSTANT::ZORDER_BULLET_KILLER);
	}
	else if (m_pBulletGroup->getPlane()->getAircraftType() == EAircraftType::Type_Player)
	{
		m_pBulletGroup->getParent()->addChild(this, CONSTANT::ZORDER_BULLET_PLAYER);
	}
	else
	{
		int planeZOrder = CONSTANT::ZORDER_BULLET_PLAYER;
		m_pBulletGroup->getParent()->addChild(this, planeZOrder - 1);
	}

	//���ﲻ����createWithSpriteFrameName�����򼤹����һ��һ�εģ��м��к�ɫ��϶��
	Sprite* pLaserNode = Sprite::create(m_pBulletGroup->m_data.styleArray.at(0));

	m_fWidth = pLaserNode->getContentSize().width;
	m_fHeight = CONSTANT::DESIGN_RES_HEIGHT + 200;

	setBodySize(Size(pBulletGroup->getBodySize().width, m_fHeight));
	setBodyCenter(pBulletGroup->getBodyCenter());

	setContentSize(Size(pBulletGroup->getBodySize().width, m_fHeight));

	int cnt = (int)(m_fHeight / pLaserNode->getContentSize().height + 0.9999);
	float h = cnt * pLaserNode->getContentSize().height;

	// 1: Create new CCRenderTexture
	RenderTexture *rt = RenderTexture::create(m_fWidth, h);

	// 2: Call CCRenderTexture:begin
	rt->begin();

	pLaserNode->setFlippedY(true);
	pLaserNode->setPosition(Vec2(m_fWidth / 2, pLaserNode->getContentSize().height / 2));
	pLaserNode->visit();

	for (int i = 1; i < cnt; i++)
	{
		//���ﲻ����createWithSpriteFrameName�����򼤹����һ��һ�εģ��м��к�ɫ��϶��
		Sprite* pLaserNode2 = Sprite::create(m_pBulletGroup->m_data.styleArray.at(0));
		pLaserNode2->setFlippedY(true);
		pLaserNode2->setPosition(Vec2(m_fWidth / 2, 
			pLaserNode->getContentSize().height / 2 + pLaserNode->getContentSize().height * i));
		pLaserNode2->visit();
	}

	// 4: Call CCRenderTexture:end
	rt->end();

	// 5: Create a new Sprite from the texture
	m_pLaser = Sprite::createWithTexture(rt->getSprite()->getTexture());

	m_pLaser->setTextureRect(Rect(0, 0, m_fWidth, m_fHeight));

	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	m_pLaser->getTexture()->setTexParameters(tp);

	m_pLaser->setAnchorPoint(Vec2(0.5, 0));
	m_pLaser->setPosition(Vec2(getContentSize().width / 2, 0));
	addChild(m_pLaser);

	//��ӱ�ը��
	m_pBlast = GameObject::createSpriteWithFileList(m_pBulletGroup->m_data.blastStyleArray, m_pBulletGroup->m_data.blastAniDura);
	if (m_pBlast)
	{
		m_pBlast->setPosition(Vec2(getBodySize().width / 2, getBodySize().height));
		addChild(m_pBlast);
	}

	scheduleUpdate();

	return true;
}

void Laser::rejustHeight(float fMinY)
{
	if (m_pBulletGroup && m_pBulletGroup->m_plane)
	{
		m_fHeight = fMinY - m_pBulletGroup->m_plane->getPosition().y;
		if (m_fHeight < 0)
		{
			m_fHeight = CONSTANT::DESIGN_RES_HEIGHT + 200;
		}
		setBodySize(Size(m_pBulletGroup->getBodySize().width, m_fHeight));
		if (m_pBlast)
		{
			m_pBlast->setPosition(Vec2(getBodySize().width / 2, getBodySize().height));
		}
	}
}

void Laser::update(float dt)
{
	if (!this->isVisible() || m_pBulletGroup == NULL)
	{
		return;
	}

	//���㵱ǰλ��
	setBodySize(Size(getBodySize().width, CONSTANT::DESIGN_RES_HEIGHT));

	m_fTmpOffset += m_pBulletGroup->m_data.speed * dt;

	const Size& textureSize = m_pLaser->getTextureRect().size;
	m_pLaser->setTextureRect(Rect(0, m_fTmpOffset, textureSize.width, m_fHeight));
	
}

//***************************************************
//
//   ������
//
//***************************************************

void LaserGroup::reset()
{
	BulletGroup::reset();
}

bool LaserGroup::init(Node* pParent, Aircraft* pPlane, const TBulletData* pData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BulletGroup::init(pParent, pPlane, pData));

		bRet = true;
	} while (0);

	return bRet;
}

void LaserGroup::startShoot()
{
	m_bIsShooting = true;

	this->scheduleUpdate();
}

void LaserGroup::stopShoot()
{
	unscheduleUpdate();
	unscheduleAllCallbacks();

	if (getAllObject()->count() > 0)
	{
		Bullet* pBullet = dynamic_cast<Bullet*>(m_pAllObject->getObjectAtIndex(0));
		pBullet->unscheduleUpdate();
		pBullet->setVisible(false);
	}

	m_bIsShooting = false;
}

void LaserGroup::update(float dt)
{
	m_iTimeCum += dt;

	if (m_data.timeLimit > 0 && m_iTimeCum > m_data.timeLimit + m_timeLimitAdd)
	{
		this->notifyBulletUseUp();
		this->stopShoot();
		return;
	}

	AddBullet(dt);
}

//����һ���ӵ������ܻ��������ӵ���
void LaserGroup::AddBullet(float dt)
{
	auto p = this->getOneBullet();
	if (p)
	{
		p->setAnchorPoint(Vec2(0.5, 0));
		p->setPosition(m_plane->getPosition());
	}	
}

//���ӵ����л�ȡһ���ӵ�
Bullet* LaserGroup::getOneBullet()
{
	if (!m_bIsShooting)
	{
		return NULL;
	}

	Bullet* pBullet = nullptr;

	if (getAllObject()->count() == 0)
	{
		pBullet = Laser::create(this);
		m_pAllObject->addObject(pBullet);
	}
	else
	{
		pBullet = dynamic_cast<Bullet*>(m_pAllObject->getObjectAtIndex(0));
		pBullet->scheduleUpdate();
	}

	pBullet->setVisible(true);
	return pBullet;
}

//����Ŀ���ը����ɾ��
void LaserGroup::blast(Bullet* pBullet)
{
	////�ڸ��������ɱ�ը����
	//PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
	//if (NULL != pLayer)
	//{
	//	Rect rect = pBullet->getBodyBox();
	//	Vec2 pos(rect.getMidX(), rect.getMaxY());

	//	//���еı�ը����
	//	pLayer->addBlast(this->getLocalZOrder(), pos, m_data.blastStyleArray, m_data.blastAniDura);

	//	//����Ч����TODO ����Ҫʹ��particle�����ȡ��������
	//	ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("res/Blast/hitEnemy.plist");
	//	emitter1->setPosition(pos);    // ���÷������ӵ�λ��  
	//	emitter1->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
	//	emitter1->setDuration(0.3f);                                      // ��������ϵͳ�ĳ���ʱ����  

	//	pLayer->addChild(emitter1, getLocalZOrder() + 1);
	//}

	//���ⲻ��remove
	//RemoveBullet(pBullet);
}