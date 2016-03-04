#include "cMenuMain.h"
#include <iostream>

MenuMain::MenuMain()
{
	m_background = NULL;
	try
	{
		int m_pos_y = (SCREEN_HEIGHT - 30) / 2, m_delta_y = 50;
		SDL_Rect BoxSingle = { (SCREEN_WIDTH - 240) / 2, m_pos_y - m_delta_y, 240, 30 };
		SDL_Rect BoxMulti = { (SCREEN_WIDTH - 220) / 2, m_pos_y, 220, 30 };
		SDL_Rect BoxQuit = { (SCREEN_WIDTH - 80) / 2, m_pos_y + m_delta_y, 80, 30 };

		m_buttons.push_back(new Button(BoxSingle, "Single player", 40));
		m_buttons.push_back(new Button(BoxMulti, "Multiplayer", 40));
		m_buttons.push_back(new Button(BoxQuit, "Quit", 40));
        
		m_background = new LTexture(BACKGROUND);
	}
	catch (char* msg_err)
	{
		if (m_background) delete m_background;

		throw msg_err;
	}
}

void MenuMain::render()
{
	SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_background->render(&screen);

	SDL_SetRenderDrawColor(LRenderer::getRenderer(), 67, 20, 186, 154);
	SDL_RenderFillRect(LRenderer::getRenderer(), &screen);
	
	Menu::render();
}

int MenuMain::update()
{
	int position = -1;
	SDL_Event e;
	if (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			SDL_Keycode key = e.key.keysym.sym;
			if (key == SDLK_DOWN)
			{
				m_position = (m_position + 1) % m_buttons.size();
			}
			else if (key == SDLK_UP)
			{
				m_position = (m_position - 1 < 0) ? m_buttons.size() - 1 : m_position - 1;
			}
			else if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
			{
				position = m_position;
			}
		}
	}
	return position;
}