#pragma once

#include "cCreature.h"

class Warrior : public Creature
{
public:
	Warrior(LTextureSet* set, int hp, int ep, int dp, int* fo, int* fn) :
		Creature(set, hp, ep, fo, fn), m_damage(dp) {}

	void attack(const std::vector<Creature*>& enms);

	virtual ~Warrior() {}
private:
	virtual SDL_Rect setAttackBox() const;
private:
	SDL_Rect m_box_attack;

	int m_damage;
};

#include "cBullet.h"

class Archer : public Creature
{
public:
	Archer(LTextureSet* set, LTextureSet* armor, int hp, int ep, int dp, int* fo, int* fn) :
		Creature(set, hp, ep, fo, fn), m_damage(dp), m_armor_set(armor) {}

	template<typename BulletType>
	void attack(std::vector<Bullet*>& blts);
	
	virtual ~Archer() {}
private:
	LTextureSet* m_armor_set;

	int m_damage;
};