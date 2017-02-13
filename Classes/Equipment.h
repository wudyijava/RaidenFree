#pragma once

#include "cocos2d.h"

USING_NS_CC;

namespace EquipmentConstant
{
	const int DefaultPlayerId = 1;
}

namespace EquipPiece
{
	const char* const Aircraft = "piece_aircraft";
	const char* const Armature = "piece_armature";
	const char* const Missile = "piece_missile";
	const char* const Wingman = "piece_wingman";

	const char* const NoAircraft = "piece_no_aircraft";
	const char* const Aircraft7 = "piece_aircraft_7";
};

typedef enum
{
	EquipTypeAircraft = 1,
	EquipTypeArmature = 2,
	EquipTypeMissile = 3,
	EquipTypeWingman = 4
}EquipType;

struct TEquipmentData
{
	int id;
	std::string name;
	EquipType type;
	int reinforce_id;
	int value;
	int price;
	std::string currency;
	int piece;
	float probabilty;
	std::string remark;
	std::string pic;

	int own;  //是否已拥有
	int equip;//是否已装备
	int grade;//强化级别
	int pick; //捡到多少碎片

	TEquipmentData()
		: id(0)
		, own(0)
		, equip(0)
		, grade(0)
		, pick(0)
	{

	}
};

class PlayerEquip
{
public:
	static PlayerEquip* getInstance();
	void destory();

	std::map<int, std::vector<const TEquipmentData*>*>& getAllPlayerEquip();
	std::vector<const TEquipmentData*>* getEquip(int playerId);
	const TEquipmentData* getEquip(int playerId, EquipType type);

	void addEquip(int playerId, int equipId);
	void addEquip(int playerId, const TEquipmentData* pEquip);
	void removeEquip(int playerId, int equipId);

	//获取随机碎片
	void getRandomPiece(unsigned piece, std::set<const TEquipmentData*>& equipPieceArray);

private:
	static PlayerEquip* m_pInstance;

	std::map<int, std::vector<const TEquipmentData*>*> m_playerEquipMap;
};
