#pragma once

#include "cocos2d.h"
#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

namespace GuideType
{
	const int WelcomeGirl = 10; //第一次进入游戏后的欢迎说明
	const int WelcomeClickButtonLevel = 20; //点击闯关模式
	const int WelcomeClickMissionOne = 30;  //点击第一关

	//战机的中心是支持飞机的动力源，只要不被击中就可以继续战斗
	//所有道具的说明
	//必杀引导
	//护盾引导
	//const int GamePlanePower = 11;
	const int GameEquipment = 120;
	const int GameKiller = 130;
	const int GameShield = 140;

	//仓库引导：仓库-装甲-金刚之盾
	//强化引导：强化-宝石转化-强化
	//奖励引导：奖励
	const int MenuBag = 210;
	const int MenuBag1 = 211;
	const int MenuBag2 = 212;
	const int MenuStrengthen = 220;
	const int MenuStrengthen1 = 221;
	const int MenuStrengthen2 = 222;
	const int MenuReward = 230;

	//无尽模式开启时，引导+游戏说明
	//急速模式开启时，引导+游戏说明
	//英雄、炼狱难度开启时的引导
	const int ModeEndless = 310;
	const int ModeRapid   = 320;
	const int Difficulty2 = 330;
	const int Difficulty3 = 340;

	//战机开启。美女提醒：战机xxx已研发完成，燃料已加满，随时准备投入战斗
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
	//显示
	Guide* show(int GuideType, Node* pRoot, Node* pTarget, const std::function<void()>& callback = nullptr);
	//还原target
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
	//在最上层创建一个GuideBg，然后把target按钮从原节点上拿下来，添加到GuideBg上
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
