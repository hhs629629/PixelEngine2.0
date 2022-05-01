#include "stdafx.h"

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;
int SHAPE_SIZE = 64; // x by x size of the texture

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_dInit has failed. Error:" << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error:" << SDL_GetError() << std::endl;

	RenderWindow window("PixelEngine2.0 v0.1", WIN_WIDTH, WIN_HEIGHT);

	SDL_Texture* testTexture = window.LoadTexture("res/textures/test.png");

	std::vector<Entity> entities = {
		Entity(Vec2f(0,0),testTexture,SHAPE_SIZE),
		Entity(Vec2f(22,22),testTexture,SHAPE_SIZE),
		Entity(Vec2f(44,44),testTexture,SHAPE_SIZE),
		Entity(Vec2f(55,55),testTexture,SHAPE_SIZE),
		Entity(Vec2f(111,111),testTexture,SHAPE_SIZE)};
	
	{
		Entity lastBox(Vec2f(333, 333), testTexture, SHAPE_SIZE);
		entities.push_back(lastBox);
	}

	bool gameRunning = true;
	SDL_Event event;

	// limit updates per time
	// linear interpolation -- guess where the car will be in between updates
	// -> smooth out movement
	// must sync rendering speed and moniter fps
	const float timeStep = 0.01f;	// deltaTime: cars move on each timeStep. here is 100ps
	float accumulator = 0.0f;		// if full then update the game
	float currentTime = utils::hireTimeInSeconds();

	while (gameRunning)
	{
		// frameTime : amount of time to complete one frame
		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;

		accumulator += frameTime;
		while (accumulator >= timeStep)
		{
			// get our controls and events
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}
			accumulator -= timeStep;
		}
		const float alpha = accumulator / timeStep; // percentage of accumulator

		window.Clear();
		for (Entity& e : entities) // renders each element of the vector
		{
			window.Render(e);
		}
		std::cout << utils::hireTimeInSeconds() << std::endl;
		window.Display();
	}

	window.CleanUp();
	SDL_Quit();
	return 0;
}
