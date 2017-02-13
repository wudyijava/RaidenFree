#include "GameObject.h"
#include "Common/Macro.h"
#include "GlobalData.h"
#include "Armature.h"

//***************************************************
//
//   GameObject
//
//***************************************************

GameObject::GameObject()
{
	reset();
}

void GameObject::reset()
{
	m_isBodySizeSet = false;
	m_pAnimateSequence = nullptr;
	m_pArmature = nullptr;
}

void GameObject::destory()
{
	if (m_pAnimateSequence)
	{
		m_pAnimateSequence->release();
	}
	removeFromParent();
}

void GameObject::pause()
{
	this->pause(this);
}

void GameObject::resume()
{
	this->resume(this);
}

void GameObject::pause(Node *pNode)
{
	Node::pause();

	for (auto p : pNode->getChildren())
	{
		p->pause();
	}
}

void GameObject::resume(Node *pNode)
{
	Node::resume();

	for (auto p : pNode->getChildren())
	{
		p->resume();
	}
}

void GameObject::setBodySize(const Size& s)
{
	if (!m_isBodySizeSet)
	{
		m_isBodySizeSet = true;
		m_orignBodySize = s;
	}
	m_bodySize = s;
}

void GameObject::setBodySize(float w, float h)
{
	setBodySize(Size(w, h));
}

const Size& GameObject::getBodySize()
{
	if (!m_isBodySizeSet)
	{
		m_isBodySizeSet = true;
		m_bodySize = m_orignBodySize;
	}
	return m_bodySize;
}

//保存原始的刚体尺寸。	
//用处：当导弹旋转超过45°时，宽高值交换
const Size& GameObject::getOrignBodySize() const
{
	return m_orignBodySize;
}

void GameObject::setBodyCenter(const Vec2& v)
{
	m_bodyCenter = v;
}

void GameObject::setBodyCenter(float x, float y)
{
	m_bodyCenter = Vec2(x, y);
}

const Vec2& GameObject::getBodyCenter() const
{
	return m_bodyCenter;
}

//获取世界坐标下的body的位置和大小
Rect GameObject::getBodyBox() const
{
	Vec2 pos = getPosition();

	return Rect(pos.x + m_bodyCenter.x - m_bodySize.width * getAnchorPoint().x,
		pos.y + m_bodyCenter.y - m_bodySize.height * getAnchorPoint().y,
		m_bodySize.width,
		m_bodySize.height);
}

Sprite* GameObject::createSpriteWithFileList(const std::vector<std::string>& fileList, float dura)
{
	if (fileList.size() == 0)
	{
		return nullptr;
	}

	Sprite* pSprite = Sprite::createWithSpriteFrameName(fileList.at(0));

	//动画
	if (fileList.size() > 1)
	{
		Animation* animation = Animation::create();
		animation->setDelayPerUnit(dura);
		for (unsigned i = 0; i < fileList.size(); i++)
		{
			SpriteFrame* pFrame = CCSpriteFrameCache::getInstance()->getSpriteFrameByName(fileList[i]);
			if (NULL == pFrame)
			{
				continue;
			}
			animation->addSpriteFrame(pFrame);
		}

		//设置重复
		Animate* animate = Animate::create(animation);
		Repeat* repeat = Repeat::create(animate, CC_REPEAT_FOREVER);
		//Sequence* sequence = Sequence::create(repeat, NULL);
		pSprite->runAction(repeat);
	}

	return pSprite;
}

bool GameObject::initSpriteWithFileList(const std::vector<std::string>& fileList, float dura)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileList.at(0));
	if (NULL == frame)
	{
		DEBUG_LOG("Error get frame of '%s'", fileList.at(0).c_str());
		CCASSERT(frame, "Error get frame");
	}
	Sprite::initWithSpriteFrame(frame);

	//动画
	if (fileList.size() > 1)
	{
		Animation* animation = Animation::create();
		animation->setDelayPerUnit(dura);
		for (unsigned i = 0; i < fileList.size(); i++)
		{
			SpriteFrame* pFrame = CCSpriteFrameCache::getInstance()->getSpriteFrameByName(fileList[i]);
			if (NULL == pFrame)
			{
				continue;
			}
			animation->addSpriteFrame(pFrame);
		}

		//设置重复
		Animate* animate = Animate::create(animation);
		Repeat* repeat = Repeat::create(animate, CC_REPEAT_FOREVER);
		m_pAnimateSequence = Sequence::create(repeat, NULL);
		m_pAnimateSequence->retain();
		runAction(m_pAnimateSequence);
	}

	return true;
}

bool GameObject::initArmature(const std::string& armatureName, float scale)
{
	if (armatureName.length() <= 0)
	{
		return true;
	}

	m_pArmature = cocostudio::Armature::create(armatureName);
	m_pArmature->setPosition(getContentSize() / 2);
	m_pArmature->getAnimation()->play(GlobalData::getInstance()->getArmatureData(armatureName)->defaultAction);
	m_pArmature->setScale(scale);

	addChild(m_pArmature);

	return true;
}

void GameObject::startAnimate()
{
	if (m_pAnimateSequence != NULL)
	{
		this->runAction(m_pAnimateSequence);
	}
}
//***************************************************
//
//   GameObjectContainer
//
//***************************************************

GameObjectContainer::GameObjectContainer()
{
	m_pAllObject = __Array::create();
	m_pAllObject->retain();
}

GameObjectContainer::~GameObjectContainer()
{
	m_pAllObject->release();
}

void GameObjectContainer::setParent(Node* parent)
{
	Node::setParent(parent);
	for (Ref* r : *m_pAllObject)
	{
		((Node*)r)->setParent(parent);
	}
}
void GameObjectContainer::destory()
{
	for (Ref* r : *m_pAllObject)
	{
		((GameObject*)r)->destory();
	}
	m_pAllObject->removeAllObjects();
}

void GameObjectContainer::setBodySize(const Size& s)
{
	m_bodySize = s;

	for (Ref* r : *m_pAllObject)
	{
		((GameObject*)r)->setBodySize(s);
	}
}

void GameObjectContainer::setBodySize(float w, float h)
{
	setBodySize(Size(w, h));
}

const Size& GameObjectContainer::getBodySize() const
{
	return m_bodySize;
}

void GameObjectContainer::setBodyCenter(const Vec2& v)
{
	m_bodyCenter = v;

	for (Ref* r : *m_pAllObject)
	{
		((GameObject*)r)->setBodyCenter(v);
	}
}

void GameObjectContainer::setBodyCenter(float x, float y)
{
	setBodyCenter(Size(x, y));
}

const Vec2& GameObjectContainer::getBodyCenter() const
{
	return m_bodyCenter;
}

//获取世界坐标下的body的位置和大小
Rect GameObjectContainer::getBodyBox(int index) const
{
	if (index >= m_pAllObject->count())
	{
		CCASSERT(NULL, "[GameObjectContainer::getBodyBox]index is too large");
	}

	
	Vec2 pos = ((GameObject*)m_pAllObject->getObjectAtIndex(index))->getPosition();

	return Rect(pos.x + m_bodyCenter.x - m_bodySize.width / 2,
		pos.y + m_bodyCenter.y - m_bodySize.height / 2,
		m_bodySize.width,
		m_bodySize.height);
}