#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Singleton.h"

USING_NS_CC;

class Aircraft;

struct ArmatureAim
{
	cocostudio::Armature* aim;
	bool inUse;
};

class AimPool : public Singleton<AimPool>
{
	friend class Singleton<AimPool>;

public:
	void addEnemy(Aircraft* pEnemy);
	void removeEnemy(Aircraft* pEnemy);
	void clear();

	virtual void update(float dt);

protected:
	AimPool();
	virtual ~AimPool();

private:
	std::map<Aircraft*, cocostudio::Armature*> m_aimMap;
	Vector<cocostudio::Armature*> m_aimArray;
};