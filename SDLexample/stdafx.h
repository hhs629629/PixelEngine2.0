#pragma once

#pragma comment(lib,"SDL2/lib/x64/SDL2.lib")
#pragma comment(lib,"SDL2/lib/x64/SDL2main.lib")
#pragma comment(lib,"SDL2/lib/x64/SDL2_image.lib")
#pragma comment(lib,"SDL_gpu/lib/Release/SDL2_gpu.lib")

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_gpu/SDL_gpu.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using uint8		= unsigned __int8;
using uint32	= unsigned __int32;

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"

#define PIXEL(surface, x, y) *((Uint32*)(\
(Uint8*)surface->pixels + ((y) * surface->pitch) + ((x) * sizeof(Uint32)))\
)

#define QUOTE(s) #s