#include "Setting.h"
#include "GameData.h"
#include "Sound.h"
#include "GlobalData.h"
#include "Preload.h"
#include "Reward.h"
#include "PromptLayer.h"
#include "Carrier.h"
#include "Equipment.h"
#include "Bmob/ExchangeGift.h"
#include "Common/Macro.h"

USING_NS_TIMELINE

SettingLayer::SettingLayer()
: m_pRootNode(nullptr)
, m_pCbSound(nullptr)
, m_pBtnClose(nullptr)

, m_pBtnSetup(nullptr)
, m_pBtnHelp(nullptr)
, m_pBtnAbout(nullptr)

, m_pLableVersion(nullptr)
, m_pBtnExchange(nullptr)

, m_pNodeSetup(nullptr)
, m_pNodeAbout(nullptr)
, m_pNodeHelp(nullptr)

, m_pLableUserId(nullptr)
, m_pExchangeGift(nullptr)
{
}

SettingLayer::~SettingLayer()
{

}

bool SettingLayer::doInit()
{
	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("Setup.csb");
	this->addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("Setup.csb");;
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, false);

	//控件
	FIND_UI_CONTROL_RE(ui::CheckBox*, "Sound_CheckBox", m_pCbSound, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Back_Btn", m_pBtnClose, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Setup_Btn", m_pBtnSetup, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Help_Btn", m_pBtnHelp, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "About_Btn", m_pBtnAbout, m_pRootNode);

	
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "TxtUserId", m_pLableUserId, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "TxtVersion", m_pLableVersion, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Exchange_Btn", m_pBtnExchange, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Exchange_Btn", m_pBtnExchange, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextField*, "Code_Txt", m_pExchangeCode, m_pRootNode);
	
	FIND_UI_CONTROL_RE(Node*, "Setup", m_pNodeSetup, m_pRootNode);
	FIND_UI_CONTROL_RE(Node*, "About", m_pNodeAbout, m_pRootNode);
	FIND_UI_CONTROL_RE(Node*, "Help", m_pNodeHelp, m_pRootNode);

	m_pNodeSetup->setVisible(true);
	m_pNodeAbout->setVisible(false);
	m_pNodeHelp->setVisible(false);
	m_pExchangeCode->setString("");
	m_pLableUserId->setString(GameData::getInstance()->getValue(GAMEDATA::USER_ID));

	((Label*)m_pLableVersion->getVirtualRenderer())->setMaxLineWidth(400.f);
	m_pLableVersion->setString(GlobalData::getInstance()->getVersionName());

	if (GameData::getInstance()->getValueToInt(GAMEDATA::SOUND))
	{
		m_pCbSound->setSelected(true);
	}
	else
	{
		m_pCbSound->setSelected(false);
	}

	m_pCbSound->addEventListener([](Ref* pSender, ui::CheckBox::EventType type)
	{
		if (type == ui::CheckBox::EventType::SELECTED)
		{
			GameData::getInstance()->setValue(GAMEDATA::SOUND, 1);
			Sound::resume();
		}
		else
		{
			GameData::getInstance()->setValue(GAMEDATA::SOUND, 0);
			Sound::pause();
		}
	});

	//控件事件
	Utils::addButtonTouchEffect(m_pBtnClose, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		removeFromParent();
	});
	
	Utils::addButtonTouchEffect(m_pBtnSetup, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		m_pNodeSetup->setVisible(true);
		m_pNodeAbout->setVisible(false);
		m_pNodeHelp->setVisible(false);

	});

	Utils::addButtonTouchEffect(m_pBtnHelp, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		m_pNodeSetup->setVisible(false);
		m_pNodeAbout->setVisible(false);
		m_pNodeHelp->setVisible(true);
	});

	Utils::addButtonTouchEffect(m_pBtnAbout, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		m_pNodeSetup->setVisible(false);
		m_pNodeAbout->setVisible(true);
		m_pNodeHelp->setVisible(false);

	});

	Utils::addButtonTouchEffect(m_pBtnExchange, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		const std::string exchangeCode = m_pExchangeCode->getString();
		if (exchangeCode.length() == 0)
		{
			return;
		}

		std::vector<std::string> arr;
		Utils::splite(exchangeCode, " ", arr);
		if (arr.size() == 2 && arr[0] == "**l##")
		{
			int l = Utils::str2int(arr[1]);
			if (l > GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_FINISH))
			{
				GameData::getInstance()->setLevelFinish(l);
				GameData::getInstance()->setLevelMax(l + 1);
				GameData::getInstance()->setValue(GAMEDATA::LEVEL, l + 1);

				Sound::playSound(SOUND::ID_BULLET_SHIELD);

				return;
			}
		}
		else if(arr.size() == 3 && arr[0] == "**p##")
		{
			int e = Utils::str2int(arr[1]);
			int p = Utils::str2int(arr[2]);
			auto pEquipment = GlobalData::getInstance()->getEquipment(e);
			if (pEquipment)
			{
				pEquipment->pick += p;
				Sound::playSound(SOUND::ID_BULLET_SHIELD);
				return;
			}
		}

		Sound::playSound(SOUND::ID_CLICK);

		m_giftMap.clear();
		m_pExchangeGift = new ExchangeGift;
		m_pExchangeGift->autorelease();
		m_pExchangeGift->retain();
		m_pExchangeGift->exchange(exchangeCode, &m_giftMap, [this](int errorCode){
		
			if (!m_pPromptLayer)
			{
				m_pPromptLayer = new PromptLayer;
				m_pPromptLayer->init();
				m_pPromptLayer->autorelease();
				m_pPromptLayer->setOnExitCallback([this](){
					m_pPromptLayer = nullptr;
				});
				this->getParent()->addChild(m_pPromptLayer);
			}
			m_pPromptLayer->enableTouch(true);

			if (m_giftMap.size() > 0 && ExchangeGiftConstant::ErrCodeSuccess == errorCode)
			{
				//TODO  显示玩家获得的礼品内容
				std::string  prompt = GlobalData::getInstance()->getParameter(GLOBALDATA::STRING_DAY_GIFT_PROMPT);

				bool bFlag = false;
				for (auto it : m_giftMap)
				{
					if (!bFlag)
					{
						char buf[21] = { 0 };
						snprintf(buf, 20, "%d", it.second);
						prompt.append(GlobalData::getInstance()->getRewardData(it.first.c_str())->name).append("x").append(buf);
						bFlag = true;
					}
					else
					{
						char buf[21] = { 0 };
						snprintf(buf, 20, "%d", it.second);
						prompt.append(",").append(GlobalData::getInstance()->getRewardData(it.first.c_str())->name).append("x").append(buf);
					}

					GameData::getInstance()->incValue(it.first.c_str(), it.second);
				}

				m_pPromptLayer->setPrompt(prompt);
				GameData::getInstance()->saveData();
			}
			else
			{
				//TODO 提示错误信息
				DEBUG_LOG("Query exchange code failed, error code = %d", errorCode);
				switch (errorCode)
				{
				case ExchangeGiftConstant::ErrCodeGiftReceived: //兑换码已使用过
					if (m_pPromptLayer)
					{
						m_pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(ExchangeGiftConstant::StrExchangeCodeUsed));
					}
					break;
				case ExchangeGiftConstant::ErrCodeInvalidExchangeCode: //无效的兑换码
					if (m_pPromptLayer)
					{
						m_pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(ExchangeGiftConstant::StrInvalidExchangeCode));
					}
					break;
				case ExchangeGiftConstant::ErrCodeQueryExchangeCode: //解析兑换码表错误
					if (m_pPromptLayer)
					{
						m_pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(ExchangeGiftConstant::StrErroeQueryExchangeCode));
					}
					break;
				default:
					if (m_pPromptLayer)
					{
						m_pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(ExchangeGiftConstant::StrUnknowError));
					}
					break;
				}
			}
		
			m_pExchangeGift->release();
		});

		//TODO 弹出正在查询兑换码的对话框
		m_pPromptLayer = new PromptLayer;
		m_pPromptLayer->enableTouch(false);
		m_pPromptLayer->setPrompt(GlobalData::getInstance()->getParameter(ExchangeGiftConstant::StrQueryExchangeCode));
		m_pPromptLayer->init();
		m_pPromptLayer->autorelease();
		m_pPromptLayer->setOnExitCallback([this](){
			m_pPromptLayer = nullptr;
		});
		getParent()->addChild(m_pPromptLayer);
	});

	//按返回键退出
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == key
			|| EventKeyboard::KeyCode::KEY_ESCAPE == key
			|| EventKeyboard::KeyCode::KEY_BACKSPACE == key)
		{
			Sound::playSound(SOUND::ID_CLICK);
			removeFromParent();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

