#pragma once

#include "cMenu.h"

class MenuPause : public Menu {
	enum MenuState { CONTINUE, QUIT };
public:
	MenuPause();

	void render();
	int update();

	~MenuPause() { delete m_header; }
private:
	Header* m_header;
};