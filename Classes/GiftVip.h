#pragma once

#include "cocos2d.h"
#include "Goods.h"
#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PlaneLayer;
struct TGoodsData;

//VIP特权
namespace VipConstant
{
	const int DailyGiftRatio = 2;   //每日签到奖励x2
	const float MoneyReward = 1.2f; //击落敌机获得的宝石x1.2
	const float RandomPiece = 1.0f; //碎片掉落随机增加的基数
	const float RelifeAttributeRatio = 1.5f; //复活后战斗力x1.5
	const int RelifeCnt = 2; //每一关可免费复活两次
}

class GiftVipLayer : public PopupLayer, public PayResultListener
{
public:
	static GiftVipLayer* create()
	{
		GiftVipLayer *pRet = new(std::nothrow) GiftVipLayer();
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

	GiftVipLayer();
	virtual bool doInit() { return true; }
	virtual bool init();

	void setCancelCallback(const std::function<void()>& callback);
	const std::function<void()>& getCancelCallback();

	void pay();
	virtual void notify(const TGoodsData* pGoodsData, EPayResult result) override;
	void doAfterPay(const char* const goods, EPayResult result);

	virtual void removeFromParent() override;

    void updateUI();

	void login(float dt = 0);

protected:
	Node* m_pRootNode;

	ui::Button* m_pBtnBuy; //购买
	ui::Button* m_pBtnBuy2;//续费
	ui::Button* m_pBtnCancel;//关闭

	Sprite* m_pSpriteLogo;
	Sprite* m_pSpriteLogoDisable;
	Sprite* m_pSpriteLogoDisableTxt1;
	Sprite* m_pSpriteLogoDisableTxt2;

	ui::TextAtlas* m_pTxtPriceNum1;
	ui::TextAtlas* m_pTxtPriceNum2;

	ui::TextBMFont* m_pTxtVipTimeLeft;

	Sprite* m_pBuySuccess;
	Sprite* m_pBuyFail;

	std::function<void()> m_cancelCallback;
};