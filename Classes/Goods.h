#pragma once

#include "cocos2d.h"
#include "BillingInterface.h"

USING_NS_CC;

namespace GoodsType
{
	const char* const DIAMOND_1W = "diamond_1w";
	const char* const LIFE_1 = "life_1";
	const char* const KILLER_5 = "killer_5";
	const char* const SHIELD_5 = "shield_5";
	const char* const LIFE_PKG = "life_pkg";
	const char* const NEW_GIFT = "new_gift";
	const char* const MID_GIFT = "mid_gift";
	const char* const BIG_GIFT = "big_gift";
	const char* const HUG_GIFT = "hug_gift";
	const char* const PLANE_4TH = "plane_4th";
	const char* const PLANE_5TH = "plane_5th";
	const char* const PLANE_6TH = "plane_6th";
	const char* const PLANE_5TH_PKG = "plane_5th_pkg";
	const char* const PLANE_6TH_PKG = "plane_6th_pkg";
	const char* const VIP = "vip";
}

namespace CurrencyType
{
	const char* const COIN = "coin";  //虚拟货币
	const char* const CASH = "cash";  //现金
}

enum EPayResult
{
	SUCCESS = 0,
	NotEnoughCoin = 1001,
	Cancel = 1002
};

struct TGoodsData
{
	int id;
	std::string goods;
	int orignPrice;
	int price;
	std::string currency;
	std::string name;
	std::string billIndex;
	std::string desc;

	std::map<std::string, int>* itemMap;
};

struct TGoodsItemData
{
	int id;
	int goodsId;
	std::string key;
	int value;
};

class PayResultListener
{
public:
	virtual void notify(const TGoodsData* pGoodsData, EPayResult result) {}
};

class Goods
{
public: 
	//话费支付+虚拟币支付
	static bool pay(const char* const goods, PayResultListener* listener);
	static bool payResult(const char* const goods, BillingResult result);

	static void getGoods(const TGoodsData* pGoodsData);

	//虚拟币支付
	static void pay(int coin, const char* name, const std::function<void()>& successlistener, const std::function<void()>& errorlistener);

private:
	static std::map<std::string, PayResultListener*> m_payResultListenerMap;
};

