#include "cCreature.h"

void Creature::move(const Level& lvl, const std::vector<RObject*>& obstacles, const SDL_Rect* p_camera)
{
	if (m_keep_action) return;

	m_box.x += m_delta_x;
	if (lvl.outOfBoardersX(m_box) || isCollision(obstacles))
	{
		m_box.x -= m_delta_x;
	}
	else if (p_camera)
	{
		if (m_box.x < p_camera->x || m_box.x + m_box.w > p_camera->x + p_camera->w)
		{
			m_box.x -= m_delta_x;
		}
	}

	m_box.y += m_delta_y;
	if (lvl.outOfBoardersY(m_box) || isCollision(obstacles))
	{
		m_box.y -= m_delta_y;
	}
	else if (p_camera)
	{
		if (m_box.y < p_camera->y || m_box.y + m_box.h > p_camera->y + p_camera->h)
		{
			m_box.y -= m_delta_y;
		}
	}
}

SDL_Rect Creature::setRenderBox() const
{
	SDL_Rect box;
	box.x = m_box.x - 1.5 * m_box.w;
	box.y = m_box.y - 1.5 * m_box.h;
	box.w = 4 * m_box.w;
	box.h = 4 * m_box.h;
	return box;
}

int Creature::setNextFrame()
{
	int m_frame_delta = 0;

	if (!isAlive())
	{
		m_frame_delta = setDeathFrame();
	}
	else if (m_keep_state)
	{
		m_frame_delta = setSkillFrame();
	}
	else if (m_keep_action)
	{
		m_frame_delta = setAttackFrame();
	}
	else
	{
		m_frame_delta = setWalkFrame();
	}

	if (m_state.isLeft())
	{
		turnOnHorizontalFlip();
	}

	m_state_prev = m_state;
	return m_frame_offset[m_state.get()] + m_frame_delta;
}

int Creature::setWalkFrame()
{
	int m_frame_delta = 0;
	
	if (m_delta_x && m_delta_y)
	{
		if (m_state_prev.isUp())
		{
			m_state = STATE_WALKUP;
		}
		else if (m_state_prev.isDown())
		{
			m_state = STATE_WALKDOWN;
		}
		else if (m_state_prev.isLeft())
		{
			m_state = STATE_WALKLEFT;
		}
		else if (m_state_prev.isRight())
		{
			m_state = STATE_WALKRIGHT;
		}
	}
	else if (m_delta_y)
	{
		m_state = (m_delta_y > 0) ? STATE_WALKDOWN : STATE_WALKUP;
	}
	else if (m_delta_x)
	{
		m_state = (m_delta_x > 0) ? STATE_WALKRIGHT : STATE_WALKLEFT;
	}
	else
	{
		if (m_state.isUp())
		{
			m_state = STATE_LOOKUP;
		}
		else if (m_state.isDown())
		{
			m_state = STATE_LOOKDOWN;
		}
		else if (m_state.isLeft())
		{
			m_state = STATE_LOOKLEFT;
		}
		else if (m_state.isRight())
		{
			m_state = STATE_LOOKRIGHT;
		}
	}

	if (m_state.isStateWalk())
	{
		if (m_state_prev.isStateWalk())
		{
			if (m_state_prev == STATE_WALKUP && !(m_delta_y < 0) ||
				m_state_prev == STATE_WALKDOWN && !(m_delta_y > 0))
			{
				m_state = (m_delta_x > 0) ? STATE_WALKRIGHT : STATE_WALKLEFT;
			}
			else if (m_state_prev == STATE_WALKLEFT && !(m_delta_x < 0) ||
				m_state_prev == STATE_WALKRIGHT && !(m_delta_x > 0))
			{
				m_state = (m_delta_y > 0) ? STATE_WALKDOWN : STATE_WALKUP;
			}
			else if (m_state_prev.isStateSkill())
			{
				m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
			}
			else
			{
				m_state = m_state_prev;
			}
		}
		else
		{
			m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
		}
	}
	
	if (m_state != m_state_prev)
	{
		m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
	}

	m_frame_delta = m_frame_counter.getNextFrame();
	return m_frame_delta;
}

int Creature::setAttackFrame()
{
	int m_frame_delta = m_frame_counter.getNextFrame();
	m_keep_action = !m_frame_counter.isFinished();

	if (!m_keep_action)
	{
		if (m_state.isUp())
		{
			m_state = STATE_LOOKUP;
		}
		else if (m_state.isDown())
		{
			m_state = STATE_LOOKDOWN;
		}
		else if (m_state.isLeft())
		{
			m_state = STATE_LOOKLEFT;
		}
		else if (m_state.isRight())
		{
			m_state = STATE_LOOKRIGHT;
		}
		m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
	}

	return m_frame_delta;
}

int Creature::setSkillFrame()
{
	int m_frame_delta = 0;
	m_state = m_state_prev;

	if (m_delta_x || m_delta_y)
	{
		m_frame_delta = m_frame_counter.getNextFrame();
	}
	else
	{
		m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
	}

	return m_frame_delta;
}

int Creature::setDeathFrame()
{
	int m_frame_delta = m_frame_counter.getNextFrame();
	m_kill_me = m_frame_counter.isFinished();

	return m_frame_delta;
}

void Creature::getDamageExtern(int dmg, StateEnum state)
{
	m_keep_state = false;
	m_keep_action = true;
	setHealth(getHealth() - dmg);

	if (isAlive())
	{
		m_state = state;
		m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
	}
	else
	{
		m_state = STATE_DYING;
		m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
	}
}

// used for rendering
bool Creature::comparator(const Creature* a, const Creature* b)
{
	return (a->getBox().y < b->getBox().y);
}