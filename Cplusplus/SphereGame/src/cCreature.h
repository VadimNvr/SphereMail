#pragma once

#include "cRObject.h"

#include "cState.h"
#include "cLevel.h"
#include "cVector.h"

struct CreatureInfo
{
    int m_delta_x, m_delta_y;
    
    bool m_keep_action; // mutex should be used in animation to protect from moving actions
    // e.g. attack and damage animations
    bool m_keep_state;	// mutex should be used in animation to protect from changing state
    // e.g. skill animation
    bool m_kill_me;		// should be set true once before deleting. for scene managment

    int m_health_max;
    int m_energy_max;
    
    int m_health;
    int m_energy;
    
    CreatureInfo (int, int);
    CreatureInfo() {};
};

class Creature : public AObject {
protected:		// I don't want you create Creature objects
	Creature(LTextureSet* set, int hp, int ep, int* fo, int* fn) :
		AObject(set, fo, fn),
		m_health(hp), m_energy(ep),
		m_health_max(hp), m_energy_max(ep),
		m_delta_x(0), m_delta_y(0),
		m_keep_action(false), m_keep_state(false), m_kill_me(false) {}
public:
	bool isAlive() const { return (getHealth() > 0); }
	bool isReadyToBeDeleted() const { return m_kill_me; }
	
	static bool comparator(const Creature* a, const Creature* b);

	void move(const Level& lvl, const std::vector<RObject*>& obstacles, const SDL_Rect* p_camera = NULL);
	
	virtual void getDamageExtern(int dmg, StateEnum state);

	void setHealth(int hp) { m_health = (hp < m_health_max) ? hp : m_health_max; }
	void setEnergy(int ep) { m_energy = (ep < m_energy_max) ? ep : m_energy_max; }
	void setHealthMax(int hp) { m_health_max = hp; if (hp < m_health) m_health = hp; }
	void setEnergyMax(int ep) { m_energy_max = ep; if (ep < m_energy) m_energy = ep; }
    void setPos(SDL_Rect &);

	int getHealth() const { return m_health; }
	int getEnergy() const { return m_energy; }
	int getHealthMax() const { return m_health_max; }
	int getEnergyMax() const { return m_energy_max; }
    
    CreatureInfo getInfo() const { return CreatureInfo(m_delta_x, m_delta_y); };
    SDL_Rect getPos();

	virtual ~Creature() {}
private:
	bool isObstacle() const { return isAlive(); }

	virtual SDL_Rect setRenderBox() const;
	int setNextFrame();

	virtual int setWalkFrame();
	virtual int setSkillFrame();
	virtual int setAttackFrame();
	virtual int setDeathFrame();
protected:				// protected for the virtual functions above
	int m_delta_x, m_delta_y;

	bool m_keep_action; // mutex should be used in animation to protect from moving actions
						// e.g. attack and damage animations
	bool m_keep_state;	// mutex should be used in animation to protect from changing state
						// e.g. skill animation
	bool m_kill_me;		// should be set true once before deleting. for scene managment
private:
	int m_health_max;
	int m_energy_max;

	int m_health;
	int m_energy;
};