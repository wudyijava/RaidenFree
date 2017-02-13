#pragma once

#include "cocos2d.h"
#include "Goods.h"
#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PlaneLayer;
struct TGoodsData;

class GiftLayer : public PopupLayer, public PayResultListener
{
public:
	static GiftLayer* create(const char* const giftType)
	{
		GiftLayer *pRet = new(std::nothrow) GiftLayer();
		if (pRet && pRet->init(giftType))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

	GiftLayer();
	virtual bool doInit() { return true; }
	virtual bool init(const char* const eGiftType);

	void setCancelCallback(const std::function<void()>& callback);
	const std::function<void()>& getCancelCallback();

	virtual void notify(const TGoodsData* pGoodsData, EPayResult result) override;

	void doAfterPay(const char* const goods, EPayResult result);

	virtual void removeFromParent() override;

protected:
	void updateButtonOk();

protected:
	const char* m_eGiftType;

	Node* m_pRootNode;

	ui::Button* m_pBtnOk;
	ui::Button* m_pBtnCancel;

	Sprite* m_pBuySuccess;
	Sprite* m_pBuyFail;

	ui::TextBMFont* m_pTxtDiamondNum;
	ui::TextBMFont* m_pTxtLifeNum;
	ui::TextBMFont* m_pTxtShieldNum;
	ui::TextBMFont* m_pTxtKillerNum;

	ui::TextAtlas* m_pTxtPriceNum1;
	ui::TextAtlas* m_pTxtPriceNum2;
	ui::TextAtlas* m_pTxtPriceNum;
	ui::TextAtlas* m_pTxtPriceNumOirg; //Ô­¼Û

	ui::TextBMFont* m_pTxtMidGiftRecommend;
	ui::TextBMFont* m_pTxtBigGiftRecommend;
	ui::TextBMFont* m_pTxtLifeRecommend;
	ui::TextBMFont* m_pTxtLifePkgRecommend;

	ui::TextBMFont* m_pTxtNotEnoughMoney;

	Layer* m_pLayer;

	std::function<void()> m_cancelCallback;
};