#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <cstdio>
#include <string>

// Screen dimension constants
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

// used for naming buttons control
static const int number_controls = 6;
enum Action {
	ACTION_WALKDOWN,
	ACTION_WALKUP,
	ACTION_WALKRIGHT,
	ACTION_WALKLEFT,
	ACTION_ATTACK,
	ACTION_SKILL
};