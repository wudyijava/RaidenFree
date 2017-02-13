#include "Relife.h"
#include "GameData.h"
#include "Sound.h"
#include "CountDown.h"
#include "PlaneLayer.h"
#include "GlobalData.h"
#include "Goods.h"
#include "Carrier.h"
#include "GiftVip.h"
#include "Common/Macro.h"
#include "Preload.h"
#include "Bmob/BmobUtils.h"

USING_NS_TIMELINE

RelifeLayer::RelifeLayer()
: m_pRootNode(nullptr)
, m_pBg(nullptr)

, m_pBuySuccess(nullptr)
, m_pBuyFail(nullptr)

, m_pBtnBuy(nullptr)
, m_pBtnVipBuy(nullptr)
, m_pBtnCancel(nullptr)

, m_pTxtPriceNum1(nullptr)
, m_pTxtPriceNum2(nullptr)

, m_pSpriteFree(nullptr)
, m_pSpritePrice(nullptr)

, m_bVipFreeRelife(false)
{

}

bool RelifeLayer::init()
{
	if (!PopupLayer::init())
	{
		return false;
	}

	m_pRootNode = Preload::getInstance()->getUI("Relife.csb");

	this->addChild(m_pRootNode);
	m_pRootNode->setPosition(Vec2(Director::getInstance()->getWinSize() / 2));

	FIND_UI_CONTROL_RE(Sprite*, "Relife_Bg", m_pBg, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Relife_Btn", m_pBtnBuy, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Vip_Relife_Btn", m_pBtnVipBuy, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Close_Btn", m_pBtnCancel, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuySuccess", m_pBuySuccess, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuyFail", m_pBuyFail, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num1", m_pTxtPriceNum1, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num2", m_pTxtPriceNum2, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Gift_Rmb_Img", m_pSpritePrice, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Gift_Rmb_Img_0", m_pSpriteFree, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Level_Left_Txt", m_pTxtLevelLeft, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Vip_Free_Relife_Txt", m_pTxtVipRelifeLeft, m_pRootNode);

	addButtonTouchEffect(m_pBtnBuy, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		Goods::pay(GoodsType::LIFE_1, this);
	});

	addButtonTouchEffect(m_pBtnVipBuy, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		if (GameData::getInstance()->getValueToInt(GAMEDATA::VIP_FLAG) > 0)
		{
			if (m_bVipFreeRelife)
			{
				GameData::getInstance()->incValue(GAMEDATA::LEVEL_LIFE, 1);
				doAfterPay(GoodsType::LIFE_1, EPayResult::SUCCESS);

				int level = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
				GameData::getInstance()->incVipFreeRelifeCnt(level);
			}
			else
			{
				Goods::pay(GoodsType::LIFE_1, this);
			}
		}
		else
		{
			auto pLayer = GiftVipLayer::create();
			pLayer->setOnExitCallback([this](){
				this->updateUI();
			});
			this->getParent()->addChild(pLayer);
		}
	});

	addButtonTouchEffect(m_pBtnCancel, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_cancelCallback)
		{
			m_cancelCallback();
		}

		removeFromParent();
	});

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

void RelifeLayer::updateUI()
{
	//价格
	const TGoodsData* pData = GlobalData::getInstance()->getGoods(GoodsType::LIFE_1);
	m_pTxtPriceNum1->setString(Utils::toString(pData->price / 100));
	m_pTxtPriceNum2->setString("." + Utils::toString(pData->price % 100, "%02d"));

	//关卡剩余
	int enemyTotal = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_ENEMY_TOTAL);
	int enemyLeft = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_ENEMY_LEFT);
	int ratio = 0;
	if (enemyTotal > 0)
	{
		ratio = (int)enemyLeft * 100.0f / enemyTotal; //还差xx%即可通过本关
	}
	m_pTxtLevelLeft->setString(Utils::toString(ratio, "%d%%"));

	//vip免费复活次数
	int cnt = GameData::getInstance()->getVipFreeRelifeCnt(GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL));
	int freeRelifeCnt = VipConstant::RelifeCnt - cnt;
	if (freeRelifeCnt <= 0)
	{
		freeRelifeCnt = 0;
	}
	if (freeRelifeCnt > VipConstant::RelifeCnt)
	{
		freeRelifeCnt = VipConstant::RelifeCnt;
	}
	m_pTxtVipRelifeLeft->setString(Utils::toString(freeRelifeCnt));

	float fCenterX = m_pBg->getContentSize().width / 2;
	if (GameData::getInstance()->getValueToInt(GAMEDATA::VIP_FLAG) > 0) //是vip
	{
		m_pBtnBuy->setVisible(false);
		m_pBtnVipBuy->setVisible(true);
		m_pBtnVipBuy->setPositionX(fCenterX);

		if (freeRelifeCnt > 0) //免费复活
		{
			m_pSpritePrice->setVisible(false);
			m_pSpriteFree->setVisible(true);

			m_bVipFreeRelife = true;
		}
		else //付费复活
		{
			m_pSpritePrice->setVisible(true);
			m_pSpriteFree->setVisible(false);

			m_bVipFreeRelife = false;
		}
	}
	else //非vip
	{
		m_pBtnBuy->setVisible(true);
		m_pBtnVipBuy->setVisible(true);

		m_pSpritePrice->setVisible(true);
		m_pSpriteFree->setVisible(false);

		m_bVipFreeRelife = false;
	}
}

void RelifeLayer::notify(const TGoodsData* pGoodsData, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		Goods::getGoods(pGoodsData);
	}

	doAfterPay(pGoodsData->goods.c_str(), result);
}

void RelifeLayer::doAfterPay(const char* const goods, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		GameData::getInstance()->saveData();

		m_pBuyFail->setVisible(false);
		if (!m_bVipFreeRelife)
		{
			m_pBuySuccess->setVisible(true);
		}

		runAction(Utils::delayToDo(0.5f, [this, goods](){
			removeFromParent();

			PlaneLayer::pushCommand(ECommand::CommandSpwanPlayer);
			PopupLayer::resumePlaneLayer();
		}));

		if (GameData::getInstance()->getValueToInt(GAMEDATA::VIP_FLAG) > 0) //是vip，战斗力增强
		{
			GameData::getInstance()->setValue(GAMEDATA::VIP_RELIFE_ATTR_ADJUST, VipConstant::RelifeAttributeRatio);
		}
		else
		{
			GameData::getInstance()->setValue(GAMEDATA::VIP_RELIFE_ATTR_ADJUST, 1.0f);
		}

		GameData::getInstance()->setValue(GAMEDATA::BUY_RELIFE, 1);
	}
	else if (m_pBuyFail && m_pBuySuccess)
	{
		DEBUG_LOG("Pay failed");
		m_pBuyFail->setVisible(true);
		m_pBuySuccess->setVisible(false);
	}
}

void RelifeLayer::setCancelCallback(const std::function<void()>& callback)
{
	m_cancelCallback = callback;
}

const std::function<void()>& RelifeLayer::getCancelCallback()
{
	return m_cancelCallback;
}

void RelifeLayer::removeFromParent()
{
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
