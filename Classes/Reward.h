#pragma once

#include "cocos2d.h"
#include "GameObject.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

class Aircraft;

//常量
const char* const REWARD_TYPE_WEAPON = "weapon";   //武器升级
const char* const REWARD_TYPE_RAMPAGE = "rampage"; //暴走
const char* const REWARD_TYPE_REPAIR = "repair";   //恢复生命
const char* const REWARD_TYPE_KILLER = "killer";   //超级必杀
const char* const REWARD_TYPE_SHIELD = "shield";   //量子护盾
const char* const REWARD_TYPE_MONEY = "money";     //钱(宝石)

const char* const REWARD_ATTR_HP = "HP";
const char* const REWARD_ATTR_WINGMAN = "WINGMAN_ID";
const char* const REWARD_ATTR_SCORE = "SCORE";

struct TRewardData
{
	int id;
	std::string name;
	std::string type;
	std::vector<std::string> styleArray;
	float aniDura;
	std::string armatureName;

	std::string prompt;
	int musicId;

	float speed;
	float maxSpeed;
	float radius;
	float lifeTime;

	float bodyCenterX;
	float bodyCenterY;
	float bodySizeW;
	float bodySizeH;

	std::map<std::string, float> paramMap;

	TRewardData()
	{
		reset();
	}

	void reset()
	{
		this->id = -1;
		this->name = "";
		this->type = "";
		this->styleArray.clear();
		this->aniDura = 0;
		this->armatureName = "";

		this->prompt = "";
		this->musicId = -1;

		this->speed = 0;
		this->maxSpeed = 0;
		this->radius = 0;
		this->lifeTime = 0;

		this->bodyCenterX = 0;
		this->bodyCenterY = 0;
		this->bodySizeW = 0;
		this->bodySizeH = 0;

		this->paramMap.clear();
	}

	TRewardData(const TRewardData& copy)
	{
		clone(copy);
	}

	void clone(const TRewardData& data)
	{
		this->id = data.id;
		this->name = data.name;
		this->type = data.type;
		this->styleArray.clear();
		for (unsigned i = 0; i < data.styleArray.size(); i++)
		{
			this->styleArray.push_back(data.styleArray.at(i));
		}
		this->aniDura = data.aniDura;
		this->armatureName = data.armatureName;

		this->prompt = data.prompt;
		this->musicId = data.musicId;

		this->speed = data.speed;
		this->maxSpeed = data.maxSpeed;
		this->radius = data.radius;
		this->lifeTime = data.lifeTime;

		this->bodyCenterX = data.bodyCenterX;
		this->bodyCenterY = data.bodyCenterY;
		this->bodySizeW = data.bodySizeW;
		this->bodySizeH = data.bodySizeH;

		this->paramMap.clear();
		std::map<std::string, float>::const_iterator it = data.paramMap.begin();
		for (; it != data.paramMap.end(); it++)
		{
			this->paramMap.insert(std::map<std::string, float>::value_type(it->first, it->second));
		}
	}
};

struct TRewardGroupData
{
	int rewardId;
	int cnt;
	float probability;
};

class Reward : public GameObject
{
	friend class RewardWeapon;

public:
	bool init(const Node* pParent, const Vector<Aircraft*>* pPlayerArray, const TRewardData* pData);
	void destory();
	void reset();

	Reward();
	virtual ~Reward();

	virtual void fly(const Size& planeSize);
	virtual void update(float delta);

protected:
	virtual void doReward() = 0;
	virtual void doPrompt();
	void doPromptDone(Sprite* pSprite);

	virtual void doPrompt(const std::string& filename, int startIndex, int endIndex);

protected:
	TRewardData m_data;
	const Vector<Aircraft*>* m_pPlayerArray;

	Vec2 m_prePos;
	Vec2 m_curPos;

	float m_speedX;
	float m_speedY;

	float m_time;
};

//***************************************************
//具体的子类
//***************************************************

//武器升级
class RewardWeapon : public Reward
{
protected:
	virtual void doReward() override;
	virtual void doPrompt() override;
};

//武器升级到最高级：暴走
class RewardRampage : public RewardWeapon
{
protected:
	virtual void doReward() override;
};

//装甲修复	增加HP
class RewardRepair : public Reward
{
protected:
	virtual void doReward() override;
	virtual void doPrompt() override;
};

//获得超级必杀
class RewardKiller : public Reward
{
protected:
	virtual void doReward() override;
	virtual void doPrompt() override;
};

//获得量子护盾
class RewardShield : public Reward
{
protected:
	virtual void doReward() override;
	virtual void doPrompt() override;
};

//获得钱
class RewardMoney : public Reward
{
public:
	RewardMoney();
	virtual void fly(const Size& planeSize) override;
	virtual void update(float delta) override;

	void setMoney(int money);

protected:
	virtual void doReward() override;

private:
	int m_iMoney;
};
//***************************************************
//工厂类
//***************************************************
class RewardCreator
{
public:
	//各种奖励出现的概率为默认值
	static Reward* create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, int id);
	static Reward* create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, const char* const rewardType);

private:
	static Reward* create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, const TRewardData* pRewardData);
};

template<typename T>
class RewardCreatorTpl
{
public:
	static T* create(const Node* parent, const Vector<Aircraft*>* pPlayerArray, const TRewardData* data)
	{
		T *pRet = new(std::nothrow) T();
		if (pRet && pRet->init(parent, pPlayerArray, data))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
};
