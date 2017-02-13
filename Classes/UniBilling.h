#pragma once

#include "cocos2d.h"
#include "BillingInterface.h"

USING_NS_CC;

////////////////////////////////////////////////////
//
//  ͳһ�Ʒ�
//
////////////////////////////////////////////////////
class UniBilling : public BillingInterface
{
public:
	//������Ϸʱ����ʹ��������������ж������Ƿ񿪹�
	virtual bool isMusicEnabled() override;

	//��ʾ������Ϸ
	virtual void moreGame() override;

	//֧���ӿ� billingIndex:�Ʒѵ���
	virtual void pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc) override;

	//�˳��ӿ�
	virtual bool exit() override;

public:
	static std::map<std::string, std::string> m_payIndexMap;
};