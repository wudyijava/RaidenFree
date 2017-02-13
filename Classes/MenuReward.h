#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

struct TAchievementData;

class MenuReward : public Layer
{
public:
	MenuReward();

	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(MenuReward);

protected:
	void updateList();
	void getReward(TAchievementData* pData, ui::Button* pBtnReceive);

private:
	Node* m_pRootNode;

	ui::Button* m_pBtnBack;

	ui::TextAtlas* m_pTextEnemy;
	ui::TextAtlas* m_pTextMoney;
	ui::TextAtlas* m_pTextScore;

	ui::ScrollView* m_pScrollView;
};