#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

namespace Utils
{
	bool initLog();
	//是否打印日志
	bool isWriteLog();
	//从全路径中获取文件名
	const char* getFileName(const char* path);
	//打印日志
	void writeLog(const char* file, int line, const char *fmt, ...);

	//字符串转int
	int str2int(const char* str);
	int str2int(const std::string& str);
	//字符串转float
	float str2float(const char* str);
	float str2float(const std::string& str);

	std::string toString(int d, const char* const format = "%d");
	std::string toString(float f, const char* const format = "%.2f");

	//获取0-1之间的随机数
	float getRandom();


	//截屏
	Texture2D* captureScreen();
	//获取最近一次截屏图像
	Texture2D* getLatestScreen();


	//延迟函数
	Action* delayToDo(float d, const std::function<void()>& func);

	//递归查找子节点
	Node* getChildByNameRe(const Node* parent, const std::string& name);

	//根据分隔符pattern，将字符串str分割，分割后的结果放到result中
	int splite(const std::string& str, const std::string& pattern, std::vector<std::string>& result);

	//格式化json
	std::string formatJson(std::string jsonStr);
	std::string getLevelStr(int level);

	//格式化时间
	void timeToString(int time, int& day, int& hour, int& minute, int& second);

	//震动
	void vibrate(int milliseconds);

	typedef enum
	{
		NONE, //无
		SINK, //下沉
		DARK  //变暗
	}ButtonTouchEffect;

	void addButtonTouchEffect(ui::Widget* pWidget,
		ButtonTouchEffect effect,
		const std::function<void(Ref*, ui::Widget::TouchEventType)>& callback);

	void addButtonTouchEffect(ui::CheckBox* pCheckBox,
		const std::function<void(Ref*, ui::CheckBox::EventType)>& callback);

	void addMoveLight(Node* parent, Node* stencil);
}
