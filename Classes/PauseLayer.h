#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"

USING_NS_CC;

class PlaneLayer;

class PauseLayer : public PopupLayer
{
public:
	virtual bool doInit();

	CREATE_FUNC(PauseLayer);

	void menuContinueCallback(Ref* pSender, ui::Widget::TouchEventType type);
	void menuBackToMainMenuCallback(Ref* pSender, ui::Widget::TouchEventType type);
	void menuSoundCallback(Ref* pSender, ui::Widget::TouchEventType type);
	void menuShopCallback(Ref* pSender, ui::Widget::TouchEventType type);
	void menuGiftCompositionCallback(Ref* pSender, ui::Widget::TouchEventType type);
	void menuGiftShieldCallback(Ref* pSender, ui::Widget::TouchEventType type);
	void menuKillerCallback(Ref* pSender, ui::Widget::TouchEventType type);

private:
	ui::Button* m_pBtnGift;
	ui::Button* m_pBtnResume;
	ui::Button* m_pBtnMainMenu;
	ui::Button* m_pBtnSoundOn;
	ui::Button* m_pBtnSoundOff;
	ui::Button* m_pBtnGiftKiller;
	ui::Button* m_pBtnGiftShield;
};