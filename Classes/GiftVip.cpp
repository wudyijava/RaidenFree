#include "GiftVip.h"
#include "GameData.h"
#include "Sound.h"
#include "CountDown.h"
#include "PlaneLayer.h"
#include "GlobalData.h"
#include "Goods.h"
#include "Carrier.h"
#include "GameLayer.h"
#include "Common/Macro.h"
#include "Preload.h"
#include "Bmob/BmobUtils.h"

USING_NS_TIMELINE

namespace GiftVipLayerConstant
{
	const char* const StringVipTimeLeft1 = "vip_time_left_1";
	const char* const StringVipTimeLeft2 = "vip_time_left_2";
	const char* const StringVipTimeLeft3 = "vip_time_left_3";
	const char* const StringVipTimeLeft4 = "vip_time_left_4";

	const char* const StringVipOffline = "vip_offline";
}

GiftVipLayer::GiftVipLayer()
: m_pRootNode(nullptr)

, m_pBuySuccess(nullptr)
, m_pBuyFail(nullptr)

, m_pBtnBuy(nullptr)
, m_pBtnBuy2(nullptr)
, m_pBtnCancel(nullptr)

, m_pSpriteLogo(nullptr)
, m_pSpriteLogoDisable(nullptr)
, m_pSpriteLogoDisableTxt1(nullptr)
, m_pSpriteLogoDisableTxt2(nullptr)

, m_pTxtPriceNum1(nullptr)
, m_pTxtPriceNum2(nullptr)

, m_pTxtVipTimeLeft(nullptr)
{

}

bool GiftVipLayer::init()
{
	if (!PopupLayer::init())
	{
		return false;
	}

	m_pRootNode = Preload::getInstance()->getUI("GiftVip.csb");

	this->addChild(m_pRootNode);
	m_pRootNode->setPosition(Vec2(Director::getInstance()->getWinSize() / 2));

	FIND_UI_CONTROL_RE(ui::Button*, "Gift_Buy_Btn", m_pBtnBuy, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Gift_Buy_Btn_2", m_pBtnBuy2, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Close_Btn", m_pBtnCancel, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuySuccess", m_pBuySuccess, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuyFail", m_pBuyFail, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Vip_Logo", m_pSpriteLogo, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Vip_Logo_Disable", m_pSpriteLogoDisable, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Vip_Logo_Txt_2", m_pSpriteLogoDisableTxt1, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Vip_Logo_Txt_3", m_pSpriteLogoDisableTxt2, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num1", m_pTxtPriceNum1, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num2", m_pTxtPriceNum2, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Vip_Day_Left_Txt", m_pTxtVipTimeLeft, m_pRootNode);

	addButtonTouchEffect(m_pBtnBuy, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		pay();
	});

	addButtonTouchEffect(m_pBtnBuy2, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		pay();
	});

	addButtonTouchEffect(m_pBtnCancel, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_cancelCallback)
		{
			m_cancelCallback();
		}

		removeFromParent();
	});

	this->login();
	this->updateUI();

	m_pBuySuccess->setVisible(false);
	m_pBuyFail->setVisible(false);

	//加载动画： 
	m_pRootNode->setScale(0.8f);
	ScaleTo* pScale1 = ScaleTo::create(0.17f, 1.03f);
	ScaleTo* pScale2 = ScaleTo::create(0.067f, 1.0f);
	Sequence* pSeq = Sequence::create(pScale1, pScale2, nullptr);
	m_pRootNode->runAction(pSeq);

	return true;
}

void GiftVipLayer::updateUI()
{
	int vipTime = GameData::getInstance()->getVipTime();
	if (vipTime > 0) //联网，且是vip
	{
		m_pBtnBuy2->setVisible(true);
		m_pSpriteLogo->setVisible(true);
		m_pTxtVipTimeLeft->setVisible(true);

		m_pBtnBuy->setVisible(false);
		m_pBtnBuy->setEnabled(true);
		m_pSpriteLogoDisable->setVisible(false);

		int vipTime = GameData::getInstance()->getVipTime();
		int day, hour, minute, second;
		Utils::timeToString(vipTime, day, hour, minute, second);

		static char buf[300 + 1] = { 0 };
		const std::string& str1 = GlobalData::getInstance()->getParameter(GiftVipLayerConstant::StringVipTimeLeft1);
		const std::string& str2 = GlobalData::getInstance()->getParameter(GiftVipLayerConstant::StringVipTimeLeft2);
		const std::string& str3 = GlobalData::getInstance()->getParameter(GiftVipLayerConstant::StringVipTimeLeft3);
		const std::string& str4 = GlobalData::getInstance()->getParameter(GiftVipLayerConstant::StringVipTimeLeft4);
		snprintf(buf, 300, "%s%d%s%d%s%d%s",
			str1.c_str(), day,
			str2.c_str(), hour,
			str3.c_str(), minute,
			str4.c_str());

		m_pTxtVipTimeLeft->setString(buf);
		m_pTxtVipTimeLeft->stopAllActions();
		m_pTxtVipTimeLeft->setOpacity(255);
	}
	else if (vipTime == 0) //联网，但非vip
	{
		m_pBtnBuy2->setVisible(false);
		m_pSpriteLogo->setVisible(false);
		m_pSpriteLogoDisableTxt1->setVisible(true);
		m_pSpriteLogoDisableTxt2->setVisible(false);
		m_pTxtVipTimeLeft->setVisible(false);

		m_pBtnBuy->setVisible(true);
		m_pBtnBuy->setEnabled(true);
		m_pSpriteLogoDisable->setVisible(true);
		m_pTxtVipTimeLeft->stopAllActions();
		m_pTxtVipTimeLeft->setOpacity(255);
	}
	else if (vipTime == -1 //未联网  是vip
		|| vipTime < -1)   //未联网，非vip
	{
		m_pBtnBuy2->setVisible(false);
		m_pSpriteLogo->setVisible(false);
		m_pSpriteLogoDisableTxt1->setVisible(false);
		m_pSpriteLogoDisableTxt2->setVisible(true);
		m_pTxtVipTimeLeft->setVisible(true);
		m_pTxtVipTimeLeft->setString(GlobalData::getInstance()->getParameter(GiftVipLayerConstant::StringVipOffline));

		m_pBtnBuy->setVisible(true);
		m_pBtnBuy->setEnabled(false);
		m_pSpriteLogoDisable->setVisible(true);

		DelayTime* pDelay = DelayTime::create(.8f);
		FadeTo* pFade1 = FadeTo::create(0.3f, 100);
		FadeTo* pFade2 = FadeTo::create(0.3f, 255);
		Sequence* pSeq = Sequence::create(pDelay, pFade1, pFade2, nullptr);
		RepeatForever* pRepeat = RepeatForever::create(pSeq);
		m_pTxtVipTimeLeft->runAction(pRepeat);
	}
}

void GiftVipLayer::login(float dt)
{
	//为保证vip起始时间准确性，需联网获取时间
	RemoteLogin* pLogin = new RemoteLogin();
	pLogin->autorelease();
	pLogin->retain();
	pLogin->run([this, pLogin](int result){
		DEBUG_LOG("RemoteLogin, result = %d", result);
		if (result == 0)//成功
		{
			this->updateUI();
		}
		else //失败
		{
			this->scheduleOnce(SEL_SCHEDULE(&GiftVipLayer::login), 2);
		}

		pLogin->release();
	});
}

void GiftVipLayer::pay()
{
	Sound::playSound(SOUND::ID_CLICK);

	//为保证vip起始时间准确性，需联网获取时间
	RemoteLogin* pLogin = new RemoteLogin();
	pLogin->autorelease();
	pLogin->retain();
	pLogin->run([this, pLogin](int result){
		DEBUG_LOG("RemoteLogin, result = %d", result);
		if (result == 0)//成功
		{
			Goods::pay(GoodsType::VIP, this);
		}
		else //失败
		{
			//提示需联网才能获取VIP特权
			GameData::getInstance()->setValue(GAMEDATA::TIME, -1);
			updateUI();

			this->login();
		}

		pLogin->release();
	});
}

void GiftVipLayer::notify(const TGoodsData* pGoodsData, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		Goods::getGoods(pGoodsData);
	}

	doAfterPay(pGoodsData->goods.c_str(), result);
}

void GiftVipLayer::doAfterPay(const char* const goods, EPayResult result)
{
	updateUI();

	if (EPayResult::SUCCESS == result)
	{
		GameData::getInstance()->saveData();

		m_pBuyFail->setVisible(false);
		m_pBuySuccess->setVisible(true);

		//m_pBtnBuy->setVisible(false);
		//m_pBtnBuy2->setVisible(true);

		runAction(Utils::delayToDo(0.5f, [this, goods](){
			m_pBuyFail->setVisible(false);
			m_pBuySuccess->setVisible(false);

			removeFromParent();
		}));
	}
	else if (m_pBuyFail && m_pBuySuccess)
	{
		DEBUG_LOG("Pay failed");
		m_pBuyFail->setVisible(true);
		m_pBuySuccess->setVisible(false);
	}
}

void GiftVipLayer::setCancelCallback(const std::function<void()>& callback)
{
	m_cancelCallback = callback;
}

const std::function<void()>& GiftVipLayer::getCancelCallback()
{
	return m_cancelCallback;
}

void GiftVipLayer::removeFromParent()
{
	//this->unscheduleUpdate();

	//加载动画： 
	ScaleTo* pScale = ScaleTo::create(0.17f, 0.5f);
	FadeTo* pFade = FadeTo::create(0.17f, 0.0f);
	Spawn* pSpawn = Spawn::create(pScale, pFade, nullptr);
	FiniteTimeAction* pEnd = CallFunc::create([this](){
		Node::removeFromParent();
	});
	Sequence* pSeq = Sequence::create(pSpawn, pEnd, nullptr);
	m_pRootNode->runAction(pSeq);
}
