#include "cMenu.h"
#include <iostream>

Button::Button(SDL_Rect box, const std::string& text, int size) :
	m_box(box), m_selected(false)
{
	m_texture_active = m_texture_passive = NULL;
	try
	{
		m_texture_active = new LTexture(text, TRUE_TYPE_FONT, { 0xFF, 0x7D, 0x00 }, size);
		m_texture_passive = new LTexture(text, TRUE_TYPE_FONT, { 0xFF, 0xFF, 0xFF }, size);
	}
	catch (char* msg_err)
	{
		if (m_texture_passive) delete m_texture_passive;
		if (m_texture_active) delete m_texture_active;
		throw msg_err;
	}
}

Button::Button(SDL_Rect box, const std::string& tex1, const std::string& tex2) :
	m_box(box)
{
	m_texture_active = m_texture_passive = NULL;
	try
	{
		m_texture_active = new LTexture(tex1);
		m_texture_passive = new LTexture(tex2);
	}
	catch (char* msg_err)
	{
		if (m_texture_passive) delete m_texture_passive;
		if (m_texture_active) delete m_texture_active;
		throw msg_err;
	}
}

void Button::render() const
{
	(m_selected) ? m_texture_active->render(&m_box) : m_texture_passive->render(&m_box);
}

Button::~Button()
{
	delete m_texture_active;
	delete m_texture_passive;
}

Header::Header(SDL_Rect box, const std::string& text, int size) : m_box(box)
{
	m_texture = new LTexture(text, TRUE_TYPE_FONT, { 0xFF, 0xFF, 0xFF }, size);
}

Header::Header(SDL_Rect box, const std::string& texture) : m_box(box)
{
	m_texture = new LTexture(texture);
}

void Menu::render()
{
	for (auto p_elem = m_buttons.begin(); p_elem != m_buttons.end(); p_elem++)
	{
		(*p_elem)->setUnSelected();
	}
	m_buttons[m_position]->setSelected();

	for (auto p_elem = m_buttons.begin(); p_elem != m_buttons.end(); p_elem++)
	{
		(*p_elem)->render();
	}
}

Menu::~Menu()
{
	for (auto p_elem = m_buttons.begin(); p_elem != m_buttons.end(); p_elem++)
	{
		delete *p_elem;
	}
	m_buttons.clear();
}