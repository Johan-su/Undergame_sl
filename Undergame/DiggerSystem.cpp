#include <cmath>
#include <memory>
#include "ECS.h"



void DiggerSystem::init(std::shared_ptr<HealthSystem> hs)
{
	m_hs = hs;
}


void DiggerSystem::update()
{
	for (auto e : m_entities)
	{
		auto& render = Game::coordinator->get_component<RenderComponent>(e);
		auto& pos = Game::coordinator->get_component<PositionComponent>(e);
		auto& size = Game::coordinator->get_component<SizeComponent>(e);
		auto& move = Game::coordinator->get_component<MovementComponent>(e);
		auto& digger = Game::coordinator->get_component<DiggerComponent>(e);
		auto& collider = Game::coordinator->get_component<ColliderComponent>(e);

#ifdef ECS_DEBUG
		SDL_assert(render.entity == e);
		SDL_assert(pos.entity == e);
		SDL_assert(size.entity == e);
		SDL_assert(move.entity == e);
		SDL_assert(digger.entity == e);
		SDL_assert(collider.entity == e);
#endif


		if (digger.drillState == 1 && collider.tile_id != 0xFFFFFFFF && Game::tileEntities[collider.tile_id])
		{
			if (is_facing_tile(collider.tile_id, pos, size, move))
			{
				//render.src_rect = { 0, 0, 0, 0 }; //TODO: determine texture when digging
				auto& tilehealth = Game::coordinator->get_component<HealthComponent>(collider.tile_id);
				m_hs->deal_damage(collider.tile_id, tilehealth, 2.0f * (1.0f + 0.4f * digger.drillLVL));

				//deal_damage_tile(collider.tile_id, tilehealth, 2.0f * (1.0f + 0.4f * digger.drillLVL));
			}
			else
			{
				//render.src_rect = { 0, 0, 0, 0 }; //TODO: determine texture when not digging
			}
		}
	}
}

bool DiggerSystem::is_facing_tile(Entity tile, PositionComponent& facersPos, SizeComponent& facersSize, MovementComponent& facersMove)
{
	auto& tilePos = Game::coordinator->get_component<PositionComponent>(tile);

	float fx = facersPos.pos.x + facersSize.size.x / 2;
	float fy = facersPos.pos.y + facersSize.size.y / 2;
	float tx = tilePos.pos.x + TILE_SIZE / 2;
	float ty = tilePos.pos.y + TILE_SIZE / 2;

	float offset = 1.0471975512f; // pi / 3

	float angleToCenter = atanf((fy - ty) / (fx - tx));

	if (fx - tx >= 0)
	{
		angleToCenter -= 3.14159265359f; // pi
	}

	angleToCenter = fmod(angleToCenter + 6.28318530718f, 6.28318530718f); // 6.28318530718 == 2pi

	float oxp = fmod(angleToCenter + offset + 6.28318530718f, 6.28318530718f);
	float oxm = fmod(angleToCenter - offset + 6.28318530718f, 6.28318530718f);

	//std::cout << fx << " fx fy " << fy << " " << tx << " tx ty " << ty << " angle " << angleToCenter << std::endl;

	//std::cout << "angle " << angleToCenter << std::endl;
	if (oxp > facersMove.angle && oxm < facersMove.angle)
	{
	//	std::cout << "is facing " << angleToCenter << std::endl;
		return 1;
	}

	if (oxm > oxp)
	{
		if (oxp > facersMove.angle || oxm < facersMove.angle)
		{
		//	std::cout << "is facing " << angleToCenter << std::endl;
			return 1;
		}
	}
	//std::cout << "is not facing " << angleToCenter <<std::endl;
	return 0;
}