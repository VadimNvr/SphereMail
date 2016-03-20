//
//  cGame.h
//  Game_Server
//
//  Created by Вадим Навражных on 13.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#ifndef cGame_h
#define cGame_h

#include <stdio.h>
#include "cScene.h"

enum GameType {SINGLE, MULTI};

class Game
{
    GameType type;
    Scene *scene;
    
    int level;
    
    void Renderer();
    void Controller();
    
public:
    
    Game(GameType, int);
    void Start();
     
};

#endif /* cGame_h */
