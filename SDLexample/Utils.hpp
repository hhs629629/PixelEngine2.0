#pragma once
#include "stdafx.h"

namespace utils
{
	// returns elapsed time in seconds
	inline float hireTimeInSeconds()
	{
		float t = SDL_GetTicks(); 
		t *= 0.001f; // because SDL_GetTicks returns miliseceonds it needs to be divide by 1000 in order to get seconds
		return t;
	}
}

