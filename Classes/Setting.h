#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"

USING_NS_CC;

class ExchangeGift;
class PromptLayer;

class SettingLayer : public PopupLayer
{
public:
	SettingLayer();
	~SettingLayer();

	virtual bool doInit() override;
	CREATE_FUNC(SettingLayer);

private:
	Node* m_pRootNode;

	ui::CheckBox* m_pCbSound;
	ui::Button* m_pBtnClose;

	ui::Button* m_pBtnSetup;
	ui::Button* m_pBtnHelp;
	ui::Button* m_pBtnAbout;

	ui::TextBMFont* m_pLableUserId;
	ui::TextBMFont* m_pLableVersion;

	ui::TextField* m_pExchangeCode;
	ui::Button* m_pBtnExchange;

	Node* m_pNodeSetup;
	Node* m_pNodeAbout;
	Node* m_pNodeHelp;

	std::map<std::string, int> m_giftMap;
	ExchangeGift* m_pExchangeGift;

	PromptLayer* m_pPromptLayer;
};