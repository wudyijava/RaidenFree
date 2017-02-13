#pragma once

#include "cocos2d.h"
#include "Bmob/BmobSdk.h"

USING_NS_CC;


///////////////////////////////////////////////
//
// ��¼
//
////////////////////////////////////////////////
class RemoteLogin : public BmobCloudDelegate, public Ref
{
public:
	RemoteLogin();

	void run(const std::function<void(int)>& callback);

protected:

	virtual void onCloudSuccess(const void* data);
	virtual void onCloudFailure(const int code, const void* msg);

private:
	std::function<void(int)> m_callback;
};

///////////////////////////////////////////////
//
// �ϴ�����ģʽ���о���
//
////////////////////////////////////////////////
class RemoteRapidDistance : public BmobCloudDelegate, public Ref
{
public:
	RemoteRapidDistance(int distance);

	void run(const std::function<void(int, int, int)>& callback);
	void unRegCallback();

protected:

	virtual void onCloudSuccess(const void* data);
	virtual void onCloudFailure(const int code, const void* msg);

private:
	std::function<void(int, int, int)> m_callback;
	int m_iDistance;
};

///////////////////////////////////////////////
//
// ǩ���Ƿ�ʱ���ϴ��Զ��ر�ʱ��
//
////////////////////////////////////////////////
class RemoteShutdown : public BmobCloudDelegate, public Ref
{
public:
	RemoteShutdown();

	void run(const std::function<void(int)>& callback);

protected:

	virtual void onCloudSuccess(const void* data);
	virtual void onCloudFailure(const int code, const void* msg);

private:
	std::function<void(int)> m_callback;
};
