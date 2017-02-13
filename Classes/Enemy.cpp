#include "Enemy.h"
#include "PlaneLayer.h"
#include "Common/Macro.h"
#include "GlobalData.h"
#include "Track.h"
#include "Difficulty.h"
#include "GameData.h"
#include "AircraftPool.h"

void EnemyPlane::reset()
{
	Aircraft::reset();

	m_iMoney = 0;
	m_iScore = 0;
	m_iDirectionFlag = 0;
}

bool EnemyPlane::init(Node* parent, const TAircraftData* data, const TAircraftLevelData* pLevelData)
{
	bool bRet = false;
	do
	{
		m_eAircraftType = EAircraftType::Type_Enemy;
		this->setLocalZOrder(CONSTANT::ZORDER_ENEMY);

		//难度增加攻击加成
		float bulletAttackAdjust = GlobalData::getInstance()->getDifficulty()->attack;
		setAttackAdjust(bulletAttackAdjust);

		//难度增加子弹速度加成
		float bulletSpeedAdjust = GlobalData::getInstance()->getDifficulty()->bullet_speed;
		setBulletSpeedAdjust(bulletSpeedAdjust);

		//初始化敌机
		CC_BREAK_IF(!Aircraft::init(parent, data, pLevelData));

		//难度增加生命加成
		float hpChange = GlobalData::getInstance()->getDifficulty()->hp;
		float endlessHpAdd = GameData::getInstance()->getValueToFloat(GAMEDATA::ENDLESS_ENEMY_HP_RATIO);
		m_data.maxHp = (int)((m_data.maxHp * (1 + hpChange) + endlessHpAdd) + 0.5);
		m_data.hp = m_data.maxHp;

		bRet = true;
	} while (0);

	return bRet;
}

//void EnemyPlane::setLevelData(const TLevelEventData& levelEventData)
//{
//	m_levelEventData.clone(levelEventData);
//
//	if (m_levelEventData.direction_flag)
//	{
//		scheduleUpdate();
//	}
//}

void EnemyPlane::makeFlyTrack(const TTrackData* pData, Vector<FiniteTimeAction*>* pActionArray)
{
	Vec2 src(pData->startPosX, pData->startPosY);

	for (unsigned i = 0; i < pData->pos.size(); i++)
	{
		const TPos& pos = pData->pos.at(i);
		Vec2 dest(pos.x, pos.y);
		FiniteTimeAction* action = NULL;
		if (!strcmp(pos.path.c_str(), TRACK_TYPE_LINE))
		{

			float distance = src.distance(dest);
			action = CCMoveTo::create(distance / pos.speed, dest);
		}
		else if (!strcmp(pos.path.c_str(), TRACK_TYPE_BESSEL))
		{
			ccBezierConfig cfg;
			cfg.endPosition.x = pos.x;
			cfg.endPosition.y = pos.y;
			cfg.controlPoint_1.x = pos.param_1;
			cfg.controlPoint_1.y = pos.param_2;
			cfg.controlPoint_2.x = pos.param_3;
			cfg.controlPoint_2.y = pos.param_4;

			float distance = src.distance(cfg.controlPoint_1)
				+ cfg.controlPoint_1.distance(cfg.controlPoint_2)
				+ cfg.controlPoint_2.distance(cfg.endPosition);

			action = BezierTo::create(distance / pos.speed, cfg);
		}

		pActionArray->pushBack(action);

		src = dest;
	}
}

void EnemyPlane::fly(const TTrackData* pData, FiniteTimeAction* callback)
{
	Vec2 start(pData->startPosX, pData->startPosY);
	this->setPosition(start);
	m_lastPos = start;

	Vector<FiniteTimeAction*> actionArray;
	makeFlyTrack(pData, &actionArray);

	if (actionArray.size() > 0)
	{
		actionArray.pushBack(CallFuncN::create(getFlyDoneFunc()));
		if (callback)
		{
			actionArray.pushBack(callback);
		}

		Sequence* sequence = Sequence::create(actionArray);
		runAction(sequence);
	}

	//this->startShoot();
}

std::function<void(Node*)> EnemyPlane::getFlyDoneFunc()
{
	return CC_CALLBACK_0(EnemyPlane::flyDone, this);
}

void EnemyPlane::flyDone()
{
	Vec2 pos(getPosition());
	if (pos.x <= 0 || pos.x >= CONSTANT::DESIGN_RES_WIDTH
		|| pos.y <= 0 || pos.y >= CONSTANT::DESIGN_RES_HEIGHT)
	{
		GameData::getInstance()->setValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0);

		//destory();
		//this->recycle();
		AircraftPool::getInstance()->recycle(this);

		//通知父Layer删除这个对象
		Node* p = getParent();
		PlaneLayer* layer = (PlaneLayer*)(getParent());
		CCASSERT(layer, "layer is not PlaneLayer");

		layer->destoryPlane(this);
	}
}

void EnemyPlane::attack()
{
	startShoot();
}

void EnemyPlane::reduceHp(int hp)
{
	Aircraft::reduceHp(hp);

	for (auto l : m_bossHpListenerArray)
	{
		l->updateBossHp(m_data.hp);
	}

	if (m_pArmature && m_data.hp > 0)
	{
		m_pArmature->setColor(Color3B(255, 85, 85));

		DelayTime* pDelay = DelayTime::create(0.08f);
		FiniteTimeAction* endFunc = CallFunc::create([this]()
		{
			m_pArmature->setColor(Color3B::WHITE);
		});
		Sequence* sequence = Sequence::create(pDelay, endFunc, NULL);
		runAction(sequence);
	}
	else if (m_data.hp > 0)
	{
		this->setColor(Color3B(255, 85, 85));

		DelayTime* pDelay = DelayTime::create(0.08f);
		FiniteTimeAction* endFunc = CallFunc::create([this]()
		{
			this->setColor(Color3B::WHITE);
		});
		Sequence* sequence = Sequence::create(pDelay, endFunc, NULL);
		runAction(sequence);
	}
}

void EnemyPlane::recoverHp(int hp)
{
	Aircraft::recoverHp(hp);

	for (auto l : m_bossHpListenerArray)
	{
		l->updateBossHp(m_data.hp);
	}
}

void EnemyPlane::update(float dt)
{
	const Vec2& pos = getPosition();

	if (fabs(m_lastPos.x - pos.x) < 0.0001 && fabs(m_lastPos.y - pos.y) < 0.0001)
	{
		return;
	}
	//转向飞行的方向
	float angle = -CC_RADIANS_TO_DEGREES((pos - m_lastPos).getAngle());

	this->setRotation(angle - 90);

	m_lastPos = pos;
}

void EnemyPlane::setMoney(int m)
{ 
	m_iMoney = m; 
}

void EnemyPlane::setScore(int s)
{ 
	m_iScore = s; 
}

void EnemyPlane::setDirectionFlag(int d)
{ 
	m_iDirectionFlag = d; 

	if (m_iDirectionFlag)
	{
		scheduleUpdate();
	}

}

void EnemyPlane::setRewardGroupArray(const std::vector<TRewardGroupData*>& array)
{
	m_RewardGroupArray.clear();
	for (auto p : array)
	{
		m_RewardGroupArray.push_back(p);
	}
}

int EnemyPlane::getMoney()
{
	int money = m_iMoney;

	//宝石强化加成
	float moneyRatio = GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_DIAMOND_RATIO);
	money = money * (1 + moneyRatio);

	//难度增加，获得的宝石加成
	moneyRatio = GlobalData::getInstance()->getDifficulty()->money;
	money = (int)((money * (1 + moneyRatio)) + 0.5);

	return money;
}

int EnemyPlane::getScore() 
{ 
	return m_iScore; 
}

int EnemyPlane::getDirectionFlag()
{
	return m_iDirectionFlag;
}

const std::vector<TRewardGroupData*>& EnemyPlane::getRewardGroup() 
{ 
	return m_RewardGroupArray;
}

void EnemyPlane::regBossHpListener(IBossHpListener* l)
{
	l->updateBossHp(m_data.hp);
	l->updateBossMaxHp(m_data.maxHp);
	m_bossHpListenerArray.push_back(l);
}

void EnemyPlane::destory()
{
	Aircraft::destory();

	for (auto l : m_bossHpListenerArray)
	{
		l->hideBossHp();
	}
}

void EnemyPlane::recycle()
{
	Aircraft::recycle();

	if (m_pArmature)
	{
		m_pArmature->setRotation(0);
		m_pArmature->setOpacity(255.f);
		m_pArmature->setScale(m_data.scale);
	}
	else
	{
		this->setRotation(0);
		this->setOpacity(255.f);
		this->setScale(m_data.scale);
	}

	for (auto l : m_bossHpListenerArray)
	{
		l->hideBossHp();
	}
}