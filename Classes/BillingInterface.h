#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct TGoodsData;

typedef enum
{
	BillingResult_SUCCESS   = 0,
	BillingResult_FAILED    = 1,
	BillingResult_CANCELLED = 2,
	BillingResult_TIMEOUT   = 3
}BillingResult;

////////////////////////////////////////////////////
//
//  抽象接口
//
////////////////////////////////////////////////////
class BillingInterface
{
public:
	//进入游戏时必须使用这个方法进行判断音乐是否开关
	virtual bool isMusicEnabled() = 0;

	//显示更多游戏
	virtual void moreGame() = 0;

	//支付接口 billingIndex:计费点编号
	virtual void pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc) = 0;

	//退出接口。返回true：已调用外部sdk退出接口；false：未调用外部sdk退出接口
	virtual bool exit() = 0;
};

////////////////////////////////////////////////////
//
//  实现类
//
////////////////////////////////////////////////////
class BillingImpl : public BillingInterface
{
public:
	static BillingInterface* getInstance();

	virtual bool isMusicEnabled() override;

	virtual void moreGame() override;

	virtual void pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc);

	virtual bool exit() override;

private:
	static BillingInterface* m_pInstance;
};
