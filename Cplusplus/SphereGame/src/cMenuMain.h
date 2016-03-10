#pragma once

#include "cMenu.h"

class MenuMain : public Menu {
	enum MenuState { SINGLE_PLAYER, MULTIPLAYER, QUIT };
public:
	MenuMain();

	void render();
	int update();

	~MenuMain() { delete m_background; }
private:
	LTexture* m_background;
};