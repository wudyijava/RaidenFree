#include "Killer.h"
#include "GlobalData.h"
#include "PlaneLayer.h"
#include "Bullet.h"
#include "GameData.h"
#include "Equipment.h"
#include "Reinforce.h"
#include "Sound.h"


//***********************************************************//
//
//  ������ɱ
//
//***********************************************************//
namespace SuperKillerConstant
{
	const int SoundPlayerLaser = 31;
}

SuperKiller* SuperKiller::create(Node* pParent, const TAircraftData* pData)
{
	SuperKiller *pRet = new(std::nothrow) SuperKiller();
	if (pRet && pRet->init(pParent, pData))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool SuperKiller::init(Node* parent, const TAircraftData* pData, const TAircraftLevelData* pLevelData)
{
	bool bRet = false;
	do
	{
		m_eAircraftType = EAircraftType::Type_Killer;
		this->setLocalZOrder(CONSTANT::ZORDER_KILLER);

		//����:��ɱ�Ż�Ⱥ�Ĺ���ǿ������ҷɻ���ǿ������һ��
		int playerId = GameData::getInstance()->getValueToInt(GAMEDATA::PLAYER);
		m_pEquipAircraft = PlayerEquip::getInstance()->getEquip(playerId, EquipTypeAircraft);
		if (m_pEquipAircraft && m_pEquipAircraft->grade)
		{
			//ǿ������
			auto array = GlobalData::getInstance()->getReinforce(m_pEquipAircraft->reinforce_id);
			TReinforceGradeData* pReinforceGradeData = array.at(m_pEquipAircraft->grade - 1);
			setAttackAdjust(pReinforceGradeData->value);
		}

		//��ʼ���л�
		CC_BREAK_IF(!Aircraft::init(parent, pData, pLevelData));

		setNoDie(true);

		setPosition(CONSTANT::DESIGN_RES_WIDTH / 2, -getContentSize().height / 2);

		bRet = true;
	} while (0);

	return bRet;
}

void SuperKiller::fly()
{
	//���Ϸɡ���ʼ������ȴ�x�롢ֹͣ��������·�
	MoveBy* pMoveUp = MoveBy::create(0.5f, Vec2(0, getContentSize().height));
	//��ʼ���
	FiniteTimeAction* pActionStartShoot = CallFunc::create([this](){
		Sound::playSound(SuperKillerConstant::SoundPlayerLaser, false);
		this->startShoot();
	});
	//�ȴ�x��
	DelayTime* pDelay = DelayTime::create(2.f);
	//ֹͣ���
	FiniteTimeAction* pActionStopShoot = CallFunc::create([this](){
		this->stopShoot();
	});
	//���·�
	MoveBy* pMoveDown = MoveBy::create(0.5f, Vec2(0, -getContentSize().height));
	//��ʧ
	FiniteTimeAction* pActionHide = CallFunc::create([this](){
		destory();

		//֪ͨ��Layerɾ���������
		Node* p = getParent();
		PlaneLayer* layer = (PlaneLayer*)(getParent());
		CCASSERT(layer, "layer is not PlaneLayer");

		layer->destoryPlane(this);

	});

	Vector<FiniteTimeAction*> actionArray;
	actionArray.pushBack(pMoveUp);
	actionArray.pushBack(pActionStartShoot);
	actionArray.pushBack(pDelay);
	actionArray.pushBack(pActionStopShoot);
	actionArray.pushBack(pMoveDown);
	actionArray.pushBack(pActionHide);

	Sequence* sequence = Sequence::create(actionArray);

	this->runAction(sequence);
}