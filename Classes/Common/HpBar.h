#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

/*******************************************************************************

血槽

*******************************************************************************/
class HpBar : public Node
{
public:

	//barArray: 底层图片在前面，上层的在后面
	//采用loadingbar
	static HpBar* createWithLoadingBar(Sprite* bg);
	void addHpBar(ui::LoadingBar* pLoadingBar);

	//采用progressbar
	static HpBar* createWithTexture(Node* parent, std::vector<std::string>& barArray, bool vertical);

	void reset();

	void setMaxValue(float max);
	void setCurValue(float current);

	virtual void setVisible(bool visible) override;

protected:
	HpBar();
	virtual ~HpBar();

	//采用loadingbar
	bool init(Sprite* bg);

	//采用progressbar
	bool init(Node* parent, std::vector<std::string>& barArray, bool vertical);
	void setTexture(const char* pBackground, const char* pBar);

private:
	//最大值
	float m_fMax;
	//当前值
	float m_fCurrent;

	//********************************//
	//采用loadingbar
	//********************************//
	Sprite* m_bg;
	std::vector<ui::LoadingBar*> m_barArray;

	//********************************//
	//采用progressbar
	//********************************//
	//背景图片
	const char* m_pBackground;
	//血条图片
	const char* m_pBar;
	//是垂直的，还是水平
	bool m_bVertical;

	Sprite* m_bgSprite;
	Sprite* m_hpSprite;

	std::vector<Sprite*> m_hpSpriteList;
	std::vector<ProgressTimer*> m_hpProgressList;

	ProgressTimer* m_pProgressTimer;
};
