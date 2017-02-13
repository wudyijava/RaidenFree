#pragma once

#include "SeniorEnemy.h"
#include "cocostudio/CCArmature.h"
#include "IListener.h"

USING_NS_CC;
using namespace cocostudio;

struct TTrackData;
class IBossHpListener;
class PlaneLayer;

//boss燃烧点设置
struct TBossBurningData
{
	float hpThreshold;  //HP门限百分比。当hp小于等于总HP值的指定百分比时，该记录生效，从大到小排序
	float x;
	float y;

	void reset()
	{
		hpThreshold = 0;
		x = 0;
		y = 0;
	}

	TBossBurningData()
	{
		reset();
	}

	TBossBurningData(const TBossBurningData& copy)
	{
		clone(copy);
	}

	inline void clone(const TBossBurningData& data)
	{
		this->hpThreshold = data.hpThreshold;
		this->x = data.x;
		this->y = data.y;
	}

	inline bool operator<(const TBossBurningData& data) const
	{
		return this->hpThreshold > data.hpThreshold ? true : false;
	}
};

enum EBulletFlag
{
	FLAG_NO_STOP = 0,    //不停止正在发射的子弹
	FLAG_STOP = 1,       //如果前后的action相同，则不停止子弹，否则停止子弹，发射新子弹
	FLAG_FORCE_STOP = 2  //强制停止子弹，发射新子弹
};

struct TBossActionGroupData
{
	int seq;
	std::string actionName;
	int noDie;
	float delay;
	int repeat;
	int bulletGradeId;
	EBulletFlag bulletFlag;

	TBossActionGroupData()
	{
		reset();
	}

	void reset()
	{
		seq = 0;
		actionName = "";
		noDie = 0;
		delay = 0;
		repeat = 0;
		bulletGradeId = 0;
		bulletFlag = EBulletFlag::FLAG_STOP;
	}

	TBossActionGroupData(const TBossActionGroupData& data)
	{
		this->seq = data.seq;
		this->actionName = data.actionName;
		this->noDie = data.noDie;
		this->delay = data.delay;
		this->repeat = data.repeat;
		this->bulletGradeId = data.bulletGradeId;
		this->bulletFlag = (EBulletFlag)data.bulletFlag;
	}

};

struct TBossHpActionData
{
	int hpThreshold;	//范围1-100
	int seq;
	float delay;
	int repeat; //执行次数，-1表示无限循环
	std::vector<TBossActionGroupData> actionArray;

	TBossHpActionData()
	{
		reset();
	}

	void reset()
	{
		hpThreshold = 0;
		seq = 0;
		delay = 0;
		repeat = 0;
		actionArray.clear();
	}

	TBossHpActionData(const TBossHpActionData& data)
	{
		this->hpThreshold = data.hpThreshold;
		this->seq = data.seq;
		this->delay = data.delay;
		this->repeat = data.repeat;
		this->actionArray.clear();
		for (auto d : data.actionArray)
		{
			this->actionArray.push_back(d);
		}
	}
};


//boss主表
struct TBossData
{
	int aircraftId;
	int hpBarCnt; //把HP分割成几管血
	std::list<TBossBurningData> burningArray; //当hp小于一定值时，在指定位置出现爆炸动画
	std::list<TBossHpActionData> hpActionArray; //当hp小于一定值，且在指定时间
	
	void reset()
	{
		aircraftId = -1;
		hpBarCnt = 0;
		burningArray.clear();
		hpActionArray.clear();

		//TODO 测试
		{
			TBossBurningData burningData;
			burningData.hpThreshold = 70;
			burningData.x = -50;
			burningData.y = 30;
			this->burningArray.push_back(burningData);
		}
		{
			TBossBurningData burningData;
			burningData.hpThreshold = 40;
			burningData.x = 10;
			burningData.y = -15;
			this->burningArray.push_back(burningData);
		}
		//测试

	}

	TBossData()
	{
		reset();
	}

	TBossData(const TBossData& copy)
	{
		clone(copy);
	}

	inline void clone(const TBossData& data)
	{
		aircraftId = data.aircraftId;
		hpBarCnt = data.hpBarCnt;

		burningArray.clear();
		std::list<TBossBurningData>::const_iterator it = data.burningArray.begin();
		for (; it != data.burningArray.end(); it++)
		{
			burningArray.push_back(*it);
		}

		hpActionArray.clear();
		std::list<TBossHpActionData>::const_iterator it2 = data.hpActionArray.begin();
		for (; it2 != data.hpActionArray.end(); it2++)
		{
			hpActionArray.push_back(*it2);
		}
	}
};

//有多次变形的敌机：根据血量的不同和攻击形态进行变形
class Boss : public SeniorEnemy
{
public:
	Boss();
	virtual bool init(Node* parent, const TAircraftData* data, const TAircraftLevelData* pLevelData = nullptr) override;

	virtual void reset() override;

	//自动飞行、自动射击对外接口
	virtual void fly(const TTrackData* pData, FiniteTimeAction* flyDoneCallback = nullptr)override;
	virtual std::function<void(Node*)> getFlyDoneFunc() override;
	//virtual void flyWithTrack();
	virtual void flyDone() override; // std::bind不能调用虚函数，所以这里不能加virtual
	virtual void attack()override;

	void ready();//boss飞到指定位置，开始战斗
	void unready();

	virtual bool isCollsion(const Rect& rect, Rect* pCollsionRect = nullptr) override;

	void movementDone(cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID);
	//爆炸
	virtual void blast();

	//子弹用完通知
	virtual void bulletUseUp() override;

	virtual void recycle() override;

protected:

	void doAction(TBossActionGroupData& actionGroup);
	void doShoot(TBossActionGroupData& actionGroup);

private:

	float m_time;
	TBossData m_boss;

	std::vector<ParticleSystemQuad*> m_fireArray;

	std::list<TBossHpActionData>::iterator m_pCurBossHpActionData;
	int m_iCurActionSeq;
	int m_iActionGroupCnt;  //动作组执行次数
	int m_iActionCnt;       //单个动作执行次数
	std::string m_preAction;

	PlaneLayer* m_pPlaneLayer;
	bool m_bReady;
};
