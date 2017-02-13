#pragma once

#include "PlaneLayer.h"
#include "cocos2d.h"

USING_NS_CC;

struct EnemyGroup
{
	int enemy1;
	int enemy2;
	int enemy3;
	int boss;

	EnemyGroup(int e1, int e2, int e3, int b)
		: enemy1(e1)
		, enemy2(e2)
		, enemy3(e3)
		, boss(b)
	{}
};

class ModeEndless : public PlaneLayer
{
public:
	ModeEndless();

	CREATE_FUNC(ModeEndless);

	virtual void procEventSpwanList(float dt) override;
	virtual void doAllPlayerKilled() override;
	virtual void gotoSettleScene(bool win) override;
	virtual void cancelGiftCallback() override;
	virtual void initGameData() override;

protected:
	std::vector<EnemyGroup> m_enemyGroupArray;

	int m_endlessLevel;
	int m_tmpSeq;
	bool m_startFlag;

	float m_fEnemyHpAdd;
};