#include "ModeLevel.h"
#include "ModeEndless.h"
#include "ModeRapid.h"
#include "ModeBase.h"
#include "PlaneLayer.h"
#include "GameData.h"

namespace ModeBase
{
	PlaneLayer* createLayer(int mode)
	{
		GameData::getInstance()->setValue(GAMEDATA::ENDLESS_ENEMY_HP_RATIO, 0.f);

		switch (mode)
		{
		case ModeBase::ModeEndless: return ModeEndless::create();
		case ModeBase::ModeRapid:   return ModeRapid::create();
		default: return ModeLevel::create();
		}
	}


}