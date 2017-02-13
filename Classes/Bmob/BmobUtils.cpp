#include "BmobUtils.h"
#include "DmlCallback.h"
#include "GameData.h"
#include "Common/Macro.h"

namespace BmobUtilsConstant
{

};

///////////////////////////////////////////////
//
// 登录
//
////////////////////////////////////////////////
RemoteLogin::RemoteLogin()
: m_callback(nullptr)
{

}

void RemoteLogin::run(const std::function<void(int)>& callback)
{
	m_callback = callback;

	BmobCloud* bcloud = new BmobCloud();
	bcloud->autorelease();

	std::map<string, Ref*> param;
	const std::string& objectid = GameData::getInstance()->getValue(GAMEDATA::USER_ID);
	const std::string& signature = GameData::getInstance()->getValue(GAMEDATA::FINGER_PRINT);

	param.insert(std::map<string, Ref*>::value_type("objectid", __String::createWithFormat("%s", objectid.c_str())));
	param.insert(std::map<string, Ref*>::value_type("signature", __String::createWithFormat("%s", signature.c_str())));

	bcloud->execCloudCode("login", param, this);
}

void RemoteLogin::onCloudSuccess(const void* data)
{
	DEBUG_LOG("RemoteLogin::onCloudSuccess : %s", (const char*)data);

	Json::Reader reader;
	Json::Value root;

	if (reader.parse((const char*)data, root))
	{
		if (!root["result"].isNull())
		{
			std::string result = root["result"].asString();
			if (reader.parse(result.c_str(), root))
			{
				if (!root["result"].isNull())
				{
					int result2 = root["result"].asInt();
				}
				if (!root["objectid"].isNull())
				{
					const std::string& userId = root["objectid"].asString();
					GameData::getInstance()->setValue(GAMEDATA::USER_ID, userId);
				}
				if (!root["time"].isNull())
				{
					const std::string& time = root["time"].asString();
					GameData::getInstance()->setValue(GAMEDATA::TIME, Utils::str2int(time));
				}
				if (!root["valid"].isNull())
				{
					int valid = root["valid"].asInt();
					GameData::getInstance()->setValue(GAMEDATA::FINGER_PRINT_FLAG, valid);
				}

			}
		}
	}

	if (m_callback)
	{
		m_callback(0);
	}
}

void RemoteLogin::onCloudFailure(const int code, const void* msg)
{
	DEBUG_LOG("RemoteLogin::onCloudFailure : %d, %s", code, (const char*)msg);

	if (m_callback)
	{
		m_callback(-1);
	}

}

///////////////////////////////////////////////
//
// 上传急速模式飞行距离
//
////////////////////////////////////////////////

RemoteRapidDistance::RemoteRapidDistance(int distance)
: m_callback(nullptr)
, m_iDistance(distance)
{

}

void RemoteRapidDistance::unRegCallback()
{
	m_callback = nullptr;
}

void RemoteRapidDistance::run(const std::function<void(int, int, int)>& callback)
{
	m_callback = callback;

	BmobCloud* bcloud = new BmobCloud();
	bcloud->autorelease();

	std::map<string, Ref*> param;
	const std::string& objectid = GameData::getInstance()->getValue(GAMEDATA::USER_ID);
	const std::string& signature = GameData::getInstance()->getValue(GAMEDATA::FINGER_PRINT);

	param.insert(std::map<string, Ref*>::value_type("objectid", __String::createWithFormat("%s", objectid.c_str())));
	param.insert(std::map<string, Ref*>::value_type("signature", __String::createWithFormat("%s", signature.c_str())));
	param.insert(std::map<string, Ref*>::value_type("distance", __Integer::create(m_iDistance)));

	bcloud->execCloudCode("rapid", param, this);
}

void RemoteRapidDistance::onCloudSuccess(const void* data)
{
	DEBUG_LOG("RemoteRapidDistance::onCloudSuccess : %s", (const char*)data);

	Json::Reader reader;
	Json::Value root;

	int high = -1;
	int total = -1;

	if (reader.parse((const char*)data, root))
	{
		if (!root["result"].isNull())
		{
			std::string result = root["result"].asString();
			if (reader.parse(result.c_str(), root))
			{
				if (!root["result"].isNull())
				{
					int result2 = root["result"].asInt();
				}
				if (!root["objectid"].isNull())
				{
					const std::string& userId = root["objectid"].asString();
					GameData::getInstance()->setValue(GAMEDATA::USER_ID, userId);
				}
				if (!root["time"].isNull())
				{
					const std::string& time = root["time"].asString();
					GameData::getInstance()->setValue(GAMEDATA::TIME, Utils::str2int(time));
				}
				if (!root["valid"].isNull())
				{
					int valid = root["valid"].asInt();
					GameData::getInstance()->setValue(GAMEDATA::FINGER_PRINT_FLAG, valid);
				}
				if (!root["high"].isNull())
				{
					high = root["high"].asInt();
				}
				if (!root["total"].isNull())
				{
					total = root["total"].asInt();
				}
			}
		}
	}

	if (m_callback)
	{
		if (high >= 0 && total > 0)
		{
			m_callback(0, high, total);
		}
		else
		{
			m_callback(-1, high, total);
		}
	}
}

void RemoteRapidDistance::onCloudFailure(const int code, const void* msg)
{
	DEBUG_LOG("RemoteRapidDistance::onCloudFailure : %d, %s", code, (const char*)msg);

	if (m_callback)
	{
		m_callback(-1, -1, -1);
	}

}

///////////////////////////////////////////////
//
// 签名非法时，上传自动关闭时间
//
////////////////////////////////////////////////
RemoteShutdown::RemoteShutdown()
: m_callback(nullptr)
{

}

void RemoteShutdown::run(const std::function<void(int)>& callback)
{
	m_callback = callback;

	BmobCloud* bcloud = new BmobCloud();
	bcloud->autorelease();

	std::map<string, Ref*> param;
	const std::string& objectid = GameData::getInstance()->getValue(GAMEDATA::USER_ID);

	param.insert(std::map<string, Ref*>::value_type("objectid", __String::createWithFormat("%s", objectid.c_str())));

	bcloud->execCloudCode("shutdown", param, this);
}

void RemoteShutdown::onCloudSuccess(const void* data)
{
	DEBUG_LOG("RemoteShutdown::onCloudSuccess : %s", (const char*)data);

	if(m_callback)
	{
		m_callback(1);
	}

}

void RemoteShutdown::onCloudFailure(const int code, const void* msg)
{
	DEBUG_LOG("RemoteShutdown::onCloudFailure : %d, %s", code, (const char*)msg);

	if (m_callback)
	{
		m_callback(-1);
	}

}