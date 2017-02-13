#pragma once

#include "Goods.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"

USING_NS_CC;

using namespace cocostudio;

class MenuShopLayer :
	public PopupLayer,
	public PayResultListener
{
public:
	static Scene* scene();

	MenuShopLayer();

	virtual bool doInit();

	CREATE_FUNC(MenuShopLayer);

	virtual void notify(const TGoodsData* pGoodsData, EPayResult result) override;

protected:
	void displayBuyPrompt(EPayResult result);
	std::string toString(int d, const char* const format = "%d");

protected:
	Node* m_pRootNode;

	ui::Button* m_pBtnClose;

	//超值礼包
	ui::Button* m_pBtnMidGift;
	Sprite* m_pSpriteMidGiftBg;
	ui::TextAtlas* m_pTxtMidGiftPrice1;
	ui::TextAtlas* m_pTxtMidGiftPrice2;
	ui::TextAtlas* m_pTxtMidGiftMoney;
	ui::TextAtlas* m_pTxtMidGiftKiller;
	ui::TextAtlas* m_pTxtMidGiftShield;
	ui::TextAtlas* m_pTxtMidGiftLife;

	//豪华礼包
	ui::Button* m_pBtnBigGift;
	Sprite* m_pSpriteBigGiftBg;
	ui::TextAtlas* m_pTxtBigGiftPrice1;
	ui::TextAtlas* m_pTxtBigGiftPrice2;
	ui::TextAtlas* m_pTxtBigGiftMoney;
	ui::TextAtlas* m_pTxtBigGiftKiller;
	ui::TextAtlas* m_pTxtBigGiftShield;
	ui::TextAtlas* m_pTxtBigGiftLife;

	//宝石
	ui::Button* m_pBtnMoney;
	Sprite* m_pSpriteMoneyBg;
	ui::TextAtlas* m_pTxtMoneyPrice1;
	ui::TextAtlas* m_pTxtMoneyPrice2;
	ui::TextAtlas* m_pTxtMoneyMoney;

	//必杀
	ui::Button* m_pBtnKiller;
	Sprite* m_pSpriteKillerBg;
	ui::TextAtlas* m_pTxtKillerPrice1;
	ui::TextAtlas* m_pTxtKillerPrice2;
	ui::TextAtlas* m_pTxtKillerKiller;

	//护盾
	ui::Button* m_pBtnShield;
	Sprite* m_pSpriteShieldBg;
	ui::TextAtlas* m_pTxtShieldPrice1;
	ui::TextAtlas* m_pTxtShieldPrice2;
	ui::TextAtlas* m_pTxtShieldShield;

	Sprite* m_pBuySuccess;
	Sprite* m_pBuyFailed;
};