#pragma once

#include "cocos2d.h"

USING_NS_CC;

class PlaneLayer;

class PopupLayer : public LayerColor
{
public:
	PopupLayer();
	~PopupLayer();

	void setPlaneLayer(Layer* layer);
	Layer* getPlaneLayer();

	void pausePlaneLayer();
	void resumePlaneLayer();

	virtual bool init();
	virtual bool doInit() = 0;
	//CREATE_FUNC(PopupLayer);

	//touch事件监听 屏蔽向下触摸
	virtual bool onTouchBegan(Touch *touch, Event *event) override;
	virtual void onTouchMoved(Touch *touch, Event *event) override;
	virtual void onTouchEnded(Touch* touch, Event* event) override;

	virtual std::function<bool(Touch*, Event*)> getTouchBeganFunc() { return CC_CALLBACK_2(PopupLayer::onTouchBegan, this); };
	virtual std::function<void(Touch*, Event*)> getTouchMovedFunc() { return CC_CALLBACK_2(PopupLayer::onTouchMoved, this); }
	virtual std::function<void(Touch*, Event*)> getTouchEndedFunc() { return CC_CALLBACK_2(PopupLayer::onTouchEnded, this); }

protected:
	EventListenerTouchOneByOne* m_pListener;

	Layer* m_pPlaneLayer;
};