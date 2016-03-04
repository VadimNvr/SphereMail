#include "cRObject.h"

void RObject::render(const SDL_Rect& camera)
{
	m_frame = setNextFrame();
	if (checkCollision(camera, m_box))
	{
		SDL_Rect dstrect;
		m_box_render = setRenderBox();
		dstrect.x = m_box_render.x - camera.x;
		dstrect.y = m_box_render.y - camera.y;
		dstrect.h = m_box_render.h;
		dstrect.w = m_box_render.w;
		m_texture_set->render(&dstrect, m_frame, m_horizontal_flip);
	}
	m_horizontal_flip = false;
}

bool RObject::isCollision(const std::vector<RObject*>& obstacles, const SDL_Rect* p_box) const
{
	SDL_Rect box = (p_box) ? *p_box : m_box;
	for (auto p_elem = obstacles.begin(); p_elem != obstacles.end(); p_elem++)
	{
		if (*p_elem != this && (*p_elem)->isObstacle() &&
			checkCollision((*p_elem)->getBox(), box))
		{
			return true;
		}
	}
	return false;
}

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
	int a_left = a.x, a_right = a.x + a.w;
	int a_top = a.y, a_bottom = a.y + a.h;
	int b_left = b.x, b_right = b.x + b.w;
	int b_top = b.y, b_bottom = b.y + b.h;

	// remember about direction of axes
	return !(a_right <= b_left || a_left >= b_right ||
		a_bottom <= b_top || a_top >= b_bottom);
}