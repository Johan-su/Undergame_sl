#pragma once
#include <memory>
#include <SDL.h>
#include "ECS.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

class Game
{
public:


	static bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	static void clean();

	static void start_game_state();

	static void loop();

	static void update();

	static void events();

	static void render();

	static void ECS_init();

	static void components_init();

	static void systems_init();

public:
	static bool Running;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static Coordinator* coordinator;

	static std::array<Entity, MAP_SIZE* MAP_SIZE> tileEntities;

	static int offsetx, offsety;

	static uint32_t seed;

	static uint32_t entityDeaths[];

private:
	Game() = delete;

private:
};