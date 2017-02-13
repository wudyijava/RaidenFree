#include "Sound.h"
#include "SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"
#include "GlobalData.h"
#include "GameData.h"
#include "Common/Macro.h"

using namespace experimental;

bool Sound::m_bBulletSound = true;
std::map<std::string, unsigned int> Sound::m_musicIdMap;
std::string Sound::m_curMusic;
int Sound::m_curMusicId = AudioEngine::INVALID_AUDIO_ID;

void Sound::preload(SoundLoadListener* pLisiener)
{
	const std::vector<TMusicData*>& musicArray = GlobalData::getInstance()->getAllMusicData();

	float total = musicArray.size() / 100.f;
	int step = 0;
	for (auto m : musicArray)
	{
		if (m->filename.length() <= 0)
		{
			continue;
		}

		if (m->isEffect == 1)
		{
			Sound::preloadEffect(m->filename);
		}
		else
		{
			Sound::preloadBackgroundMusic(m->filename);
		}

		DEBUG_LOG("Load sound: %s", m->filename.c_str());
		pLisiener->onSoundProgress(++step / total);
	}
}

void Sound::preloadBackgroundMusic(const std::string& filename)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(Sound::getFullPath(filename).c_str());
#else

	const std::string& path = Sound::getFullPath(filename);
	AudioEngine::preload(path, [path](bool isSuccess){
		if (!isSuccess)
		{
			DEBUG_LOG("Load fail: %s", path.c_str());
		}
	});
#endif
}

void Sound::preloadEffect(const std::string& filename)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(Sound::getFullPath(filename).c_str());
#else

	const std::string& path = Sound::getFullPath(filename);
	AudioEngine::preload(path, [path](bool isSuccess){
		if (!isSuccess)
		{
			DEBUG_LOG("Load fail: %s", path.c_str());
		}
	});
#endif

}

void Sound::playSound(int id, bool loop)
{
	if (id <= 0 || !GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
		return;
	}

	const TMusicData* data = GlobalData::getInstance()->getMusic(id);
	if (!data)
	{
		CCASSERT(data, "Sound::playSound : data is NULL");
		return;
	}

	if (data->isEffect)
	{
		Sound::playEffect(data->filename, loop);
	}
	else
	{
		Sound::playMusic(data->filename, loop);
	}
}

void Sound::playMusic(const std::string& filename, bool loop)
{
	if (filename.length() <= 0 || !GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
		return;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
#else
	if (m_curMusicId != AudioEngine::INVALID_AUDIO_ID) //正在播放背景音乐
#endif
	{
		if (m_curMusic == filename)
		{
			return;
		}
		else
		{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#else
			AudioEngine::stop(m_curMusicId);
			auto it = m_musicIdMap.find(m_curMusic);
			if (it != m_musicIdMap.end())
			{
				m_musicIdMap.erase(it);
			}
#endif
		}
	}
	m_curMusic = filename;

	unsigned int location = filename.find_last_of('.');
	if (location != std::string::npos)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), loop);
#else
		m_curMusicId = AudioEngine::play2d(filename, loop);
#endif
	}
	else
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(getFullPath(filename).c_str(), loop);
#else
		m_curMusicId = AudioEngine::play2d(getFullPath(filename), loop);
#endif
	}

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
	if (m_curMusicId != AudioEngine::INVALID_AUDIO_ID)
	{
		m_musicIdMap.insert(std::map<std::string, unsigned int>::value_type(filename, m_curMusicId));
	}
#endif

	if (!GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
		Sound::pause();
	}
}

void Sound::playEffect(const std::string& filename, bool loop)
{
	if (filename.length() <= 0 || !GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
		return;
	}

	int ret = AudioEngine::INVALID_AUDIO_ID;
	unsigned int location = filename.find_last_of('.');
	if (location != std::string::npos)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		ret = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(filename.c_str(), loop);
#else
		ret = AudioEngine::play2d(filename, loop);
#endif
	}
	else
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		ret = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(getFullPath(filename).c_str(), loop);
#else
		ret = AudioEngine::play2d(getFullPath(filename), loop);
#endif
	}

	m_musicIdMap.insert(std::map<std::string, unsigned int>::value_type(filename, ret));
}

void Sound::stopAll()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#else
	AudioEngine::stopAll();
	m_curMusicId = AudioEngine::INVALID_AUDIO_ID;
#endif
	m_curMusic = "";

	m_musicIdMap.clear();
}

void Sound::pause()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
#else
	AudioEngine::pauseAll();
#endif
}

void Sound::resume()
{
	if (GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
#else
		AudioEngine::resumeAll();
#endif
	}
}

void Sound::setVolume(float volume)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
#else
	for (auto it : m_musicIdMap)
	{
		AudioEngine::setVolume(it.second, volume);
	}
#endif
}

float Sound::getVolume()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
#else

	for (auto it : m_musicIdMap)
	{
		return AudioEngine::getVolume(it.second);
	}

	return 1.0f;
#endif
}

bool Sound::isPlayMusic()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
#else

	
	if (m_curMusicId != AudioEngine::INVALID_AUDIO_ID
		&& AudioEngine::AudioState::PLAYING == AudioEngine::getState(m_curMusicId))
	{
		return true;
	}
	else
	{
		return false;
	}
#endif
}

void Sound::turnOffBulletSound()
{
	m_bBulletSound = false;
}

void Sound::turnOnBulletSound()
{
	m_bBulletSound = true;
}

bool Sound::isBulletSound()
{
	return 	m_bBulletSound;
}

const std::string Sound::getFullPath(const std::string& filename)
{
	unsigned int location = filename.find_last_of('.');
	if (location != std::string::npos)
	{
		return filename;
	}
	else
	{
		std::string path = "sound/";

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		//win32平台洗无法播放ogg音频，所以用mp3代替
		path.append("ogg/").append(filename).append(".ogg");
#else
		path.append("ogg/").append(filename).append(".ogg");
#endif
		
		return path;
	}
}
