#include "ExchangeCode.h"
#include "Bmob\DmlCallback.h"
#include "Common\Macro.h"

namespace ExchangeCodeConstant
{
	const char* const TableName = "exchange_code";

	const char* const ColNameObjectId = "objectId";
	const char* const ColNameCode = "code";
	const char* const ColNameGiftId = "gift_id";
	const char* const ColNameReceiveDate = "receive_date";

	const char* const JsonRoot = "results";
}

ExchangeCode::ExchangeCode() 
: BmobObject(ExchangeCodeConstant::TableName)
, m_iGiftId(-1)
{

}

ExchangeCode::~ExchangeCode()
{
	DEBUG_LOG("ExchangeCode::~ExchangeCode()");
}

bool ExchangeCode::parse(const char* const jsonString)
{
	Json::Reader reader;
	Json::Value root;

	m_code = "";
	m_iGiftId = -1;
	m_receiveDate = "";

	if (reader.parse((const char*)jsonString, root))
	{
		if (!root[ExchangeCodeConstant::JsonRoot].isNull() && root[ExchangeCodeConstant::JsonRoot].size() > 0)
		{
			int idx = 0;
			auto node = root[ExchangeCodeConstant::JsonRoot][idx];

			setObjectId(node[ExchangeCodeConstant::ColNameObjectId].asString());
			setCode(node[ExchangeCodeConstant::ColNameCode].asString());
			setGiftId(node[ExchangeCodeConstant::ColNameGiftId].asInt());
			setReceiveDate(node[ExchangeCodeConstant::ColNameReceiveDate].asString());

			return true;
		}
	}

	return false;
}

void ExchangeCode::queryOneByCode(const std::string& code,
	const std::function<void(const void* data)>& onSuccessFunc,
	const std::function<void(int code, const void* data)>& onErrorFunc)
{
	BmobQuery* query = new BmobQuery(ExchangeCodeConstant::TableName);
	query->autorelease();
	query->clear();

	//query->addQueryKeys("code,gift_id,receive_date"); //字段之间不能有空格
	query->addWhereEqualTo(ExchangeCodeConstant::ColNameCode, __String::createWithFormat("%s", code.c_str()));

	QueryCallback* pBmobQueryCallback = new QueryCallback(onSuccessFunc, onErrorFunc);
	pBmobQueryCallback->autorelease();
	pBmobQueryCallback->retain();
	query->findObjects(pBmobQueryCallback);
}

void ExchangeCode::updateReceiveDate(const std::string& date,
	const std::function<void(const void* data)>& onSuccessFunc,
	const std::function<void(int code, const void* data)>& onErrorFunc)
{
	this->clear();

	this->setValue(ExchangeCodeConstant::ColNameReceiveDate, __String::createWithFormat("%s", date.c_str()));

	UpdateCallback* pUpdateCallback = new UpdateCallback(onSuccessFunc, onErrorFunc);
	pUpdateCallback->autorelease();
	pUpdateCallback->retain();
	this->update(getObjectId(), pUpdateCallback);
}
