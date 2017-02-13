#include "Goods.h"
#include "Analytics.h"
#include "GlobalData.h"
#include "GameData.h"
#include "BillingInterface.h"
#include "GameScene.h"
#include "Carrier.h"
#include "Common/Macro.h"

std::map<std::string, PayResultListener*> Goods::m_payResultListenerMap;

bool Goods::pay(const char* const goods, PayResultListener* listener)
{
	const TGoodsData* pData = GlobalData::getInstance()->getGoods(goods);


	if (pData->currency == CurrencyType::CASH)
	{
		Analytics::getInstance()->event(AnalyticsEvent::ClickBuy, pData->goods.c_str());

		//调用话费支付接口
		m_payResultListenerMap.insert(std::map<std::string, PayResultListener*>::value_type(goods, listener));

		BillingImpl::getInstance()->pay(pData->billIndex, pData->goods, pData->name, pData->price, pData->desc);
	}
	else if (pData->currency == CurrencyType::COIN)
	{

		int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);

		//余额不足
		if (money < pData->price)
		{
			listener->notify(pData, EPayResult::NotEnoughCoin);
			return false;
		}
		else //扣减虚拟币
		{
			GameData::getInstance()->incValue(GAMEDATA::MONEY, -1 * pData->price);
			listener->notify(pData, EPayResult::SUCCESS);

			//统计
			Analytics::getInstance()->buy(pData->goods.c_str(), 1, pData->price);
		}

	}

	return true;
}

bool Goods::payResult(const char* const goods, BillingResult result)
{
	//判断计费类型
	BillingType billingType = Carrier::getBillingType();
	switch (billingType)
	{
	case BillingTypeChinaMobile:
	case BillingTypeChinaMobileCh:
		billingType = BillingTypeChinaMobile;
		break;
	case BillingTypeChinaTelcom:
	case BillingTypeChinaTelcomCh:
		billingType = BillingTypeChinaTelcom;
		break;
	case BillingTypeChinaUnicom:
	case BillingTypeChinaUnicomCh:
		billingType = BillingTypeChinaUnicom;
		break;
	//case BillingTypeXiaoMi:
	//	billingType = BillingTypeXiaoMi;
	//	break;
	case BillingTypeYijie:
		billingType = BillingTypeYijie;
		break;
	case BillingTypeThreeNet:
		billingType = (BillingType)Carrier::getCarrierType();
		if (BillingTypeChinaMobile != billingType
			&& BillingTypeChinaTelcom != billingType
			&& BillingTypeChinaUnicom != billingType
			&& BillingTypeSimulate != billingType)
		{
			billingType = BillingTypeChinaMobile;
		}
		break;
	case BillingTypeSimulate:
		billingType = BillingTypeSimulate;
		break;
	default:
		break;
	}
	int source = EPayChannel::UnknownPayChannel;
	switch (billingType)
	{
	case BillingTypeChinaMobile: source = EPayChannel::Cmcc; break;
	case BillingTypeChinaTelcom: source = EPayChannel::Telcom; break;
	case BillingTypeChinaUnicom: source = EPayChannel::Unicom; break;
	//case BillingTypeXiaoMi: source = EPayChannel::Xiaomi; break;
	case BillingTypeYijie: source = EPayChannel::Yijie; break;
	case BillingTypeSimulate: source = EPayChannel::Simulation; break;
	default:
		source = EPayChannel::UnknownPayChannel; break;
	}

	//
	const TGoodsData* pData = GlobalData::getInstance()->getGoods(goods);

	auto it = m_payResultListenerMap.find(goods);
	if (it != m_payResultListenerMap.end())
	{
		if (BillingResult_SUCCESS == result)
		{
			it->second->notify(pData, EPayResult::SUCCESS);

			//统计
			if (source != EPayChannel::Simulation)
			{
				Analytics::getInstance()->event(AnalyticsEvent::BuySuccess, pData->goods.c_str());

				Analytics::getInstance()->pay(pData->price / 100.0f, source, pData->goods.c_str(), 1,
					pData->price / 100.0f);
			}
		}
		else
		{
			if (source != EPayChannel::Simulation)
			{
				Analytics::getInstance()->event(AnalyticsEvent::BuyFailed, pData->goods.c_str());
			}

			it->second->notify(pData, EPayResult::Cancel);
		}

		m_payResultListenerMap.erase(it);
	}

	return true;
}

void Goods::pay(int coin, const char* name, const std::function<void()>& successlistener, const std::function<void()>& errorlistener)
{
	int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);

	//余额不足
	if (money < coin)
	{
		errorlistener();
	}
	else
	{
		Analytics::getInstance()->use(name, 1, coin);

		GameData::getInstance()->incValue(GAMEDATA::MONEY, -coin);
		successlistener();
	}

}

void Goods::getGoods(const TGoodsData* pGoodsData)
{
	//因不同运营商同一个计费点的金额可能不同，购买的物品也可能不同，所以这里调用Carrier类
	Carrier::getInstance()->getGoods(pGoodsData);
}
