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

	int isAchieve; //�Ƿ���
	int isReceive; //�Ƿ���ȡ����

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

	//��ʼ��
	bool init();
	//�Ѵ�ɵ�δ��ȡ�ĳɾ͵�����
	int judgeAchievement();
	//�ж��Ƿ���ָ���ɾ�
	bool judgeAchievement(Layer* pLayer, const char* const key);

protected:
	AchievementMgr();
	virtual ~AchievementMgr();

	//��ʾ���ĳ���ɾ͵���Ϣ
	bool displayInfo(Layer* pLayer);

private:
	static AchievementMgr* m_pInstance;

	//Ϊ���Ч�ʣ��轫std::map<int, TAchievementData*>ת��Ϊ���½ṹ
	std::map<std::string, std::vector<TAchievementData*>*> m_achievementMap;

	std::list<TAchievementData*> m_toDisplayInfo;

	bool  m_bDisplayInfo;
};