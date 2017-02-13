#pragma once

#include "cocos2d.h"
#include "GameObject.h"
#include "IListener.h"
#include "GlobalData.h"
#include "Bullet.h"
#include "Common/Macro.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

class HpBar;
class Armature;

//飞机类型。
enum EAircraftType
{
	Type_Unknown = -1,
	Type_Player = 0,
	Type_Wingman = 1,
	Type_Enemy = 2,
	Type_Killer = 3
};

//飞机扩展属性
namespace AircraftAttr
{
	const char* const Desc = "desc";
}

//飞机类型，初始化飞机时使用
namespace AircraftType
{
	const char* const PLAYER  = "PLAYER";
	const char* const WINGMAN = "WINGMAN";
	const char* const ENEMY1  = "ENEMY1";
	const char* const ENEMY2  = "ENEMY2";
	const char* const BOSS = "BOSS";
}

//血槽
struct THpBarData
{
	std::string hpBgStyle; //血槽背景样式
	std::string hpStyle;   //血槽样式
};

//飞机等级数据
struct TAircraftLevelData
{
	int id;        //
	int aircraftId;//飞机id
	int grade;     //等级
	int hp;        //最大生命值
	int attack;    //攻击
	int money;     //击落后可获得的宝石数
	int score;     //击落后可获得的分数

	std::vector<std::vector<int>*> bulletIdArray;  //多种子弹
};

//飞机属性数据
struct TAircraftData
{
	int id;
	std::string name;//名字
	std::string type;//类型，值为AircraftType
	std::vector<std::string> styleArray;  //飞机动画
	float aniDura; //动画帧时长
	std::string armatureName; //骨骼动画名称

	int hp;           //当前生命值
	int maxHp;        //最大生命值
	float defence;    //防御
	int attack;       //攻击

	float bodyCenterX; //刚体中心坐标
	float bodyCenterY;
	float bodySizeW;   //刚体尺寸
	float bodySizeH;
	float scale;       //缩放比例

	std::string hpBgStyle; //血槽背景样式
	std::string hpStyle;   //血槽样式
	float hpPosX;          //血槽的位置
	float hpPosY;          //血槽的位置

	std::vector<std::vector<int>*> bulletIdArray;  //多种子弹

	std::vector<std::string> fireFameNameArray; //尾部火焰动画，图片列表
	float fireAniDura; //动画帧时长
	int   fireOnTop;
	float fireOffsetX;//火焰中心点相对于飞机底部中心点的偏移。如果等于0，则只有一个火焰；否则是两个火焰
	float fireOffsetY;

	float startPosX;  //飞机的起始位置
	float startPosY;

	std::vector<int> wingmanArray; //僚机

	std::vector<std::string> blastStyleArray; //飞机爆炸的动画
	float blastAniDura;
	int blastMusicId; //飞机爆炸的音效id

	std::map<std::string, std::string> paramMap; //飞机参数

	const TAircraftLevelData* pAircraftLevelData; //飞机等级数据

	TAircraftData()
	{
		reset();
	}

	void reset()
	{
		this->id = -1;
		this->name = "";
		this->type = "";
		this->styleArray.clear();
		this->aniDura = 0;
		this->armatureName = "";

		this->hp = INT_MAX;
		this->maxHp = INT_MAX;
		this->defence = -1;
		this->attack = INT_MAX;

		this->bodyCenterX = 0;
		this->bodyCenterY = 0;
		this->bodySizeW = 0;
		this->bodySizeH = 0;
		this->scale = 0;

		this->hpBgStyle = "";
		this->hpStyle = "";
		this->hpPosX = 0;
		this->hpPosY = 0;

		for (unsigned i = 0; i < this->bulletIdArray.size(); i++)
		{
			delete bulletIdArray.at(i);
		}
		this->bulletIdArray.clear();

		this->fireFameNameArray.clear();
		this->fireAniDura = 0;
		this->fireOnTop = 0;
		this->fireOffsetX = 0;
		this->fireOffsetY = 0;

		this->startPosX = 0;
		this->startPosY = 0;

		this->wingmanArray.clear();

		this->blastAniDura = 0;
		this->blastStyleArray.clear();
		this->blastMusicId = -1;

		this->paramMap.clear();

		this->pAircraftLevelData = nullptr;
	}

	TAircraftData(const TAircraftData& copy)
	{
		clone(copy);
	}

	void clone(const TAircraftData& data)
	{
		this->id = data.id;
		this->name = data.name;
		this->type = data.type;
		this->styleArray.clear();
		for (unsigned i = 0; i < data.styleArray.size(); i++)
		{
			this->styleArray.push_back(data.styleArray.at(i));
		}
		this->aniDura = data.aniDura;
		this->armatureName = data.armatureName;

		this->hp = data.hp;
		this->maxHp = data.maxHp;
		this->defence = data.defence;
		this->attack = data.attack;

		this->bodyCenterX = data.bodyCenterX;
		this->bodyCenterY = data.bodyCenterY;
		this->bodySizeW = data.bodySizeW;
		this->bodySizeH = data.bodySizeH;
		this->scale = data.scale;

		this->hpBgStyle = data.hpBgStyle;
		this->hpStyle = data.hpStyle;
		this->hpPosX = data.hpPosX;
		this->hpPosY = data.hpPosY;

		this->bulletIdArray.clear();
		for (unsigned i = 0; i < data.bulletIdArray.size(); i++)
		{
			const std::vector<int>* src = data.bulletIdArray.at(i);
			std::vector<int>* dest = new std::vector<int>;
			for (unsigned j = 0; j < src->size(); j++)
			{
				dest->push_back(src->at(j));
			}
			this->bulletIdArray.push_back(dest);
		}

		this->fireFameNameArray.clear();
		for (unsigned i = 0; i < data.fireFameNameArray.size(); i++)
		{
			this->fireFameNameArray.push_back(data.fireFameNameArray.at(i));
		}
		this->fireAniDura = data.fireAniDura;
		this->fireOnTop = data.fireOnTop;
		this->fireOffsetX = data.fireOffsetX;
		this->fireOffsetY = data.fireOffsetY;

		this->startPosX = data.startPosX;
		this->startPosY = data.startPosY;

		this->wingmanArray.clear();
		for (unsigned i = 0; i < data.wingmanArray.size(); i++)
		{
			this->wingmanArray.push_back(data.wingmanArray.at(i));
		}

		this->blastAniDura = data.blastAniDura;
		this->blastStyleArray.clear();
		for (unsigned i = 0; i < data.blastStyleArray.size(); i++)
		{
			this->blastStyleArray.push_back(data.blastStyleArray.at(i));
		}
		this->blastMusicId = data.blastMusicId;

		this->paramMap.clear();
		std::map<std::string, std::string>::const_iterator it = data.paramMap.begin();
		for (; it != data.paramMap.end(); it++)
		{
			this->paramMap.insert(std::map<std::string, std::string>::value_type(it->first, it->second));
		}

		this->pAircraftLevelData = data.pAircraftLevelData;
	}

	inline int getMaxAttack(float adjust = 0) const
	{
		//计算最高级别子弹的威力
		std::vector<int>* array = this->bulletIdArray.at(0);
		int max = 0;
		for (int bulletId : *array)
		{
			const TBulletData* pBullet = GlobalData::getInstance()->getBulletData(bulletId);
			//只计算非导弹类子弹的威力
			if (strcmp(pBullet->type.c_str(), BULLET_TYPE::MISSILE)
				|| strcmp(this->type.c_str(), AircraftType::PLAYER))
			{
				max += const_cast<TBulletData*>(pBullet)->getMaxAttack(adjust);
			}
		}

		return max;
	}

	//设置当前飞机的等级数据
	inline void setLevelData(const TAircraftLevelData* levelData)
	{
		if (!levelData)
		{
			return;
		}

		this->pAircraftLevelData = levelData;

		this->maxHp = levelData->hp;
		this->hp = levelData->hp;
		this->attack = levelData->attack;

		this->bulletIdArray.clear();
		bulletIdArray.assign(levelData->bulletIdArray.begin(), levelData->bulletIdArray.end());
	}
};

//飞机的基类
class Aircraft : public GameObject, public IBulletUseUpListener
{
public:
	Aircraft();
	virtual ~Aircraft();

	virtual bool init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData = nullptr);
	virtual void reset();
	virtual void destory();

	virtual void reduceHp(int hp);
	virtual void recoverHp(int hp);
	virtual bool isAlive();

	virtual void startShoot();
	virtual void stopShoot();
	virtual bool isShooting();

	virtual void autoShoot();
	virtual void stopAutoShoot();
	virtual void autoChangeBulletGrade();

	//自动飞行
	virtual void fly(){};

	//爆炸
	virtual void blast();
	
	//更新装备
	bool updateEquip();

	//子弹升级
	virtual bool upgradeBullet();
	virtual bool upgradeMaxBullet();
	//子弹降级
	virtual bool downgradeBullet();
	//是否暴走
	virtual bool isMaxLevelBullet();
	//是否最低级子弹
	virtual bool isMinLevelBullet();
	//获取子弹等级
	virtual int getBulletLevel();
	//重置子弹级别
	virtual bool resetBulletLevel();

	//子弹用完通知
	virtual void bulletUseUp() override;

	//量子护盾
	void addShield();
	void removeShield();
	//防御效果结束
	virtual void defenceDone(float dt);

	//碰撞检测
	virtual bool isCollsion(const Rect& rect, Rect* pCollsionRect = nullptr);

	//获取攻击、防御、导弹、僚机、综合性能
	inline int getAttrAttack(){ return m_iAttrAttack; }
	inline int getAttrArmor() { return m_iAttrArmor; }
	inline int getAttrMissile(){ return m_iAttrMissile; }
	inline int getAttrWingman() { return m_iAttrWingman; }
	inline int getAttrTotal() { return m_iAttrTotal; }

	void calculateAttr();

	inline int getId()
	{
		return m_data.id;
	}

	inline int getLevelId()
	{
		if (m_data.pAircraftLevelData)
		{
			return m_data.pAircraftLevelData->id;
		}
		else
		{
			return -1;
		}
	}

	inline int getHp()
	{
		return m_data.hp;
	}

	inline void setHp(int hp)
	{
		m_data.hp = hp;
	}

	inline int getMaxHp()
	{
		return m_data.maxHp;
	}

	inline void setMaxHp(int max)
	{
		m_data.maxHp = max;
	}

	inline int getAttack()
	{
		//两机相撞时，对对方产生的伤害就是自己的血量
		return getHp();
	}

	inline void setAttack(int a)
	{
		m_data.attack = a;
	}

	void setNoDie(bool b);

	inline bool isNoDie()
	{
		return m_bNoDie;
	}

	inline Vector<BulletGroup*>* getBulletGroupArray()
	{
		return &m_bulletGroupArray;
	}

	inline EAircraftType getAircraftType()
	{
		return m_eAircraftType;
	}

	inline const std::string& getAttr(const std::string& key)
	{
		static std::string empty = "";
		auto it = m_data.paramMap.find(key);
		if (it != m_data.paramMap.end())
		{
			return it->second;
		}
		else
		{
			return empty;
		}
	}

	inline int getAircraftLevelId()
	{
		if (m_data.pAircraftLevelData)
		{
			return m_data.pAircraftLevelData->id;
		}
		else
		{
			return -1;
		}
	}

	Vector<Aircraft*>* getOtherSidePlane() const;
	void setOtherSidePlane(Vector<Aircraft*>* const planes);

	//回收
	virtual void recycle();
	//重用
	virtual void reuse();
	//是否已回收（是否可用）
	bool isRecycled();

protected:
	virtual void setBulletGrade(unsigned grade){ m_iBulletGrade = grade; }
	virtual void setAttackAdjust(float adjust){ m_fAttackAdjust = adjust; }
	virtual void setMissileAdjust(float adjust){ m_fMissileAdjust = adjust; }
	virtual void setBulletSpeedAdjust(float adjust){ m_fBulletSpeedAdjust = adjust; }

	virtual bool initBody(Node* parent, const TAircraftData* pData);
	virtual bool initPosition(Node* parent, const TAircraftData* pData);
	virtual bool initFire(Node* parent, const TAircraftData* pData);
	virtual bool initHpBar(Node* parent, const TAircraftData* pData);
	virtual bool initBullet(Node* parent, const TAircraftData* pData);
	virtual bool initWingman(Node* parent, const TAircraftData* pData);

	//添加尾部的左右两个火焰动画
	bool addFire(float offsetX, float offsetY, bool isFlipped);

protected:
	EAircraftType m_eAircraftType;
	TAircraftData m_data;
	const TAircraftData* m_pDataCopy;

	//战机所有的装备
	const TEquipmentData* m_pEquipAircraft;
	const TEquipmentData* m_pEquipArmature;
	const TEquipmentData* m_pEquipMissile;
	const TEquipmentData* m_pEquipWingman;

	int m_iBulletGrade;
	float m_fAttackAdjust;
	float m_fMissileAdjust;
	float m_fBulletSpeedAdjust;

	float m_fVipRelifeAttrAdjust;

	float m_fDefence;           //防御系数
	float m_fDefenceDura;       //防御持续时间
	int m_iCurBulletGrade; //当前子弹等级
	bool m_bNoDie;              //无敌
	cocostudio::Armature* m_pDefenceBall;     //护盾球
	HpBar* m_pHpBar;            //血槽精灵
	Vector<BulletGroup*> m_bulletGroupArray;  //多种子弹
	Vector<Aircraft*>    m_wingmanArray;      //僚机精灵
	Vector<Aircraft*>*   m_otherSideArray;    //对方飞机。对于玩家来说就是敌机，对于敌机来说就是玩家

	int m_iAttrAttack;
	int m_iAttrArmor;
	int m_iAttrMissile;
	int m_iAttrWingman;
	int m_iAttrTotal;

	bool m_bRecycled;

	bool m_bAutoShoot;
	int m_orignBulletGrade;
};

//飞机工厂类
template<typename T>
class PlaneCreator
{
public:
	static T* create(Node* parent, const TAircraftData* data)
	{
		T *pRet = new(std::nothrow) T();
		if (pRet && pRet->init(parent, data))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

	static T* create(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData)
	{
		T *pRet = new(std::nothrow) T();
		if (pRet && pRet->init(parent, pData, pLevelData))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

};
