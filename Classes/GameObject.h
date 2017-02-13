#pragma once

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

//包含单个精灵的对象，例如飞机、敌机、装备等
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

	//获取世界坐标下的body的位置和大小
	virtual Rect getBodyBox() const;

	void startAnimate();

protected:
	//用于碰撞检测的刚体大小和位置
	Vec2 m_bodyCenter; //刚体的中心点坐标（相对于精灵锚点的坐标）
	Size m_bodySize;   //刚体的宽高
	Size m_orignBodySize; //刚体的初始宽高
	bool m_isBodySizeSet; //是否设置过宽高

	cocostudio::Armature* m_pArmature;
	Sequence* m_pAnimateSequence;
};

//包含多个相同精灵的对象，例如子弹
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

	//获取世界坐标下的body的位置和大小
	virtual Rect getBodyBox(int index) const;

private:
	virtual Rect getBodyBox() const { return Rect(0, 0, 0, 0); }

protected:
	//包含相同类型的GameObject，所以大小是一样的，但是位置可能因为旋转的原因会不一样
	__Array* m_pAllObject;
};