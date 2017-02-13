#include "GuideImpl.h"
#include "Common/Macro.h"
#include "GlobalData.h"
#include "GameData.h"
#include "Preload.h"
#include "Reinforce.h"
#include "Equipment.h"
#include "ModeBase.h"
#include "Difficulty.h"

enum GuideTest
{
	GuideTestNoGuide,  //不显示引导
	GuideTestNormal,   //正常判断是否显示引导
	GuideTestNecessary //必显示引导
};

namespace GuideConstant
{
	GuideTest GuideDisplay = GuideTestNormal;
	const char* const StringGuideWelcome = "guide_welcome";
	const char* const StringGuidePressLevelMode = "guide_press_level_mode";
	const char* const StringGuidePressLevelOne = "guide_press_level_1";

	const char* const StringGuideKiller = "guide_killer";
	const char* const StringGuideShield = "guide_shield";

	const char* const StringGuideMenuBag = "guide_bag";
	const char* const StringGuideMenuBag1 = "guide_bag1";
	const char* const StringGuideMenuBag2 = "guide_bag2";

	const char* const StringGuideMenuStrengthen = "guide_strengthen";
	const char* const StringGuideMenuStrengthen1 = "guide_strengthen1";
	const char* const StringGuideMenuStrengthen2 = "guide_strengthen2";

	const char* const StringGuideMenuReward = "guide_reward";

	const char* const StringGuideModeEndless = "guide_endless";
	const char* const StringGuideModeRapid = "guide_rapid";

	const char* const StringGuideModeDifficulty2 = "guide_difficulty2";
	const char* const StringGuideModeDifficulty3 = "guide_difficulty3";

	const char* const StringGuideAircraft2Open = "guide_aircraft2_open";
	const char* const StringGuideAircraft3Open = "guide_aircraft3_open";
}

////////////////////////////////////////////////////////////////
//
//  首次进入游戏的介绍
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::WelcomeGirl, GuideWelcomeGirl);

bool GuideWelcomeGirl::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) == 0;
	}
}

bool GuideWelcomeGirl::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideWelcome));

	if (!Guide::doInit())
	{
		return false;
	}

	//GameData::getInstance()->addGuide(mType);
	return true;
}

////////////////////////////////////////////////////////////////
//
//  首次进入游戏，点击闯关模式
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::WelcomeClickButtonLevel, GuideWelcomeClickButtonLevel);

bool GuideWelcomeClickButtonLevel::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) == 0;
	}
}

bool GuideWelcomeClickButtonLevel::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuidePressLevelMode));

	if (!Guide::doInit())
	{
		return false;
	}

	//GameData::getInstance()->addGuide(mType);
	return true;
}

////////////////////////////////////////////////////////////////
//
//  首次进入游戏，点击第一关
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::WelcomeClickMissionOne, GuideWelcomeClickMissionOne);

bool GuideWelcomeClickMissionOne::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) == 0;
	}
}

bool GuideWelcomeClickMissionOne::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuidePressLevelOne));

	if (!Guide::doInit())
	{
		return false;
	}

	//GameData::getInstance()->addGuide(mType);
	return true;
}

////////////////////////////////////////////////////////////////
//
//  所有道具的说明
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::GameEquipment, GuideGameEquipment);

bool GuideGameEquipment::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::WelcomeClickMissionOne)
			&& GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 1;
	}
}

bool GuideGameEquipment::doInit()
{
	if (!Guide::doInit())
	{
		return false;
	}

	//中间显示奖励品说明
	Node* pRewardRemark = Preload::getInstance()->getUI("Instruction.csb");
	pRewardRemark->setAnchorPoint(Vec2(0.5, 0.5));
	pRewardRemark->setPosition(CONSTANT::DESIGN_RES_WIDTH / 2, CONSTANT::DESIGN_RES_HEIGHT / 2);
	this->addChild(pRewardRemark);

	auto pAction = Preload::getInstance()->getUIAction("Instruction.csb");
	pRewardRemark->runAction(pAction);
	pAction->gotoFrameAndPlay(0, false);

	//GameData::getInstance()->addGuide(mType);
	return true;
}

////////////////////////////////////////////////////////////////
//
//  必杀引导
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::GameKiller, GuideGameKiller);

bool GuideGameKiller::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::WelcomeClickMissionOne)
			&& GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 1;
	}
}

bool GuideGameKiller::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideKiller));

	if (!Guide::doInit())
	{
		return false;
	}

	//GameData::getInstance()->addGuide(mType);
	return true;
}
////////////////////////////////////////////////////////////////
//
//  护盾引导
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::GameShield, GuideGameShield);

bool GuideGameShield::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::WelcomeClickMissionOne)
			&& GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL) == 1;
	}
}

bool GuideGameShield::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideShield));

	if (!Guide::doInit())
	{
		return false;
	}

	//GameData::getInstance()->addGuide(mType);
	return true;
}

////////////////////////////////////////////////////////////////
//
//  仓库引导
//
////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::MenuBag, GuideMenuBag);

bool GuideMenuBag::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		const TEquipmentData* pData = GlobalData::getInstance()->getEquipment(21);//装备21的价格

		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->getValueToInt(GAMEDATA::MONEY) >= pData->price
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= 1;
	}
}

bool GuideMenuBag::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuBag));

	if (!Guide::doInit())
	{
		return false;
	}

	//GameData::getInstance()->addGuide(mType);
	return true;
}

////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::MenuBag1, GuideMenuBag1);

bool GuideMenuBag1::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::MenuBag);
	}
}

bool GuideMenuBag1::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuBag1));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::MenuBag2, GuideMenuBag2);

bool GuideMenuBag2::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::MenuBag1);
	}
}

bool GuideMenuBag2::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuBag2));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////
//
//  强化引导
//
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::MenuStrengthen, GuideMenuStrengthen);

bool GuideMenuStrengthen::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		const TReinforceGradeData* pGradeData = GlobalData::getInstance()->getReinforce(ReinforceTypeDiamond).at(1);

		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->getValueToInt(GAMEDATA::MONEY) >= pGradeData->money
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= 1;
	}
}

bool GuideMenuStrengthen::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuStrengthen));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::MenuStrengthen1, GuideMenuStrengthen1);

bool GuideMenuStrengthen1::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		const TReinforceGradeData* pGradeData = GlobalData::getInstance()->getReinforce(ReinforceTypeDiamond).at(1);

		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::MenuStrengthen);
	}
}

bool GuideMenuStrengthen1::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuStrengthen1));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////
REGISTER_GUIDE(GuideType::MenuStrengthen2, GuideMenuStrengthen2);

bool GuideMenuStrengthen2::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		const TReinforceGradeData* pGradeData = GlobalData::getInstance()->getReinforce(ReinforceTypeDiamond).at(1);

		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::MenuStrengthen1);
	}
}

bool GuideMenuStrengthen2::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuStrengthen2));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////
//
//  成就奖励引导
//
////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::MenuReward, GuideMenuReward);

bool GuideMenuReward::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType);
	}
}

bool GuideMenuReward::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideMenuReward));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////
//
//  无尽模式引导
//
////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::ModeEndless, GuideModeEndless);

bool GuideModeEndless::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::MenuBag)
			//&& GameData::getInstance()->checkGuide(GuideType::MenuStrengthen)
			//&& GameData::getInstance()->checkGuide(GuideType::MenuReward)
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= ModeBase::ModeEndlessOpen;
	}
}

bool GuideModeEndless::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideModeEndless));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////
//
//  急速模式引导
//
////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::ModeRapid, GuideModeRapid);

bool GuideModeRapid::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::ModeEndless)
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= ModeBase::ModeRapidOpen;
	}
}

bool GuideModeRapid::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideModeRapid));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////
//
//  英雄、炼狱难度引导
//
////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::Difficulty2, GuideDifficulty2);

bool GuideDifficulty2::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::MenuBag)
			&& GameData::getInstance()->getLevelMax(Difficulty::Normal) == 1
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= Difficulty::Difficulty2Open;
	}
}

bool GuideDifficulty2::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideModeDifficulty2));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::Difficulty3, GuideDifficulty3);

bool GuideDifficulty3::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::Difficulty2)
			&& GameData::getInstance()->getLevelMax(Difficulty::Hard) == 1
			&& GameData::getInstance()->getLevelFinish(Difficulty::Easy) >= Difficulty::Difficulty3Open;
	}
}

bool GuideDifficulty3::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideModeDifficulty3));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////
//
//  战机2/3开启
//
////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::Aircraft2Open, GuideAircraft2Open);

bool GuideAircraft2Open::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType);
	}
}

bool GuideAircraft2Open::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideAircraft2Open));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////

REGISTER_GUIDE(GuideType::Aircraft3Open, GuideAircraft3Open);

bool GuideAircraft3Open::checkCondition()
{
	switch (GuideConstant::GuideDisplay)
	{
	case GuideTestNoGuide:   return false;
	case GuideTestNecessary: return true;
	default:
		return !GameData::getInstance()->checkGuide(mType)
			&& GameData::getInstance()->checkGuide(GuideType::Aircraft2Open);
	}
}

bool GuideAircraft3Open::doInit()
{
	Guide::setPrompt(GlobalData::getInstance()->getParameter(GuideConstant::StringGuideAircraft3Open));

	if (!Guide::doInit())
	{
		return false;
	}

	return true;
}