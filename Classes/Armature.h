#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct TArmatureData
{
	int id;
	std::string name;
	std::string defaultAction;
	std::string pngFile;
	std::string plistFile;
	std::string jsonFile;

	TArmatureData()
	{
		reset();
	}

	void reset()
	{
		id = -1;
		name = "";
		pngFile = "";
		plistFile = "";
		jsonFile = "";
		defaultAction = "";
	}

	TArmatureData(const TArmatureData& copy)
	{
		clone(copy);
	}

	void clone(const TArmatureData& data)
	{
		this->id = data.id;
		this->name = data.name;
		this->defaultAction = data.defaultAction;

		this->pngFile = data.pngFile;
		this->plistFile = data.plistFile;
		this->jsonFile = data.jsonFile;
	}
};