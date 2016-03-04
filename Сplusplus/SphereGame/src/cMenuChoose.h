#pragma once

#include "cMenu.h"

class MenuChoose : public Menu {
	enum MenuState { KNIGHT, WIZARD, BACK };
public:
	MenuChoose();

	void render();
	int update();

	~MenuChoose();
private:
	LTexture* m_background;
	LTexture* m_card_back;
	Header* m_header;
};