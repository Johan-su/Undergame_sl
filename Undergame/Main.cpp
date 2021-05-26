#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <SDL.h>
#include "DebugMacros.h"
#include "EntityCreator.h"
#include "Texture.h"
#include "ECS.h"
#include "Game.h"

//#define NLOOP
//#define FPSLOOP






int main(int argc, char* argv[])
{
	auto before = std::chrono::high_resolution_clock::now();
#ifdef ECS_DEBUG
	std::cout << "ECS_DEBUG ENABLED" << std::endl;
#endif
	DP("PRINT_DEBUG ENABLED");
	if (Game::init("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0))
	{
		std::cout << "Game_initalization failed" << std::endl;

		return 1;
	}
	std::cout << Game::seed << std::endl;

	EntityCreator::init();

	//std::thread t1(Create_entities);

	Game::start_game_state();

	auto after = std::chrono::high_resolution_clock::now();

	auto dt = after - before;

	std::cout << "ns: " << dt.count() << " ms: " << dt.count() / 1000000.0f << " s: " << dt.count() / 1000000000.0f << std::endl;

	Game::loop();

	for (int i = 0; i < 100; ++i)
	{
		std::cout << "" << std::endl;
	}

	Game::clean();

	std::cout << Game::entityDeaths[ENTITY_TYPE_MOLE] << " points" << std::endl;

	std::cin.ignore();

	return 0;
}

