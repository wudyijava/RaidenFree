#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BlastNode;

class BlastPool
{
public:
	static BlastPool* getInstance();

	bool init();
	BlastNode* getOneBlast(Node* parent);
	void clear(Node* parent);

protected:
	BlastPool();

private:
	std::vector<BlastNode*> m_pool;
	bool m_bInit;

	static BlastPool* m_instance;
};

class BlastNode : public Node
{
public:
	static BlastNode* create(Node* parent)
	{
		BlastNode *pRet = new(std::nothrow) BlastNode();
		if (pRet && pRet->init(parent))
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

	virtual bool init(Node* parent);

	void run(const Vec2& vPos, float scale, int zorder);

	bool isActive();

protected:
	BlastNode();
	virtual ~BlastNode();
	void PostExplosion();

protected:
	Sprite* m_pExplosion;
	Sprite* m_pLight;

	Texture2D* m_pTextureFrame;
	Animate* m_pAnimate;

	ParticleSystem* m_SmallParticle;
	ParticleSystem* m_SmallParticle2;
	ParticleSystem* m_Smoke;

	bool m_bIsActive;
};
