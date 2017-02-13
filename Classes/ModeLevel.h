#pragma once

#include "PlaneLayer.h"
#include "cocos2d.h"

USING_NS_CC;

//´³¹ØÄ£Ê½
class ModeLevel : public PlaneLayer
{
public:
	CREATE_FUNC(ModeLevel);

	virtual bool init() override;
	virtual void setHubLayer(HubLayer* layer) override;

	virtual void procEventSpwanList(float dt) override;
	virtual void doAllPlayerKilled() override;
	virtual void gotoSettleScene(bool win) override;
	virtual void cancelGiftCallback() override;
	virtual void initGameData() override;

	virtual void update(float dt) override;

protected:
	int m_iSoundFlag;
	int m_iOldKillCnt;
};