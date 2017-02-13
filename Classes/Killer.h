#pragma once

#include "cocos2d.h"
#include "Aircraft.h"

USING_NS_CC;


//***********************************************************//
//
//  ≥¨º∂±ÿ…±
//
//***********************************************************//
class SuperKiller : public Aircraft
{
public:
	static SuperKiller* create(Node* pParent, const TAircraftData* pData);

	virtual bool init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData = nullptr);
	virtual void fly();
};
