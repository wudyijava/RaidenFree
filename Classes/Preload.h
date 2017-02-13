#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "Sound.h"
#include "GlobalData.h"

USING_NS_CC;

enum PreloadError
{
	Success,
	CommonError,
	RemoteDataError,
};

enum PreloadType
{
	Image = 0,
	Plist,
	Armature,
	Ui,
	Other,
	Remote
};

class PreloadListener
{
public:
	virtual void onStart() = 0;
	virtual void onProgress(int percent) = 0;
	virtual void onError(const char* info) = 0;
	virtual void onWarning(const char* info) = 0;
	virtual void onEnd(PreloadError errorCode) = 0;
};

class Preload : public Ref, public GlobalDataLoadListener, public SoundLoadListener
{
public:
	static Preload* getInstance();

	void start(PreloadListener* listener);

	Node* getUI(const std::string& filename);
	cocostudio::timeline::ActionTimeline* getUIAction(const std::string& filename);

	void onGlobalDataProgress(int percent) override;
	virtual void onSoundProgress(int percent) override;

protected:
	Preload();
	virtual ~Preload();

private:
	//������ͨͼƬ
	void asynLoadingImage();
	void asynLoadingImageDone(Texture2D* texture, const std::string& filename);

	//����plist�ϳ�ͼƬ
	void asynLoadingPlist();
	void asynLoadingPlistDone(Texture2D* texture, const std::string& filename);

	//���ع�������
	void asynLoadingArmature();
	void asynLoadingArmatureDone(float dt);

	//ͬ������csb
	void syncLoadingUI();

	//��������
	void asynLoadingDatabase();

	//��ȡbmob����
	void asynRemoteData();

	void loadingDone(PreloadType type);

	void notifyProgress(int progress);
	void notifyError(const char* info);
	void notifyWarning(const char* info);
	void notifyStart();
	void notifyEnd(PreloadError result);

private:
	static Preload* m_pInstance;

	PreloadListener* m_pListener;
	int m_iAllProgress;
	int m_iTmpProgress;

	int m_iImageCnt;
	int m_iImagePlistCnt;
	int m_iArmatureCnt;

	bool m_bImageLoaded;
	bool m_bImagePlistLoaded;
	bool m_bArmatureLoaded;
	bool m_bUILoaded;
	bool m_bOtherLoaded;
	bool m_bRemoteDataLoaded;  //Զ�������Ƿ��������

	bool m_bRemoteDataSuccess; //Զ�������Ƿ�����ɹ�

	std::vector<std::string> m_imageArray;
	std::vector<std::string> m_plistArray;
	std::vector<std::string> m_armatureArray;
	std::vector<std::string> m_uiArray;

	std::map<std::string, Node*> m_uiMap;
	std::map<std::string, cocostudio::timeline::ActionTimeline*> m_actionMap;

	PreloadError m_errorCode;
};