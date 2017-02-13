#pragma once

#ifdef GROUP_NAME
#undef GROUP_NAME
#endif

namespace DBTABLE
{
	const char* const DBNAME = "cfg/raiden.db";

	namespace PARAMETER
	{
		//表名
		const char* const TABLE = "PARAMETER";
		//字段
		const char* const COL_GROUP_NAME = "GROUP_NAME";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_KEY = "KEY";
		const char* const COL_VALUE = "VALUE";
	}

	namespace ACHIEVEMENT
	{
		//表名
		const char* const TABLE = "ACHIEVEMENT";
		//字段
		const char* const 	COL_ID = "ID";
		const char* const 	COL_NAME = "NAME";
		const char* const 	COL_REMARK = "REMARK";
		const char* const 	COL_KEY = "KEY";
		const char* const 	COL_VALUE = "VALUE";
		const char* const 	COL_REWARD_NAME = "REWARD_NAME";
		const char* const 	COL_REWARD_KEY = "REWARD_KEY";
		const char* const 	COL_REWARD_VALUE = "REWARD_VALUE";
	}

	namespace AIRCRAFT
	{
		//表名
		const char* const TABLE = "AIRCRAFT";
		//字段
		const char* const 	COL_ID = "ID";
		const char* const 	COL_NAME = "NAME";
		const char* const 	COL_TYPE = "TYPE";
		const char* const 	COL_STYLE_ANI_ID = "STYLE_ANI_ID";
		const char* const 	COL_ARMATURE_NAME = "ARMATURE_NAME";
		const char* const 	COL_HP = "HP";
		const char* const 	COL_ATTACK = "ATTACK";
		const char* const 	COL_DEFENCE = "DEFENCE";
		const char* const 	COL_HPBAR_ID = "HPBAR_ID";
		const char* const 	COL_HPBAR_OFFSET_X = "HPBAR_OFFSET_X";
		const char* const 	COL_HPBAR_OFFSET_Y = "HPBAR_OFFSET_Y";
		const char* const 	COL_BULLET_ID = "BULLET_ID";
		const char* const 	COL_BULLET_GRADE_ID = "BULLET_GRADE_ID";
		const char* const 	COL_BODY_CENTER_X = "BODY_CENTER_X";
		const char* const 	COL_BODY_CENTER_Y = "BODY_CENTER_Y";
		const char* const 	COL_BODY_SIZE_W = "BODY_SIZE_W";
		const char* const 	COL_BODY_SIZE_H = "BODY_SIZE_H";
		const char* const 	COL_SCALE = "SCALE";
		const char* const 	COL_FIRE_ANI_ID = "FIRE_ANI_ID";
		const char* const 	COL_FIRE_ON_TOP = "FIRE_ON_TOP";
		const char* const 	COL_FIRE_OFFSET_X = "FIRE_OFFSET_X";
		const char* const 	COL_FIRE_OFFSET_Y = "FIRE_OFFSET_Y";
		const char* const 	COL_START_POS_X = "START_POS_X";
		const char* const 	COL_START_POS_Y = "START_POS_Y";
		const char* const 	COL_BLAST_ANI_ID = "BLAST_ANI_ID";
		const char* const 	COL_BLAST_MUSIC_ID = "BLAST_MUSIC_ID";
	}

	namespace AIRCRAFT_LEVEL
	{
		const char* const TABLE = "AIRCRAFT_LEVEL";

		const char* const COL_ID = "ID";
		const char* const COL_AIRCRAFT_ID = "AIRCRAFT_ID";
		const char* const COL_GRADE = "GRADE";
		const char* const COL_HP = "HP";
		const char* const COL_BULLET_ID = "BULLET_ID";
		const char* const COL_BULLET_GRADE_ID = "BULLET_GRADE_ID";
		const char* const COL_MONEY = "MONEY";
		const char* const COL_SCORE = "SCORE";
	}

	namespace AIRCRAFT_ATTR
	{
		const char* const TABLE = "AIRCRAFT_ATTR";

		const char* const COL_AIRCRAFT_ID = "AIRCRAFT_ID";
		const char* const COL_KEY = "KEY";
		const char* const COL_VALUE = "VALUE";
	}

	namespace ANIMATION
	{
		const char* const TABLE = "ANIMATION";

		const char* const COL_ID = "ID";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_NAME = "NAME";
		const char* const COL_DURATION = "DURATION";

		const char* const TYPE_PLAYER = "PLAYER";
		const char* const TYPE_ENEMY = "ENEMY";
		const char* const TYPE_FIRE = "FIRE";
		const char* const TYPE_BLAST = "BLAST";
		const char* const TYPE_BULLET = "BULLET";
	}

	namespace ANIMATION_FILE
	{
		//表名
		const char* const TABLE = "ANIMATION_FILE";

		const char* const COL_ANIMATION_ID = "ANIMATION_ID";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_FILENAME = "FILENAME";
		const char* const COL_DURATION = "DURATION";
	}

	namespace HPBAR
	{
		const char* const TABLE = "HPBAR";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_FOREGROUND = "FOREGROUND";
		const char* const COL_BACKGROUND = "BACKGROUND";
	}
	
	namespace WINGMAN
	{
		const char* const TABLE = "WINGMAN";

		const char* const COL_ID = "ID";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_NAME = "NAME";
		const char* const COL_AIRCRAFT_ID = "AIRCRAFT_ID";
	}
	
	namespace BULLET_GRADE
	{
		const char* const TABLE = "BULLET_GRADE";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_GRADE = "GRADE";
		const char* const COL_BULLET_ID = "BULLET_ID";
	}

	namespace BULLET
	{
		const char* const TABLE = "BULLET";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_STYLE_ANI_ID = "STYLE_ANI_ID";
		const char* const COL_ARMATURE_NAME = "ARMATURE_NAME";
		const char* const COL_TOP_OF_AIRCRAFT = "TOP_OF_AIRCRAFT";
		const char* const COL_MUSIC_ID = "MUSIC_ID";

		const char* const COL_ATTACK = "ATTACK";
		const char* const COL_SPEED = "SPEED";

		const char* const COL_COUNT = "COUNT";
		const char* const COL_MAX_COUNT = "MAX_COUNT";
		const char* const COL_TIME_LIMIT = "TIME_LIMIT";

		const char* const COL_ANGLE_OF_CENTER = "ANGLE_OF_CENTER";
		const char* const COL_ANGLE_INTERVAL = "ANGLE_INTERVAL";

		const char* const COL_X_INTERVAL = "X_INTERVAL";

		const char* const COL_DELAY = "DELAY";
		const char* const COL_INTERVAL = "INTERVAL";
		const char* const COL_INTERVAL_2 = "INTERVAL_2";
		const char* const COL_INTERVAL_2_CNT = "INTERVAL_2_CNT";

		const char* const COL_ROTATE_ANGLE = "ROTATE_ANGLE";
		const char* const COL_ROTATE_MAX_ANGLE = "ROTATE_MAX_ANGLE";
		const char* const COL_ROTATE_FLAG = "ROTATE_FLAG";

		const char* const COL_BODY_CENTER_X = "BODY_CENTER_X";
		const char* const COL_BODY_CENTER_Y = "BODY_CENTER_Y";
		const char* const COL_BODY_SIZE_W = "BODY_SIZE_W";
		const char* const COL_BODY_SIZE_H = "BODY_SIZE_H";

		const char* const COL_ORIGIN_OFFSET_X = "ORIGIN_OFFSET_X";
		const char* const COL_ORIGIN_OFFSET_Y = "ORIGIN_OFFSET_Y";

		const char* const COL_BLAST_ANI_ID = "BLAST_ANI_ID";
		const char* const COL_FIRE_ANI_ID = "FIRE_ANI_ID";
		const char* const COL_FIRE_ON_TOP = "FIRE_ON_TOP";
		const char* const COL_FIRE_OFFSET_X = "FIRE_OFFSET_X";
		const char* const COL_FIRE_OFFSET_Y = "FIRE_OFFSET_Y";
	}

	namespace BULLET_ATTR
	{
		const char* const TABLE = "BULLET_ATTR";

		const char* const COL_BULLET_ID = "BULLET_ID";
		const char* const COL_KEY = "KEY";
		const char* const COL_VALUE = "VALUE";
	}

	namespace BULLET_CUSTOM
	{
		const char* const TABLE = "BULLET_CUSTOM";

		const char* const COL_BULLET_ID = "BULLET_ID";
		const char* const COL_TIME = "TIME";
		const char* const COL_X = "X";
		const char* const COL_Y = "Y";
		const char* const COL_SPEED = "SPEED";
		const char* const COL_ANGLE = "ANGLE";
	}

	namespace TRACK
	{
		const char* const TABLE = "TRACK";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_LOOP = "LOOP";
		const char* const COL_START_POS_X = "START_POS_X";
		const char* const COL_START_POS_Y = "START_POS_Y";
	}

	namespace TRACK_SEGMENT
	{
		const char* const TABLE = "TRACK_SEGMENT";

		const char* const COL_TRACK_ID = "TRACK_ID";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_SPEED = "SPEED";

		const char* const COL_X = "X";
		const char* const COL_Y = "Y";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_CTRL_X_1 = "CTRL_X_1";
		const char* const COL_CTRL_Y_1 = "CTRL_Y_1";
		const char* const COL_CTRL_X_2 = "CTRL_X_2";
		const char* const COL_CTRL_Y_2 = "CTRL_Y_2";
	}

	namespace LEVEL
	{
		const char* const TABLE = "LEVEL";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_MUSIC_ID = "MUSIC_ID";
		const char* const COL_BG_GROUP_ID = "BG_GROUP_ID";
		const char* const COL_REWARD_GROUP_ID = "REWARD_GROUP_ID";
		const char* const COL_ENEMY_GROUP_ID = "ENEMY_GROUP_ID";
		const char* const COL_BOSS_TYPE = "BOSS_TYPE";
		const char* const COL_PIECE = "PIECE";
	}

	namespace LEVEL_BACKGROUND
	{
		const char* const TABLE = "LEVEL_BACKGROUND";

		const char* const COL_GROUP_ID = "GROUP_ID";
		const char* const COL_SUB_GROUP_ID = "SUB_GROUP_ID";
		const char* const COL_ZORDER = "ZORDER";
		const char* const COL_FILENAME = "FILENAME";
		const char* const COL_SPEED_START = "SPEED_START";
		const char* const COL_SPEED_END = "SPEED_END";
		const char* const COL_DURATION = "DURATION"; //speed从start变到end所需的时长
		const char* const COL_DISTANCE = "DISTANCE"; //两张图之间的间距
		const char* const COL_COUNT = "COUNT";       //重复次数，空为无限
		const char* const COL_FLIP_Y = "FLIP_Y";     //是否垂直反转
	}

	namespace LEVEL_REWARD
	{
		const char* const TABLE = "LEVEL_REWARD";

		const char* const COL_GROUP_ID = "GROUP_ID";
		const char* const COL_REWARD_ID = "REWARD_ID";
		const char* const COL_PROBABILITY = "PROBABILITY";
	}

	namespace LEVEL_EVENT
	{
		const char* const TABLE = "LEVEL_EVENT";

		const char* const COL_GROUP_ID = "GROUP_ID";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_EVENT = "EVENT";
		const char* const COL_TIME = "TIME";
		const char* const COL_PAUSE = "PAUSE";
		const char* const COL_OBJECT_ID = "OBJECT_ID";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_BULLET_ID = "BULLET_ID";
		const char* const COL_TRACK_ID = "TRACK_ID";
		const char* const COL_DIRECTION_FLAG = "DIRECTION_FLAG";
		const char* const COL_REPEAT = "REPEAT";
		const char* const COL_INTERVAL = "INTERVAL";
		const char* const COL_REWARD_GROUP = "REWARD_GROUP_ID";
	}

	namespace REWARD
	{
		//表名
		const char* const TABLE = "REWARD";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_STYLE_ANI_ID = "STYLE_ANI_ID";
		const char* const COL_ARMATURE_NAME = "ARMATURE_NAME";
		const char* const COL_PROMPT = "PROMPT";
		const char* const COL_MUSIC_ID = "MUSIC_ID";
		const char* const COL_SPEED = "SPEED";
		const char* const COL_MAX_SPEED = "MAX_SPEED";
		const char* const COL_RADIUS = "RADIUS";
		const char* const COL_LIFE_TIME = "LIFE_TIME";
		const char* const COL_BODY_CENTER_X = "BODY_CENTER_X";
		const char* const COL_BODY_CENTER_Y = "BODY_CENTER_Y";
		const char* const COL_BODY_SIZE_W = "BODY_SIZE_W";
		const char* const COL_BODY_SIZE_H = "BODY_SIZE_H";
	}

	namespace REWARD_ATTR
	{
		const char* const TABLE = "REWARD_ATTR";

		const char* const COL_REWARD_ID = "REWARD_ID";
		const char* const COL_KEY = "KEY";
		const char* const COL_VALUE = "VALUE";
	}

	namespace REWARD_GROUP
	{
		const char* const TABLE = "REWARD_GROUP";

		const char* const COL_GROUP_ID = "GROUP_ID";
		const char* const COL_REWARD_ID = "REWARD_ID";
		const char* const COL_CNT = "CNT";
		const char* const COL_PROBABILITY = "PROBABILITY";
	}

	namespace ARMATURE
	{
		const char* const TABLE = "ARMATURE";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_DEFAULT_ACTION = "DEFAULT_ACTION";
	}

	namespace BOSS_HP_ACTION
	{
		const char* const TABLE = "BOSS_HP_ACTION";

		const char* const COL_AIRCRAFT_ID = "AIRCRAFT_ID";
		const char* const COL_HP_THRESHOLD = "HP_THRESHOLD";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_ACTION_GROUP_ID = "ACTION_GROUP_ID";
		const char* const COL_DELAY = "DELAY";
		const char* const COL_REPEAT = "REPEAT";
	}

	namespace BOSS_ACTION_GROUP
	{
		const char* const TABLE = "BOSS_ACTION_GROUP";

		const char* const COL_GROUP_ID = "GROUP_ID";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_ARMATURE_ACTION = "ARMATURE_ACTION";
		const char* const COL_NO_DIE = "NO_DIE";
		const char* const COL_DELAY = "DELAY";
		const char* const COL_REPEAT = "REPEAT";
		const char* const COL_BULLET_GRADE_ID = "BULLET_GRADE_ID";
		const char* const COL_BULLET_FLAG = "BULLET_FLAG";
	}

	namespace MUSIC
	{
		const char* const TABLE = "MUSIC";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_FILENAME = "FILENAME";
		const char* const COL_IS_EFFECT = "IS_EFFECT";
	}

	namespace REINFORCE
	{
		const char* const TABLE = "REINFORCE";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_KEY = "KEY";
	}

	namespace REINFORCE_GRADE
	{
		const char* const TABLE = "REINFORCE_GRADE";

		const char* const COL_ID = "ID";
		const char* const COL_REINFORCE_ID = "REINFORCE_ID";
		const char* const COL_GRADE = "GRADE";
		const char* const COL_MONEY = "MONEY";
		const char* const COL_KEY = "KEY";
		const char* const COL_VALUE = "VALUE";
		const char* const COL_REMARK = "REMARK";
	}

	namespace GOODS
	{
		const char* const TABLE = "GOODS";

		const char* const COL_ID = "ID";
		const char* const COL_CARRIER = "CARRIER";
		const char* const COL_GOODS = "GOODS";
		const char* const COL_ORIGN_PRICE = "ORIGN_PRICE";
		const char* const COL_PRICE = "PRICE";
		const char* const COL_CURRENCY = "CURRENCY";
		const char* const COL_NAME = "NAME";
		const char* const COL_BILL_INDEX = "BILL_INDEX";
		const char* const COL_DESC = "DESC";
	}

	namespace GOODS_ITEM
	{
		const char* const TABLE = "GOODS_ITEM";

		const char* const COL_ID = "ID";
		const char* const COL_CARRIER = "CARRIER";
		const char* const COL_GOODS_ID = "GOODS_ID";
		const char* const COL_KEY = "KEY";
		const char* const COL_VALUE = "VALUE";
	}

	namespace DIALOG
	{
		const char* const TABLE = "DIALOG";

		const char* const COL_ID = "ID";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_NAME = "NAME";
		const char* const COL_PIC = "PIC";
		const char* const COL_CONTENT = "CONTENT";
	}

	namespace DAY_GIFT
	{
		const char* const TABLE = "DAY_GIFT";

		const char* const COL_DAY = "DAY";
		const char* const COL_SEQ = "SEQ";
		const char* const COL_GIFT = "GIFT";
		const char* const COL_GIFT_NAME = "GIFT_NAME";
		const char* const COL_COUNT = "COUNT";
	}

	namespace DIFFICULTY
	{
		const char* const TABLE = "DIFFICULTY";

		const char* const COL_ID = "ID";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_NAME = "NAME";
		const char* const COL_MONEY = "MONEY";
		const char* const COL_BULLET_SPEED = "BULLET_SPEED";
		const char* const COL_ATTACK = "ATTACK";
		const char* const COL_HP = "HP";
		const char* const COL_REWARD_PROB = "REWARD_PROB";
		const char* const COL_ENEMY_GRADE = "ENEMY_GRADE";
	}

	namespace EQUIPMENT
	{
		const char* const TABLE = "EQUIPMENT";

		const char* const COL_ID = "ID";
		const char* const COL_NAME = "NAME";
		const char* const COL_TYPE = "TYPE";
		const char* const COL_REINFORCE_ID = "REINFORCE_ID";
		const char* const COL_VALUE = "VALUE";
		const char* const COL_PRICE = "PRICE";
		const char* const COL_CURRENCY = "CURRENCY";
		const char* const COL_PIECE = "PIECE";
		const char* const COL_PROBABILITY = "PROBABILITY";
		const char* const COL_REMARK = "REMARK";
		const char* const COL_PIC = "PIC";
	}

}
