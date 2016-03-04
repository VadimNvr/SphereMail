#include <iostream>

#include "globals.h"
#include "cViewManager.h"

SDL_Window* g_window = NULL;
LRenderer* g_renderer = NULL;

void init()
{
	char msg_err[256];
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		sprintf(msg_err, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		throw msg_err;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	g_window = SDL_CreateWindow("Quest Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		sprintf(msg_err, "Window could not be created! SDL Error: %s\n", SDL_GetError());
		throw msg_err;
	}

	try
	{
		g_renderer = LRenderer::Instance(g_window);
	}
	catch (char* msg_err)
	{
		SDL_DestroyWindow(g_window);
		throw msg_err;
	}

	try
	{
		int img_flag = IMG_INIT_PNG | IMG_INIT_JPG;
		if ((IMG_Init(img_flag) & img_flag) != img_flag)
		{
			char err_msg[256];
			sprintf(err_msg, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			throw err_msg;
		}
	}
	catch (char* msg_err)
	{
		delete g_renderer;
		SDL_DestroyWindow(g_window);

		IMG_Quit();
		SDL_Quit();
		throw msg_err;
	}

	try
	{
		if (TTF_Init() == -1)
		{
			char err_msg[256];
			sprintf(err_msg, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			throw err_msg;
		}
	}
	catch (char* msg_err)
	{
		delete g_renderer;
		SDL_DestroyWindow(g_window);

		IMG_Quit();
		SDL_Quit();
		throw msg_err;
	}
}

void shutdown()
{
	delete g_renderer;
	SDL_DestroyWindow(g_window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	try
	{
		init();
	}
	catch (char* msg_err)
	{
		std::cout << msg_err;
		return 1;
	}

	ViewManager* menu = NULL;
	try
	{
		menu = ViewManager::Instance();
		menu->Loop();
	}
	catch (char* msg_err)
	{
		std::cout << msg_err;
	}

	if (menu) delete menu;
	shutdown();

	return 0;
}