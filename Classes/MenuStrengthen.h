#pragma once

#include "cocos2d.h"  
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"
#include "IListener.h"

USING_NS_CC;

struct StrengthenItem
{
	ui::CheckBox* node;
	ui::LoadingBar* grade;
};

class MenuStrengthen :public PopupLayer, public IListener
{
public:
	friend class TutorialUpgradeLayer;

	static Scene* scene();

	MenuStrengthen(void);
	virtual ~MenuStrengthen(void);

	virtual bool doInit();

	CREATE_FUNC(MenuStrengthen);

	virtual void onEnter() override;
	virtual void removeFromParent() override;
	virtual void updateValue(const std::string& type, int value) override;

protected:

	//显示强化黄/红方块
	void updateStrengthenGrade(int item);
	//显示强化信息
	void updateStrengthenInfo(int item);

	//强化
	void doStrengthen(int item);
	//更改强化等级数据
	void SetSystemUpgradeVal();

	void updateScoreAndMoney();

protected:
	Node* m_pRootNode;

	ui::Button* m_pBtnClose;
	ui::Button* m_pBtnStrengthen;

	ui::Text* m_pStrengthenInfo;
	ui::Text* m_pStrengthenPrice;

	//分数、宝石
	ui::TextAtlas* m_pTextScore;
	ui::TextAtlas* m_pTextMoney;

	std::vector<StrengthenItem> m_itemArray;
	std::vector<int> m_gradeArray;

	int m_curItem;
};