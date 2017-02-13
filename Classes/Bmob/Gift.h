#pragma once

#include <iostream>
#include "cocos2d.h"
#include "BmobSdk.h"

USING_NS_CC;

class Gift : public BmobObject
{
public:
	Gift();
	virtual ~Gift();

	static void queryOneByGiftId(int giftId,
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc);

	static bool parseJosnToArray(const char* const jsonString, Vector<Gift*>& array);

	CC_SYNTHESIZE(int, m_iGiftId, GiftId);
	CC_SYNTHESIZE(std::string, m_giftName, GiftName);
	CC_SYNTHESIZE(int, m_iCount, Count);

};
