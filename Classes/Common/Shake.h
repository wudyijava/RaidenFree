#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Shake : public ActionInterval
{
public:
	Shake();

	// Create the action with a time and a strength (same in x and y)
	// ������Ч���ĳ�ʼ����������,����������ͬ
	// @param d �𶯳�����ʱ��
	// @param strength �𶯵ķ���
	static Shake* create(float d, float strength);
	// Create the action with a time and strengths (different in x and y)
	// ������Ч���ĳ�ʼ������,��������ֵ��һ��
	static Shake* create(float d, float strength_x, float strength_y);
	bool initWithDuration(float d, float strength_x, float strength_y);

	//���¶�����д���������ĺ���(������д)
	virtual Shake* clone() const override;
	virtual Shake* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
	virtual void stop(void);

protected:
	// Initial position of the shaked node
	// �����λ��
	float _initial_x, _initial_y;
	// Strength of the action
	// �����ķ���
	float _strength_x, _strength_y;
};