#include "Carrier.h"
#include "Goods.h"
#include "GameData.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "Equipment.h"
#include "Common/Macro.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#endif

namespace CarrierConstant
{
	const int VipDays = 30;
}

Carrier* Carrier::m_pInstance = nullptr;

Carrier* Carrier::getInstance()
{
	if (m_pInstance)
	{
		return m_pInstance;
	}

	BillingType type = Carrier::getBillingType();
	if (BillingTypeThreeNet == type)
	{
		type = (BillingType)Carrier::getCarrierType();
	}

	switch (type)
	{
	case BillingTypeChinaMobile:
	case BillingTypeChinaMobileCh:
		m_pInstance = new CarrierChinaMobile();
		break;
	case BillingTypeChinaTelcom:
	case BillingTypeChinaTelcomCh:
		m_pInstance = new CarrierChinaTelcom();
		break;
	case BillingTypeChinaUnicom:
	case BillingTypeChinaUnicomCh:
		m_pInstance = new CarrierChinaUnicom();
		break;
	//case BillingTypeXiaoMi:
	//	m_pInstance = new CarrierXiaoMi();
	//	break;
	case BillingTypeYijie:
		m_pInstance = new CarrierYijie();
		break;
	case BillingTypeSimulate:
		m_pInstance = new CarrierSimulation();
		break;
	default:
		m_pInstance = new CarrierChinaMobile();
		break;
	}

	return m_pInstance;
}


CarrierType Carrier::getCarrierType()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台  
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/Carrier", "getCarrier", "()I");

	if (!isHave) {
		DEBUG_LOG("org/cocos2dx/cpp/Carrier.getCarrier() not exists");
		return CarrierTypeChinaMobile;
	}
	else{
		//调用此函数  
		jint _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("org/cocos2dx/cpp/Carrier.getCarrier() : result %d", _int);
		return (CarrierType)_int;
	}
#else
	return CarrierTypeChinaMobile;
#endif

}

BillingType Carrier::getBillingType()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台  
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/Billing", "getBillingType", "()I");

	if (!isHave) {
		DEBUG_LOG("org/cocos2dx/cpp/Carrier.getBillingType() not exists");
		return BillingTypeChinaMobile;
	}
	else{
		//调用此函数  
		jint _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("org/cocos2dx/cpp/Carrier.getBillingType() : result %d", _int);
		return (BillingType)_int;
	}
#else
	return BillingTypeThreeNet;
#endif

}

void Carrier::getGoods(const TGoodsData* pGoodsData)
{
	if (pGoodsData->itemMap)
	{
		for (auto item : *pGoodsData->itemMap)
		{
			if (!strcmp(item.first.c_str(), GAMEDATA::MONEY)
				|| !strcmp(item.first.c_str(), GAMEDATA::SHIELD)
				|| !strcmp(item.first.c_str(), GAMEDATA::KILLER))
			{
				GameData::getInstance()->incValue(item.first.c_str(), item.second);
			}
			else if (!strcmp(item.first.c_str(), GAMEDATA::LIFE))
			{
				if (GameScene::isSceneRunning())
				{
					GameData::getInstance()->incValue(GAMEDATA::LEVEL_LIFE, item.second);
				}
				else
				{
					GameData::getInstance()->incValue(GAMEDATA::LIFE, item.second);
				}
			}
			else if (!strcmp(item.first.c_str(), GoodsType::PLANE_4TH))
			{
				PlayerEquip::getInstance()->addEquip(4, 4);
			}
			else if (!strcmp(item.first.c_str(), GoodsType::PLANE_5TH))
			{
				PlayerEquip::getInstance()->addEquip(5, 5);
			}
			else if (!strcmp(item.first.c_str(), GoodsType::PLANE_6TH))
			{
				PlayerEquip::getInstance()->addEquip(6, 6);
			}
			else
			{
				char buf[30 + 1] = { 0 };
				snprintf(buf, 30, "Unknown gift type: %s", item.first.c_str());
				DEBUG_LOG(buf);
				CCASSERT(false, buf);
			}
		}
	}

	if (!strcmp(GoodsType::VIP, pGoodsData->goods.c_str()))
	{
		GameData::getInstance()->addVipTime(CarrierConstant::VipDays);
	}

	if (!strcmp(GoodsType::NEW_GIFT, pGoodsData->goods.c_str()))
	{
		GameData::getInstance()->setValue(GAMEDATA::NEW_GIFT, 1);
		GameData::getInstance()->setValue(GAMEDATA::NEW_GIFT_RECOMMEND, 1);
	}
	else if (!strcmp(GoodsType::MID_GIFT, pGoodsData->goods.c_str()))
	{
		GameData::getInstance()->setValue(GAMEDATA::MID_GIFT, 1);
		GameData::getInstance()->setValue(GAMEDATA::MID_GIFT_RECOMMEND, 1);
	}
	else if (!strcmp(GoodsType::BIG_GIFT, pGoodsData->goods.c_str()))
	{
		GameData::getInstance()->setValue(GAMEDATA::BIG_GIFT, 1);
		GameData::getInstance()->setValue(GAMEDATA::BIG_GIFT_RECOMMEND, 1);
	}
	else if (!strcmp(GoodsType::HUG_GIFT, pGoodsData->goods.c_str()))
	{
		GameData::getInstance()->setValue(GAMEDATA::HUG_GIFT, 1);
		GameData::getInstance()->setValue(GAMEDATA::HUG_GIFT_RECOMMEND, 1);
	}
	else if (!strcmp(GoodsType::LIFE_1, pGoodsData->goods.c_str())
		|| !strcmp(GoodsType::LIFE_PKG, pGoodsData->goods.c_str()))
	{
		GameData::getInstance()->setValue(GAMEDATA::REVIVE, 1);
	}
}

bool Carrier::isDisplayNewGift()
{
	return true;
}

const char* Carrier::getHugGiftUi()
{
	return "GiftHugPkg.csb";
}

const char* Carrier::getBigGiftUi()
{
	return "GiftBigPkg.csb";
}

const char* Carrier::getMidGiftUi()
{
	return "GiftMidPkg.csb";
}

const char* Carrier::getNewGiftUi()
{
	return "GiftNewPkg.csb";
}

const char* Carrier::getLifePkgUi()
{
	return "GiftLifePkg.csb";
}

const char* Carrier::getPlane5PkgUi()
{
	return "GiftAircraftPkg5.csb";
}

const char* Carrier::getPlane6PkgUi()
{
	return "GiftAircraftPkg6.csb";
}

const char* Carrier::getDiamondGiftUi()
{
	return "GiftDiamond.csb";
}

const char* Carrier::getKillerGiftUi()
{
	return "GiftKiller.csb";
}

const char* Carrier::getShieldGiftUi()
{
	return "GiftShield.csb";
}

const char* Carrier::getLifeGiftUi()
{
	return "GiftLife.csb";
}

const char* Carrier::getPlane4Ui()
{
	return "GiftAircraft4.csb";
}

const char* Carrier::getPlane5Ui()
{
	return "GiftAircraft5.csb";
}

const char* Carrier::getPlane6Ui()
{
	return "GiftAircraft6.csb";
}
//******************************************
//
//  中国移动游戏基地
//
//******************************************

//******************************************
//
//  中国电信
//
//******************************************

//******************************************
//
//  中国联通
//
//******************************************

//******************************************
//
//  小米
//
//******************************************

//******************************************
//
//  易接
//
//******************************************

//******************************************
//
//  模拟
//
//******************************************
