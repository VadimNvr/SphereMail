#include "cCreatureTypes.h"

void Warrior::attack(const std::vector<Creature*>& enms)
{
	if (!(m_state.isStateAttack() &&
		static_cast<int>(m_frame_number[m_state.get()] * 0.6) ==
		m_frame_counter.getSettedFrame()))
		return;

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
		}
	}
}

SDL_Rect Warrior::setAttackBox() const
{
	SDL_Rect box = m_box;
	if (m_state.isRight())
	{
		box.x = m_box.x;
		box.y = m_box.y;
		box.w = 2.5 * m_box.w;
		box.h = m_box.h;
	}
	else if (m_state.isLeft())
	{
		box.x = m_box.x - 1.5 * m_box.w;
		box.y = m_box.y;
		box.w = 2.5 * m_box.w;
		box.h = m_box.h;
	}
	else if (m_state.isUp())
	{
		box.x = m_box.x;
		box.y = m_box.y - 1.5 * m_box.h;
		box.w = m_box.w;
		box.h = 2.5 * m_box.h;
	}
	else if (m_state.isDown())
	{
		box.x = m_box.x;
		box.y = m_box.y;
		box.w = m_box.w;
		box.h = 2.5 * m_box.h;
	}
	return box;
}

template<typename BulletType>
void Archer::attack(std::vector<Bullet*>& blts)
{
	// it's a drawback of engineering
}

template<>
void Archer::attack<FireBall>(std::vector<Bullet*>& blts)
{
	if (!(m_state.isStateAttack() &&
		static_cast<int>(m_frame_number[m_state.get()] * 0.6) ==
		m_frame_counter.getSettedFrame()))
		return;

	Vector2D pos;
	State state;

	if (m_state.isUp())
	{
		pos = Vector2D(m_box.x, m_box.y - 1.5 * m_box.h);
		state = STATE_ATTACKUP;
	}
	else if (m_state.isDown())
	{
		pos = Vector2D(m_box.x, m_box.y + 1.5 * m_box.h);
		state = STATE_ATTACKDOWN;
	}
	else if (m_state.isLeft())
	{
		pos = Vector2D(m_box.x - 1.5 * m_box.w, m_box.y);
		state = STATE_ATTACKLEFT;
	}
	else if (m_state.isRight())
	{
		pos = Vector2D(m_box.x + 1.5 * m_box.w, m_box.y);
		state = STATE_ATTACKRIGHT;
	}
	blts.push_back(new FireBall(m_armor_set, pos, state, m_damage));
}

template<>
void Archer::attack<Arrow>(std::vector<Bullet*>& blts)
{
	if (!(m_state.isStateAttack() && m_frame_counter.getSettedFrame() == m_frame_number[m_state.get()] / 2))
		return;

	Vector2D pos;
	State state;

	if (m_state.isUp())
	{
		pos = Vector2D(m_box.x, m_box.y - 1.5 * m_box.h);
		state = STATE_ATTACKUP;
	}
	else if (m_state.isDown())
	{
		pos = Vector2D(m_box.x, m_box.y + 1.5 * m_box.h);
		state = STATE_ATTACKDOWN;
	}
	else if (m_state.isLeft())
	{
		pos = Vector2D(m_box.x - 1.5 * m_box.w, m_box.y);
		state = STATE_ATTACKLEFT;
	}
	else if (m_state.isRight())
	{
		pos = Vector2D(m_box.x + 1.5 * m_box.w, m_box.y);
		state = STATE_ATTACKRIGHT;
	}
	blts.push_back(new Arrow(m_armor_set, pos, state, m_damage));
}