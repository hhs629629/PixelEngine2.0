#pragma once

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#pragma comment(lib,"SDL2_image.lib")

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_sdlrenderer.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include <string>
#include <iostream>

#include "Game.hpp"
#include "Simulation.hpp"
#include "Graphics.hpp"
#include "Texture.hpp"

// main globals
const Uint32 SCREEN_WIDTH = 1200;
const Uint32 SCREEN_HEIGHT = 800;
const SDL_Rect SIMULATION_RECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

const Uint32 SCREEN_FPS = 60;
const Uint32 TICKS_PER_FRAME = 1000/SCREEN_FPS;
const Uint32 PERFORMANCE_POLL_RATE = 15;

const std::string FONT_FILE_PATH = "../res/NanumGothic.ttf";

const Uint16 MIN_DRAW_RADIUS = 3; // brush default radius
const Uint16 MAX_DRAW_RADIUS = 75;

// Simulation globals
const std::string MATERIAL_FILE_PATH = "../res/Materials.json";

// Texture globals
const SDL_PixelFormatEnum PIXEL_FORMAT = SDL_PIXELFORMAT_ARGB8888;
const SDL_Color TEXT_COLOR = { 255, 255, 255, 255 };