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

	//ǿ������
	ui::Text* propName;
	ui::TextAtlas* propValue;
	ui::LoadingBar* propProgress;

	//��Ƭ
	Sprite* piece;
	ui::TextAtlas* pieceCnt;

	//ǿ��������Ľ��
	Sprite* currency;
	ui::TextAtlas* price;

	//ǿ����װ�������򡢺ϳɰ�ť
	ui::Button* btnStrengthen;
	ui::Button* btnBuy;
	ui::Button* btnEquip;
	ui::Button* btnCompose;

	//˵��
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

	//��������ʯ
	ui::TextAtlas* m_pTextScore;
	ui::TextAtlas* m_pTextMoney;

	//��ǰ��ʾ��tab
	EquipType m_curEquipType;
	EquipType m_lastEquipType;

	std::vector<BagItem*> m_bagItemArray;
};