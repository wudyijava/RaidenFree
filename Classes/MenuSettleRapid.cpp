#include "MenuSettleRapid.h"
#include "GameData.h"
#include "Sound.h"
#include "Preload.h"
#include "ModeBase.h"
#include "MenuMainUI.h"
#include "PlaneLayer.h"
#include "Equipment.h"
#include "GiftVip.h"
#include "Bmob/BmobUtils.h"
#include "Common/Macro.h"

MenuSettleRapid::MenuSettleRapid()
: m_pRootNode(nullptr)
, m_pBtnQuit(nullptr)
, m_pBtnRetry(nullptr)

, m_pLableCurrent(nullptr)
, m_pLableBest(nullptr)
, m_pLableRank(nullptr)

, m_iCurrentDistance(0)
, m_iBestDistance(0)
, m_iRank1(0)
, m_iRank2(0)

, m_pSpriteDistanceRank(nullptr)
, m_pSpriteConnectingWeb(nullptr)
, m_pSpriteConnectFailed(nullptr)

, m_pRemote(nullptr)
, m_bRemoteAvailable(false)

, m_pTxtGetPiece(nullptr)
, m_pNodePiece(nullptr)
{

}

Scene* MenuSettleRapid::scene()
{
	Scene *scene = Scene::create();
	MenuSettleRapid *layer = MenuSettleRapid::create();
	scene->addChild(layer);

	return scene;
}

bool MenuSettleRapid::doInit()
{
	Sound::stopAll();

	m_pRootNode = Preload::getInstance()->getUI("SettleRapid.csb");
	this->addChild(m_pRootNode);

    m_pRootAction = Preload::getInstance()->getUIAction("SettleRapid.csb");
	m_pRootAction->gotoFrameAndPlay(0, 20, false);
	m_pRootNode->runAction(m_pRootAction);

	//控件
	FIND_UI_CONTROL_RE(ui::Button*, "Quit_Btn", m_pBtnQuit, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Retry_Btn", m_pBtnRetry, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Current_Distance_Num", m_pLableCurrent, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Best_Distance_Num", m_pLableBest, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextBMFont*, "Distance_Rank_Num", m_pLableRank, m_pRootNode);
	
	FIND_UI_CONTROL_RE(Sprite*, "Distance_Rank", m_pSpriteDistanceRank, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Connecting_Web", m_pSpriteConnectingWeb, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Get_Rank_Failed", m_pSpriteConnectFailed, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Get_Piece_Txt", m_pTxtGetPiece, m_pRootNode);
	FIND_UI_CONTROL_RE(Node*, "Node_Piece", m_pNodePiece, m_pRootNode);

	Utils::addButtonTouchEffect(m_pBtnQuit, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		//Director::getInstance()->replaceScene(MenuMainUI::scene());
		PlaneLayer::pushCommand(ECommand::CommandBackToMainMenu);
		PopupLayer::resumePlaneLayer();
		this->removeFromParent();
	});

	Utils::addButtonTouchEffect(m_pBtnRetry, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		PlaneLayer::pushCommand(ECommand::CommandRestartGame);
		PopupLayer::resumePlaneLayer();
		this->removeFromParent();
	});

	m_pLableCurrent->setString("0");
	m_pLableBest->setString("0");
	m_pLableRank->setString("0%");
	m_pSpriteDistanceRank->setVisible(false);
	m_pSpriteConnectingWeb->setVisible(true);
	m_pSpriteConnectFailed->setVisible(false);
	m_pTxtGetPiece->setVisible(false);

	return true;
}

void MenuSettleRapid::removeFromParent()
{
	if (m_pRemote && m_bRemoteAvailable)
	{
		m_pRemote->unRegCallback();
	}

	//m_pRootAction = Preload::getInstance()->getUIAction("SettleRapid.csb");
	m_pRootNode->runAction(m_pRootAction);
	m_pRootAction->gotoFrameAndPlay(20, 30, false);
	m_pRootAction->setLastFrameCallFunc([this](){
		Node::removeFromParent();
	});
}

void MenuSettleRapid::setCurDistance(int d)
{
	m_iCurrentDistance = d;
	char buf[11] = { 0 };
	snprintf(buf, 10, "%d", m_iCurrentDistance);
	m_pLableCurrent->setString(buf);
}

void MenuSettleRapid::setBestDistance(int d)
{
	m_iBestDistance = d;
	char buf[11] = { 0 };
	snprintf(buf, 10, "%d", m_iBestDistance);
	m_pLableBest->setString(buf);
}

void MenuSettleRapid::getRemoteRank()
{
	m_pRemote = new RemoteRapidDistance(m_iCurrentDistance);
	m_pRemote->retain();
	m_pRemote->autorelease();
	m_bRemoteAvailable = true;

	m_pRemote->run([this](int result, int high, int total){
		if (result == 0 && total > 0 && high >= 0)
		{
			m_pSpriteDistanceRank->setVisible(true);
			m_pSpriteConnectingWeb->setVisible(false);
			m_pSpriteConnectFailed->setVisible(false);

			float rank = 100.0f * (total - high - 1) / total;
			char buf[11] = { 0 };
			snprintf(buf, 10, "%.1f%%", rank);
			m_pLableRank->setString(buf);

			GameData::getInstance()->setValue(GAMEDATA::RAPID_RANK, rank);

			getPiece();
		}
		else
		{
			m_pSpriteDistanceRank->setVisible(false);
			m_pSpriteConnectingWeb->setVisible(false);
			m_pSpriteConnectFailed->setVisible(true);
		}

		m_bRemoteAvailable = false;
		m_pRemote->release();
	});
}

void MenuSettleRapid::getPiece()
{
	//碎片
	/*
	急速模式：
	距离超过100 +1
	距离超过300 +1
	排名超过历史值+1
	排名超过90 +1
	VIP随机加倍
	*/
	m_equipPieceArray.clear();

	unsigned piece = 0;
	int distance = GameData::getInstance()->getValueToInt(GAMEDATA::RAPID_DISTANCE);
	int distanceBest = GameData::getInstance()->getValueToInt(GAMEDATA::RAPID_DISTANCE_BEST);
	float rank = GameData::getInstance()->getValueToFloat(GAMEDATA::RAPID_RANK);
	float rankBest = GameData::getInstance()->getValueToFloat(GAMEDATA::RAPID_RANK_BEST);

	if (distance >= 100) ++piece;
	if (distance >= 300) ++piece;
	if (rank > rankBest) ++piece;
	if (rank >= 90) ++piece;

	int vipFlag = GameData::getInstance()->getValueToInt(GAMEDATA::VIP_FLAG);
	if (vipFlag == 1 && piece > 0)
	{
		float tmp = piece * VipConstant::RandomPiece * Utils::getRandom();
		if (tmp < 1)
		{
			tmp = 1;
		}
		piece += tmp;
	}

	//获取随机碎片
	PlayerEquip::getInstance()->getRandomPiece(piece, m_equipPieceArray);

	//保存最大值
	if (distance > distanceBest)
	{
		GameData::getInstance()->setValue(GAMEDATA::RAPID_DISTANCE_BEST, distance);
	}

	if (rank > rankBest)
	{
		GameData::getInstance()->setValue(GAMEDATA::RAPID_RANK_BEST, rank);
	}

	//TODO 界面显示获得碎片
	if (m_equipPieceArray.size() == 0)
	{
		m_pTxtGetPiece->setVisible(false);
		return;
	}

	m_pTxtGetPiece->setVisible(true);

	//碎片放到m_pNodePiece中
	const Vec2& posCenter = m_pNodePiece->getPosition();
	Vec2 left;
	left.y = 0;
	float delta = 26;
	float itemWidth = 120;

	int i = 0;
	float scale = 0.7f;
	if (m_equipPieceArray.size() <= 5)
	{
		scale = 0.7f;
	}
	else
	{
		scale = 0.7f / (1 + 0.2 * (m_equipPieceArray.size() - 5));
	}

	itemWidth *= scale;
	delta *= scale;

	if (m_equipPieceArray.size() % 2 == 0) //偶数
	{
		left.x = -(delta + itemWidth) / 2 - (m_equipPieceArray.size() / 2 - 1) * (delta + itemWidth);
	}
	else //奇数
	{
		left.x = -(float)(m_equipPieceArray.size() / 2) * (delta + itemWidth);
	}

	for (const TEquipmentData* pData : m_equipPieceArray)
	{
		Node* pItemNodeRoot = Preload::getInstance()->getUI("PieceGot.csb");
		m_pNodePiece->addChild(pItemNodeRoot);
		pItemNodeRoot->setPosition(Vec2(left.x + i * (itemWidth + delta), left.y));

		Sprite* bg = nullptr;
		FIND_UI_CONTROL_RE(Sprite*, "Item_Bg", bg, pItemNodeRoot);
		bg->setSpriteFrame(pData->pic);
		bg->setScale(scale);
		i++;
	}
}