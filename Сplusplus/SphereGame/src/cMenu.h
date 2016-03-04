#pragma once

#include <stack>
#include "cTexture.h"
#include "globals.h"

#define BACKGROUND			"../data/menu/tumblr.jpg"
#define TRUE_TYPE_FONT		"../data/goodbyeDespair.ttf"

class Button {
public:
	Button(SDL_Rect box, const std::string& text, int size);
	Button(SDL_Rect box, const std::string& tex1, const std::string& tex2);
	
	void render() const;

	void setSelected() { m_selected = true; }
	void setUnSelected() { m_selected = false; }

	~Button();
private:
	bool m_selected;
	SDL_Rect m_box;

	LTexture* m_texture_active;
	LTexture* m_texture_passive;
};

class Header {
public:
	Header(SDL_Rect box, const std::string& text, int size);
	Header(SDL_Rect box, const std::string& texture);

	void render() const { m_texture->render(&m_box); }

	~Header() { delete m_texture; }
private:
	SDL_Rect m_box;
	LTexture* m_texture;
};

class Menu {
public:
	Menu() : m_position(0) {}
	void setUp() { m_position = 0; }
	virtual int update() = 0;
	virtual ~Menu();
protected:
	void render();
protected:
	std::vector<Button*> m_buttons;
	int m_position;
};