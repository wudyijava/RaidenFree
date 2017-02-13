#pragma once

#include "cocos2d.h"  
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Equipment.h"
#include "PopupLayer.h"
#include "IListener.h"

USING_NS_CC;

typedef struct
{
	Sprite* bg;
	Sprite* img;
	ui::Text* name;

	//强化属性
	ui::Text* propName;
	ui::TextAtlas* propValue;
	ui::LoadingBar* propProgress;

	//碎片
	Sprite* piece;
	ui::TextAtlas* pieceCnt;

	//强化、购买的金额
	Sprite* currency;
	ui::TextAtlas* price;

	//强化、装备、购买、合成按钮
	ui::Button* btnStrengthen;
	ui::Button* btnBuy;
	ui::Button* btnEquip;
	ui::Button* btnCompose;

	//说明
	Sprite* remark;
}BagItem;

class MenuBag : public PopupLayer, public IListener
{
public:
	static Scene* scene();

	MenuBag();
	virtual ~MenuBag();

	virtual bool doInit();

	CREATE_FUNC(MenuBag);

	virtual void removeFromParent() override;
	virtual void updateValue(const std::string& type, int value) override;

protected:
	void updateTitle();
	void updateList();
	void updateScoreAndMoney();

protected:
	Node* m_pRootNode;

	ui::ScrollView* m_pScrollView;

	ui::CheckBox* m_pCbTitleAircraft;
	ui::CheckBox* m_pCbTitleArmature;
	ui::CheckBox* m_pCbTitleMissile;
	ui::CheckBox* m_pCbTitleWingman;

	ui::Button* m_pBtnClose;

	//分数、宝石
	ui::TextAtlas* m_pTextScore;
	ui::TextAtlas* m_pTextMoney;

	//当前显示的tab
	EquipType m_curEquipType;
	EquipType m_lastEquipType;

	std::vector<BagItem*> m_bagItemArray;
};