#include "Preload.h"
#include "Common/Macro.h"
#include "Dialog.h"

namespace DialogConstant
{
	const std::string LayerName = "Dialog";

	const int PositionOne = 1;
	const int PositionTwo = 2;
};

Dialog::Dialog()
 : m_pRootNode(nullptr)
 , m_nextDialog(false)
{

}

Dialog::~Dialog()
{

}

Layer* Dialog::show(Node* pParent, const TDialogData* pData, bool nextDialog)
{
	auto pLayer = dynamic_cast<Dialog*>(pParent->getChildByName(DialogConstant::LayerName));
	if (!pLayer)
	{
		pLayer = Dialog::create();
		pLayer->setName(DialogConstant::LayerName);
		pParent->addChild(pLayer);
	}
	
	pLayer->showDialog(pData, nextDialog);

	return pLayer;
}

bool Dialog::doInit()
{
	m_pRootNode = Preload::getInstance()->getUI("Dialog.csb");
	this->addChild(m_pRootNode);

	DialogBoxItem item1;
	FIND_UI_CONTROL_RE(Sprite*, "Dialogbox_Bg_1", item1.box, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Avatar", item1.pic, item1.box);
	FIND_UI_CONTROL_RE(ui::Text*, "Text_Name", item1.name, item1.box);
	FIND_UI_CONTROL_RE(ui::Text*, "Text_Content", item1.content, item1.box);
	item1.box->setVisible(false);
	m_boxArray.push_back(item1);
	item1.content->setTextAreaSize(Size(360, 110));
	item1.content->ignoreContentAdaptWithSize(false);

	DialogBoxItem item2;
	FIND_UI_CONTROL_RE(Sprite*, "Dialogbox_Bg_2", item2.box, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Avatar", item2.pic, item2.box);
	FIND_UI_CONTROL_RE(ui::Text*, "Text_Name", item2.name, item2.box);
	FIND_UI_CONTROL_RE(ui::Text*, "Text_Content", item2.content, item2.box);
	item2.box->setVisible(false);
	m_boxArray.push_back(item2);
	item2.content->setTextAreaSize(Size(360, 110));
	item2.content->ignoreContentAdaptWithSize(false);

	return true;
}

void Dialog::showDialog(const TDialogData* pData, bool nextDialog)
{
	DialogBoxItem* item = nullptr;
	if (DialogConstant::PositionOne == pData->type && m_boxArray.size() > 0)
	{
		item = &m_boxArray.at(0);
	}
	else if (DialogConstant::PositionTwo == pData->type && m_boxArray.size() > 1)
	{
		item = &m_boxArray.at(1);
	}

	if (item->box->isVisible())
	{
		item->pic->setSpriteFrame(pData->pic.c_str());
		item->name->setString(pData->name);
		item->content->setString(pData->content);
	}
	else
	{
		item->box->setVisible(true);
		item->pic->setSpriteFrame(pData->pic.c_str());
		item->name->setString(pData->name);
		item->content->setString(pData->content);

		item->box->setOpacity(100);
		FadeTo* pFade = FadeTo::create(0.3f, 255);
		item->box->runAction(pFade);
	}

	m_nextDialog = nextDialog;
}

void Dialog::onTouchEnded(Touch* touch, Event* event)
{
	if (!m_nextDialog)
	{
		FadeTo* pFade = FadeTo::create(0.3f, 100);
		FiniteTimeAction* pDone = CallFunc::create([this](){
			removeFromParent();
		});
		Sequence* pSeq = Sequence::create(pFade, pDone, nullptr);
		runAction(pSeq);
	}
	else
	{
		if (_onExitCallback)
			_onExitCallback();
	}
}
