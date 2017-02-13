#pragma once

#include "cocos2d.h"

USING_NS_CC;

#include "Common/ScrolledBackground.h"

class ScrolledBackground;
struct TLevelBackground;

typedef std::map<int, ScrolledBackground*> ScrolledBgMap;

typedef std::map<int, const TLevelBackground*> LevelBgMap;
typedef std::list<LevelBgMap> LevelBgMapList;



class BackgroundLayer : public Layer, public BackgroundListener
{
public:
	BackgroundLayer();
	virtual ~BackgroundLayer();

	CREATE_FUNC(BackgroundLayer);

	void run();
	void change();

	virtual void pause() override;
	virtual void resume() override;

	void constant(bool c); //匀速

	virtual void onEnd() override;
	virtual void onMove(int zorder, float dt) override;

	//添加相对背景静止的物体
	void setOnMoveCallback(const std::function<void(float dt)>& callback);

protected:
	virtual bool init();

private:
	ScrolledBgMap m_scrolledBgMap;
	LevelBgMapList m_levelBgMapList;

	std::function<void(float dt)> m_onMoveCallback;
};
