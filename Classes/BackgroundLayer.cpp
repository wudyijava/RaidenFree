#include "Common/Macro.h"
#include "GlobalData.h"
#include "GameData.h"
#include "BackgroundLayer.h"
#include "Level.h"
#include "ModeBase.h"
#include "Common/ScrolledBackground.h"

BackgroundLayer::BackgroundLayer(void)
{
}

BackgroundLayer::~BackgroundLayer(void)
{
}

bool BackgroundLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		bRet=true;
	} while (0);

	//��ȡȫ�ֹؿ���������
	const std::vector<TLevelBackground*>* levelBgArray = nullptr;
	if (GameData::getInstance()->getValueToInt(GAMEDATA::MODE) == ModeBase::ModeLevel
		|| GameData::getInstance()->getValueToInt(GAMEDATA::MODE) == ModeBase::ModeRapid)
	{
		int level = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
		auto pLevelData = GlobalData::getInstance()->getLevelData(level);
		levelBgArray = pLevelData->background;
	}
	else
	{
		levelBgArray = GlobalData::getInstance()->getRandomBackground();
	}

	//��ȫ�ֹؿ���������ת��Ϊ�ڲ����ݽṹ
	int preSubGroupId = -1;
	for (unsigned i = 0; i < levelBgArray->size(); i++)
	{
		const TLevelBackground* p = levelBgArray->at(i);

		if (p->sub_group_id != preSubGroupId)
		{
			LevelBgMap map;
			m_levelBgMapList.push_back(map);
			preSubGroupId = p->sub_group_id;
		}

		m_levelBgMapList.back().insert(LevelBgMap::value_type(p->zorder, p));
	}

	//m_levelBgMapList: ÿһ��sub_group_id��list��һ��Ԫ�أ�ÿ��Ԫ����һ����zorderΪ��ֵ��map
	if (m_levelBgMapList.size() <= 0)
	{
		bRet = false;
	}

	//���ɵ�һ��sub_group_id�ı���map����zorderΪ��ֵ
	LevelBgMap& m = *m_levelBgMapList.begin();
	for (auto it = m.begin(); it != m.end(); it++)
	{
		ScrolledBackground* pBg = ScrolledBackground::create(it->second, Director::getInstance()->getWinSize());
		pBg->regBgListener(this);
		pBg->setLocalZOrder(it->first);
		addChild(pBg);
		m_scrolledBgMap.insert(ScrolledBgMap::value_type(it->first, pBg));
	}

	this->run();

	return bRet;
}

void BackgroundLayer::run()
{
	for (auto it : m_scrolledBgMap)
	{
		it.second->run();
	}
}

void BackgroundLayer::change()
{
	if (m_levelBgMapList.size() <= 1)
	{
		return;
	}

	m_levelBgMapList.pop_front();

	LevelBgMap& m = *m_levelBgMapList.begin();
	for (auto levelBgIt = m.begin(); levelBgIt != m.end(); levelBgIt++)
	{
		auto scrolledBgIt = m_scrolledBgMap.find(levelBgIt->first);
		if (scrolledBgIt == m_scrolledBgMap.end())
		{
			ScrolledBackground* pBg = ScrolledBackground::create(levelBgIt->second, Director::getInstance()->getWinSize());
			pBg->regBgListener(this);
			pBg->setLocalZOrder(levelBgIt->first);
			addChild(pBg);
			m_scrolledBgMap.insert(ScrolledBgMap::value_type(levelBgIt->first, pBg));
			pBg->run();
		}
		else
		{
			scrolledBgIt->second->addBackground(levelBgIt->second);
		}
	}

	for (auto scrolledBgIt : m_scrolledBgMap)
	{
		auto levelBgIt = m.find(scrolledBgIt.first);
		if (levelBgIt == m.end())
		{
			scrolledBgIt.second->removeBackground();
		}
	}
}

void BackgroundLayer::pause()
{
	for (auto p : m_scrolledBgMap)
	{
		p.second->pause();
	}
}

void BackgroundLayer::resume()
{
	for (auto p : m_scrolledBgMap)
	{
		p.second->resume();
	}
}

void BackgroundLayer::constant(bool c)
{
	for (auto p : m_scrolledBgMap)
	{
		p.second->constant(c);
	}
}

void BackgroundLayer::onEnd()
{
	this->change();
}

void BackgroundLayer::onMove(int zorder, float dt)
{
	//ÿһ��������ÿһ֡�ƶ��ľ���
	if (m_onMoveCallback)
	{
		m_onMoveCallback(dt);
	}
}

void BackgroundLayer::setOnMoveCallback(const std::function<void(float dt)>& callback)
{
	m_onMoveCallback = callback;
}
