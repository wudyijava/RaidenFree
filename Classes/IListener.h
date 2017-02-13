#pragma once

//分数、钱、必杀僚机、子弹晶化数量监听器
class IListener
{
public:
	virtual void updateValue(const std::string& type, int value) = 0;
};

//玩家飞机HP变化监听器
class IHpListener
{
public:
	virtual void displayHp() = 0;
	virtual void hideHp() = 0;

	virtual void updateHp(int hp) = 0;
	virtual void updateMaxHp(int hp) = 0;
};

//BOSS HP变化监听器
class IBossHpListener
{
public:
	virtual void displayBossHp() = 0;
	virtual void hideBossHp() = 0;

	virtual void updateBossHp(int hp) = 0;
	virtual void updateBossMaxHp(int hp) = 0;
};

//子弹用完
class IBulletUseUpListener
{
public:
	virtual void bulletUseUp() = 0;
};
