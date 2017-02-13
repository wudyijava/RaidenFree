#include "DebugLayer.h"
#include "GameObject.h"

DebugLayer::DebugLayer()
{
}

DebugLayer::~DebugLayer()
{
}

bool DebugLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		
		m_drawNode = DrawNode::create();
		addChild(m_drawNode);

		bRet=true;
	} while (0);
	
	return bRet;
}

void DebugLayer::begin()
{
	m_drawNode->clear();
}

void DebugLayer::end()
{
}

void DebugLayer::drawRect(const Rect& rect)
{
	Vec2 v1 = Vec2(rect.origin.x, rect.origin.y);
	Vec2 v2 = Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
	m_drawNode->drawRect(v1, v2, Color4F(1, 0, 0, 1));
}

void DebugLayer::drawPoint(const Vec2& point)
{
	m_drawNode->drawPoint(point, 4, Color4F(1, 0, 0, 1));
}

//开启精灵位置显示	display_box!!!!!!!!!!!

void DebugLayer::drawAll(const Node* node)
{
	for (Node* n : node->getChildren())
	{
		GameObject* p = dynamic_cast<GameObject*>(n);
		if (p != NULL)
		{
			drawRect(p->getBodyBox());
			drawPoint(p->getPosition());
		}
	}
}