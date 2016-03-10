#include "cState.h"

bool State::isDown() const
{
	return (STATE_LOOKDOWN == m_state || STATE_ATTACKDOWN == m_state ||
		STATE_WALKDOWN == m_state || STATE_DAMAGEDOWN == m_state ||
		STATE_LOOKDOWN_SKILL == m_state || STATE_WALKDOWN_SKILL == m_state);
}

bool State::isUp() const
{
	return (STATE_LOOKUP == m_state || STATE_ATTACKUP == m_state ||
		STATE_WALKUP == m_state || STATE_DAMAGEUP == m_state ||
		STATE_LOOKUP_SKILL == m_state || STATE_WALKUP_SKILL == m_state);
}

bool State::isRight() const
{
	return (STATE_LOOKRIGHT == m_state || STATE_ATTACKRIGHT == m_state ||
		STATE_WALKRIGHT == m_state || STATE_DAMAGERIGHT == m_state ||
		STATE_LOOKRIGHT_SKILL == m_state || STATE_WALKRIGHT_SKILL == m_state);
}

bool State::isLeft() const
{
	return (STATE_LOOKLEFT == m_state || STATE_ATTACKLEFT == m_state ||
		STATE_WALKLEFT == m_state || STATE_DAMAGELEFT == m_state ||
		STATE_LOOKLEFT_SKILL == m_state || STATE_WALKLEFT_SKILL == m_state);
}

bool State::isStateLook() const
{
	return (STATE_LOOKDOWN == m_state || STATE_LOOKUP == m_state ||
		STATE_LOOKRIGHT == m_state || STATE_LOOKLEFT == m_state ||
		STATE_LOOKDOWN_SKILL == m_state || STATE_LOOKUP_SKILL == m_state ||
		STATE_LOOKRIGHT_SKILL == m_state || STATE_LOOKLEFT_SKILL == m_state);
}

bool State::isStateWalk() const
{
	return (STATE_WALKDOWN == m_state || STATE_WALKUP == m_state ||
		STATE_WALKRIGHT == m_state || STATE_WALKLEFT == m_state ||
		STATE_WALKDOWN_SKILL == m_state || STATE_WALKUP_SKILL == m_state ||
		STATE_WALKRIGHT_SKILL == m_state || STATE_WALKLEFT_SKILL == m_state);
}

bool State::isStateDamage() const
{
	return (STATE_DAMAGEDOWN == m_state || STATE_DAMAGEUP == m_state ||
		STATE_DAMAGERIGHT == m_state || STATE_DAMAGELEFT == m_state);
}

bool State::isStateAttack() const
{
	return (STATE_ATTACKDOWN == m_state || STATE_ATTACKUP == m_state ||
		STATE_ATTACKRIGHT == m_state || STATE_ATTACKLEFT == m_state);
}

bool State::isStateSkill() const
{
	return (STATE_LOOKDOWN_SKILL == m_state || STATE_LOOKUP_SKILL == m_state ||
		STATE_LOOKRIGHT_SKILL == m_state || STATE_LOOKLEFT_SKILL == m_state ||
		STATE_WALKDOWN_SKILL == m_state || STATE_WALKUP_SKILL == m_state ||
		STATE_WALKRIGHT_SKILL == m_state || STATE_WALKLEFT_SKILL == m_state);
}