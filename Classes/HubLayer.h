#pragma once

#include "cocos2d.h"
#include "IListener.h"
#include "BaseLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class HpBar;
class PlaneLayer;

class HubLayer : public BaseLayer,
	public IHpListener,
	public IBossHpListener,
	public IListener
{
public:

	HubLayer();
	virtual ~HubLayer();

	static HubLayer* create(PlaneLayer* layer);

	virtual bool init(PlaneLayer* layer);

	virtual void displayHp() override;
	virtual void hideHp() override;
	virtual void updateHp(int hp) override;
	virtual void updateMaxHp(int hp) override;

	void initBossHp();
	virtual void displayBossHp() override;
	virtual void hideBossHp() override;
	virtual void updateBossHp(int hp) override;
	virtual void updateBossMaxHp(int hp) override;

	virtual void updateValue(const std::string& type, int value) override;

	void displayKm(bool display);
	void displayCombo(bool display);
	void updateKm(int num);
	void updateCombo(int num);
	void updateMoneyPresent(int num);

	void initGiftAni();
	void displayNewGift();
	void displayHugGift(bool bAnimation = true);

	void menuPauseCallback(Ref* pSender);
	void menuGiftCallback(Ref* pSender);
	void menuShopCallback(Ref* pSender);

	void menuKillerCallback(Ref* pSender);
	void menuShieldCallback(Ref* pSender);

	void menuBuyNewGift(Ref* pSender);
	void menuBuyHugGift(Ref* pSender);

	void displayGuideShield();
	void displayGuideKiller();

protected:

	void updateScore(int score);
	void updateMoney(int money);
	void updateLife(int life);

	void updateKiller(int cnt);
	void updateShield(int cnt);

	void makeSkillCD(const std::string& filename, ui::Widget* pMenuItem, float fSecond);

private:
	PlaneLayer* m_pPlaneLayer;

	Node* m_pHubRootNode;

	ui::Button* m_pBtnPause;
	ui::Button* m_pBtnShop;

	ui::Button* m_pBtnKiller;
	ui::Button* m_pBtnShield;

	ui::LoadingBar* m_pPlayerHpBar;

	Sprite* m_pBossHpBg;
	ui::LoadingBar* m_pBossHp1;
	ui::LoadingBar* m_pBossHp2;
	ui::LoadingBar* m_pBossHp3;

	ui::TextAtlas* m_pTextScore;
	ui::TextAtlas* m_pTextMoney;
	ui::TextAtlas* m_pTextLife;

	ui::TextAtlas* m_pTextKiller;
	ui::TextAtlas* m_pTextShield;

	ui::TextAtlas* m_pTextComboNum; //连杀
	Sprite* m_pSpriteCombo;

	ui::TextAtlas* m_pTextMoneyPresent; //连杀赠送的宝石
	Sprite* m_pSpriteMoneyPresent;
	Vec2 m_pSpriteMoneyPresentPos;

	ui::TextAtlas* m_pTextKmNum;    //飞行距离
	Sprite* m_pSpriteKm;

	//礼包
	Sprite* m_pSpriteNewGift;
	ui::Button* m_pBtnNewGift;

	Node* m_pHugGiftNode;
	ui::Button* m_pBtnHugGift;

	int m_iMaxHp;

	HpBar* m_pBossHpBar;
};
