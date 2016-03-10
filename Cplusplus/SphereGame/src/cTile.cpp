#include "cTile.h"

Tile::Tile(LTextureSet* set, Vector2D r, int tile_type): RObject(set), m_type(tile_type)
{
	m_box.x = r.x;
	m_box.y = r.y;
	m_box.w = m_width;
	m_box.h = m_height;
}

bool Tile::isObstacle() const
{
	// magic tile numbers of textures
	return (m_type >= 3 && m_type <= 11);
}

