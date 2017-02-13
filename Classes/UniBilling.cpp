#include "UniBilling.h"
#include "Goods.h"
#include "Common/Macro.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#endif

//可通过下面这个命令查看内部类型签名
//javap -s -p billing.class
//
//  public static void pay(java.lang.String, java.lang.String, int, java.lang.String);
//  Signature: (Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
	void Java_org_cocos2dx_cpp_NativeInterface_exitConfirm(JNIEnv *env)
	{
		Director::getInstance()->end();
	}

	void Java_org_cocos2dx_cpp_NativeInterface_exitCancel(JNIEnv *env)
	{

	}

	void Java_org_cocos2dx_cpp_NativeInterface_payResult(JNIEnv *env, jobject thiz, jstring billingIndex, jint resultCode)
	{
		std::string bi = JniHelper::jstring2string(billingIndex);

		DEBUG_LOG("billingIndex = %s.", bi.c_str());

		auto it = UniBilling::m_payIndexMap.find(bi);
		if (it != UniBilling::m_payIndexMap.end())
		{
			DEBUG_LOG("process pay result : %d(%s)", resultCode, it->second.c_str());

			Goods::payResult(it->second.c_str(), (BillingResult)resultCode);
		}
	}
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
jstring   charTojstring(JNIEnv* env, const char* str)
{
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(str));
	env->SetByteArrayRegion(bytes, 0, strlen(str), (const jbyte*)str);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

#endif

std::map<std::string, std::string> UniBilling::m_payIndexMap;

bool UniBilling::isMusicEnabled()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Billing", "isMusicEnabled", "()Z");

	if (!isHave)
	{
		DEBUG_LOG("jni: UniBilling::isMusicEnabled() not exists");
		return true;
	}
	else
	{
		DEBUG_LOG("jni:UniBilling::isMusicEnabled() ok");
		//调用此函数  
		jboolean jbool = (jboolean)minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("jni-java: UniBilling::isMusicEnabled finish : %s", jbool ? "true" : "false");
		return jbool;
	}
#else
	return true;
#endif
}

void UniBilling::moreGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Billing", "moreGame", "()V");

	if (!isHave)
	{
		DEBUG_LOG("jni: UniBilling::moreGame() not exists");
	}
	else
	{
		DEBUG_LOG("jni:UniBilling::moreGame() ok");
		//调用此函数  
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("jni-java: UniBilling::moreGame finish");
		return;
	}
#endif
}

void UniBilling::pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;

	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Billing", "pay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V");

	if (!isHave)
	{
		DEBUG_LOG("jni: pay() not exists");
	}
	else
	{
		DEBUG_LOG("jni:pay() ok");

		m_payIndexMap.insert(std::map<std::string, std::string>::value_type(billingIndex, code.c_str()));

		//调用此函数  
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, 
			charTojstring(minfo.env, billingIndex.c_str()),
			charTojstring(minfo.env, code.c_str()),
			charTojstring(minfo.env, name.c_str()),
			price,
			charTojstring(minfo.env, desc.c_str())
			);
		return;
	}
#else
	Goods::payResult(code.c_str(), BillingResult::BillingResult_SUCCESS);
#endif
}

bool UniBilling::exit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/Billing", "exit", "()Z");

	if (!isHave)
	{
		DEBUG_LOG("jni:UniBilling::exit not exists");
		return false;
	}
	else
	{
		DEBUG_LOG("jni:UniBilling::exit ok");
		//调用此函数  
		jboolean jbool = (jboolean)minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		DEBUG_LOG("jni-java: UniBilling::exit finish : %s", jbool ? "true" : "false");
		return jbool;
	}
#else
	return false;
#endif
}
