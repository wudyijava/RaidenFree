#include "Analytics.h"
#include "Common/Macro.h"

Analytics* Analytics::m_AnalyticsImpl = NULL;

Analytics* Analytics::getInstance()
{
	if (!m_AnalyticsImpl)
	{
		m_AnalyticsImpl = new LocalAnalytics;
	}

	return m_AnalyticsImpl;
}

Analytics::Analytics()
{

}

Analytics::~Analytics()
{

}

// 玩家支付货币兑换虚拟币,用于统计游戏的收入情况
// 充值.
void Analytics::pay(double cash, int source, double coin)
{
	m_AnalyticsImpl->pay(cash, source, coin);
}

// 充值并购买道具.
void Analytics::pay(double cash, int source, const char * item, int amount, double price)
{
	m_AnalyticsImpl->pay(cash, source, item, amount, price);
}

// 购买道具.玩家用虚拟币兑换一定数量、价值的道具
void Analytics::buy(const char *item, int amount, double price)
{
	m_AnalyticsImpl->buy(item, amount, price);
}

// 消耗道具.玩家使用道具的情况
void Analytics::use(const char *item, int amount, double price)
{
	m_AnalyticsImpl->use(item, amount, price);
}

//针对游戏中额外获得的虚拟币进行统计，比如系统赠送，节日奖励，打怪掉落。
// 赠送金币.
void Analytics::bonus(double coin, int source)
{
	m_AnalyticsImpl->bonus(coin, source);
}

// 赠送道具.
void Analytics::bonus(const char *item, int amount, double price, int source)
{
	m_AnalyticsImpl->bonus(item, amount, price, source);
}

// 进入关卡.
void Analytics::startLevel(const char *level)
{
	m_AnalyticsImpl->startLevel(level);
}

// 通过关卡.
void Analytics::finishLevel(const char *level)
{
	m_AnalyticsImpl->finishLevel(level);
}

// 未通过关卡.
void Analytics::failLevel(const char *level)
{
	m_AnalyticsImpl->failLevel(level);
}

//事件数量统计
void Analytics::event(const char * eventId, const char * label)
{
	m_AnalyticsImpl->event(eventId, label);
}

// 玩家支付货币兑换虚拟币,用于统计游戏的收入情况
// 充值.
void LocalAnalytics::pay(double cash, int source, double coin)
{
	DEBUG_LOG("LocalAnalytics::pay : cash = %.2f, source = %d, coin = %.2f", cash, source, coin);
}

// 充值并购买道具.
void LocalAnalytics::pay(double cash, int source, const char * item, int amount, double price)
{
	DEBUG_LOG("LocalAnalytics::pay : cash = %.2f, source = %d, item = %s, amount = %d, price = %.2f", 
		cash, source, item, amount, price);
}

// 购买道具.玩家用虚拟币兑换一定数量、价值的道具
void LocalAnalytics::buy(const char *item, int amount, double price)
{
	DEBUG_LOG("LocalAnalytics::buy : item = %s, amount = %d, price = %.2f", item, amount, price);
}

// 消耗道具.玩家使用道具的情况
void LocalAnalytics::use(const char *item, int amount, double price)
{
	DEBUG_LOG("LocalAnalytics::use : item = %s, amount = %d, price = %.2f", item, amount, price);
}

//针对游戏中额外获得的虚拟币进行统计，比如系统赠送，节日奖励，打怪掉落。
// 赠送金币.
void LocalAnalytics::bonus(double coin, int source)
{
	DEBUG_LOG("LocalAnalytics::use : source = %d, coin = %.2f", source, coin);
}

// 赠送道具.
void LocalAnalytics::bonus(const char *item, int amount, double price, int source)
{
	DEBUG_LOG("LocalAnalytics::bonus : item = %s, amount = %d, price = %.2f, source = %d", item, amount, price, source);
}

// 进入关卡.
void LocalAnalytics::startLevel(const char *level)
{
	DEBUG_LOG("LocalAnalytics::startLevel : level = %s", level);
}

// 通过关卡.
void LocalAnalytics::finishLevel(const char *level)
{
	DEBUG_LOG("LocalAnalytics::finishLevel : level = %s", level);
}

// 未通过关卡.
void LocalAnalytics::failLevel(const char *level)
{
	DEBUG_LOG("LocalAnalytics::failLevel : level = %s", level);
}

//事件数量统计
void LocalAnalytics::event(const char * eventId, const char * label)
{
	DEBUG_LOG("LocalAnalytics::event : eventId = %s, label = %s", eventId, label);
}
