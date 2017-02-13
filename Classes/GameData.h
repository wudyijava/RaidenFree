#pragma once

#include "cocos2d.h"
#include "IListener.h"

USING_NS_CC;

namespace GAMEDATA
{
	//ȫ������
	const char* const SOUND = "sound";
	const char* const FINGER_PRINT_FLAG = "finger_print_flag";//apkǩ���Ƿ���Ч
	const char* const FINGER_PRINT = "finger_print";//apkǩ���Ƿ���Ч

	//Զ������
	const char* const USER_ID = "user_id";

	//�û���ǰѡ��Ĺؿ��ͷɻ�
	const char* const PLAYER = "player";
	const char* const MAX_KILL_ENEMY_RATIO = "max_kill_enemy_ratio";  // ɱ���������ܵĵл���
	const char* const MAX_GET_REWARD_RATIO = "max_get_reward_ratio";  // ��ȡ���Ľ���Ʒ��/�ܵĽ���Ʒ��
	const char* const CONTINUOUS_KILL_ENEMY_CNT = "CONTINUOUS_KILL_ENEMY_CNT"; //����ɱ����

	//��Ϸģʽ��1 �ؿ���2 �޾���3 ����
	const char* const MODE = "mode";

	const char* const LEVEL_STAR_1 = "level_star_1";  //�Ѷ�1�ؿ������� 0/1/2/3
	const char* const LEVEL_STAR_2 = "level_star_2";  //�Ѷ�2�ؿ������� 0/1/2/3
	const char* const LEVEL_STAR_3 = "level_star_3";  //�Ѷ�3�ؿ������� 0/1/2/3

	const char* const LEVEL = "level";               // ��ǰ������ڼ���
	const char* const LEVEL_MAX = "level_max";       // ����浽�ڼ���
	const char* const LEVEL_FINISH = "level_finish"; // ��ͨ���ڼ���

	const char* const RAPID_MODE_MAX_DISTANCE = "rapid_mode_max_distance";//����ģʽ�����о���

	const char* const DIFFICULTY_LEVEL = "difficulty_level";     //����ģʽ�Ѷȣ�1��2��3  ����->��
	const char* const DIFFICULTY_ENDLESS = "difficulty_endless"; //�޾�ģʽ����101��ʼ
	const char* const DIFFICULTY_RAPID = "difficulty_rapid";     //����ģʽ�Ѷȣ�201��ʼ

	const char* const ENDLESS_ENEMY_HP_RATIO = "endless_enemy_hp_ratio";   //�޾�ģʽ�л�hp�ӳ�

	//��ǰ�ؿ����ݣ�����־û�
	const char* const LEVEL_ENEMY  = "level_enemy"; //ɱ����
	const char* const LEVEL_ENEMY_LEFT = "level_enemy_left"; //ʣ�µĵл��� = �л����� - ����� - ���ܵ�
	const char* const LEVEL_SCORE  = "level_score"; //�÷�
	const char* const LEVEL_MONEY  = "level_money"; //��ʯ/Ǯ
	const char* const LEVEL_LIFE   = "level_life";  //�������
	const char* const LEVEL_REWARD = "level_reward";//��õĽ���Ʒ����
	const char* const LEVEL_KILLER = "level_killer";//�ͷű�ɱ�Ĵ���

	const char* const LEVEL_ENEMY_TOTAL = "level_enemy_total";  //�ؿ��ܵĵл���
	const char* const LEVEL_SCORE_TOTAL = "level_score_total";  //�ؿ��ܵķ���
	const char* const LEVEL_MONEY_TOTAL  = "level_money_total";  //�ؿ��ܵı�ʯ��
	const char* const LEVEL_REWARD_TOTAL = "level_reward_total"; //�ؿ��ܵĽ���Ʒ����

	//�޾�ģʽ
	const char* const ENDLESS_BOSS       = "endless_boss";      //���εл�����
	const char* const ENDLESS_BOSS_LAST  = "endless_boss_last"; //�ϴεл�����

	//����ģʽ
	const char* const RAPID_DISTANCE      = "rapid_distance";     //���ξ���
	const char* const RAPID_DISTANCE_BEST = "rapid_distance_best";//��ʷ��þ���
	const char* const RAPID_RANK          = "rapid_rank";	      //��������
	const char* const RAPID_RANK_BEST     = "rapid_rank_best";	  //��ʷ�������

	//�ؿ��ۼ�����
	const char* const ENEMY = "enemy"; //�ۼ�����ĵл�����
	const char* const SCORE = "score"; //�÷�
	const char* const MONEY = "money"; //��ʯ/Ǯ
	const char* const LIFE  = "life";  //�������

	//�����Ϊ��0��ʾû�д���Ϊ��1��ʾִ�й�����Ϊ
	const char* const DIE = "die";                        // ����
	const char* const REVIVE = "revive";                  // ����
	const char* const USE_KILLER = "use_killer";          // ʹ�ñ�ɱ
	const char* const USE_SHIELD = "use_shield";          // ʹ�û���
	const char* const USE_MAX_BULLLET = "use_max_bullet"; // ����

	const char* const NEW_GIFT = "new_gift";          // ������������
	const char* const MID_GIFT = "mid_gift";          // �������ֵ���
	const char* const BIG_GIFT = "big_gift";          // ������������
	const char* const HUG_GIFT = "hug_gift";          // ��������������
	const char* const NEW_GIFT_RECOMMEND = "new_gift_recommend"; //�Ƽ����������
	const char* const MID_GIFT_RECOMMEND = "mid_gift_recommend"; //�Ƽ�����ֵ���
	const char* const BIG_GIFT_RECOMMEND = "big_gift_recommend"; //�Ƽ����������
	const char* const HUG_GIFT_RECOMMEND = "hug_gift_recommend"; //�Ƽ������������

	const char* const REINFORCE_FIRST     = "reinforce_first";     // ��һ��ʹ��ǿ��
	const char* const REINFORCE_ALL       = "reinforce_all";       // TODO ����ǿ������

	//ǿ����ȫ�֣�����
	const char* const REINFORCE_GLOBAL_DIAMOND = "reinforce_diamond";   // ��ʯת��
	const char* const REINFORCE_GLOBAL_RAMPAGE = "reinforce_rampage";   // ս�����ߣ���ũ���ڣ�
	const char* const REINFORCE_GLOBAL_KILLER  = "reinforce_killer";    // ��ɱ�Ż�
	const char* const REINFORCE_GLOBAL_SHIELD  = "reinforce_shield";    // ���ӻ���
	const char* const REINFORCE_GLOBAL_REPAIR  = "reinforce_repair";    // װ���޸�

	const char* const REINFORCE_VALUE_DIAMOND_RATIO = "reinforce_value_diamond_ratio";  // ��ʯת���������� float
	const char* const REINFORCE_VALUE_RAMPAGE_DURA = "reinforce_value_rampage_dura";    // ս������ʱ������ float
	const char* const REINFORCE_VALUE_KILLER_COLD = "reinforce_value_killer_cold";    // ������ɱ��ȴʱ����� float
	const char* const REINFORCE_VALUE_SHIELD_INIT   = "reinforce_value_shield_init";    // ��ʼ���ܸ������� float
	const char* const REINFORCE_VALUE_REPAIR_RATIO  = "reinforce_value_repair_ratio";   // װ���޸����� float

	//ǿ����װ�������𡣽ṹ��id1,own1,equip1,grade1;
	const char* const REINFORCE_EQUIP = "reinforce_equip";  // ս��/����/����/�Ż�

	//�ṹ��equip1,equip2,equip3,aricraft1;
	const char* const PLAYER_EQUIP = "player_equip";        //��ͬ��ս����װ��������

	const char* const EQUIP_AIRCRAFT = "equip_aircraft";    // ��ǰװ����ս��id
	const char* const EQUIP_ARMATURE = "equip_armature";    // ��ǰװ���Ļ���id
	const char* const EQUIP_MISSILE  = "equip_missile";     // ��ǰװ���ĵ���id
	const char* const EQUIP_WINGMAN  = "equip_wingman";     // ��ǰװ�����Ż�id

	//��ұ���
	const char* const KILLER  = "killer";  //������ɱ����
	const char* const SHIELD  = "shield";  //�ӵ���������
	const char* const SKILL_CD_TIME = "skill_cd_time"; //��ɱ����ȴʱ��

	//�ɾ�
	const char* const ACHIEVEMENT = "achievement";//�ɾ��Ƿ��ɡ��Ƿ���ȡ������ϸ

	const char* const USE_AIRCRAFT_4 = "use_aircraft_4";//�ɾ��Ƿ���
	const char* const USE_AIRCRAFT_5 = "use_aircraft_5";//�ɾ��Ƿ���
	const char* const USE_AIRCRAFT_6 = "use_aircraft_6";//�ɾ��Ƿ���

	//ÿ�ս���
	const char* const DAY_REWARD_LOGIN_DAYS = "day_reward_login_days"; //������½������(>=0)
	const char* const DAY_REWARD_LAST_LOGIN = "day_reward_last_login"; //��һ�ε�½������(1970�굽����0��0��0�������)

	//VIP
	const char* const VIP_START_TIME = "vip_start_time"; //vip��ʼʱ�䣨��ȷ���룩
	const char* const VIP_DAYS = "vip_days";             //��������
	const char* const VIP_FLAG = "vip_flag";             //vip��־����������Ϸ�п����ж��Ƿ���vip

	const char* const VIP_EXPIRE_REMIND = "vip_expire_remind";//vip�����������ѣ�ÿ������ʱ�����ѣ������ر�����
	const char* const VIP_ORDER_REMIND = "vip_order_remind";  //vip��ͨ���ѣ�������һ�Σ������������

	const char* const VIP_FREE_REFLIE = "vip_free_relife";    //vipÿһ�ص���Ѹ������
	const char* const VIP_RELIFE_ATTR_ADJUST = "vip_relife_attr_adjust"; //vip�����ս��������ǿ

	//��������
	const char* const GUIDE = "guide";

	//��Ϸ����ʱ����
	const char* const TIME = "time"; //��Զ�̻�ȡ�ĵ�ǰʱ��

	//�����˸���
	const char* const BUY_RELIFE = "buy_relife";
};

enum EGameDataType
{
	DataType_Int = 0,
	DataType_Float = 1,
	DataType_String = 2,
};

struct TGameData
{
	std::string key;
	EGameDataType type;
	int iDefaultValue;
	float fDefaultValue;
	std::string strDefaultValue;

	TGameData(const std::string& k, int v) :key(k), type(EGameDataType::DataType_Int), iDefaultValue(v){}
	TGameData(const std::string& k, float v) :key(k), type(EGameDataType::DataType_Float), fDefaultValue(v){}
	TGameData(const std::string& k, std::string v) :key(k), type(EGameDataType::DataType_String), strDefaultValue(v){}
};

typedef std::vector<IListener*> ListenerArray;
typedef std::vector<IListener*>* ListenerArrayPtr;
typedef std::map<std::string, ListenerArrayPtr>  ListenerMap;


class GameData
{
public:
	static GameData* getInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new GameData();
		}

		return m_pInstance;
	}

	void reset();
	bool loadData();
	bool saveData();

	int getValueToInt(const char* const key, int defaultVallue = 0);
	const std::string& getValue(const char* const key, const std::string& defaultVallue = "");
	float getValueToFloat(const char* const key, float defaultVallue = 0);

	int incValue(const char* const key, int value);
	float incValue(const char* const key, float value);

	void setValue(const char* const key, int value);
	void setValue(const char* const key, float value);
	void setValue(const char* const key, const std::string& value);

	//��ʶ�ɾ��Ƿ��ɣ��û��Ƿ�����ȡ����
	void setAchieveData(int id, int isAchieve, int isReceive);

	//��������
	void setLevelStar(int difficulty, int level, int star);
	int getLevelStar(int difficulty, int level);

	void setLevelMax(int level);
	void setLevelFinish(int level);

	int getLevelMax(int difficulty = -1);
	int getLevelFinish(int difficulty = -1);

	//��������
	void addGuide(int type);
	bool checkGuide(int type);

	//vip: ��ȡvipʣ���ʱ�䣨�룩
	int getVipTime();
	int addVipTime(int days);

	//vip��Ѹ���
	void incVipFreeRelifeCnt(int level);
	int  getVipFreeRelifeCnt(int level);

	//���ü�����
	void regListener(IListener* listener, const std::string& type);
	void unRegListener(IListener* listener, const std::string& type);
	void notifyListener(const std::string& type, int value);

protected:
	GameData();
	virtual ~GameData();

protected:
	bool saveJson(std::string& buf);
	bool loadJson(const char* json);

	bool saveAchievement();
	bool loadAchievement();

	bool saveReinforceEquip();
	bool loadReinforceEquip();

	bool savePlayerEquip();
	bool loadPlayerEquip();

	bool saveLevelStar();
	bool loadLevelStar();

	bool saveLevelMax();
	bool loadLevelMax();

	bool saveLevelFinish();
	bool loadLevelFinish();

	bool saveGuide();
	bool loadGuide();

	bool saveFile();
	bool loadFile();

	bool saveVipFreeRelifeCnt();
	bool loadVipFreeRelifeCnt();

	//
	int     getIntegerForKey(const char* pKey, int defaultValue = 0);
	float    getFloatForKey(const char* pKey, float defaultValue = 0.f);
	std::string getStringForKey(const char* pKey, const std::string & defaultValue = "");

	void    setIntegerForKey(const char* pKey, int value);
	void    setFloatForKey(const char* pKey, float value);
	void    setStringForKey(const char* pKey, const std::string & value);

private:
	static GameData* m_pInstance;

	FILE* m_pFile;

	std::map<std::string, int> m_iMap;
	std::map<std::string, float> m_fMap;
	std::map<std::string, std::string> m_strMap;

	std::vector<TGameData> m_gameDataDefine;

	std::map<int, std::vector<int>> m_levelStar;
	std::map<int, int> m_levelMax;
	std::map<int, int> m_levelFinish;

	std::set<int> m_guideList;

	std::map<int, int> m_vipFreeRelife;

	//������
	ListenerMap m_listenerMap;
};