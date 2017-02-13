#pragma once

#include "cocos2d.h"
#include "Preload.h"

USING_NS_CC;

class LoadingLayer :
	public Layer, public PreloadListener
{
public:
	static Scene* scene();

	LoadingLayer();
	virtual ~LoadingLayer();

	virtual bool init();
	virtual void update(float dt) override;

	CREATE_FUNC(LoadingLayer);

	void initUI();
	void ToMainMenu();

	virtual void onStart() override;
	virtual void onProgress(int percent) override;
	virtual void onError(const char* info) override;
	virtual void onWarning(const char* info) override;
	virtual void onEnd(PreloadError errorCode) override;

private:
	Node* m_pRootNode;

	Sprite* m_pNeedle;
	ui::LoadingBar* m_pLoadingBar;
	ui::Text* m_pTxtErrorInfo;

	long m_iBeginTime;
	long m_iEndTime;

	int m_iStart;
};
