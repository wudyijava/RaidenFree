#pragma once

#include "cocos2d.h"
#include "BmobSdk.h"

USING_NS_CC;

class ExchangeCode : public BmobObject
{
public:
	ExchangeCode();
	virtual ~ExchangeCode();

	bool parse(const char* const jsonString);

	static void queryOneByCode(const std::string& code,
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc);

	void updateReceiveDate(const std::string& date,
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc);

protected:

	CC_SYNTHESIZE(std::string, m_code, Code);
	CC_SYNTHESIZE(int, m_iGiftId, GiftId);
	CC_SYNTHESIZE(string, m_receiveDate, ReceiveDate);
};
