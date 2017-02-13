#include "GiftLayer.h"
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
#include "GiftDuoLayer.h"

USING_NS_TIMELINE

GiftLayer::GiftLayer()
: m_pRootNode(nullptr)
, m_pBuySuccess(nullptr)
, m_pBuyFail(nullptr)
, m_pLayer(nullptr)
, m_pBtnOk(nullptr)
, m_pBtnCancel(nullptr)

, m_pTxtDiamondNum(nullptr)
, m_pTxtLifeNum(nullptr)
, m_pTxtShieldNum(nullptr)
, m_pTxtKillerNum(nullptr)
, m_pTxtPriceNum1(nullptr)
, m_pTxtPriceNum2(nullptr)
, m_pTxtPriceNum(nullptr)

, m_pTxtMidGiftRecommend(nullptr)
, m_pTxtBigGiftRecommend(nullptr)
, m_pTxtLifeRecommend(nullptr)
, m_pTxtLifePkgRecommend(nullptr)

, m_pTxtNotEnoughMoney(nullptr)
{

}

bool GiftLayer::init(const char* const eGiftType)
{
	if (!PopupLayer::init())
	{
		return false;
	}

	m_eGiftType = eGiftType;

	if (!strcmp(GoodsType::HUG_GIFT, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getHugGiftUi());
	}
	else if (!strcmp(GoodsType::BIG_GIFT, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getBigGiftUi());
	}
	else if (!strcmp(GoodsType::MID_GIFT, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getMidGiftUi());
	}
	else if (!strcmp(GoodsType::NEW_GIFT, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getNewGiftUi());
	}
	else if (!strcmp(GoodsType::LIFE_PKG, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getLifePkgUi());
	}
	else if (!strcmp(GoodsType::PLANE_5TH_PKG, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getPlane5PkgUi());
	}
	else if (!strcmp(GoodsType::PLANE_6TH_PKG, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getPlane6PkgUi());
	}
	else if (!strcmp(GoodsType::SHIELD_5, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getShieldGiftUi());
	}
	else if (!strcmp(GoodsType::KILLER_5, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getKillerGiftUi());
	}
	else if (!strcmp(GoodsType::DIAMOND_1W, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getDiamondGiftUi());
	}
	else if (!strcmp(GoodsType::LIFE_1, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getLifeGiftUi());
	}
	else if (!strcmp(GoodsType::PLANE_4TH, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getPlane4Ui());
	}
	else if (!strcmp(GoodsType::PLANE_5TH, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getPlane5Ui());
	}
	else if (!strcmp(GoodsType::PLANE_6TH, m_eGiftType))
	{
		m_pRootNode = Preload::getInstance()->getUI(Carrier::getInstance()->getPlane6Ui());
	}
	else
	{
		DEBUG_LOG("Unkonwn gift layer type : %s", eGiftType);
		m_pRootNode = nullptr;
		return false;
	}

	this->addChild(m_pRootNode);
	m_pRootNode->setPosition(Vec2(Director::getInstance()->getWinSize() / 2));

	FIND_UI_CONTROL_RE(ui::Button*, "Gift_Buy_Btn", m_pBtnOk, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Close_Btn", m_pBtnCancel, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuySuccess", m_pBuySuccess, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuyFail", m_pBuyFail, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Diamond_Num_Txt", m_pTxtDiamondNum, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Life_Num_Txt", m_pTxtLifeNum, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Shield_Num_Txt", m_pTxtShieldNum, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Killer_Num_Txt", m_pTxtKillerNum, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num1", m_pTxtPriceNum1, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num2", m_pTxtPriceNum2, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num", m_pTxtPriceNum, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num1_Orig", m_pTxtPriceNumOirg, m_pRootNode);

	const TGoodsData* pData = GlobalData::getInstance()->getGoods(m_eGiftType);

	if(m_pTxtDiamondNum) m_pTxtDiamondNum->setString(Utils::toString(pData->itemMap->at(GAMEDATA::MONEY)));
	if (m_pTxtLifeNum) m_pTxtLifeNum->setString(Utils::toString(pData->itemMap->at(GAMEDATA::LIFE)));
	if (m_pTxtShieldNum) m_pTxtShieldNum->setString(Utils::toString(pData->itemMap->at(GAMEDATA::SHIELD)));
	if (m_pTxtKillerNum) m_pTxtKillerNum->setString(Utils::toString(pData->itemMap->at(GAMEDATA::KILLER)));

	if (m_pTxtPriceNum1) m_pTxtPriceNum1->setString(Utils::toString(pData->price / 100));
	if (m_pTxtPriceNum2) m_pTxtPriceNum2->setString("." + Utils::toString(pData->price % 100, "%02d"));
	if (m_pTxtPriceNum) m_pTxtPriceNum->setString(Utils::toString(pData->price));
	if (m_pTxtPriceNumOirg) m_pTxtPriceNumOirg->setString(Utils::toString(pData->orignPrice / 100));

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Mid_Gift_Recommend", m_pTxtMidGiftRecommend, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Big_Gift_Recommend", m_pTxtBigGiftRecommend, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Life_Recommend", m_pTxtLifeRecommend, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Life_Pkg_Recommend", m_pTxtLifePkgRecommend, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Gift_Not_Enough_Money", m_pTxtNotEnoughMoney, m_pRootNode);

	addButtonTouchEffect(m_pBtnOk, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		Goods::pay(m_eGiftType, this);
	});

	addButtonTouchEffect(m_pBtnCancel, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_cancelCallback)
		{
			m_cancelCallback();
		}

		removeFromParent();
	});

	if (m_pTxtMidGiftRecommend)
	{
		m_pTxtMidGiftRecommend->setTouchEnabled(true);
		addButtonTouchEffect(m_pTxtMidGiftRecommend, Utils::ButtonTouchEffect::SINK, [this](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			auto pLayer = GiftLayer::create(GoodsType::MID_GIFT);

			pLayer->setOnExitCallback(this->getOnExitCallback());
			pLayer->setCancelCallback(this->getCancelCallback());

			getParent()->addChild(pLayer);
			this->setOnExitCallback(nullptr);
			removeFromParent();
		});
	}

	if (m_pTxtBigGiftRecommend)
	{
		m_pTxtBigGiftRecommend->setTouchEnabled(true);
		addButtonTouchEffect(m_pTxtBigGiftRecommend, Utils::ButtonTouchEffect::SINK, [this](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			auto pLayer = GiftLayer::create(GoodsType::BIG_GIFT);

			pLayer->setOnExitCallback(this->getOnExitCallback());
			pLayer->setCancelCallback(this->getCancelCallback());

			getParent()->addChild(pLayer);
			this->setOnExitCallback(nullptr);
			removeFromParent();
		});
	}

	if (m_pTxtLifeRecommend)
	{
		m_pTxtLifeRecommend->setTouchEnabled(true);
		addButtonTouchEffect(m_pTxtLifeRecommend, Utils::ButtonTouchEffect::SINK, [this](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			auto pLayer = GiftLayer::create(GoodsType::LIFE_1);

			pLayer->setOnExitCallback(this->getOnExitCallback());
			pLayer->setCancelCallback(this->getCancelCallback());

			getParent()->addChild(pLayer);
			this->setOnExitCallback(nullptr);
			removeFromParent();
		});
	}

	if (m_pTxtLifePkgRecommend)
	{
		m_pTxtLifePkgRecommend->setTouchEnabled(true);
		addButtonTouchEffect(m_pTxtLifePkgRecommend, Utils::ButtonTouchEffect::SINK, [this](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			auto pLayer = GiftLayer::create(GoodsType::LIFE_PKG);

			pLayer->setOnExitCallback(this->getOnExitCallback());
			pLayer->setCancelCallback(this->getCancelCallback());

			getParent()->addChild(pLayer);
			this->setOnExitCallback(nullptr);
			removeFromParent();
		});
	}
	
	if (m_pTxtNotEnoughMoney)
	{
		m_pTxtNotEnoughMoney->setTouchEnabled(true);
		m_pTxtNotEnoughMoney->setVisible(false);
		addButtonTouchEffect(m_pTxtNotEnoughMoney, Utils::ButtonTouchEffect::SINK, [this](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			//auto pLayer = GiftLayer::create(GoodsType::BIG_GIFT);
			std::vector<const char*> goodsArray;
			goodsArray.push_back(GoodsType::HUG_GIFT);
			goodsArray.push_back(GoodsType::BIG_GIFT);
			auto pLayer = GiftDuoLayer::create(goodsArray);

			pLayer->setOnExitCallback([this](){
				this->updateButtonOk();
			});

			//pLayer->setOnExitCallback(this->getOnExitCallback());
			getParent()->addChild(pLayer);
			//this->setOnExitCallback(nullptr);
			//removeFromParent();
		});

	}

	m_pBuySuccess->setVisible(false);
	m_pBuyFail->setVisible(false);

	this->updateButtonOk();

	//加载动画： 
	m_pRootNode->setScale(0.8f);
	ScaleTo* pScale1 = ScaleTo::create(0.17f, 1.03f);
	ScaleTo* pScale2 = ScaleTo::create(0.067f, 1.0f);
	Sequence* pSeq = Sequence::create(pScale1, pScale2, nullptr);
	m_pRootNode->runAction(pSeq);

	return true;
}

void GiftLayer::updateButtonOk()
{
	const TGoodsData* pData = GlobalData::getInstance()->getGoods(m_eGiftType);
	if (!strcmp(pData->currency.c_str(), "coin"))
	{
		int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);
		if (money < pData->price)
		{
			m_pTxtNotEnoughMoney->setVisible(true);
			m_pBtnOk->setEnabled(false);

			m_pTxtNotEnoughMoney->stopAllActions();
			m_pTxtNotEnoughMoney->setOpacity(255);
			FadeTo* pFade1 = FadeTo::create(0.5f, 100);
			FadeTo* pFade2 = FadeTo::create(0.5f, 255);
			Sequence* pSeq = Sequence::create(pFade1, pFade2, nullptr);
			RepeatForever* pRepeat = RepeatForever::create(pSeq);
			m_pTxtNotEnoughMoney->runAction(pRepeat);
		}
		else
		{
			m_pTxtNotEnoughMoney->setVisible(false);
			m_pBtnOk->setEnabled(true);
		}
	}
}

void GiftLayer::notify(const TGoodsData* pGoodsData, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		Goods::getGoods(pGoodsData);
	}

	doAfterPay(pGoodsData->goods.c_str(), result);
}

void GiftLayer::doAfterPay(const char* const goods, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		GameData::getInstance()->saveData();

		m_pBuyFail->setVisible(false);
		m_pBuySuccess->setVisible(true);

		runAction(Utils::delayToDo(0.5f, [this, goods](){
			removeFromParent();

			if (!strcmp(GoodsType::LIFE_1, goods) || !strcmp(GoodsType::LIFE_PKG, goods))
			{
				PlaneLayer::pushCommand(ECommand::CommandSpwanPlayer);
				PopupLayer::resumePlaneLayer();
			}
		}));
	}
	else if (m_pBuyFail && m_pBuySuccess)
	{
		DEBUG_LOG("Pay failed");
		m_pBuyFail->setVisible(true);
		m_pBuySuccess->setVisible(false);
	}
}

void GiftLayer::setCancelCallback(const std::function<void()>& callback)
{
	m_cancelCallback = callback;
}

const std::function<void()>& GiftLayer::getCancelCallback()
{
	return m_cancelCallback;
}

void GiftLayer::removeFromParent()
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
