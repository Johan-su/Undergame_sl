#include <iostream>
#include <chrono>
#include <time.h>
#include "Game.h"
#include "TileMapGenerator.h"
#include "Texture.h"
#include "ECS.h"
#include "EntityCreator.h"



//const uint32_t Game::seed = -1;
//const uint32_t Game::seed = 1606315639;

uint32_t Game::seed;

bool Game::Running;
SDL_Window* Game::window;
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
Coordinator* Game::coordinator;

int Game::offsetx;
int Game::offsety;

uint32_t Game::entityDeaths[20];


//std::vector<Entity>* Game::entities;
std::array<Entity, MAP_SIZE * MAP_SIZE> Game::tileEntities;


static std::shared_ptr<AiSystem> aiSystem;
static std::shared_ptr<CollisionSystem> collisionSystem;
static std::shared_ptr<DiggerSystem> diggerSystem;
static std::shared_ptr<HealthSystem> healthSystem;
static std::shared_ptr<MovementSystem> movementSystem;
static std::shared_ptr<PlayerSystem> playerSystem;
static std::shared_ptr<ProjectileSystem> projectileSystem;
static std::shared_ptr<RenderSystem> renderSystem;
static std::shared_ptr<ShooterSystem> shooterSystem;
static std::shared_ptr<SpawnerSystem> spawnerSystem;
static std::shared_ptr<StaticCollisionSystem> staticcollisionSystem;
static std::shared_ptr<TargetingSystem> targetingSystem;



bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	auto before = std::chrono::high_resolution_clock::now();
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Systems initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "window created" << std::endl;
			SDL_SetWindowInputFocus(window);

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer) {
				std::cout << "renderer created" << std::endl;
				Running = true;
				offsetx = 0;
				offsety = 0;

				auto after = std::chrono::high_resolution_clock::now();

				auto dt = after - before;

				Game::seed = (int)dt.count();

				Texture::init();
				ECS_init();
				return 0;
			}
		}
	}

	Running = false;
	return 1;

}
void Game::clean()
{
	coordinator->clean();

	projectileSystem->clean();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	delete coordinator;

	SDL_Quit();
}
//#define FPSLOOP
void Game::start_game_state()
{
	//auto before = std::chrono::high_resolution_clock::now(); // för tidsmätning av noise funktionerna

	//auto tm = TileMapGenerator::create_map_random();
	//auto tm = TileMapGenerator::create_map_value();
	auto tm = TileMapGenerator::create_map_perlin();
	//auto tm = TileMapGenerator::create_map_simplex();

	/*auto after = std::chrono::high_resolution_clock::now();

	auto dt = after - before;

	std::cout << "ns: " << dt.count() << " ms: " << dt.count() / 1000000.0f << " s: " << dt.count() / 1000000000.0f << std::endl;
	std::cin.ignore();*/ // för tidsmätning av noise funktionerna

	TileMapGenerator::entities_from_map(tm);
	float x = 0.0f, y = 0.0f;
	auto& health = Game::coordinator->get_component<HealthComponent>(2 * MAP_SIZE);

	for (unsigned int i = 0; i < MAP_SIZE * MAP_SIZE; ++i)
	{
		if (Game::tileEntities[i] == 0)
		{
			x = (float)(TILE_SIZE * (i % MAP_SIZE) + TILE_SIZE / 2);
			y = (float)(TILE_SIZE * (i / MAP_SIZE) + TILE_SIZE / 2);
			break;
		}
	}
	EntityCreator::create_entity(ENTITY_TYPE_PLAYER, x, y, 0);
	delete tm;
}

//#define FPSLOOP

void Game::loop()
{
#ifdef FPSLOOP
	uint32_t fps = 0;
	uint16_t count = 0;
	auto NS_PER_UPDATE = std::chrono::nanoseconds(16666666);
	auto previous = std::chrono::high_resolution_clock::now();
	std::chrono::nanoseconds lag(0);
	while (Game::Running) {
		auto current = std::chrono::high_resolution_clock::now();
		auto elapsed = current - previous;
		previous = current;
		lag += elapsed;


		while (lag >= NS_PER_UPDATE)
		{
			Game::events();
			Game::update();
			lag -= NS_PER_UPDATE;
			if (count >= 60)
			{
				std::cout << "FPS: " << fps << std::endl;
				count = 0;
				fps = 0;
			}
			++count;
		}
		Game::render();
		++fps;
	}
#else
	auto NS_PER_UPDATE = std::chrono::nanoseconds(16666666);
	auto previous = std::chrono::high_resolution_clock::now();
	std::chrono::nanoseconds lag(0);
	while (Game::Running) {
		auto current = std::chrono::high_resolution_clock::now();
		auto elapsed = current - previous;
		previous = current;
		lag += elapsed;


		while (lag >= NS_PER_UPDATE)
		{
			Game::events();
			Game::update();
			lag -= NS_PER_UPDATE;
		}
		Game::render();
	}
#endif
}

void Game::update()
{
	playerSystem->update();
	shooterSystem->update(); //creates entities
	movementSystem->update();
	staticcollisionSystem->update();
	collisionSystem->update();
	projectileSystem->update(); // pushes to delete
	diggerSystem->update(); // pushes to delete
	aiSystem->update(); // pushes to delete



	healthSystem->delete_entites(); // deletes

	spawnerSystem->update(); // creates

	//std::cout << "Points: " << entityDeaths[ENTITY_TYPE_MOLE] << std::endl;

	//offsetx++;
	//offsety+= 16;

}
void Game::events()
{
	while (SDL_PollEvent(&event))
	{
		InputSystem::update();
	}
}
void Game::render()
{
	SDL_RenderClear(renderer);

	//SDL_RenderCopy(renderer, Texture::get_texture(1), NULL, NULL);

	//staticrenderSystem->render_tiles(offsetx, offsety);
	StaticRenderSystem::render_tiles(offsetx, offsety);
	renderSystem->render();

	

	SDL_RenderPresent(renderer);
}


void Game::ECS_init()
{
	Game::coordinator = new Coordinator();
	Game::coordinator->init();
	Game::coordinator->test();
	components_init();
	systems_init();
	//Game::entities = new std::vector<Entity>(MAX_ENTITIES);
}

void Game::components_init()
{
	Game::coordinator->register_component<AiComponent>();
	Game::coordinator->register_component<ColliderComponent>();
	Game::coordinator->register_component<DiggerComponent>();
	Game::coordinator->register_component<HealthComponent>();
	Game::coordinator->register_component<MovementComponent>();
	Game::coordinator->register_component<PlayerComponent>();
	Game::coordinator->register_component<PositionComponent>();
	Game::coordinator->register_component<ProjectileComponent>();
	Game::coordinator->register_component<RenderComponent>();
	Game::coordinator->register_component<ShooterComponent>();
	Game::coordinator->register_component<SizeComponent>();
	Game::coordinator->register_component<TileComponent>();
}

void Game::systems_init()
{
	StaticRenderSystem::init();
	std::bitset<MAX_COMPONENTS> sig;


	aiSystem = Game::coordinator->register_system<AiSystem>();
	sig.set(Game::coordinator->get_signature_pos<AiComponent>());
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<ColliderComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	sig.set(Game::coordinator->get_signature_pos<DiggerComponent>());
	Game::coordinator->set_signature(aiSystem, sig);
	sig.reset();


	collisionSystem = Game::coordinator->register_system<CollisionSystem>();
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<ColliderComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	Game::coordinator->set_signature(collisionSystem, sig);
	sig.reset();


	diggerSystem = Game::coordinator->register_system<DiggerSystem>();
	sig.set(Game::coordinator->get_signature_pos<RenderComponent>());
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<DiggerComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	sig.set(Game::coordinator->get_signature_pos<ColliderComponent>());
	Game::coordinator->set_signature(diggerSystem, sig);
	sig.reset();


	healthSystem = Game::coordinator->register_system<HealthSystem>();
	sig.set(Game::coordinator->get_signature_pos<HealthComponent>());
	Game::coordinator->set_signature(healthSystem, sig);
	sig.reset();


	movementSystem = Game::coordinator->register_system<MovementSystem>();
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	Game::coordinator->set_signature(movementSystem, sig);
	sig.reset();


	playerSystem = Game::coordinator->register_system<PlayerSystem>();
	sig.set(Game::coordinator->get_signature_pos<PlayerComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<ShooterComponent>());
	sig.set(Game::coordinator->get_signature_pos<DiggerComponent>());
	Game::coordinator->set_signature(playerSystem, sig);
	sig.reset();


	projectileSystem = Game::coordinator->register_system<ProjectileSystem>();
	sig.set(Game::coordinator->get_signature_pos<ProjectileComponent>());
	sig.set(Game::coordinator->get_signature_pos<ColliderComponent>());
	sig.set(Game::coordinator->get_signature_pos<HealthComponent>());
	Game::coordinator->set_signature(projectileSystem, sig);
	sig.reset();


	renderSystem = Game::coordinator->register_system<RenderSystem>();
	sig.set(Game::coordinator->get_signature_pos<RenderComponent>());
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	Game::coordinator->set_signature(renderSystem, sig);
	sig.reset();


	shooterSystem = Game::coordinator->register_system<ShooterSystem>();
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	sig.set(Game::coordinator->get_signature_pos<ShooterComponent>());
	Game::coordinator->set_signature(shooterSystem, sig);
	sig.reset();


	spawnerSystem = Game::coordinator->register_system<SpawnerSystem>();
	sig.set(Game::coordinator->get_signature_pos<PlayerComponent>());
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	Game::coordinator->set_signature(spawnerSystem, sig);
	spawnerSystem->init();
	sig.reset();


	staticcollisionSystem = Game::coordinator->register_system<StaticCollisionSystem>();
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	sig.set(Game::coordinator->get_signature_pos<ColliderComponent>());
	sig.set(Game::coordinator->get_signature_pos<MovementComponent>());
	Game::coordinator->set_signature(staticcollisionSystem, sig);
	sig.reset();


	targetingSystem = Game::coordinator->register_system<TargetingSystem>();
	sig.set(Game::coordinator->get_signature_pos<PlayerComponent>());
	sig.set(Game::coordinator->get_signature_pos<PositionComponent>());
	sig.set(Game::coordinator->get_signature_pos<SizeComponent>());
	Game::coordinator->set_signature(targetingSystem, sig);
	sig.reset();


	projectileSystem->init(healthSystem);
	diggerSystem->init(healthSystem);
	aiSystem->init(targetingSystem, healthSystem);
}
