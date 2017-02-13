#include "ExchangeGift.h"
#include "Gift.h"
#include "ExchangeCode.h"
#include "GameData.h"
#include "Common\Macro.h"

ExchangeGift::ExchangeGift()
	: m_pExchangeCode(nullptr)
{

}

ExchangeGift::~ExchangeGift()
{
	if (m_pExchangeCode)
	{
		m_pExchangeCode->release();
		m_pExchangeCode = nullptr;
	}
	m_giftArray.clear();
}

void ExchangeGift::exchange(const std::string& code, 
	std::map<std::string, int>* pGiftMap, 
	const std::function<void(int code)>& callback)
{
	m_callback = callback;
	m_pGiftMap = pGiftMap;

	queryExchangeCode(code);
}

void ExchangeGift::queryExchangeCode(const std::string& code)
{
	//查询兑换码
	ExchangeCode::queryOneByCode(code,
		[this](const void* data){
		//success
		DEBUG_LOG("ExchangeGift::queryExchangeCode() : %s", (const char*)data);

		m_pExchangeCode = new ExchangeCode;
		m_pExchangeCode->autorelease();
		m_pExchangeCode->retain();

		if (m_pExchangeCode->parse((const char*)data))
		{
			if (m_pExchangeCode->getReceiveDate().length() > 0)
			{
				DEBUG_LOG("Fuck!!! Gifts have been received!");
				m_callback(ExchangeGiftConstant::ErrCodeGiftReceived);
			}
			else
			{
				this->queryGift(m_pExchangeCode->getGiftId());
			}
		}
		else
		{
			//解析“兑换码”表错误
			m_callback(ExchangeGiftConstant::ErrCodeInvalidExchangeCode);
		}
	},
		[this](int code, const void* data){
		//error
		const char* p = (const char*)data;
		DEBUG_LOG("ExchangeGift::queryExchangeCode(%d): %s", code, p);

		m_callback(ExchangeGiftConstant::ErrCodeQueryExchangeCode);
	});
}

void ExchangeGift::queryGift(int giftId)
{
	//查询礼品明细
	Gift::queryOneByGiftId(giftId,
		[this](const void* data){
		//success
		DEBUG_LOG("Gift::queryOneByGiftId() : %s", (const char*)data);

		if (Gift::parseJosnToArray((const char*)data, m_giftArray))
		{
			this->updateExchangeCodeDate(m_pExchangeCode);
		}
		else
		{
			m_callback(ExchangeGiftConstant::ErrCodeNoGift);
		}
	},
		[this](int code, const void* data){
		//error
		const char* p = (const char*)data;
		DEBUG_LOG("Gift::queryOneByGiftId(%d): %s", code, p);

		m_callback(ExchangeGiftConstant::ErrCodeQueryGiftId);
	});
}

void ExchangeGift::updateExchangeCodeDate(ExchangeCode* ec)
{
	struct tm *today;
	time_t now = time(nullptr);
	today = localtime(&now);
	char szTimeString[128] = { 0 };
	strftime(szTimeString, 128, "%Y-%m-%d %H:%M:%S", today);

	//标记礼品领取时间
	ec->updateReceiveDate(szTimeString,
		[this, ec](const void* data){
		//success
		DEBUG_LOG("updateReceiveDate() : %s", (const char*)data);

		//领取礼品
		m_pGiftMap->clear();
		for (auto g : m_giftArray)
		{
			m_pGiftMap->insert(std::map<std::string, int>::value_type(g->getGiftName(), g->getCount()));
		}

		m_callback(ExchangeGiftConstant::ErrCodeSuccess);
	},
		[this, ec](int code, const void* data){
		//error
		const char* p = (const char*)data;
		DEBUG_LOG("updateReceiveDate(%d): %s", code, p);

		m_callback(ExchangeGiftConstant::ErrCodeRemarkReceivedDate);
	});

}
