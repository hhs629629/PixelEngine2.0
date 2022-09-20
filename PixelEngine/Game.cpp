#include "stdafx.h"
#include "Game.hpp"

Game::Game()
{
	gFailFlag = 0;
	gTexture = nullptr;
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) gFailFlag++;

	gWindow = SDL_CreateWindow("Pixel Engine ver 0.0.0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!gWindow) gFailFlag++;

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!gRenderer) gFailFlag++;

	TTF_Init();
	gFont = TTF_OpenFont(FONT_FILE_PATH.c_str(), 16);
	if (!gFont) gFailFlag++;
}

Game::~Game()
{
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	TTF_CloseFont(gFont);
	gFont = nullptr;

	delete gTexture;

	TTF_Quit();
	SDL_Quit();

	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Game::ErrorHandler()
{
	if (gFailFlag != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(),this->gWindow);
		exit(-1);
	}
}
