#pragma once

#include "cocos2d.h"
#include "Goods.h"
#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PlaneLayer;
struct TGoodsData;

class RelifeLayer : public PopupLayer, public PayResultListener
{
public:
	static RelifeLayer* create()
	{
		RelifeLayer *pRet = new(std::nothrow) RelifeLayer();
		if (pRet && pRet->init())
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

	RelifeLayer();
	virtual bool doInit() { return true; }
	virtual bool init();

	void setCancelCallback(const std::function<void()>& callback);
	const std::function<void()>& getCancelCallback();

	void pay();
	virtual void notify(const TGoodsData* pGoodsData, EPayResult result) override;
	void doAfterPay(const char* const goods, EPayResult result);

	virtual void removeFromParent() override;

    void updateUI();

protected:
	Node* m_pRootNode;

	Sprite* m_pBg;

	ui::Button* m_pBtnBuy;   //普通复活
	ui::Button* m_pBtnVipBuy;//VIP复活
	ui::Button* m_pBtnCancel;//关闭

	ui::TextBMFont* m_pTxtLevelLeft;
	ui::TextBMFont* m_pTxtVipRelifeLeft;

	ui::TextAtlas* m_pTxtPriceNum1;
	ui::TextAtlas* m_pTxtPriceNum2;

	Sprite* m_pSpritePrice;
	Sprite* m_pSpriteFree;

	Sprite* m_pBuySuccess;
	Sprite* m_pBuyFail;
	
	bool m_bVipFreeRelife;

	std::function<void()> m_cancelCallback;
};