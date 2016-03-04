#pragma once

#include "cTexture.h"
#include "cFrameCounter.h"
#include "cState.h"

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);

class RObject {
public:
	RObject(LTextureSet* set) : m_texture_set(set), m_frame(0), m_horizontal_flip(false) {}

	void render(const SDL_Rect& camera);
	SDL_Rect getBox() const { return m_box; }
	
	virtual ~RObject() {}
protected:
	void turnOnHorizontalFlip() { m_horizontal_flip = true; }

	bool isCollision(const std::vector<RObject*>& obstacles, const SDL_Rect* p_box = NULL) const;
private:
	virtual SDL_Rect setRenderBox() const = 0;
	virtual int setNextFrame() = 0;
	virtual bool isObstacle() const = 0;
protected:
	SDL_Rect m_box;
private:
	LTextureSet* m_texture_set;
	SDL_Rect m_box_render;
	int m_frame;
	bool m_horizontal_flip;
};

class AObject : public RObject {
public:
	AObject(LTextureSet* set, int* fo, int* fm) :
		RObject(set), m_frame_offset(fo), m_frame_number(fm) {}
	virtual ~AObject() {}
protected:
	FrameCounter m_frame_counter;

	State m_state, m_state_prev;

	const int* m_frame_offset;
	const int* m_frame_number;
};