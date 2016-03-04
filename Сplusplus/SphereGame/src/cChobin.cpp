#include "cChobin.h"

Chobin::Chobin(LTextureSet* set, LTextureSet* arrow, Vector2D r) :
	Archer(set, arrow, 50, 50, 5, (int*)m_frame_offset, (int*)m_frame_number)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}

void Chobin::botActions(SDL_Rect& camera, const std::vector<Creature*>& enms, const std::vector<RObject*>& obstacles)
{
	if (!checkCollision(m_box, camera)) return;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	* * * * * * * * * * * * * * * * * * * * * * ATTACK LOGIC  * * * * * * * * * * * * * * * * * * * * * * *
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (m_keep_action) return;

	SDL_Rect vertical_box_attack;
	vertical_box_attack.x = m_box.x;
	vertical_box_attack.y = camera.y;
	vertical_box_attack.w = m_box.w;
	vertical_box_attack.h = camera.h;

	SDL_Rect horizontal_box_attack;
	horizontal_box_attack.x = camera.x;
	horizontal_box_attack.y = m_box.y;
	horizontal_box_attack.w = camera.w;
	horizontal_box_attack.h = m_box.h;

	for (auto p_elem = enms.begin(); p_elem != enms.end(); p_elem++)
	{
		if (checkCollision((*p_elem)->getBox(), vertical_box_attack))
		{
			SDL_Rect vertical_box = vertical_box_attack;

			int y1 = m_box.y,
				y2 = (*p_elem)->getBox().y,
				h1 = m_box.h,
				h2 = (*p_elem)->getBox().h;


			if (y1 < y2)
			{
				vertical_box.y = y1 + h1;
				vertical_box.h = y2 - vertical_box.y;
			}
			else
			{
				vertical_box.y = y2 + h2;
				vertical_box.h = y1 - vertical_box.y;
			}

			if (!isCollision(obstacles, &vertical_box))
			{
				m_keep_action = true;
				m_keep_state = false;
				m_delta_x = m_delta_y = 0;
				m_state = (y1 < y2) ? STATE_ATTACKDOWN : STATE_ATTACKUP;
				m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
				return;
			}
		}
		else if (checkCollision((*p_elem)->getBox(), horizontal_box_attack))
		{
			SDL_Rect horizontal_box = horizontal_box_attack;

			int x1 = m_box.x,
				x2 = (*p_elem)->getBox().x,
				w1 = m_box.w,
				w2 = (*p_elem)->getBox().w;


			if (x1 < x2)
			{
				horizontal_box.x = x1 + w1;
				horizontal_box.w = x2 - horizontal_box.x;
			}
			else
			{
				horizontal_box.x = x2 + w2;
				horizontal_box.w = x1 - horizontal_box.x;
			}

			if (!isCollision(obstacles, &horizontal_box))
			{
				m_keep_action = true;
				m_keep_state = false;
				m_delta_x = m_delta_y = 0;
				m_state = (x1 < x2) ? STATE_ATTACKRIGHT : STATE_ATTACKLEFT;
				m_frame_counter.resetCounter(m_frame_number[m_state.get()]);
				return;
			}
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

		if (fabs((v2 - v1).x) < fabs((v2 - v1).y))
		{
			for (int i = -1; i <= 1; i += 2)
			{
				box = m_box;
				box.x += i * m_step;

				dist = (v1 + Vector2D(i * m_step, 0) - v2).length();

				if (dist <= min_dist && !isCollision(obstacles, &box))
				{
					m_delta_x = i * m_step;
					m_delta_y = 0;
					min_dist = 0;
				}
			}
		}
		else
		{
			for (int j = -1; j <= 1; j += 2)
			{
				box = m_box;
				box.y += j * m_step;
				dist = (v1 + Vector2D(0, j * m_step) - v2).length();

				if (dist <= min_dist && !isCollision(obstacles, &box))
				{
					m_delta_x = 0;
					m_delta_y = j * m_step;
					min_dist = 0;
				}
			}
		}

		if (min_dist < 0.01)
		{
			for (int i = -1; i <= 1; i += 2)
			{
				box = m_box;
				box.x += i * m_step;

				dist = (v1 + Vector2D(i * m_step, 0) - v2).length();

				if (dist <= min_dist && !isCollision(obstacles, &box))
				{
					m_delta_x = i * m_step;
					m_delta_y = 0;
					min_dist = 0;
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
					min_dist = 0;
				}
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