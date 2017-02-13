#pragma once

#include "cocos2d.h"
#include "IListener.h"
#include "BaseLayer.h"

USING_NS_CC;

class HubLayer;
class PlayerPlane;
class EnemyPlane;
class Boss;
class Aircraft;
class BulletGroup;
class GameObject;
class Reward;
class KillerGroup;
class Killer;
class DebugLayer;
class BackgroundLayer;
struct TLevelEventData;
struct TTrackData;
struct TLevelData;

namespace PlaneLayerConstant
{
	const char* const ArmatureReadyGo = "UI_Ready";
	const char* const ArmatureVictory = "UI_Victory";
	const char* const ArmatureWarning = "UI_Warning";

	const char* const EventWait = "WAIT";
	const char* const EventEnemy = "ENEMY";
	const char* const EventEnemyLevel = "ENEMY_LEVEL";
	const char* const EventBackground = "BACKGROUND";
	const char* const EventDialog = "DIALOG";
	const char* const EventReadygo = "READYGO";
	const char* const EventWarning = "WARNING";
	const char* const EventTile = "TILE";
	const char* const EventMusic = "MUSIC";
	const char* const EventPlayerUp = "PLAYERUP";
	const char* const EventStartShoot = "START_SHOOT";
	const char* const EventStopShoot = "STOP_SHOOT";
	const char* const EventBossDown = "BOSS_DOWN";
	const char* const EventBossFlyShoot = "BOSS_FLY_SHOOT";
}

enum EBossStatus
{
	BossNo = 0,
	BossUnready = 1,
	BossReady = 2
};

enum ECommand
{
	CommandNone = -1,
	CommandSpwanPlayer,
	CommandKiller,
	CommandSuperKiller,
	CommandShield,
	CommandBackToMainMenu,
	CommandReadyGo,
	CommandRestartGame
};

class GameEvent
{
public:
	virtual void run() = 0;
};

class PlaneLayer : public BaseLayer
{
public:

	PlaneLayer();
	virtual ~PlaneLayer();

	virtual bool init();
	bool preload();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void registerTouchDispatcher();

	bool spwanPlayer(const std::function<void()>& listener);
	bool spwanEvent();
	bool spwanReward(EnemyPlane* pEnemy);
	bool spwanReward(PlayerPlane* pPlayer);

	//不同的游戏模式
	virtual void initGameData() = 0;                //初始化游戏数据
	virtual void procEventSpwanList(float dt) = 0;  //产生事件队列
	virtual void doAllPlayerKilled() = 0;           //当玩家飞机都死光时的处理方法
	virtual void gotoSettleScene(bool win) = 0;     //游戏结束时调用不同的过关界面
	virtual void cancelGiftCallback() = 0;          

	bool spwanSuperKiller();

	void spwanOneEvent(std::list<TLevelEventData>::iterator curEvent);
	void spwanOneEnemy(std::list<TLevelEventData>::iterator curEnemy);

	//void waitBossWarning(Boss* pBoss, const TTrackData* pTrackData);
	//void bossWarningDone(Boss* pBoss, const TTrackData* pTrackData);
	void playWarning();
	void playWarningDone();

	void bossReady();
	void bossUnready();

	Vector<Aircraft*>* getPlayerArray()	{ return &m_playerArray; }
	Vector<Aircraft*>* getEnemyArray()	{ return &m_enemyArray; }
	PlayerPlane* getPlayer();

	virtual void destoryPlane(Aircraft* aircraft);
	virtual void doDestoryPlane(Aircraft* aircraft);

	void addBullet(BulletGroup* pBullet);
	void removeBullet(BulletGroup* pBullet);
	void doRemoveBullet(BulletGroup* pBullet);

	void addBlast(int zorder, const Vec2& postion, const std::vector<std::string>& filenames, float aniDura);
	void addBlast(int zorder, const Size& size, const Vec2& postion, const std::vector<std::string>& filenames, float aniDura);
	void removeBlast(Sprite* pSprite);

	void playerWin(float dt);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void pause() override;
	virtual void resume() override;

	//命令
	static void pushCommand(ECommand c);
	static ECommand getCommand();
	static void clearCommand();
	static void removeCommand(ECommand c);
	void execCommand();

	//子弹晶化
	int bulletShield();

	//时间变化
	void speedSlow();  //减慢速度
	void speedNormal();//恢复速度

	void saveTimeScale();   //保存当前的速度
	void recoverTimeScale();//恢复到保存的速度

	//HP监听器
	inline void regHpListener(IHpListener* l)
	{
		m_hpListenerArray.push_back(l);
	}

	inline void notifyHp(int hp)
	{
		for (unsigned i = 0; i < m_hpListenerArray.size(); i++)
		{
			m_hpListenerArray.at(i)->updateHp(hp);
		}
	}

	inline void notifyMaxHp(int max)
	{
		for (unsigned i = 0; i < m_hpListenerArray.size(); i++)
		{
			m_hpListenerArray.at(i)->updateMaxHp(max);
		}
	}

	inline void regBossHpListener(IBossHpListener* l)
	{
		m_bossHpListenerArray.push_back(l);
	}

	virtual void setHubLayer(HubLayer* layer);

protected:
	void update(float dt);
	void setOtherSidePlane();

protected:
	BackgroundLayer* m_backgroundLayer;

	HubLayer* m_pHubLayer;

	//关卡
	const TLevelData* m_pLevelData;

	//玩家飞机
	Vector<Aircraft*> m_playerArray;
	Vector<BulletGroup*>  m_playerBulletArray;

	//屏幕HUB 血槽
	std::vector<IHpListener*>      m_hpListenerArray;
	std::vector<IBossHpListener*>  m_bossHpListenerArray;

	//事件列表
	std::list<const char*> m_eventArray;

	//敌机
	Vector<Aircraft*>     m_enemyArray;
	Vector<Aircraft*>     m_bossArray;
	Vector<BulletGroup*>  m_enemyBulletArray;

	//必杀：料机群
	Vector<Aircraft*>   m_killerArray;

	//相对背景静止的物体
	std::list<Aircraft*> m_staticObjectArray;

	//奖励物
	Vector<Reward*> m_rewardArray;

	bool m_bTouch;

	bool m_bShoot;
	bool m_bSlow;
	float m_fTimeScale;
	float m_fVolume;

	std::list<TLevelEventData> m_eventSpwanList;

	//游戏时间
	float m_time;
	//是否第一次出敌机
	bool m_bFirstEnemy;
	//boss是否在做准备工作：出现、变形等
	EBossStatus m_bBossStatus;

	//分数/宝石/敌机
	int m_iEnemy;
	int m_iEnemyTotal;

	//debug
	DebugLayer*  m_debugLayer;
	//是否正在子弹晶化
	bool m_bBulletShieldFlag;
	//玩家是否准备好
	bool m_bPlayerReady;

	//待删除飞机和子弹
	std::list<Aircraft*> m_toDeleteAircraft;
	std::list<BulletGroup*> m_toDeleteBullet;

	//命令列表
	static std::list<ECommand> m_commandList;
};
