#pragma once

#include "Aircraft.h"
#include "Level.h"

USING_NS_CC;

struct TTrackData;

//敌机
class EnemyPlane : public Aircraft
{
public:
	virtual bool init(Node* parent, const TAircraftData* data, const TAircraftLevelData* pLevelData = nullptr);
	virtual void reset();

	virtual void destory() override;

	//自动飞行、自动射击对外接口
	virtual void fly(const TTrackData* pData, FiniteTimeAction* callback = nullptr);
	virtual std::function<void(Node*)> getFlyDoneFunc();
	virtual void flyDone();
	virtual void attack();

	virtual void reduceHp(int hp) override;
	virtual void recoverHp(int hp) override;
	
	//listener
	void regBossHpListener(IBossHpListener* l);

	void setMoney(int m);
	void setScore(int s);
	void setDirectionFlag(int d);
	void setRewardGroupArray(const std::vector<TRewardGroupData*>& array);

	int getMoney();
	int getScore();
	int getDirectionFlag();
	const std::vector<TRewardGroupData*>& getRewardGroup();

	virtual void recycle() override;

protected:
	virtual void update(float dt) override;

	virtual void makeFlyTrack(const TTrackData* pData, Vector<FiniteTimeAction*>* pActionArray);

protected:
	int m_iMoney;
	int m_iScore;
	int m_iDirectionFlag;
	std::vector<TRewardGroupData*> m_RewardGroupArray;

	Vec2 m_lastPos;  //上一次位置

	std::vector<IBossHpListener*>  m_bossHpListenerArray;
};
