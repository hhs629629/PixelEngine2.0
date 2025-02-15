#include "stdafx.h"
#include <algorithm>

int main(int argc, char **argv)
{
	Game* game = new Game();
	game->ErrorHandler();

	Simulation sim(SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_FORMAT);

	bool success = true;

	game->gTexture = new Texture(game->gRenderer, success, SIMULATION_RECT);

	if (!success) game->gFailFlag++;
	game->ErrorHandler();

	SDL_Event eventQueue;
	SDL_Point cursor = {0, 0};
	SDL_Point lastCursor = cursor;
	Uint32 lastTick = 0;
	Uint32 lastRenderTime = 0;
	Simulation::Material materialStack = Simulation::Material::EMPTY; 
	Simulation::Material material = Simulation::Material::SAND; // defualt material
	Uint16 drawRadius = 15;										// default brush radius
	bool drawRadiusChanged;
	bool lmbPressed;
	bool rmbPressed;
	bool lmbHeld = false;
	bool rmbHeld = false;
	bool paused = false;
	bool quit = false;

#pragma region Imgui setup
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(game->gWindow, game->gRenderer);
	ImGui_ImplSDLRenderer_Init(game->gRenderer);

	// Our state
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#pragma endregion

	// Main loop 
	while(!quit)
	{
		// ====User input event processing====
		while(SDL_PollEvent(&eventQueue))
		{
			ImGui_ImplSDL2_ProcessEvent(&eventQueue);
			switch(eventQueue.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYDOWN:
				switch(eventQueue.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				
				}
				break;

			case SDL_MOUSEMOTION:
				cursor.x = eventQueue.button.x;
				cursor.y = eventQueue.button.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				switch (eventQueue.button.button)
				{
				case SDL_BUTTON_LEFT: // left click
					lmbPressed = true;
					lmbHeld = true;
					break;

				case SDL_BUTTON_RIGHT: // right click
					rmbPressed = true;
					rmbHeld = true;
					break;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				lmbHeld = false;
				rmbHeld = false;
				break;

			case SDL_MOUSEWHEEL:
				drawRadius = std::clamp<Sint16>(drawRadius + eventQueue.wheel.y, MIN_DRAW_RADIUS, MAX_DRAW_RADIUS);
				drawRadiusChanged = true;
				break;
			}
		}
		SDL_ShowCursor(SDL_ENABLE); 
		if(lmbHeld) 
		{ 
			sim.setCellLine(cursor, lastCursor, drawRadius, material); 
		}
		if (rmbHeld)
		{
			materialStack = material;
			material = Simulation::Material::EMPTY;
			sim.setCellLine(cursor, lastCursor, drawRadius, material);
			material = materialStack; // 원상복귀
		}


		// ====Start the Dear ImGui frame====
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// main gui
		{
			static float f = 0.0f;

			ImGui::Begin("Materials");      

			ImGui::Text("Brush radius : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(drawRadius).c_str());

			ImGui::Checkbox("Help", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Pause"))                         
				paused = !paused;
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				sim.reset();

			// select material button
			if (ImGui::Button("Sand"))
				material = Simulation::Material::SAND;
			ImGui::SameLine();
			if (ImGui::Button("Water"))
				material = Simulation::Material::WATER;
			ImGui::SameLine();
			if (ImGui::Button("Fire"))
				material = Simulation::Material::FIRE;
			ImGui::SameLine();
			if (ImGui::Button("Lava"))
				material = Simulation::Material::LAVA;
			ImGui::SameLine();
			if (ImGui::Button("Rock"))
				material = Simulation::Material::ROCK;
			ImGui::SameLine();
			if (ImGui::Button("Steam"))
				material = Simulation::Material::STEAM;
			ImGui::SameLine();
			if (ImGui::Button("Wood"))
				material = Simulation::Material::WOOD;

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin("Help", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Erase : Right click");

			if (ImGui::Button("Close"))
				show_another_window = false;
			ImGui::End();
		}



		// ====Run simulation====
		if(!paused) sim.update();



		// ====Rendering====
		// draw background
		SDL_SetRenderDrawColor(game->gRenderer, 0, 0, 0, 1);
		SDL_RenderClear(game->gRenderer);

		// render cells
		game->gTexture->changeTexture(sim.getDrawBuffer(), SCREEN_WIDTH);
		game->gTexture->renderTexture(); 

		// draw hollow circle cursor
		SDL_SetRenderDrawColor(game->gRenderer, 255, 255, 255, 150);
		Graphics::drawCircle(game->gRenderer, &cursor, &SIMULATION_RECT, drawRadius);

		// draw ImGui window
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(game->gRenderer);


		// ================
		lastCursor = cursor;

		lmbPressed = false;
		rmbPressed = false;

		drawRadiusChanged = false;

		Uint32 renderTime = SDL_GetTicks() - lastTick;
		SDL_Delay(TICKS_PER_FRAME - std::min(renderTime, TICKS_PER_FRAME));
		lastRenderTime = renderTime;
		lastTick = SDL_GetTicks();

		game->ErrorHandler();
	}

	delete game;
	return EXIT_SUCCESS;
}