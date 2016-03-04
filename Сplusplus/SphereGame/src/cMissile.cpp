#include "cMissile.h"

Missile::Missile(LTextureSet* set, StateEnum s, int* fo, int* fn, int dmg) :
	AObject(set, fo, fn), m_damage(dmg), m_kill_me(false), m_keep_state(false)
{
	m_state = s;
	if (m_state.isUp()) {
		m_delta_y = -m_step;
	}
	else if (m_state.isDown())
	{
		m_delta_y = m_step;
	}
	else if (m_state.isRight())
	{
		m_delta_x = m_step;
	}
	else if (m_state.isLeft())
	{
		m_delta_x = -m_step;
	}
	m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
}

void Missile::update(const SDL_Rect& camera) {
	if (m_state.isStateDying())
	{
		m_kill_me = m_frame_counter.isFinished();
	}
	else if (!checkCollision(m_box, camera))
	{
		m_kill_me = true;
	}
}

void Missile::move(const std::vector<RObject*>& tiles, const std::vector<Creature*>& enms)
{
	if (m_keep_state) return;

	m_box.x += m_delta_x;
	if (isCollision(tiles))
	{
		m_box.x -= m_delta_x;
		m_state = STATE_DYING;
		m_delta_x = 0;
		m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
	}
	else
	{
		for (std::vector<Creature*>::const_iterator p_elem = enms.begin(); p_elem != enms.end(); p_elem++)
		{
			if (checkCollision((*p_elem)->getBox(), m_box))
			{
				(*p_elem)->getDamageExtern(m_damage, (m_delta_x > 0) ? STATE_DAMAGELEFT : STATE_DAMAGERIGHT);
				m_state = STATE_DYING;
				m_delta_x = 0;
				m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
			}
		}
	}

	m_box.y += m_delta_y;
	if (isCollision(tiles))
	{
		m_box.y -= m_delta_y;
		m_state = STATE_DYING;
		m_delta_y = 0;
		m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
	}
	else
	{
		for (std::vector<Creature*>::const_iterator p_elem = enms.begin(); p_elem != enms.end(); p_elem++)
		{
			if (checkCollision((*p_elem)->getBox(), m_box))
			{
				(*p_elem)->getDamageExtern(m_damage, (m_delta_y > 0) ? STATE_DAMAGEUP : STATE_DAMAGEDOWN);
				m_state = STATE_DYING;
				m_delta_y = 0;
				m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
			}
		}
	}
}