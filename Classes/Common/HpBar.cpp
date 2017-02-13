#include "Common/HpBar.h"

HpBar::HpBar()
{
	reset();
}

HpBar::~HpBar()
{
}

void HpBar::reset()
{
	m_fMax = 100.0f;
	m_fCurrent = m_fMax;
	m_bg = NULL;

	m_pBackground = NULL;
	m_pBar = NULL;
	m_bVertical = false;

	m_bgSprite = NULL;
	m_hpSprite = NULL;
	m_pProgressTimer = NULL;

	m_hpSpriteList.clear();
	m_hpProgressList.clear();

}

HpBar* HpBar::createWithLoadingBar(Sprite* bg)
{
	HpBar *pRet = new(std::nothrow) HpBar();
	if (pRet && pRet->init(bg))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool HpBar::init(Sprite* bg)
{
	m_bg = bg;
	return true;
}

void HpBar::addHpBar(ui::LoadingBar* pLoadingBar)
{
	m_barArray.push_back(pLoadingBar);
}

void HpBar::setVisible(bool visible)
{
	m_bg->setVisible(visible);
	for (auto bar : m_barArray)
	{
		bar->setVisible(visible);
	}
	Node::setVisible(visible);
}

void HpBar::setMaxValue(float max)
{
	CCASSERT(max >0.01f, "max value must large then 0.01");
	m_fMax = max;
}

void HpBar::setCurValue(float current)
{
	m_fCurrent = current;

	if (m_barArray.size() > 0)
	{
		int barCnt = m_barArray.size();
		float tmpMax = 100.0f / barCnt;//每一条血槽的血量（0-100）
		float orignProgress = m_fCurrent * 100 / m_fMax;//当前的血量（0-100）
		for (int i = 0; i < barCnt; i++)
		{
			float tmp = orignProgress - tmpMax * i;
			if (tmp >= 0 && tmp < tmpMax)
			{
				m_barArray.at(i)->setPercent(tmp * 100 / tmpMax);
			}
			else if (tmp < 0)
			{
				m_barArray.at(i)->setPercent(0);
			}
			else
			{
				m_barArray.at(i)->setPercent(100);
			}
		}
	}
	else if (m_hpProgressList.size() > 0)
	{
		int barCnt = m_hpProgressList.size() - 1;
		float tmpMax = 100.0f / barCnt;
		float orignProgress = m_fCurrent * 100 / m_fMax;
		for (int i = barCnt; i > 0; i--)
		{
			float tmp = orignProgress - tmpMax * (i - 1);
			if (tmp >= 0 && tmp < tmpMax)
			{
				m_hpProgressList.at(i)->setPercentage(tmp * 100 / tmpMax);
			}
			else if (tmp < 0)
			{
				m_hpProgressList.at(i)->setPercentage(0);
				//this->removeChild(m_hpProgressList.at(i));
				//m_hpProgressList.pop_back();
			}
			else
			{
				m_hpProgressList.at(i)->setPercentage(100);
			}
		}
	}
}

HpBar* HpBar::createWithTexture(Node* parent, std::vector<std::string>& barArray, bool vertical)
{
	HpBar *pRet = new(std::nothrow) HpBar();
	if (pRet && pRet->init(parent, barArray, vertical))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool HpBar::init(Node* parent, std::vector<std::string>& barArray, bool vertical)
{
	parent->addChild(this);

	CCASSERT(barArray.size() >= 2, "HpBar::init : error size of barArray");

	for (auto file : barArray)
	{
		Sprite* p = Sprite::createWithSpriteFrameName(file);

		ProgressTimer* pProgressTimer = ProgressTimer::create(p);
		pProgressTimer->setType(ProgressTimer::Type::BAR);
		if (vertical)
		{
			pProgressTimer->setMidpoint(Vec2(0.5, 1));
			pProgressTimer->setBarChangeRate(Vec2(0, 1));
		}
		else
		{
			pProgressTimer->setMidpoint(Vec2(0, 0.5));
			pProgressTimer->setBarChangeRate(Vec2(1, 0));
		}
		pProgressTimer->setPercentage(100);

		m_hpSpriteList.push_back(p);
		m_hpProgressList.push_back(pProgressTimer);
		addChild(pProgressTimer);
	}

	return true;
}