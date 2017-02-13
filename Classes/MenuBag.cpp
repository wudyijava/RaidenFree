#include "MenuBag.h"
#include "GameData.h"
#include "Sound.h"
#include "GlobalData.h"
#include "Preload.h"
#include "Equipment.h"
#include "Goods.h"
#include "Bullet.h"
#include "Aircraft.h"
#include "Goods.h"
#include "GiftLayer.h"
#include "GiftDuoLayer.h"
#include "Reinforce.h"
#include "Common/Macro.h"
#include "Preload.h"
#include "Guide.h"

namespace MenuBagConstant
{
	const int GradeCnt = 10;

	const int SoundBuy = 71;
	const int SoundEquip = 72;
	const int SoundCompose = 73;
	const int SoundUpgrade = 74;
};

Scene* MenuBag::scene()
{
	Scene *scene = Scene::create();
	MenuBag *layer = MenuBag::create();
	scene->addChild(layer, 1);

	return scene;
}

MenuBag::MenuBag()
: m_pRootNode(nullptr)
, m_pScrollView(nullptr)

, m_pCbTitleAircraft(nullptr)
, m_pCbTitleArmature(nullptr)
, m_pCbTitleMissile(nullptr)
, m_pCbTitleWingman(nullptr)
//分数、宝石
, m_pTextScore(nullptr)
, m_pTextMoney(nullptr)

, m_pBtnClose(nullptr)
, m_curEquipType(EquipTypeAircraft)
, m_lastEquipType(EquipTypeMissile)
{
}

MenuBag::~MenuBag()
{
	GameData::getInstance()->unRegListener(this, GAMEDATA::MONEY);

	//delete m_bagItemArray
	for (auto item : m_bagItemArray)
	{
		delete item;
	}
}

bool MenuBag::doInit()
{
	//载入场景
	m_pRootNode = Preload::getInstance()->getUI("Bag.csb");
	addChild(m_pRootNode);

	//加载动画
	auto pAimAction = Preload::getInstance()->getUIAction("Bag.csb");;
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(0,13, false);
	pAimAction->setLastFrameCallFunc([this, pAimAction](){
		Guide* pGuide = GuideMgr::getInstance()->show(GuideType::MenuBag1, this->getParent(), m_pCbTitleMissile);
	});

	//控件
	FIND_UI_CONTROL_RE(ui::ScrollView*, "ScrollView_1", m_pScrollView, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::CheckBox*, "Bag_Title1", m_pCbTitleAircraft, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::CheckBox*, "Bag_Title2", m_pCbTitleArmature, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::CheckBox*, "Bag_Title3", m_pCbTitleMissile, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::CheckBox*, "Bag_Title4", m_pCbTitleWingman, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num1", m_pTextScore, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "MainUI_Num2", m_pTextMoney, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::Button*, "Close_Btn", m_pBtnClose, m_pRootNode);

	//控件事件
	Utils::addButtonTouchEffect(m_pBtnClose, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->removeFromParent();
	});
	//战机tab
	Utils::addButtonTouchEffect(m_pCbTitleAircraft, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		if (m_curEquipType != EquipTypeAircraft)
		{
			m_curEquipType = EquipTypeAircraft;
			this->updateList();
		}
		this->updateTitle();
	});
	//装甲tab
	Utils::addButtonTouchEffect(m_pCbTitleArmature, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->updateTitle();
		if (m_curEquipType != EquipTypeArmature)
		{
			m_curEquipType = EquipTypeArmature;
			this->updateList();
		}
		this->updateTitle();
	});
	//导弹tab
	Utils::addButtonTouchEffect(m_pCbTitleMissile, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		GuideMgr::getInstance()->destory(GuideType::MenuBag1);

		if (m_curEquipType != EquipTypeMissile)
		{
			m_curEquipType = EquipTypeMissile;
			this->updateList();
		}
		this->updateTitle();
	});
	//僚机tab
	Utils::addButtonTouchEffect(m_pCbTitleWingman, [this](Ref* pSender, ui::CheckBox::EventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		if (m_curEquipType != EquipTypeWingman)
		{
			m_curEquipType = EquipTypeWingman;
			this->updateList();
		}
		this->updateTitle();
	});

	//控件属性
	m_pScrollView->jumpToTop();

	this->updateTitle();
	this->updateList();
	this->updateScoreAndMoney();

	//按返回键退出
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == key
			|| EventKeyboard::KeyCode::KEY_ESCAPE == key
			|| EventKeyboard::KeyCode::KEY_BACKSPACE == key)
		{
			GameData::getInstance()->saveData();
			this->removeFromParent();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	GameData::getInstance()->regListener(this, GAMEDATA::MONEY);

	if (!GameData::getInstance()->checkGuide(GuideType::MenuBag))
	{
		GameData::getInstance()->addGuide(GuideType::MenuBag);
		GameData::getInstance()->addGuide(GuideType::MenuBag1);
		GameData::getInstance()->addGuide(GuideType::MenuBag2);
	}

	return true;
}

void MenuBag::removeFromParent()
{
	auto pAimAction = Preload::getInstance()->getUIAction("Bag.csb");
	m_pRootNode->runAction(pAimAction);
	pAimAction->gotoFrameAndPlay(14, 23, false);
	pAimAction->setLastFrameCallFunc([this](){
		Node::removeFromParent();
	});
}

void MenuBag::updateTitle()
{
	switch (m_curEquipType)
	{
	case EquipTypeAircraft:
		m_pCbTitleAircraft->setSelected(true);
		m_pCbTitleArmature->setSelected(false);
		m_pCbTitleMissile->setSelected(false);
		m_pCbTitleWingman->setSelected(false);
		break;
	case EquipTypeArmature:
		m_pCbTitleAircraft->setSelected(false);
		m_pCbTitleArmature->setSelected(true);
		m_pCbTitleMissile->setSelected(false);
		m_pCbTitleWingman->setSelected(false);
		break;
	case EquipTypeMissile:
		m_pCbTitleAircraft->setSelected(false);
		m_pCbTitleArmature->setSelected(false);
		m_pCbTitleMissile->setSelected(true);
		m_pCbTitleWingman->setSelected(false);
		break;
	case EquipTypeWingman:
		m_pCbTitleAircraft->setSelected(false);
		m_pCbTitleArmature->setSelected(false);
		m_pCbTitleMissile->setSelected(false);
		m_pCbTitleWingman->setSelected(true);
		break;
	default:
		break;
	}

	if (m_pCbTitleAircraft->isSelected())
	{
		m_pCbTitleAircraft->setLocalZOrder(2);
		m_pCbTitleArmature->setLocalZOrder(1);
		m_pCbTitleMissile->setLocalZOrder(1);
		m_pCbTitleWingman->setLocalZOrder(1);
	}
	else if (m_pCbTitleArmature->isSelected())
	{
		m_pCbTitleAircraft->setLocalZOrder(1);
		m_pCbTitleArmature->setLocalZOrder(2);
		m_pCbTitleMissile->setLocalZOrder(1);
		m_pCbTitleWingman->setLocalZOrder(1);
	}
	else if (m_pCbTitleMissile->isSelected())
	{
		m_pCbTitleAircraft->setLocalZOrder(1);
		m_pCbTitleArmature->setLocalZOrder(1);
		m_pCbTitleMissile->setLocalZOrder(2);
		m_pCbTitleWingman->setLocalZOrder(1);
	}
	else if (m_pCbTitleWingman->isSelected())
	{
		m_pCbTitleAircraft->setLocalZOrder(1);
		m_pCbTitleArmature->setLocalZOrder(1);
		m_pCbTitleMissile->setLocalZOrder(2);
		m_pCbTitleWingman->setLocalZOrder(1);
	}
}

void MenuBag::updateList()
{
	m_pScrollView->removeAllChildren();
	m_bagItemArray.clear();

	const std::vector<TEquipmentData*>& equipArray = GlobalData::getInstance()->getEquipmentArray(m_curEquipType);

	int playerId = GameData::getInstance()->getValueToInt(GAMEDATA::PLAYER);
	auto pPlayerEquip = PlayerEquip::getInstance()->getEquip(playerId, m_curEquipType);

	int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);

	char buf[51] = { 0 };
	int realEquipmentCnt = 0;
	for (unsigned i = 0; i < equipArray.size(); i++)
	{
		TEquipmentData* pEquipment = equipArray[i];
		if (EquipTypeAircraft != m_curEquipType)
		{
			if (pPlayerEquip && pEquipment->id == pPlayerEquip->id)
			{
				pEquipment->own = 1;
				pEquipment->equip = 1;
			}
			else
			{
				pEquipment->equip = 0;
			}
		}

		//对于飞机来说，只显示已经购买的项目
		if (EquipTypeAircraft == m_curEquipType && 0 == pEquipment->own && 0 == pEquipment->pick)
		{
			continue;
		}
		realEquipmentCnt++;

		auto node = Preload::getInstance()->getUI("BagListItem.csb");
		m_pScrollView->addChild(node);

		auto action = Preload::getInstance()->getUIAction("BagListItem.csb");
		action->gotoFrameAndPlay(0, true);
		node->runAction(action);

		auto a = m_pScrollView->getInnerContainerSize();

		Sprite* p = (Sprite*)node->getChildByName("Bag_Bg_Little1");
		auto itemSize = p->getContentSize() - Size(0, 20); //去掉每个item上下的空隙
		node->setContentSize(itemSize);

		m_pScrollView->setInnerContainerSize(Size(
			m_pScrollView->getInnerContainerSize().width,
			itemSize.height * equipArray.size()
			));

		Vec2 dest = Vec2(m_pScrollView->getContentSize().width / 2,
			m_pScrollView->getInnerContainerSize().height - itemSize.height * (realEquipmentCnt - 1 + 0.5f));

		//是否显示飞入特效
		if (m_lastEquipType == m_curEquipType)
		{
			node->setPosition(dest);
		}
		else
		{
			Vec2 src = dest + Vec2(500, 0);

			node->setPosition(src);
			DelayTime* pDelay = DelayTime::create((realEquipmentCnt - 1) * 0.05f);
			MoveTo* pMove = MoveTo::create(0.1f, dest);
			Sequence* pSeq = Sequence::create(pDelay, pMove, nullptr);
			node->runAction(pSeq);
		}

		BagItem* item = new BagItem();
		FIND_UI_CONTROL_RE(Sprite*, "Bag_Bg_Little1", item->bg, node);
		FIND_UI_CONTROL_RE(Sprite*, "Bag_Prop_Img", item->img, node);
		FIND_UI_CONTROL_RE(ui::Text*, "Bag_Name_1", item->name, node);

		item->name->setString(pEquipment->name);
		item->img->setSpriteFrame(pEquipment->pic);
		//强化属性
		FIND_UI_CONTROL_RE(ui::Text*, "Bag_Strengthen_Txt", item->propName, node);
		FIND_UI_CONTROL_RE(ui::TextAtlas*, "Bag_Strengthen_Num", item->propValue, node);
		FIND_UI_CONTROL_RE(ui::LoadingBar*, "Bag_Prog", item->propProgress, node);
		//没强化：写原始数值；已强化，写强化后的数值
		item->propName->setString(pEquipment->remark);
		if (EquipTypeAircraft == m_curEquipType)
		{
			int aircraftId = pEquipment->value;

			//原始攻击
			const TAircraftData* pAircraft = GlobalData::getInstance()->getPlayerData(aircraftId);
			float adjust = 0;

			//强化后的攻击
			if (pEquipment->grade > 0)
			{
				auto array = GlobalData::getInstance()->getReinforce(pEquipment->reinforce_id);
				TReinforceGradeData* pReinforceGradeData = array.at(pEquipment->grade - 1);
				adjust = pReinforceGradeData->value;
			}
			int maxAttack = pAircraft->getMaxAttack(adjust);

			snprintf(buf, 50, "%d", maxAttack);
			item->propValue->setString(buf);
		}
		else if (EquipTypeArmature == m_curEquipType)
		{
			int hpAdjust = pEquipment->value;
			if (pEquipment->grade > 0)
			{
				auto array = GlobalData::getInstance()->getReinforce(pEquipment->reinforce_id);
				TReinforceGradeData* pReinforceGradeData = array.at(pEquipment->grade - 1);

				hpAdjust = (int)((hpAdjust * (1 + pReinforceGradeData->value)) + 0.5);
			}
			
			snprintf(buf, 50, "%d", hpAdjust);
			item->propValue->setString(buf);
		}
		else if (EquipTypeMissile == m_curEquipType)
		{
			int bulletId = pEquipment->value;
			const TBulletData* pBullet = GlobalData::getInstance()->getBulletData(bulletId);
			int maxAttack = pBullet->getMaxAttack();

			//强化后的攻击
			if (pEquipment->grade > 0)
			{
				auto array = GlobalData::getInstance()->getReinforce(pEquipment->reinforce_id);
				TReinforceGradeData* pReinforceGradeData = array.at(pEquipment->grade - 1);

				maxAttack = (int)((maxAttack * (1 + pReinforceGradeData->value)) + 0.5);
			}

			snprintf(buf, 50, "%d", maxAttack);
			item->propValue->setString(buf);
		}
		else if (EquipTypeWingman == m_curEquipType)
		{
			int aircraftId = pEquipment->value;
			const TAircraftData* pAircraft = GlobalData::getInstance()->getWingmanData(aircraftId);
			int maxAttack = pAircraft->getMaxAttack();

			//强化后的攻击
			if (pEquipment->grade > 0)
			{
				auto array = GlobalData::getInstance()->getReinforce(pEquipment->reinforce_id);
				TReinforceGradeData* pReinforceGradeData = array.at(pEquipment->grade - 1);

				maxAttack = (int)((maxAttack * (1 + pReinforceGradeData->value)) + 0.5);
			}

			snprintf(buf, 50, "%d", maxAttack);
			item->propValue->setString(buf);
		}

		item->propProgress->setPercent(pEquipment->grade * 10);
		//强化、购买的金额
		FIND_UI_CONTROL_RE(Sprite*, "Bag_Diamond_Img", item->currency, node);
		FIND_UI_CONTROL_RE(ui::TextAtlas*, "Bag_Num", item->price, node);

		int price = 0;
		const char* currency = pEquipment->currency.c_str();

		if (pEquipment->equip && pEquipment->grade >= MenuBagConstant::GradeCnt)//已满级且已装备
		{
			item->currency->setVisible(false);
		}
		else if (pEquipment->equip && pEquipment->grade < MenuBagConstant::GradeCnt)//已装备、未满级
		{
			int grade = pEquipment->grade + 1;
			const TReinforceGradeData* pGradeData = GlobalData::getInstance()->getReinforce(pEquipment->reinforce_id).at(grade - 1);
			item->currency->setTexture("Com/MainUI_Diamond_Img.png");
			snprintf(buf, 50, "%d", pGradeData->money);
			item->price->setString(buf);
			item->currency->setVisible(true);

			price = pGradeData->money;
			currency = CurrencyType::COIN;
		}
		else if (!pEquipment->equip && pEquipment->own)//已购买、未装备
		{
			item->currency->setVisible(false);
		}
		else if(!pEquipment->own)//未购买的显示购买金额
		{
			if (pEquipment->currency == CurrencyType::COIN)
			{
				item->currency->setTexture("Com/MainUI_Diamond_Img.png");
			}
			else if (pEquipment->currency == CurrencyType::CASH)
			{
				item->currency->setTexture("Com/Shop_Rmb_Img.png");
			}
			snprintf(buf, 50, "%d", pEquipment->price);
			item->price->setString(buf);
			item->currency->setVisible(true);

			price = pEquipment->price;
			currency = pEquipment->currency.c_str();
		}

		//强化、装备、购买、合成按钮
		FIND_UI_CONTROL_RE(ui::Button*, "Bag_Strengthen_Btn", item->btnStrengthen, node);
		FIND_UI_CONTROL_RE(ui::Button*, "Bag_Buy_Btn", item->btnBuy, node);
		FIND_UI_CONTROL_RE(ui::Button*, "Bag_Equip_Btn", item->btnEquip, node);
		FIND_UI_CONTROL_RE(ui::Button*, "Bag_Compose_Btn", item->btnCompose, node);

		if (pEquipment->equip && pEquipment->grade >= MenuBagConstant::GradeCnt)//已满级且已装备
		{
			pEquipment->grade = MenuBagConstant::GradeCnt;

			item->btnBuy->setVisible(false);
			item->btnEquip->setVisible(false);
			item->btnStrengthen->setVisible(false);
			item->btnCompose->setVisible(false);
		}
		else if (pEquipment->equip)//已装备、未满级
		{
			item->btnBuy->setVisible(false);
			item->btnEquip->setVisible(false);
			item->btnStrengthen->setVisible(true);
			item->btnCompose->setVisible(false);
		}
		else if (!pEquipment->equip && pEquipment->own) //已购买、未装备
		{
			item->btnBuy->setVisible(false);
			item->btnEquip->setVisible(true);
			item->btnStrengthen->setVisible(false);
			item->btnCompose->setVisible(false);
		}
		else if (!pEquipment->own) //未购买
		{
			item->btnBuy->setVisible(true);
			item->btnEquip->setVisible(false);
			item->btnStrengthen->setVisible(false);
			item->btnCompose->setVisible(false);

			if (EquipTypeMissile == m_curEquipType  && 0 == i)
			{
				this->runAction(Utils::delayToDo(0.5f, [this, item](){
					Guide* pGuide = GuideMgr::getInstance()->show(GuideType::MenuBag2, this->getParent(), item->btnBuy);
				}));
			}
		}
		
		if (pEquipment->pick >= pEquipment->piece && pEquipment->pick > 0//碎片已凑齐
			&& pEquipment->grade < MenuBagConstant::GradeCnt)  //尚未满级
		{
			item->btnBuy->setVisible(false);
			item->btnEquip->setVisible(false);

			if (pEquipment->own)
			{
				item->btnStrengthen->setVisible(true);//直接升级
				item->btnCompose->setVisible(false); 
			}
			else
			{
				item->btnStrengthen->setVisible(false);
				item->btnCompose->setVisible(true); //开启合成
			}

			item->currency->setVisible(false);
		}

		//第七架飞机
		if (EquipTypeAircraft == m_curEquipType && 6 == i) 
		{
			//第七架飞机不显示金额
			item->currency->setVisible(false);
			//第七架飞机不显示购买按钮
			item->btnBuy->setVisible(false);
			item->btnStrengthen->setVisible(false);
			item->btnCompose->setVisible(false);
			//item->btnEquip->setVisible(false);
			if (pEquipment->pick >= pEquipment->piece && pEquipment->pick > 0)//碎片已凑齐
			{
				if (pEquipment->own)
				{
					item->btnStrengthen->setVisible(true);
					item->btnCompose->setVisible(false);
				}
				else
				{
					item->btnStrengthen->setVisible(false);
					item->btnCompose->setVisible(true);
				}
			}
		}

		//强化/合成按钮的动态提醒效果
		Node* message = nullptr;
		FIND_UI_CONTROL_RE(Sprite*, "Bag_Message_Bg", message, node);
		if ((pEquipment->pick >= pEquipment->piece && pEquipment->piece > 0) //有足够的碎片
			//|| money >= price //有足够的宝石
			)
		{
			message->setVisible(true);
		}
		else
		{
			message->setVisible(false);
		}


		//购买
		Utils::addButtonTouchEffect(item->btnBuy, Utils::ButtonTouchEffect::NONE, 
			[this, &equipArray, pEquipment, price, playerId](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(MenuBagConstant::SoundBuy);

			GuideMgr::getInstance()->destory(GuideType::MenuBag2);

			//TODO 仅支持虚拟币支付
			Goods::pay(price, 
				pEquipment->name.c_str(),
				//成功
				[this, &equipArray, pEquipment, playerId](){
				//自动装备
				for (auto e : equipArray)
				{
					if (e == pEquipment)
					{
						e->equip = 1;
						e->own = 1;
						PlayerEquip::getInstance()->addEquip(playerId, e);
					}
					else
					{
						e->equip = 0;
						PlayerEquip::getInstance()->removeEquip(playerId, e->id);
					}
				}

				this->updateList();
				this->updateScoreAndMoney();

				GameData::getInstance()->saveData();

			}, 
				//失败
				[this](){
				std::vector<const char*> goodsArray;
				goodsArray.push_back(GoodsType::HUG_GIFT);
				goodsArray.push_back(GoodsType::BIG_GIFT);
				auto pLayer = GiftDuoLayer::create(goodsArray);
				this->addChild(pLayer);
			});
			
		});

		//装备
		Utils::addButtonTouchEffect(item->btnEquip, Utils::ButtonTouchEffect::NONE,
			[this, &equipArray, pEquipment, playerId](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			//自动装备
			for (auto e : equipArray)
			{
				if (e == pEquipment)
				{
					e->equip = 1;
					if (EquipTypeAircraft == m_curEquipType)
					{
						GameData::getInstance()->setValue(GAMEDATA::PLAYER, e->id);
						PlayerEquip::getInstance()->addEquip(e->id, e);
					}
					else
					{
						PlayerEquip::getInstance()->addEquip(playerId, e);
					}
				}
				else
				{
					e->equip = 0;
					PlayerEquip::getInstance()->removeEquip(playerId, e->id);
				}
			}

			this->updateList();
			GameData::getInstance()->saveData();
		});

		//强化
		Utils::addButtonTouchEffect(item->btnStrengthen, Utils::ButtonTouchEffect::NONE,
			[this, &equipArray, pEquipment, price, item](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			int realPrice = price;

			//如果碎片数足够，则直接升级
			if (pEquipment->pick >= pEquipment->piece && pEquipment->pick > 0)
			{
				realPrice = 0;
				pEquipment->pick -= pEquipment->piece;
			}
			else//否则要扣除相应的宝石
			{
				realPrice = price;
			}

			//TODO 仅支持虚拟币支付
			Goods::pay(realPrice,
				pEquipment->name.c_str(),
				//成功
				[this, &equipArray, pEquipment](){

				if (MenuBagConstant::GradeCnt > pEquipment->grade)
				{
					pEquipment->grade++;
				}

				this->updateList();
				this->updateScoreAndMoney();
				GameData::getInstance()->saveData();
			},
				//失败
				[this](){
				std::vector<const char*> goodsArray;
				goodsArray.push_back(GoodsType::HUG_GIFT);
				goodsArray.push_back(GoodsType::BIG_GIFT);
				auto pLayer = GiftDuoLayer::create(goodsArray);
				this->addChild(pLayer);
			});


		});

		//合成
		Utils::addButtonTouchEffect(item->btnCompose, Utils::ButtonTouchEffect::NONE,
			[this, &equipArray, pEquipment, playerId](Ref* pSender, ui::Widget::TouchEventType type)
		{
			Sound::playSound(SOUND::ID_CLICK);

			if (pEquipment->pick < pEquipment->piece || pEquipment->pick == 0)
			{
				return;
			}

			pEquipment->pick -= pEquipment->piece;

			//碎片已凑齐
			//没有购买
			if (pEquipment->own == 0)
			{
				//合成并装备（相当于购买）
				for (auto e : equipArray)
				{
					if (e == pEquipment)
					{
						e->equip = 1;
						e->own = 1;
						PlayerEquip::getInstance()->addEquip(playerId, e);
					}
					else
					{
						e->equip = 0;
						PlayerEquip::getInstance()->removeEquip(playerId, e->id);
					}
				}

			}
			else
			{
				//升级并装备
				if (MenuBagConstant::GradeCnt > pEquipment->grade)
				{
					pEquipment->grade++;
				}

			}

			this->updateList();

			GameData::getInstance()->saveData();

		});

		//碎片
		FIND_UI_CONTROL_RE(Sprite*, "Bag_Piece_Txt", item->piece, node);
		FIND_UI_CONTROL_RE(ui::TextAtlas*, "Bag_Pick_Cnt_Txt", item->pieceCnt, node);
		if (pEquipment->pick <= 0)
		{
			item->piece->setVisible(false);
		}
		else
		{
			item->piece->setVisible(true);

			char buf[21] = { 0 };
			snprintf(buf, 20, "%d/%d", pEquipment->pick, pEquipment->piece);
			item->pieceCnt->setString(buf);
		}

		//说明
		FIND_UI_CONTROL_RE(Sprite*, "Bag_Equip_Txt", item->remark, node);
		if (pEquipment->equip)
		{
			item->remark->setVisible(true);
		}
		else
		{
			item->remark->setVisible(false);
		}

		m_bagItemArray.push_back(item);
	}

	m_lastEquipType = m_curEquipType;
	m_pScrollView->scrollToTop(0.3f, true);
}

void MenuBag::updateScoreAndMoney()
{
	int money = GameData::getInstance()->getValueToInt(GAMEDATA::MONEY);
	int score = GameData::getInstance()->getValueToInt(GAMEDATA::SCORE);

	char strScore[20] = {0};
	char strMoney[20] = {0};
	sprintf(strScore, "%d", score);
	sprintf(strMoney, "%d", money);

	m_pTextScore->setString(strScore);
	m_pTextMoney->setString(strMoney);
}

void MenuBag::updateValue(const std::string& type, int value)
{
	if (type == GAMEDATA::MONEY)
	{
		updateScoreAndMoney();
	}
}
