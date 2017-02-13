#pragma once

#include "cocos2d.h"
#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

namespace GuideType
{
	const int WelcomeGirl = 10; //��һ�ν�����Ϸ��Ļ�ӭ˵��
	const int WelcomeClickButtonLevel = 20; //�������ģʽ
	const int WelcomeClickMissionOne = 30;  //�����һ��

	//ս����������֧�ַɻ��Ķ���Դ��ֻҪ�������оͿ��Լ���ս��
	//���е��ߵ�˵��
	//��ɱ����
	//��������
	//const int GamePlanePower = 11;
	const int GameEquipment = 120;
	const int GameKiller = 130;
	const int GameShield = 140;

	//�ֿ��������ֿ�-װ��-���֮��
	//ǿ��������ǿ��-��ʯת��-ǿ��
	//��������������
	const int MenuBag = 210;
	const int MenuBag1 = 211;
	const int MenuBag2 = 212;
	const int MenuStrengthen = 220;
	const int MenuStrengthen1 = 221;
	const int MenuStrengthen2 = 222;
	const int MenuReward = 230;

	//�޾�ģʽ����ʱ������+��Ϸ˵��
	//����ģʽ����ʱ������+��Ϸ˵��
	//Ӣ�ۡ������Ѷȿ���ʱ������
	const int ModeEndless = 310;
	const int ModeRapid   = 320;
	const int Difficulty2 = 330;
	const int Difficulty3 = 340;

	//ս����������Ů���ѣ�ս��xxx���з���ɣ�ȼ���Ѽ�������ʱ׼��Ͷ��ս��
	const int Aircraft2Open = 420;
	const int Aircraft3Open = 430;

};

#define DECLARE_GUIDE(name)\
private:\
	static bool mInit; \
	static int mType; \
public:\
	static Guide* create()\
	{\
		return new name(); \
	}\
	static bool checkCondition();

#define REGISTER_GUIDE(type, name) \
	bool name::mInit = GuideMgr::getInstance()->registerFunc(type, name::checkCondition, name::create); \
	int name::mType = type;

class Guide : public PopupLayer
{
public:
	Guide();

public:
	//��ʾ
	Guide* show(int GuideType, Node* pRoot, Node* pTarget, const std::function<void()>& callback = nullptr);
	//��ԭtarget
	void restore();

	void setPrompt(const std::string& prompt);

	virtual void onTouchEnded(Touch *touch, Event *event) override;
	virtual std::function<void(Touch*, Event*)> getTouchEndedFunc() override
	{
		return CC_CALLBACK_2(Guide::onTouchEnded, this);
	}

	virtual void onEnter() override;
	virtual void onExit() override;

protected:
	//�����ϲ㴴��һ��GuideBg��Ȼ���target��ť��ԭ�ڵ�������������ӵ�GuideBg��
	virtual bool doInit() override;

protected:
	int m_iGuideType;
	std::function<void()> m_callback;

	Node* m_pRoot;

	Node* m_pTarget;
	int m_targetLocalZOrder;
	Vec2 m_targetPos;
	Node* m_targetParent;

	ui::Text* m_pTextPrompt;
	std::string m_prompt;
	bool m_touchEnable;
};

class GuideMgr
{
public:
	static GuideMgr* getInstance();

	bool registerFunc(int type, const std::function<bool()>& checkFunc, const std::function<Guide*()>& createFunc);

	Guide* show(int GuideType, Node* parent, Node* widget, const std::function<void()>& callback = nullptr);
	bool destory(int GuideType);

	void incGuideCnt();
	void decGuideCnt();
	bool hasGuide();

protected:
	GuideMgr();

private:
	std::map<int, std::function<bool()>> m_condMap;
	std::map<int, std::function<Guide*()>> m_createMap;
	std::map<int, Guide*> m_objectMap;
	int m_iCnt;

	static GuideMgr* m_pInstance;
};
