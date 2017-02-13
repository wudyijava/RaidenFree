#include "GeneralDialog.h"
#include "Sound.h"
#include "Preload.h"
#include "GlobalData.h"
#include "GameData.h"
#include "Common/Macro.h"

#include "PlaneLayer.h"
#include "GameLayer.h"
#include "CountDown.h"
#include "MenuMainUI.h"

USING_NS_TIMELINE

//////////////////////////////////////////////
//
//   �Ի���ֻ��һ��ȷ����ť
//
//////////////////////////////////////////////

GeneralDialogOneButtonLayer::GeneralDialogOneButtonLayer()
    : m_okCallback(nullptr)
	, m_pTxtTitle(nullptr)
	, m_pBtnOk(nullptr)
{

}

void GeneralDialogOneButtonLayer::setTitle(const std::string& title, float scale)
{
	m_title = title;
	m_scale = scale;
}

void GeneralDialogOneButtonLayer::setOkCallback(const std::function<void()>& cb)
{
	m_okCallback = cb;
}

bool GeneralDialogOneButtonLayer::doInit()
{
	Node *pRootNode = NULL;
	pRootNode = Preload::getInstance()->getUI("GeneralDialogOneBtn.csb");

	pRootNode->setOpacity(0);
	pRootNode->setScale(0.85f);
	FadeTo* pFade = FadeTo::create(.1f, 255.f);
	ScaleTo* pSclae = ScaleTo::create(.1f, 1.0f);
	Spawn* pSpawn = Spawn::create(pFade, pSclae, NULL);
	pRootNode->runAction(pSpawn);

	this->addChild(pRootNode);
	pRootNode->setPosition(Director::getInstance()->getWinSize() / 2);

	Node* pBg = pRootNode->getChildByName("SpriteBg");

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Title_Txt", m_pTxtTitle, pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Ok_Btn", m_pBtnOk, pRootNode);

	m_pTxtTitle->setString(m_title);
	m_pTxtTitle->setScale(m_scale);

	//�ؼ��¼�
	addButtonTouchEffect(m_pBtnOk, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_okCallback)
		{
			Sound::playSound(SOUND::ID_CLICK);
			m_okCallback();
		}
	});

	return true;
}

//////////////////////////////////////////////
//
//   �Ի���ȷ����ȡ��������ť
//
//////////////////////////////////////////////

GeneralDialogTwoButtonLayer::GeneralDialogTwoButtonLayer()
: m_cancelCallback(nullptr)
, m_pBtnCancel(nullptr)
{

}

void GeneralDialogTwoButtonLayer::setCancelCallback(const std::function<void()>& cb)
{
	m_cancelCallback = cb;
}

bool GeneralDialogTwoButtonLayer::doInit()
{
	Node *pRootNode = NULL;
	pRootNode = Preload::getInstance()->getUI("GeneralDialog.csb");

	pRootNode->setOpacity(0);
	pRootNode->setScale(0.85f);
	FadeTo* pFade = FadeTo::create(.1f, 255.f);
	ScaleTo* pSclae = ScaleTo::create(.1f, 1.0f);
	Spawn* pSpawn = Spawn::create(pFade, pSclae, NULL);
	pRootNode->runAction(pSpawn);

	this->addChild(pRootNode);
	pRootNode->setPosition(Director::getInstance()->getWinSize() / 2);

	Node* pBg = pRootNode->getChildByName("SpriteBg");

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Title_Txt", m_pTxtTitle, pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Ok_Btn", m_pBtnOk, pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Cancel_Btn", m_pBtnCancel, pRootNode);

	m_pTxtTitle->setString(m_title);
	m_pTxtTitle->setScale(m_scale);

	//�ؼ��¼�
	addButtonTouchEffect(m_pBtnOk, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_okCallback)
		{
			Sound::playSound(SOUND::ID_CLICK);
			m_okCallback();
		}
	});

	addButtonTouchEffect(m_pBtnCancel, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_cancelCallback)
		{
			Sound::playSound(SOUND::ID_CLICK);
			m_cancelCallback();
		}
	});

	return true;
}
//////////////////////////////////////////////
//
//   ʵ�ֶ��ֶԻ���
//
//////////////////////////////////////////////
namespace GeneralDialogCreatorConstant
{
	const char* const STRING_EXIT_GAME = "dialog_exit_game";
	const char* const STRING_BACK_TO_MAIN_MENU_GAME = "dialog_back_to_main_menu";
	const char* const STRING_RESTRART_GAME = "dialog_restart_game";
	const char* const STRING_MISSION_BORBIDDEN = "dialog_mission_forbidden";
	const char* const STRING_DIFFICULTY2_BORBIDDEN = "dialog_difficulty2_forbidden";
	const char* const STRING_DIFFICULTY3_BORBIDDEN = "dialog_difficulty3_forbidden";
	const char* const STRING_NEXT_VIP = "dialog_next_vip";
}

//////////////////////////////////////////////
//
//   ���汾��
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::restartGame()
{
	GeneralDialogTwoButtonLayer* layer = new GeneralDialogTwoButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_RESTRART_GAME));
	
	layer->setOkCallback([layer](){
		PlaneLayer::pushCommand(ECommand::CommandRestartGame);
		layer->resumePlaneLayer();
		layer->removeFromParent();
	});

	layer->setCancelCallback([layer](){
		PlaneLayer::pushCommand(ECommand::CommandBackToMainMenu);
		layer->resumePlaneLayer();
		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   ���漱��ģʽ
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::restartRapidMode()
{
	GeneralDialogTwoButtonLayer* layer = new GeneralDialogTwoButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_RESTRART_GAME));

	layer->setOkCallback([layer](){
		PlaneLayer::pushCommand(ECommand::CommandRestartGame);
		layer->resumePlaneLayer();
		layer->removeFromParent();
	});

	layer->setCancelCallback([layer](){
		Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   �˳���Ϸ
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::exitGame()
{
	GeneralDialogTwoButtonLayer* layer = new GeneralDialogTwoButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_EXIT_GAME));

	layer->setOkCallback([layer](){
		GameData::getInstance()->saveData();

		Director::getInstance()->end();
	});

	layer->setCancelCallback([layer](){

		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   �������˵�
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::backToMainMenu()
{
	GeneralDialogTwoButtonLayer* layer = new GeneralDialogTwoButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_BACK_TO_MAIN_MENU_GAME));

	layer->setOkCallback([layer](){

		PlaneLayer::pushCommand(ECommand::CommandBackToMainMenu);

		layer->resumePlaneLayer();
		layer->removeFromParent();
	});

	layer->setCancelCallback([layer](){

		auto layer2 = CountDownLayer::create();
		layer2->setPlaneLayer(layer->getPlaneLayer());

		layer->getParent()->addChild(layer2);
		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   �ؿ�δ����
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::missionForbidden()
{
	GeneralDialogOneButtonLayer* layer = new GeneralDialogOneButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_MISSION_BORBIDDEN));

	layer->setOkCallback([layer](){
		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   ͨ����ͨ�Ѷȵ�9�ؿ���
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::difficulty2Forbidden()
{
	GeneralDialogOneButtonLayer* layer = new GeneralDialogOneButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_DIFFICULTY2_BORBIDDEN));

	layer->setOkCallback([layer](){
		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   ͨ����ͨ�Ѷȵ�16�ؿ���
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::difficulty3Forbidden()
{
	GeneralDialogOneButtonLayer* layer = new GeneralDialogOneButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_DIFFICULTY3_BORBIDDEN), 0.9f);

	layer->setOkCallback([layer](){
		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}

//////////////////////////////////////////////
//
//   ���ؽ����û�����vip����ʾ�û����´�ս��ʤ�����ɻ��VIP���⽱��
//
//////////////////////////////////////////////
PopupLayer* GeneralDialogCreator::vipNext()
{
	GeneralDialogOneButtonLayer* layer = new GeneralDialogOneButtonLayer();

	layer->setTitle(GlobalData::getInstance()->getParameter(GeneralDialogCreatorConstant::STRING_NEXT_VIP), 0.6f);

	layer->setOkCallback([layer](){
		layer->removeFromParent();
	});

	if (layer->init())
	{
		return layer;
	}
	else
	{
		return nullptr;
	}
}
