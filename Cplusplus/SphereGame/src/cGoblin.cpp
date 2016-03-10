#include "cGoblin.h"

Goblin::Goblin(LTextureSet* set, Vector2D r) :
	Warrior(set, 50, 50, 5, (int*)m_frame_offset, (int*)m_frame_number)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}

void Goblin::botActions(SDL_Rect& camera, const std::vector<Creature*>& enms, const std::vector<RObject*>& obstacles)
{
	if (!checkCollision(m_box, camera)) return;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * ATTACK LOGIC  * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (m_keep_action) return;

	SDL_Rect vertical_box_attack;
	vertical_box_attack.x = m_box.x;
	vertical_box_attack.y = m_box.y - 1.5 * m_box.h;
	vertical_box_attack.w = m_box.w;
	vertical_box_attack.h = 4 * m_box.h;

	SDL_Rect horizontal_box_attack;
	horizontal_box_attack.x = m_box.x - 1.5 * m_box.w;
	horizontal_box_attack.y = m_box.y;
	horizontal_box_attack.w = 4 * m_box.w;
	horizontal_box_attack.h = m_box.h;

	for (auto p_elem = enms.begin(); p_elem != enms.end(); p_elem++)
	{
		if (checkCollision((*p_elem)->getBox(), vertical_box_attack))
		{
			m_keep_action = true;
			m_keep_state = false;
			m_delta_x = m_delta_y = 0;
			m_state = (m_box.y < (*p_elem)->getBox().y) ? STATE_ATTACKDOWN : STATE_ATTACKUP;
			m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
			return;
		}
		else if (checkCollision((*p_elem)->getBox(), horizontal_box_attack))
		{
			m_keep_action = true;
			m_keep_state = false;
			m_delta_x = m_delta_y = 0;
			m_state = (m_box.x < (*p_elem)->getBox().x) ? STATE_ATTACKRIGHT : STATE_ATTACKLEFT;
			m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
			return;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * WALKING LOGIC * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (m_keep_state)
	{
		if (m_state.isStateWalk())
		{
			m_keep_state = !(m_frame_counter.getSettedFrame() == 2);
		}
		else if (m_state.isStateLook())
		{
			m_keep_state = false;
		}
	}
	if (m_keep_state) return;

	float min_dist = 0, dist = 0;
	for (auto p_elem = enms.begin(); p_elem != enms.end(); p_elem++)
	{
		Vector2D v1(m_box.x, m_box.y);
		Vector2D v2((*p_elem)->getBox().x, (*p_elem)->getBox().y);
		SDL_Rect box;

		min_dist = (v2 - v1).length();

		for (int i = -1; i <= 1; i += 2)
		{
			box = m_box;
			box.x += i * m_step;
			dist = (v1 + Vector2D(i * m_step, 0) - v2).length();

			if (dist <= min_dist && !isCollision(obstacles, &box))
			{
				m_delta_x = i * m_step;
				m_delta_y = 0;
				min_dist = dist;
			}
		}

		for (int j = -1; j <= 1; j += 2)
		{
			box = m_box;
			box.y += j * m_step;
			dist = (v1 + Vector2D(0, j * m_step) - v2).length();

			if (dist <= min_dist && !isCollision(obstacles, &box))
			{
				m_delta_x = 0;
				m_delta_y = j * m_step;
				min_dist = dist;
			}
		}
	}

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

	if (m_state != m_state_prev)
	{
		m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
	}
	m_state_prev = m_state;
	m_keep_state = true;
	m_keep_action = false;
}