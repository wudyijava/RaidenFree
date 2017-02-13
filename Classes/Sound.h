#pragma once

#include "cocos2d.h"

USING_NS_CC;

struct TMusicData
{
	int id;
	std::string filename;
	int isEffect;
};

class SoundLoadListener
{
public:
	virtual void onSoundProgress(int percent) = 0;
};

namespace SOUND
{
	const int ID_WARNING = 101;
	const int ID_FLY_AWAY = 102;
	const int ID_READY_GO = 103;
	const int ID_SCORE = 104;
	const int ID_CLICK = 105;
	const int ID_BULLET_LEVEL_MAX = 106;
	const int ID_MENU_GB = 107;
	const int ID_BULLET_SHIELD = 108;
	const int ID_COUNT_DOWN = 109;
	const int ID_SCORE_STAR = 110;

	const int ID_BOOM_L = 51;//长
	const int ID_BOOM_M = 52;//中
	const int ID_BOOM_S = 53;//短
}

class Sound
{
public:

	//预载入
	static void preload(SoundLoadListener* pLisiener = nullptr);

	//播放
	static void playEffect(const std::string& filename, bool loop = false);
	static void playMusic(const std::string& filename, bool loop = false);
	static void playSound(int id, bool loop = false);

	//停止
	//static void stopMusic();
	//static void stopEffect(int musicId);
	//static void stopEffect(const std::string& filename);
	//static void stopAllEffects();
	static void stopAll();

	static void pause();
	static void resume();

	//音量
	static void setVolume(float volume);
	static float getVolume();

	//判断
	static bool isPlayMusic();

	//控制子弹音效。在非游戏中无需播放子弹音效
	static void turnOffBulletSound();
	static void turnOnBulletSound();

	static bool isBulletSound();

protected:
	static const std::string getFullPath(const std::string& filename);
	static void preloadBackgroundMusic(const std::string& filename);
	static void preloadEffect(const std::string& filename);

private:
	static bool m_bBulletSound;
	static std::map<std::string , unsigned int> m_musicIdMap;

	static std::string m_curMusic;
	static int m_curMusicId;
};