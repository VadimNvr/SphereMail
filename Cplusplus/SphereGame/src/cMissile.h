#pragma once

#include "cCreature.h"
#include "cTile.h"
#include "cLevel.h"
#include "cRObject.h"
#include "cFrameCounter.h"

#include "cState.h"

class Missile : public AObject {
public:
	Missile(LTextureSet* set, StateEnum s, int* fo, int* fn, int dmg);

	void update(const SDL_Rect& camera);
	void move(const std::vector<RObject*>& tiles, const std::vector<Creature*>& enms);

	bool isObstacle() { return !m_state.isStateDying(); }
	bool isReadyToBeDeleted() const { return m_kill_me; }

	virtual ~Missile() {}
private:
	SDL_Rect setRenderBox() { return m_box; }
	int setNextFrame() { return m_frame_offset[m_state.get()] + m_frame_counter.getNextFrame(); }
protected:
	int m_delta_x, m_delta_y;
private:
	static const int m_width = 32;
	static const int m_height = 32;
	static const int m_step = 3;

	int m_damage;
	State m_state;
	bool m_keep_state;

	bool m_kill_me;
};

class FireBall : public Missile {
public:
	FireBall(LTextureSet* set, StateEnum state, int damage) :
		Missile(set, state, (int*)m_frame_offset, (int*)m_frame_number, damage) {}
private:
	const int m_frame_offset[number_states] = {
		-1, -1, -1, -1,
		 0,  3,  6,  6,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 9
	};

	const int m_frame_number[number_states] = {
		-1, -1, -1, -1,
		 3,  3,  3,  3,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 6
	};
};