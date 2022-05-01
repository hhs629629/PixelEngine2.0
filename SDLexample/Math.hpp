#pragma once
#include "stdafx.h"

typedef struct Vec2f
{
	float x, y;

	Vec2f()
	:x(0.f), y(0.f) 
	{}

	Vec2f(float p_x, float p_y)
	:x(p_x), y(p_y)
	{}

	void print() // prints vector
	{
		std::cout << x << "," << y << std::endl;
	}
}Vec2f;


