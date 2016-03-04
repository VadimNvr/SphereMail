#include "cViewManager.h"
#include <iostream>

#define LVL_PATH		"../data/levels/level"
#define EXT_SET			".settings"
#define EXT_MAP			".map"

ViewManager* ViewManager::m_pinstance = NULL;

ViewManager::ViewManager() : m_scene(NULL), m_game_won(false)
{
	m_menuMain = NULL;
	m_menuChoose = NULL;
	m_menuPause = NULL;
    m_menuMulti = NULL;

	m_scene = NULL;

	try
	{
		m_menuMain = new MenuMain;
		m_menuChoose = new MenuChoose;
		m_menuPause = new MenuPause;
        m_menuMulti = new MenuMulti;
	}
	catch (char* msg_err)
	{
		if (m_menuMain) delete m_menuMain;
		if (m_menuChoose) delete m_menuChoose;
		if (m_menuPause) delete m_menuPause;
        if (m_menuMulti) delete m_menuMulti;

		throw msg_err;
	}

	m_stack.push([this]() { MenuMainFunc(); });
}

ViewManager* ViewManager::Instance()
{
	if (!m_pinstance)
	{
		m_pinstance = new ViewManager;
	}
	return m_pinstance;
}

void ViewManager::Loop()
{
	while (m_stack.size())
	{
		(m_stack.top())();
	}
}

ViewManager::~ViewManager()
{
	exit();

	if (m_scene) delete m_scene;

	delete m_menuPause;
	delete m_menuChoose;
	delete m_menuMain;

	m_pinstance = NULL;
}

void ViewManager::exit()
{
	while (!m_stack.empty())
	{
		m_stack.pop();
	}
}

void ViewManager::MenuMainFunc()
{
	int position = m_menuMain->update();

	m_menuMain->render();
	SDL_RenderPresent(LRenderer::getRenderer());

	switch (position)
	{
	case 0:
		m_menuChoose->setUp();
		m_stack.push([this]() { MenuChooseFunc(); });
		break;
	case 1:
            /*
		m_chosen_knight = true;
		m_chosen_wizard = true;
		m_level = 0;
		m_scene = new Scene(
			LVL_PATH + std::to_string(m_level + 1) + EXT_SET,
			LVL_PATH + std::to_string(m_level + 1) + EXT_MAP,
			m_chosen_knight, m_chosen_wizard
		);
		m_stack.push([this]() { GameStart(); });
		m_stack.push([this]() { EnteringLevel(); });
		m_stack.push([this]() { ExitingLevel(); });
             */
        m_menuMulti->setUp();
        m_stack.push([this]() { MenuMultiFunc(); });
		break;
	case 2:
		exit();
		break;
	default:
		break;
	}
}

void ViewManager::MenuMultiFunc()
{
    int position = m_menuMulti->update();
    
    m_menuMulti->render();
    SDL_RenderPresent(LRenderer::getRenderer());
    
    switch (position) {
        case 0:
            m_stack.pop();
            break;
            
        case 1:
            m_stack.pop();
            break;
            
        case 2:
            m_stack.pop();
            
        default:
            break;
    }
}


void ViewManager::MenuChooseFunc()
{
	int position = m_menuChoose->update();

	m_menuChoose->render();
	SDL_RenderPresent(LRenderer::getRenderer());


	switch (position)
	{
	case 0:			// Knight character is chosen
		m_chosen_knight = true;
		m_chosen_wizard = false;
		m_level = 0;
		m_scene = new Scene(
			LVL_PATH + std::to_string(m_level + 1) + EXT_SET,
			LVL_PATH + std::to_string(m_level + 1) + EXT_MAP,
			m_chosen_knight, m_chosen_wizard
		);

		m_stack.pop();
		m_stack.push([this]() { GameStart(); });
		m_stack.push([this]() { EnteringLevel(); });
		m_stack.push([this]() { ExitingLevel(); });
		break;
	case 1:			// Wizard character is chosen
		m_chosen_knight = false;
		m_chosen_wizard = true;
		m_level = 0;
		m_scene = new Scene(
			LVL_PATH + std::to_string(m_level + 1) + EXT_SET,
			LVL_PATH + std::to_string(m_level + 1) + EXT_MAP,
			m_chosen_knight, m_chosen_wizard
		);
		
		m_stack.pop();
		m_stack.push([this]() { GameStart(); });
		m_stack.push([this]() { EnteringLevel(); });
		m_stack.push([this]() { ExitingLevel(); });
		break;
	case 2:
		m_stack.pop();
	default:
		break;
	}
}

void ViewManager::MenuPauseFunc()
{
	int position = m_menuPause->update();

	m_scene->render();
	
	SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x00, 0x00, 0x00, 224);
	SDL_RenderFillRect(LRenderer::getRenderer(), &screen);

	m_menuPause->render();
	
	SDL_RenderPresent(LRenderer::getRenderer());

	switch (position)
	{
	case 0:
		m_stack.pop();
		break;
	case 1:
		m_stack.pop();
		m_stack.pop();

		delete m_scene;
		m_scene = NULL;
		
		m_menuMain->setUp();
		m_stack.push([this]() { ExitingLevel(); });
		break;
	default:
		break;
	}
}

void ViewManager::GameStart()
{
	SDL_Event e;
	bool quit = false;

	m_scene->prepare();

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		else
		{
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				SDL_Keycode key = e.key.keysym.sym;
				if (key == SDLK_ESCAPE)
				{
					m_menuPause->setUp();
					m_stack.push([this]() { MenuPauseFunc(); });
				}
			}

			m_scene->handleEvent(e);
		}
	}

	m_scene->update();

	SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(LRenderer::getRenderer());

	m_scene->render();

	SDL_RenderPresent(LRenderer::getRenderer());

	if (m_scene->isGameOver())
	{
		m_game_won = false;
		m_stack.push([this]() { GameOver(); });
	}

	if (m_scene->isLevelFinished())
	{
		delete m_scene;
		m_scene = NULL;

		if (++m_level >= m_level_max)
		{
			m_game_won = true;
			m_stack.push([this]() { GameOver(); });
			m_level = 0;
		}
		else
		{
			m_scene = new Scene(
				LVL_PATH + std::to_string(m_level + 1) + EXT_SET,
				LVL_PATH + std::to_string(m_level + 1) + EXT_MAP,
				m_chosen_knight, m_chosen_wizard
			);

			m_stack.push([this]() { EnteringLevel(); });
			m_stack.push([this]() { ExitingLevel(); });
		}
	}

	if (quit)
	{
		exit();
	}
}

void ViewManager::EnteringLevel()
{
	SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect BoxLevel = { (SCREEN_WIDTH - 160) / 2, (SCREEN_HEIGHT - 40) / 2, 160, 40 };
	LTexture* name_level = new LTexture(
		"level "+std::to_string(m_level + 1),
		TRUE_TYPE_FONT, { 0xFF, 0xFF, 0xFF }, 40
	);

	for (int i = 0; i <= 255; i += 10)
	{
		name_level->setAlpha(i);
		name_level->render(&BoxLevel);

		SDL_RenderPresent(LRenderer::getRenderer());
		SDL_Delay(100);
	}

	for (int i = 255; i >= 0; i -= 10)
	{
		m_scene->render();

		SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x00, 0x00, 0x00, i);
		SDL_RenderFillRect(LRenderer::getRenderer(), &screen);
		
		name_level->setAlpha(i);
		name_level->render(&BoxLevel);

		SDL_RenderPresent(LRenderer::getRenderer());
		SDL_Delay(100);
	}

	delete name_level;
	m_stack.pop();

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0);
}

void ViewManager::ExitingLevel()
{
	SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int i = 0; i <= 255; i += 10)
	{
		SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x00, 0x00, 0x00, i);
		SDL_RenderFillRect(LRenderer::getRenderer(), &screen);
		SDL_RenderPresent(LRenderer::getRenderer());
		SDL_Delay(100);
	}

	m_stack.pop();

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0);
}

void ViewManager::GameOver()
{
	SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int i = 0; i < 10; i++)
	{
		SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x00, 0x00, 0x00, 10);
		SDL_RenderFillRect(LRenderer::getRenderer(), &screen);
		SDL_RenderPresent(LRenderer::getRenderer());
		SDL_Delay(100);
	}

	SDL_Rect BoxMsg;
	LTexture* message;

	if (m_game_won)
	{
		BoxMsg = { (SCREEN_WIDTH - 190) / 2, (SCREEN_HEIGHT - 40) / 2, 190, 40 };
		message = new LTexture("You won!", TRUE_TYPE_FONT, { 0xFF, 0xFF, 0xFF }, 40);
	}
	else
	{
		BoxMsg = { (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 40) / 2, 200, 40 };
		message = new LTexture("You lost!", TRUE_TYPE_FONT, { 0xFF, 0xFF, 0xFF }, 40);
	}

	for (int i = 0; i <= 255; i += 10)
	{
		message->setAlpha(i);
		message->render(&BoxMsg);

		SDL_RenderPresent(LRenderer::getRenderer());
		SDL_Delay(100);
	}

	message->setAlpha(0xFF);
	
	for (int i = 0; i <= 255; i += 10)
	{
		SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x00, 0x00, 0x00, i);
		SDL_RenderFillRect(LRenderer::getRenderer(), &screen);

		message->render(&BoxMsg);

		SDL_RenderPresent(LRenderer::getRenderer());
		SDL_Delay(100);
	}

	delete message;

	SDL_Delay(2000);
	exit();

	m_menuMain->setUp();
	m_stack.push([this]() { MenuMainFunc(); });
}