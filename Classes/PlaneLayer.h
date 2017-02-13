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

	//��ͬ����Ϸģʽ
	virtual void initGameData() = 0;                //��ʼ����Ϸ����
	virtual void procEventSpwanList(float dt) = 0;  //�����¼�����
	virtual void doAllPlayerKilled() = 0;           //����ҷɻ�������ʱ�Ĵ�����
	virtual void gotoSettleScene(bool win) = 0;     //��Ϸ����ʱ���ò�ͬ�Ĺ��ؽ���
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

	//����
	static void pushCommand(ECommand c);
	static ECommand getCommand();
	static void clearCommand();
	static void removeCommand(ECommand c);
	void execCommand();

	//�ӵ�����
	int bulletShield();

	//ʱ��仯
	void speedSlow();  //�����ٶ�
	void speedNormal();//�ָ��ٶ�

	void saveTimeScale();   //���浱ǰ���ٶ�
	void recoverTimeScale();//�ָ���������ٶ�

	//HP������
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

	//�ؿ�
	const TLevelData* m_pLevelData;

	//��ҷɻ�
	Vector<Aircraft*> m_playerArray;
	Vector<BulletGroup*>  m_playerBulletArray;

	//��ĻHUB Ѫ��
	std::vector<IHpListener*>      m_hpListenerArray;
	std::vector<IBossHpListener*>  m_bossHpListenerArray;

	//�¼��б�
	std::list<const char*> m_eventArray;

	//�л�
	Vector<Aircraft*>     m_enemyArray;
	Vector<Aircraft*>     m_bossArray;
	Vector<BulletGroup*>  m_enemyBulletArray;

	//��ɱ���ϻ�Ⱥ
	Vector<Aircraft*>   m_killerArray;

	//��Ա�����ֹ������
	std::list<Aircraft*> m_staticObjectArray;

	//������
	Vector<Reward*> m_rewardArray;

	bool m_bTouch;

	bool m_bShoot;
	bool m_bSlow;
	float m_fTimeScale;
	float m_fVolume;

	std::list<TLevelEventData> m_eventSpwanList;

	//��Ϸʱ��
	float m_time;
	//�Ƿ��һ�γ��л�
	bool m_bFirstEnemy;
	//boss�Ƿ�����׼�����������֡����ε�
	EBossStatus m_bBossStatus;

	//����/��ʯ/�л�
	int m_iEnemy;
	int m_iEnemyTotal;

	//debug
	DebugLayer*  m_debugLayer;
	//�Ƿ������ӵ�����
	bool m_bBulletShieldFlag;
	//����Ƿ�׼����
	bool m_bPlayerReady;

	//��ɾ���ɻ����ӵ�
	std::list<Aircraft*> m_toDeleteAircraft;
	std::list<BulletGroup*> m_toDeleteBullet;

	//�����б�
	static std::list<ECommand> m_commandList;
};
