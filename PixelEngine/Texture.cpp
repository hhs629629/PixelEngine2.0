#include "stdafx.h"
#include "Texture.hpp"
#include <algorithm>

Texture::Texture(SDL_Renderer *_renderer, bool &_success, SDL_Rect _rect)
{
	renderer = _renderer;
	font = nullptr;
	rect = _rect;
	texture = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
	_success = _success && texture;
	button = nullptr;
	buttonCount = 0;
}

Texture::Texture(SDL_Renderer *_renderer, Sint32 _x, Sint32 _y, TTF_Font *_font)
{
	renderer = _renderer;
	font = _font;
	rect.x = _x;
	rect.y = _y;
	texture = nullptr;
	button = nullptr;
	buttonCount = 0;
}

Texture::Texture()
{
}

Texture::~Texture()
{
	if(texture) { SDL_DestroyTexture(texture); }
	if(button) { delete[] button; }
}

void Texture::renderTexture()
{
	if(texture) { SDL_RenderCopy(renderer, texture, nullptr, &rect); }
}

void Texture::changeTexture(Uint32 *_buffer, Uint32 _size)
{
	SDL_UpdateTexture(texture, nullptr, _buffer, _size * sizeof(Uint32));
}
