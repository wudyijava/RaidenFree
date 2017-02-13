#include "Guide.h"
#include "Preload.h"
#include "GameData.h"
#include "Common/Macro.h"

////////////////////////////////////////////////////////////////
//
//  Guide
//
////////////////////////////////////////////////////////////////
Guide::Guide()
	: m_callback(nullptr)
	, m_pRoot(nullptr)
	, m_pTarget(nullptr)
	, m_targetParent(nullptr)
	, m_pTextPrompt(nullptr)
	, m_touchEnable(false)
{
}

Guide* Guide::show(int iGuideType, Node* pRoot, Node* pTarget, const std::function<void()>& callback)
{
	m_iGuideType = iGuideType;
	m_pRoot = pRoot;
	m_pTarget = pTarget;
	m_callback = callback;

	if (init())
	{
		return this;
	}
	else
	{
		return nullptr;
	}
}

void Guide::setPrompt(const std::string& prompt)
{
	m_prompt = prompt;
	if (m_pTextPrompt)
	{
		m_pTextPrompt->setString(m_prompt);
	}
}

//�����ϲ㴴��һ��GuideBg��Ȼ���target��ť��ԭ�ڵ�������������ӵ�GuideBg��
bool Guide::doInit()
{
	if (m_pTarget)
	{
		m_targetPos = m_pTarget->getPosition();
		m_targetParent = m_pTarget->getParent();
		m_targetLocalZOrder = m_pTarget->getLocalZOrder();

		//Vec2 pos = m_pRoot->convertToWorldSpace(m_pTarget->getPosition());
		Vec2 pos = m_pTarget->convertToWorldSpace(Vec2::ZERO);
		pos += m_pTarget->getAnchorPointInPoints();

		//��target�Ƶ���layer��
		m_pRoot->addChild(this);
		m_pTarget->retain();
		//m_pTarget->removeFromParent();//���ﲻ��remove�������������
		this->addChild(m_pTarget);
		m_pTarget->release();
		m_pTarget->setPosition(pos);

		//m_pTarget������λ�ã�����ê�����ת����
		float deltaX = (0.5 - m_pTarget->getAnchorPoint().x) * m_pTarget->getAnchorPointInPoints().x / m_pTarget->getAnchorPoint().x;
		float deltaY = (0.5 - m_pTarget->getAnchorPoint().y) * m_pTarget->getAnchorPointInPoints().y / m_pTarget->getAnchorPoint().y;

		//���ָʾ��ͷ
		Sprite* pArrow = Sprite::createWithSpriteFrameName("Guide_Arrow.png");
		const Size& targetSize = m_pTarget->getContentSize();
		const Size& arrowSize = pArrow->getContentSize();
		bool bArrowTop = true;

		Node* pDialogBox = nullptr;
		Sprite* pDialogBoxBg = nullptr;
		ui::Text* m_pTextPrompt = nullptr;
		if (m_prompt.length() > 0)
		{
			pDialogBox = Preload::getInstance()->getUI("GameUI_Dialogbox.csb");
			FIND_UI_CONTROL_RE(ui::Text*, "Text_Content", m_pTextPrompt, pDialogBox);
			FIND_UI_CONTROL_RE(Sprite*, "GameUI_Dialogbox_Bg", pDialogBoxBg, pDialogBox);
			m_pTextPrompt->setTextAreaSize(Size(360, 110));
			m_pTextPrompt->ignoreContentAdaptWithSize(false);
		}
		if (m_pTarget->getPositionY() < CONSTANT::DESIGN_RES_HEIGHT / 2 + 100)
		{
			//��ͷ�ڿؼ����Ϸ�
			bArrowTop = true;
			pArrow->setPosition(m_pTarget->getPosition().x + deltaX,
				m_pTarget->getPosition().y + targetSize.height / 2 + arrowSize.height / 2 + 20 + deltaY);
		
			if (pDialogBox)
			{
				pDialogBox->setPosition(CONSTANT::DESIGN_RES_WIDTH / 2, CONSTANT::DESIGN_RES_HEIGHT - pDialogBoxBg->getContentSize().height / 2);
				m_pTextPrompt->setString(m_prompt);
				this->addChild(pDialogBox);
			}
		}
		else
		{
			//��ͷ�ڿؼ����·�
			bArrowTop = false;
			pArrow->setFlippedY(true);
			pArrow->setPosition(m_pTarget->getPosition().x + deltaX,
				m_pTarget->getPosition().y - targetSize.height / 2 - arrowSize.height / 2 - 20 + deltaY);
			
			if (pDialogBox)
			{
				pDialogBox->setPosition(CONSTANT::DESIGN_RES_WIDTH / 2, pDialogBoxBg->getContentSize().height / 2);
				m_pTextPrompt->setString(m_prompt);
				this->addChild(pDialogBox);
			}
		}
		this->addChild(pArrow);

		//��ͷ����
		DelayTime* pDelay = DelayTime::create(0.4f);
		MoveBy* pMove1 = MoveBy::create(0.15f, Vec2(0, -10.0f));
		MoveBy* pMove2 = MoveBy::create(0.15f, Vec2(0, 10.0f));
		Sequence* pSeq = nullptr;
		if (bArrowTop)
		{
			pSeq = Sequence::create(pDelay, pMove1, pMove2, nullptr);
		}
		else
		{
			pSeq = Sequence::create(pDelay, pMove2, pMove1, nullptr);
		}
		RepeatForever* pRepeat = RepeatForever::create(pSeq);
		pArrow->runAction(pRepeat);
		m_touchEnable = false;
	}
	else if (!m_pTarget && m_prompt.length() > 0)
	{
		auto pNode = Preload::getInstance()->getUI("BeautyGuide.csb");

		this->addChild(pNode);

		FIND_UI_CONTROL_RE(ui::Text*, "Prompt_Text", m_pTextPrompt, pNode);

		m_pTextPrompt->ignoreContentAdaptWithSize(false);
		m_pTextPrompt->setTextAreaSize(Size(480, 150));
		m_pTextPrompt->setString(m_prompt);

		auto *pAction = Preload::getInstance()->getUIAction("BeautyGuide.csb");
		pNode->runAction(pAction);
		pAction->gotoFrameAndPlay(0, false);

		m_pRoot->addChild(this);
		m_touchEnable = true;
	}
	else
	{
		m_pRoot->addChild(this);
		m_touchEnable = true;
	}

	//this->setOpacity(200);
	return true;
}

//��ԭtarget
void Guide::restore()
{
	if (m_pTarget)
	{
		m_pTarget->retain();

		//m_pTarget->removeFromParent();//���ﲻ��remove�������������
		this->getChildren().eraseObject(m_pTarget);
		m_targetParent->addChild(m_pTarget);
		m_pTarget->setLocalZOrder(m_targetLocalZOrder);
		m_pTarget->setPosition(m_targetPos);

		m_pTarget->release();
	}

	this->removeFromParent();
}

void Guide::onTouchEnded(Touch *touch, Event *event)
{
	if (m_touchEnable)
	{
		GuideMgr::getInstance()->decGuideCnt();
		//removeFromParent();
		if (m_callback)
		{
			m_callback();
		}
		GuideMgr::getInstance()->destory(m_iGuideType);
	}
}

void Guide::onEnter()
{
	GuideMgr::getInstance()->incGuideCnt();

	Node::onEnter();
}

void Guide::onExit()
{
	//���ﲻ�ܼ�onExit����Ȼ����ᵼ�³��������ԭ��δ֪
	//Node::onExit();
	
	GuideMgr::getInstance()->decGuideCnt();

	GameData::getInstance()->addGuide(m_iGuideType);
	GameData::getInstance()->saveData();
}
////////////////////////////////////////////////////////////////
//
//  GuideMgr
//
////////////////////////////////////////////////////////////////
GuideMgr* GuideMgr::m_pInstance = nullptr;

GuideMgr* GuideMgr::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new GuideMgr();
	}

	return m_pInstance;
}

GuideMgr::GuideMgr()
 : m_iCnt(0)
{

}

bool GuideMgr::registerFunc(int type, const std::function<bool()>& checkFunc, const std::function<Guide*()>& createFunc)
{
	m_condMap.insert(std::map<int, std::function<bool()>>::value_type(type, checkFunc));
	m_createMap.insert(std::map<int, std::function<Guide*()>>::value_type(type, createFunc));

	return true;
}

Guide* GuideMgr::show(int guideType, Node* parent, Node* widget, const std::function<void()>& callback)
{
	if (hasGuide())
	{
		return nullptr;
	}

	auto itCond = m_condMap.find(guideType);
	auto itCreate = m_createMap.find(guideType);
	if (itCond != m_condMap.end() && itCreate != m_createMap.end())
	{
		if (itCond->second())
		{
			Guide* pGuide = itCreate->second();
			pGuide->autorelease();

			if (pGuide)
			{
				auto it = m_objectMap.find(guideType);
				if (it != m_objectMap.end())
				{
					m_objectMap.erase(it);
				}
				m_objectMap.insert(std::map<int, Guide*>::value_type(guideType, pGuide));
				return pGuide->show(guideType, parent, widget, callback);
			}
		}
	}

	return nullptr;
}

bool GuideMgr::destory(int GuideType)
{
	auto it = m_objectMap.find(GuideType);
	if (it != m_objectMap.end())
	{
		it->second->restore();
		m_objectMap.erase(it);

		return true;
	}
	else
	{
		return false;
	}
	
}

void GuideMgr::incGuideCnt()
{ 
	++m_iCnt; 
}

void GuideMgr::decGuideCnt()
{ 
	--m_iCnt; 
	if (m_iCnt < 0)
	{
		m_iCnt = 0;
	}
}

bool GuideMgr::hasGuide()
{ 
	return m_iCnt > 0; 
}
