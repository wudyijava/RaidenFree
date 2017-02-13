#include "AircraftPool.h"

AircraftPool* AircraftPool::m_pInstance = nullptr;

AircraftPool* AircraftPool::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new AircraftPool();
	}

	return m_pInstance;
}

AircraftPool::AircraftPool()
{
}

//回收一架飞机
void AircraftPool::recycle(Aircraft* pAircraft)
{
	//如果在池中，则回收；否则直接销毁
	auto it = m_aircraftMap.find(pAircraft->getAircraftLevelId());
	std::vector<Aircraft*>* pArray = nullptr;
	if (it != m_aircraftMap.end())
	{
		pArray = it->second;
		for (Aircraft* p : *pArray)
		{
			if (p == pAircraft)
			{
				p->recycle();
				return;
			}
		}
	}

	pAircraft->destory();
}

//清空pool
void AircraftPool::release()
{
	for (auto it : m_aircraftMap)
	{
		bool bInUse = false;
		std::vector<Aircraft*>* pArray = it.second;
		for (Aircraft* p : *pArray)
		{
			if (p && p->isRecycled())
			{
				p->destory();
				p->release();
			}
			else if (p)
			{
				bInUse = true;
				DEBUG_LOG("Aircraft[%d] can't release", p->getId());
				//CCASSERT(false, "Release error aircraft");
			}
		}

		if (!bInUse)
		{
			pArray->clear();
			delete pArray;
		}
	}

	m_aircraftMap.clear();
}
