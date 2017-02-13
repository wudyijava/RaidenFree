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

//�ɻ����͡�
enum EAircraftType
{
	Type_Unknown = -1,
	Type_Player = 0,
	Type_Wingman = 1,
	Type_Enemy = 2,
	Type_Killer = 3
};

//�ɻ���չ����
namespace AircraftAttr
{
	const char* const Desc = "desc";
}

//�ɻ����ͣ���ʼ���ɻ�ʱʹ��
namespace AircraftType
{
	const char* const PLAYER  = "PLAYER";
	const char* const WINGMAN = "WINGMAN";
	const char* const ENEMY1  = "ENEMY1";
	const char* const ENEMY2  = "ENEMY2";
	const char* const BOSS = "BOSS";
}

//Ѫ��
struct THpBarData
{
	std::string hpBgStyle; //Ѫ�۱�����ʽ
	std::string hpStyle;   //Ѫ����ʽ
};

//�ɻ��ȼ�����
struct TAircraftLevelData
{
	int id;        //
	int aircraftId;//�ɻ�id
	int grade;     //�ȼ�
	int hp;        //�������ֵ
	int attack;    //����
	int money;     //�����ɻ�õı�ʯ��
	int score;     //�����ɻ�õķ���

	std::vector<std::vector<int>*> bulletIdArray;  //�����ӵ�
};

//�ɻ���������
struct TAircraftData
{
	int id;
	std::string name;//����
	std::string type;//���ͣ�ֵΪAircraftType
	std::vector<std::string> styleArray;  //�ɻ�����
	float aniDura; //����֡ʱ��
	std::string armatureName; //������������

	int hp;           //��ǰ����ֵ
	int maxHp;        //�������ֵ
	float defence;    //����
	int attack;       //����

	float bodyCenterX; //������������
	float bodyCenterY;
	float bodySizeW;   //����ߴ�
	float bodySizeH;
	float scale;       //���ű���

	std::string hpBgStyle; //Ѫ�۱�����ʽ
	std::string hpStyle;   //Ѫ����ʽ
	float hpPosX;          //Ѫ�۵�λ��
	float hpPosY;          //Ѫ�۵�λ��

	std::vector<std::vector<int>*> bulletIdArray;  //�����ӵ�

	std::vector<std::string> fireFameNameArray; //β�����涯����ͼƬ�б�
	float fireAniDura; //����֡ʱ��
	int   fireOnTop;
	float fireOffsetX;//�������ĵ�����ڷɻ��ײ����ĵ��ƫ�ơ��������0����ֻ��һ�����棻��������������
	float fireOffsetY;

	float startPosX;  //�ɻ�����ʼλ��
	float startPosY;

	std::vector<int> wingmanArray; //�Ż�

	std::vector<std::string> blastStyleArray; //�ɻ���ը�Ķ���
	float blastAniDura;
	int blastMusicId; //�ɻ���ը����Чid

	std::map<std::string, std::string> paramMap; //�ɻ�����

	const TAircraftLevelData* pAircraftLevelData; //�ɻ��ȼ�����

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
		//������߼����ӵ�������
		std::vector<int>* array = this->bulletIdArray.at(0);
		int max = 0;
		for (int bulletId : *array)
		{
			const TBulletData* pBullet = GlobalData::getInstance()->getBulletData(bulletId);
			//ֻ����ǵ������ӵ�������
			if (strcmp(pBullet->type.c_str(), BULLET_TYPE::MISSILE)
				|| strcmp(this->type.c_str(), AircraftType::PLAYER))
			{
				max += const_cast<TBulletData*>(pBullet)->getMaxAttack(adjust);
			}
		}

		return max;
	}

	//���õ�ǰ�ɻ��ĵȼ�����
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

//�ɻ��Ļ���
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

	//�Զ�����
	virtual void fly(){};

	//��ը
	virtual void blast();
	
	//����װ��
	bool updateEquip();

	//�ӵ�����
	virtual bool upgradeBullet();
	virtual bool upgradeMaxBullet();
	//�ӵ�����
	virtual bool downgradeBullet();
	//�Ƿ���
	virtual bool isMaxLevelBullet();
	//�Ƿ���ͼ��ӵ�
	virtual bool isMinLevelBullet();
	//��ȡ�ӵ��ȼ�
	virtual int getBulletLevel();
	//�����ӵ�����
	virtual bool resetBulletLevel();

	//�ӵ�����֪ͨ
	virtual void bulletUseUp() override;

	//���ӻ���
	void addShield();
	void removeShield();
	//����Ч������
	virtual void defenceDone(float dt);

	//��ײ���
	virtual bool isCollsion(const Rect& rect, Rect* pCollsionRect = nullptr);

	//��ȡ�������������������Ż����ۺ�����
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
		//������ײʱ���ԶԷ��������˺������Լ���Ѫ��
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

	//����
	virtual void recycle();
	//����
	virtual void reuse();
	//�Ƿ��ѻ��գ��Ƿ���ã�
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

	//���β���������������涯��
	bool addFire(float offsetX, float offsetY, bool isFlipped);

protected:
	EAircraftType m_eAircraftType;
	TAircraftData m_data;
	const TAircraftData* m_pDataCopy;

	//ս�����е�װ��
	const TEquipmentData* m_pEquipAircraft;
	const TEquipmentData* m_pEquipArmature;
	const TEquipmentData* m_pEquipMissile;
	const TEquipmentData* m_pEquipWingman;

	int m_iBulletGrade;
	float m_fAttackAdjust;
	float m_fMissileAdjust;
	float m_fBulletSpeedAdjust;

	float m_fVipRelifeAttrAdjust;

	float m_fDefence;           //����ϵ��
	float m_fDefenceDura;       //��������ʱ��
	int m_iCurBulletGrade; //��ǰ�ӵ��ȼ�
	bool m_bNoDie;              //�޵�
	cocostudio::Armature* m_pDefenceBall;     //������
	HpBar* m_pHpBar;            //Ѫ�۾���
	Vector<BulletGroup*> m_bulletGroupArray;  //�����ӵ�
	Vector<Aircraft*>    m_wingmanArray;      //�Ż�����
	Vector<Aircraft*>*   m_otherSideArray;    //�Է��ɻ������������˵���ǵл������ڵл���˵�������

	int m_iAttrAttack;
	int m_iAttrArmor;
	int m_iAttrMissile;
	int m_iAttrWingman;
	int m_iAttrTotal;

	bool m_bRecycled;

	bool m_bAutoShoot;
	int m_orignBulletGrade;
};

//�ɻ�������
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
