#pragma once

#include "cocos2d.h"

enum EPayChannel
{
	UnknownPayChannel = -1,

	AppStore = 1,
	Alipay = 2,
	NetBank = 3,
	Tenpay = 4,
	Cmcc = 5,
	Unicom = 6,
	Telcom = 7,
	Paypal = 8,

	Xiaomi = 11,
	Yijie = 12,

	Simulation = 99
};

namespace AnalyticsEventKey
{
	const char* const Item = "Item";
	const char* const Diamond = "Diamond";
	const char* const Cash = "Cash";
};

namespace AnalyticsEvent
{
	//急速模式 飞行距离
	const char* const RapidDistnace = "RapidDistnace";
	//全局强化	项目	宝石数
	const char* const StrengthenGlobal = "StrengthenGlobal";
	//仓库强化	项目	宝石数
	const char* const StrengthenEquip = "StrengthenEquip";

	//仓库购买	项目	宝石数
	const char* const BuyEquip = "BuyEquip";

	//弹出计费窗	次数
	const char* const OpenBillingDlg = "OpenBillingDlg";
	//点击购买	次数
	const char* const ClickBuy = "ClickBuy";
	//购买成功	次数	金额
	const char* const BuySuccess = "BuySuccess";
	//购买失败	次数	金额
	const char* const BuyFailed = "BuyFailed";

	//签名
	const char* const FingerPrint = "FingerPrint";
}

class Analytics
{
public:

	static Analytics* getInstance();

	// 玩家支付货币兑换虚拟币,用于统计游戏的收入情况
	// 充值.
	virtual void pay(double cash, int source, double coin);
	// 充值并购买道具.
	virtual void pay(double cash, int source, const char * item, int amount, double price);

	// 购买道具.玩家用虚拟币兑换一定数量、价值的道具
	virtual void buy(const char *item, int amount, double price);

	// 消耗道具.玩家使用道具的情况
	virtual void use(const char *item, int amount, double price);

	//针对游戏中额外获得的虚拟币进行统计，比如系统赠送，节日奖励，打怪掉落。
	// 赠送金币.
	virtual void bonus(double coin, int source);
	// 赠送道具.
	virtual void bonus(const char *item, int amount, double price, int source);

	// 进入关卡.
	virtual void startLevel(const char *level);
	// 通过关卡.
	virtual void finishLevel(const char *level);
	// 未通过关卡.
	virtual void failLevel(const char *level);

	//事件数量统计
	virtual void event(const char * eventId, const char * label = NULL);

protected:
	Analytics();

	virtual ~Analytics();

private:

	static Analytics* m_AnalyticsImpl;
};

class LocalAnalytics : public Analytics
{
	// 玩家支付货币兑换虚拟币,用于统计游戏的收入情况
	// 充值.
	virtual void pay(double cash, int source, double coin) override;
	// 充值并购买道具.
	virtual void pay(double cash, int source, const char * item, int amount, double price) override;

	// 购买道具.玩家用虚拟币兑换一定数量、价值的道具
	virtual void buy(const char *item, int amount, double price) override;

	// 消耗道具.玩家使用道具的情况
	virtual void use(const char *item, int amount, double price) override;

	//针对游戏中额外获得的虚拟币进行统计，比如系统赠送，节日奖励，打怪掉落。
	// 赠送金币.
	virtual void bonus(double coin, int source) override;
	// 赠送道具.
	virtual void bonus(const char *item, int amount, double price, int source) override;

	// 进入关卡.
	virtual void startLevel(const char *level) override;
	// 通过关卡.
	virtual void finishLevel(const char *level) override;
	// 未通过关卡.
	virtual void failLevel(const char *level) override;

	//事件数量统计
	virtual void event(const char * eventId, const char * label = NULL) override;
};

