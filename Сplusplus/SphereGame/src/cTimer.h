#pragma once

#include <SDL2/SDL.h>

class LTimer {
public:
	LTimer() : m_time(SDL_GetTicks()) {}
	Uint32 getTime() const { return SDL_GetTicks() - m_time; }
	void resetTimer() { m_time = SDL_GetTicks(); }
private:
	int m_time;
};