#pragma once

#include "Bmob/Gift.h"
#include "Bmob/ExchangeCode.h"
#include "Bmob/BmobSdk.h"
#include "cocos2d.h"

USING_NS_CC;

namespace ExchangeGiftConstant
{
	const int ErrCodeSuccess = 0;              //成功领取

	const int ErrCodeInvalidExchangeCode = -1; //无效的兑换码
	const int ErrCodeGiftReceived = -2;        //兑换码已被使用过
	const int ErrCodeQueryExchangeCode = -3;   //解析兑换码表错误
	const int ErrCodeQueryGiftId = -4;         //解析礼品表错误
	const int ErrCodeNoGift = -5;              //礼品表查不到记录
	const int ErrCodeRemarkReceivedDate = -10; //标记领取时间失败

	const char* const StrQueryExchangeCode = "querying_exchange_code";//正在连线礼品中心，请稍后...
	const char* const StrExchangeCodeUsed = "exchange_code_used";//很抱歉，该兑换码已被使用过
	const char* const StrInvalidExchangeCode = "invalid_exchange_code";//很抱歉，您输入的兑换码无效
	const char* const StrErroeQueryExchangeCode = "error_query_exchange_code";//很抱歉，无法查询您输入的兑换码，请检查网络是否正常
	const char* const StrUnknowError = "unknown_error";//很抱歉，发生未知错误，请重启游戏后重新尝试

}

class ExchangeGift : public Ref
{
public:
	ExchangeGift();
	virtual ~ExchangeGift();

	void exchange(const std::string& code, 
		std::map<std::string, int>* pGiftMap,
		const std::function<void(int errorCode)>& callback);

protected:
	void queryExchangeCode(const std::string& code);
	void queryGift(int giftId);
	void updateExchangeCodeDate(ExchangeCode* ec);

private:
	std::map<std::string, int>* m_pGiftMap;
	std::function<void(int errorCode)> m_callback;

	ExchangeCode* m_pExchangeCode;
	Vector<Gift*> m_giftArray;
};