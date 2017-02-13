#pragma once

#include "cocos2d.h"
#include "BillingInterface.h"

USING_NS_CC;

////////////////////////////////////////////////////
//
//  统一计费
//
////////////////////////////////////////////////////
class UniBilling : public BillingInterface
{
public:
	//进入游戏时必须使用这个方法进行判断音乐是否开关
	virtual bool isMusicEnabled() override;

	//显示更多游戏
	virtual void moreGame() override;

	//支付接口 billingIndex:计费点编号
	virtual void pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc) override;

	//退出接口
	virtual bool exit() override;

public:
	static std::map<std::string, std::string> m_payIndexMap;
};