#pragma once

//������Ǯ����ɱ�Ż����ӵ���������������
class IListener
{
public:
	virtual void updateValue(const std::string& type, int value) = 0;
};

//��ҷɻ�HP�仯������
class IHpListener
{
public:
	virtual void displayHp() = 0;
	virtual void hideHp() = 0;

	virtual void updateHp(int hp) = 0;
	virtual void updateMaxHp(int hp) = 0;
};

//BOSS HP�仯������
class IBossHpListener
{
public:
	virtual void displayBossHp() = 0;
	virtual void hideBossHp() = 0;

	virtual void updateBossHp(int hp) = 0;
	virtual void updateBossMaxHp(int hp) = 0;
};

//�ӵ�����
class IBulletUseUpListener
{
public:
	virtual void bulletUseUp() = 0;
};
