#pragma once

#include "globals.h"
#include "cRObject.h"
#include "cVector.h"

class Tile: public RObject {
public:
	Tile(LTextureSet* set, Vector2D r, int tile_type);
	bool isObstacle() const;
	int getType() const { return m_type; }
private:
	SDL_Rect setRenderBox() const { return m_box; }
	int setNextFrame() { return m_type; }
public:
	static const int m_width = 64;
	static const int m_height = 64;
private:
	int m_type;
};