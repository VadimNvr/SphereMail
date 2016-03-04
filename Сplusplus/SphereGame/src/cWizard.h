#pragma once

#include "globals.h"
#include "cCreatureTypes.h"

class Wizard : public Archer {
public:
	Wizard(LTextureSet* set, LTextureSet* fire, Vector2D r, SDL_Keycode* keys);

	void handleEvent(const SDL_Event& e);
	void reloadEnergy(int ep) { if (m_state.isStateSkill()) Creature::setEnergy(ep); }
private:
	SDL_Rect setAttackBox() { return m_box; }

	int setSkillFrame();
public:
	static const int m_width = 16;
	static const int m_height = 16;
	static const int m_step = 1;
	static const int m_attack_cost = 10;
private:
	const int m_frame_offset[number_states] = {
		 0, 14, 28, 28,
		 1, 15, 29, 29,
		12, 26, 40, 40,
		 5, 19, 33, 33,
		 9, 23, 37, 37,
		10, 24, 38, 38,
		42
	};

	const int m_frame_number[number_states] = {
		1, 1, 1, 1,
		4, 4, 4, 4,
		2, 2, 2, 2,
		4, 4, 4, 4,
		2, 2, 2, 2,
		2, 2, 2, 2,
		1
	};

	const SDL_Keycode *m_action_keys;
};