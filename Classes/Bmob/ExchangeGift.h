#pragma once

#include "Bmob/Gift.h"
#include "Bmob/ExchangeCode.h"
#include "Bmob/BmobSdk.h"
#include "cocos2d.h"

USING_NS_CC;

namespace ExchangeGiftConstant
{
	const int ErrCodeSuccess = 0;              //�ɹ���ȡ

	const int ErrCodeInvalidExchangeCode = -1; //��Ч�Ķһ���
	const int ErrCodeGiftReceived = -2;        //�һ����ѱ�ʹ�ù�
	const int ErrCodeQueryExchangeCode = -3;   //�����һ�������
	const int ErrCodeQueryGiftId = -4;         //������Ʒ�����
	const int ErrCodeNoGift = -5;              //��Ʒ��鲻����¼
	const int ErrCodeRemarkReceivedDate = -10; //�����ȡʱ��ʧ��

	const char* const StrQueryExchangeCode = "querying_exchange_code";//����������Ʒ���ģ����Ժ�...
	const char* const StrExchangeCodeUsed = "exchange_code_used";//�ܱ�Ǹ���öһ����ѱ�ʹ�ù�
	const char* const StrInvalidExchangeCode = "invalid_exchange_code";//�ܱ�Ǹ��������Ķһ�����Ч
	const char* const StrErroeQueryExchangeCode = "error_query_exchange_code";//�ܱ�Ǹ���޷���ѯ������Ķһ��룬���������Ƿ�����
	const char* const StrUnknowError = "unknown_error";//�ܱ�Ǹ������δ֪������������Ϸ�����³���

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