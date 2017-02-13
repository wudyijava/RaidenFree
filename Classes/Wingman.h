#pragma once

#include "cocos2d.h"
#include "Aircraft.h"

USING_NS_CC;

namespace WingmanFlyStatus
{
	//正常
	const int Normal = 0;

	//变成暴走、一直暴走、恢复正常
	const int TurnRampage = 31;
	const int Rampage = 32;
	const int RampageRecover = 33;
};

//僚机
class Wingman : public Aircraft
{
public:
	virtual bool init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData = nullptr);

	virtual bool upgradeBullet() override;
	virtual bool downgradeBullet() override;
	virtual bool resetBulletLevel() override;
	virtual void autoChangeBulletGrade() override;

	virtual void setLeft(const Vec2& offset);
	virtual void setRight(const Vec2& offset);

protected:
	virtual bool initBullet(Node* parent, const TAircraftData* pData);

	void toRampage();
	void toNormal();

	void playArmature(int status, int endStatus);

protected:
	int m_flyStatus;
	std::map<int, std::string> m_armatureMap;

};