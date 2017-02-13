#pragma once

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

//������������Ķ�������ɻ����л���װ����
class GameObject : public Sprite
{
public:
	GameObject();

	static Sprite* createSpriteWithFileList(const std::vector<std::string>& fileList, float dura);

	bool initSpriteWithFileList(const std::vector<std::string>& fileList, float dura);
	bool initArmature(const std::string& armatureName, float scale);
	virtual void reset();
	virtual void destory();

	virtual void pause() override;
	virtual void resume() override;
	void pause(Node *pNode);
	void resume(Node *pNode);

	virtual void setBodySize(const Size& s);
	virtual void setBodySize(float w, float h);
	virtual const Size& getBodySize();
	virtual const Size& getOrignBodySize() const;

	virtual void setBodyCenter(const Vec2& v);
	virtual void setBodyCenter(float x, float y);
	virtual const Vec2& getBodyCenter() const;

	//��ȡ���������µ�body��λ�úʹ�С
	virtual Rect getBodyBox() const;

	void startAnimate();

protected:
	//������ײ���ĸ����С��λ��
	Vec2 m_bodyCenter; //��������ĵ����꣨����ھ���ê������꣩
	Size m_bodySize;   //����Ŀ��
	Size m_orignBodySize; //����ĳ�ʼ���
	bool m_isBodySizeSet; //�Ƿ����ù����

	cocostudio::Armature* m_pArmature;
	Sequence* m_pAnimateSequence;
};

//���������ͬ����Ķ��������ӵ�
class GameObjectContainer : public GameObject
{
public:
	GameObjectContainer();
	virtual ~GameObjectContainer();

	virtual void setParent(Node* parent);

	inline __Array* getAllObject() { return m_pAllObject; }

	virtual void destory();

	virtual void setBodySize(const Size& s);
	virtual void setBodySize(float w, float h);
	virtual const Size& getBodySize() const;

	virtual void setBodyCenter(const Vec2& v);
	virtual void setBodyCenter(float x, float y);
	virtual const Vec2& getBodyCenter() const;

	//��ȡ���������µ�body��λ�úʹ�С
	virtual Rect getBodyBox(int index) const;

private:
	virtual Rect getBodyBox() const { return Rect(0, 0, 0, 0); }

protected:
	//������ͬ���͵�GameObject�����Դ�С��һ���ģ�����λ�ÿ�����Ϊ��ת��ԭ��᲻һ��
	__Array* m_pAllObject;
};