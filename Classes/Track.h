#pragma once

#include "cocos2d.h"
#include "Common/Macro.h"

USING_NS_CC;

const char* const TRACK_TYPE_LINE = "LINE";
const char* const TRACK_TYPE_BESSEL = "BESSEL";

//飞行轨迹数据
struct TPos
{
	float speed;
	float x;
	float y;
	std::string path;
	float param_1;
	float param_2;
	float param_3;
	float param_4;

	TPos()
	{
		reset();
	}

	void reset()
	{
		speed = 0;
		x = 0;
		y = 0;
		path = "";
		param_1 = 0;
		param_2 = 0;
		param_3 = 0;
		param_4 = 0;
	}

	TPos(const TPos& copy)
	{
		clone(copy);
	}

	const TPos operator=(const TPos& data)
	{
		clone(data);
	}

	void clone(const TPos& data)
	{
		this->speed = data.speed;
		this->x = data.x;
		this->y = data.y;
		this->path = data.path;
		this->param_1 = data.param_1;
		this->param_2 = data.param_2;
		this->param_3 = data.param_3;
		this->param_4 = data.param_4;
	}
};

struct TTrackData
{
	int id;
	std::string name;

	int loop;
	float startPosX;  //起始位置
	float startPosY;

	std::vector<TPos> pos;

	TTrackData()
	{
		reset();
	}

	void reset()
	{
		id = -1;
		name = "";
		loop = 0;
		startPosX = 0;
		startPosY = 0;
		pos.clear();
	}

	TTrackData(const TTrackData& copy)
	{
		clone(copy);
	}

	void clone(const TTrackData& data)
	{
		this->id = data.id;
		this->name = data.name;
		this->loop = data.loop;

		this->startPosX = data.startPosX;
		this->startPosY = data.startPosY;

		this->pos.clear();
		for (unsigned i = 0; i < data.pos.size(); i++)
		{
			this->pos.push_back(data.pos.at(i));
		}
	}
};