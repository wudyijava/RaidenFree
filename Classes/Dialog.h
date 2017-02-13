#pragma once

#include "PopupLayer.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

struct TDialogData
{
	int id;
	int type;
	std::string name;
	std::string pic;
	std::string content;
};

struct DialogBoxItem
{
	Sprite* box;
	Sprite* pic;
	ui::Text* name;
	ui::Text* content;
};

class Dialog : public PopupLayer
{
public:
	static Layer* show(Node* pParent, const TDialogData* pData, bool nextDialog);
	CREATE_FUNC(Dialog);

public:
	virtual bool doInit();

	void showDialog(const TDialogData* pData, bool nextDialog);

	Dialog();
	virtual ~Dialog();

	virtual void onTouchEnded(Touch* touch, Event* event) override;
	virtual std::function<void(Touch*, Event*)> getTouchEndedFunc() { return CC_CALLBACK_2(Dialog::onTouchEnded, this); }

protected:
	Node* m_pRootNode;
	std::vector<DialogBoxItem> m_boxArray;
	bool m_nextDialog;
};