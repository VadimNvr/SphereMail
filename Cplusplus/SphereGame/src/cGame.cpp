//
//  cGame.cpp
//  Game_Server
//
//  Created by Вадим Навражных on 13.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#include "cGame.h"

#define LVL_PATH		"../data/levels/level"
#define EXT_SET			".settings"
#define EXT_MAP			".map"

#define PATH_TO_SERVER "../Server/server"
#define SERVER_PORT "8002"

Game::Game(GameType type, int charater)
{
    this->type = type;
    level = 0;
    
    switch (type) {
        case SINGLE:
            scene = new Scene(
                            LVL_PATH + std::to_string(level + 1) + EXT_SET,
                            LVL_PATH + std::to_string(level + 1) + EXT_MAP,
                            1, 0
            );
            
            
            m_stack.push([this]() { GameStart(); });
            m_stack.push([this]() { EnteringLevel(); });
            m_stack.push([this]() { ExitingLevel(); });
            break;
            
        case MULTI:
            break;
    }
}

void Game::Start()
{
    SDL_Event e;
    bool quit = false;
    Client::m_pscene = m_scene;
    
    m_scene->prepare();
    
    if (mode != SERVER)
    {
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
                
                if (mode == CLIENT) {
                    SyncData data;
                    
                    data.type = SyncData::CtS;
                    data.id = Client::id;
                    data.player = m_scene->m_knight ? m_scene->m_knight->getPos() : m_scene->m_wizard->getPos();
                    Client::sendToServer(&data, sizeof(data));
                }
            }
        }
    }
    
    m_scene->update();
    
    if (ViewManager::mode == SERVER)
    {
        SyncData data;
        data.type = SyncData::StC;
        data.id = 0;
        data.bots[0] = m_scene->m_monsters[0]->getPos();
        data.bots[1] = m_scene->m_monsters[1]->getPos();
        
        for (int client: Server::clients) {
            send(client, &data, sizeof(data), 0);
        }
    }
    
    
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