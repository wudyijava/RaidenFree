#pragma once

#include "cocos2d.h"
#include "GameObject.h"
#include "Common/Macro.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

class Aircraft;
class BulletGroup;
class Missile;
class IBulletUseUpListener;

//�ӵ�����
namespace BULLET_TYPE
{
	//ͨ��
	const char* const COMMON = "COMMON";
	const char* const COMMON_AIM = "COMMON_AIM";
	//�Զ���
	const char* const CUSTOM = "CUSTOM";
	//��ҵ���
	const char* const MISSILE = "MISSILE";
	//��Ҽ���
	const char* const LASER = "LASER";
	//��ɱ��������
	const char* const SUPERLASER = "SUPERLASER";
}

namespace BULLET_ATTR
{
	//����ר������
	const char* const MISSILE_VELOCITY = "MISSILE_VELOCITY";  //�ٶ�
	const char* const MISSILE_ACCEL = "MISSILE_ACCEL";        //���ٶ�
	const char* const MISSILE_TURN_RATE = "MISSILE_TURN_RATE";//���ٶ�
	const char* const MISSILE_MAX_ANGLE = "MISSILE_MAX_ANGLE";//���Ƕ�

};

struct TBulletCustomData
{
	float time;
	float x;
	float y;
	float speed;
	float angle;

	TBulletCustomData()
	{
		reset();
	}

	void reset()
	{
		time = 0;
		x = 0;
		y = 0;
		speed = 0;
		angle = 0;
	}

	TBulletCustomData(const TBulletCustomData& copy)
	{
		clone(copy);
	}

	inline void clone(const TBulletCustomData& data)
	{
		time = data.time;
		x = data.x;
		y = data.y;
		speed = data.speed;
		angle = data.angle;
	}

	inline bool operator<(const TBulletCustomData& data) const
	{
		return this->time < data.time ? true : false;
	}
};

struct TBulletData
{
	int id;
	std::string name;
	std::string type;
	std::vector<std::string> styleArray;
	float aniDura;
	std::string armatureName;
	int topOfAircraft;
	int musicId;

	int attack;
	int speed;

	int count;//ÿ�η�����ӵ��Ŀ���
	int max_count;
	float timeLimit;

	float angle_of_center;
	float angle_interval;

	float x_interval;

	float delay;
	float interval;
	float interval_2;
	int interval_2_cnt;

	float rotate_angle;
	float rotate_max_angle;
	int rotate_flag;

	float bodyCenterX;
	float bodyCenterY;
	float bodySizeW;
	float bodySizeH;

	float origin_offset_x;
	float origin_offset_y;

	std::vector<std::string> blastStyleArray;
	float blastAniDura;

	std::vector<std::string> fireFameNameArray; //β�����涯����ͼƬ�б�
	float fireAniDura; //����֡ʱ��
	int   fireOnTop;
	float fireOffsetX;//�������ĵ�����ڷɻ��ײ����ĵ��ƫ�ơ��������0����ֻ��һ�����棻��������������
	float fireOffsetY;

	std::map<std::string, float> paramMap;
	std::vector<TBulletCustomData*> customArray;

	void reset()
	{
		this->id = -1;
		this->name = "";
		this->type = BULLET_TYPE::COMMON;
		this->styleArray.clear();
		this->aniDura = 0;
		this->armatureName = "";
		this->topOfAircraft = 0;
		this->musicId = -1;

		this->attack = 0;
		this->speed = 0;

		this->count = -1;
		this->max_count = -1;
		this->timeLimit = -1;

		this->angle_of_center = 90;
		this->angle_interval = 0;
		this->x_interval = 0;
		
		this->delay = 0;
		this->interval = 0;
		this->interval_2 = 0;
		this->interval_2_cnt = 0;

		this->rotate_angle = 0;
		this->rotate_max_angle = 0;
		this->rotate_flag = 0;

		this->bodyCenterX = 0;
		this->bodyCenterY = 0;
		this->bodySizeW = 0;
		this->bodySizeH = 0;

		this->origin_offset_x = 0;
		this->origin_offset_y = 0;

		this->blastAniDura = 0;
		this->blastStyleArray.clear();

		this->fireFameNameArray.clear();
		this->fireAniDura = 0;
		this->fireOnTop = 0;
		this->fireOffsetX = 0;
		this->fireOffsetY = 0;

		this->paramMap.clear();
		this->customArray.clear();
	}

	TBulletData()
	{
		reset();
	}

	TBulletData(const TBulletData& copy)
	{
		clone(copy);
	}

	void clone(const TBulletData& data)
	{
		this->id = data.id;
		this->name = data.name;
		this->type = data.type;
		this->musicId = data.musicId;
		this->styleArray.clear();
		for (unsigned i = 0; i < data.styleArray.size(); i++)
		{
			this->styleArray.push_back(data.styleArray.at(i));
		}
		this->aniDura = data.aniDura;
		this->armatureName = data.armatureName;
		this->topOfAircraft = data.topOfAircraft;

		this->attack = data.attack;
		this->speed = data.speed;

		this->count = data.count;
		this->max_count = data.max_count;
		this->timeLimit = data.timeLimit;

		this->angle_of_center = data.angle_of_center;
		this->angle_interval = data.angle_interval;
		this->x_interval = data.x_interval;

		this->delay = data.delay;
		this->interval = data.interval;
		this->interval_2 = data.interval_2;
		this->interval_2_cnt = data.interval_2_cnt;

		this->rotate_angle = data.rotate_angle;
		this->rotate_max_angle = data.rotate_max_angle;
		this->rotate_flag = data.rotate_flag;

		this->bodyCenterX = data.bodyCenterX;
		this->bodyCenterY = data.bodyCenterY;
		this->bodySizeW = data.bodySizeW;
		this->bodySizeH = data.bodySizeH;

		this->origin_offset_x = data.origin_offset_x;
		this->origin_offset_y = data.origin_offset_y;

		this->blastAniDura = data.blastAniDura;
		this->blastStyleArray.clear();
		for (unsigned i = 0; i < data.blastStyleArray.size(); i++)
		{
			this->blastStyleArray.push_back(data.blastStyleArray.at(i));
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

		this->paramMap.clear();
		std::map<std::string, float>::const_iterator it = data.paramMap.begin();
		for (; it != data.paramMap.end(); it++)
		{
			this->paramMap.insert(std::map<std::string, float>::value_type(it->first, it->second));
		}

		this->customArray.clear();
		for (unsigned i = 0; i < data.customArray.size(); i++)
		{
			this->customArray.push_back(data.customArray.at(i));
		}
	}

	inline int getMaxAttack(float adjust = 0) const
	{
		int attack = (int)((this->attack * (1 + adjust)) + 0.5);

		float cntPerSecond = 0;
		if (this->interval_2_cnt > 0)
		{
			cntPerSecond = 1.f / ((this->interval + this->interval_2 * this->interval_2_cnt) / this->interval_2_cnt);
		}
		else
		{
			cntPerSecond = 1.f / this->interval;
		}

		if (this->count > 0)
		{
			return this->count * attack * cntPerSecond;
		}
		else
		{
			return attack * cntPerSecond;
		}
	}

};

class Bullet : public GameObject
{
public:
	friend class BulletGroup;

	static Bullet* create(BulletGroup* pBulletGroup);

	//����pBulletGroup�е��ӵ�������Դ����ӵ�
	virtual bool init(BulletGroup* pBulletGroup);
	virtual void reset();

protected:
	Bullet();
	virtual ~Bullet();

	//���β���������������涯��
	bool addFire(float offsetX, float offsetY, bool isFlipped);

protected:
	BulletGroup* m_pBulletGroup;
	bool m_bBlast;
};

//�ӵ��Ļ��ࡣ�ڲ�ά����һ���ӵ���
class BulletGroup : public GameObjectContainer
{
public:
	friend class Bullet;
	friend class Missile;
	friend class Laser;

	BulletGroup();
	virtual ~BulletGroup();

	virtual bool init(Node* pParent, Aircraft* pPlane, const TBulletData* pData);
	virtual void reset();
	virtual void destory();

	//��ʼ���
	virtual void startShoot();
	//����ָ���������ӵ�
	virtual void startShoot(int cnt);
	//ֹͣ���
	virtual void stopShoot();
	//�Ƿ��������
	virtual bool isShooting();

	//���ӵ�����ɾ��һ���ӵ���������Ļ��Χ�����߻���Ŀ��
	virtual void RemoveBullet(Bullet* pBullet);
	//��ը
	virtual void blast(Bullet* pBullet);

	//���Է��ɻ����ӻ��߼���ʱ��֪ͨ����
	virtual void nodifyTargetRemove(Aircraft* pAircraft) {}

	//ע���ӵ����������
	void regBulletUseUpListener(IBulletUseUpListener* l);
	void notifyBulletUseUp();
	inline bool isUseUp() { return m_bIsUseUp; }

public:
	//�ӵ��ɳ���Ļ�����ɶ������ڲ�������
	virtual void bulletMoveFinished(Node* pSender);

	inline int getAttack()
	{
		return m_data.attack;
	}

	inline void setAttack(int a)
	{
		m_data.attack = a;
	}

	inline void setPlane(Aircraft* plane)
	{
		m_plane = plane;
	}

	inline Aircraft* getPlane()
	{
		return m_plane;
	}

	inline void setOtherSidePlane(Vector<Aircraft*>* const planes)
	{
		m_otherSideArray = planes;
	}

	virtual void update(float dt) override;

protected:

	//����һ���ӵ������ܻ��������ӵ���
	virtual void AddBullet(float dt) {};

	//���ӵ����л�ȡһ���ӵ�
	virtual Bullet* getOneBullet();

	float getPara(const char* const param);

	bool isSameGroup() { return m_bSameGroup; }
protected:

	TBulletData m_data;

	int m_iCount;
	int m_iAvailableBullet;
	bool m_bIsShooting;
	Aircraft* m_plane;
	Vector<Aircraft*> * m_otherSideArray;

	int m_iIntervalCnt; //С����ӵ��������
	float m_iTimeCum;   //ʱ���ۼ�
	float m_iTimeCumThisGrade;   //���ȼ��µ�ʱ���ۼ�
	bool m_bFirstShoot; //

	bool m_bSameGroup;  //�Ƿ���ͬһ��������

	IBulletUseUpListener* m_pBulletUseUpListener;
	bool m_bIsUseUp;
	float m_timeLimitAdd;
};

//�ӵ��鹤����
class BulletGroupCreator
{
public:
	static BulletGroup* create(Node* pParent, Aircraft* pPlane, const TBulletData* pData);
};
