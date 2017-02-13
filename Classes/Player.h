#pragma once

#include "cocos2d.h"
#include "Aircraft.h"

USING_NS_CC;

namespace PlayerFlyStatus
{
	//正常
	const int Normal = 0;

	//向左侧身、一直侧着、恢复正常
	const int TurnLeft = 11;
	const int Left = 12;
	const int LeftRecover = 13;

	//向右侧身、一直侧着、恢复正常
	const int TurnRight = 21;
	const int Right = 22;
	const int RightRecover = 23;

	//变成暴走、一直暴走、恢复正常
	const int TurnRampage = 31;
	const int Rampage = 32;
	const int RampageRecover = 33;
};

//玩家飞机
class PlayerPlane : public Aircraft
{
public:
	virtual bool init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData = nullptr);
	virtual void reset();

	virtual void jumpBy(Vec2 offset, bool inWindow);

	virtual bool upgradeBullet() override;
	virtual bool downgradeBullet() override;
	virtual bool resetBulletLevel() override;
	virtual void autoChangeBulletGrade() override;

	virtual void reduceHp(int hp);
	virtual void recoverHp(int hp);
	
protected:
	bool initHpBar(Node* parent, const TAircraftData* pData);

	void toLeft();
	void toMiddle();
	void toRight();
	void toRampage();
	void toNormal();

	void playArmature(int status, int endStatus);

	virtual void update(float dt) override;

protected:
	int m_flyStatus;
	std::map<int, std::string> m_armatureMap;

	int m_iFrameCnt;
};
