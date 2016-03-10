#pragma once

#include "cScene.h"

#include "cMenuMain.h"
#include "cMenuChoose.h"
#include "cMenuPause.h"
#include "cMenuMulti.h"

#include "cServer.h"
#include "cClient.h"

#include <functional>

enum Mode { SINGLE, CLIENT, SERVER };

class ViewManager {
private:
	ViewManager();
public:
	static ViewManager* Instance();
    static Mode mode;
	void Loop();

	~ViewManager();
private:
	void exit();

	void MenuMainFunc();
	void MenuChooseFunc();
	void MenuPauseFunc();
    void MenuMultiFunc();

	void GameStart();
	void EnteringLevel();
	void ExitingLevel();
	void GameOver();
private:
	static ViewManager* m_pinstance;

	int m_level = 0;
	const int m_level_max = 1;
	bool m_game_won;

	Scene* m_scene;
	bool m_chosen_wizard, m_chosen_knight;

	std::stack<std::function<void()>> m_stack;

	MenuMain* m_menuMain;
	MenuChoose* m_menuChoose;
	MenuPause* m_menuPause;
    MenuMulti* m_menuMulti;
};