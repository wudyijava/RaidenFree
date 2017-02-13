#pragma once

#include "cocos2d.h"

USING_NS_CC;

typedef enum
{
	ReinforceTypeDiamond = 1,
	ReinforceTypeRampage = 2,
	ReinforceTypeKiller  = 3,
	ReinforceTypeShield  = 4,
	ReinforceTypeRepair  = 5,

	ReinforceTypeAircraft = 11,
	ReinforceTypeArmature = 12,
	ReinforceTypeMissile  = 13,
	ReinforceTypeWingman  = 14

}ReinforceType;

struct TReinforceGradeData
{
	int grade; //等级（1-10）
	int money; //所需金币
	std::string key;
	float value;
	std::string remark;//升级说明（例如：获得宝石数量额外+160%）
};

struct TReinforceData
{
	int id;
	std::string name;
	std::string key;
	std::vector<TReinforceGradeData*> gradeArray;

	TReinforceData()
	{
		reset();
	}

	void reset()
	{
		id = -1;
		name = "";
		key = "";
		for (auto p : gradeArray)
		{
			delete p;
		}
		gradeArray.clear();
	}

	TReinforceData(const TReinforceData& copy)
	{
		clone(copy);
	}

	void clone(const TReinforceData& data)
	{
		this->id = data.id;
		this->name = data.name;
		this->key = data.key;

		this->gradeArray.clear();
		for (unsigned i = 0; i < data.gradeArray.size(); i++)
		{
			this->gradeArray.push_back(data.gradeArray.at(i));
		}
	}
};