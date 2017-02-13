#pragma once

#include "PlaneLayer.h"
#include "cocos2d.h"

USING_NS_CC;

class ModeRapid : public PlaneLayer
{
public:
	ModeRapid();

	CREATE_FUNC(ModeRapid);

	virtual bool init() override;
	virtual void setHubLayer(HubLayer* layer) override;

	virtual void procEventSpwanList(float dt) override;
	virtual void doAllPlayerKilled() override;
	virtual void gotoSettleScene(bool win) override;
	virtual void cancelGiftCallback() override;
	virtual void initGameData() override;

protected:
	virtual void updateMove(float dt);
	virtual void spwanEnemy();

protected:
	int m_rapidLevel;
	int m_tmpSeq;
	bool m_startFlag;

	float m_fMinMovement;
	bool m_bMinMovement;
	float m_fMovementRatio;

	float m_fMovementSum;//飞行的总路程，像素
	float m_fTime;//用于判断是否超时
	float m_fInterval;
};