#include "stdafx.h"
#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_width, int p_height)
	:_window(nullptr), _renderer(nullptr)
{
	_window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_width, p_height, SDL_WINDOW_SHOWN);
	if (_window == nullptr)
		std::cout << "Window failed to init. Error:" << SDL_GetError() << std::endl;

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
		std::cout << "Renderer failed to init. Error:" << SDL_GetError() << std::endl;
}

void RenderWindow::CleanUp()
{
	SDL_DestroyWindow(_window);
}

void RenderWindow::Clear()
{
	SDL_RenderClear(_renderer);
}

void RenderWindow::Render(Entity& p_entity)
{
	SDL_Rect src; // the actual entity itself
	src.x = p_entity.GetCurrentFrame().x;
	src.y = p_entity.GetCurrentFrame().y;
	src.w = p_entity.GetCurrentFrame().w;
	src.h = p_entity.GetCurrentFrame().h;	
	
	SDL_Rect dst; // shape and position of an entity on the viewport
	dst.x = p_entity.GetPos().x;
	dst.y = p_entity.GetPos().y;
	dst.w = p_entity.GetCurrentFrame().w * 2; // 2 times bigger than original entity
	dst.h = p_entity.GetCurrentFrame().h * 2;

	SDL_RenderCopy(_renderer, p_entity.GetTexture(), &src, &dst);
}

void RenderWindow::Display()
{
	SDL_RenderPresent(_renderer);
}

SDL_Texture* RenderWindow::LoadTexture(const char* p_fileName) 
{
	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(_renderer, p_fileName);
	if (texture == nullptr)
		std::cout << "Failed to load texture. Error:" << SDL_GetError() << std::endl;
		
	return texture;
}
