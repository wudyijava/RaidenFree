#include "Common/Utils.h"
#include "GlobalData.h"
#include "Analytics.h"
#include "ui/CocosGUI.h"
#include "Common/Macro.h"
#include "GameData.h"
#include <iostream> 


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#endif

#define LOG_TAG "thorqq"

namespace Utils
{
	const char* const LOG_FILE_NAME = "thorqq.log";

	bool initLog()
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#else
		FILE *pFile = fopen(LOG_FILE_NAME, "w");
		if (pFile)
		{
			fclose(pFile);
		}
#endif

		return true;
	}

	bool isWriteLog()
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//return GlobalData::getInstance()->isLog();
		return true;
#else
		return true;
#endif
	}

	const char* getFileName(const char* path)
	{
		int i;
		if (NULL == path)
		{
			return "null";
		}
		for (i = strlen(path) - 1; i >= 0; i--)
		{
			if (*(path + i) == '\\' || *(path + i) == '/')
			{
				return path + i + 1;
			}
		}
		return path;
	}

	void writeLog(const char* file, int line, const char *fmt, ...)
	{
		char buf[4096 + 1] = { 0 };
		va_list ap;// typedef char *  va_list;
		va_start(ap, fmt);//#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )
		vsnprintf(buf, 4096, fmt, ap);
		va_end(ap);   //#define va_end(ap)      ( ap = (va_list)0 )

		char szTimeString[128] = { 0 };
		struct timeval tv;
		struct tm *today;
		time_t tmp_time;
		int off;

		gettimeofday(&tv, NULL);

		tmp_time = tv.tv_sec;
		today = localtime(&tmp_time);
		off = strftime(szTimeString, 128, "%y-%m-%d %H:%M:%S", today);
		sprintf(szTimeString + off, ".%03ld", tv.tv_usec / 1000);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		static char szBuf[4096 + 1] = { 0 };
		snprintf(szBuf, 4096, "[%s,%s:%d]>> %s", szTimeString, getFileName(file), line, buf);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, szBuf, NULL);
#else
		cocos2d::log("[%s,%s:%d]>> %s", szTimeString, Utils::getFileName(file), line, buf);

		FILE *pFile = fopen(LOG_FILE_NAME, "a+");
		fprintf(pFile, "[%s,%s:%d]>> %s\n", szTimeString, Utils::getFileName(file), line, buf);
		fclose(pFile);
#endif
	}

	int str2int(const char* str)
	{
		return atoi(str);
	}

	int str2int(const std::string& str)
	{
		return atoi(str.c_str());

		//int number;
		//std::stringstream ss;

		//ss << str;  //可以是其他数据类型
		//ss >> number; //string -> int
		//return number;
	}

	float str2float(const char* str)
	{
		return atof(str);
	}

	float str2float(const std::string& str)
	{
		return atof(str.c_str());
	}

	std::string toString(int d, const char* const format)
	{
		char sz[31] = { 0 };
		snprintf(sz, 30, format, d);
		return std::string(sz);
	}

	std::string toString(float f, const char* const format)
	{
		char sz[31] = { 0 };
		snprintf(sz, 30, format, f);
		return std::string(sz);
	}

	float getRandom()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, RAND_MAX);
		return dis(gen) / (float)RAND_MAX;
	}

	static Texture2D* texture2d = NULL;
	Texture2D* captureScreen()
	{
		if (texture2d)
		{
			texture2d->release();
			texture2d = NULL;
		}

		Size visibleSize = Director::getInstance()->getVisibleSize();
		RenderTexture* pRenderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);

		//截图。  
		pRenderTexture->begin();
		Director::getInstance()->getRunningScene()->visit();
		pRenderTexture->end();

		texture2d = pRenderTexture->getSprite()->getTexture();
		texture2d->retain();

		return texture2d;
	}

	Texture2D* getLatestScreen()
	{
		return texture2d;
	}

	Action* delayToDo(float d, const std::function<void()>& func)
	{
		DelayTime* delay = DelayTime::create(d);
		FiniteTimeAction* endFunc = CallFunc::create(func);
		return Sequence::create(delay, endFunc, NULL);
	}

	Node* getChildByNameRe(const Node* parent, const std::string& name)
	{
		CCASSERT(name.length() != 0, "Invalid name");
		CCASSERT(parent != nullptr, "Invalid parent");

		if (!strcmp(parent->getName().c_str(), name.c_str()))
		{
			return const_cast<Node*>(parent);
		}

		Node* pNode = parent->getChildByName(name);
		if (pNode)
		{
			return pNode;
		}
		else if (parent->getChildrenCount() > 0)
		{
			for (const auto& child : parent->getChildren())
			{
				pNode = Utils::getChildByNameRe(child, name);
				if (pNode)
				{
					return pNode;
				}
			}
		}

		return nullptr;
	}

	int splite(const std::string& str, const std::string& pattern, std::vector<std::string>& result)
	{
		int size = str.size();
		for (int i = 0; i < size;)
		{
			int pos = str.find(pattern, i);
			if (pos > 0 && pos < size)
			{
				std::string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size();
			}
			else
			{
				std::string s = str.substr(i);
				result.push_back(s);
				break;
			}
		}

		return result.size();
	}

	std::string getLevelStr(int level)
	{
		std::string levelStr;
		for (int levelI = 0; levelI < level; levelI++)
		{
			levelStr.append("\t");
		}
		return levelStr;
	}

	std::string formatJson(std::string jsonStr)
	{
		int level = 0;
		std::string jsonForMatStr;
		for (unsigned i = 0; i < jsonStr.length(); i++)
		{
			char c = jsonStr.at(i);
			if (level > 0 && !'\n' == jsonForMatStr.at(jsonForMatStr.length() - 1))
			{
				jsonForMatStr.append(getLevelStr(level));
			}

			switch (c)
			{
			case '{':
			case '[':
				jsonForMatStr += c;
				jsonForMatStr.append("\n");
				level++;
				break;
			case ',':
				jsonForMatStr += c;
				jsonForMatStr.append("\n");
				break;
			case '}':
			case ']':
				jsonForMatStr.append("\n");
				level--;
				jsonForMatStr.append(getLevelStr(level));
				jsonForMatStr += c;
				break;
			default:
				jsonForMatStr += c;
				break;
			}
		}

		return jsonForMatStr;

	}

	//格式化时间
	void timeToString(int time, int& day, int& hour, int& minute, int& second)
	{
		const int DAY = 24 * 60 * 60;
		const int HOUR = 60 * 60;
		const int MINUTE = 60;

		day = time / DAY;
		time = (day == 0 ? 0 : time % (DAY * day));

		hour = time / HOUR;
		time = (hour == 0 ? 0 : time % (HOUR * hour));

		minute = time / MINUTE;
		time = (minute == 0 ? 0 : time % (MINUTE * minute));

		second = time;
	}

	//震动
	void vibrate(int milliseconds)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo minfo;//定义Jni函数信息结构体  
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
		bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/Device", "vibrate", "(I)V");

		if (isHave) 
		{
			//调用此函数  
			minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID, milliseconds);
		}
#endif	
	}

	void addButtonTouchEffect(ui::Widget* pWidget,
		ButtonTouchEffect effect,
		const std::function<void(Ref*, ui::Widget::TouchEventType)>& callback)
	{
		if (!pWidget)
		{
			DEBUG_LOG("addButtonTouchEffect : pWidget is nullptr");
			CCASSERT(pWidget, "addButtonTouchEffect : pWidget is nullptr");

			return;
		}

		pWidget->addTouchEventListener([effect, callback](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Node* pNode = (Node*)pSender;

			if (ButtonTouchEffect::SINK == effect)
			{
				if (ui::Widget::TouchEventType::BEGAN == type)
				{
					pNode->setPosition(pNode->getPosition() + Vec2(5, -5));
				}
				else if (ui::Widget::TouchEventType::ENDED == type || ui::Widget::TouchEventType::CANCELED == type)
				{
					pNode->setPosition(pNode->getPosition() + Vec2(-5, 5));
				}
			}
			else if (ButtonTouchEffect::DARK == effect)
			{
				if (ui::Widget::TouchEventType::BEGAN == type)
				{
					pNode->setColor(Color3B(0xbb, 0xbb, 0xbb));
				}
				else if (ui::Widget::TouchEventType::ENDED == type || ui::Widget::TouchEventType::CANCELED == type)
				{
					pNode->setColor(Color3B(0xff, 0xff, 0xff));
				}
			}

			if (ui::Widget::TouchEventType::ENDED == type)
			{
				callback(pSender, type);
			}
		});

	}

	void addButtonTouchEffect(ui::CheckBox* pCheckBox,
		const std::function<void(Ref*, ui::CheckBox::EventType)>& callback)
	{
		CCASSERT(pCheckBox, "addButtonTouchEffect : pCheckBox is nullptr");

		pCheckBox->addEventListener([callback](Ref* pSender, ui::CheckBox::EventType type)
		{
			//if (ui::CheckBox::EventType::SELECTED == type)
			{
				callback(pSender, type);
			}
		});

	}

	void addMoveLight(Node* parent, Node* stencil)
	{
		//get pSprite size
		Size clipSize = stencil->getContentSize();
		stencil->getParent()->getChildren().eraseObject(stencil);

		//spark
		Sprite* pLight = Sprite::createWithSpriteFrameName("light.png");
		pLight->setPosition(-clipSize.width, 0);

		//clippingNode
		ClippingNode* clippingNode = ClippingNode::create();
		clippingNode->setPosition(stencil->getPosition());
		parent->addChild(clippingNode);

		clippingNode->setAlphaThreshold(0.05f); //设置alpha闸值
		clippingNode->setContentSize(clipSize); //设置尺寸大小

		clippingNode->setStencil(stencil);   //设置模板stencil
		clippingNode->addChild(stencil, 1);  //先添加标题,会完全显示出来,因为跟模板一样大小
		clippingNode->addChild(pLight, 2);       //会被裁减

		//Action
		MoveTo* moveAction = MoveTo::create(0.8f, Vec2(clipSize.width / 2 - 20, 0));
		MoveTo* moveBackAction = MoveTo::create(2.0f, Vec2(-clipSize.width / 2 + 20, 0));
		pLight->runAction(RepeatForever::create(Sequence::create(Show::create(), moveAction, Hide::create(), moveBackAction, nullptr)));

	}
}//namespace Utils