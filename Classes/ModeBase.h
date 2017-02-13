#pragma once

#include "cocos2d.h"

USING_NS_CC;

class PlaneLayer;

namespace ModeBase
{
	enum GameMode
	{
		ModeLevel = 1,
		ModeEndless = 2,
		ModeRapid = 3
	};

	const int ModeEndlessOpen = 2;
	const int ModeRapidOpen = 5;

	const int LevelIdEndless = 801;
	const int LevelIdRapid = 901;

	PlaneLayer* createLayer(int mode);
}

