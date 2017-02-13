#pragma once

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

struct TLevelBackground;

class BackgroundListener
{
public:
	virtual void onEnd() = 0;
	virtual void onMove(int zorder, float dt) = 0;
};

/*******************************************************************************

�Զ������ı�������

*******************************************************************************/
class ScrolledBackground : public Node
{
public:
	static ScrolledBackground* create(const TLevelBackground* pTLevelBackground, const Size& size);

	void regBgListener(BackgroundListener* pListener);
	void unRegBgListener(BackgroundListener* pListener);
	void notifyEnd();
	void notifyMove(int zorder, float dt);

	//������ʼ��y���ꡣĬ����0��������������ͬ�ı�������ʱ���ڶ�������Ӧ�����ڵ�һ���������ϱ�Ե
	void run();
	void stop();
	void addBackground(const TLevelBackground* pTLevelBackground);
	void removeBackground();

	void constant(bool c); //����

protected:
	ScrolledBackground();
	virtual ~ScrolledBackground();
	virtual bool init(const TLevelBackground* pTLevelBackground, const Size& size);
	void calculateDelta(const TLevelBackground* pTLevelBackground);

private:
	void update(float dt);

	void incBgCnt(const std::string& name, int value);
	void removeBgCnt(const std::string& name);
	int getBgCnt(const std::string& name);

private:
	const TLevelBackground* m_pTLevelBackground;
	Size m_size;
	bool m_bRemove;

	std::vector<BackgroundListener*> m_bgListenerArray;

	std::list<Sprite*> m_backgroundArray;
	std::map<std::string, int> m_levelBgCntMap;

	float m_fDelta;

	//����
	float m_fDeltaDelta;
	int m_iFrameCnt;

	bool m_bConstantSpeed;
};
