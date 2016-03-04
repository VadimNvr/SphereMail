//
//  cMenuMulti.h
//  Game_Server
//
//  Created by Вадим Навражных on 04.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#ifndef cMenuMulti_h
#define cMenuMulti_h

#pragma once

#include "cMenu.h"

class MenuMulti : public Menu {
    enum MenuState { CLIENT, SERVER };
public:
    MenuMulti();
    
    void render();
    int update();
    
    ~MenuMulti() { delete m_background; }
private:
    LTexture* m_background;
};

#endif /* cMenuMulti_h */
