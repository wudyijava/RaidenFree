#include "Boss.h"
#include "PlaneLayer.h"
#include "GlobalData.h"
#include "Track.h"
#include "Sound.h"
#include "Bullet.h"
#include "GlobalData.h"
#include "Difficulty.h"
#include "GameData.h"
#include "AircraftPool.h"
#include "Common/HpBar.h"
#include "Common/Macro.h"
#include "Common/Shake.h"

Boss::Boss()
{
	reset();
}

void Boss::reset()
{
	Aircraft::reset();

	m_time = 0;

	m_iCurActionSeq = 0;
	m_iActionGroupCnt = 0;
	m_iActionCnt = 0;

	m_pTrackData = nullptr;
	m_pPlaneLayer = nullptr;
	m_bReady = false;
}

bool Boss::init(Node* parent, const TAircraftData* data, const TAircraftLevelData* pLevelData)
{
	if (!Node::init())
	{
		return false;
	}

	m_eAircraftType = EAircraftType::Type_Enemy;

	const TBossData* pBoss = GlobalData::getInstance()->getBossData(data->id);
	if (!pBoss)
	{
		char sz[100];
		snprintf(sz, 99, "Can't find boss data[%d]", data->id);
		CCASSERT(pBoss, sz);
	}
	m_boss.clone(*pBoss);

	const TAircraftData* pAircraft = GlobalData::getInstance()->getEnemyData(m_boss.aircraftId);
	if (!pAircraft)
	{
		char sz[100];
		snprintf(sz, 99, "Can't find aircraft data[%d]", m_boss.aircraftId);
		CCASSERT(pAircraft, sz);
	}
	m_data.clone(*pAircraft);
	m_data.setLevelData(pLevelData);
	if (m_data.scale <= 0.001)
	{
		m_data.scale = 1;
	}

	//�Ѷ����������ӳ�
	float hpChange = GlobalData::getInstance()->getDifficulty()->hp;
	float endlessHpAdd = GameData::getInstance()->getValueToFloat(GAMEDATA::ENDLESS_ENEMY_HP_RATIO);
	m_data.maxHp = (int)((m_data.maxHp * (1 + hpChange) + endlessHpAdd) + 0.5);
	m_data.hp = m_data.maxHp;

	//�Ѷ����ӹ����ӳ�
	float bulletAttackAdjust = GlobalData::getInstance()->getDifficulty()->attack;
	Aircraft::setAttackAdjust(bulletAttackAdjust);

	//�Ѷ������ӵ��ٶȼӳ�
	float bulletSpeedAdjust = GlobalData::getInstance()->getDifficulty()->bullet_speed;
	Aircraft::setBulletSpeedAdjust(bulletSpeedAdjust);

	m_pPlaneLayer = dynamic_cast<PlaneLayer*>(parent);

	//Sprite����Ҫ��һ��ͼƬ��ʼ��һ�£����������쳣����
	this->initSpriteWithFileList(m_data.styleArray, m_data.aniDura);
	this->setNoDie(true);

	if (m_data.bodySizeW > 1 && m_data.bodySizeH > 1)
	{
		Vec2 center(m_data.bodyCenterX, m_data.bodyCenterY);
		setBodyCenter(center);

		Size size(m_data.bodySizeW, m_data.bodySizeH);
		setBodySize(size * m_data.scale);
	}

	//��������
	m_pArmature = cocostudio::Armature::create(data->armatureName);
	m_pArmature->setScale(m_data.scale);

	parent->addChild(this, CONSTANT::ZORDER_BOSS);
	this->addChild(m_pArmature, CONSTANT::ZORDER_BOSS);

	m_pArmature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Boss::movementDone, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return true;
}

void Boss::fly(const TTrackData* pData, FiniteTimeAction* flyDoneCallback)
{
	m_pTrackData = pData;

	Vec2 start(pData->startPosX, pData->startPosY);
	this->setPosition(start);
	m_lastPos = start;

	if (!m_bFlyDone) //boss��û�з�������
	{
		makeFlyTrack(pData, &m_actionArray);
		m_actionArray.pushBack(CallFuncN::create(getFlyDoneFunc()));
		if (flyDoneCallback)
		{
			m_actionArray.pushBack(flyDoneCallback);
		}
		Sequence* sequence = Sequence::create(m_actionArray);
		runAction(sequence);
	}
	else //boss�Ѿ���������
	{
		ready();
		runFlyTrack(pData);
	}
}

//void Boss::flyWithTrack()
//{
//	Vec2 src = getPosition();
//
//	if (!m_bFlyDone) //boss��û�з�������
//	{
//		makeFlyTrack(pData, &m_actionArray);
//		m_actionArray.pushBack(CallFuncN::create(getFlyDoneFunc()));
//		if (flyDoneCallback)
//		{
//			m_actionArray.pushBack(flyDoneCallback);
//		}
//		Sequence* sequence = Sequence::create(m_actionArray);
//		runAction(sequence);
//	}
//	else //boss�Ѿ���������
//	{
//		ready();
//		runFlyTrack(pData);
//	}
//}

std::function<void(Node*)> Boss::getFlyDoneFunc()
{
	return CC_CALLBACK_0(Boss::flyDone, this);
}

void Boss::ready()
{
	this->setNoDie(false);
	PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
	if (pLayer != NULL)
	{
		pLayer->bossReady();
	}
	m_bReady = true;
}

void Boss::unready()
{
	this->setNoDie(true);
	PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
	if (pLayer != NULL)
	{
		pLayer->bossUnready();
	}
	m_bReady = false;
}

void Boss::flyDone()
{
	m_bFlyDone = true;

	//if (m_pTrack->loop)
	//{
	//	flyWithTrack();
	//}
	//else
	//{
	//	Vec2 pos(getPosition());
	//	if (pos.x <= 0 || pos.x >= CONSTANT::DESIGN_RES_WIDTH
	//		|| pos.y <= 0 || pos.y >= CONSTANT::DESIGN_RES_HEIGHT)
	//	{
	//		destory();

	//		//֪ͨ��Layerɾ���������
	//		Node* p = getParent();
	//		PlaneLayer* layer = (PlaneLayer*)(getParent());
	//		CCASSERT(layer, "layer is not PlaneLayer");

	//		layer->destoryPlane(this);
	//	}
	//}
}

bool Boss::isCollsion(const Rect& rect, Rect* pCollsionRect)
{
	if (getBodySize().width > 1 && getBodySize().height > 1)
	{
		bool bCollsion = this->getBodyBox().intersectsRect(rect);
		if (pCollsionRect && bCollsion)
		{
			*pCollsionRect = this->getBodyBox();
		}
		return bCollsion;
	}

	Rect resultRect(99999,99999,99999,99999);
	bool bCollsion = false;

	cocos2d::Map<std::string, Bone*>::const_iterator it = m_pArmature->getBoneDic().begin();
	for (; it != m_pArmature->getBoneDic().end(); it++)
	{
		Bone* bone = it->second;
		ColliderDetector* pDetector = bone->getColliderDetector();
		if (!pDetector)
		{
			continue;
		}
		const cocos2d::Vector<ColliderBody*>& array = pDetector->getColliderBodyList();

		//���ڰ�����Σ����ݰ���ĸ���Ҫ��ֳɶ��͹�����
		for (int i = 0; i < array.size(); i++)
		{
			float minx = 0, miny = 0, maxx = 0, maxy = 0;

			const std::vector<cocos2d::Vec2> &data = array.at(i)->getCalculatedVertexList();
			for (unsigned j = 0; j < data.size(); j++)
			{
				Vec2 pos = this->convertToWorldSpace(data.at(j));

				if (0 == j)
				{
					minx = maxx = pos.x;
					miny = maxy = pos.y;
				}
				else
				{
					minx = (pos.x < minx ? pos.x : minx);
					miny = (pos.y < miny ? pos.y : miny);
					maxx = (pos.x > maxx ? pos.x : maxx);
					maxy = (pos.y > maxy ? pos.y : maxy);
				}
			}

			if (!(maxx < rect.getMinX() || minx > rect.getMaxX()
				|| maxy < rect.getMinY() || miny > rect.getMaxY()))
			{
				if (pCollsionRect && miny < resultRect.getMinY())
				{
					resultRect.setRect(minx, miny, maxx - minx, maxy - miny);
				}
				bCollsion = true;
				//return true;
			}
		}

	}

	if (pCollsionRect && bCollsion)
	{
		*pCollsionRect = resultRect;
	}

	return bCollsion;
}

void Boss::attack()
{
	CCASSERT(m_boss.hpActionArray.size() > 0, "Boss::attack : m_boss.hpActionArray.size() == 0");
	CCASSERT(m_boss.hpActionArray.front().hpThreshold == 100, 
		"Boss::attack : m_boss.hpActionArray.front().hpThreshold != 100");

	m_pCurBossHpActionData = m_boss.hpActionArray.begin();
	m_iCurActionSeq = 0;
	m_iActionGroupCnt = 0;
	m_iActionCnt = 0;

	doAction(m_boss.hpActionArray.front().actionArray.at(m_iCurActionSeq));
}

void Boss::bulletUseUp()
{
	//�������ӵ����þ�ʱ���л�
	for (BulletGroup* bulletGroup : m_bulletGroupArray)
	{
		if (!bulletGroup->isUseUp())
		{
			return;
		}
	}

	if (isMaxLevelBullet())
	{
		this->resetBulletLevel();
	}
	else
	{
		upgradeBullet();
	}
}

void Boss::doAction(TBossActionGroupData& actionGroup)
{
	if (actionGroup.delay > 0.00001)
	{
		DelayTime* delay = DelayTime::create(actionGroup.delay);

		auto delayEnd = [&actionGroup, this]()
		{
			//DEBUG_LOG("%s start, repeat = %d", actionGroup.actionName.c_str(), m_iActionCnt);

			m_pArmature->getAnimation()->play(actionGroup.actionName);
			m_iActionCnt++;
		};

		FiniteTimeAction* end = CallFunc::create(delayEnd);
		Sequence* sequence = Sequence::create(delay, end, NULL);
		runAction(sequence);
	}
	else
	{
		//DEBUG_LOG("%s start, repeat = %d", actionGroup.actionName.c_str(), m_iActionCnt);

		m_pArmature->getAnimation()->play(actionGroup.actionName, -1);
		m_iActionCnt++;
	}

	if (actionGroup.noDie)
	{
		this->setNoDie(true);
		m_pPlaneLayer->bossUnready();
	}
	else
	{
		if (m_bReady)
		{
			this->setNoDie(false);
			m_pPlaneLayer->bossReady();
		}
	}

	//׼����λ֮��ſ�ʼ���
	if (m_bReady)
	{
		doShoot(actionGroup);
	}

	m_preAction = actionGroup.actionName;
}

void Boss::doShoot(TBossActionGroupData& actionGroup)
{
	//ֹͣ�ӵ����������ӵ�
	//1��ǿ�Ƹ����ӵ�
	//2��ִ�в�ͬ�Ķ����������ӵ�
	//3��������ͬ��֮ǰû�����
	if (EBulletFlag::FLAG_FORCE_STOP == actionGroup.bulletFlag
		|| (EBulletFlag::FLAG_STOP == actionGroup.bulletFlag && actionGroup.actionName != m_preAction) 
		|| (EBulletFlag::FLAG_STOP == actionGroup.bulletFlag && actionGroup.actionName == m_preAction 
			&& !this->isShooting())
		)
	{
		//ֹͣ���
		Aircraft::stopShoot();

		//����ӵ��б�
		for (unsigned i = 0; i < m_data.bulletIdArray.size(); i++)
		{
			delete m_data.bulletIdArray[i];
		}
		m_data.bulletIdArray.clear();

		//�����ӵ�
		auto array = GlobalData::getInstance()->getBulletGradeData(actionGroup.bulletGradeId);
		if (array != NULL)
		{
			for (auto a : *array)
			{
				auto xx = new std::vector<int>;
				for (auto i : *a)
				{
					xx->push_back(i);
				}
				m_data.bulletIdArray.push_back(xx);
			}
		}

		//��ʼ���ӵ�
		Aircraft::initBullet(getParent(), &m_data);

		//��ʼ���
		if (m_data.bulletIdArray.size() > 0)
		{
			this->startShoot();
		}
	}
	//����֮ǰ���ӵ�
	else if (EBulletFlag::FLAG_NO_STOP == actionGroup.bulletFlag
		|| (EBulletFlag::FLAG_STOP == actionGroup.bulletFlag && actionGroup.actionName == m_preAction))
	{


	}
	else
	{
		CCASSERT(NULL, "Wrong bullet flag");
	}
}

void Boss::movementDone(cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (MovementEventType::START == movementType)
	{
		return;
	}

	//DEBUG_LOG("%s completed, type = %d", movementID.c_str(), movementType);

	if (m_bReady)
	{
		this->setNoDie(false);
	}
	else
	{
		this->setNoDie(true);
	}

	//�Ż��
	while (m_boss.burningArray.begin() != m_boss.burningArray.end())
	{
		const TBossBurningData& data = m_boss.burningArray.front();
		if (m_data.hp <= data.hpThreshold * m_data.maxHp / 100.f)
		{
			//�ȱ�ը
			Sound::playSound(SOUND::ID_BOOM_S);

			PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
			if (pLayer)
			{
				pLayer->addBlast(this->getLocalZOrder(), m_pArmature->getContentSize() * m_data.scale / 2, getPosition() + Vec2(data.x, data.y),
					m_data.blastStyleArray, m_data.blastAniDura);
			}

			//�����
			ParticleSystemQuad* pEmitter = ParticleSystemQuad::create("res/Blast/boss_fire.plist");
			pEmitter->setPosition(data.x, data.y);    // ���÷������ӵ�λ��  
			this->addChild(pEmitter, getLocalZOrder() + 1);
			m_fireArray.push_back(pEmitter);

			m_boss.burningArray.pop_front();
		}
		else
		{
			break;
		}
	}

	//�ж�HP���ޡ��ѽ�����һ��HP����
	if (m_boss.hpActionArray.size() > 1)
	{
		std::list<TBossHpActionData>::iterator it = m_boss.hpActionArray.begin();
		it++;

		if (m_data.hp <= it->hpThreshold * m_data.maxHp / 100.f)
		{
			m_boss.hpActionArray.pop_front();

			m_iCurActionSeq = 0;;
			m_iActionGroupCnt = 0;
			m_iActionCnt = 0;

			doAction(m_boss.hpActionArray.front().actionArray.at(m_iCurActionSeq));
			return;
		}
	}

	//��������ѭ�����
	if (m_boss.hpActionArray.front().actionArray.at(m_iCurActionSeq).repeat <= 0
		|| m_iActionCnt >= m_boss.hpActionArray.front().actionArray.at(m_iCurActionSeq).repeat)
	{
		//��һ������
		m_iCurActionSeq++;
		m_iActionCnt = 0;

		//�������еĶ�����ȫ����������ͷ��ʼִ��
		if (m_iCurActionSeq >= (int)m_boss.hpActionArray.front().actionArray.size())
		{
			m_iCurActionSeq = 0;
			m_iActionGroupCnt++;

			//��ǰHP���޶�������ִ�н���
			if (m_boss.hpActionArray.front().repeat >= 0
				&& m_iActionGroupCnt >= m_boss.hpActionArray.front().repeat)
			{
				m_boss.hpActionArray.pop_front();

				m_iCurActionSeq = 0;;
				m_iActionGroupCnt = 0;
				m_iActionCnt = 0;

			}
		}
	}
	doAction(m_boss.hpActionArray.front().actionArray.at(m_iCurActionSeq));
}

void Boss::blast()
{
	//Sound::playSound(m_data.blastMusicId, true);

	stopShoot();
	stopAllActions();
	m_pArmature->stopAllActions();
	m_pArmature->getAnimation()->stop();

	//�ڸ��������ɱ�ը����
	PlaneLayer* pLayer = dynamic_cast<PlaneLayer*>(getParent());
	if (NULL == pLayer)
	{
		return;
	}

	//��ը�����
	Sprite* pLight = Sprite::createWithSpriteFrameName("blast1.png");
	pLight->setPosition(this->getPosition());
	pLayer->addChild(pLight, getLocalZOrder() + 1 + 1);

	float fScaleStart = 0.2f;
	float fScaleEnd = 3.f;

	float fOpacityStart = 255.f;
	float fOpacityEnd = 10.f;

	pLight->setScale(fScaleStart);
	pLight->setOpacity(fOpacityStart);
	FiniteTimeAction* pScaleTo1 = ScaleTo::create(5.f, fScaleEnd, fScaleEnd);
	//FiniteTimeAction* pScaleTo2 = ScaleTo::create(EXPLOSION_TIME / 1.5f, fScaleStart, fScaleStart);
	FadeTo* pFade1 = FadeTo::create(5.f, fOpacityEnd);
	Spawn* pSpawn = Spawn::create(pScaleTo1, pFade1, NULL);
	FiniteTimeAction* pEnd = CallFunc::create([pLight](){
		pLight->removeFromParent();
	});
	Sequence* pSeq = Sequence::create(pSpawn, pEnd, nullptr);
	pLight->runAction(pSeq);


	//������ը
	auto b1 = [pLayer, this]()
	{
		Sound::playSound(SOUND::ID_BOOM_S);
		pLayer->addBlast(this->getLocalZOrder(), m_pArmature->getContentSize() * m_data.scale / 2 / 3, getPosition() + Vec2(50, 50),
			m_data.blastStyleArray, m_data.blastAniDura);
	};
	FiniteTimeAction* blast1 = CallFunc::create(b1);

	auto b2 = [pLayer, this]()
	{
		Sound::playSound(SOUND::ID_BOOM_S);
		pLayer->addBlast(this->getLocalZOrder(), m_pArmature->getContentSize() * m_data.scale / 2 / 2, getPosition() + Vec2(-70, 0),
			m_data.blastStyleArray, m_data.blastAniDura);
	};
	FiniteTimeAction* blast2 = CallFunc::create(b2);

	auto b3 = [pLayer, this]()
	{
		Sound::playSound(SOUND::ID_BOOM_S);
		pLayer->addBlast(this->getLocalZOrder(), m_pArmature->getContentSize() / 3, getPosition() + Vec2(30, 20),
			m_data.blastStyleArray, m_data.blastAniDura);
	};
	FiniteTimeAction* blast3 = CallFunc::create(b3);

	//����
	FiniteTimeAction* whiteScreen = CallFunc::create([pLayer, this]()
	{
		Layer* pWhiteLayer = LayerColor::create(Color4B(255, 255, 255, 200));
		pLayer->addChild(pWhiteLayer, 60000);
		//����
		pLayer->runAction(Shake::create(1.f, 7));
		Sound::playSound(SOUND::ID_BOOM_L);

		pLayer->runAction(Utils::delayToDo(.15f, [pWhiteLayer]()
		{
			pWhiteLayer->removeFromParent();
		}));

	});

	//����
	auto end = [pLayer, this]()
	{
		//this->destory();
		//this->recycle();
		AircraftPool::getInstance()->recycle(this);
		pLayer->destoryPlane(this);

		//Sound::stopEffect(m_data.blastMusicId);
	};
	FiniteTimeAction* endFunc = CallFunc::create(end);

	Vector<FiniteTimeAction*> array;
	array.pushBack(blast1);
	array.pushBack(DelayTime::create(0.5f));  // 0.5
	array.pushBack(blast2);
	array.pushBack(DelayTime::create(0.5f));  // 0.5
	array.pushBack(blast3);
	array.pushBack(DelayTime::create(0.5f));  // 0.5
	array.pushBack(blast2);
	array.pushBack(DelayTime::create(0.5f));  // 0.5
	array.pushBack(blast1);
	array.pushBack(whiteScreen);
	array.pushBack(endFunc);

	Sequence* sequence = Sequence::create(array);
	this->runAction(sequence);

	//��ת
	RotateBy* pRotate = RotateBy::create(4.0f, -20.0f);
	this->runAction(pRotate);

	//��С
	ScaleTo* pScale = ScaleTo::create(4.0f, 0.7f);
	this->runAction(pScale);

	//��͸��
	FadeTo* pFade = FadeTo::create(4.0f, 80);
	m_pArmature->runAction(pFade);
	//this->runAction(pFade);

}

void Boss::recycle()
{
	EnemyPlane::recycle();

	for (auto p : m_fireArray)
	{
		if (p)
		{
			p->removeFromParent();
		}
	}
	m_fireArray.clear();
}