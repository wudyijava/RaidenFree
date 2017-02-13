#pragma once

#include "cocos2d.h"

USING_NS_CC;

namespace Difficulty
{
	const int Easy = 1;
	const int Normal = 2;
	const int Hard = 3;

	const int Difficulty2Open = 8;
	const int Difficulty3Open = 16;
}


struct TDifficultyData
{
	int id;
	int type;
	std::string name;
	float money;
	float bullet_speed;
	float attack;
	float hp;
	float reward_prob;
	int enemy_grade;
};