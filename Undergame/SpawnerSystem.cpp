#include <cmath>
#include <random>
#include "EntityCreator.h"
#include "SpawnerSystem.h"
#include "Game.h"

void SpawnerSystem::init()
{
	srand((Game::seed));

	spawntimer = 0;
	iterations = 0;
	spawnrate = 360;
}

void SpawnerSystem::clean()
{

}

void SpawnerSystem::update() //TODO: add random generators
{
	if (spawntimer >= spawnrate)
	{
		for (auto e : m_entities)
		{
			float angle, x, y;




			auto& pos = Game::coordinator->get_component<PositionComponent>(e);
			auto& size = Game::coordinator->get_component<SizeComponent>(e);

			float px = pos.pos.x + size.size.x / 2;
			float py = pos.pos.y + size.size.y / 2;

			do
			{
				angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float)M_PI));

				x = (cos(angle) * TILE_SIZE * 10) + px;
				y = (sin(angle) * TILE_SIZE * 10) + py; //TODO: change to real values


			} while (!inside_map(x, y));



			EntityCreator::create_entity(ENTITY_TYPE_MOLE, x, y, 0);
		}
		spawntimer = 0;
		spawnrate = 120 * (int)(exp(-0.2 * iterations)) + 60;
		++iterations;
	}
	++spawntimer;
}



bool SpawnerSystem::inside_map(float x, float y)
{
	return !(x < TILE_SIZE || y < TILE_SIZE || x > TILE_SIZE * (MAP_SIZE - 1) || y > TILE_SIZE * (MAP_SIZE - 1));
}