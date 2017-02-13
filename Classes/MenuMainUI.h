#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "IListener.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PlayerPlane;

class MenuMainUI :
	public Layer, 
	public ScrollViewDelegate,
	public IListener
{
public:
	static Scene* scene();

	MenuMainUI();
	virtual ~MenuMainUI();

	CREATE_FUNC(MenuMainUI);

	virtual bool init();
	virtual void onEnter();

	void SelectPlane(int playerId);
	void scrollImage(Ref* sender);

	virtual void updateValue(const std::string& type, int value) override;
	void displayUnlockPrompt(const std::string& prompt);

protected:
	void moveCenterDone(PlayerPlane* p);
	void updateScoreAndMoney(int &money, int &score);
	void updateAircraftAttr();

	void updateButtonState();
	void updateVipState();
	void showVipOffline();

	void addKeyboardListener();
	void removeKeyboardListener();

	void unlockPlane();

protected:
	Node* m_pRootNode;
	Node* m_pRootNodeVip;

	//三种游戏模式
	ui::Button* m_pBtnLevel;
	ui::Button* m_pBtnEndless;
	ui::Button* m_pBtnRapidly;

	ui::Button* m_pBtnUnlock;

	ui::TextBMFont* m_pUnlockPrompt;

	//菜单按钮
	ui::Button* m_pBtnShop;
	ui::Button* m_pBtnSetup;
	ui::Button* m_pBtnBag;
	ui::Button* m_pBtnStrengthen;

	//左右选飞机
	ui::Button* m_pBtnArrowL;
	ui::Button* m_pBtnArrowR;

	//分数、宝石
	ui::TextAtlas* m_pTextScore;
	ui::TextAtlas* m_pTextMoney;

	//战机评级
	ui::TextAtlas* m_pTextAttack;
	ui::TextAtlas* m_pTextMissile;
	ui::TextAtlas* m_pTextArmor;
	ui::TextAtlas* m_pTextWingman;

	//战机介绍
	ui::TextBMFont* m_pTextAircraftTitle;
	ui::TextBMFont* m_pTextAircraftDesc;

	//战机评级条
	ui::LoadingBar* m_pBarAttack;
	ui::LoadingBar* m_pBarMissile;
	ui::LoadingBar* m_pBarArmor;
	ui::LoadingBar* m_pBarWingman;

	ui::Button* m_pBtnGift;
	ui::Button* m_pBtnReward;

	//vip
	ui::Button* m_pBtnVip;
	ui::TextAtlas* m_pTxtVipDays;

	EventListenerKeyboard* m_pKeyboardListener;

	Size m_visibleSize;
	int m_pageNumber; //背包第几页
	int m_playerNumber;
	bool m_menuFlg;
	bool m_moveFlg;

	int m_money;
	int m_score;

	//奖励提醒
	Sprite* m_pNewAchievement;

	std::vector<Sprite*> m_indicatorArray;
	std::vector<PlayerPlane*> m_playerArray;

};