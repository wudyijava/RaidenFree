#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct TRewardGroupData;
 
struct TLevelBackground
{
	int group_id;
	int sub_group_id;
	int zorder;
	std::string filename;
	float speed_start;
	float speed_end;
	float duration;
	float distance;
	int count;
	int flip_y;
};

struct TLevelEventData
{
	int group_id;
	int seq;
	std::string event;
	float time;
	int pause;
	int object_id;
	int bullet;
	int track;
	int direction_flag;
	int repeat;
	float interval;
	std::vector<TRewardGroupData*> rewardArray;

	void reset()
	{
		group_id = 0;
		seq = 0;
		event = "";
		time = 0;
		pause = 0;
		object_id = 0;
		bullet = 0;
		track = 0;
		repeat = 0;
		direction_flag = 0;
		interval = 0;
		rewardArray.clear();
	}

	TLevelEventData()
	{
		reset();
	}

	TLevelEventData(const TLevelEventData& copy)
	{
		clone(copy);
	}

	inline void clone(const TLevelEventData& data)
	{
		group_id = data.group_id;
		seq = data.seq;
		event = data.event;
		time = data.time;
		pause = data.pause;
		object_id = data.object_id;
		bullet = data.bullet;
		track = data.track;
		direction_flag = data.direction_flag;
		repeat = data.repeat;
		interval = data.interval;

		rewardArray.clear();
		for (unsigned i = 0; i < data.rewardArray.size(); i++)
		{
			rewardArray.push_back(data.rewardArray.at(i));
		}
	}

	inline bool operator<(const TLevelEventData& data) const
	{
		return this->time < data.time ? true : false;
	}

	inline bool operator==(const TLevelEventData& data) const
	{
		if (this->group_id == data.group_id
			&& this->seq == data.seq
			&& this->event == data.event
			&& this->time == data.time
			&& this->pause == data.pause
			&& this->object_id == data.object_id
			&& this->track == data.track
			&& this->direction_flag == data.direction_flag
			&& this->repeat == data.repeat
			&& this->interval == data.interval
			&& this->rewardArray.size() == data.rewardArray.size())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct TLevelRewardData
{
	int id;
	int cnt;
	float probability;

	void reset()
	{
		id = -1;
		cnt = 0;
		probability = 0;
	}

	TLevelRewardData()
	{
		reset();
	}

	TLevelRewardData(const TLevelRewardData& copy)
	{
		clone(copy);
	}

	inline void clone(const TLevelRewardData& data)
	{
		id = data.id;
		cnt = data.cnt;
		probability = data.probability;
	}
};

typedef enum
{
	BossTypeNone = 1,
	BossTypeSmall = 2,
	BossTypeBig = 3
}BossType;

struct TLevelData
{
	int id;
	std::string name;
	int musicId;
	BossType bossType;

	std::string boss;
	float boss_time;
	std::string boss_music;
	int piece;

	std::vector<TLevelBackground*>* background;
	std::vector<TLevelRewardData*>* reward;
	std::vector<TLevelEventData*>* enemy;

	void reset()
	{
		id = -1;
		name = "";
		musicId = -1;
		boss = "";
		boss_time = 0;
		boss_music = "";
		piece = 0;

		background->clear();
		reward->clear();
		enemy->clear();
	}

	TLevelData()
	{
		id = -1;
		name = "";
		musicId = -1;
		boss = "";
		boss_time = 0;
		boss_music = "";
		piece = 0;

		background = nullptr;
		reward = nullptr;
		enemy = nullptr;
	}

	TLevelData(const TLevelData& copy)
	{
		clone(copy);
	}

	void clone(const TLevelData& data)
	{
		id = data.id;
		name = data.name;
		musicId = data.musicId;
		boss = "";
		boss_time = 0;
		boss_music = "";

		background->clear();
		for (unsigned i = 0; i < data.background->size(); i++)
		{
			background->push_back(data.background->at(i));
		}

		reward->clear();
		for (unsigned i = 0; i < data.reward->size(); i++)
		{
			reward->push_back(data.reward->at(i));
		}

		enemy->clear();
		for (unsigned i = 0; i < data.enemy->size(); i++)
		{
			enemy->push_back(data.enemy->at(i));
		}
	}

};