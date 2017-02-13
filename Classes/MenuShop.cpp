#include "MenuShop.h"
#include "GameData.h"
#include "GiftLayer.h"
#include "GameScene.h"
#include "Sound.h"
#include "GlobalData.h"
#include "Carrier.h"
#include "Preload.h"
#include "Common/Macro.h"

Scene* MenuShopLayer::scene()
{
	Scene *scene = Scene::create();
	MenuShopLayer *layer = MenuShopLayer::create();
	scene->addChild(layer, 1);

	return scene;
}

MenuShopLayer::MenuShopLayer()
    : m_pRootNode(nullptr)
	, m_pBtnClose(nullptr)

	, m_pBtnMidGift(nullptr)
	, m_pSpriteMidGiftBg(nullptr)
	, m_pTxtMidGiftPrice1(nullptr)
	, m_pTxtMidGiftPrice2(nullptr)
	, m_pTxtMidGiftMoney(nullptr)
	, m_pTxtMidGiftKiller(nullptr)
	, m_pTxtMidGiftShield(nullptr)
	, m_pTxtMidGiftLife(nullptr)

	, m_pBtnBigGift(nullptr)
	, m_pSpriteBigGiftBg(nullptr)
	, m_pTxtBigGiftPrice1(nullptr)
	, m_pTxtBigGiftPrice2(nullptr)
	, m_pTxtBigGiftMoney(nullptr)
	, m_pTxtBigGiftKiller(nullptr)
	, m_pTxtBigGiftShield(nullptr)
	, m_pTxtBigGiftLife(nullptr)

	, m_pBtnMoney(nullptr)
	, m_pSpriteMoneyBg(nullptr)
	, m_pTxtMoneyPrice1(nullptr)
	, m_pTxtMoneyPrice2(nullptr)
	, m_pTxtMoneyMoney(nullptr)

	, m_pBtnKiller(nullptr)
	, m_pSpriteKillerBg(nullptr)
	, m_pTxtKillerPrice1(nullptr)
	, m_pTxtKillerPrice2(nullptr)
	, m_pTxtKillerKiller(nullptr)

	, m_pBtnShield(nullptr)
	, m_pSpriteShieldBg(nullptr)
	, m_pTxtShieldPrice1(nullptr)
	, m_pTxtShieldPrice2(nullptr)
	, m_pTxtShieldShield(nullptr)

	, m_pBuySuccess(nullptr)
	, m_pBuyFailed(nullptr)

{
}


bool MenuShopLayer::doInit()
{
	if (!Layer::init())
	{
		return false;
	}

	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("Shop.csb");
	addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("Shop.csb");
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0, false);

	//控件
	FIND_UI_CONTROL_RE(ui::Button*, "Back_Btn", m_pBtnClose, m_pRootNode);
	Utils::addButtonTouchEffect(m_pBtnClose, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		GameData::getInstance()->saveData();
		//Director::getInstance()->popScene();
		this->removeFromParent();
	});

	FIND_UI_CONTROL_RE(Sprite*, "Shop_Bg1", m_pSpriteBigGiftBg, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Shop_Buy_Btn", m_pBtnBigGift, m_pSpriteBigGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num1", m_pTxtBigGiftPrice1, m_pSpriteBigGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num2", m_pTxtBigGiftPrice2, m_pSpriteBigGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Diamond_Num", m_pTxtBigGiftMoney, m_pSpriteBigGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Nuclear_Num", m_pTxtBigGiftKiller, m_pSpriteBigGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Shield_Num", m_pTxtBigGiftShield, m_pSpriteBigGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Life_Num", m_pTxtBigGiftLife, m_pSpriteBigGiftBg);

	Utils::addButtonTouchEffect(m_pBtnBigGift, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);
		Goods::pay(GoodsType::BIG_GIFT, this);
	});

	const TGoodsData* pGoodsData = GlobalData::getInstance()->getGoods(GoodsType::BIG_GIFT);
	m_pTxtBigGiftPrice1->setString(toString(pGoodsData->price / 100));
	m_pTxtBigGiftPrice2->setString("." + toString(pGoodsData->price % 100, "%02d"));

	m_pTxtBigGiftMoney->setString(toString(pGoodsData->itemMap->at(GAMEDATA::MONEY)));
	m_pTxtBigGiftKiller->setString(toString(pGoodsData->itemMap->at(GAMEDATA::KILLER)));
	m_pTxtBigGiftShield->setString(toString(pGoodsData->itemMap->at(GAMEDATA::SHIELD)));
	m_pTxtBigGiftLife->setString(toString(pGoodsData->itemMap->at(GAMEDATA::LIFE)));

	FIND_UI_CONTROL_RE(Sprite*, "Shop_Bg2", m_pSpriteMidGiftBg, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Shop_Buy_Btn", m_pBtnMidGift, m_pSpriteMidGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num1", m_pTxtMidGiftPrice1, m_pSpriteMidGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num2", m_pTxtMidGiftPrice2, m_pSpriteMidGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Diamond_Num", m_pTxtMidGiftMoney, m_pSpriteMidGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Nuclear_Num", m_pTxtMidGiftKiller, m_pSpriteMidGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Shield_Num", m_pTxtMidGiftShield, m_pSpriteMidGiftBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Life_Num", m_pTxtMidGiftLife, m_pSpriteMidGiftBg);

	Utils::addButtonTouchEffect(m_pBtnMidGift, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);
		Goods::pay(GoodsType::MID_GIFT, this);
	});

	pGoodsData = GlobalData::getInstance()->getGoods(GoodsType::MID_GIFT);
	m_pTxtMidGiftPrice1->setString(toString(pGoodsData->price / 100));
	m_pTxtMidGiftPrice2->setString("." + toString(pGoodsData->price % 100, "%02d"));

	m_pTxtMidGiftMoney->setString(toString(pGoodsData->itemMap->at(GAMEDATA::MONEY)));
	m_pTxtMidGiftKiller->setString(toString(pGoodsData->itemMap->at(GAMEDATA::KILLER)));
	m_pTxtMidGiftShield->setString(toString(pGoodsData->itemMap->at(GAMEDATA::SHIELD)));
	m_pTxtMidGiftLife->setString(toString(pGoodsData->itemMap->at(GAMEDATA::LIFE)));

	FIND_UI_CONTROL_RE(Sprite*, "Shop_Bg3", m_pSpriteMoneyBg, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Shop_Buy_Btn", m_pBtnMoney, m_pSpriteMoneyBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num1", m_pTxtMoneyPrice1, m_pSpriteMoneyBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num2", m_pTxtMoneyPrice2, m_pSpriteMoneyBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Diamond_Num", m_pTxtMoneyMoney, m_pSpriteMoneyBg);

	Utils::addButtonTouchEffect(m_pBtnMoney, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);
		Goods::pay(GoodsType::DIAMOND_1W, this);
	});

	pGoodsData = GlobalData::getInstance()->getGoods(GoodsType::DIAMOND_1W);
	m_pTxtMoneyPrice1->setString(toString(pGoodsData->price / 100));
	m_pTxtMoneyPrice2->setString("." + toString(pGoodsData->price % 100, "%02d"));

	m_pTxtMoneyMoney->setString(toString(pGoodsData->itemMap->at(GAMEDATA::MONEY)));

	FIND_UI_CONTROL_RE(Sprite*, "Shop_Bg4", m_pSpriteKillerBg, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Shop_Buy_Btn", m_pBtnKiller, m_pSpriteKillerBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num1", m_pTxtKillerPrice1, m_pSpriteKillerBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num2", m_pTxtKillerPrice2, m_pSpriteKillerBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Nuclear_Num", m_pTxtKillerKiller, m_pSpriteKillerBg);
	
	Utils::addButtonTouchEffect(m_pBtnKiller, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);
		Goods::pay(GoodsType::KILLER_5, this);
	});

	pGoodsData = GlobalData::getInstance()->getGoods(GoodsType::KILLER_5);
	m_pTxtKillerPrice1->setString(toString(pGoodsData->price / 100));
	m_pTxtKillerPrice2->setString("." + toString(pGoodsData->price % 100, "%02d"));

	m_pTxtKillerKiller->setString(toString(pGoodsData->itemMap->at(GAMEDATA::KILLER)));

	FIND_UI_CONTROL_RE(Sprite*, "Shop_Bg5", m_pSpriteShieldBg, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Shop_Buy_Btn", m_pBtnShield, m_pSpriteShieldBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num1", m_pTxtShieldPrice1, m_pSpriteShieldBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Num2", m_pTxtShieldPrice2, m_pSpriteShieldBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shop_Shield_Num", m_pTxtShieldShield, m_pSpriteShieldBg);

	Utils::addButtonTouchEffect(m_pBtnShield, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);
		Goods::pay(GoodsType::SHIELD_5, this);
	});

	pGoodsData = GlobalData::getInstance()->getGoods(GoodsType::SHIELD_5);
	m_pTxtShieldPrice1->setString(toString(pGoodsData->price / 100));
	m_pTxtShieldPrice2->setString("." + toString(pGoodsData->price % 100, "%02d"));

	m_pTxtShieldShield->setString(toString(pGoodsData->itemMap->at(GAMEDATA::SHIELD)));

	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuySuccess", m_pBuySuccess, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuyFailed", m_pBuyFailed, m_pRootNode);

	m_pBuySuccess->setVisible(false);
	m_pBuyFailed->setVisible(false);

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

void MenuShopLayer::displayBuyPrompt(EPayResult result)
{

	if (EPayResult::SUCCESS == result)
	{
		//显示购买信息
		m_pBuySuccess->setVisible(true);
		//隐藏购买信息
		DelayTime* delay = DelayTime::create(2.f);
		auto hideBuyPrompt = [this]()
		{
			m_pBuySuccess->setVisible(false);
			m_pBuyFailed->setVisible(false);
		};
		FiniteTimeAction* end = CallFunc::create(hideBuyPrompt);
		Sequence* sequence = Sequence::create(delay, end, NULL);
		runAction(sequence);
	}
	else if (m_pBuyFailed && m_pBuySuccess)
	{
		//显示购买信息
		m_pBuyFailed->setVisible(true);
		//隐藏购买信息
		DelayTime* delay = DelayTime::create(2.f);
		auto hideBuyPrompt = [this]()
		{
			m_pBuySuccess->setVisible(false);
			m_pBuyFailed->setVisible(false);
		};
		FiniteTimeAction* end = CallFunc::create(hideBuyPrompt);
		Sequence* sequence = Sequence::create(delay, end, NULL);
		runAction(sequence);
	}
}

void MenuShopLayer::notify(const TGoodsData* pGoodsData, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		Goods::getGoods(pGoodsData);
	}

	displayBuyPrompt(result);
}

std::string MenuShopLayer::toString(int d, const char* const format)
{
	char sz[11] = { 0 };
	snprintf(sz, 10, format, d);
	return std::string(sz);
}
