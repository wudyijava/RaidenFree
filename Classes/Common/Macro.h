#pragma once

#include "cocos2d.h"
#include "Utils.h"

USING_NS_CC;

namespace CONSTANT
{
	const char* const EMPTY = "";
	const std::string EMPTY_STRING("");

	//设计分辨率
	const float DESIGN_RES_WIDTH = 540;    //宽度
	const float DESIGN_RES_HEIGHT = 960;   //高度
	const float DESIGN_RES_DIAGONAL = 1101.453585f; //对角线长度
	const float RES_RATIO = 0.75f;

	//z-order数字大的在上面，数字小的在下面
	//PlaneLayer
	const int ZORDER_DEBUG = 9999999;
	const int ZORDER_ACHIEVEMENT = 7000;
	const int ZORDER_WARNING = 6500;
	const int ZORDER_REWARD = 6000;

	//废弃
	//const int ZORDER_BLAST = 4000;

	const int ZORDER_BULLET_ONTOP = 4000;

	const int ZORDER_PLAYER = 3500;
	const int ZORDER_WINGMAN = 3300;

	const int ZORDER_BULLET_ENEMY = 2200;  //敌机子弹不能被玩家子弹遮住
	const int ZORDER_BULLET_PLAYER = 2100; //玩家子弹必须在战机的下面

	const int ZORDER_ENEMY = 1700;
	const int ZORDER_BOSS = 1600;

	const int ZORDER_KILLER = 1100;
	const int ZORDER_BULLET_KILLER = 1000; //加农神炮

	const int ZORDER_BACKGROUND = -9999999;

	//PlayerPlane 玩家飞机精灵内部
	const int ZORDER_PLAYERPLANE_FIRE = -99;

	//关卡数
	const int TOTAL_MISSION = 40;

	//数学常量
	const float PI_DIV_180 = 0.017453292519943f;
	const float YIBA0_DIV_PI = 57.295779513082f;
	const float PI = 3.1415926536f;
}

#define DEBUG_LOG(fmt, ...) \
do{\
	if (Utils::isWriteLog())\
	{\
		Utils::writeLog(__FILE__, __LINE__, fmt, ##__VA_ARGS__); \
	}\
} while (0);

#define FIND_UI_CONTROL(type, name, ptr, parent) \
do\
{\
	ptr = dynamic_cast<type>(parent->getChildByName(name)); \
	/*CCASSERT(ptr, std::string(std::string(name) + std::string(" is nullptr")).c_str());*/ \
} while (0)

#define FIND_UI_CONTROL_RE(type, name, ptr, parent) \
do\
{\
	ptr = dynamic_cast<type>(Utils::getChildByNameRe(parent, name)); \
	/*CCASSERT(ptr, std::string(std::string(name) + std::string(" is nullptr")).c_str());*/ \
} while (0)

#define COPY_VECTOR(a, b) \
do{\
	a.clear(); \
	for (unsigned i = 0; i < b.size(); i++)\
	{\
		a.push_back(b.at(i)); \
	}\
} while (0)

#define CLEAR_VECTOR(a) \
do\
{\
	for (unsigned i = 0; i < a.size(); i++)\
	{\
		delete a.at(i); \
	}\
	a.clear(); \
} while (0)

#define CLEAR_MAP(a) \
do\
{\
	for (auto it = m_GoodsMap.begin(); it != m_GoodsMap.end(); ++it)\
	{\
		delete it->second; \
	}\
	a.clear(); \
} while (0)
