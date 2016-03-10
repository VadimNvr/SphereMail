#pragma once

#include "cRObject.h"
#include "cVector.h"
#include "globals.h"

class StatusBar {
public:
	StatusBar(LTexture* bar, LTexture* icon, Vector2D pos);
	void render() const;
	void setEnergyBarLength(float ratio) { m_energy.w = ratio * m_bar_width_max; }
	void setHealthBarLength(float ratio) { m_health.w = ratio * m_bar_width_max; }
private:
	SDL_Rect m_box_icon, m_box_bar;
	SDL_Rect m_energy, m_health;

	LTexture* m_texture_bar;
	LTexture* m_texture_icon;

	const int m_bar_width_max = 56;
	const int m_bar_height = 4;
};