#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"

USING_NS_CC;

class GeneralDialogOneButtonLayer : public PopupLayer
{
public:
	GeneralDialogOneButtonLayer();

	virtual bool doInit() override;
	CREATE_FUNC(GeneralDialogOneButtonLayer);

	void setTitle(const std::string& title, float scale = 1.0f);
	void setOkCallback(const std::function<void()>& cb);

protected:
	std::function<void()> m_okCallback;
	std::string m_title;
	float m_scale;

	ui::TextBMFont* m_pTxtTitle;
	ui::Button* m_pBtnOk;
};

class GeneralDialogTwoButtonLayer : public GeneralDialogOneButtonLayer
{
public:
	GeneralDialogTwoButtonLayer();

	virtual bool doInit() override;
	CREATE_FUNC(GeneralDialogTwoButtonLayer);

	void setCancelCallback(const std::function<void()>& cb);

protected:
	std::function<void()> m_cancelCallback;

	ui::Button* m_pBtnCancel;
};

class GeneralDialogCreator
{
public:
	//确定、取消
	static PopupLayer* restartGame();
	static PopupLayer* restartRapidMode();
	static PopupLayer* exitGame();
	static PopupLayer* backToMainMenu();

	//确定
	static PopupLayer* missionForbidden();
	static PopupLayer* difficulty2Forbidden();
	static PopupLayer* difficulty3Forbidden();
	static PopupLayer* vipNext();
};