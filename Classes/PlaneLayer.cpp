#include "GlobalData.h"
#include "PlaneLayer.h"
#include "HubLayer.h"
#include "MenuSettle.h"
#include "Level.h"
#include "Bullet.h"
#include "Laser.h"
#include "SuperLaser.h"
#include "Boss.h"
#include "Enemy.h"
#include "SeniorEnemy.h"
#include "Wingman.h"
#include "Player.h"
#include "Reward.h"
#include "Killer.h"
#include "IListener.h"
#include "DebugLayer.h"
#include "GameData.h"
#include "Track.h"
#include "GiftLayer.h"
#include "GiftDuoLayer.h"
#include "Blast.h"
#include "Sound.h"
#include "Guide.h"
#include "MenuMainUI.h"
#include "Achievement.h"
#include "Analytics.h"
#include "GameScene.h"
#include "CountDown.h"
#include "Goods.h"
#include "Difficulty.h"
#include "Common/Shake.h"
#include "BackgroundLayer.h"
#include "Armature.h"
#include "Dialog.h"
#include "ModeBase.h"
#include "AircraftPool.h"
#include "AimPool.h"
#include "Common/Macro.h"

class SpeedSlowPromptLayer : public LayerColor
{
public:
	CREATE_FUNC(SpeedSlowPromptLayer);

	SpeedSlowPromptLayer() :m_pListener(nullptr){}

	~SpeedSlowPromptLayer()
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pListener);
	}

	bool init()
	{
		if (!LayerColor::init())
		{
			return false;
		}

		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->setSwallowTouches(true);
		m_pListener->onTouchBegan = CC_CALLBACK_2(SpeedSlowPromptLayer::onTouchBegan, this);
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

		setColor(Color3B(0, 0, 0));
		Size s = getContentSize();
		setContentSize(getContentSize()*1.2f);
		setPosition(-getContentSize().width*0.1f, -getContentSize().height*0.1f);

		setOpacity(0);
		FadeTo* pFadeTo = FadeTo::create(0.2f, 160);
		FiniteTimeAction* pFunc = CallFunc::create([](){
			Director::getInstance()->getScheduler()->setTimeScale(0.05f);
			Sound::setVolume(0.3f);
		});
		Sequence* pSeq = Sequence::create(pFadeTo, pFunc, nullptr);
		runAction(pSeq);

		return true;
	}

	virtual bool onTouchBegan(Touch* touch, Event* event) override
	{
		this->removeFromParent();
		return false;
	}

private: 
	EventListenerTouchOneByOne* m_pListener;
};

std::list<ECommand> PlaneLayer::m_commandList;

PlaneLayer::PlaneLayer()
{
	m_pLevelData = NULL;

	m_playerArray.clear();
	m_hpListenerArray.clear();

	m_bTouch = false;

	m_bShoot = false;
	m_bSlow = true;
	m_fTimeScale = 1.0f;
	m_fVolume = 1.0f;

	m_eventSpwanList.clear();
	m_time = 0;
	m_bBossStatus = EBossStatus::BossNo;
	m_bFirstEnemy = true;
	m_enemyArray.clear();

	m_rewardArray.clear();

	m_iEnemy = 0;

	m_pHubLayer = nullptr;

	//m_pWarningNode = NULL;
	//m_pWarnAction = NULL;

	//m_pWinNode = NULL;
	//m_pWinAction = NULL;

	//m_pReadyGoNode = NULL;
	//m_pReadyGoAction = NULL;

	m_bPause = false;
	m_bBulletShieldFlag = false;
	m_bPlayerReady = false;

	m_toDeleteAircraft.clear();
	m_toDeleteBullet.clear();
}

PlaneLayer::~PlaneLayer()
{
	AimPool::getInstance()->clear();

	GameLayer::setPlaneLayer(nullptr);
	GameLayer::setHubLayer(nullptr);

	AircraftPool::getInstance()->release();
	BlastPool::getInstance()->clear(this);

	m_bTouch = false;
	m_bSlow = false;
	for (int i = 0; i < m_playerArray.size(); i++)
	{
		m_playerArray.at(i)->stopShoot();
	}
	m_playerArray.clear();

	m_eventSpwanList.clear();
	m_time = 0;
	m_enemyArray.clear();

	m_rewardArray.clear();

	//m_pWarningNode->release();
	//m_pWarnAction->release();

	//m_pWinNode->release();
	//m_pWinAction->release();


	//m_pReadyGoNode->release();
	//m_pReadyGoAction->release();

	m_bBulletShieldFlag = false;
	m_bPlayerReady = false;

	m_toDeleteAircraft.clear();
	m_toDeleteBullet.clear();
}

void PlaneLayer::setHubLayer(HubLayer* layer)
{
	m_pHubLayer = layer;
}

bool PlaneLayer::init()
{
	bool bRet=false;
	do 
	{
		m_bPlayerReady = false;
		this->clearCommand();

		CC_BREAK_IF(!Layer::init());

		//关卡数据
		int level = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
		m_pLevelData = GlobalData::getInstance()->getLevelData(level);
		CCASSERT(m_pLevelData, "PlaneLayer::init : m_pLevelData is NULL");

		//背景
		m_backgroundLayer = BackgroundLayer::create();
		addChild(m_backgroundLayer);
		m_backgroundLayer->run();

		//CC_BREAK_IF(!spwanPlayer());

		//setOtherSidePlane();

		initGameData();

		//加入调试层
		this->m_debugLayer = DebugLayer::create();
		this->addChild(m_debugLayer);

		scheduleUpdateWithPriority(INT_MAX);

		//TODO preload 要放到启动前的update中运行
		preload();

		Analytics::getInstance()->startLevel(m_pLevelData->name.c_str());

		//设置可触摸
		this->registerTouchDispatcher();

		bRet=true;
	} while (0);
	
	spwanEvent();

	return bRet;
}

bool PlaneLayer::preload()
{
	//预先生成多个爆炸
	for (int i = 0; i < 5; i++)
	{
		BlastPool::getInstance()->getOneBlast(this);
	}

	return true;
}

void PlaneLayer::pushCommand(ECommand c)
{
	for (auto cmd : m_commandList)
	{
		if (c == cmd)
		{
			return;
		}
	}

	m_commandList.push_back(c);
}

ECommand PlaneLayer::getCommand()
{
	if (m_commandList.size() <= 0)
	{
		return ECommand::CommandNone;
	}
	else
	{
		ECommand c = m_commandList.front();
		return c;
	}
}

void PlaneLayer::removeCommand(ECommand c)
{
	m_commandList.remove(c);
}

void PlaneLayer::clearCommand()
{
	m_commandList.clear();
}

void PlaneLayer::execCommand()
{
	ECommand cmd = getCommand();
	removeCommand(cmd);
	int life = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_LIFE, 0);

	//子弹晶化
	if (ECommand::CommandShield == cmd)
	{
		int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD);
		if (cnt > 0)
		{
			this->bulletShield();
			cnt--;
			GameData::getInstance()->setValue(GAMEDATA::SHIELD, cnt);
		}
	}
	//呼叫僚机群
	else if (ECommand::CommandKiller == cmd)
	{
		int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::KILLER);
		if (cnt > 0)
		{
			//this->spwanKiller();
			m_pHubLayer->menuKillerCallback(nullptr);
			cnt--;
			GameData::getInstance()->setValue(GAMEDATA::KILLER, cnt);
		}
	}
	//呼叫无敌核武
	else if (ECommand::CommandSuperKiller == cmd)
	{
		int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::KILLER);
		if (cnt > 0)
		{
			//this->spwanSuperKiller();
			m_pHubLayer->menuKillerCallback(nullptr);
			cnt--;
			GameData::getInstance()->setValue(GAMEDATA::KILLER, cnt);
		}
	}
	//生成玩家
	else if (ECommand::CommandSpwanPlayer == cmd)
	{
		if (this->getPlayer() == NULL && life > 0)
		{
			this->spwanPlayer(nullptr);
		}
	}
	else if (ECommand::CommandBackToMainMenu == cmd)
	{
		//Director::getInstance()->replaceScene(MenuMainUI::scene());
		Analytics::getInstance()->failLevel(m_pLevelData->name.c_str());
		pause();
		gotoSettleScene(false);
	}
	else if (ECommand::CommandRestartGame == cmd)
	{
		Director::getInstance()->replaceScene(GameScene::create());
	}
	else if (ECommand::CommandNone == cmd)
	{
		//do nothing
	}
	else
	{
		DEBUG_LOG("Unknown command : %d", cmd);
	}

	//全部死光，生命也没了，退出游戏
	if (this->getPlayer() == NULL && life <= 0)
	{
		runAction(Utils::delayToDo(1.0f, [this](){
			pause();
			gotoSettleScene(false);
		}));
		return;
	}

	int shieldCnt = GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD);
	if (shieldCnt > 0 && m_playerArray.size() > 0 && m_playerArray.at(0))
	{
		m_playerArray.at(0)->addShield();
	}
}

void PlaneLayer::onEnter()
{
	Layer::onEnter();

	this->resume();
}

void PlaneLayer::onExit()
{
	Layer::onExit();
}

void PlaneLayer::pause()
{
	this->saveTimeScale();
	this->speedNormal();

	m_bTouch = false;
	m_bSlow = false;
	m_backgroundLayer->pause();
	//Sound::pause();  //这里不pause，而在出现的新的layer或者scene中根据需要pause
	
	BaseLayer::pause();
	m_pHubLayer->pause();
}

void PlaneLayer::resume()
{
	this->recoverTimeScale();
	this->execCommand();

	m_backgroundLayer->resume();
	
	Sound::resume();
	BaseLayer::resume();
	m_pHubLayer->resume();

	m_bTouch = true;
	m_bSlow = true;
}

PlayerPlane* PlaneLayer::getPlayer()
{ 
	if (m_playerArray.size() > 0)
	{
		return dynamic_cast<PlayerPlane*>(m_playerArray.at(0));
	}
	else
	{
		return NULL;
	}
}

void PlaneLayer::destoryPlane(Aircraft* aircraft)
{
	m_toDeleteAircraft.push_back(aircraft);
}

void PlaneLayer::doDestoryPlane(Aircraft* aircraft)
{
	removeChild(aircraft);

	if (aircraft->getAircraftType() == Type_Player)
	{
		m_playerArray.eraseObject((Aircraft*)aircraft);
		m_bTouch = false;
		m_bSlow = false;
	}
	else if (aircraft->getAircraftType() == Type_Enemy)
	{
		//通知玩家飞机的跟踪导弹
		for (BulletGroup* pGroup : m_playerBulletArray)
		{
			pGroup->nodifyTargetRemove((Aircraft*)aircraft);
		}

		m_enemyArray.eraseObject((Aircraft*)aircraft);
		GameData::getInstance()->incValue(GAMEDATA::LEVEL_ENEMY_LEFT, -1);
	}
	else if (aircraft->getAircraftType() == Type_Killer)
	{
		m_killerArray.eraseObject((Aircraft*)aircraft);
	}
}

void PlaneLayer::registerTouchDispatcher()
{
	//控制飞机飞行
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	//创建一个排队的触控事件监听器 ( 同时仅仅处理一个触控事件 )
	auto listener = EventListenerTouchOneByOne::create();

	// 当 "swallow touches" 设置为 true, 然后，在 onTouchBegan 方法发返回 'true' 将会吃掉触控事件, 防止其他监听器使用这个事件.
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PlaneLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PlaneLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PlaneLayer::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool PlaneLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	speedNormal();

	return true;
}

void PlaneLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (m_playerArray.size() <= 0)
	{
		return;
	}

	PlayerPlane* player = dynamic_cast<PlayerPlane*>(m_playerArray.at(0));
	if (m_bTouch && player != NULL && player->isAlive())
	{
		//当前触摸点
		Vec2 beginPoint = pTouch->getLocation();

		//起始触摸点
		Vec2 endPoint = pTouch->getPreviousLocation();

		//移动飞机
		Vec2 offSet = beginPoint - endPoint;
		player->jumpBy(offSet, true);
	}
}

void PlaneLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (m_playerArray.size() <= 0)
	{
		return;
	}

	PlayerPlane* player = dynamic_cast<PlayerPlane*>(m_playerArray.at(0));
	player->jumpBy(Vec2::ZERO, true);
	
	speedSlow();
}

void PlaneLayer::speedSlow()
{
	if (m_bSlow
		&& GameData::getInstance()->getValueToInt(GAMEDATA::MODE) != ModeBase::ModeRapid)
	{
		this->addChild(SpeedSlowPromptLayer::create(), CONSTANT::ZORDER_DEBUG);
	}
}

void PlaneLayer::speedNormal()
{
	Director::getInstance()->getScheduler()->setTimeScale(1.0f);
	Sound::setVolume(1.0f);
}

void PlaneLayer::saveTimeScale()
{
	m_fTimeScale = Director::getInstance()->getScheduler()->getTimeScale();
	m_fVolume = Sound::getVolume();
}

void PlaneLayer::recoverTimeScale()
{
	Director::getInstance()->getScheduler()->setTimeScale(m_fTimeScale);
	Sound::setVolume(m_fVolume);
}


void PlaneLayer::setOtherSidePlane()
{
	for (int i = 0; i < m_playerArray.size(); i++)
	{
		m_playerArray.at(i)->setOtherSidePlane(&m_enemyArray);
	}

	for (int j = 0; j < m_enemyArray.size(); j++)
	{
		m_enemyArray.at(j)->setOtherSidePlane(&m_playerArray);
	}
}

bool PlaneLayer::spwanEvent()
{
	//生成敌机产生队列
	float time = 0;
	const std::vector<TLevelEventData*>* enemyDataArray = m_pLevelData->enemy;

	m_iEnemyTotal = 0;
	for (unsigned i = 0; i < enemyDataArray->size(); i++)
	{
		TLevelEventData* data = enemyDataArray->at(i);
		if (data->event.length() == 0)
		{
			data->event = PlaneLayerConstant::EventEnemy;
		}

		//only_boss测试标识
		if (GlobalData::getInstance()->isOnlyBoss() == 1)
		{
			auto p = GlobalData::getInstance()->getEnemyData(enemyDataArray->at(i)->object_id);
			if (p && strcmp(p->type.c_str(), AircraftType::BOSS))
			{
				continue;
			}
		}

		//起始关卡、起始敌机测试标识
		if ((data->event == PlaneLayerConstant::EventEnemy || data->event == PlaneLayerConstant::EventEnemyLevel)
			&&data->seq < GlobalData::getInstance()->getTestEnemySeq()
			&& GlobalData::getInstance()->getTestLevelId() == GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL))
		{
			continue;
		}

		//重复出现的敌机
		if (data->pause == 1 && m_eventSpwanList.size() > 0)
		{
			time = m_eventSpwanList.back().time + data->time;
		}
		else
		{
			time += data->time;
		}

		int repeat = data->repeat;
		if (data->event != PlaneLayerConstant::EventEnemy)
		{
			repeat = 1;
		}

		for (int j = 0; j < repeat; j++)
		{
			TLevelEventData d(*data);

			//时间
			d.time = time + d.interval * j;

			//对于重复出现的敌机，只有最后一架才允许爆奖励品
			if (data->repeat > 1 && j != data->repeat - 1)
			{
				d.rewardArray.clear();
			}

			//只有第一架才能设置pause
			if (j > 0)
			{
				d.pause = 0;
			}

			m_eventSpwanList.push_back(d);

			if (!strcmp(d.event.c_str(), PlaneLayerConstant::EventEnemy)
				|| !strcmp(d.event.c_str(), PlaneLayerConstant::EventEnemyLevel)
				|| !strcmp(d.event.c_str(), PlaneLayerConstant::EventBossFlyShoot))
			{
				++m_iEnemyTotal;
			}
		}
	}
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_ENEMY_TOTAL, m_iEnemyTotal);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_ENEMY_LEFT, m_iEnemyTotal);

	m_eventSpwanList.sort();

	return true;
}

bool PlaneLayer::spwanReward(PlayerPlane* pPlayer)
{
	if (NULL == pPlayer)
	{
		return false;
	}

	int grade = pPlayer->getBulletLevel();
	if (grade <= 0 || grade > 10)
	{
		return true;
	}

	const Size& planeSize = pPlayer->getBodySize();
	const Vec2& postion = pPlayer->getPosition();

	//武器升级
	for (int i = 0; i < grade; i++)
	{
		Reward* pReward = RewardCreator::create(this, &m_playerArray, REWARD_TYPE_WEAPON);
		this->addChild(pReward, CONSTANT::ZORDER_REWARD);
		pReward->setPosition(postion);
		pReward->fly(planeSize);
	}

	//晶化
	{
		Reward* pReward = RewardCreator::create(this, &m_playerArray, REWARD_TYPE_SHIELD);
		this->addChild(pReward, CONSTANT::ZORDER_REWARD);
		pReward->setPosition(postion);
		pReward->fly(planeSize);
	}

	return true;
}

bool PlaneLayer::spwanReward(EnemyPlane* pEnemy)
{
	if (NULL == pEnemy)
	{
		return false;
	}

	const Size& planeSize = pEnemy->getBodySize();

	const Vec2& postion = pEnemy->getPosition();

	//统计分数
	GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE, pEnemy->getScore());

	m_iEnemy++;
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_ENEMY, m_iEnemy);

	//爆金币
	int money = pEnemy->getMoney();
	//缩小十倍
	int ratio = 10;
	for (int i = 0; i < money / ratio; i++)
	{
		Reward* pReward = RewardCreator::create(this, &m_playerArray, REWARD_TYPE_MONEY);
		this->addChild(pReward, CONSTANT::ZORDER_REWARD);
		pReward->setPosition(postion);
		((RewardMoney*)pReward)->setMoney(ratio);
		pReward->fly(planeSize);
	}

	int tmp = money % ratio;
	if (tmp > 0)
	{
		Reward* pReward = RewardCreator::create(this, &m_playerArray, REWARD_TYPE_MONEY);
		this->addChild(pReward, CONSTANT::ZORDER_REWARD);
		pReward->setPosition(postion);
		((RewardMoney*)pReward)->setMoney(tmp);
		pReward->fly(planeSize);
	}

	//爆固定奖励品
	float rewardProb = GlobalData::getInstance()->getDifficulty()->reward_prob;      //难度等级
	const std::vector<TRewardGroupData*>& rewardGroup = pEnemy->getRewardGroup();
	for (unsigned i = 0; i < rewardGroup.size(); i++)
	{
		const TRewardGroupData* data = rewardGroup.at(i);
		if (data->probability <= 0.0001)
		{
			continue;
		}

		//如果重复打已通过的关卡，则不爆必杀或护盾
		int level = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
		int finishLevel = GameData::getInstance()->getLevelFinish();
		const TRewardData* pData = GlobalData::getInstance()->getRewardData(data->rewardId);
		if ((pData->type == REWARD_TYPE_KILLER || pData->type == REWARD_TYPE_SHIELD)
			&& level <= finishLevel)
		{
			continue;
		}

		for (int j = 0; j < data->cnt; j++)
		{
			float random = Utils::getRandom();
			if (random <= data->probability * (1 - rewardProb))
			{
				Reward* pReward = RewardCreator::create(this, &m_playerArray, data->rewardId);
				this->addChild(pReward, CONSTANT::ZORDER_REWARD);
				pReward->setPosition(postion);
				pReward->fly(planeSize);

				//break;
			}
		}
	}

	//1秒后出现奖励品新手引导
	if (money > 0)
	{
		this->runAction(Utils::delayToDo(1.0f, [this](){
			Guide* pGuide = GuideMgr::getInstance()->show(GuideType::GameEquipment, this->getParent(), nullptr, [this](){
				this->resume();
			});

			if (pGuide)//如果Guide不满足条件，则pGuide为nullptr，则不需要pause
			{
				this->pause();
			}
		}));
	}

	return true;
}

bool PlaneLayer::spwanSuperKiller()
{
	int wingmanId = GlobalData::getInstance()->getSuperKillerId();
	const TAircraftData* pData = GlobalData::getInstance()->getWingmanData(wingmanId);
	CCASSERT(pData, "PlaneLayer::spwanSuperKiller : pData is NULL");

	auto killer = SuperKiller::create(this, pData);
	killer->fly();

	int cnt = GameData::getInstance()->incValue(GAMEDATA::LEVEL_KILLER, 1);
	if (cnt == 3)
	{
		AchievementMgr::getInstance()->judgeAchievement(this, GAMEDATA::LEVEL_KILLER);
	}
	return true;
}

void PlaneLayer::playerWin(float dt)
{
	Analytics::getInstance()->finishLevel(m_pLevelData->name.c_str());

	if (getPlayer())
	{
		getPlayer()->stopShoot();
	}

	//this->unscheduleUpdate();

	//已通过关卡
	int currentLevel = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
	int finishLevel = GameData::getInstance()->getLevelFinish();
	if (currentLevel > finishLevel)
	{
		GameData::getInstance()->setLevelFinish(currentLevel);
	}

	//设置最大关卡
	int maxLevel = GameData::getInstance()->getLevelMax();
	if (currentLevel == maxLevel)
	{
		++maxLevel;
	}
	++currentLevel;
	GameData::getInstance()->setLevelMax(maxLevel);

	//杀敌比例
	int enemy = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_ENEMY);
	if (enemy == m_iEnemyTotal)
	{
		GameData::getInstance()->setValue(GAMEDATA::MAX_KILL_ENEMY_RATIO, 1);
	}

	//任意一关中获取到所有奖励品
	int iReward = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_REWARD);
	int iRewardTotal = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_REWARD_TOTAL);
	if (iRewardTotal == iReward)
	{
		GameData::getInstance()->setValue(GAMEDATA::MAX_GET_REWARD_RATIO, 1);
	}

	auto flyEnd = [this]()
	{
		//战斗胜利动画
		auto pArmatureVirctory = cocostudio::Armature::create(PlaneLayerConstant::ArmatureVictory);
		addChild(pArmatureVirctory);
		pArmatureVirctory->setPosition(getContentSize().width / 2, getContentSize().height / 3. * 2);
		pArmatureVirctory->getAnimation()->play(GlobalData::getInstance()->getArmatureData(PlaneLayerConstant::ArmatureVictory)->defaultAction);
		pArmatureVirctory->getAnimation()->setMovementEventCallFunc(
			[this](cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID)
		{
			if (MovementEventType::START == movementType)
			{
				return;
			}

			armature->removeFromParent();

			DelayTime* delay = DelayTime::create(1.0f);

			auto levelOver = [this]()
			{
				Size visibleSize = Director::getInstance()->getVisibleSize();
				RenderTexture* texture = RenderTexture::create(visibleSize.width, visibleSize.height);

				//截图。  
				texture->begin();
				Director::getInstance()->getRunningScene()->visit();
				texture->end();

				pause();
				gotoSettleScene(true);
			};
			FiniteTimeAction* delayEnd = CallFunc::create(levelOver);

			Sequence* sequence = Sequence::create(delay, delayEnd, NULL);
			this->runAction(sequence);
		}
		);
	};

	m_bTouch = false;
	m_bSlow = false;

	//飞机飞走
	Sound::playSound(SOUND::ID_FLY_AWAY);

	Vec2 pos = getPlayer()->getPosition();
	float orignY = pos.y;
	const Size& size = getPlayer()->getContentSize();
	pos.y = CONSTANT::DESIGN_RES_HEIGHT + 300;
	float speed = 800.0f;
	float dura = (pos.y - orignY) / speed;
	MoveTo* fly = MoveTo::create(dura, pos);

	FiniteTimeAction* end = CallFunc::create(flyEnd);
	Sequence* sequence = Sequence::create(fly, end, NULL);
	getPlayer()->runAction(sequence);
}

void PlaneLayer::update(float dt)
{

	GameLayer::setPlaneLayer(this);
	GameLayer::setHubLayer(m_pHubLayer);

	m_time += dt;

	AimPool::getInstance()->update(dt);

	while (m_toDeleteAircraft.size() > 0)
	{
		//因为这里只是回收飞机，所以为避免doDestoryPlane销毁飞机，先retain一下
		auto p = m_toDeleteAircraft.front();
		doDestoryPlane(p);
		if (!m_toDeleteAircraft.empty())
		{
			m_toDeleteAircraft.pop_front();
		}

		//战斗胜利
		if (m_enemyArray.size() <= 0 && m_eventSpwanList.size() <= 0 && m_bPlayerReady
			&& getPlayer() && getPlayer()->isAlive())
		{
			//TODO
			m_bSlow = false;
			this->scheduleOnce(CC_SCHEDULE_SELECTOR(PlaneLayer::playerWin), 2.0f);
			//return;
		}

	}
	while (m_toDeleteBullet.size() > 0)
	{
		doRemoveBullet(m_toDeleteBullet.front());
		if (!m_toDeleteBullet.empty())
		{
			m_toDeleteBullet.pop_front();
		}
	}

	//产生敌机
	procEventSpwanList(dt);

	//碰撞检测
	Vector<Aircraft*> toDeleteAircraftArray;
	//飞机
	PlayerPlane* pPlane = (PlayerPlane*)this->getPlayer();
	//敌机群
	Vector<Aircraft*>* enemyArray = this->getEnemyArray();
	//僚机群
	Vector<Aircraft*>* killerArray = &m_killerArray;

	//双方子弹
	Vector<BulletGroup*>* pPlaneBulletArray = &m_playerBulletArray;
	Vector<BulletGroup*>* pEnemyBulletArray = &m_enemyBulletArray;

	//僚机群
	for (Aircraft* pKiller : *killerArray)
	{
		//僚机与敌机
		for (Aircraft* pEnemy : *enemyArray)
		{
			if (!pEnemy->isNoDie() && pEnemy->isAlive() && pEnemy->isCollsion(pKiller->getBodyBox()))
			{
				pKiller->reduceHp(pEnemy->getAttack());
				pEnemy->reduceHp(pKiller->getAttack());

				if (!pEnemy->isAlive())
				{
					toDeleteAircraftArray.pushBack(pEnemy);
				}

				if (!pKiller->isAlive())
				{
					toDeleteAircraftArray.pushBack(pKiller);
				}
			}
		}

		//僚机与敌机子弹
		for (BulletGroup* pBulletGroup : *pEnemyBulletArray)
		{
			for (int j = 0; j < pBulletGroup->getAllObject()->count(); j++)
			{
				Bullet* pBullet = (Bullet*)pBulletGroup->getAllObject()->getObjectAtIndex(j);
				if ( pBullet->isVisible() && pKiller->isCollsion(pBullet->getBodyBox()) )
				{
					pKiller->reduceHp(pBulletGroup->getAttack());
					pBulletGroup->blast(pBullet);
					break;
				}
			}

		}

	}

	//飞机与敌机
	if (pPlane != NULL && pPlane->isAlive() && !pPlane->isNoDie())
	{
		for (int i = 0; i < enemyArray->size(); i++)
		{
			Aircraft* pEnemy = enemyArray->at(i);
			if (pEnemy->isAlive() && pEnemy->isCollsion(pPlane->getBodyBox()))
			{
				pPlane->reduceHp(pEnemy->getAttack());
				pEnemy->reduceHp(pPlane->getAttack());

				GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE_TOTAL, pEnemy->getAttack());

				if (!pEnemy->isAlive())
				{
					toDeleteAircraftArray.pushBack(pEnemy);
				}
			}
		}

	}

	//超级激光与敌机子弹
	for (int i = 0; i < pPlaneBulletArray->size(); i++)
	{
		BulletGroup* pPlayerBulletGroup = pPlaneBulletArray->at(i);
		__Array* allBullet = pPlayerBulletGroup->getAllObject();
		if (allBullet->count() > 0)
		{
			SuperLaser* pLaser = dynamic_cast<SuperLaser*>(allBullet->getObjectAtIndex(0));
			if (pLaser)
			{
				for (int j = 0; j < pEnemyBulletArray->size(); j++)
				{
					BulletGroup* pBulletGroup = pEnemyBulletArray->at(j);
					for (int k = 0; k < pBulletGroup->getAllObject()->count(); k++)
					{
						Bullet* pBullet = (Bullet*)pBulletGroup->getAllObject()->getObjectAtIndex(k);
						if (pBullet->isVisible())
						{
							pBulletGroup->blast(pBullet);
						}
					}
				}

				break;
			}
		}
	}

	//飞机与敌机子弹
	if (pPlane != NULL && pPlane->isAlive() && !pPlane->isNoDie())
	{
		for (int i = 0; i < pEnemyBulletArray->size(); i++)
		{
			BulletGroup* pBulletGroup = pEnemyBulletArray->at(i);
			for (int j = 0; j < pBulletGroup->getAllObject()->count(); j++)
			{
				Bullet* pBullet = (Bullet*)pBulletGroup->getAllObject()->getObjectAtIndex(j);
				if (pBullet->isVisible() && pPlane->isCollsion(pBullet->getBodyBox()))
				{
					int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD);
					if (cnt > 0)
					{
						cnt--;
						GameData::getInstance()->setValue(GAMEDATA::SHIELD, cnt);

						this->bulletShield();

						Shake* shake = Shake::create(0.1f, 5);
						this->runAction(shake);
					}
					else
					{
						pPlane->reduceHp(pBulletGroup->getAttack());

						GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE_TOTAL, pBulletGroup->getAttack());
					}
					pBulletGroup->blast(pBullet);

					if (0 == cnt)
					{
						pPlane->removeShield();
					}
					break;
				}
			}
		}
	}

	//敌机与玩家（包括玩家飞机、僚机、僚机群）子弹
	bool bLaserRejustHeight = false;
	Laser* pLaser = nullptr;
	int iEnemyCnt = enemyArray->size();
	for (int i = 0; i < pPlaneBulletArray->size(); i++)
	{
		BulletGroup* pBulletGroup = pPlaneBulletArray->at(i);
		__Array* allBullet = pBulletGroup->getAllObject();
		for (int j = 0; j < allBullet->count(); j++)
		{
			Bullet* pBullet = dynamic_cast<Bullet*>(allBullet->getObjectAtIndex(j));
			if (pBullet == NULL || !pBullet->isVisible())
			{
				continue;
			}

			pLaser = dynamic_cast<Laser*>(pBullet);
			Rect resultRect(99999, 99999, 99999, 99999);
			bool bCollsion = false;

			int k = 0;
			Aircraft* pTargetEnemy = nullptr;
			for (; k < enemyArray->size(); k++)
			{
				auto pEnemy = enemyArray->at(k);

				Rect r;
				if (!pEnemy->isNoDie() && pEnemy->isAlive() && pEnemy->isCollsion(pBullet->getBodyBox(), &r))
				{
					bCollsion = true;
					pBulletGroup->blast(pBullet);

					if (pLaser) //如果是激光，需找到碰撞到最低点
					{
						if (r.getMinY() < resultRect.getMinY()) //只针对激光
						{
							resultRect = r;
							pTargetEnemy = pEnemy;
						}
					}
					else
					{
						pEnemy->reduceHp(pBulletGroup->getAttack());
						if (!pEnemy->isAlive())
						{
							toDeleteAircraftArray.pushBack(pEnemy);
							iEnemyCnt--;
						}
					}

				}
			}

			if (bCollsion && pLaser && pTargetEnemy)
			{
				pTargetEnemy->reduceHp(pBulletGroup->getAttack() * Director::getInstance()->getScheduler()->getTimeScale());
				if (!pTargetEnemy->isAlive())
				{
					toDeleteAircraftArray.pushBack(pTargetEnemy);
					iEnemyCnt--;

					pLaser->rejustHeight(CONSTANT::DESIGN_RES_HEIGHT + 200);
					bLaserRejustHeight = true;

				}
				else
				{
					pLaser->rejustHeight(resultRect.getMinY());
					bLaserRejustHeight = true;
				}
			}
			else if (pLaser)
			{
				pLaser->rejustHeight(CONSTANT::DESIGN_RES_HEIGHT + 200);
				bLaserRejustHeight = true;
			}

			if (iEnemyCnt <= 0)
			{
				break;
			}
		}

		if (iEnemyCnt <= 0)
		{
			break;
		}
	}

	if (pLaser && !bLaserRejustHeight)
	{
		pLaser->rejustHeight(CONSTANT::DESIGN_RES_HEIGHT);
	}

	//收尾，清除敌机
	for (int i = 0; i < toDeleteAircraftArray.size(); i++)
	{
		//飞出奖励物
		this->spwanReward(dynamic_cast<EnemyPlane*>(toDeleteAircraftArray.at(i)));

		Aircraft* pEnemy = toDeleteAircraftArray.at(i);
		if (dynamic_cast<Boss*>(pEnemy))
		{
			GameData::getInstance()->setValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0);
			GameData::getInstance()->incValue(GAMEDATA::ENDLESS_BOSS, 1);
		}
		else if (dynamic_cast<EnemyPlane*>(pEnemy))
		{
			GameData::getInstance()->incValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 1);
		}
			
		//爆炸
		pEnemy->blast();

	}

	//收尾，清除飞机
	if (pPlane && !pPlane->isAlive())
	{
		AimPool::getInstance()->clear();

		this->spwanReward(pPlane);
		
		pPlane->blast();

		int life = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_LIFE);
		--life;
		GameData::getInstance()->setValue(GAMEDATA::LEVEL_LIFE, life);
		GameData::getInstance()->setValue(GAMEDATA::DIE, 1);
		GameData::getInstance()->setValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0);
		AchievementMgr::getInstance()->judgeAchievement(this, GAMEDATA::DIE);
		if (life <= 0)
		{
			//延迟两秒钟
			DelayTime* delay = DelayTime::create(1.5f);
			auto end = [this]()
			{
				m_backgroundLayer->constant(true);

				this->saveTimeScale();
				this->speedNormal();

				doAllPlayerKilled();
			};

			FiniteTimeAction* endFunc = CallFunc::create(end);
			Sequence* sequence = Sequence::create(delay, endFunc, NULL);
			runAction(sequence);
		}
		else
		{
			this->spwanPlayer(nullptr);
		}
	}

	if (GlobalData::getInstance()->isDisplayBody())
	{
		//绘制调试信息
		m_debugLayer->begin();
		
		m_debugLayer->drawAll(this);

		m_debugLayer->end();
	}
}

bool PlaneLayer::spwanPlayer(const std::function<void()>& listener)
{
	//创建飞机
	int id = GameData::getInstance()->getValueToInt(GAMEDATA::PLAYER);
	const TAircraftData* pData = GlobalData::getInstance()->getPlayerData(id);
	CCASSERT(pData, "PlaneLayer::spwanPlayer : pData is NULL");

	PlayerPlane* player = PlaneCreator<PlayerPlane>::create(this, pData);
	m_playerArray.pushBack(player);

	//初始护盾个数增加
	int shieldCnt = GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD);
	int shieldCntAdd = (int)GameData::getInstance()->getValueToFloat(GAMEDATA::REINFORCE_VALUE_SHIELD_INIT);
	shieldCnt = GameData::getInstance()->incValue(GAMEDATA::SHIELD, shieldCntAdd);
	if (shieldCnt > 0)
	{
		player->addShield();
	}

	setOtherSidePlane();

	//飞机闪烁，1秒，3次
	player->setNoDie(true);
	Blink *blink = Blink::create(1.5, 4);
	FiniteTimeAction*funcBlinkDone = CallFunc::create([this]()
	{
		getPlayer()->setNoDie(false);
		if (GameData::getInstance()->getValueToInt(GAMEDATA::BUY_RELIFE))
		{
			getPlayer()->upgradeMaxBullet();
			GameData::getInstance()->setValue(GAMEDATA::BUY_RELIFE, 0);
		}
	});

	Sequence* seq1 = nullptr;
	if (listener)
	{
		seq1 = Sequence::create(blink, funcBlinkDone, CallFunc::create(listener), nullptr);
	}
	else
	{
		seq1 = Sequence::create(blink, funcBlinkDone, nullptr);
	}

	//从屏幕下方起飞
	m_bTouch = false;
	m_bSlow = false;
	Vec2 dest = Vec2(Director::getInstance()->getWinSize().width / 2, 130);
	FiniteTimeAction* fly = CCMoveTo::create(0.5, dest);
	FiniteTimeAction* funcFlyDone = CallFunc::create([this]()
	{
		//显示了ReadyGo之后才能开始游戏
		if (m_bPlayerReady)
		{
			m_bTouch = true;
			m_bSlow = true;
			m_backgroundLayer->constant(false);
			if (m_bShoot)
			{
				getPlayer()->startShoot();
			}
		}
	});
	Sequence* seq2 = Sequence::create(fly, funcFlyDone, nullptr);

	player->runAction(Spawn::create(seq1, seq2, nullptr));

	return true;
}

void PlaneLayer::spwanOneEvent(std::list<TLevelEventData>::iterator curEvent)
{
	DEBUG_LOG("[%d]Event: %s, object_id = %d%s", 
		curEvent->seq,
		curEvent->event.c_str(),
		curEvent->object_id,
		curEvent->pause ? ", pause = 1" : "");

	//产生敌机
	if (curEvent->event == PlaneLayerConstant::EventEnemy
		|| curEvent->event == PlaneLayerConstant::EventEnemyLevel
		|| curEvent->event == PlaneLayerConstant::EventBossDown)
	{
		spwanOneEnemy(curEvent);
	}
	//不产生新敌机，而是现在的指定敌机开始按轨迹飞行，并开始射击
	else if (curEvent->event == PlaneLayerConstant::EventBossFlyShoot)
	{
		//TODO  1、需修改轨迹循环的方法（把起点也要算在内？）2、找到指定飞机，开始飞行并射击
		for (auto p : m_bossArray)
		{
			Boss* pBoss = dynamic_cast<Boss*>(p);
			if (p /*&& p->getLevelId() == curEvent->object_id*/)
			{
				const TTrackData* pTrackData = GlobalData::getInstance()->getTrackData(curEvent->track);
				pBoss->fly(pTrackData);
				pBoss->attack();

				m_enemyArray.pushBack(pBoss);
				m_bossArray.eraseObject(pBoss);
			}
		}
	}
	//开始射击
	else if (curEvent->event == PlaneLayerConstant::EventStartShoot)
	{
		m_bShoot = true;
		if (getPlayer())
		{
			getPlayer()->startShoot();
		}
	}
	//停止射击
	else if (curEvent->event == PlaneLayerConstant::EventStopShoot)
	{
		m_bShoot = false;
		if (getPlayer())
		{
			getPlayer()->stopShoot();
		}
	}
	//等待
	else if (curEvent->event == PlaneLayerConstant::EventWait)
	{
		m_eventArray.push_back(PlaneLayerConstant::EventWait);
		auto pDelay = Utils::delayToDo(curEvent->time, [this](){
			m_eventArray.pop_front();
		});
		runAction(pDelay);
	}
	//切换背景
	else if (curEvent->event == PlaneLayerConstant::EventBackground)
	{
		m_backgroundLayer->change();
	}
	//切换音乐
	else if (curEvent->event == PlaneLayerConstant::EventMusic)
	{
		if (curEvent->repeat < 0)
		{
			Sound::playSound(curEvent->object_id, true);
		}
		else
		{
			Sound::playSound(curEvent->object_id, false);
		}
	}
	//对话
	else if (curEvent->event == PlaneLayerConstant::EventDialog)
	{
		//只有在简单模式下才有对话
		if (GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL) != Difficulty::Easy)
		{
			return;
		}

		m_eventArray.push_back(PlaneLayerConstant::EventDialog);

		m_bTouch = false;
		m_bSlow = false;
		m_backgroundLayer->constant(true);

		for (int i = 0; i < m_enemyArray.size(); i++)
		{
			m_enemyArray.at(i)->stopShoot();
		}

		auto nextEvent = curEvent;
		nextEvent++;
		bool nextDialog = true;
		if (nextEvent == m_eventSpwanList.end() || nextEvent->event != PlaneLayerConstant::EventDialog)
		{
			nextDialog = false;
		}

		auto pLayer = Dialog::show(m_pHubLayer, GlobalData::getInstance()->getDialog(curEvent->object_id), nextDialog);
		pLayer->setOnExitCallback([nextDialog, this](){

			if (!nextDialog)
			{
				m_bTouch = true;
				m_bSlow = true;
				m_backgroundLayer->constant(false);

				for (int i = 0; i < m_enemyArray.size(); i++)
				{
					m_enemyArray.at(i)->startShoot();
				}
			}

			if (!m_eventArray.empty())
			{
				m_eventArray.pop_front();
			}
		});
	}
	//Ready...Go！
	else if (curEvent->event == PlaneLayerConstant::EventReadygo)
	{
		m_eventArray.push_back(PlaneLayerConstant::EventReadygo);
		
		Sound::playSound(SOUND::ID_READY_GO);

		auto pArmatureReadyGo = cocostudio::Armature::create(PlaneLayerConstant::ArmatureReadyGo);
		addChild(pArmatureReadyGo);
		pArmatureReadyGo->setPosition(getContentSize() / 2);
		pArmatureReadyGo->getAnimation()->play(GlobalData::getInstance()->getArmatureData(PlaneLayerConstant::ArmatureReadyGo)->defaultAction);
		pArmatureReadyGo->getAnimation()->setMovementEventCallFunc(
			[this](cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID)
		{
			if (MovementEventType::START == movementType)
			{
				return;
			}

			armature->removeFromParent();

			if (!m_eventArray.empty())
			{
				m_eventArray.pop_front();
			}

			removeChild(armature);

			getPlayer()->setNoDie(false);
			//getPlayer()->startShoot();
			m_bTouch = true;
			m_bSlow = true;

			m_bPlayerReady = true;
		}
		);
	}
	//前方高能警告
	else if (curEvent->event == PlaneLayerConstant::EventWarning)
	{
		GameData::getInstance()->setValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0);

		m_eventArray.push_back(PlaneLayerConstant::EventWarning);

		//DelayTime* pDelay = DelayTime::create(1.5f);
		//FiniteTimeAction* pFunc = CallFunc::create(CC_CALLBACK_0(PlaneLayer::playWarning, this));
		runAction( Utils::delayToDo(1.5f, CC_CALLBACK_0(PlaneLayer::playWarning, this)) );
	}
	//瓦片地图
	else if (curEvent->event == PlaneLayerConstant::EventTile)
	{

	}
	//玩家飞机飞出
	else if (curEvent->event == PlaneLayerConstant::EventPlayerUp)
	{
		m_eventArray.push_back(PlaneLayerConstant::EventPlayerUp);
		spwanPlayer([this](){
			if (!m_eventArray.empty())
			{
				m_eventArray.pop_front();
			}
		});
	}
}

void PlaneLayer::playWarning()
{
	Sound::playSound(SOUND::ID_WARNING);

	if (getPlayer() && getPlayer()->isAlive())
	{
		getPlayer()->stopShoot();
	}

	auto pArmatureWarning = cocostudio::Armature::create(PlaneLayerConstant::ArmatureWarning);
	addChild(pArmatureWarning, CONSTANT::ZORDER_WARNING);
	pArmatureWarning->setPosition(getContentSize().width / 2, getContentSize().height / 3. * 2);
	pArmatureWarning->getAnimation()->play(GlobalData::getInstance()->getArmatureData(PlaneLayerConstant::ArmatureWarning)->defaultAction);
	pArmatureWarning->getAnimation()->setMovementEventCallFunc(
		[this](cocostudio::Armature *armature, MovementEventType movementType, const std::string& movementID)
	{
		if (MovementEventType::START == movementType)
		{
			return;
		}

		armature->removeFromParent();
		//getPlayer()->startShoot();
		if (!m_eventArray.empty())
		{
			m_eventArray.pop_front();
		}

		playWarningDone();
	}
	);
}

void PlaneLayer::playWarningDone()
{

}

void PlaneLayer::spwanOneEnemy(std::list<TLevelEventData>::iterator curEnemy)
{
	const TTrackData* pTrackData = GlobalData::getInstance()->getTrackData(curEnemy->track);
	if (NULL == pTrackData)
	{
		DEBUG_LOG("Can't find track data[%d]", curEnemy->track);
		CCASSERT(NULL, "PlaneLayer::spwanEnemy : Can't find track data");
	}

	EnemyPlane* pEnemy = nullptr;

	const TAircraftLevelData* pEnemyLevelData = nullptr;
	const TAircraftData* pAircraftData = nullptr;

	pEnemyLevelData = GlobalData::getInstance()->getAircraftLevelData(curEnemy->object_id);

	//难度提升
	int gradeIncrease2 = GlobalData::getInstance()->getDifficulty()->enemy_grade;
	if (gradeIncrease2 > 0)
	{
		int newEnemyGrade = pEnemyLevelData->grade + gradeIncrease2;
		pEnemyLevelData = GlobalData::getInstance()->getAircraftLevelData(pEnemyLevelData->aircraftId, newEnemyGrade);
	}

	pAircraftData = GlobalData::getInstance()->getEnemyData(pEnemyLevelData->aircraftId);
	if (!pAircraftData)
	{
		DEBUG_LOG("Can't find aircraft data[%d]", curEnemy->object_id);
	}

	if (pAircraftData->type.length() > 0)
	{
		if (pAircraftData->type == AircraftType::ENEMY2)
		{
			pEnemy = AircraftPool::getInstance()->get<SeniorEnemy>(this, pAircraftData, pEnemyLevelData);
		}
		else if (pAircraftData->type == AircraftType::BOSS)
		{
			pEnemy = AircraftPool::getInstance()->get<Boss>(this, pAircraftData, pEnemyLevelData);
		}
	}

	if (!pEnemy)
	{
		pEnemy = AircraftPool::getInstance()->get<EnemyPlane>(this, pAircraftData, pEnemyLevelData);
	}

	if (curEnemy->event == PlaneLayerConstant::EventBossDown)
	{
		for (auto l : m_bossHpListenerArray)
		{
			pEnemy->regBossHpListener(l);
		}

		//EventBossDown 不用射击
		SeniorEnemy* p = dynamic_cast<SeniorEnemy*>(pEnemy);
		if (p)
		{
			auto callback = CallFunc::create([this](){
				DEBUG_LOG("Boss down finish");
				if (!m_eventArray.empty())
				{
					m_eventArray.pop_front();
				}

				for (auto l : m_bossHpListenerArray)
				{
					l->displayBossHp();
				}

			});

			p->fly(pTrackData, callback);
		}
		else
		{
			char buf[100 + 1] = { 0 };
			snprintf(buf, 100, "EventBossDown, but aircraft[%d,%s] is not SeniorEnemy", 
				pAircraftData->id, pAircraftData->type.c_str());
			CCASSERT(false, buf);
		}
	}
	else
	{
		pEnemy->fly(pTrackData); //飞行
		pEnemy->attack();    //射击
	}

	if (curEnemy->event == PlaneLayerConstant::EventBossDown)
	{
		m_bossArray.pushBack(pEnemy);
		m_eventArray.push_back(PlaneLayerConstant::EventBossDown);
	}
	else
	{
		m_enemyArray.pushBack(pEnemy);
	}

	pEnemy->setOtherSidePlane(&m_playerArray);

	pEnemy->setMoney(pEnemyLevelData->money);
	pEnemy->setScore(pEnemyLevelData->score);
	pEnemy->setDirectionFlag(curEnemy->direction_flag);
	pEnemy->setRewardGroupArray(curEnemy->rewardArray);

	GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE_TOTAL, pEnemyLevelData->score);

	if (pTrackData->pos.size() == 0)
	{
		m_staticObjectArray.push_back(pEnemy);
	}

	if (curEnemy->seq == 290 
		&& GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 1
		&& GameData::getInstance()->getValueToInt(GAMEDATA::SHIELD) > 0)
	{
		this->runAction(Utils::delayToDo(1.0f, [this](){
			m_pHubLayer->displayGuideShield();
		}));
	}


	////新手教程：必杀
	//if (levelEventData.seq == 54
	//	&& GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 1
	//	&& GameData::getInstance()->getValueToInt(GAMEDATA::KILLER) > 0
	//	&& !GameData::getInstance()->getValueToInt(GAMEDATA::TUTORIAL_KILLER)
	//	)
	//{
	//	DelayTime* delay = DelayTime::create(1.5f);
	//	auto delayEnd = [this]()
	//	{
	//		Scene* pScene = TutorialLayer::scene(Utils::captureScreen(), ETutorialType::TutorialKiller);
	//		Director::getInstance()->pushScene(pScene);
	//	};
	//	FiniteTimeAction* delayEndFunc = CallFunc::create(delayEnd);
	//	Sequence* seq = Sequence::create(delay, delayEndFunc, NULL);
	//	this->runAction(seq);
	//	GameData::getInstance()->setValue(GAMEDATA::TUTORIAL_KILLER, 1);
	//}

	if (curEnemy->seq == 500
		&& GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 1
		&& GameData::getInstance()->getValueToInt(GAMEDATA::KILLER) > 0)
	{
		this->runAction(Utils::delayToDo(1.0f, [this](){
			m_pHubLayer->displayGuideKiller();
		}));
	}

}

void PlaneLayer::bossReady()
{
	m_bBossStatus = EBossStatus::BossReady;

	if (getPlayer() && !getPlayer()->isShooting())
	{
		getPlayer()->startShoot();
	}
}

void PlaneLayer::bossUnready()
{
	m_bBossStatus = EBossStatus::BossUnready;

	if (getPlayer())
	{
		getPlayer()->stopShoot();
	}
}

void PlaneLayer::addBullet(BulletGroup* pBulletGroup)
{
	Aircraft* pAircraft = pBulletGroup->getPlane();
	if (dynamic_cast<PlayerPlane*>(pAircraft)
		|| dynamic_cast<SuperKiller*>(pAircraft)
		|| dynamic_cast<Wingman*>(pAircraft))
	{
		m_playerBulletArray.pushBack(pBulletGroup);
	}
	else if (dynamic_cast<EnemyPlane*>(pAircraft)
		|| dynamic_cast<Boss*>(pAircraft))
	{
		m_enemyBulletArray.pushBack(pBulletGroup);
	}
	else
	{
		CCASSERT(NULL, "PlaneLayer::addBullet : unknown type pBulletGroup");
	}

	//这里必须要addChild，否则就不会加入到schedule中
	//addChild(pBulletGroup);
}

void PlaneLayer::removeBullet(BulletGroup* pBulletGroup)
{
	if (pBulletGroup != NULL)
	{
		pBulletGroup->stopShoot();
	}

	m_toDeleteBullet.push_back(pBulletGroup);
}

void PlaneLayer::doRemoveBullet(BulletGroup* pBulletGroup)
{
	if (m_bBulletShieldFlag)
	{
		//如果正在子弹晶化，则等待两秒再删除
		//这个机制很恶心，当PlaneLayer::bulletShield的循环过程中，也会插入调用本函数，倒是循环过程中访问到野指针
		DelayTime* delay = DelayTime::create(2.0f);
		auto delayEnd = [pBulletGroup, this]()
		{
			this->removeBullet(pBulletGroup);
		};
		FiniteTimeAction* end = CallFunc::create(delayEnd);
		Sequence* sequence = Sequence::create(delay, end, NULL);
		runAction(sequence);
	}
	else
	{
		m_playerBulletArray.eraseObject(pBulletGroup);
		m_enemyBulletArray.eraseObject(pBulletGroup);
	}

}

void PlaneLayer::addBlast(int zorder, const Size& size, const Vec2& postion, const std::vector<std::string>& filenames, float aniDura)
{
	static float xx = sqrt(64*64+64*64);
	float ratio = sqrt(size.width * size.width + size.height * size.height)/xx;
	BlastNode* b = BlastPool::getInstance()->getOneBlast(this);
	if (b)
	{
		b->run(postion, ratio, zorder);
	}

	//if (filenames.size() <= 0)
	//{
	//	return;
	//}

	//Sprite* pSprite = Sprite::createWithSpriteFrameName(filenames.at(0));
	//CCASSERT(pSprite, "PlaneLayer::addBlast : pSprite is NULL");
	//addChild(pSprite, CONSTANT::ZORDER_BULLET_BLAST);

	////位置
	//pSprite->setPosition(postion);

	////开始动画
	//Animation* animation = Animation::create();
	//animation->setDelayPerUnit(aniDura);
	//for (unsigned i = 0; i < filenames.size(); i++)
	//{
	//	SpriteFrame* pFrame = CCSpriteFrameCache::getInstance()->getSpriteFrameByName(filenames[i]);
	//	if (NULL == pFrame)
	//	{
	//		continue;
	//	}
	//	animation->addSpriteFrame(pFrame);
	//}

	////设置重复
	//Animate* animate = Animate::create(animation);
	//Repeat* repeat = Repeat::create(animate, 1);
	//FiniteTimeAction* actionDone = CallFuncN::create(CC_CALLBACK_0(PlaneLayer::removeBlast, this, pSprite));

	//Sequence* sequence = Sequence::create(repeat, actionDone, NULL);
	//pSprite->runAction(sequence);
}

void PlaneLayer::addBlast(int zorder, const Vec2& postion, const std::vector<std::string>& filenames, float aniDura)
{
	if (filenames.size() <= 0)
	{
		return;
	}

	Sprite* pSprite = Sprite::createWithSpriteFrameName(filenames.at(0));
	CCASSERT(pSprite, "PlaneLayer::addBlast : pSprite is NULL");
	addChild(pSprite, zorder);

	//位置
	pSprite->setPosition(postion);

	//开始动画
	Animation* animation = Animation::create();
	animation->setDelayPerUnit(aniDura);
	for (unsigned i = 0; i < filenames.size(); i++)
	{
		SpriteFrame* pFrame = CCSpriteFrameCache::getInstance()->getSpriteFrameByName(filenames[i]);
		if (NULL == pFrame)
		{
			continue;
		}
		animation->addSpriteFrame(pFrame);
	}

	//设置重复
	Animate* animate = Animate::create(animation);
	Repeat* repeat = Repeat::create(animate, 1);
	FiniteTimeAction* actionDone = CallFuncN::create(CC_CALLBACK_0(PlaneLayer::removeBlast, this, pSprite));

	Sequence* sequence = Sequence::create(repeat, actionDone, NULL);
	pSprite->runAction(sequence);
}

void PlaneLayer::removeBlast(Sprite* pSprite)
{
	removeChild(pSprite);
}

int PlaneLayer::bulletShield()
{
	//GameData::getInstance()->setValue(GAMEDATA::TUTORIAL_SHIELD, 1);

	m_bBulletShieldFlag = true;

	Sound::playSound(SOUND::ID_BULLET_SHIELD);

	int cnt = 0;
	for (BulletGroup* pBulletGroup : m_enemyBulletArray)
	{
		int count = pBulletGroup->getAllObject()->count();
		for (int j = 0; j < count; j++)
		{
			Bullet* pBullet = (Bullet*)pBulletGroup->getAllObject()->getObjectAtIndex(j);
			if (pBullet->isVisible())
			{
				Vec2 pos = pBullet->getPosition();
				pBulletGroup->RemoveBullet(pBullet);

				Reward* pReward = RewardCreator::create(this, &m_playerArray, REWARD_TYPE_MONEY);
				this->addChild(pReward, CONSTANT::ZORDER_REWARD);
				pReward->setPosition(pos);
				((RewardMoney*)pReward)->setMoney(1);
				pReward->fly(Size(0, 0));

				cnt++;
			}
		}

	}

	m_bBulletShieldFlag = false;

	return cnt;
}
