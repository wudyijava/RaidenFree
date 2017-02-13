#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BaseLayer : public Layer
{
public:
	BaseLayer()
		: m_bPause(0)
	{

	}

	virtual void pause() override
	{
		++m_bPause;
		//this->pause(this);
		Node::pause();

		for (auto p : getChildren())
		{
			p->pause();
		}

	}

	virtual void resume() override
	{
		--m_bPause;
		if (m_bPause < 0)
		{
			m_bPause = 0;
		}
		//this->resume(this);
		Node::resume();

		for (auto p : getChildren())
		{
			p->resume();
		}

	}

	bool isPause() { return m_bPause > 0; }

protected:
	int m_bPause;
};