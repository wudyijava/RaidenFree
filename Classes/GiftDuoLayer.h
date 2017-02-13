#pragma once

#include "cocos2d.h"
#include "Goods.h"
#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PlaneLayer;
struct TGoodsData;

struct GoodsItem
{
	Sprite* m_pBg;

	ui::TextAtlas* m_pTxtDiamondNum;
	ui::TextAtlas* m_pTxtLifeNum;
	ui::TextAtlas* m_pTxtShieldNum;
	ui::TextAtlas* m_pTxtKillerNum;
	ui::TextAtlas* m_pTxtPriceNum1;
	ui::TextAtlas* m_pTxtPriceNum2;
	ui::Button* m_pBtnOk;

	GoodsItem()
		: m_pBg(nullptr)
		, m_pTxtDiamondNum(nullptr)
		, m_pTxtLifeNum(nullptr)
		, m_pTxtShieldNum(nullptr)
		, m_pTxtKillerNum(nullptr)
		, m_pTxtPriceNum1(nullptr)
		, m_pTxtPriceNum2(nullptr)
		, m_pBtnOk(nullptr)
	{

	}
};

class GiftDuoLayer : public PopupLayer, public PayResultListener
{
public:
	static GiftDuoLayer* create(const std::vector<const char*>& giftTypeArray)
	{
		GiftDuoLayer *pRet = new(std::nothrow) GiftDuoLayer();
		if (pRet && pRet->init(giftTypeArray))
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

	GiftDuoLayer();
	virtual bool doInit() { return true; }
	virtual bool init(const std::vector<const char*>& giftTypeArray);
	virtual bool initSubItem(const char* const node, const char* const goodsType);

	void setCancelCallback(const std::function<void()>& callback);
	const std::function<void()>& getCancelCallback();

	virtual void notify(const TGoodsData* pGoodsData, EPayResult result) override;

	void doAfterPay(const char* const goods, EPayResult result);

	virtual void removeFromParent() override;

protected:
	void updateButtonOk();
	void updateString(ui::TextAtlas* pWidget, const TGoodsData* pGoodData, const char* const name);

protected:
	Node* m_pRootNode;

	ui::Button* m_pBtnCancel;

	Sprite* m_pBuySuccess;
	Sprite* m_pBuyFail;

	std::vector<GoodsItem> m_goods;

	std::function<void()> m_cancelCallback;
};