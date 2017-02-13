#pragma once

#include "cocos2d.h"

enum EPayChannel
{
	UnknownPayChannel = -1,

	AppStore = 1,
	Alipay = 2,
	NetBank = 3,
	Tenpay = 4,
	Cmcc = 5,
	Unicom = 6,
	Telcom = 7,
	Paypal = 8,

	Xiaomi = 11,
	Yijie = 12,

	Simulation = 99
};

namespace AnalyticsEventKey
{
	const char* const Item = "Item";
	const char* const Diamond = "Diamond";
	const char* const Cash = "Cash";
};

namespace AnalyticsEvent
{
	//����ģʽ ���о���
	const char* const RapidDistnace = "RapidDistnace";
	//ȫ��ǿ��	��Ŀ	��ʯ��
	const char* const StrengthenGlobal = "StrengthenGlobal";
	//�ֿ�ǿ��	��Ŀ	��ʯ��
	const char* const StrengthenEquip = "StrengthenEquip";

	//�ֿ⹺��	��Ŀ	��ʯ��
	const char* const BuyEquip = "BuyEquip";

	//�����ƷѴ�	����
	const char* const OpenBillingDlg = "OpenBillingDlg";
	//�������	����
	const char* const ClickBuy = "ClickBuy";
	//����ɹ�	����	���
	const char* const BuySuccess = "BuySuccess";
	//����ʧ��	����	���
	const char* const BuyFailed = "BuyFailed";

	//ǩ��
	const char* const FingerPrint = "FingerPrint";
}

class Analytics
{
public:

	static Analytics* getInstance();

	// ���֧�����Ҷһ������,����ͳ����Ϸ���������
	// ��ֵ.
	virtual void pay(double cash, int source, double coin);
	// ��ֵ���������.
	virtual void pay(double cash, int source, const char * item, int amount, double price);

	// �������.���������Ҷһ�һ����������ֵ�ĵ���
	virtual void buy(const char *item, int amount, double price);

	// ���ĵ���.���ʹ�õ��ߵ����
	virtual void use(const char *item, int amount, double price);

	//�����Ϸ�ж����õ�����ҽ���ͳ�ƣ�����ϵͳ���ͣ����ս�������ֵ��䡣
	// ���ͽ��.
	virtual void bonus(double coin, int source);
	// ���͵���.
	virtual void bonus(const char *item, int amount, double price, int source);

	// ����ؿ�.
	virtual void startLevel(const char *level);
	// ͨ���ؿ�.
	virtual void finishLevel(const char *level);
	// δͨ���ؿ�.
	virtual void failLevel(const char *level);

	//�¼�����ͳ��
	virtual void event(const char * eventId, const char * label = NULL);

protected:
	Analytics();

	virtual ~Analytics();

private:

	static Analytics* m_AnalyticsImpl;
};

class LocalAnalytics : public Analytics
{
	// ���֧�����Ҷһ������,����ͳ����Ϸ���������
	// ��ֵ.
	virtual void pay(double cash, int source, double coin) override;
	// ��ֵ���������.
	virtual void pay(double cash, int source, const char * item, int amount, double price) override;

	// �������.���������Ҷһ�һ����������ֵ�ĵ���
	virtual void buy(const char *item, int amount, double price) override;

	// ���ĵ���.���ʹ�õ��ߵ����
	virtual void use(const char *item, int amount, double price) override;

	//�����Ϸ�ж����õ�����ҽ���ͳ�ƣ�����ϵͳ���ͣ����ս�������ֵ��䡣
	// ���ͽ��.
	virtual void bonus(double coin, int source) override;
	// ���͵���.
	virtual void bonus(const char *item, int amount, double price, int source) override;

	// ����ؿ�.
	virtual void startLevel(const char *level) override;
	// ͨ���ؿ�.
	virtual void finishLevel(const char *level) override;
	// δͨ���ؿ�.
	virtual void failLevel(const char *level) override;

	//�¼�����ͳ��
	virtual void event(const char * eventId, const char * label = NULL) override;
};

