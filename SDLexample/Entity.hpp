#pragma once
#include "stdafx.h"
#include "Math.hpp"

// everything that can be rendered on a screen is a Entity
class Entity
{
public:
	Entity(Vec2f p_pos, SDL_Texture* p_texture, int p_shapeSize);

	// getter function
	Vec2f& GetPos();
	SDL_Texture* GetTexture();
	SDL_Rect GetCurrentFrame();

private:
	Vec2f _pos;			// two float positions on viewport

	SDL_Rect _currentFrame;
	SDL_Texture* _texture; // memory address of where a texture is living in VRAM

};
