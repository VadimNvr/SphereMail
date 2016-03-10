#pragma once

#include "globals.h"
#include "cCreatureTypes.h"

class Knight: public Warrior {
public:
	Knight(LTextureSet* set, Vector2D r, SDL_Keycode* keys);

	void handleEvent(const SDL_Event& e);
	void reloadEnergy(int ep) { if (!m_state.isStateSkill()) Creature::setEnergy(ep); }
	
	void getDamageExtern(int dmg, StateEnum state);
private:
	SDL_Rect setAttackBox();

	int setSkillFrame();
public:
	static const int m_width = 16;
	static const int m_height = 16;
	static const int m_step = 1;
	static const int m_attack_cost = 10;
private:
	const int m_frame_offset[number_states] = {
		 0, 13, 26, 26,
		 1, 14, 27, 27,
		11, 24, 37, 37,
		 5, 18, 31, 31,
		 9, 22, 35, 35,
		 9, 22, 35, 35,
		39
	};

	const int m_frame_number[number_states] = {
		1, 1, 1, 1,
		4, 4, 4, 4,
		2, 2, 2, 2,
		4, 4, 4, 4,
		1, 1, 1, 1,
		2, 2, 2, 2,
		1
	};

	const SDL_Keycode *m_action_keys;
};