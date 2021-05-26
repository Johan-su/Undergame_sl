#include <cmath>
#include <chrono>
#include "DebugMacros.h"
#include "AiSystem.h"
#include "HealthSystem.h"
#include "TargetingSystem.h"
#include "MovementComponent.h"
#include "DiggerComponent.h"
#include "PositionComponent.h"
#include "ECS.h"



void AiSystem::init(std::shared_ptr<TargetingSystem> ts, std::shared_ptr<HealthSystem> hs)
{
	m_ts = ts;
	m_hs = hs;
}

void AiSystem::clean()
{
}

void AiSystem::update()
{
	for (auto e : m_entities)
	{
		auto& ai = Game::coordinator->get_component<AiComponent>(e);
		auto& pos = Game::coordinator->get_component<PositionComponent>(e);
		auto& size = Game::coordinator->get_component<SizeComponent>(e);
		auto& digger = Game::coordinator->get_component<DiggerComponent>(e);
		auto& move = Game::coordinator->get_component<MovementComponent>(e);
		auto& collider = Game::coordinator->get_component<ColliderComponent>(e);



#ifdef ECS_DEBUG
		SDL_assert(ai.entity == e);
		SDL_assert(pos.entity == e);
		SDL_assert(size.entity == e);
		SDL_assert(digger.entity == e);
		SDL_assert(move.entity == e);
		SDL_assert(collider.entity == e);
#endif

		Entity ep = m_ts->nearest_player(pos.pos.x, pos.pos.y);

		float pdistance = 0.0f;
		Vec2f ppos = { 0.0f, 0.0f };
		Vec2f psize = { 0.0f, 0.0f };

		if (ep != 0xFFFFFFFF)
		{
			pdistance = m_ts->nearest_player_distance(pos.pos.x, pos.pos.y);
			ppos = m_ts->nearest_player_pos(pos.pos.x, pos.pos.y);
			psize = m_ts->nearest_player_size(pos.pos.x, pos.pos.y);
		}



		if (collider.tile_id != 0xFFFFFFFF)
		{

			digger.drillState = 1;

			move_to(collider.tile_id, pos, size, move, collider);

		}
		else
		{
			switch (ai.state)
			{
			case AI_STATE_RANDOM_WALKING:

				if (pdistance < ai.detectionRadius)
				{
					ai.state = AI_STATE_TRACK_LAST_KNOWN;
					break;
				}

				if (move_to(ai.id, pos, size, move, collider))
				{

					ai.id = random_walk(pos, size, move, digger);

				}
				break;

			case AI_STATE_TRACK_LAST_KNOWN:

				if (pdistance < ai.trackRadius)
				{
					ai.state = AI_STATE_TRACKING;
					ai.path_list.clear();
					break;
				}


				if (ai.path_list.size() > 0)
				{
					if (move_to(ai.path_list.back(), pos, size, move, collider))
					{
						ai.path_list.pop_back();
					}
				}
				else
				{
					if (pdistance > ai.detectionRadius)
					{
						ai.state = AI_STATE_RANDOM_WALKING;
						ai.path_list.clear();
						break;
					}

					ai.lastX = ppos.x + psize.x / 2;
					ai.lastY = ppos.y + psize.y / 2;



					dijkstra(ai.lastX, ai.lastY, pos, size, move, digger, ai.path_list);
				}
				break;

			case AI_STATE_TRACKING: 

				if (pdistance > ai.trackRadius)
				{
					ai.state = AI_STATE_TRACK_LAST_KNOWN;
					break;
				}



				ai_track(ppos, psize, pos, size, move);

				if (collider.other_entity != 0xFFFFFFFF)
				{
					auto& chealth = Game::coordinator->get_component<HealthComponent>(collider.other_entity);
					if (chealth.entity_type == ENTITY_TYPE_PLAYER)
					{
						m_hs->deal_damage(collider.other_entity, chealth, ai.damage);

						move.velocity.x *= -ai.trackRadius / 7.5f;
						move.velocity.y *= -ai.trackRadius / 7.5f;
					}
				}


				break;

			default:

				break;
			}

		}
	}
}

bool AiSystem::move_to(uint32_t gridID, PositionComponent& pos, const SizeComponent& size, MovementComponent& move, const ColliderComponent& collider)
{
	if (gridID < 0 || gridID >= MAP_SIZE * MAP_SIZE)
	{
		return 1;
	}
	auto type = Game::tileEntities[gridID];


	float gcx = (float)(TILE_SIZE * (gridID % MAP_SIZE) + TILE_SIZE / 2);
	float gcy = (float)(TILE_SIZE * (gridID / MAP_SIZE) + TILE_SIZE / 2);


	float ecx = pos.pos.x + size.size.x / 2;
	float ecy = pos.pos.y + size.size.y / 2;

	
	float targetangle = atanf((ecy - gcy) / (ecx - gcx));

	if (ecx - gcx >= 0)
	{
		targetangle -= 3.14159265359f; // pi
	}
	move.angle = fmod(targetangle + 6.28318530718f, 6.28318530718f); // 6.28318530718 == 2pi

	if (type == 16)
	{

		if (is_right(gcx, ecx)) // right
		{
			pos.pos.x += size.size.x / 8;
		}
		else
		{
			pos.pos.x -= size.size.x / 8;
		}
		if (is_down(gcy, ecy))
		{
			pos.pos.y += size.size.y / 8;

		}
		else
		{
			pos.pos.y -= size.size.y / 8;
		}
		SDL_assert(!isnan(pos.pos.x));
		SDL_assert(!isnan(pos.pos.y));
		return 0;
	}

	move.velocity.x = cos(-targetangle);
	move.velocity.y = sin(targetangle);




	if (abs(ecx - gcx) < 16.0f && abs(ecy - gcy) < 16.0f)
	{
		move.velocity.x = 0.0f;
		move.velocity.y = 0.0f;
		return 1;
	}
	return 0;
}

void AiSystem::Astar(float x, float y, const PositionComponent& pos, const SizeComponent& size, const MovementComponent& move, const DiggerComponent& digger, std::vector<uint32_t>& path)
{
	SDL_assert(path.size() == 0);

	nodesSearched = 1;


	uint16_t targetid = ((int)x / TILE_SIZE) + ((int)y / TILE_SIZE) * MAP_SIZE;
	uint16_t startid = ((int)(pos.pos.x + size.size.x / 2) / TILE_SIZE) + ((int)(pos.pos.y + size.size.y / 2) / TILE_SIZE) * MAP_SIZE;


	uint16_t id[5];

	id[0] = startid;

	for (int i = 0; i < MAP_SIZE * MAP_SIZE; ++i)
	{
		distance_to_grid[i] = 1E37f; // assumes infinite distance until searched
		before_grid[i] = 0xFFFFFFFF; // grid path initalized to unused value.
		searched_grid[i] = false; // initalize every searched_grid to false
	}

	distance_to_grid[startid] = 0.0f; // distance to start tile is 0;
	searched_grid[startid] = true; // start tile is searched instantly



	while (!searched_grid[targetid]) // loop until target grid is searched
	{
		uint32_t minid = 0;
		float mindistance = 1E38f;


		id[1] = id[0] + 1;
		id[2] = id[0] - MAP_SIZE;
		id[3] = id[0] - 1;
		id[4] = id[0] + MAP_SIZE;



		for (int i = 1; i < 5; ++i)
		{
			if (!(searched_grid[id[i]]))
			{
				auto time = dig_time(id[i], move, digger);

				auto g = before_grid[id[i]];

				if (g == 0xFFFFFFFF)
				{
					before_grid[id[i]] = id[0];
					g = before_grid[id[i]];
				}
				while (g != startid)
				{
					time += distance_to_grid[g];
					g = before_grid[g];
				}
				time += hypotf(x - pos.pos.x, y - pos.pos.y);

				if (time < distance_to_grid[id[i]])
				{
					distance_to_grid[id[i]] = time;
					before_grid[id[i]] = id[0];
				}
			}
		}

		for (int i = 0; i < distance_to_grid.size(); ++i)
		{
			if (distance_to_grid[i] < mindistance && !(searched_grid[i]))
			{
				minid = i;
				mindistance = distance_to_grid[i];
			}
		}

		searched_grid[minid] = true;
		id[0] = minid;
		++nodesSearched;
	}

	uint16_t t = targetid;


	while (t != startid)
	{
		path.push_back(t);
		t = before_grid[t];
	}
}

void AiSystem::dijkstra(float x, float y, const PositionComponent& pos, const SizeComponent& size, const MovementComponent& move, const DiggerComponent& digger, std::vector<uint32_t>& path)
{
	SDL_assert(path.size() == 0);

	nodesSearched = 1;


	uint16_t targetid = ((int)x / TILE_SIZE) + ((int)y / TILE_SIZE) * MAP_SIZE;
	uint16_t startid = ((int)(pos.pos.x + size.size.x / 2) / TILE_SIZE) + ((int)(pos.pos.y + size.size.y / 2) / TILE_SIZE) * MAP_SIZE;


	uint16_t id[5];

	id[0] = startid;

	for (int i = 0; i < MAP_SIZE * MAP_SIZE; ++i)
	{
		distance_to_grid[i] = 1E37f; // assumes infinite distance until searched
		before_grid[i] = 0xFFFFFFFF; // grid path initalized to unused value.
		searched_grid[i] = false; // initalize every searched_grid to false
	}

	distance_to_grid[startid] = 0.0f; // distance to start tile is 0;
	searched_grid[startid] = true; // start tile is searched instantly



	while (!searched_grid[targetid]) // loop until target grid is searched
	{
		uint32_t minid = 0;
		float mindistance = 1E38f;



		id[1] = id[0] + 1;
		id[2] = id[0] - MAP_SIZE;
		id[3] = id[0] - 1;
		id[4] = id[0] + MAP_SIZE;


		for (int i = 1; i < 5; ++i)
		{
			if (!(searched_grid[id[i]]))
			{
				auto time = dig_time(id[i], move, digger);

				auto g = before_grid[id[i]];

				if (g == 0xFFFFFFFF)
				{
					before_grid[id[i]] = id[0];
					g = before_grid[id[i]];
				}
				while (g != startid)
				{
					time += distance_to_grid[g];
					g = before_grid[g];
				}

				if (time < distance_to_grid[id[i]])
				{
					distance_to_grid[id[i]] = time;
					before_grid[id[i]] = id[0];
				}
			}

		}

		for (int i = 0; i < distance_to_grid.size(); ++i)
		{
			if (distance_to_grid[i] < mindistance && !(searched_grid[i]))
			{
				minid = i;
				mindistance = distance_to_grid[i];
			}
		}

		searched_grid[minid] = true;
		id[0] = minid;
		++nodesSearched;
	}


	uint16_t t = targetid;

	while (t != startid)
	{
		path.push_back(t);
		t = before_grid[t];
	}
}

uint16_t AiSystem::random_walk(const PositionComponent& pos, const SizeComponent& size, const MovementComponent& move, const DiggerComponent& digger)
{
	uint16_t id[5];

	id[0] = ((int)(pos.pos.x + size.size.x / 2) / TILE_SIZE) + ((int)(pos.pos.y + size.size.y / 2) / TILE_SIZE) * MAP_SIZE;
	id[1] = id[0] + 1;
	id[2] = id[0] - MAP_SIZE;
	id[3] = id[0] - 1;
	id[4] = id[0] + MAP_SIZE;

	float time = 0.0f;
	uint8_t r = 0;
	do
	{
		//DP("random_walk_loop");
		r = 1 + (4 * std::rand() / RAND_MAX);
		time = dig_time(id[r], move, digger);

	} while (time > 5000.0f);

	return id[r];
}

float AiSystem::dig_time(uint32_t gridID, const MovementComponent& move, const DiggerComponent& digger)
{
	float time = 0.0f;
	if (Game::tileEntities[gridID] != 0)
	{
		auto health = Game::coordinator->get_component<HealthComponent>(gridID);
		float digspeed =  2.0f * (1.0f + 0.4f * digger.drillLVL);

		time += health.health / digspeed;
	}
	return time + ((float)(TILE_SIZE) / move.speed);
}

void AiSystem::ai_track(const Vec2f& ppos, const Vec2f& psize, const PositionComponent& pos, const SizeComponent& size, MovementComponent& move)
{
	float px = ppos.x + psize.x / 2;
	float py = ppos.y + psize.y / 2;


	float ex = pos.pos.x + size.size.x / 2;
	float ey = pos.pos.y + size.size.y / 2;

	float targetangle = atanf((ey - py) / (0.0001f + ex - px));

	if (ex - px >= 0)
	{
		targetangle -= 3.14159265359f; // pi
	}

	move.angle = targetangle;

	SDL_assert(!isnan(move.velocity.x));
	SDL_assert(!isnan(move.velocity.y));

	move.velocity.x = cos(-targetangle);
	move.velocity.y = sin(targetangle);


	SDL_assert(!isnan(move.velocity.x));
	SDL_assert(!isnan(move.velocity.y));

}

bool AiSystem::is_right(float gcx, float ecx)
{
	return ecx > gcx;
}

bool AiSystem::is_down(float gcy, float ecy)
{
	return ecy > gcy;
}


