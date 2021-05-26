#include "DebugMacros.h"
#include "StaticCollisionSystem.h"
#include "ECS.h"


void StaticCollisionSystem::update() 
{
	for (auto e : m_entities)
	{
		auto& pos = Game::coordinator->get_component<PositionComponent>(e);
		auto& movement = Game::coordinator->get_component<MovementComponent>(e);
		auto& size = Game::coordinator->get_component<SizeComponent>(e);
		auto& collider = Game::coordinator->get_component<ColliderComponent>(e);

		collider.tile_id = 0xFFFFFFFF;

#ifdef ECS_DEBUG
		SDL_assert(pos.entity == e);
		SDL_assert(movement.entity == e);
		SDL_assert(size.entity == e);
		SDL_assert(collider.entity == e);
#endif

		int middlex = static_cast<int>(pos.pos.x + size.size.x);
		int middley = static_cast<int>(pos.pos.y + size.size.y);

		unsigned short id[9];

		id[4] = (middlex / TILE_SIZE) + MAP_SIZE * (middley / TILE_SIZE);

		id[0] = id[4] - MAP_SIZE - 1;
		id[1] = id[4] - MAP_SIZE;
		id[2] = id[4] - MAP_SIZE + 1;
		id[3] = id[4] - 1;
		id[5] = id[4] + 1;
		id[6] = id[4] + MAP_SIZE - 1;
		id[7] = id[4] + MAP_SIZE;
		id[8] = id[4] + MAP_SIZE + 1;

		for (Uint8 i = 0; i < 9; ++i) 
		{
			if (Game::tileEntities[id[i]] != 0)
			{


				int x1 = TILE_SIZE * (id[i] % MAP_SIZE); // lx
				int x2 = TILE_SIZE * (id[i] % MAP_SIZE) + TILE_SIZE; //rx 
				int y1 = TILE_SIZE * (id[i] / MAP_SIZE); // ly
				int y2 = TILE_SIZE * (id[i] / MAP_SIZE) + TILE_SIZE; // ry


				bool b1 = pos.pos.x < x2;

				bool b2 = pos.pos.x + size.size.x > x1;

				bool b3 = pos.pos.y < y2;

				bool b4 = pos.pos.y + size.size.y > y1;

				if (b1 && b2 && b3 && b4)
				{

					collider.tile_id = id[i];
					auto speed = movement.speed;
					if (movement.velocity.x && movement.velocity.y)
					{
						speed *= 0.707106781187f; // 1 / sqrt(2);
					}

					pos.pos.x += speed * (-movement.velocity.x);
					pos.pos.y += speed * (-movement.velocity.y);

				}
			}
		}
	}
}