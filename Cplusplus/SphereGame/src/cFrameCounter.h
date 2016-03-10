#pragma once

struct FrameCounter
{
	FrameCounter() :
		m_frame(0), m_frame_max(1),
		m_delay(0), m_delay_max(8),
		m_finished(false) {};

	void resetCounter(int fm, int dm = 0);
	void resetCounter();
	
	int getNextFrame();
	int getSettedFrame() const { return (m_delay) ? -1 : m_frame; }
	
	bool isFinished() const { return m_finished; }
private:
	int m_frame, m_frame_max;
	int m_delay, m_delay_max;
	bool m_finished;
};