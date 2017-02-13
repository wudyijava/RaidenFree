#include "Gift.h"
#include "DmlCallback.h"
#include "Common\Macro.h"

namespace GiftConstant
{
	const char* const TableName = "gift";

	const char* const ColNameObjectId = "objectId";
	const char* const ColNameGiftId = "gift_id";
	const char* const ColNameGiftName = "gift_name";
	const char* const ColNameCount = "count";

	const char* const JsonRoot = "results";
}

Gift::Gift() 
: BmobObject(GiftConstant::TableName)
{
}

Gift::~Gift()
{
	DEBUG_LOG("Gift::~Gift()");
}

void Gift::queryOneByGiftId(int giftId,
	const std::function<void(const void* data)>& onSuccessFunc,
	const std::function<void(int code, const void* data)>& onErrorFunc)
{
	BmobQuery* query = new BmobQuery(GiftConstant::TableName);
	query->autorelease();
	query->clear();

	//query->addQueryKeys("objectId,gift_id,gift_name,count"); //字段之间不能有空格
	query->addWhereEqualTo(GiftConstant::ColNameGiftId, Integer::create(giftId));

	QueryCallback* pBmobQueryCallback = new QueryCallback(onSuccessFunc, onErrorFunc);
	pBmobQueryCallback->autorelease();
	pBmobQueryCallback->retain();
	query->findObjects(pBmobQueryCallback);
}

bool Gift::parseJosnToArray(const char* const jsonString, Vector<Gift*>& array)
{
	Json::Reader reader;
	Json::Value root;

	if (reader.parse((const char*)jsonString, root))
	{
		if (!root[GiftConstant::JsonRoot].isNull())
		{
			int cnt = root[GiftConstant::JsonRoot].size();
			for (int i = 0; i < cnt; i++)
			{
				auto node = root[GiftConstant::JsonRoot][i];

				Gift* g = new Gift();
				g->autorelease();
				g->setGiftId(node[GiftConstant::ColNameGiftId].asInt());
				g->setGiftName(node[GiftConstant::ColNameGiftName].asString());
				g->setCount(node[GiftConstant::ColNameCount].asInt());

				array.pushBack(g);
			}
		}
	}

	if (array.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
