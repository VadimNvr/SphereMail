#include "cBullet.h"

Bullet::Bullet(LTextureSet* set, int dmg, State st, int* fo, int* fn) :
	AObject(set, fo, fn), m_damage(dmg), m_kill_me(false), m_collided(false)
{
	m_state = st;

	m_delta_x = m_delta_y = 0;
	if (m_state.isDown())
	{
		m_delta_y = m_step;
	}
	else if (m_state.isUp())
	{
		m_delta_y = -m_step;
	}
	else if (m_state.isRight())
	{
		m_delta_x = m_step;
	}
	else if (m_state.isLeft())
	{
		m_delta_x = -m_step;
	}
}

void Bullet::move(Level lvl, const std::vector<RObject*> obstacles)
{
	if (m_state.isStateDying()) return;

	m_box.x += m_delta_x;
	if (lvl.outOfBoardersX(m_box) || isCollision(obstacles))
	{
		m_box.x -= m_delta_x;
		m_collided = true;
	}

	m_box.y += m_delta_y;
	if (lvl.outOfBoardersY(m_box) || isCollision(obstacles))
	{
		m_box.y -= m_delta_y;
		m_collided = true;
	}
}

void Bullet::attack(const std::vector<Creature*>& enms)
{
	if (!m_collided || !m_state.isStateAttack()) return;

	m_box_attack = setAttackBox();

	for (auto p_elem = enms.begin(); p_elem != enms.end(); p_elem++)
	{
		if ((*p_elem)->isAlive() && checkCollision((*p_elem)->getBox(), m_box_attack))
		{
			if (m_state.isUp())
			{
				(*p_elem)->getDamageExtern(m_damage, STATE_DAMAGEDOWN);
			}
			else if (m_state.isDown())
			{
				(*p_elem)->getDamageExtern(m_damage, STATE_DAMAGEUP);
			}
			else if (m_state.isLeft())
			{
				(*p_elem)->getDamageExtern(m_damage, STATE_DAMAGERIGHT);
			}
			else if (m_state.isRight())
			{
				(*p_elem)->getDamageExtern(m_damage, STATE_DAMAGELEFT);
			}
			m_state = STATE_DYING;
		}
	}
}

SDL_Rect Bullet::setRenderBox() const
{
	SDL_Rect box;
	box.x = m_box.x - 0.5 * m_box.w;
	box.y = m_box.y - 0.5 * m_box.h;
	box.w = 2 * m_box.w;
	box.h = 2 * m_box.h;
	return box;
}

SDL_Rect Bullet::setAttackBox() const
{
	SDL_Rect box;
	box.x = m_box.x - 0.5 * m_box.w;
	box.y = m_box.y - 0.5 * m_box.h;
	box.w = 2 * m_box.w;
	box.h = 2 * m_box.h;
	return box;
}

int Bullet::setNextFrame()
{
	int m_frame_delta = 0;

	if (m_collided) m_state = STATE_DYING;

	if (m_state != m_state_prev)
	{
		m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
	}

	if (m_state.isStateDying())
	{
		m_frame_delta = m_frame_counter.getNextFrame();
		m_kill_me = m_frame_counter.isFinished();
	}
	else
	{
		m_frame_delta = m_frame_counter.getNextFrame();
	}

	if (m_state.isLeft())
	{
		turnOnHorizontalFlip();
	}

	m_state_prev = m_state;
	return m_frame_offset[m_state.get()] + m_frame_delta;
}

FireBall::FireBall(LTextureSet* set, Vector2D r, State st, int dp) :
	Bullet(set, dp, st, (int*)m_frame_offset, (int*)m_frame_number)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}

Arrow::Arrow(LTextureSet* set, Vector2D r, State st, int dp) :
	Bullet(set, dp, st, (int*)m_frame_offset, (int*)m_frame_number)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}