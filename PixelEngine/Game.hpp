#pragma once
#include "Texture.hpp"

class Game
{
public:
	Game();
	~Game();
	void ErrorHandler();

public:
	Uint8 gFailFlag;
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	TTF_Font* gFont;
	Texture* gTexture;

private:

};

