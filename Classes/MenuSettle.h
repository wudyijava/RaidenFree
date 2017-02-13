#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

struct TEquipmentData;

class MenuSettle : public Layer
{
public:
	MenuSettle();

	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(MenuSettle);

	static void setBattleResult(bool result);

protected:
	void setEnemy(int enemy);
	void setScore(int score);
	void setMoney(int money);
	void setMoneyVip(int money);

	void updateVipState();

	virtual void update(float dt);

private:
	static bool m_bBattleResult;

	Node* m_pRootNode;

	Node* m_pWin;
	Node* m_pLose;

	ui::Button* m_pBtnContinue;
	ui::Button* m_pBtnUpgrade;
	ui::Button* m_pBtnVip;

	Sprite* m_pStar1;
	Sprite* m_pStar2;
	Sprite* m_pStar3;

	ui::TextAtlas* m_pLableEnemy;
	ui::TextAtlas* m_pLableScore;
	ui::TextAtlas* m_pLableMoney;
	Sprite* m_pSpriteMoneyVip;
	ui::TextAtlas* m_pLableMoneyVip;

	Sprite* m_pTxtGetPiece;
	Node* m_pNodePiece;

	Sprite* m_pTxtVipPrompt;

	int m_iEnemy;
	int m_iScore;
	int m_iMoney;
	int m_iMoneyVip;

	float m_fTmp;

	float m_fEnemyCur;
	float m_fScoreCur;
	float m_fMoneyCur;
	float m_fMoneyVipCur;
	
	std::set<const TEquipmentData*> m_equipPieceArray;

	int m_iScoreOver;
	float m_btnContinuePosX;

	int m_grade;
};