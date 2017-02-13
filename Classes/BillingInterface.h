#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct TGoodsData;

typedef enum
{
	BillingResult_SUCCESS   = 0,
	BillingResult_FAILED    = 1,
	BillingResult_CANCELLED = 2,
	BillingResult_TIMEOUT   = 3
}BillingResult;

////////////////////////////////////////////////////
//
//  ����ӿ�
//
////////////////////////////////////////////////////
class BillingInterface
{
public:
	//������Ϸʱ����ʹ��������������ж������Ƿ񿪹�
	virtual bool isMusicEnabled() = 0;

	//��ʾ������Ϸ
	virtual void moreGame() = 0;

	//֧���ӿ� billingIndex:�Ʒѵ���
	virtual void pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc) = 0;

	//�˳��ӿڡ�����true���ѵ����ⲿsdk�˳��ӿڣ�false��δ�����ⲿsdk�˳��ӿ�
	virtual bool exit() = 0;
};

////////////////////////////////////////////////////
//
//  ʵ����
//
////////////////////////////////////////////////////
class BillingImpl : public BillingInterface
{
public:
	static BillingInterface* getInstance();

	virtual bool isMusicEnabled() override;

	virtual void moreGame() override;

	virtual void pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc);

	virtual bool exit() override;

private:
	static BillingInterface* m_pInstance;
};
