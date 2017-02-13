#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"

USING_NS_CC;

class PromptLayer : public PopupLayer
{
public:
	PromptLayer();
	~PromptLayer();

	virtual bool doInit() override;
	CREATE_FUNC(PromptLayer);

	virtual void onTouchEnded(Touch *touch, Event *event) override;
	virtual std::function<void(Touch*, Event*)> getTouchEndedFunc() override 
	{ 
		return CC_CALLBACK_2(PromptLayer::onTouchEnded, this);
	}

	void setPrompt(const std::string& prompt);
	void enableTouch(bool enable);

private:
	Node *m_pRootNode;

	std::string m_prompt;
	ui::Text* m_pTextPrompt;
	bool m_touchEnable;
};