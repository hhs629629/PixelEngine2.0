#include "stdafx.h"
#include "Entity.hpp"

Entity::Entity(Vec2f p_pos, SDL_Texture* p_texture, int p_shapeSize)
	:_pos(p_pos), _texture(p_texture)
{
	// read from x,y to w,h of a texture file
	_currentFrame.x = 0;
	_currentFrame.y = 0;
	_currentFrame.w = p_shapeSize;
	_currentFrame.h = p_shapeSize;
	
}

struct Vec2f& Entity::GetPos()
{
	return _pos;
}

SDL_Texture* Entity::GetTexture()
{
	return _texture;
}

SDL_Rect Entity::GetCurrentFrame()
{
	return _currentFrame;
}