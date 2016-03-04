#include "cStatusBar.h"

StatusBar::StatusBar(LTexture* bar, LTexture* icon, Vector2D pos) :
	m_texture_bar(bar), m_texture_icon(icon)
{
	m_box_icon.x = pos.x;
	m_box_icon.y = pos.y;
	m_box_icon.w = 16;
	m_box_icon.h = 16;

	m_box_bar.x = m_box_icon.x + m_box_icon.w + 3;
	m_box_bar.y = m_box_icon.y;
	m_box_bar.w = 95;
	m_box_bar.h = 16;

	m_health.x = m_box_bar.x + 31;
	m_health.y = m_box_bar.y + 2;
	m_health.w = m_bar_width_max;
	m_health.h = m_bar_height;

	m_energy.x = m_box_bar.x + 31;
	m_energy.y = m_box_bar.y + 10;
	m_energy.w = m_bar_width_max;
	m_energy.h = m_bar_height;
}

void StatusBar::render() const
{
	SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(LRenderer::getRenderer(), &m_health);

	SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(LRenderer::getRenderer(), &m_energy);

	m_texture_bar->render(&m_box_bar);
	m_texture_icon->render(&m_box_icon);
}