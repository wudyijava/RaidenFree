#include "Blast.h"
#include "SimpleAudioEngine.h"
#include "Common/Macro.h"

#define EXPLOSION_TIME 0.8f

BlastPool* BlastPool::m_instance = NULL;

BlastPool* BlastPool::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new BlastPool;

		m_instance->init();
	}

	return m_instance;
}

BlastPool::BlastPool()
{
	m_bInit = false;
}

bool BlastPool::init()
{
	if (!m_bInit)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Explosion.wav");
		Director::getInstance()->getTextureCache()->addImage("res/Blast/ExplosionFrame.png");
		Director::getInstance()->getTextureCache()->addImage("res/Blast/Light.png");

		m_bInit = true;
	}

	return true;
}

BlastNode* BlastPool::getOneBlast(Node* parent)
{
	for (auto p : m_pool)
	{
		if (!p->isActive())
		{
			return p;
		}
	}

	auto p = BlastNode::create(parent);
	if (p)
	{
		m_pool.push_back(p);
		return p;
	}
	else
	{
		return NULL;
	}
}

void BlastPool::clear(Node* parent)
{
	for (auto p : m_pool)
	{
		parent->removeChild(parent);
	}
	m_pool.clear();
}

BlastNode::BlastNode()
{
	m_pExplosion = NULL;
	m_pLight = NULL;

	m_pTextureFrame = NULL;
	m_pAnimate = NULL;

	m_SmallParticle = NULL;
	m_SmallParticle2 = NULL;
	m_Smoke = NULL;

	m_bIsActive = false;;
}

BlastNode::~BlastNode()
{
	if (m_pAnimate != NULL)
	{
		m_pAnimate->release();
	}
}

bool BlastNode::init(Node* parent)
{
	parent->addChild(this, CONSTANT::ZORDER_PLAYER + 1);
	m_bIsActive = false;

	Size size = Director::getInstance()->getWinSize();

	//ÑÌÎí
	m_Smoke = ParticleSystemQuad::create("res/Blast/Smoke.plist");
	m_Smoke->setBlendAdditive(true);
	m_Smoke->stopSystem();
	parent->addChild(m_Smoke, CONSTANT::ZORDER_PLAYER + 1);
	//¹â
	Texture2D* pLightTexture = Director::getInstance()->getTextureCache()->getTextureForKey("res/Blast/Light.png");
	m_pLight = Sprite::createWithTexture(pLightTexture);
	m_pLight->setPosition(Vec2(size.width / 2, size.height / 2));
	m_pLight->setVisible(false);
	m_pLight->setOpacity(180);
	//parent->addChild(m_pLight, CONSTANT::ZORDER_BLAST + 1);
	parent->addChild(m_pLight, CONSTANT::ZORDER_PLAYER + 2);
	//±¬Õ¨
	m_pTextureFrame = Director::getInstance()->getTextureCache()->getTextureForKey("res/Blast/ExplosionFrame.png");
	m_pExplosion = Sprite::createWithTexture(m_pTextureFrame, Rect(0, 0, 1, 1));
	m_pExplosion->setPosition(Vec2(size.width / 2, size.height / 2));
	//parent->addChild(m_pExplosion, CONSTANT::ZORDER_BLAST + 2);
	parent->addChild(m_pExplosion, CONSTANT::ZORDER_PLAYER + 3);
	////Á£×Ó1
	m_SmallParticle = ParticleSystemQuad::create("res/Blast/SmallParticle.plist");
	m_SmallParticle->setBlendAdditive(true);
	m_SmallParticle->stopSystem();
	//parent->addChild(m_SmallParticle, CONSTANT::ZORDER_BLAST + 3);
	parent->addChild(m_SmallParticle, CONSTANT::ZORDER_PLAYER + 4);
	//Á£×Ó2
	m_SmallParticle2 = ParticleSystemQuad::create("res/Blast/SmallParticle2.plist");
	m_SmallParticle2->setBlendAdditive(true);
	m_SmallParticle2->stopSystem();
	//parent->addChild(m_SmallParticle2, CONSTANT::ZORDER_BLAST + 3);
	parent->addChild(m_SmallParticle2, CONSTANT::ZORDER_PLAYER + 4);

	if (!m_pLight || !m_pTextureFrame || !m_Smoke || !m_SmallParticle || !m_SmallParticle2)
	{
		return false;
	}

	return true;
}

bool BlastNode::isActive()
{
	return m_bIsActive;
}

void BlastNode::run(const Vec2& vPos, float scale, int zorder)
{
	m_pLight->setScale(scale);
	m_Smoke->setScale(scale/1.5);
	m_SmallParticle->setScale(scale/2);
	m_SmallParticle2->setScale(scale/2);
	m_pExplosion->setScale((1 + CCRANDOM_0_1()) * scale);

	this->stopAllActions();
	m_bIsActive = true;

	m_SmallParticle->setPosition(vPos);
	m_SmallParticle2->setPosition(vPos);
	m_Smoke->setPosition(vPos);
	m_pExplosion->setPosition(vPos);
	m_pLight->setPosition(vPos);

	m_pExplosion->setVisible(true);
	m_pLight->setVisible(true);

	m_pLight->setLocalZOrder(zorder + 1);
	m_pExplosion->setLocalZOrder(zorder + 2);
	m_SmallParticle->setLocalZOrder(zorder + 3);
	m_SmallParticle2->setLocalZOrder(zorder + 4);
	m_Smoke->setLocalZOrder(zorder + 5);

	m_pExplosion->setRotation(360.0f*CCRANDOM_0_1());

	m_SmallParticle->resetSystem();
	m_SmallParticle2->resetSystem();
	m_Smoke->resetSystem();

	//±¬Õ¨¶¯»­
	if (m_pAnimate == NULL)
	{
		Animation* animation = Animation::create();
		for (int i = 0; i<16; i++)
		{
			int row = i / 4;
			int col = i % 4;
			Rect rect(col * 64, row * 64, 64, 64);
			animation->addSpriteFrameWithTexture(m_pTextureFrame, rect);
		}
		// should last EXPLOSION_TIME seconds. And there are 16 frames.
		animation->setDelayPerUnit(EXPLOSION_TIME / 16.0f);
		animation->setRestoreOriginalFrame(true);

		m_pAnimate = Animate::create(animation);
		m_pAnimate->retain();
	}

	m_pExplosion->runAction(m_pAnimate);

	//¹â
	float fScaleStart = 0.7f;
	float fScaleEnd = 3.f;

	float fOpacityStart = 255.f;
	float fOpacityEnd = 10.f;

	m_pLight->setScale(fScaleStart);
	m_pLight->setOpacity(fOpacityStart);
	FiniteTimeAction* pScaleTo1 = ScaleTo::create(EXPLOSION_TIME * 2.f, fScaleEnd, fScaleEnd);
	FadeTo* pFade = FadeTo::create(EXPLOSION_TIME * 1.5f, fOpacityEnd);
	Spawn* seq = Spawn::create(pScaleTo1, pFade, NULL);
	m_pLight->runAction(seq);

	//ÒôÐ§
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Explosion.wav");

	FiniteTimeAction* pDelay = DelayTime::create(EXPLOSION_TIME);
	FiniteTimeAction* pPostExplosion = CallFunc::create([this]()
	{
		PostExplosion();
	});

	Sequence* seq2 = Sequence::create(pDelay, pPostExplosion, NULL);
	this->runAction(seq2);
}

void BlastNode::PostExplosion()
{
	m_pExplosion->setVisible(false);
	m_pLight->setVisible(false);

	m_bIsActive = false;
}