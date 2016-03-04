#include "cFrameCounter.h"

int FrameCounter::getNextFrame()
{
	if (++m_delay == m_delay_max)
	{
		m_delay = 0;
		m_frame = (m_frame + 1) % m_frame_max;
		m_finished = (m_frame == 0);
	}
	return m_frame;
}

void FrameCounter::resetCounter(int fm, int dm)
{
	m_frame = m_delay = 0;
	m_frame_max = fm;
	m_delay_max = (dm) ? dm : 8;
	m_finished = false;
}

void FrameCounter::resetCounter()
{
	m_frame = m_delay = 0;
	m_finished = false;
}