#include "Analytics.h"
#include "Common/Macro.h"

Analytics* Analytics::m_AnalyticsImpl = NULL;

Analytics* Analytics::getInstance()
{
	if (!m_AnalyticsImpl)
	{
		m_AnalyticsImpl = new LocalAnalytics;
	}

	return m_AnalyticsImpl;
}

Analytics::Analytics()
{

}

Analytics::~Analytics()
{

}

// ���֧�����Ҷһ������,����ͳ����Ϸ���������
// ��ֵ.
void Analytics::pay(double cash, int source, double coin)
{
	m_AnalyticsImpl->pay(cash, source, coin);
}

// ��ֵ���������.
void Analytics::pay(double cash, int source, const char * item, int amount, double price)
{
	m_AnalyticsImpl->pay(cash, source, item, amount, price);
}

// �������.���������Ҷһ�һ����������ֵ�ĵ���
void Analytics::buy(const char *item, int amount, double price)
{
	m_AnalyticsImpl->buy(item, amount, price);
}

// ���ĵ���.���ʹ�õ��ߵ����
void Analytics::use(const char *item, int amount, double price)
{
	m_AnalyticsImpl->use(item, amount, price);
}

//�����Ϸ�ж����õ�����ҽ���ͳ�ƣ�����ϵͳ���ͣ����ս�������ֵ��䡣
// ���ͽ��.
void Analytics::bonus(double coin, int source)
{
	m_AnalyticsImpl->bonus(coin, source);
}

// ���͵���.
void Analytics::bonus(const char *item, int amount, double price, int source)
{
	m_AnalyticsImpl->bonus(item, amount, price, source);
}

// ����ؿ�.
void Analytics::startLevel(const char *level)
{
	m_AnalyticsImpl->startLevel(level);
}

// ͨ���ؿ�.
void Analytics::finishLevel(const char *level)
{
	m_AnalyticsImpl->finishLevel(level);
}

// δͨ���ؿ�.
void Analytics::failLevel(const char *level)
{
	m_AnalyticsImpl->failLevel(level);
}

//�¼�����ͳ��
void Analytics::event(const char * eventId, const char * label)
{
	m_AnalyticsImpl->event(eventId, label);
}

// ���֧�����Ҷһ������,����ͳ����Ϸ���������
// ��ֵ.
void LocalAnalytics::pay(double cash, int source, double coin)
{
	DEBUG_LOG("LocalAnalytics::pay : cash = %.2f, source = %d, coin = %.2f", cash, source, coin);
}

// ��ֵ���������.
void LocalAnalytics::pay(double cash, int source, const char * item, int amount, double price)
{
	DEBUG_LOG("LocalAnalytics::pay : cash = %.2f, source = %d, item = %s, amount = %d, price = %.2f", 
		cash, source, item, amount, price);
}

// �������.���������Ҷһ�һ����������ֵ�ĵ���
void LocalAnalytics::buy(const char *item, int amount, double price)
{
	DEBUG_LOG("LocalAnalytics::buy : item = %s, amount = %d, price = %.2f", item, amount, price);
}

// ���ĵ���.���ʹ�õ��ߵ����
void LocalAnalytics::use(const char *item, int amount, double price)
{
	DEBUG_LOG("LocalAnalytics::use : item = %s, amount = %d, price = %.2f", item, amount, price);
}

//�����Ϸ�ж����õ�����ҽ���ͳ�ƣ�����ϵͳ���ͣ����ս�������ֵ��䡣
// ���ͽ��.
void LocalAnalytics::bonus(double coin, int source)
{
	DEBUG_LOG("LocalAnalytics::use : source = %d, coin = %.2f", source, coin);
}

// ���͵���.
void LocalAnalytics::bonus(const char *item, int amount, double price, int source)
{
	DEBUG_LOG("LocalAnalytics::bonus : item = %s, amount = %d, price = %.2f, source = %d", item, amount, price, source);
}

// ����ؿ�.
void LocalAnalytics::startLevel(const char *level)
{
	DEBUG_LOG("LocalAnalytics::startLevel : level = %s", level);
}

// ͨ���ؿ�.
void LocalAnalytics::finishLevel(const char *level)
{
	DEBUG_LOG("LocalAnalytics::finishLevel : level = %s", level);
}

// δͨ���ؿ�.
void LocalAnalytics::failLevel(const char *level)
{
	DEBUG_LOG("LocalAnalytics::failLevel : level = %s", level);
}

//�¼�����ͳ��
void LocalAnalytics::event(const char * eventId, const char * label)
{
	DEBUG_LOG("LocalAnalytics::event : eventId = %s, label = %s", eventId, label);
}
