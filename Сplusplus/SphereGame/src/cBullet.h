#pragma once

#include "cCreature.h"

class Bullet : public AObject {
public:
	Bullet(LTextureSet* set, int dmg, State st, int* fo, int* fn);

	void move(Level lvl, const std::vector<RObject*> obstacles);
	void attack(const std::vector<Creature*>& enms);
	bool isReadyToBeDeleted() const { return m_kill_me; }

	virtual ~Bullet() {}
private:
	SDL_Rect setAttackBox() const;
	SDL_Rect setRenderBox() const;
	int setNextFrame();
	bool isObstacle() const { return m_state.isStateAttack(); }
private:
	static const int m_step = 5;

	int m_delta_x, m_delta_y;

	SDL_Rect m_box_attack;
	int m_damage;

	bool m_kill_me, m_collided;
};

class FireBall : public Bullet {
public:
	FireBall(LTextureSet* set, Vector2D r, State st, int dp);
public:
	static const int m_width = 16;
	static const int m_height = 16;
private:
	const int m_frame_offset[number_states] = {
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 0,  3,  6,  6,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 9
	};

	const int m_frame_number[number_states] = {
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 3,  3,  3,  3,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 6
	};
};

class Arrow : public Bullet {
public:
	Arrow(LTextureSet* set, Vector2D r, State st, int dp);
public:
	static const int m_width = 16;
	static const int m_height = 16;
private:
	const int m_frame_offset[number_states] = {
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 1,  0,  2,  2,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 3
	};

	const int m_frame_number[number_states] = {
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 1,  1,  1,  1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		 1
	};
};