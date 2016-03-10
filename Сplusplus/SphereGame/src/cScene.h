#pragma once

#include "globals.h"

#include "cKnight.h"
#include "cWizard.h"
#include "cGoblin.h"
#include "cChobin.h"
#include "cTile.h"
#include "cCreature.h"

#include "cStatusBar.h"
#include "cTimer.h"
#include "cLevel.h"

#include <algorithm>
#include <fstream>


class Scene {
public:
	Scene(const std::string& f_settings, const std::string& f_map, bool isBoy, bool isGirl);
	
	void prepare();
	void update();
	void render();
	void handleEvent(const SDL_Event& e)
	{
		if (m_knight) m_knight->handleEvent(e);
		if (m_wizard) m_wizard->handleEvent(e);
	}

	bool isLevelFinished() const;
	bool isGameOver() const;

	~Scene();
private:
	void setCamera();

	void deleteTextures();
	void deleteTextureSets();
	void deleteTiles();
	void deleteBullets();
	void deleteCreatures();
public:
	Level m_level;

	SDL_Rect m_camera;
	SDL_Rect m_finish_area;

	Knight*		 m_knight;
	StatusBar*	 m_knight_bar;
	LTexture*	 m_knight_icon;
	LTextureSet* m_knight_set;

	Wizard*		 m_wizard;
	StatusBar*	 m_wizard_bar;
	LTexture*	 m_wizard_icon;
	LTextureSet* m_wizard_set;

	LTexture* m_bar;

	std::vector<Creature*> m_monsters;
	LTextureSet* m_goblin_set;
	LTextureSet* m_chobin_set;

	std::vector<Tile*> m_tile_map;
	LTextureSet* m_tile_set;

	std::vector<Bullet*> m_bullets;
	LTextureSet* m_fireball_set;
	LTextureSet* m_arrow_set;

	LTimer m_timer;

	const SDL_Keycode knight_keys[6] = {
		SDLK_s, SDLK_w, SDLK_d, SDLK_a, SDLK_i, SDLK_o
	};

	const SDL_Keycode wizard_keys[6] = {
		SDLK_DOWN, SDLK_UP, SDLK_RIGHT, SDLK_LEFT, SDLK_KP_MULTIPLY, SDLK_KP_MINUS
	};
};