#include "PopupLayer.h"

PopupLayer::PopupLayer()
	: m_pListener(nullptr)
	, m_pPlaneLayer(nullptr)
{
}

PopupLayer::~PopupLayer()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(m_pListener);
}

bool PopupLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}

	m_pListener = EventListenerTouchOneByOne::create();
	m_pListener->setSwallowTouches(true);
	m_pListener->onTouchBegan = getTouchBeganFunc();
	m_pListener->onTouchMoved = getTouchMovedFunc();
	m_pListener->onTouchEnded = getTouchEndedFunc();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

	setColor(Color3B(0, 0, 0));
	setOpacity(160);

	return doInit();
}

void PopupLayer::setPlaneLayer(Layer* layer)
{
	m_pPlaneLayer = layer;
}

Layer* PopupLayer::getPlaneLayer()
{
	return m_pPlaneLayer;
}


void PopupLayer::pausePlaneLayer()
{
	if (m_pPlaneLayer)
	{
		m_pPlaneLayer->pause();
	}
}

void PopupLayer::resumePlaneLayer()
{
	if (m_pPlaneLayer)
	{
		m_pPlaneLayer->resume();
	}
}

bool PopupLayer::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void PopupLayer::onTouchMoved(Touch *touch, Event *event)
{

}

void PopupLayer::onTouchEnded(Touch* touch, Event* event)
{

}
