#include "Preload.h"

#include "Common/Macro.h"
#include "GameData.h"
#include "Sound.h"
#include "Achievement.h"
#include "Armature.h"
#include "Carrier.h"
#include "cocostudio/CCArmatureDataManager.h"
#include "base/CCAsyncTaskPool.h"
#include "Bmob/BmobUtils.h"

using namespace cocostudio::timeline;
using namespace cocostudio;

namespace PreloadConstant
{
	const char* const StringTipsDaily = "tips_daily";
}

namespace PreloadProgress
{
	const int GlobalData = 20;
	const int GameData = 3;
	const int AchievementMgr = 3;
	const int Sound = 10;
}

Preload* Preload::m_pInstance = nullptr;

Preload* Preload::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Preload();
	}

	return m_pInstance;
}

Preload::Preload()
: m_pListener(nullptr)
, m_iTmpProgress(0)
, m_iAllProgress(0)
, m_iImageCnt(0)
, m_iArmatureCnt(0)
, m_iImagePlistCnt(0)

, m_bImageLoaded(false)
, m_bArmatureLoaded(false)
, m_bUILoaded(false)
, m_bOtherLoaded(false)
, m_bImagePlistLoaded(false)
, m_bRemoteDataLoaded(false)

, m_bRemoteDataSuccess(false)

, m_errorCode(PreloadError::Success)
{

}

Preload::~Preload()
{
	m_uiMap.clear();
}

void Preload::start(PreloadListener* listener)
{
	m_pListener = listener;

	//单独的png或jpg
	m_imageArray.push_back("BigImg/Bag_Bg.png");
	m_imageArray.push_back("BigImg/BigScreen_Bg.png");
	m_imageArray.push_back("BigImg/Daily_Bg.png");
	m_imageArray.push_back("BigImg/MainUI_Bg.jpg");
	m_imageArray.push_back("BigImg/Mission_Map1.jpg");
	m_imageArray.push_back("BigImg/Mission_Map2.jpg");
	m_imageArray.push_back("BigImg/Mission_Map3.jpg");
	m_imageArray.push_back("BigImg/Relive_Bg.png");
	m_imageArray.push_back("BigImg/Settle_Bg.jpg");

	//m_imageArray.push_back("Scene/Bg_Base.png");
	//m_imageArray.push_back("Scene/Bg_Base_2.png");
	//m_imageArray.push_back("Scene/Bg_Cloud.png");
	//m_imageArray.push_back("Scene/Bg_Cloud_2.png");
	//m_imageArray.push_back("Scene/Bg_Meteorite.png");
	//m_imageArray.push_back("Scene/Bg_Planet.png");
	//m_imageArray.push_back("Scene/Map_01.jpg");
	//m_imageArray.push_back("Scene/Map_02.jpg");
	//m_imageArray.push_back("Scene/Map_03.jpg");
	//m_imageArray.push_back("Scene/Map_04.jpg");
	//m_imageArray.push_back("Scene/Map_05.jpg");
	//m_imageArray.push_back("Scene/Map_06.png");
	//m_imageArray.push_back("Scene/Map_07.jpg");
	//m_imageArray.push_back("Scene/Map_08.png");

	m_iAllProgress += m_imageArray.size();

	//plist图片
	m_plistArray.push_back("res/Bullet/bullet");
	m_plistArray.push_back("res/Blast/blast");
	m_plistArray.push_back("res/Missile/missile");

	m_plistArray.push_back("Bag");
	m_plistArray.push_back("Common");
	m_plistArray.push_back("Daily");
	m_plistArray.push_back("Enemy");
	m_plistArray.push_back("GameUI");
	m_plistArray.push_back("Instruction");
	m_plistArray.push_back("ItemProp");
	m_plistArray.push_back("MainUI");
	m_plistArray.push_back("Mission");
	m_plistArray.push_back("Pause");
	m_plistArray.push_back("Retry");
	m_plistArray.push_back("Reward");
	m_plistArray.push_back("Settle");
	m_plistArray.push_back("Setup");
	m_plistArray.push_back("Shop");
	m_plistArray.push_back("Strengthen");

	m_plistArray.push_back("Other/SuperKiller");
	m_plistArray.push_back("Other/BulletHit");

	m_iAllProgress += m_plistArray.size();
	//armature
	m_armatureArray.push_back("Anim/Anim_Plane_01");
	m_armatureArray.push_back("Anim/Anim_Plane_02");
	m_armatureArray.push_back("Anim/Anim_Plane_03");
	m_armatureArray.push_back("Anim/Anim_Plane_04");
	m_armatureArray.push_back("Anim/Anim_Plane_05");
	m_armatureArray.push_back("Anim/Anim_Plane_06");
	m_armatureArray.push_back("Anim/Anim_Plane_07");

	m_armatureArray.push_back("Anim/Anim_Wingman_01");
	m_armatureArray.push_back("Anim/Anim_Wingman_02");
	m_armatureArray.push_back("Anim/Anim_Wingman_03");
	m_armatureArray.push_back("Anim/Anim_Wingman_04");
	m_armatureArray.push_back("Anim/Anim_Wingman_05");
	m_armatureArray.push_back("Anim/Anim_Wingman_06");

	m_armatureArray.push_back("Anim/Anim_Enemy_01");
	m_armatureArray.push_back("Anim/Anim_Enemy_02");
	m_armatureArray.push_back("Anim/Anim_Enemy_03");
	m_armatureArray.push_back("Anim/Anim_Enemy_04");
	m_armatureArray.push_back("Anim/Anim_Enemy_05");
	m_armatureArray.push_back("Anim/Anim_Enemy_06");
	m_armatureArray.push_back("Anim/Anim_Enemy_07");
	m_armatureArray.push_back("Anim/Anim_Enemy_08");
	m_armatureArray.push_back("Anim/Anim_Enemy_09");
	m_armatureArray.push_back("Anim/Anim_Enemy_10");
	m_armatureArray.push_back("Anim/Anim_Enemy_11");
	m_armatureArray.push_back("Anim/Anim_Enemy_12");
	m_armatureArray.push_back("Anim/Anim_Enemy_13");
	m_armatureArray.push_back("Anim/Anim_Enemy_14");
	m_armatureArray.push_back("Anim/Anim_Enemy_15");
	m_armatureArray.push_back("Anim/Anim_Enemy_16");
	m_armatureArray.push_back("Anim/Anim_Enemy_17");
	m_armatureArray.push_back("Anim/Anim_Enemy_18");

	m_armatureArray.push_back("Anim/Anim_Boss_01");
	m_armatureArray.push_back("Anim/Anim_Boss_02");
	m_armatureArray.push_back("Anim/Anim_Boss_03");
	m_armatureArray.push_back("Anim/Anim_Boss_04");
	m_armatureArray.push_back("Anim/Anim_Boss_05");

	m_armatureArray.push_back("Anim/Aerolite_01");

	m_armatureArray.push_back("Anim/UI_Ready");
	m_armatureArray.push_back("Anim/UI_Victory");
	m_armatureArray.push_back("Anim/UI_Warning");

	m_armatureArray.push_back("Anim/Item_GameUI_Diamond");
	m_armatureArray.push_back("Anim/Item_GameUI_Levelup");
	m_armatureArray.push_back("Anim/Item_GameUI_Life");
	m_armatureArray.push_back("Anim/Item_GameUI_Nuclear");
	m_armatureArray.push_back("Anim/Item_GameUI_Rampage");
	m_armatureArray.push_back("Anim/Item_GameUI_Shield");
	m_armatureArray.push_back("Anim/Item_GameUI_Wingman");
	m_armatureArray.push_back("Anim/Prop_Secondary_01");
	m_armatureArray.push_back("Anim/Prop_Secondary_02");
	m_armatureArray.push_back("Anim/Prop_Secondary_03");
	m_armatureArray.push_back("Anim/Prop_Secondary_04");
	m_armatureArray.push_back("Anim/Prop_Secondary_05");
	m_armatureArray.push_back("Anim/Prop_Secondary_06");
	m_armatureArray.push_back("Anim/Prop_Shield");

	m_armatureArray.push_back("Anim/Anim_AimEnemy");

	m_iAllProgress += m_armatureArray.size();
	//csb
	////菜单
	//m_uiArray.push_back("Bag.csb");
	////m_uiArray.push_back("BagListItem.csb"); 因需产生多项，这里无需预加载
	//m_uiArray.push_back("Daily.csb");
	//m_uiArray.push_back("Instruction.csb");
	//m_uiArray.push_back("MainUI.csb");
	//m_uiArray.push_back("Mission.csb");
	//m_uiArray.push_back("Pause.csb");
	//m_uiArray.push_back("Relive.csb");
	//m_uiArray.push_back("Retry.csb");
	//m_uiArray.push_back("Reward.csb");
	//m_uiArray.push_back("Settle.csb");
	//m_uiArray.push_back("Setup.csb");
	//m_uiArray.push_back("Shop.csb");
	//m_uiArray.push_back("Strengthen.csb");
	////游戏
	//m_uiArray.push_back("GameUI.csb");

	m_iAllProgress += m_uiArray.size();

	m_iAllProgress += PreloadProgress::GlobalData;
	m_iAllProgress += PreloadProgress::GameData;
	m_iAllProgress += PreloadProgress::AchievementMgr;
	m_iAllProgress += PreloadProgress::Sound;

	m_iTmpProgress = 0;

	//开始载入
	this->notifyStart();

	this->notifyProgress(m_iTmpProgress);

	this->asynLoadingDatabase();
	//this->asynLoadingImage();
	//this->asynLoadingPlist();
	//this->asynLoadingArmature();
	//this->syncLoadingUI();
}

Node* Preload::getUI(const std::string& filename)
{
	DEBUG_LOG("Preload::getUI: %s", filename.c_str());
	return CSLoader::createNode(filename);;

	//cocos2dx-3.8 不支持以下操作。3.4支持
	//auto ui = m_uiMap.find(filename);
	//if (ui != m_uiMap.end())
	//{
	//	return ui->second;
	//}
	//else
	//{
	//	auto csb = CSLoader::createNode(filename);
	//	csb->retain();
	//	m_uiMap.insert(std::map<std::string, Node*>::value_type(filename, csb));

	//	return csb;
	//}
}

cocostudio::timeline::ActionTimeline* Preload::getUIAction(const std::string& filename)
{ 
	return CSLoader::createTimeline(filename);

	//cocos2dx-3.8 不支持以下操作。3.4支持
	//auto ui = m_actionMap.find(filename);
	//if (ui != m_actionMap.end())
	//{
	//	return ui->second;
	//}
	//else
	//{
	//	auto timeLine = CSLoader::createTimeline(filename);
	//	timeLine->retain();
	//	m_actionMap.insert(std::map<std::string, cocostudio::timeline::ActionTimeline*>::value_type(filename, timeLine));

	//	return timeLine;
	//}
}

//*****************************************************
//
//   加载单纯的png，无plist
//
//*****************************************************
void Preload::asynLoadingImage()
{
	//png加入全局cache中
	m_iImageCnt = m_imageArray.size();
	for (unsigned i = 0; i < m_imageArray.size(); i++)
	{
		Director::getInstance()->getTextureCache()->addImageAsync(
			m_imageArray[i],
			CC_CALLBACK_1(Preload::asynLoadingImageDone, this, m_imageArray[i]));
	}
}

void Preload::asynLoadingImageDone(Texture2D* texture, const std::string& filename)
{
	this->notifyProgress(++m_iTmpProgress);
	m_iImageCnt--;

	DEBUG_LOG("Preload::asynLoadingImageDone[%d]: %s", m_iImageCnt, filename.c_str());

	if (0 == m_iImageCnt)
	{
		DEBUG_LOG("asynLoadingImageDone OK");

		m_bImageLoaded = true;
		this->loadingDone(PreloadType::Image);
	}
}

//*****************************************************
//
//  加载png+plist
//
//*****************************************************
void Preload::asynLoadingPlist()
{
	//BlastPool::getInstance()->init();

	//png加入全局cache中
	m_iImagePlistCnt = m_plistArray.size();
	for (unsigned i = 0; i < m_plistArray.size(); i++)
	{
		Director::getInstance()->getTextureCache()->addImageAsync(
			std::string(m_plistArray[i]).append(".png"),
			CC_CALLBACK_1(Preload::asynLoadingPlistDone, this, m_plistArray[i]));
	}
}

void Preload::asynLoadingPlistDone(Texture2D* texture, const std::string& filename)
{
	this->notifyProgress(++m_iTmpProgress);

	std::string file = filename;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(file.append(".plist"), texture);
	m_iImagePlistCnt--;

	DEBUG_LOG("Preload::asynLoadingPlist[%d]: %s", m_iImagePlistCnt, filename.c_str());

	if (0 == m_iImagePlistCnt)
	{
		DEBUG_LOG("asynLoadingPlist OK");

		m_bImagePlistLoaded = true;
		this->loadingDone(PreloadType::Plist);
	}
}

//*****************************************************
//
//   加载骨骼动画
//
//*****************************************************
void Preload::asynLoadingArmatureDone(float dt)
{
	this->notifyProgress(++m_iTmpProgress);

	m_iArmatureCnt++;
	if (m_armatureArray.size() == m_iArmatureCnt)
	{
		m_bArmatureLoaded = true;
		this->loadingDone(PreloadType::Armature);
	}
	else
	{
		asynLoadingArmature();
	}
}

void Preload::asynLoadingArmature()
{
	auto p = m_armatureArray[m_iArmatureCnt];
	DEBUG_LOG("Preload::asynLoadingArmature: %s", p.c_str());
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(
		std::string(p).append("0.png"),
		std::string(p).append("0.plist"),
		std::string(p).append(".ExportJson"),
		this,
		CC_SCHEDULE_SELECTOR(Preload::asynLoadingArmatureDone));
}
//*****************************************************
//
//   加载csb
//
//*****************************************************
void Preload::syncLoadingUI()
{
	//不能在非主线程中调用CSLoader::createNode，否则会导致OpenGL异常

	for (auto file : m_uiArray)
	{
		auto node = Preload::getUI(file);
		node->retain();
		m_uiMap.insert(std::map<std::string, Node*>::value_type(file, node));

		auto timeLine = CSLoader::createTimeline(file);
		timeLine->retain();
		m_actionMap.insert(std::map<std::string, cocostudio::timeline::ActionTimeline*>::value_type(file, timeLine));

		DEBUG_LOG("Preload::syncLoadingUI: %s", file.c_str());
		this->notifyProgress(++m_iTmpProgress);
	}

	m_bUILoaded = true;
	this->loadingDone(PreloadType::Ui);
}

//*****************************************************
//
//   加载数据库、存档数据、成就、音乐
//
//*****************************************************
void Preload::asynLoadingDatabase()
{
	auto loadEnd = [this](void*)
	{
		DEBUG_LOG("asynLoadingDatabase OK");

		m_bOtherLoaded = true;
		this->loadingDone(PreloadType::Other);
	};

	AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, loadEnd, (void*)NULL, [this]()
	{
		if (!GlobalData::getInstance()->initialize(this))
		{
			CCLOG("Initialize globla data failed");
			this->notifyError("Initialize globla data failed");
			return;
		}

		m_iTmpProgress += PreloadProgress::GlobalData;
		this->notifyProgress(m_iTmpProgress);

		if (!GameData::getInstance()->loadData())
		{
			CCLOG("Initialize game data failed");
			this->notifyError("Initialize game data failed");
			return;
		}

		m_iTmpProgress += PreloadProgress::GameData;
		this->notifyProgress(m_iTmpProgress);

		if (!AchievementMgr::getInstance()->init())
		{
			CCLOG("Initialize achievement data failed");
			this->notifyError("Initialize achievement data failed");
			return;
		}

		m_iTmpProgress += PreloadProgress::AchievementMgr;
		this->notifyProgress(m_iTmpProgress);

		Sound::preload(this);

		m_iTmpProgress += PreloadProgress::Sound;
		this->notifyProgress(m_iTmpProgress);
	});
}

void Preload::asynRemoteData()
{
	DEBUG_LOG("asynRemoteData");

	RemoteLogin* pLogin = new RemoteLogin();
	pLogin->autorelease();
	pLogin->retain();
	pLogin->run([this, pLogin](int result){
		DEBUG_LOG("RemoteLogin, result = %d", result);
		if (result == 0)
		{
			m_bRemoteDataSuccess = true;
		}
		else
		{
			//暂时关闭每日签到奖励
			//parameter表中要增加：510	tips	tips_daily	在联网状态下才能领取每日登陆礼包
			if (Utils::str2int(GlobalData::getInstance()->getParameter(GLOBALDATA::DAILY_GIFT)))
			{
				this->notifyWarning(GlobalData::getInstance()->getParameter(PreloadConstant::StringTipsDaily).c_str());
			}
			m_bRemoteDataSuccess = false;
		}

		m_bRemoteDataLoaded = true;
		loadingDone(PreloadType::Remote);

		pLogin->release();
	});

}

void Preload::onGlobalDataProgress(int percent)
{
	this->notifyProgress(m_iTmpProgress + PreloadProgress::GlobalData * percent * 1.f / 100.f);
}

void Preload::onSoundProgress(int percent)
{
	this->notifyProgress(m_iTmpProgress + PreloadProgress::Sound * percent * 1.f / 100.f);
}

void Preload::loadingDone(PreloadType type)
{
	DEBUG_LOG("m_bImageLoaded = %d, m_bImagePlistLoaded = %d, m_bArmatureLoaded = %d, m_bUILoaded = %d, m_bOtherLoaded = %d, m_bRemoteDataLoaded = %d",
		m_bImageLoaded ? 1 : 0,
		m_bImagePlistLoaded ? 1 : 0,
		m_bArmatureLoaded ? 1 : 0,
		m_bUILoaded ? 1 : 0,
		m_bOtherLoaded ? 1 : 0,
		m_bRemoteDataLoaded? 1 : 0);

	switch (type)
	{
	case PreloadType::Other:
		this->asynLoadingImage();
		this->asynRemoteData(); //获取网络数据，需要在同步GameData之后，可以和其他任务并行执行
		break;
	case PreloadType::Image:
		this->asynLoadingPlist();
		break;
	case PreloadType::Plist:
		this->asynLoadingArmature();
		break;
	case PreloadType::Armature:
		this->syncLoadingUI();
		break;
	case PreloadType::Ui:
	case PreloadType::Remote:
		if(m_bUILoaded && m_bRemoteDataLoaded)
		{
			DEBUG_LOG("Preload OK");
			this->notifyEnd(m_errorCode);
		}
		break;

	}
}

void Preload::notifyProgress(int progress)
{
	if (m_pListener)
	{
		DEBUG_LOG("Preload progress: %d", (int)(progress * 100.f / m_iAllProgress));
		m_pListener->onProgress((int)(progress * 100.f / m_iAllProgress));
	}
}

void Preload::notifyWarning(const char* info)
{
	if (m_pListener)
	{
		DEBUG_LOG("Preload warning: %s", info);
		m_pListener->onWarning(info);
	}
}

void Preload::notifyError(const char* info)
{
	if (m_pListener)
	{
		DEBUG_LOG("Preload error: %s", info);
		m_pListener->onError(info);
		m_errorCode = PreloadError::CommonError;
	}
}

void Preload::notifyStart()
{
	if (m_pListener)
	{
		DEBUG_LOG("Preload start");
		m_pListener->onStart();
	}
}

void Preload::notifyEnd(PreloadError result)
{
	if (m_pListener)
	{
		if (result == PreloadError::Success)
		{
			DEBUG_LOG("Preload end, result = Success");
			m_pListener->onEnd(PreloadError::Success);
		}
		else
		{
			if (!m_bRemoteDataSuccess)
			{
				DEBUG_LOG("Preload end, result = RemoteDataError");
				m_pListener->onEnd(PreloadError::RemoteDataError);
			}
			else
			{
				DEBUG_LOG("Preload end, result = CommonError");
				m_pListener->onEnd(PreloadError::CommonError);
			}
		}
	}
}