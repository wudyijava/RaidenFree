#pragma once

#include "Bmob\BmobSdk.h"
#include "cocos2d.h"

USING_NS_CC;

//***************************************************************//
//
//  ͳһ�ص�
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
		//��ѯ���ݳɹ��ص���data�ǲ�ѯ���ص�����
		if (m_onSuccessFunc)
		{
			m_onSuccessFunc(data);
		}

		Ref::release();
	}

	virtual void onError(int code, const void* data)
	{
		//��ѯ����ʧ�ܻص���data��ʧ����Ϣ
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
//  ��ѯ�ص�
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
//  ���»ص�
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
//  ���»ص�
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
