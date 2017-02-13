#include "Common/ScrolledBackground.h"
#include "Common/Utils.h"
#include "Level.h"

const float INVALID_SPEED_DELTA = 999999;

ScrolledBackground* ScrolledBackground::create(const TLevelBackground* pTLevelBackground, const Size& size)
{
	ScrolledBackground* ptr = new (std::nothrow) ScrolledBackground();
	if (ptr && ptr->init(pTLevelBackground, size))
	{
		ptr->autorelease();
		return ptr;
	}
	CC_SAFE_DELETE(ptr);
	return nullptr;
}

ScrolledBackground::ScrolledBackground()
: m_fDelta(0)
, m_fDeltaDelta(INVALID_SPEED_DELTA)
, m_iFrameCnt(0)
, m_bRemove(false)
, m_pTLevelBackground(nullptr)
, m_bConstantSpeed(false)
{
}

ScrolledBackground::~ScrolledBackground()
{
}

bool ScrolledBackground::init(const TLevelBackground* pTLevelBackground, const Size& size)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());

		bRet = true;
	} while (0);

	m_pTLevelBackground = pTLevelBackground;
	m_size = size;

	calculateDelta(pTLevelBackground);

	char szName[100 + 1] = { 0 };
	snprintf(szName, 100, "%s;%d;%d", 
		pTLevelBackground->filename.c_str(), 
		(int)time(nullptr), 
		(int)(Utils::getRandom() * 1000));
	Sprite *bg = Sprite::create(pTLevelBackground->filename);
	bg->setName(szName);
	if (pTLevelBackground->flip_y)
	{
		bg->setFlippedY(true);
	}

	int h = bg->getContentSize().height + pTLevelBackground->distance;
	int cnt = int(m_size.height / h) + 2;
	if (cnt < 2)
	{
		cnt = 2;
	}
	if (cnt > pTLevelBackground->count && pTLevelBackground->count > 0)
	{
		cnt = pTLevelBackground->count;
	}

	incBgCnt(szName, cnt);

	//从下往上排背景
	for (int i = 0; i < cnt; i++)
	{
		if (0 != i)
		{
			bg = Sprite::create(pTLevelBackground->filename);
			bg->setName(szName);
			if (pTLevelBackground->flip_y)
			{
				bg->setFlippedY(true);
			}
		}
		bg->setAnchorPoint(Vec2(0.5, 0));
		bg->setPosition(Vec2(m_size.width / 2, i * h));

		Node::addChild(bg);
		m_backgroundArray.push_back(bg);
	}

	return bRet;
}

//计算背景每一帧移动的距离，已经每一帧这个距离的变化、持续的帧数
void ScrolledBackground::calculateDelta(const TLevelBackground* pTLevelBackground)
{
	m_fDelta = pTLevelBackground->speed_start * Director::getInstance()->getAnimationInterval();

	if (pTLevelBackground->duration > 0)
	{
		float m_fDeltaEnd = pTLevelBackground->speed_end * Director::getInstance()->getAnimationInterval();
		m_fDeltaDelta = (m_fDeltaEnd - m_fDelta) / pTLevelBackground->duration * Director::getInstance()->getAnimationInterval();
		m_iFrameCnt = pTLevelBackground->duration / Director::getInstance()->getAnimationInterval();
	}
	else
	{
		m_fDeltaDelta = INVALID_SPEED_DELTA;
		m_iFrameCnt = 0;
	}
}

void ScrolledBackground::run()
{
	this->scheduleUpdate();
}

void ScrolledBackground::stop()
{
	this->unscheduleUpdate();
}

void ScrolledBackground::regBgListener(BackgroundListener* pListener)
{
	for (auto p : m_bgListenerArray)
	{
		if (p == pListener)
		{
			return;
		}
	}

	m_bgListenerArray.push_back(pListener);
}

void ScrolledBackground::unRegBgListener(BackgroundListener* pListener)
{
	for (auto it = m_bgListenerArray.begin(); it != m_bgListenerArray.end(); it++)
	{
		if (*it == pListener)
		{
			m_bgListenerArray.erase(it);
		}
	}
}

void ScrolledBackground::notifyEnd()
{
	for (auto it = m_bgListenerArray.begin(); it != m_bgListenerArray.end(); it++)
	{
		if (*it != nullptr)
		{
			(*it)->onEnd();
		}
	}
}

void ScrolledBackground::notifyMove(int zorder, float dt)
{
	for (auto it = m_bgListenerArray.begin(); it != m_bgListenerArray.end(); it++)
	{
		if (*it != nullptr)
		{
			(*it)->onMove(zorder, dt);
		}
	}
}

void ScrolledBackground::addBackground(const TLevelBackground* pTLevelBackground)
{
	m_pTLevelBackground = pTLevelBackground;
	m_bRemove = false;
	calculateDelta(pTLevelBackground);

	char szName[100 + 1] = { 0 };
	snprintf(szName, 100, "%s;%d;%d", 
		pTLevelBackground->filename.c_str(), 
		(int)time(nullptr), 
		(int)(Utils::getRandom() * 1000));

	Sprite *bg = Sprite::create(pTLevelBackground->filename);
	bg->setName(szName);
	if (pTLevelBackground->flip_y)
	{
		bg->setFlippedY(true);
	}

	int h = bg->getContentSize().height + pTLevelBackground->distance;
	int cnt = int(m_size.height / h) + 2;
	if (cnt < 2)
	{
		cnt = 2;
	}
	if (cnt > pTLevelBackground->count && pTLevelBackground->count > 0)
	{
		cnt = pTLevelBackground->count;
	}
	incBgCnt(szName, cnt);

	//找到当前背景中的最高点
	float fMaxY = 0;
	for (auto p : m_backgroundArray)
	{
		if (p->getPosition().y + h > fMaxY)
		{
			fMaxY = p->getPosition().y + h;
		}
	}

	for (int i = 0; i < cnt; i++)
	{
		if (0 != i)
		{
			bg = Sprite::create(pTLevelBackground->filename);
			bg->setName(szName);
			if (pTLevelBackground->flip_y)
			{
				bg->setFlippedY(true);
			}
		}
		bg->setAnchorPoint(Vec2(0.5, 0));
		bg->setPosition(Vec2(m_size.width / 2, fMaxY + i * h));

		Node::addChild(bg);
		m_backgroundArray.push_back(bg);
	}

	run();
}

void ScrolledBackground::removeBackground()
{
	m_bRemove = true;
}

void ScrolledBackground::constant(bool c)
{
	m_bConstantSpeed = c;
}

//背景滚动
void ScrolledBackground::update(float dt)
{
	if (m_backgroundArray.size() == 0)
	{
		this->stop();
		return;
	}

	//变速
	if (!m_bConstantSpeed && m_iFrameCnt > 0 && fabs(m_fDeltaDelta - INVALID_SPEED_DELTA) > 1)
	{
		m_fDelta += m_fDeltaDelta;
		m_iFrameCnt--;
	}

	//统统往下偏移 m_fDelta
	float distance = m_fDelta * Director::getInstance()->getScheduler()->getTimeScale();
	for (auto p : m_backgroundArray)
	{
		p->setPositionY(p->getPositionY() + distance);
	}
	this->notifyMove(getLocalZOrder(), distance);

	//当前背景彻底消失在下方
	Sprite* first = m_backgroundArray.front();
	Sprite* last = m_backgroundArray.back();
	if (first->getPositionY() + first->getContentSize().height <= 0)
	{
		if (first->getName() != last->getName() || m_bRemove)
		{
			//切换背景/删除背景
			m_backgroundArray.pop_front();
		}
		else
		{
			//达到上限
			if (m_pTLevelBackground->count > 0 && getBgCnt(first->getName()) >= m_pTLevelBackground->count)
			{
				removeBgCnt(first->getName());
				this->notifyEnd();
			}
			else if (m_iFrameCnt <= 0 && m_pTLevelBackground->duration > 0)
			{
				this->notifyEnd();
			}
			else
			{
				//立即移到最上方
				first->setPositionY(last->getPositionY() + last->getContentSize().height + m_pTLevelBackground->distance);
				m_backgroundArray.push_back(first);
				m_backgroundArray.pop_front();

				incBgCnt(first->getName(), 1);
			}
		}
	}

	//最上面的图片的上边缘已经跟屏幕上边缘重合
	if (last->getPositionY() + last->getContentSize().height <= m_size.height)
	{
		removeBgCnt(last->getName());
		this->notifyEnd();
	}
}

void ScrolledBackground::incBgCnt(const std::string& name, int value)
{
	auto it = m_levelBgCntMap.find(name);
	if (it == m_levelBgCntMap.end())
	{
		m_levelBgCntMap.insert(std::map<std::string, int>::value_type(name, value));
	}
	else
	{
		it->second += value;
	}
}

void ScrolledBackground::removeBgCnt(const std::string& name)
{
	auto it = m_levelBgCntMap.find(name);
	if (it != m_levelBgCntMap.end())
	{
		m_levelBgCntMap.erase(it);
	}
}

int ScrolledBackground::getBgCnt(const std::string& name)
{
	auto it = m_levelBgCntMap.find(name);
	if (it == m_levelBgCntMap.end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}
