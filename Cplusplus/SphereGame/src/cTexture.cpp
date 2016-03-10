#include "cTexture.h"
#include <iostream>
#include <fstream>

SDL_Renderer* LTexture::m_renderer = NULL;

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

LTexture::LTexture(const std::string& path)
{
	m_texture = NULL;

	if (m_renderer == NULL)
	{
		char err_msg[256];
		sprintf(err_msg, "Renderer has not been created, yet!\n");
		throw err_msg;
	}
	
	SDL_Surface* loadedSurface = NULL; 
	try
	{
		loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL)
		{
			char err_msg[256];
			sprintf(err_msg, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
			throw err_msg;
		}
	}
	catch (char* err_msg)
	{
		throw err_msg;
	}
	
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	SDL_Texture* newTexture = NULL;
	try
	{
		newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		if (newTexture == NULL)
		{
			char err_msg[256];
			sprintf(err_msg, "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			throw err_msg;
		}
	}
	catch (char* err_msg)
	{
		SDL_FreeSurface(loadedSurface);
		throw err_msg;
	}

	m_texture = newTexture;
	m_width = loadedSurface->w;
	m_height = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
}

LTexture::LTexture(const std::string& text, const std::string& font, SDL_Color color, int size)
{
	m_texture = NULL;
    
	if (m_renderer == NULL)
	{
		char err_msg[256];
		sprintf(err_msg, "Renderer has not been created, yet!\n");
		throw err_msg;
	}

	TTF_Font* m_font = NULL;
	try {
		m_font = TTF_OpenFont(font.c_str(), size);
		if (m_font == NULL)
		{
            std::cout << is_file_exist(font.c_str()) << std::endl;
			char err_msg[256];
			sprintf(err_msg, "Unable to open the font %s! SDL_ttf Error: %s\n", font.c_str(), TTF_GetError());
			throw err_msg;
		}
	}
	catch (char* err_msg)
	{
		throw err_msg;
	}

	SDL_Surface* loadedSurface = NULL;
	try
	{
		loadedSurface = TTF_RenderText_Solid(m_font, text.c_str(), color);
		if (loadedSurface == NULL)
		{
			char err_msg[256];
			sprintf(err_msg, "Unable to create text image! SDL_ttf Error: %s\n", TTF_GetError());
			throw err_msg;
		}
	}
	catch (char* err_msg)
	{
		TTF_CloseFont(m_font);
		throw err_msg;
	}

	SDL_Texture* newTexture = NULL;
	try
	{
		newTexture= SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		if (newTexture == NULL)
		{
			char err_msg[256];
			sprintf(err_msg, "Unable to create text image! SDL Error: %s\n", SDL_GetError());
			throw err_msg;
		}
	}
	catch (char* err_msg)
	{
		SDL_FreeSurface(loadedSurface);
		TTF_CloseFont(m_font);
		throw err_msg;
	}

	m_texture = newTexture;
	m_width = loadedSurface->w;
	m_height = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
	TTF_CloseFont(m_font);
}

void LTexture::render(const SDL_Rect* dstrect, const SDL_Rect* srcrect, bool h_flip) const
{
	SDL_RenderCopyEx(m_renderer, m_texture, srcrect, dstrect, 0, NULL,
		(h_flip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_texture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

LTextureSet::LTextureSet(const std::string& path, int width_clip, int height_clip) :
	LTexture(path), m_width_clip(width_clip), m_height_clip(height_clip)
{
	int n_width = m_width / width_clip, n_height = m_height / height_clip;

	for (int j = 0; j < n_height; j++)
	{
		for (int i = 0; i < n_width; i++)
		{
			SDL_Rect new_clip;
			new_clip.x = i * width_clip;
			new_clip.y = j * height_clip;
			new_clip.w = width_clip;
			new_clip.h = height_clip;
			m_texture_clips.push_back(new_clip);
		}
	}
}

void LTextureSet::render(const SDL_Rect* dstrect, int number_clip, bool h_flip) const
{
	LTexture::render(dstrect, &m_texture_clips[number_clip], h_flip);
}

LRenderer* LRenderer::m_pinstance = NULL;

LRenderer* LRenderer::Instance(SDL_Window* window)
{
	if (!m_pinstance)
	{
		m_pinstance = new LRenderer(window);
	}
	return m_pinstance;
}

LRenderer::LRenderer(SDL_Window* window)
{
	if (LTexture::m_renderer != NULL)
	{
		char err_msg[256];
		sprintf(err_msg, "Renderer has already been created!\n");
		throw err_msg;
	}

	LTexture::m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (LTexture::m_renderer == NULL)
	{
		char err_msg[256];
		sprintf(err_msg, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		throw err_msg;
	}

	SDL_SetRenderDrawColor(LTexture::m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawBlendMode(LTexture::m_renderer, SDL_BLENDMODE_BLEND);
}

LRenderer::~LRenderer()
{
	SDL_DestroyRenderer(LTexture::m_renderer);
	LTexture::m_renderer = NULL;
}