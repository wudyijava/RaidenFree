#pragma once

#include "cocos2d.h"
#include "PopupLayer.h"

USING_NS_CC;

class CountDownLayer : public PopupLayer
{
public:
	virtual bool doInit();

	CREATE_FUNC(CountDownLayer);

	CountDownLayer();
	virtual ~CountDownLayer();

	void displayBuySuccess();

protected:
	void displayThree();
	void displayTwo();
	void displayOne();

private:
	Sprite* m_pOne;
	Sprite* m_pTwo;
	Sprite* m_pThree;

	Size m_visibleSize;

	bool m_bDisplayBuySuccess;
};