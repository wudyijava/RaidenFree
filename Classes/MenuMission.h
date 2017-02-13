#pragma once

#include "cocos2d.h"  
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class SpriteWithHue;

typedef struct
{
	ui::ImageView* bg;
	ui::TextAtlas* num;
	Sprite* star1;
	Sprite* star2;
	Sprite* star3;
	Sprite* line;
}MissionNode;

class MenuMission :
	public Layer
{
public:
	static Scene* scene();

	MenuMission();
	virtual ~MenuMission();

	virtual bool init();
	virtual void onEnter();

	CREATE_FUNC(MenuMission);

protected:
	void addKeyboardListener();
	void removeKeyboardListener();

	void updateDifficulty(int difficulty);
	void updateLevelStar();
	void updateBg();

	void ToPlaneMenu();
	void ToGame();
	void ToShop();

	void missionForbidden();

protected:
	Node* m_pRootNode;

	ui::ScrollView* m_pScrollView;

	//三种游戏难度
	ui::CheckBox* m_pCbDifficulty_1;
	ui::CheckBox* m_pCbDifficulty_2;
	ui::CheckBox* m_pCbDifficulty_3;

	ui::Button* m_pBtnReturn;
	ui::Button* m_pBtnWarehouse;

	Sprite* m_pSpritePlayer;

	unsigned m_levelMax;
	unsigned m_levelFinish;

	bool m_bInit;

	std::vector<SpriteWithHue*> m_BgArray;

	std::vector<MissionNode> m_missionArray;

	EventListenerKeyboard* m_pKeyboardListener;
};