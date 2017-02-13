#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

/*******************************************************************************

Ѫ��

*******************************************************************************/
class HpBar : public Node
{
public:

	//barArray: �ײ�ͼƬ��ǰ�棬�ϲ���ں���
	//����loadingbar
	static HpBar* createWithLoadingBar(Sprite* bg);
	void addHpBar(ui::LoadingBar* pLoadingBar);

	//����progressbar
	static HpBar* createWithTexture(Node* parent, std::vector<std::string>& barArray, bool vertical);

	void reset();

	void setMaxValue(float max);
	void setCurValue(float current);

	virtual void setVisible(bool visible) override;

protected:
	HpBar();
	virtual ~HpBar();

	//����loadingbar
	bool init(Sprite* bg);

	//����progressbar
	bool init(Node* parent, std::vector<std::string>& barArray, bool vertical);
	void setTexture(const char* pBackground, const char* pBar);

private:
	//���ֵ
	float m_fMax;
	//��ǰֵ
	float m_fCurrent;

	//********************************//
	//����loadingbar
	//********************************//
	Sprite* m_bg;
	std::vector<ui::LoadingBar*> m_barArray;

	//********************************//
	//����progressbar
	//********************************//
	//����ͼƬ
	const char* m_pBackground;
	//Ѫ��ͼƬ
	const char* m_pBar;
	//�Ǵ�ֱ�ģ�����ˮƽ
	bool m_bVertical;

	Sprite* m_bgSprite;
	Sprite* m_hpSprite;

	std::vector<Sprite*> m_hpSpriteList;
	std::vector<ProgressTimer*> m_hpProgressList;

	ProgressTimer* m_pProgressTimer;
};
