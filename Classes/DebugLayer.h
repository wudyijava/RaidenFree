#pragma once

#include "cocos2d.h"

USING_NS_CC;


class DebugLayer : public Layer
{
public:

	DebugLayer();
	virtual ~DebugLayer();

	CREATE_FUNC(DebugLayer);

	virtual bool init();

	void begin();
	void end();

	void drawRect(const Rect& rect);
	void drawPoint(const Vec2& point);
	void drawAll(const Node* node);

private:
	DrawNode* m_drawNode;
};
