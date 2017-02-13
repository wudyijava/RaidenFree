#pragma once

#include "cocos2d.h"

USING_NS_CC;

typedef enum
{
	CarrierTypeUnknown     = -1, //未知
	CarrierTypeChinaMobile = 10, //移动基地
	CarrierTypeChinaTelcom = 20, //电信
	CarrierTypeChinaUnicom = 30, //联通
	//CarrierTypeXiaoMi      = 40, //小米
	CarrierTypeYijie       = 50, //易接
	CarrierTypeSimulation  = 99  //模拟
}CarrierType;

typedef enum
{
	BillingTypeChinaMobile   = 10,
	BillingTypeChinaMobileCh = 11,
	BillingTypeChinaTelcom   = 20,
	BillingTypeChinaTelcomCh = 21,
	BillingTypeChinaUnicom   = 30,
	BillingTypeChinaUnicomCh = 31,
	//BillingTypeXiaoMi        = 40,
	BillingTypeYijie         = 50,
	BillingTypeThreeNet      = 90,
	BillingTypeSimulate      = 99
}BillingType;

struct TGoodsData;

//基类
class Carrier
{
public:
	static Carrier* getInstance();

	static CarrierType getCarrierType();
	static BillingType getBillingType();

public:
	virtual void getGoods(const TGoodsData* pGoodsData);
	virtual bool isDisplayNewGift();      //是否显示新手礼包

	virtual const char* getHugGiftUi();   //超土豪礼包界面
	virtual const char* getBigGiftUi();   //豪华礼包界面
	virtual const char* getMidGiftUi();   //超值礼包界面
	virtual const char* getNewGiftUi();   //新手礼包界面
	virtual const char* getLifePkgUi();   //生命礼包界面
	virtual const char* getPlane5PkgUi(); //战机五礼包界面
	virtual const char* getPlane6PkgUi(); //战机六礼包界面

	virtual const char* getDiamondGiftUi();//购买宝石
	virtual const char* getKillerGiftUi(); //购买必杀
	virtual const char* getShieldGiftUi(); //购买护盾
	virtual const char* getLifeGiftUi();   //购买生命

	virtual const char* getPlane4Ui();     //购买战机4
	virtual const char* getPlane5Ui();     //购买战机5
	virtual const char* getPlane6Ui();     //购买战机6

private:
	static Carrier* m_pInstance;
};

//中国移动游戏基地
class CarrierChinaMobile :public Carrier
{
public:
};

//中国电信
class CarrierChinaTelcom :public Carrier
{
public:
};

//中国联通
class CarrierChinaUnicom :public Carrier
{
public:
};

//小米
//class CarrierXiaoMi :public Carrier
//{
//public:
//};

//易接
class CarrierYijie :public Carrier
{
public:
};

//模拟
class CarrierSimulation :public Carrier
{
public:
};