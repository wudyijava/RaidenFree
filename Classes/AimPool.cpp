#include "AimPool.h"
#include "Aircraft.h"
#include "Boss.h"

namespace AimPoolConstant
{
	const char* const ArmatureAimEnemy = "Anim_AimEnemy";

	const char* const ActionStartAim = "StartAim";
	const char* const ActionAim = "Aim";

	const int PoolSize = 5;
}

AimPool::AimPool()
{
	//TODO ´´½¨³Ø
	for (int i = 0; i < AimPoolConstant::PoolSize; i++)
	{

	}
}

AimPool::~AimPool()
{
	clear();
}

void AimPool::addEnemy(Aircraft* pEnemy)
{
	if (!pEnemy || !pEnemy->isAlive())
	{
		return;
	}

	auto it = m_aimMap.find(pEnemy);
	if (it != m_aimMap.end())
	{
		return;
	}

	if (dynamic_cast<Boss*>(pEnemy) != nullptr)
	{
		return;
	}

	auto pArmature = cocostudio::Armature::create(AimPoolConstant::ArmatureAimEnemy);
	pArmature->getAnimation()->play(AimPoolConstant::ActionStartAim);
	pArmature->getAnimation()->setMovementEventCallFunc(
		[this, pArmature](cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
	{
		if (cocostudio::MovementEventType::START == movementType)
		{
			return;
		}

		pArmature->getAnimation()->play(AimPoolConstant::ActionAim, -1, 1);
	});

	if (pEnemy->getParent())
	{
		pEnemy->getParent()->addChild(pArmature, CONSTANT::ZORDER_ENEMY + 1);
	}
	m_aimMap.insert(std::map<Aircraft*, cocostudio::Armature*>::value_type(pEnemy, pArmature));
}

void AimPool::removeEnemy(Aircraft* pEnemy)
{
	auto it = m_aimMap.find(pEnemy);
	if (it != m_aimMap.end())
	{
		it->second->removeFromParent();
		m_aimMap.erase(it);
	}
}

void AimPool::clear()
{
	for (auto item : m_aimMap)
	{
		item.second->removeFromParent();
	}
	m_aimMap.clear();
	m_aimArray.clear();
}

void AimPool::update(float dt)
{
	for (auto item : m_aimMap)
	{
		if (item.first == nullptr || !item.first->isAlive())
		{
			item.second->setVisible(false);
		}
		else
		{
			item.second->setPosition(item.first->getPosition());
			DEBUG_LOG("AimPool::update : %.2f, %.2f", item.first->getPosition().x, item.first->getPosition().y);
		}
	}
}