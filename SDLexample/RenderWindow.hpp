#pragma once
#include "stdafx.h"
#include "Entity.hpp"

class RenderWindow
{
public:
	RenderWindow(const char* p_title, int p_width, int p_height);
	void CleanUp();

	void Clear();
	void Render(Entity& p_entity);
	void Display();
	SDL_Texture* LoadTexture(const char* p_fileName);


private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};

