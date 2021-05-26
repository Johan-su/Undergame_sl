#include <cmath>
#include "TargetingSystem.h"
#include "Entity.h"
#include "Game.h"
#include "PositionComponent.h"
#include "SizeComponent.h"
#include "vecf.h"

Entity TargetingSystem::nearest_player(float x, float y)
{
	Entity emin = 0xFFFFFFFF;
	for (auto e : m_entities)
	{
		if (emin == 0xFFFFFFFF)
		{
			emin = e;
			continue;
		}
		if (emin == e)
		{
			continue;
		}


		auto& pos = Game::coordinator->get_component<PositionComponent>(e);
		auto& size = Game::coordinator->get_component<SizeComponent>(e);

		auto& pos2 = Game::coordinator->get_component<PositionComponent>(emin);
		auto& size2 = Game::coordinator->get_component<SizeComponent>(emin);

		if (hypotf(pos2.pos.x + size2.size.x - x, pos2.pos.y + size2.size.y - y) < hypotf(pos.pos.x + size.size.x - x, pos.pos.y + size.size.y - y))
		{
			emin = e;
		}
	}
	return emin;
}

float TargetingSystem::nearest_player_distance(float x, float y)
{
	Entity e = nearest_player(x, y);

	auto& pos = Game::coordinator->get_component<PositionComponent>(e);
	auto& size = Game::coordinator->get_component<SizeComponent>(e);

	return hypotf(pos.pos.x + size.size.x / 2 - x, pos.pos.y + size.size.y / 2 - y);
}

Vec2f TargetingSystem::nearest_player_pos(float x, float y)
{
	Entity e = nearest_player(x, y);

	auto& pos = Game::coordinator->get_component<PositionComponent>(e);

	return pos.pos;
}

Vec2f TargetingSystem::nearest_player_size(float x, float y)
{
	Entity e = nearest_player(x, y);

	auto& size = Game::coordinator->get_component<SizeComponent>(e);

	return size.size;
}