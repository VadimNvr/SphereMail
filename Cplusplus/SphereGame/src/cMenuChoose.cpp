#include "cMenuChoose.h"

#define BUTTON_KNIGHT_ON	"../data/menu/knight_active.png"
#define BUTTON_KNIGHT_OFF	"../data/menu/knight_passive.png"

#define BUTTON_WIZARD_ON	"../data/menu/wizard_active.png"
#define BUTTON_WIZARD_OFF	"../data/menu/wizard_passive.png"

#define CARD_BACK			"../data/menu/carpet.png"

MenuChoose::MenuChoose()
{
	m_header = NULL;
	m_background = m_card_back = NULL;
	try
	{
		int m_width = 60, m_height = 155, m_delta_y = 10, m_delta_x = 30;

		SDL_Rect BoxKnight;
		BoxKnight.x = (SCREEN_WIDTH / 2 - m_width) / 2 + m_delta_x;
		BoxKnight.y = (SCREEN_HEIGHT - m_height) / 2 + m_delta_y;
		BoxKnight.w = m_width;
		BoxKnight.h = m_height;

		SDL_Rect BoxWizard;
		BoxWizard.x = (SCREEN_WIDTH * 3 / 2 - m_width) / 2 - m_delta_x;
		BoxWizard.y = (SCREEN_HEIGHT - m_height) / 2 + m_delta_y;
		BoxWizard.w = m_width;
		BoxWizard.h = m_height;

		m_width = 60; m_height = 20;

		SDL_Rect BoxBack;
		BoxBack.x = (SCREEN_WIDTH - m_width) / 2;
		BoxBack.y = SCREEN_HEIGHT - 40;
		BoxBack.w = m_width;
		BoxBack.h = m_height;

		m_width = 380; m_height = 30;

		SDL_Rect BoxHeader;
		BoxHeader.x = (SCREEN_WIDTH - m_width) / 2;
		BoxHeader.y = 20;
		BoxHeader.w = m_width;
		BoxHeader.h = m_height;

		m_buttons.push_back(new Button(BoxKnight, BUTTON_KNIGHT_ON, BUTTON_KNIGHT_OFF));
		m_buttons.push_back(new Button(BoxWizard, BUTTON_WIZARD_ON, BUTTON_WIZARD_OFF));
		m_buttons.push_back(new Button(BoxBack, "Back", 40));

		m_header = new Header(BoxHeader, "Choose the character", BoxHeader.h);
		m_background = new LTexture(BACKGROUND);
		m_card_back = new LTexture(CARD_BACK);
	}
	catch (char* msg_err)
	{
		if (m_header) delete m_header;
		if (m_background) delete m_background;
		if (m_card_back) delete m_card_back;

		throw msg_err;
	}
}

void MenuChoose::render()
{
	int m_width = 104, m_height = 128, m_delta_y = 10, m_delta_x = 30;

	SDL_Rect card1, card2, screen;
	card1.x = (SCREEN_WIDTH / 2 - m_width) / 2 + m_delta_x;
	card1.y = (SCREEN_HEIGHT - m_height) / 2 + m_delta_y;
	card1.w = m_width;
	card1.h = m_height;

	card2.x = (SCREEN_WIDTH * 3 / 2 - m_width) / 2 - m_delta_x;
	card2.y = (SCREEN_HEIGHT - m_height) / 2 + m_delta_y;
	card2.w = m_width;
	card2.h = m_height;

	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	m_background->render(&screen);
	SDL_SetRenderDrawColor(LRenderer::getRenderer(), 0x43, 0x14, 0xBA, 0x9A);
	SDL_RenderFillRect(LRenderer::getRenderer(), &screen);

	m_header->render();
	m_card_back->render(&card1, NULL, false);
	m_card_back->render(&card2, NULL, true);

	Menu::render();
}

int MenuChoose::update()
{
	int position = -1;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			SDL_Keycode key = e.key.keysym.sym;
			if (key == SDLK_RIGHT)
			{
				if (m_position == KNIGHT || m_position == WIZARD)
				{
					m_position = (m_position + 1) % (m_buttons.size() - 1);
				}
			}
			else if (key == SDLK_LEFT)
			{
				if (m_position == KNIGHT || m_position == WIZARD)
				{
					m_position = (m_position - 1 < 0) ? m_buttons.size() - 2 : m_position - 1;
				}
			}
			else if (key == SDLK_UP || key == SDLK_DOWN)
			{
				m_position = (m_position == BACK) ? 0 : 2;
			}
			else if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
			{
				position = m_position;
			}
		}
	}
	return position;
}

MenuChoose::~MenuChoose()
{
	delete m_card_back;
	delete m_background;
	delete m_header;
}