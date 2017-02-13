#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class FakeLoadingLayer : public Layer
{
public:
	static Scene* scene();
	CREATE_FUNC(FakeLoadingLayer);

	FakeLoadingLayer();
	virtual bool init();

private:
	ui::Text* m_pTxtInfo;

};
