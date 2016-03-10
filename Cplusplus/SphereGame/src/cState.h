#pragma once

static const int number_states = 25;
enum StateEnum {
	STATE_LOOKDOWN,
	STATE_LOOKUP,
	STATE_LOOKRIGHT,
	STATE_LOOKLEFT,

	STATE_WALKDOWN,
	STATE_WALKUP,
	STATE_WALKRIGHT,
	STATE_WALKLEFT,

	STATE_DAMAGEDOWN,
	STATE_DAMAGEUP,
	STATE_DAMAGERIGHT,
	STATE_DAMAGELEFT,

	STATE_ATTACKDOWN,
	STATE_ATTACKUP,
	STATE_ATTACKRIGHT,
	STATE_ATTACKLEFT,

	STATE_LOOKDOWN_SKILL,
	STATE_LOOKUP_SKILL,
	STATE_LOOKRIGHT_SKILL,
	STATE_LOOKLEFT_SKILL,

	STATE_WALKDOWN_SKILL,
	STATE_WALKUP_SKILL,
	STATE_WALKRIGHT_SKILL,
	STATE_WALKLEFT_SKILL,

	STATE_DYING
};

class State {
public:
	State(StateEnum s = STATE_LOOKDOWN) : m_state(s) {}

	void set(StateEnum s) { m_state = s; }
	StateEnum get() const { return m_state; }

	State& operator = (StateEnum s)
	{
		m_state = s;
		return *this;
	}
	bool operator == (StateEnum s) const { return m_state == s; }
	bool operator != (StateEnum s) const { return m_state == s; }
	bool operator == (const State& s) const { return m_state == s.m_state; }
	bool operator != (const State& s) const { return m_state != s.m_state; }

	bool isStateLook() const;
	bool isStateWalk() const;
	bool isStateDamage() const;
	bool isStateAttack() const;
	bool isStateSkill() const;
	bool isStateDying() const { return STATE_DYING == m_state; }

	bool isUp() const;
	bool isDown() const;
	bool isLeft() const;
	bool isRight() const;
private:
	StateEnum m_state;
};