#include "Game.hpp"

#undef main

int main(int argc, char* argv[])
{
	Game* game = new Game();
	
	game->clArgs = new CLArgs(&result);
	
	return game->init(argc, argv);
}
