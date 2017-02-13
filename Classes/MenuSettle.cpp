#include "MenuSettle.h"
#include "GameData.h"
#include "Sound.h"
#include "MenuMission.h"
#include "MenuStrengthen.h"
#include "Preload.h"
#include "ModeBase.h"
#include "MenuMainUI.h"
#include "Sound.h"
#include "Level.h"
#include "Equipment.h"
#include "GiftVip.h"
#include "GeneralDialog.h"
#include "Common/Shake.h"
#include "Common/Macro.h"

bool MenuSettle::m_bBattleResult = true;

MenuSettle::MenuSettle()
: m_pRootNode(nullptr)
, m_pBtnContinue(nullptr)
, m_pBtnUpgrade(nullptr)
, m_pBtnVip(nullptr)

, m_pStar1(nullptr)
, m_pStar2(nullptr)
, m_pStar3(nullptr)

, m_pLableEnemy(nullptr)
, m_pLableMoney(nullptr)
, m_pLableScore(nullptr)
, m_pLableMoneyVip(nullptr)
, m_pSpriteMoneyVip(nullptr)

, m_pTxtGetPiece(nullptr)
, m_pNodePiece(nullptr)

, m_pTxtVipPrompt(nullptr)

, m_iEnemy(0)
, m_iScore(0)
, m_iMoney(0)
, m_iMoneyVip(0)

, m_fTmp(0)
, m_btnContinuePosX(0)

, m_fEnemyCur(0)
, m_fScoreCur(0)
, m_iScoreOver(0)
, m_fMoneyCur(0)
, m_fMoneyVipCur(0)
{

}

Scene* MenuSettle::scene()
{
	Scene *scene = Scene::create();
	MenuSettle *layer = MenuSettle::create();
	scene->addChild(layer, 1);

	return scene;
}

void MenuSettle::setBattleResult(bool result)
{
	m_bBattleResult = result;
}

bool MenuSettle::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Sound::stopAll();

	m_pRootNode = Preload::getInstance()->getUI("Settle.csb");
	this->addChild(m_pRootNode);

	//控件
	FIND_UI_CONTROL_RE(ui::Button*, "Continue_Btn", m_pBtnContinue, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Strengthen_Btn", m_pBtnUpgrade, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::Button*, "Vip_Btn", m_pBtnVip, m_pRootNode);

	m_btnContinuePosX = m_pBtnContinue->getPositionX();

	FIND_UI_CONTROL_RE(Node*, "Win", m_pWin, m_pRootNode);
	FIND_UI_CONTROL_RE(Node*, "Lose", m_pLose, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Win_Star1", m_pStar1, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Win_Star2", m_pStar2, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Win_Star3", m_pStar3, m_pRootNode);

	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Win_Score_Num1", m_pLableEnemy, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Win_Score_Num2", m_pLableScore, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Win_Score_Num3", m_pLableMoney, m_pRootNode);
	FIND_UI_CONTROL_RE(ui::TextAtlas*, "Win_Score_Num4", m_pLableMoneyVip, m_pRootNode);
	FIND_UI_CONTROL_RE(Sprite*, "Win_Txt4", m_pSpriteMoneyVip, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Get_Piece_Txt", m_pTxtGetPiece, m_pRootNode);
	FIND_UI_CONTROL_RE(Node*, "Node_Piece", m_pNodePiece, m_pRootNode);

	FIND_UI_CONTROL_RE(Sprite*, "Vip_Piece_Prompt", m_pTxtVipPrompt, m_pRootNode);

	Utils::addButtonTouchEffect(m_pBtnContinue, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::stopAll();
		Sound::playSound(SOUND::ID_CLICK);

		int mode = GameData::getInstance()->getValueToInt(GAMEDATA::MODE);
		if (ModeBase::ModeLevel == mode)
		{
			Director::getInstance()->replaceScene(MenuMission::scene());
		}
		else
		{
			Director::getInstance()->replaceScene(MenuMainUI::scene());
		}
	});

	Utils::addButtonTouchEffect(m_pBtnUpgrade, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		this->addChild(MenuStrengthen::create());
	});

	Utils::addButtonTouchEffect(m_pBtnVip, Utils::ButtonTouchEffect::NONE, [this](Ref* pSender, ui::Widget::TouchEventType type)
	{
		Sound::playSound(SOUND::ID_CLICK);

		auto pLayer = GiftVipLayer::create();
		pLayer->setOnExitCallback([this](){

			if(GameData::getInstance()->getVipTime() > 0)
			{
				this->addChild(GeneralDialogCreator::vipNext());
				this->updateVipState();
			}
		});
		addChild(pLayer);

	});

	if (!m_bBattleResult) //失败 在这里就退出了！！
	{
		m_pWin->setVisible(false);
		m_pLose->setVisible(true);
		m_pBtnVip->setVisible(false);
		m_pBtnContinue->setPositionX(Director::getInstance()->getWinSize().width / 2);
		return true;
	}
	else //成功
	{
		m_pWin->setVisible(true);
		m_pLose->setVisible(false);
	}

	int vipFlag = GameData::getInstance()->getValueToInt(GAMEDATA::VIP_FLAG);
	if (vipFlag) //是vip
	{
		m_pSpriteMoneyVip->setSpriteFrame("Win_Txt5.png");
		m_pBtnVip->setVisible(false);
		m_pBtnContinue->setPositionX(Director::getInstance()->getWinSize().width / 2);
	}
	else
	{
		m_pSpriteMoneyVip->setSpriteFrame("Win_Txt4.png");
		m_pBtnVip->setVisible(true);
	}

	//累计数值
	m_iEnemy = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_ENEMY);
	m_iMoney = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_MONEY);
	m_iScore = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_SCORE);
	m_iMoneyVip = m_iMoney * (VipConstant::MoneyReward - 1);
	int life = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_LIFE);

	//累加当前关卡数值
	GameData::getInstance()->incValue(GAMEDATA::ENEMY, m_iEnemy);
	GameData::getInstance()->incValue(GAMEDATA::MONEY, m_iMoney);
	GameData::getInstance()->incValue(GAMEDATA::SCORE, m_iScore);
	if (vipFlag)
	{
		GameData::getInstance()->incValue(GAMEDATA::MONEY, m_iMoneyVip);
	}

	if (GameData::getInstance()->getValueToInt(GAMEDATA::MODE) == ModeBase::ModeLevel
		&& life > 0)
	{
		GameData::getInstance()->setValue(GAMEDATA::LIFE, life);
	}

	//评级: [0,30), [30,70), [70, 90), [90, 100]
	int iScoreTotal = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_SCORE_TOTAL);
	int grade = (int)(m_iScore * 100.0f / iScoreTotal);
	int level = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
	const TLevelData* pLevelData = GlobalData::getInstance()->getLevelData(level);
	m_grade = 0;
	m_pStar1->setVisible(false);
	m_pStar2->setVisible(false);
	m_pStar3->setVisible(false);

	if (grade < 30)
	{
		m_grade = 0;
	}
	else if (grade >= 30 && grade < 70)
	{
		m_grade = 1;
	}
	else if (grade >= 70 && grade < 90)
	{
		m_grade = 2;
	}
	else if (grade >= 90)
	{
		m_grade = 3;
	}
	else
	{
		m_grade = 0;
	}

	int difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	int preGrade = GameData::getInstance()->getLevelStar(difficulty, level);
	if (m_grade > preGrade)
	{
		GameData::getInstance()->setLevelStar(difficulty, level, m_grade);
	}

	GameData::getInstance()->incValue(GAMEDATA::LEVEL, 1);

	/*
	无尽模式、急速模式次数限制：
	1、普通用户每次游戏的前三次按正常规则掉落，后面每次增加一块，封顶6块
	2、VIP用户每天前二次按正常规则掉落，后面每次增加一块，封顶10块

	碎片掉落规则
	关卡模式：
	根据level定义的碎片数确定
	VIP随机加倍

	无尽模式：
	击落敌机的波数比上次的多+1
	超过3波敌机+1
	超过6波敌机+1
	超过12波敌机+1
	VIP随机加倍
	*/
	//获得的碎片。VIP用户加倍
	unsigned piece = 0;
	int mode = GameData::getInstance()->getValueToInt(GAMEDATA::MODE);
	if (mode == ModeBase::ModeLevel) //闯关模式
	{
		piece = (unsigned)pLevelData->piece;
	}
	else if (mode == ModeBase::ModeEndless) //无尽模式
	{
		int boss = GameData::getInstance()->getValueToInt(GAMEDATA::ENDLESS_BOSS);
		int bossLast = GameData::getInstance()->getValueToInt(GAMEDATA::ENDLESS_BOSS_LAST);

		if (boss > bossLast) ++piece;
		if (boss >= 3) ++piece;
		if (boss >= 6) ++piece;
		if (boss >= 12) ++piece;

		GameData::getInstance()->setValue(GAMEDATA::ENDLESS_BOSS_LAST, boss);
	}

	if (vipFlag == 1 && piece > 0)
	{
		float tmp = piece * VipConstant::RandomPiece * Utils::getRandom();
		if (tmp < 1)
		{
			tmp = 1;
		}
		piece += tmp;
	}

	PlayerEquip::getInstance()->getRandomPiece(piece, m_equipPieceArray);
	//const std::vector<TEquipmentData*>& array1 = GlobalData::getInstance()->getEquipmentArray(EquipTypeArmature);
	//const std::vector<TEquipmentData*>& array2 = GlobalData::getInstance()->getEquipmentArray(EquipTypeMissile);
	//const std::vector<TEquipmentData*>& array3 = GlobalData::getInstance()->getEquipmentArray(EquipTypeWingman);
	//unsigned total = array1.size() + array2.size() + array3.size();
	//unsigned idx = 0;
	//m_equipPieceArray.clear();
	//for (unsigned i = 0; i < piece || m_equipPieceArray.size() < piece; i++)
	//{
	//	int random = total * Utils::getRandom();
	//	idx += random;
	//	if (idx < 0)
	//	{
	//		idx = 0;
	//	}
	//	while (idx >= total)
	//	{
	//		idx -= total;
	//	}

	//	if (idx < array1.size())
	//	{
	//		array1.at(idx)->pick++;
	//		m_equipPieceArray.insert(array1.at(idx));
	//	}
	//	else if (idx >= array1.size() && idx < array1.size() + array2.size())
	//	{
	//		int ii = idx - array1.size();
	//		array2.at(ii)->pick++;
	//		m_equipPieceArray.insert(array2.at(ii));
	//	}
	//	else if (idx >= array1.size() + array2.size() && idx < total)
	//	{
	//		int ii = idx - array1.size() - array2.size();
	//		array3.at(ii)->pick++;
	//		m_equipPieceArray.insert(array3.at(ii));
	//	}
	//}

	//显示获得碎片
	if (m_equipPieceArray.size() > 0)
	{
		m_pTxtVipPrompt->setVisible(true);
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
	else
	{
		m_pTxtVipPrompt->setVisible(false);
		m_pTxtGetPiece->setVisible(false);
	}

	//保存到数据库
	GameData::getInstance()->saveData();

	m_fEnemyCur = 0;
	m_fMoneyCur = 0;
	m_fScoreCur = 0;
	m_iScoreOver = 0;

	float a = m_iEnemy / 2.0 / 60.0f;
	float b = m_iScore / 2.0 / 60.0f;
	float c = m_iMoney / 2.0 / 60.0f;
	m_fTmp = MAX(a, MAX(b, c));

	//LableEnemy LableMoney LableScore
	setEnemy(0);
	setMoney(0);
	setScore(0);
	setMoneyVip(0);

	if (m_iScore > 0)
	{
		this->scheduleUpdate();

		Sound::playSound(SOUND::ID_SCORE, true);
	}

	return true;
}

void MenuSettle::updateVipState()
{
	int vipFlag = GameData::getInstance()->getValueToInt(GAMEDATA::VIP_FLAG);
	if (vipFlag) //是vip
	{
		m_pBtnVip->setVisible(false);
		m_pBtnContinue->setPositionX(Director::getInstance()->getWinSize().width / 2);
	}
	else
	{
		m_pSpriteMoneyVip->setSpriteFrame("Win_Txt4.png");
		m_pBtnVip->setVisible(true);
		m_pBtnContinue->setPositionX(m_btnContinuePosX);
	}

}

void MenuSettle::setEnemy(int enemy)
{
	char sz[20] = {0};
	snprintf(sz, 19, "%d", enemy);
	m_pLableEnemy->setString(sz);

}

void MenuSettle::setScore(int score)
{
	char sz[20] = { 0 };
	snprintf(sz, 19, "%d", score);
	m_pLableScore->setString(sz);
}

void MenuSettle::setMoney(int money)
{
	char sz[20] = { 0 };
	snprintf(sz, 19, "%d", money);
	m_pLableMoney->setString(sz);
}

void MenuSettle::setMoneyVip(int money)
{
	char sz[20] = { 0 };
	snprintf(sz, 19, "%d", money);
	m_pLableMoneyVip->setString(sz);
}

void MenuSettle::update(float dt)
{
	if ((int)m_fEnemyCur < m_iEnemy)
	{
		m_fEnemyCur += m_fTmp;
		if (fabs(m_fEnemyCur - m_iEnemy) < 0.1 || m_fEnemyCur > m_iEnemy)
		{
			setEnemy(m_iEnemy);
			++m_iScoreOver;
		}
		else
		{
			setEnemy((int)m_fEnemyCur);
		}
	}

	if (m_fMoneyCur < m_iMoney)
	{
		m_fMoneyCur += m_fTmp;
		if (fabs(m_fMoneyCur - m_iMoney) < 0.1 || m_fMoneyCur > m_iMoney)
		{
			setMoney(m_iMoney);
			++m_iScoreOver;
		}
		else
		{
			setMoney((int)m_fMoneyCur);
		}
	}

	if (m_fMoneyVipCur < m_iMoneyVip)
	{
		m_fMoneyVipCur += m_fTmp;
		if (fabs(m_fMoneyVipCur - m_iMoneyVip) < 0.1 || m_fMoneyVipCur > m_iMoneyVip)
		{
			setMoneyVip(m_iMoneyVip);
			++m_iScoreOver;
		}
		else
		{
			setMoneyVip((int)m_fMoneyVipCur);
		}
	}

	if (m_fScoreCur < m_iScore)
	{
		m_fScoreCur += m_fTmp;
		if (fabs(m_fScoreCur - m_iScore) < 0.1 || m_fScoreCur > m_iScore)
		{
			setScore(m_iScore);
			++m_iScoreOver;
		}
		else
		{
			setScore(m_fScoreCur);
		}
	}

	if (4 == m_iScoreOver)
	{
		Sound::stopAll();
		this->unscheduleUpdate();

		//加载动画： 
		//auto pAimAction = Preload::getInstance()->getUIAction("Settle.csb");;
		//m_pRootNode->runAction(pAimAction);
		//pAimAction->gotoFrameAndPlay(0, false);

		m_pStar1->setVisible(false);
		m_pStar2->setVisible(false);
		m_pStar3->setVisible(false);
		
		if (1 <= m_grade)
		{
			m_pStar1->setVisible(true);
			m_pStar1->setScale(6.0f);
			ScaleTo* pScaleTo = ScaleTo::create(0.2f, 1.0f);
			FiniteTimeAction* pDone = CallFunc::create([this](){
				Shake* shake = Shake::create(0.1f, 5);
				this->runAction(shake);

				if (2 <= m_grade)
				{
					m_pStar2->setVisible(true);
					m_pStar2->setScale(6.0f);
					ScaleTo* pScaleTo = ScaleTo::create(0.2f, 1.0f);
					FiniteTimeAction* pDone = CallFunc::create([this](){
						Shake* shake = Shake::create(0.1f, 5);
						this->runAction(shake);

						if (3 <= m_grade)
						{
							m_pStar3->setVisible(true);
							m_pStar3->setScale(6.0f);
							ScaleTo* pScaleTo = ScaleTo::create(0.2f, 1.0f);
							FiniteTimeAction* pDone = CallFunc::create([this](){
								Shake* shake = Shake::create(0.1f, 5);
								this->runAction(shake);
							});
							Sequence* pSeq = Sequence::create(pScaleTo, pDone, nullptr);
							m_pStar3->runAction(pSeq);
							Sound::playSound(SOUND::ID_SCORE_STAR);
						}
					});
					Sequence* pSeq = Sequence::create(pScaleTo, pDone, nullptr);
					m_pStar2->runAction(pSeq);
					Sound::playSound(SOUND::ID_SCORE_STAR);
				}
			});
			Sequence* pSeq = Sequence::create(pScaleTo, pDone, nullptr);
			m_pStar1->runAction(pSeq);
			Sound::playSound(SOUND::ID_SCORE_STAR);
		}

	}
}
