#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

typedef struct
{
	Sprite* bg;
	Sprite* gift;
	Sprite* received;
	ui::TextAtlas* day;
	ui::TextAtlas* num;
}DailyGiftNode;

class MenuDayGift : public Layer
{
public:
	static bool isDisplayDailyGift();
	static void setTimeVal(int now, int continueDays);
	static time_t truncTime(time_t t);

	static Scene* scene();

	MenuDayGift();
	virtual ~MenuDayGift();
	virtual bool init();

	CREATE_FUNC(MenuDayGift);

	void SetDataVal();
	void updateUI();

	void login(float dt = 0);

private:
	Node* m_pRootNode;

	ui::Button* m_pBtnReceive;
	ui::Button* m_pBtnVip;
	ui::Button* m_pBtnVipContinue;
	ui::Button* m_pBtnGiveup;

	ui::TextAtlas* m_pTextDay;
	ui::ScrollView* m_pScrollView;

	std::vector<DailyGiftNode*> m_giftArray;

	static int m_day;
	static int m_now;

	std::string m_prompt;
};