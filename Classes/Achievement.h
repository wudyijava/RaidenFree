#pragma once

#include "cocos2d.h"
USING_NS_CC;

struct TAchievementData
{
	int id;
	std::string name;
	std::string remark;
	std::string key;
	int value;
	std::string rewardName;
	std::string rewardKey;
	int rewardValue;

	int isAchieve; //是否达成
	int isReceive; //是否领取奖励

	TAchievementData()
	{
		reset();
	}

	void reset()
	{
		id = 0;
		name = "";
		remark = "";
		key = "";
		value = 0;
		rewardName = "";
		rewardKey = "";
		rewardValue = 0;

		isAchieve = 0;
		isReceive = 0;
	}

	TAchievementData(const TAchievementData& copy)
	{
		clone(copy);
	}

	const TAchievementData operator=(const TAchievementData& data)
	{
		clone(data);
	}

	void clone(const TAchievementData& data)
	{
		id = data.id;
		name = data.name;
		remark = data.remark;
		key = data.key;
		value = data.value;
		rewardName = data.rewardName;
		rewardKey = data.rewardKey;
		rewardValue = data.rewardValue;
	}
};

class AchievementMgr
{
public:
	static AchievementMgr* getInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new AchievementMgr();
		}

		return m_pInstance;
	}

	//初始化
	bool init();
	//已达成但未领取的成就的数量
	int judgeAchievement();
	//判断是否达成指定成就
	bool judgeAchievement(Layer* pLayer, const char* const key);

protected:
	AchievementMgr();
	virtual ~AchievementMgr();

	//显示达成某个成就的信息
	bool displayInfo(Layer* pLayer);

private:
	static AchievementMgr* m_pInstance;

	//为提高效率，需将std::map<int, TAchievementData*>转化为以下结构
	std::map<std::string, std::vector<TAchievementData*>*> m_achievementMap;

	std::list<TAchievementData*> m_toDisplayInfo;

	bool  m_bDisplayInfo;
};