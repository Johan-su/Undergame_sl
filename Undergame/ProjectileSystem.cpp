#include "DebugMacros.h"
#include "EntityCreator.h"
#include "ProjectileSystem.h"
#include "ECS.h"

void ProjectileSystem::init(std::shared_ptr<HealthSystem> hs)
{

	m_hs = hs;

}

void ProjectileSystem::clean()
{
}

void ProjectileSystem::update()
{
	for (auto e : m_entities)
	{
		auto& collider = Game::coordinator->get_component<ColliderComponent>(e);
		auto& proj = Game::coordinator->get_component<ProjectileComponent>(e);
		auto& health = Game::coordinator->get_component<HealthComponent>(e);


#ifdef ECS_DEBUG
		SDL_assert(collider.entity == e);
		SDL_assert(proj.entity == e);
		SDL_assert(health.entity == e);
#endif
		if (collider.other_entity != 0xFFFFFFFF)
		{
			auto& other_entity_health = Game::coordinator->get_component<HealthComponent>(collider.other_entity);

#ifdef ECS_DEBUG
			SDL_assert(other_entity_health.entity == collider.other_entity);
#endif
			m_hs->deal_damage(collider.other_entity, other_entity_health, proj.damage);

			m_hs->deal_damage(e, health, proj.damage);
		}




		if (collider.tile_id != 0xFFFFFFFF)
		{
			auto& tile_health = Game::coordinator->get_component<HealthComponent>(collider.tile_id);
#ifdef ECS_DEBUG
			SDL_assert(collider.tile_id == tile_health.entity);
#endif
			m_hs->deal_damage(collider.tile_id, tile_health, proj.damage);

			m_hs->deal_damage(e, health, proj.damage);
		}


	}
}