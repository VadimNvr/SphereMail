#include "cKnight.h"

Knight::Knight(LTextureSet* set, Vector2D r, SDL_Keycode* keys) : 
	Warrior(set, 150, 150, 10, (int*)m_frame_offset, (int*)m_frame_number), m_action_keys(keys)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}

void Knight::handleEvent(const SDL_Event& e)
{
	if (!isAlive()) return;

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{		
		if (m_keep_action) return;

		SDL_Keycode key = e.key.keysym.sym;
		if (key == m_action_keys[ACTION_WALKUP])
		{
			if (m_delta_y == 0)
			{
				m_delta_y = -m_step;
				m_state = STATE_WALKUP;
			}
		}
		else if (key == m_action_keys[ACTION_WALKDOWN])
		{
			if (m_delta_y == 0)
			{
				m_delta_y = m_step;
				m_state = STATE_WALKDOWN;
			}
		}
		else if (key == m_action_keys[ACTION_WALKLEFT])
		{
			if (m_delta_x == 0)
			{
				m_delta_x = -m_step;
				m_state = STATE_WALKLEFT;
			}
		}
		else if (key == m_action_keys[ACTION_WALKRIGHT])
		{
			if (m_delta_x == 0)
			{
				m_delta_x = m_step;
				m_state = STATE_WALKRIGHT;
			}
		}
		else if (key == m_action_keys[ACTION_ATTACK])
		{
			if (m_keep_state || getEnergy() < m_attack_cost) return;
			
			setEnergy(getEnergy() - m_attack_cost);
			if (m_state.isUp())
			{
				m_state = STATE_ATTACKUP;
			}
			else if (m_state.isDown())
			{
				m_state = STATE_ATTACKDOWN;
			}
			else if (m_state.isLeft())
			{
				m_state = STATE_ATTACKLEFT;
			}
			else if (m_state.isRight())
			{
				m_state = STATE_ATTACKRIGHT;
			}
			m_keep_action = true;
			m_keep_state = false;
			m_frame_counter.resetCounter(m_frame_number[m_state.get()], 4);
		}
		else if (key == m_action_keys[ACTION_SKILL])
		{
			if (m_state.isUp())
			{
				m_state = STATE_LOOKUP_SKILL;
			}
			else if (m_state.isDown())
			{
				m_state = STATE_LOOKDOWN_SKILL;
			}
			else if (m_state.isLeft())
			{
				m_state = STATE_LOOKLEFT_SKILL;
			}
			else if (m_state.isRight())
			{
				m_state = STATE_LOOKRIGHT_SKILL;
			}
			m_keep_action = false;
			m_keep_state = true;
			m_state_prev = m_state;
			m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{		
		SDL_Keycode key = e.key.keysym.sym;
		if (key == m_action_keys[ACTION_WALKUP])
		{
			if (m_delta_y < 0)
			{
				m_delta_y = 0;
				m_state = STATE_LOOKUP;
			}
		}
		else if (key == m_action_keys[ACTION_WALKDOWN])
		{
			if (m_delta_y > 0)
			{
				m_delta_y = 0;
				m_state = STATE_LOOKDOWN;
			}
		}
		else if (key == m_action_keys[ACTION_WALKLEFT])
		{
			if (m_delta_x < 0)
			{
				m_delta_x = 0;
				m_state = STATE_LOOKLEFT;
			}
		}
		else if (key == m_action_keys[ACTION_WALKRIGHT])
		{
			if (m_delta_x > 0)
			{
				m_delta_x = 0;
				m_state = STATE_LOOKRIGHT;
			}
		}
		else if (key == m_action_keys[ACTION_SKILL])
		{
			m_keep_state = false;
		}
	}

	if (m_keep_state) m_state = m_state_prev;
}

void Knight::getDamageExtern(int dmg, StateEnum state)
{
	if (getEnergy() > 0 && m_keep_state && (
		m_state.isLeft() && STATE_DAMAGELEFT == state ||
		m_state.isRight() && STATE_DAMAGERIGHT ==  state ||
		m_state.isUp() && STATE_DAMAGEUP == state ||
		m_state.isDown() && STATE_DAMAGEDOWN == state))
	{
		setEnergy(getEnergy() - dmg);
	}
	else 
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
}

int Knight::setSkillFrame()
{
	int m_frame_delta = 0;

	m_state = m_state_prev;

	if (m_delta_x || m_delta_y)
	{
		if (m_state_prev.isUp())
		{
			m_state = STATE_WALKUP_SKILL;
		}
		else if (m_state_prev.isDown())
		{
			m_state = STATE_WALKDOWN_SKILL;
		}
		else if (m_state_prev.isLeft())
		{
			m_state = STATE_WALKLEFT_SKILL;
		}
		else if (m_state_prev.isRight())
		{
			m_state = STATE_WALKRIGHT_SKILL;
		}

		if (!m_state_prev.isStateWalk())
		{
			m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
		}
		else
		{
			m_frame_delta = m_frame_counter.getNextFrame();
		}
	}
	else
	{
		if (m_state_prev.isUp())
		{
			m_state = STATE_LOOKUP_SKILL;
		}
		else if (m_state_prev.isDown())
		{
			m_state = STATE_LOOKDOWN_SKILL;
		}
		else if (m_state_prev.isLeft())
		{
			m_state = STATE_LOOKLEFT_SKILL;
		}
		else if (m_state_prev.isRight())
		{
			m_state = STATE_LOOKRIGHT_SKILL;
		}

		if (m_state_prev.isStateWalk())
		{
			m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
		}
		else
		{
			m_frame_delta = m_frame_counter.getNextFrame();
		}
	}
	return m_frame_delta;
}

SDL_Rect Knight::setAttackBox()
{
	SDL_Rect box = m_box;
	if (m_state.isRight())
	{
		box.x = m_box.x;
		box.y = m_box.y - m_box.h;
		box.w = 2 * m_box.w;
		box.h = 3 * m_box.h;
	}
	else if (m_state.isLeft())
	{
		box.x = m_box.x - m_box.w;
		box.y = m_box.y - m_box.h;
		box.w = 2 * m_box.w;
		box.h = 3 * m_box.h;
	}
	else if (m_state.isUp())
	{
		box.x = m_box.x - m_box.w;
		box.y = m_box.y - m_box.h;
		box.w = 3 * m_box.w;
		box.h = 2 * m_box.h;
	}
	else if (m_state.isDown())
	{
		box.x = m_box.x - m_box.w;
		box.y = m_box.y;
		box.w = 3 * m_box.w;
		box.h = 2 * m_box.h;
	}
	return box;
}