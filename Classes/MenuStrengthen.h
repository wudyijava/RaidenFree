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

	//��ʾǿ����/�췽��
	void updateStrengthenGrade(int item);
	//��ʾǿ����Ϣ
	void updateStrengthenInfo(int item);

	//ǿ��
	void doStrengthen(int item);
	//����ǿ���ȼ�����
	void SetSystemUpgradeVal();

	void updateScoreAndMoney();

protected:
	Node* m_pRootNode;

	ui::Button* m_pBtnClose;
	ui::Button* m_pBtnStrengthen;

	ui::Text* m_pStrengthenInfo;
	ui::Text* m_pStrengthenPrice;

	//��������ʯ
	ui::TextAtlas* m_pTextScore;
	ui::TextAtlas* m_pTextMoney;

	std::vector<StrengthenItem> m_itemArray;
	std::vector<int> m_gradeArray;

	int m_curItem;
};