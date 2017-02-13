#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PopupLayer.h"

USING_NS_CC;

class RemoteRapidDistance;
struct TEquipmentData;

class MenuSettleRapid : public PopupLayer
{
public:
	MenuSettleRapid();

	virtual bool doInit();
	static Scene* scene();
	CREATE_FUNC(MenuSettleRapid);

	void setCurDistance(int d);
	void setBestDistance(int d);
	void getRemoteRank();

	void getPiece();

	virtual void removeFromParent() override;

private:
	Node* m_pRootNode;
	cocostudio::timeline::ActionTimeline* m_pRootAction;

	ui::Button* m_pBtnQuit;
	ui::Button* m_pBtnRetry;

	ui::TextBMFont* m_pLableCurrent;
	ui::TextBMFont* m_pLableBest;
	ui::TextBMFont* m_pLableRank;

	Sprite* m_pSpriteDistanceRank;
	Sprite* m_pSpriteConnectingWeb;
	Sprite* m_pSpriteConnectFailed;

	int m_iCurrentDistance;
	int m_iBestDistance;
	int m_iRank1;
	int m_iRank2;

	Sprite* m_pTxtGetPiece;
	Node* m_pNodePiece;

	RemoteRapidDistance* m_pRemote;
	bool m_bRemoteAvailable;

	std::set<const TEquipmentData*> m_equipPieceArray;
};