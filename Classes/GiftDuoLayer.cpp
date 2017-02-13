#include "GiftDuoLayer.h"
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

USING_NS_TIMELINE

GiftDuoLayer::GiftDuoLayer()
: m_pRootNode(nullptr)
, m_pBuySuccess(nullptr)
, m_pBuyFail(nullptr)
, m_pBtnCancel(nullptr)
{

}

bool GiftDuoLayer::init(const std::vector<const char*>& giftTypeArray)
{
	if (!PopupLayer::init())
	{
		return false;
	}

	if (giftTypeArray.size() != 2)
	{
		return false;
	}

	if ((!strcmp(giftTypeArray.at(0), GoodsType::BIG_GIFT) && !strcmp(giftTypeArray.at(1), GoodsType::HUG_GIFT))
		|| (!strcmp(giftTypeArray.at(0), GoodsType::HUG_GIFT) && !strcmp(giftTypeArray.at(1), GoodsType::BIG_GIFT)))
	{
		//王者+至尊礼包
		m_pRootNode = Preload::getInstance()->getUI("GiftHugBigPkg.csb");
	}
	else if ((!strcmp(giftTypeArray.at(0), GoodsType::LIFE_1) && !strcmp(giftTypeArray.at(1), GoodsType::LIFE_PKG))
		|| (!strcmp(giftTypeArray.at(0), GoodsType::LIFE_PKG) && !strcmp(giftTypeArray.at(1), GoodsType::LIFE_1)))
	{
		//单生命+生命礼包
		m_pRootNode = Preload::getInstance()->getUI("GiftLifeDuo.csb");
	}

	this->addChild(m_pRootNode);
	m_pRootNode->setPosition(Vec2(Director::getInstance()->getWinSize() / 2));

	FIND_UI_CONTROL_RE(ui::Button*, "Close_Btn", m_pBtnCancel, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuySuccess", m_pBuySuccess, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "SpriteBuyFail", m_pBuyFail, m_pRootNode);

	if (!initSubItem("Gift_Bg1", giftTypeArray.at(0))
		|| !initSubItem("Gift_Bg2", giftTypeArray.at(1)))
	{
		return false;
	}

	addButtonTouchEffect(m_pBtnCancel, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (m_cancelCallback)
		{
			m_cancelCallback();
		}

		removeFromParent();
	});

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

bool GiftDuoLayer::initSubItem(const char* const node, const char* const goodsType)
{
	GoodsItem item;
	FIND_UI_CONTROL_RE(Sprite*, node, item.m_pBg, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Gift_Buy_Btn", item.m_pBtnOk, item.m_pBg);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Diamond_Num_Txt", item.m_pTxtDiamondNum, item.m_pBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Life_Num_Txt", item.m_pTxtLifeNum, item.m_pBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Shield_Num_Txt", item.m_pTxtShieldNum, item.m_pBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Killer_Num_Txt", item.m_pTxtKillerNum, item.m_pBg);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num1", item.m_pTxtPriceNum1, item.m_pBg);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Gift_Num2", item.m_pTxtPriceNum2, item.m_pBg);

	const TGoodsData* pData = GlobalData::getInstance()->getGoods(goodsType);

	updateString(item.m_pTxtDiamondNum, pData, GAMEDATA::MONEY);
	updateString(item.m_pTxtLifeNum, pData, GAMEDATA::LIFE);
	updateString(item.m_pTxtShieldNum, pData, GAMEDATA::SHIELD);
	updateString(item.m_pTxtKillerNum, pData, GAMEDATA::KILLER);

	char sz[31] = { 0 };
	if (item.m_pTxtPriceNum1)
	{
		snprintf(sz, 30, "%d", pData->price / 100);
		item.m_pTxtPriceNum1->setString(sz);
	}
	if (item.m_pTxtPriceNum2)
	{
		snprintf(sz, 30, ".%02d", pData->price % 100);
		item.m_pTxtPriceNum2->setString(sz);
	}

	addButtonTouchEffect(item.m_pBtnOk, Utils::ButtonTouchEffect::NONE, [this, goodsType](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		Goods::pay(goodsType, this);
	});

	return true;
}

void GiftDuoLayer::updateString(ui::TextAtlas* pWidget, const TGoodsData* pGoodData, const char* const name)
{
	char sz[31] = { 0 };
	if (pWidget)
	{
		auto it = pGoodData->itemMap->find(name);
		if (it != pGoodData->itemMap->end())
		{
			snprintf(sz, 30, "%d", it->second);
			pWidget->setString(sz);
		}
		else
		{
			pWidget->setString("0");
		}
	}
}

void GiftDuoLayer::notify(const TGoodsData* pGoodsData, EPayResult result)
{
	if (EPayResult::SUCCESS == result)
	{
		Goods::getGoods(pGoodsData);
	}

	doAfterPay(pGoodsData->goods.c_str(), result);
}

void GiftDuoLayer::doAfterPay(const char* const goods, EPayResult result)
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

void GiftDuoLayer::setCancelCallback(const std::function<void()>& callback)
{
	m_cancelCallback = callback;
}

const std::function<void()>& GiftDuoLayer::getCancelCallback()
{
	return m_cancelCallback;
}

void GiftDuoLayer::removeFromParent()
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
