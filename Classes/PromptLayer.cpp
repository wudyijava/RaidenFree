#include "PromptLayer.h"
#include "MenuMainUI.h"
#include "GlobalData.h"
#include "Preload.h"
#include "GameData.h"
#include "Common/Macro.h"

USING_NS_TIMELINE

namespace PromptLayerNodeName
{
	const char* const StrLabelPrompt = "Prompt_Text";
}

PromptLayer::PromptLayer()
: m_pRootNode(nullptr)
, m_pTextPrompt(nullptr)
, m_touchEnable(true)
{

}

PromptLayer::~PromptLayer()
{

}

void PromptLayer::setPrompt(const std::string& prompt)
{
	m_prompt = prompt;
	if (m_pTextPrompt)
	{
		m_pTextPrompt->setString(m_prompt);
	}
}

void PromptLayer::enableTouch(bool enable)
{
	m_touchEnable = enable;
}

bool PromptLayer::doInit()
{
	DEBUG_LOG("PromptLayer::doInit");

	m_pRootNode = Preload::getInstance()->getUI("BeautyGuide.csb");

	this->addChild(m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Text*, PromptLayerNodeName::StrLabelPrompt, m_pTextPrompt, m_pRootNode);

	//m_pRootNode->setOpacity(100);
	//FadeTo* pFade = FadeTo::create(0.5f, 255);
	//FiniteTimeAction* pDone = CallFunc::create([this](){
	//	//¼ÓÔØ¶¯»­£º 
	//	ActionTimeline *action = CSLoader::createTimeline("BeautyGuide.csb");
	//	m_pRootNode->runAction(action);
	//	action->gotoFrameAndPlay(0, false);
	//});
	//Sequence* pSeq = Sequence::create(pFade, pDone, nullptr);
	//m_pRootNode->runAction(pSeq);

	m_pTextPrompt->ignoreContentAdaptWithSize(false);
	m_pTextPrompt->setTextAreaSize(Size(480, 150));
	m_pTextPrompt->setString(m_prompt);

	ActionTimeline *action = CSLoader::createTimeline("BeautyGuide.csb");
	m_pRootNode->runAction(action);
	action->gotoFrameAndPlay(0, false);

	return true;
}

void PromptLayer::onTouchEnded(Touch *touch, Event *event)
{
	DEBUG_LOG("Go to scene MenuMainUI");

	if (m_touchEnable)
	{
		removeFromParent();
	}
}


