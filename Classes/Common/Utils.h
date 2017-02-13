#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

namespace Utils
{
	bool initLog();
	//�Ƿ��ӡ��־
	bool isWriteLog();
	//��ȫ·���л�ȡ�ļ���
	const char* getFileName(const char* path);
	//��ӡ��־
	void writeLog(const char* file, int line, const char *fmt, ...);

	//�ַ���תint
	int str2int(const char* str);
	int str2int(const std::string& str);
	//�ַ���תfloat
	float str2float(const char* str);
	float str2float(const std::string& str);

	std::string toString(int d, const char* const format = "%d");
	std::string toString(float f, const char* const format = "%.2f");

	//��ȡ0-1֮��������
	float getRandom();


	//����
	Texture2D* captureScreen();
	//��ȡ���һ�ν���ͼ��
	Texture2D* getLatestScreen();


	//�ӳٺ���
	Action* delayToDo(float d, const std::function<void()>& func);

	//�ݹ�����ӽڵ�
	Node* getChildByNameRe(const Node* parent, const std::string& name);

	//���ݷָ���pattern�����ַ���str�ָ�ָ��Ľ���ŵ�result��
	int splite(const std::string& str, const std::string& pattern, std::vector<std::string>& result);

	//��ʽ��json
	std::string formatJson(std::string jsonStr);
	std::string getLevelStr(int level);

	//��ʽ��ʱ��
	void timeToString(int time, int& day, int& hour, int& minute, int& second);

	//��
	void vibrate(int milliseconds);

	typedef enum
	{
		NONE, //��
		SINK, //�³�
		DARK  //�䰵
	}ButtonTouchEffect;

	void addButtonTouchEffect(ui::Widget* pWidget,
		ButtonTouchEffect effect,
		const std::function<void(Ref*, ui::Widget::TouchEventType)>& callback);

	void addButtonTouchEffect(ui::CheckBox* pCheckBox,
		const std::function<void(Ref*, ui::CheckBox::EventType)>& callback);

	void addMoveLight(Node* parent, Node* stencil);
}
