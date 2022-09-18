#pragma once
#include "Texture.hpp"

enum class TextureID : Uint8 {
	SIMULATION_TEXTURE = 0,
	INFO_UI_TEXTURE,
	TOOLS_UI_TEXTURE,
	MATERIALS_UI_TEXTURE,
	TOTAL_TEXTURES
};

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
	Texture* gTexture[static_cast<int>(TextureID::TOTAL_TEXTURES)];

private:

};

