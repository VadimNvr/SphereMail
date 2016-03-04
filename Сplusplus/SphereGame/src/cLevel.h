#pragma once

#include "cTile.h"

struct Level {
	bool outOfBoardersX(const SDL_Rect& box) const
	{
		return (box.x < 0 || box.x + box.w > m_width_px);
	}
	bool outOfBoardersY(const SDL_Rect& box) const
	{
		return (box.y < 0 || box.y + box.h > m_height_px);
	}

	int m_width, m_width_px;
	int m_height, m_height_px;
};