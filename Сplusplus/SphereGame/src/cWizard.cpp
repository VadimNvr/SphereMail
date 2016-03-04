#include "cWizard.h"

Wizard::Wizard(LTextureSet* set, LTextureSet* fire, Vector2D r, SDL_Keycode* keys) :
	Archer(set, fire, 150, 150, 10, (int*)m_frame_offset, (int*)m_frame_number), m_action_keys(keys)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}

void Wizard::handleEvent(const SDL_Event& e)
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
			if (getEnergy() == getEnergyMax()) return;

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

int Wizard::setSkillFrame()
{
	int m_frame_delta = 0;

	if (m_delta_x && m_delta_y)
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
	}
	else if (m_delta_y)
	{
		m_state = (m_delta_y > 0) ? STATE_WALKDOWN_SKILL : STATE_WALKUP_SKILL;
	}
	else if (m_delta_x)
	{
		m_state = (m_delta_x > 0) ? STATE_WALKRIGHT_SKILL : STATE_WALKLEFT_SKILL;
	}
	else
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
	}

	if (m_state.isStateWalk())
	{
		if (m_state_prev.isStateWalk())
		{
			if (m_state_prev == STATE_WALKUP_SKILL && !(m_delta_y < 0) ||
				m_state_prev == STATE_WALKDOWN_SKILL && !(m_delta_y > 0))
			{
				m_state = (m_delta_x > 0) ? STATE_WALKRIGHT_SKILL : STATE_WALKLEFT_SKILL;
			}
			else if (m_state_prev == STATE_WALKLEFT_SKILL && !(m_delta_x < 0) ||
				m_state_prev == STATE_WALKRIGHT_SKILL && !(m_delta_x > 0))
			{
				m_state = (m_delta_y > 0) ? STATE_WALKDOWN_SKILL : STATE_WALKUP_SKILL;
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
	m_keep_state = (getEnergy() != getEnergyMax());
	return m_frame_delta;
}