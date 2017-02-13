#include "SeniorEnemy.h"
#include "Common/Macro.h"
#include "GlobalData.h"
#include "Track.h"
#include "Common/Shake.h"

SeniorEnemy::SeniorEnemy()
: m_bFlyDone(false)
, m_bToShoot(false)
, m_bTransForm(false)
, m_pTrackData(nullptr)
{

}

void SeniorEnemy::fly(const TTrackData* pData, FiniteTimeAction* flyDoneCallback)
{
	Vec2 start(pData->startPosX, pData->startPosY);
	this->setPosition(start);
	m_lastPos = start;
	m_pTrackData = pData;

	m_actionArray.clear();
	makeFlyTrack(pData, &m_actionArray);
	m_actionArray.pushBack(CallFunc::create(CC_CALLBACK_0(SeniorEnemy::flyDone, this)));
	Sequence* sequence = Sequence::create(m_actionArray);
	runAction(sequence);
}

void SeniorEnemy::runFlyTrack(const TTrackData* pData)
{
	m_actionArray.clear();
	makeFlyTrack(pData, &m_actionArray);
	
	if (pData->loop)
	{
		m_actionArray.pushBack(CallFunc::create([pData, this](){
			runFlyTrack(pData);
		}));
	}
	Sequence* sequence = Sequence::create(m_actionArray);
	runAction(sequence);
}

void SeniorEnemy::flyDone()
{
	//±äÐÎ²¢Éä»÷
	m_pArmature->getAnimation()->play("Transform_1");
	m_pArmature->getAnimation()->setMovementEventCallFunc(
		[this](cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID)
		{
			if (MovementEventType::START == movementType)
			{
				return;
			}

			transformDone();
		}
	);
}

void SeniorEnemy::transformDone()
{
	m_bTransForm = true;
	m_pArmature->getAnimation()->play("Wait_2");
	m_pArmature->getAnimation()->setMovementEventCallFunc(nullptr);

	this->attack();
}

void SeniorEnemy::attack()
{
	m_bToShoot = true;
	if (m_bTransForm)
	{
		startShoot();
	}
}

void SeniorEnemy::blast()
{
	Aircraft::blast();

	if (getParent())
	{
		getParent()->runAction(Shake::create(.8f, 4));
	}

}
