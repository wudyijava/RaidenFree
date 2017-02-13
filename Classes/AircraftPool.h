#pragma once

#include "Aircraft.h"
#include "ModeBase.h"
#include "GameData.h"

USING_NS_CC;

class AircraftPool
{
public:
	static AircraftPool* getInstance();

	//获取一架飞机
	template<typename T>
	T* get(Node* parent, const TAircraftData* pAircraftData, const TAircraftLevelData* pAircraftLevelData)
	{
		//AircraftPool 只在急速模式下使用。其他两种模式反而会增加内存
		if (GameData::getInstance()->getValueToInt(GAMEDATA::MODE) != ModeBase::ModeRapid)
		{
			return PlaneCreator<T>::create(parent, pAircraftData, pAircraftLevelData);
		}

		auto it = m_aircraftMap.find(pAircraftLevelData->id);
		std::vector<Aircraft*>* pArray = nullptr;
		if (it != m_aircraftMap.end())
		{
			pArray = it->second;
		}
		else
		{
			pArray = new std::vector<Aircraft*>;
			m_aircraftMap.insert(std::map<int, std::vector<Aircraft*>*>::value_type(pAircraftLevelData->id, pArray));
		}

		//查找可用的飞机
		for (Aircraft* p : *pArray)
		{
			if (p && p->isRecycled())
			{
				p->reuse();
				T* ret = dynamic_cast<T*>(p);
				if (ret)
				{
					parent->addChild(ret);
					return ret;
				}
				else
				{
					DEBUG_LOG("fuck error type of aircraft");
				}
			}
		}

		//没找到，新建一个
		auto p = PlaneCreator<T>::create(parent, pAircraftData, pAircraftLevelData);
		p->retain();
		for (unsigned i = 0; i < pArray->size(); i++)
		{
			if ((*pArray)[i] == nullptr)
			{
				(*pArray)[i] = p;
				return p;
			}
		}

		pArray->push_back(p);
		return p;
	}

	//回收一架飞机
	void recycle(Aircraft* pAircraft);

	//清空pool
	void release();

protected:
	AircraftPool();

private:
	static AircraftPool* m_pInstance;

	std::map<int, std::vector<Aircraft*>*> m_aircraftMap;
};