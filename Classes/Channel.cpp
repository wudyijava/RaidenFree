#include "Channel.h"
#include "Common/Macro.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#endif

std::string Channel::getMainChannel()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台  
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/Channel", "getMainChannel", "()Ljava/lang/String;");

	if (!isHave) {
		DEBUG_LOG("org/cocos2dx/cpp/Channel.getMainChannel() is not exists");
		return CHANNEL::DEFAULT;
	}
	else{
		//调用此函数  
		jstring jstr = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		std::string ret = JniHelper::jstring2string(jstr);
		DEBUG_LOG("org/cocos2dx/cpp/Channel.getMainChannel(), result = %s", ret.c_str());
		return ret;
	}
#else
	return CHANNEL::TENCENT;
#endif
}

std::string Channel::getSubChannel()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台  
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/Channel", "getSubChannel", "()Ljava/lang/String;");

	if (!isHave) {
		DEBUG_LOG("org/cocos2dx/cpp/Channel.getSubChannel() is not exists");
		return CHANNEL::DEFAULT;
	}
	else{
		//调用此函数  
		jstring jstr = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		std::string ret = JniHelper::jstring2string(jstr);
		DEBUG_LOG("org/cocos2dx/cpp/Channel.getSubChannel(), result = %s", ret.c_str());
		return ret;
	}
#else
	return "";
#endif
}

bool Channel::start()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Channel", "start", "()Z");

	if (!isHave)
	{
		DEBUG_LOG("jni:Channel::start not exists");
		return false;
	}
	else
	{
		DEBUG_LOG("jni:Channel::start ok");
		//调用此函数  
		jboolean jbool = (jboolean)minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("jni-java: Channel::start finish : %s", jbool ? "true" : "false");
		return jbool;
	}
#else
	return false;
#endif
}

bool Channel::pause()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Channel", "pause", "()Z");

	if (!isHave)
	{
		DEBUG_LOG("jni:Channel::pause not exists");
		return false;
	}
	else
	{
		DEBUG_LOG("jni:Channel::pause ok");
		//调用此函数  
		jboolean jbool = (jboolean)minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("jni-java: Channel::pause finish : %s", jbool ? "true" : "false");
		return jbool;
	}
#else
	return false;
#endif
}

bool Channel::exit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Channel", "exit", "()Z");

	if (!isHave)
	{
		DEBUG_LOG("jni:Channel::exit not exists");
		return false;
	}
	else
	{
		DEBUG_LOG("jni:Channel::exit ok");
		//调用此函数  
		jboolean jbool = (jboolean)minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("jni-java: Channel::exit finish : %s", jbool ? "true" : "false");
		return jbool;
	}
#else
	return false;
#endif
}

std::string Channel::getUserId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/BillingYijie", "getUserId", "()Ljava/lang/String;");

	if (!isHave)
	{
		DEBUG_LOG("jni:BillingYijie::getUserId not exists");
		return "-1";
	}
	else
	{
		DEBUG_LOG("jni:BillingYijie::getUserId ok");
		//调用此函数  
		jstring jstr = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		std::string ret = JniHelper::jstring2string(jstr);

		DEBUG_LOG("jni-java: BillingYijie::getUserId finish : %s", ret.c_str());
		return ret;
	}
#else
	return "-2";
#endif
}