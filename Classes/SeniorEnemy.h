#pragma once

#include "Level.h"
#include "Enemy.h"

USING_NS_CC;


//��һ�α��εĵл���������->����->����
class SeniorEnemy : public EnemyPlane
{
public:
	SeniorEnemy();

	virtual void fly(const TTrackData* pData, FiniteTimeAction* flyDoneCallback = nullptr) override;
	virtual void flyDone()override;
	virtual void transformDone();
	virtual void attack()override;
	virtual void blast()override;

	virtual void runFlyTrack(const TTrackData* pData);

protected:
	bool m_bToShoot;
	bool m_bFlyDone;
	bool m_bTransForm;
	Vector<FiniteTimeAction*> m_actionArray;
	const TTrackData* m_pTrackData;
};
