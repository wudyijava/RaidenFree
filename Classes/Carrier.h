#pragma once

#include "cocos2d.h"

USING_NS_CC;

typedef enum
{
	CarrierTypeUnknown     = -1, //δ֪
	CarrierTypeChinaMobile = 10, //�ƶ�����
	CarrierTypeChinaTelcom = 20, //����
	CarrierTypeChinaUnicom = 30, //��ͨ
	//CarrierTypeXiaoMi      = 40, //С��
	CarrierTypeYijie       = 50, //�׽�
	CarrierTypeSimulation  = 99  //ģ��
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

//����
class Carrier
{
public:
	static Carrier* getInstance();

	static CarrierType getCarrierType();
	static BillingType getBillingType();

public:
	virtual void getGoods(const TGoodsData* pGoodsData);
	virtual bool isDisplayNewGift();      //�Ƿ���ʾ�������

	virtual const char* getHugGiftUi();   //�������������
	virtual const char* getBigGiftUi();   //�����������
	virtual const char* getMidGiftUi();   //��ֵ�������
	virtual const char* getNewGiftUi();   //�����������
	virtual const char* getLifePkgUi();   //�����������
	virtual const char* getPlane5PkgUi(); //ս�����������
	virtual const char* getPlane6PkgUi(); //ս�����������

	virtual const char* getDiamondGiftUi();//����ʯ
	virtual const char* getKillerGiftUi(); //�����ɱ
	virtual const char* getShieldGiftUi(); //���򻤶�
	virtual const char* getLifeGiftUi();   //��������

	virtual const char* getPlane4Ui();     //����ս��4
	virtual const char* getPlane5Ui();     //����ս��5
	virtual const char* getPlane6Ui();     //����ս��6

private:
	static Carrier* m_pInstance;
};

//�й��ƶ���Ϸ����
class CarrierChinaMobile :public Carrier
{
public:
};

//�й�����
class CarrierChinaTelcom :public Carrier
{
public:
};

//�й���ͨ
class CarrierChinaUnicom :public Carrier
{
public:
};

//С��
//class CarrierXiaoMi :public Carrier
//{
//public:
//};

//�׽�
class CarrierYijie :public Carrier
{
public:
};

//ģ��
class CarrierSimulation :public Carrier
{
public:
};