#include "stdafx.h"
#include <algorithm>

int main(int argc, char **argv)
{
	Game* game = new Game();
	game->ErrorHandler();

	Simulation sim(SIMULATION_WIDTH, SIMULATION_HEIGHT, PIXEL_FORMAT);

	bool success = true;

	game->gTexture[static_cast<int>(TextureID::SIMULATION_TEXTURE)] = new Texture(game->gRenderer, success, SIMULATION_RECT);
	game->gTexture[static_cast<int>(TextureID::INFO_UI_TEXTURE)] = new Texture(game->gRenderer, SIMULATION_WIDTH
		+ UI_HORIZONTAL_MARGIN, UI_VERTICAL_MARGIN[static_cast<int>(TextureID::INFO_UI_TEXTURE)], game->gFont);

	SDL_Rect rect = { SIMULATION_WIDTH + UI_HORIZONTAL_MARGIN, UI_VERTICAL_MARGIN[static_cast<int>(TextureID::TOOLS_UI_TEXTURE)],
		SCREEN_WIDTH - SIMULATION_WIDTH - UI_HORIZONTAL_MARGIN * 2, 0 };

	game->gTexture[static_cast<int>(TextureID::TOOLS_UI_TEXTURE)] = new Texture(game->gRenderer, success, rect, game->gFont, "Pause Erase Reset ");
	rect.y = UI_VERTICAL_MARGIN[static_cast<int>(TextureID::MATERIALS_UI_TEXTURE)];
	game->gTexture[static_cast<int>(TextureID::MATERIALS_UI_TEXTURE)] = new Texture(game->gRenderer, success, rect, game->gFont, sim.getMaterialString());
	
	if (!success) game->gFailFlag++;
	game->ErrorHandler();

	SDL_Event e;
	SDL_Point cursor = {0, 0};
	SDL_Point lastCursor = cursor;
	Uint32 lastTick = 0;
	Uint32 lastRenderTime = 0;
	Simulation::Material material = Simulation::Material::SAND;
	Uint16 drawRadius = 15;
	bool drawRadChanged;
	bool lmbPressed;
	bool lmbHeld = false;
	bool paused = false;
	bool quit = false;

	// Main loop 
	while(!quit)
	{
		lmbPressed = false;
		drawRadChanged = false;

		// 유저 input
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				
				case SDLK_SPACE:
					paused = !paused;
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				cursor.x = e.button.x;
				cursor.y = e.button.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				lmbPressed = true;
				lmbHeld = true;
				break;

			case SDL_MOUSEBUTTONUP:
				lmbHeld = false;
				break;

			case SDL_MOUSEWHEEL:
				drawRadius = std::clamp<Sint16>(drawRadius + e.wheel.y, MIN_DRAW_RADIUS, MAX_DRAW_RADIUS);
				drawRadChanged = true;
				break;
			}
		}

		if(SDL_PointInRect(&cursor, &SIMULATION_RECT)) // 커서가 시뮬영역 안에 있을 경우
		{

			SDL_ShowCursor(SDL_DISABLE); 
			if(lmbHeld) 
			{ 
				sim.setCellLine(cursor, lastCursor, drawRadius, material); 
			}
		}
		else
		{
			SDL_ShowCursor(SDL_ENABLE);
			if(lmbPressed)
			{
				Uint8 clicked;
				if(game->gTexture[static_cast<int>(TextureID::TOOLS_UI_TEXTURE)]->isButtonClicked(&cursor, clicked))
				{
					switch(static_cast<ToolButton>(clicked))
					{
					case ToolButton::PAUSE:
						paused = !paused;
						break;

					case ToolButton::ERASE:
						material = Simulation::Material::EMPTY;
						break;

					case ToolButton::RESET:
						sim.reset();
						break;
					}
				}
				else if(game->gTexture[static_cast<int>(TextureID::MATERIALS_UI_TEXTURE)]->isButtonClicked(&cursor, clicked))
				{
					material = static_cast<Simulation::Material>(clicked + 1);
				}
			}
		}

		// 시뮬 돌리기
		if(!paused) sim.update();

		// 화면에 그리기
		Graphics::setRenderColor(game->gRenderer, &UI_PANEL_COLOR);
		SDL_RenderClear(game->gRenderer);

		if(SDL_GetTicks() % PERFORMANCE_POLL_RATE == 0 || drawRadChanged)
		{
			std::string text = std::to_string(std::min(1000 / std::max<Uint32>(lastRenderTime, 1), SCREEN_FPS)) + "fps      Brush radius: " + std::to_string(drawRadius);
			game->gTexture[static_cast<int>(TextureID::INFO_UI_TEXTURE)]->changeText(text);
		}

		game->gTexture[static_cast<int>(TextureID::SIMULATION_TEXTURE)]->changeTexture(sim.getDrawBuffer(), SIMULATION_WIDTH);
		for(int i = 0; i < static_cast<int>(TextureID::TOTAL_TEXTURES); ++i) 
		{ 
			game->gTexture[i]->renderTexture(); 
		}

		Graphics::setRenderColor(game->gRenderer, &CURSOR_COLOR); // = 흰색
		Graphics::drawCircle(game->gRenderer, &cursor, &SIMULATION_RECT, drawRadius);

		SDL_RenderPresent(game->gRenderer);

		lastCursor = cursor;

		//Calculate performance and limit program speed
		Uint32 renderTime = SDL_GetTicks() - lastTick;
		//std::cout << renderTime << "ms" << std::endl;

		SDL_Delay(TICKS_PER_FRAME - std::min(renderTime, TICKS_PER_FRAME));
		lastRenderTime = renderTime;
		lastTick = SDL_GetTicks();

		game->ErrorHandler();
	}

	delete game;
	return EXIT_SUCCESS;
}