#pragma once

#pragma once

#include "cCreatureTypes.h"

class Chobin : public Archer {
public:
	Chobin(LTextureSet* set, LTextureSet* arrow, Vector2D r);

	void botActions(SDL_Rect& camera, const std::vector<Creature*>& enms, const std::vector<RObject*>& tiles);
public:
	static const int m_width = 16;
	static const int m_height = 16;
	static const int m_step = 1;

	const int m_frame_offset[number_states] = {
		5, 15, 25, 25,
		0, 10, 20, 20,
		30, 40, 50, 50,
		60, 70, 80, 80,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		90
	};

	const int m_frame_number[number_states] = {
		1,  1,  1,  1,
		6,  6,  6,  6,
		6,  6,  6,  6,
		10, 10, 10, 10,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		10
	};
};