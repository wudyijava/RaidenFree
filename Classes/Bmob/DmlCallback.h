#pragma once

#include "Bmob\BmobSdk.h"
#include "cocos2d.h"

USING_NS_CC;

//***************************************************************//
//
//  统一回调
//
//***************************************************************//
class BmobCallback : public Ref
{
public:
	BmobCallback() : m_onSuccessFunc(nullptr), m_onErrorFunc(nullptr)
	{

	}

	BmobCallback(
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc)
	{
		m_onSuccessFunc = onSuccessFunc;
		m_onErrorFunc = onErrorFunc;
	}

	virtual ~BmobCallback()
	{

	}

	virtual void onSucess(const void* data)
	{
		//查询数据成功回调，data是查询返回的数据
		if (m_onSuccessFunc)
		{
			m_onSuccessFunc(data);
		}

		Ref::release();
	}

	virtual void onError(int code, const void* data)
	{
		//查询数据失败回调，data是失败信息
		if (m_onErrorFunc)
		{
			m_onErrorFunc(code, data);
		}

		Ref::release();
	}

protected:
	std::function<void(const void* data)> m_onSuccessFunc;
	std::function<void(int code, const void* data)> m_onErrorFunc;
};

//***************************************************************//
//
//  查询回调
//
//***************************************************************//
class QueryCallback :public BmobFindDelegate, public BmobCallback
{
public:

	QueryCallback(
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc)
		: BmobCallback(onSuccessFunc, onErrorFunc)
	{
	}

	virtual void onFindSucess(const void* data)
	{
		BmobCallback::onSucess(data);
	}

	virtual void onFindError(int code, const void* data)
	{
		BmobCallback::onError(code, data);
	}
};

//***************************************************************//
//
//  更新回调
//
//***************************************************************//
class UpdateCallback :public BmobUpdateDelegate, public BmobCallback
{
public:
	UpdateCallback(
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc)
		: BmobCallback(onSuccessFunc, onErrorFunc)
	{
	}


	virtual void onUpdateSucess(const void* data)
	{
		BmobCallback::onSucess(data);
	}

	virtual void onUpdateError(int code, const void* data)
	{
		BmobCallback::onError(code, data);
	}
};

//***************************************************************//
//
//  更新回调
//
//***************************************************************//
class CloudCallback :public BmobCloudDelegate, public BmobCallback
{
public:
	CloudCallback(
		const std::function<void(const void* data)>& onSuccessFunc,
		const std::function<void(int code, const void* data)>& onErrorFunc)
		: BmobCallback(onSuccessFunc, onErrorFunc)
	{
	}

	virtual void onCloudSuccess(const void* data)
	{
		BmobCallback::onSucess(data);
	}

	virtual void onCloudFailure(const int code, const void* msg)
	{
		BmobCallback::onError(code, msg);
	}
};
