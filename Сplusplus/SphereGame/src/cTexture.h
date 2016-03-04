#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <vector>
#include <string>

class LTexture {
	friend class LRenderer;
public:
	LTexture(const std::string& path);
	LTexture(const std::string& text, const std::string& font, SDL_Color color, int size);
	
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void render(const SDL_Rect* dstrect, const SDL_Rect* srcrect = NULL, bool h_flip = false) const;

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	virtual ~LTexture() { SDL_DestroyTexture(m_texture); }
protected:
	SDL_Texture* m_texture;
	int m_width, m_height;
private:
	static SDL_Renderer* m_renderer;
};

class LTextureSet: public LTexture {
public:
	LTextureSet(const std::string& path, int width_clip, int height_clip);

	void render(const SDL_Rect* dstrect, int number_clip, bool h_flip = false) const;

	int getWidthClip() const { return m_width_clip; }
	int getHeightClip() const { return m_height_clip; }
private:
	int m_width_clip, m_height_clip;
	std::vector<SDL_Rect> m_texture_clips;
};

class LRenderer {
public:
	static LRenderer* Instance(SDL_Window* window);
	static SDL_Renderer* getRenderer() { return LTexture::m_renderer; }

	~LRenderer();
private:
	LRenderer(SDL_Window* window);
private:
	static LRenderer* m_pinstance;
};
