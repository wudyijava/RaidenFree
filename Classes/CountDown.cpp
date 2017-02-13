#include "CountDown.h"
#include "Sound.h"
#include "GameLayer.h"
#include "PlaneLayer.h"

CountDownLayer::CountDownLayer()
: m_bDisplayBuySuccess(false)
{

}

CountDownLayer::~CountDownLayer()
{

}

void CountDownLayer::displayBuySuccess()
{
	m_bDisplayBuySuccess = true;
}

bool CountDownLayer::doInit()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();

	m_pOne = Sprite::createWithSpriteFrameName("1.png");
	m_pTwo = Sprite::createWithSpriteFrameName("2.png");
	m_pThree = Sprite::createWithSpriteFrameName("3.png");

	this->addChild(m_pThree);
	this->addChild(m_pTwo);
	this->addChild(m_pOne);

	m_pThree->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	m_pTwo->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	m_pOne->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);

	m_pTwo->setVisible(false);
	m_pOne->setVisible(false);

	if (m_bDisplayBuySuccess)
	{
		Sprite* pSprite = Sprite::createWithSpriteFrameName("buy-success.png");
		this->addChild(pSprite);
		pSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 4);
	}

	Sound::playSound(SOUND::ID_COUNT_DOWN);

	displayThree();

	return true;
}

void CountDownLayer::displayThree()
{
	ScaleTo* pScale = ScaleTo::create(1.0f, 1.4f);
	FadeTo*  pFade = FadeTo::create(1.0f, 150);
	FiniteTimeAction* pEnd = CallFunc::create(CC_CALLBACK_0(CountDownLayer::displayTwo, this));
	Spawn* pSpawn = Spawn::create(pScale, pFade, NULL);
	Sequence* seq = Sequence::create(pSpawn, pEnd, NULL);

	m_pThree->runAction(seq);
}

void CountDownLayer::displayTwo()
{
	m_pThree->setVisible(false);
	m_pTwo->setVisible(true);

	ScaleTo* pScale = ScaleTo::create(1.0f, 1.4f);
	FadeTo*  pFade = FadeTo::create(1.0f, 150);
	FiniteTimeAction* pEnd = CallFunc::create(CC_CALLBACK_0(CountDownLayer::displayOne, this));
	Spawn* pSpawn = Spawn::create(pScale, pFade, NULL);
	Sequence* seq = Sequence::create(pSpawn, pEnd, NULL);

	m_pTwo->runAction(seq);
}

void CountDownLayer::displayOne()
{
	m_pTwo->setVisible(false);
	m_pOne->setVisible(true);

	ScaleTo* pScale = ScaleTo::create(1.0f, 1.4f);
	FadeTo*  pFade = FadeTo::create(1.0f, 150);
	auto end = [this]()
	{
		PopupLayer::resumePlaneLayer();
		this->removeFromParent();
	};
	FiniteTimeAction* pEnd = CallFunc::create(end);
	Spawn* pSpawn = Spawn::create(pScale, pFade, NULL);
	Sequence* seq = Sequence::create(pSpawn, pEnd, NULL);

	m_pOne->runAction(seq);
}
