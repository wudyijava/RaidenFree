#include "Equipment.h"
#include "GlobalData.h"
#include "Common/Utils.h"

PlayerEquip* PlayerEquip::m_pInstance = nullptr;

PlayerEquip* PlayerEquip::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new PlayerEquip;
	}

	return m_pInstance;
}

void PlayerEquip::destory()
{
	//destory m_playerEquipMap
}

std::map<int, std::vector<const TEquipmentData*>*>& PlayerEquip::getAllPlayerEquip()
{
	return m_playerEquipMap;
}

std::vector<const TEquipmentData*>* PlayerEquip::getEquip(int playerId)
{
	auto it = m_playerEquipMap.find(playerId);
	if (it != m_playerEquipMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

const TEquipmentData* PlayerEquip::getEquip(int playerId, EquipType type)
{
	auto it = m_playerEquipMap.find(playerId);
	if (it != m_playerEquipMap.end())
	{
		for (auto p : *(it->second))
		{
			if (p->type == type)
			{
				return p;
			}
		}
	}

	return nullptr;
}

void PlayerEquip::addEquip(int playerId, const TEquipmentData* pEquip)
{
	(const_cast<TEquipmentData*>(pEquip))->own = 1;

	auto it = m_playerEquipMap.find(playerId);
	if (it != m_playerEquipMap.end())
	{
		for (auto p : *it->second)
		{
			if (p->id == pEquip->id)
			{
				return;
			}
		}

		it->second->push_back(pEquip);
	}
	else
	{
		auto p = new std::vector<const TEquipmentData*>;
		p->push_back(pEquip);
		m_playerEquipMap.insert(std::map<int, std::vector<const TEquipmentData*>*>::value_type(playerId, p));
	}
}

void PlayerEquip::addEquip(int playerId, int equipId)
{
	this->addEquip(playerId, GlobalData::getInstance()->getEquipment(equipId));
}

void PlayerEquip::removeEquip(int playerId, int equipId)
{
	auto it = m_playerEquipMap.find(playerId);
	if (it != m_playerEquipMap.end())
	{
		auto array = it->second;
		if (array->size() == 0)
		{
			return;
		}

		for (auto it2 = array->begin(); it2 != array->end(); it2++)
		{
			if ((*it2)->id == equipId)
			{
				array->erase(it2);
				return;
			}
		}
	}
}

void PlayerEquip::getRandomPiece(unsigned piece, std::set<const TEquipmentData*>& equipPieceArray)
{
	const std::vector<TEquipmentData*>& array1 = GlobalData::getInstance()->getEquipmentArray(EquipTypeArmature);
	const std::vector<TEquipmentData*>& array2 = GlobalData::getInstance()->getEquipmentArray(EquipTypeMissile);
	const std::vector<TEquipmentData*>& array3 = GlobalData::getInstance()->getEquipmentArray(EquipTypeWingman);
	unsigned total = array1.size() + array2.size() + array3.size();
	unsigned idx = 0;
	equipPieceArray.clear();
	for (unsigned i = 0; i < piece || equipPieceArray.size() < piece; i++)
	{
		int random = total * Utils::getRandom();
		idx += random;
		if (idx < 0)
		{
			idx = 0;
		}
		while (idx >= total)
		{
			idx -= total;
		}

		if (idx < array1.size())
		{
			array1.at(idx)->pick++;
			equipPieceArray.insert(array1.at(idx));
		}
		else if (idx >= array1.size() && idx < array1.size() + array2.size())
		{
			int ii = idx - array1.size();
			array2.at(ii)->pick++;
			equipPieceArray.insert(array2.at(ii));
		}
		else if (idx >= array1.size() + array2.size() && idx < total)
		{
			int ii = idx - array1.size() - array2.size();
			array3.at(ii)->pick++;
			equipPieceArray.insert(array3.at(ii));
		}
	}
}
