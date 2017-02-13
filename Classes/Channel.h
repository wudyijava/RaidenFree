#pragma once

#include "cocos2d.h"

USING_NS_CC;

namespace CHANNEL
{
	const char* const DEFAULT = "default";
	const char* const HUAWEI  = "huawei";
	const char* const BAIDU = "baidu";
	const char* const TENCENT = "tencent";
	const char* const YIJIE = "yijie";
	const char* const QIHU = "qihu";
}

class Channel
{
public:
	static std::string getMainChannel();
	static std::string getSubChannel();

	static bool start();
	static bool pause();
	static bool exit();

	static std::string getUserId();
};