#pragma once

#include "SeniorEnemy.h"
#include "cocostudio/CCArmature.h"
#include "IListener.h"

USING_NS_CC;
using namespace cocostudio;

struct TTrackData;
class IBossHpListener;
class PlaneLayer;

//bossȼ�յ�����
struct TBossBurningData
{
	float hpThreshold;  //HP���ްٷֱȡ���hpС�ڵ�����HPֵ��ָ���ٷֱ�ʱ���ü�¼��Ч���Ӵ�С����
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
	FLAG_NO_STOP = 0,    //��ֹͣ���ڷ�����ӵ�
	FLAG_STOP = 1,       //���ǰ���action��ͬ����ֹͣ�ӵ�������ֹͣ�ӵ����������ӵ�
	FLAG_FORCE_STOP = 2  //ǿ��ֹͣ�ӵ����������ӵ�
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
	int hpThreshold;	//��Χ1-100
	int seq;
	float delay;
	int repeat; //ִ�д�����-1��ʾ����ѭ��
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


//boss����
struct TBossData
{
	int aircraftId;
	int hpBarCnt; //��HP�ָ�ɼ���Ѫ
	std::list<TBossBurningData> burningArray; //��hpС��һ��ֵʱ����ָ��λ�ó��ֱ�ը����
	std::list<TBossHpActionData> hpActionArray; //��hpС��һ��ֵ������ָ��ʱ��
	
	void reset()
	{
		aircraftId = -1;
		hpBarCnt = 0;
		burningArray.clear();
		hpActionArray.clear();

		//TODO ����
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
		//����

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

//�ж�α��εĵл�������Ѫ���Ĳ�ͬ�͹�����̬���б���
class Boss : public SeniorEnemy
{
public:
	Boss();
	virtual bool init(Node* parent, const TAircraftData* data, const TAircraftLevelData* pLevelData = nullptr) override;

	virtual void reset() override;

	//�Զ����С��Զ��������ӿ�
	virtual void fly(const TTrackData* pData, FiniteTimeAction* flyDoneCallback = nullptr)override;
	virtual std::function<void(Node*)> getFlyDoneFunc() override;
	//virtual void flyWithTrack();
	virtual void flyDone() override; // std::bind���ܵ����麯�����������ﲻ�ܼ�virtual
	virtual void attack()override;

	void ready();//boss�ɵ�ָ��λ�ã���ʼս��
	void unready();

	virtual bool isCollsion(const Rect& rect, Rect* pCollsionRect = nullptr) override;

	void movementDone(cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID);
	//��ը
	virtual void blast();

	//�ӵ�����֪ͨ
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
	int m_iActionGroupCnt;  //������ִ�д���
	int m_iActionCnt;       //��������ִ�д���
	std::string m_preAction;

	PlaneLayer* m_pPlaneLayer;
	bool m_bReady;
};
