#include "cMenuPause.h"

MenuPause::MenuPause()
{
	m_header = NULL;
	try
	{
		int m_height = (SCREEN_HEIGHT - 30) / 2, m_delta_y = 50;

		SDL_Rect BoxContinue = { (SCREEN_WIDTH - 160) / 2, m_height, 160, 30 };
		SDL_Rect BoxQuit = { (SCREEN_WIDTH - 80) / 2, m_height + m_delta_y, 80, 30 };
		SDL_Rect BoxHeader = { (SCREEN_WIDTH - 200) / 2, 30, 200, 50 };

		m_buttons.push_back(new Button(BoxContinue, "Continue", 40));
		m_buttons.push_back(new Button(BoxQuit, "Quit", 40));
		m_header = new Header(BoxHeader, "Pause", 50);
	}
	catch (char* msg_err)
	{
		if (m_header) delete m_header;

		throw msg_err;
	}
}

int MenuPause::update()
{
	int position = -1;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
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

void MenuPause::render()
{
	m_header->render();
	Menu::render();
}